using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Text.RegularExpressions;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;

namespace EditorTool
{
    public partial class Model_Importer : Form
    {
        public Model_Importer()
        {
            InitializeComponent();
        }

        /* Browse to model file */
        private void browseToModel_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "OBJ Model (OBJ)|*.OBJ";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                modelPath.Text = filePicker.FileName;
                if (assetName.Text == "")
                {
                    assetName.Text = Path.GetFileNameWithoutExtension(modelPath.Text);
                }
            }
        }
        
        /* Import model and textures */
        private void importModel_Click(object sender, EventArgs e)
        {
            string import_directory = "Models/" + assetName.Text + "/";
            
            if (Directory.Exists(import_directory) || modelPath.Text == "" || assetName.Text == "" || !Regex.IsMatch(assetName.Text, "^[_a-zA-Z0-9\x20]+$"))
            {
                if (modelPath.Text == "" || assetName.Text == "")
                {
                    MessageBox.Show("Please fill out all required inputs.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else if (!Regex.IsMatch(assetName.Text, "^[_a-zA-Z0-9\x20]+$"))
                {
                    MessageBox.Show("Your asset name cannot contain any special characters.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                MessageBox.Show("Couldn't import model, a model with the same name already exists.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                string pathWithoutExtension = modelPath.Text.Substring(0, modelPath.Text.Length - Path.GetExtension(modelPath.Text).Length);
                string importedMTL = "";
                string original_mtl_path = "";

                bool didHaveUglyPath = false;
                bool didFindMTL = false;
                
                int material_count = 0;
                List<string> referenced_materials = new List<string>();
                int found_count = 0;
                int lost_count = 0;

                //Copy model into our output directory - basically our working directory
                Directory.CreateDirectory(import_directory);
                File.Copy(pathWithoutExtension + ".obj", import_directory + Path.GetFileName(modelPath.Text));

                //Trawl through our model to find the original MTL name
                //Fix the name for the crappy DXTK converter if necessary
                string[] obj_file = File.ReadAllLines(import_directory + Path.GetFileName(pathWithoutExtension + ".obj"));
                string mtl_suggestedname = "";
                int obj_index = 0;
                foreach (string line in obj_file)
                {
                    if (line.Contains("mtllib"))
                    {
                        mtl_suggestedname = line.Substring(7);
                        if (mtl_suggestedname.Contains(" "))
                        {
                            obj_file[obj_index] = line.Substring(0, 7) + line.Substring(7).Replace(' ', '_'); //SHODDY TOOLKIT!
                        }
                        break;
                    }
                    obj_index++;
                }
                File.WriteAllLines(import_directory + Path.GetFileName(pathWithoutExtension + ".obj"), obj_file);
                
                //Find and fix MTL if one is required
                if (mtl_suggestedname != "")
                {
                    //Check for UGLY paths to original MTL - I'm not writing a handler for this just yet unless we need it
                    if (mtl_suggestedname.Split('/').Length > 1 || mtl_suggestedname.Split('\\').Length > 1)
                    {
                        didHaveUglyPath = true;
                        //We can still succeed from here - but if we fail, this is most likely why.
                        //Really a fix should be implemented, but I'll wait and see if it becomes an issue first.
                        //Downloading models from the internet will be the cause of this occuring.
                    }

                    //If original MTL exists, copy it over
                    if (File.Exists(Path.GetDirectoryName(modelPath.Text) + @"\" + mtl_suggestedname))
                    {
                        //Must replace spaces here b/c TK is TRASH - you'll see this continuing further down... EEK!
                        importedMTL = mtl_suggestedname.Replace(' ', '_');
                        original_mtl_path = Path.GetDirectoryName(modelPath.Text) + @"\" + mtl_suggestedname;
                        didFindMTL = true;
                        File.Copy(original_mtl_path, import_directory + importedMTL);
                    }

                    //If we couldn't find our original MTL, attempt to match with our OBJ name
                    if (!didFindMTL && File.Exists(pathWithoutExtension + ".mtl"))
                    {
                        importedMTL = Path.GetFileName(pathWithoutExtension.Replace(' ', '_') + ".mtl");
                        original_mtl_path = pathWithoutExtension + ".mtl";
                        didFindMTL = true;
                        File.Copy(original_mtl_path, import_directory + importedMTL); 

                        //Second fix for crappy DXTK converter - rewrite OBJ to have the correct MTL path
                        obj_index = 0;
                        foreach (string line in obj_file)
                        {
                            if (line.Contains("mtllib"))
                            {
                                obj_file[obj_index] = "mtllib " + importedMTL;
                                break;
                            }
                            obj_index++;
                        }
                        File.WriteAllLines(import_directory + Path.GetFileName(pathWithoutExtension + ".obj"), obj_file);
                    }

                    //Still can't find our MTL file - ask the user to locate it
                    if (!didFindMTL)
                    {
                        MessageBox.Show("Could not find the MTL file for this model.\nPlease locate it.", "Could not auto-locate MTL!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        OpenFileDialog filePicker = new OpenFileDialog();
                        filePicker.Filter = "Model Material Info (MTL)|*.MTL";
                        if (filePicker.ShowDialog() == DialogResult.OK)
                        {
                            importedMTL = Path.GetFileName(filePicker.FileName).Replace(' ', '_');
                            original_mtl_path = filePicker.FileName;
                            didFindMTL = true;
                            File.Copy(original_mtl_path, import_directory + importedMTL);

                            //Re-do second fix for crappy DXTK converter now we have our MTL - rewrite OBJ to have the correct MTL path
                            obj_index = 0;
                            foreach (string line in obj_file)
                            {
                                if (line.Contains("mtllib"))
                                {
                                    obj_file[obj_index] = "mtllib " + importedMTL;
                                    break;
                                }
                                obj_index++;
                            }
                            File.WriteAllLines(import_directory + Path.GetFileName(pathWithoutExtension + ".obj"), obj_file);
                        }
                    }

                    if (didFindMTL)
                    {
                        //THIRD fix for crappy DXTK converter - rewrite the MTL with proper paths
                        int mtl_index = 0;
                        string[] mtl_file = File.ReadAllLines(import_directory + importedMTL);
                        referenced_materials.Clear();
                        foreach (string line in mtl_file)
                        {
                            if (line.Contains("map"))
                            {
                                string[] map_split = new string[2];
                                map_split = line.Split(new[] {' '}, 2);
                                referenced_materials.Add(map_split[1]);
                                map_split[1] = Path.GetFileName(map_split[1]);
                                mtl_file[mtl_index] = map_split[0] + " " + map_split[1].Replace(' ', '_'); //Will apply this to copied materials next
                                material_count++;
                            }
                            mtl_index++;
                        }
                        File.WriteAllLines(import_directory + importedMTL, mtl_file);

                        //Go through all referenced materials in our MTL and try to import them
                        foreach (string material_file in referenced_materials)
                        {
                            string root_path_to_material_file = "";
                            string local_path_to_material_file = Path.GetDirectoryName(original_mtl_path) + "\\" + Path.GetFileName(material_file);
                            if (Path.IsPathRooted(material_file))
                            {
                                if (File.Exists(material_file))
                                {
                                    //Material exists and we already have the root path for it
                                    root_path_to_material_file = material_file;
                                }
                                else
                                {
                                    if (File.Exists(local_path_to_material_file))
                                    {
                                        //Material doesn't exist at its original path, but is in our folder, grab the new root path for it
                                        root_path_to_material_file = local_path_to_material_file;
                                    }
                                }
                            }
                            else
                            {
                                if (File.Exists(local_path_to_material_file))
                                {
                                    //Path suggests material resides in our folder, and it does, grab the root path for it
                                    root_path_to_material_file = local_path_to_material_file;
                                }
                            }

                            if (root_path_to_material_file == "")
                            {
                                //Couldn't find file
                                lost_count++;
                                continue;
                            }
                            string output_file = import_directory + Path.GetFileName(material_file).Replace(' ', '_');
                            if (File.Exists(output_file)) {
                                continue;
                            }

                            found_count++;
                            File.Copy(root_path_to_material_file, output_file);
                        }
                    }
                }
                else
                {
                    //We didn't actually find the MTL, because this model doesn't have one
                    didFindMTL = true;
                }

                //Double check we found the MTL in our popup... if not, don't continue
                if (didFindMTL)
                {
                    //Run the model converter to swap our OBJ into an SDKMESH
                    ProcessStartInfo meshConverter = new ProcessStartInfo();
                    meshConverter.WorkingDirectory = import_directory;
                    meshConverter.FileName = "Models/meshconvert.exe";
                    meshConverter.Arguments = "\"" + Path.GetFileName(modelPath.Text) + "\" -sdkmesh -nodds -y";
                    meshConverter.UseShellExecute = false;
                    meshConverter.RedirectStandardOutput = true;
                    Process converterProcess = Process.Start(meshConverter);
                    StreamReader reader = converterProcess.StandardOutput;
                    converterProcess.WaitForExit();

                    //Capture write info from converter
                    string output = reader.ReadToEnd();
                    string[] outputArray = output.Split('\n');
                    string writeInfo = "";
                    foreach (string line in outputArray)
                    {
                        if (line.Contains("written"))
                        {
                            writeInfo = line.Substring(0,line.Length - 2);
                            break;
                        }
                    }

                    if (File.Exists(import_directory + Path.GetFileNameWithoutExtension(modelPath.Text) + ".sdkmesh"))
                    {
                        string final_asset_path = import_directory + assetName.Text + ".sdkmesh";

                        //Output face vertex data for generating our collmap
                        string[] final_obj_file = File.ReadAllLines(import_directory + Path.GetFileName(modelPath.Text));
                        List<string> model_vertices = new List<string>();
                        List<List<int>> model_face_vert_index = new List<List<int>>();
                        List<string> final_collmap_data = new List<string>();
                        bool was_all_triangles = true;
                        //Grab all vertices and face vert indexes from our OBJ
                        foreach (string line in final_obj_file)
                        {
                            //Vertices
                            if (line.Length > 2 && line.Substring(0, 2) == "v ")
                            {
                                string[] vert_array = line.Substring(2).Split(' ');
                                string this_final_vert_data = "(";
                                foreach (string vert in vert_array)
                                {
                                    this_final_vert_data += vert + ", ";
                                }
                                this_final_vert_data = this_final_vert_data.Substring(0, this_final_vert_data.Length - 2); //crash potential here if OBJ is messy
                                this_final_vert_data += "), "; //", " will have to be trimmed for file writing
                                model_vertices.Add(this_final_vert_data);
                            }
                            //Faces
                            if (line.Length > 2 && line.Substring(0, 2) == "f ")
                            {
                                string[] face_array = line.Substring(2).Split(' ');
                                List<int> face_array_parsed = new List<int>();
                                foreach (string face in face_array)
                                {
                                    if (face != "")
                                    {
                                        string this_face = face;
                                        if (face.Contains('/'))
                                        {
                                            this_face = face.Split('/')[0]; //only want positional vertex data, not normals or other crap
                                        }
                                        face_array_parsed.Add(Convert.ToInt32(this_face)); //we now know this is an int, so can convert confidently
                                    }
                                }
                                model_face_vert_index.Add(face_array_parsed);
                            }
                        }
                        //Build up total model verts for collmap reader from parsed data
                        foreach (List<int> vert_index_list in model_face_vert_index)
                        {
                            string this_face_complete = "";
                            int this_face_vert_count = 0;
                            foreach (int vert_index in vert_index_list)
                            {
                                this_face_complete += model_vertices.ElementAt(vert_index-1); //crash potential here if OBJ is messy
                                this_face_vert_count++;
                                if (this_face_vert_count > 3)
                                {
                                    was_all_triangles = false;
                                }
                            }
                            this_face_complete = this_face_complete.Substring(0, this_face_complete.Length - 2);

                            final_collmap_data.Add(this_face_complete);
                        }
                        File.WriteAllLines(import_directory + Path.GetFileNameWithoutExtension(final_asset_path) + ".collmap", final_collmap_data);

                        //Conversion complete - delete the OBJ and MTL
                        File.Delete(import_directory + Path.GetFileName(modelPath.Text));
                        if (importedMTL != "")
                        {
                            File.Delete(import_directory + importedMTL);
                        }

                        //Create JSON data
                        JToken asset_json = JToken.Parse("{\"asset_name\": \"" + assetName.Text + "\", \"asset_type\": \"Models\", \"visible\": true, \"start_x\": 0, \"start_y\": 0, \"start_z\": 0, \"modelscale\": 1.0, \"rot_x\": 0, \"rot_y\": 0, \"rot_z\": 0}");
                        File.WriteAllText(final_asset_path.Substring(0, final_asset_path.Length - 7) + "json", asset_json.ToString(Formatting.Indented));

                        //Move new SDKMESH to the correct requested filename
                        File.Move(import_directory + Path.GetFileNameWithoutExtension(modelPath.Text) + ".sdkmesh", final_asset_path);
                        string final_confirmation = "Model imported with" + writeInfo + ".";
                        if (found_count > 0)
                        {
                            final_confirmation += "\nFound " + found_count + " material(s).";
                        }
                        if (lost_count > 0)
                        {
                            final_confirmation += "\nWARNING: Failed to find " + lost_count + " materials.";
                        }
                        if (!was_all_triangles)
                        {
                            final_confirmation += "\nWARNING: Not all faces are triangulated, model may experience collision issues.";
                        }
                        MessageBox.Show(final_confirmation, "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        //Conversion must've failed - GAH!
                        if (didHaveUglyPath)
                        {
                            //Failed because I haven't implemented a handler for ugly MTL paths
                            MessageBox.Show("Model import failed because the original MTL path is UGLY!\nDrop this message to Matt.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        else
                        {
                            //Failed! Show full output from DXTK converter if requested.
                            DialogResult showErrorInfo = MessageBox.Show("Model import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                            if (showErrorInfo == DialogResult.Yes)
                            {
                                MessageBox.Show(output, "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                        }
                    }

                    this.Close();
                }
                else
                {
                    //Couldn't locate MTL - FAIL!
                    File.Delete(import_directory + Path.GetFileName(modelPath.Text));
                    Directory.Delete(import_directory);
                    MessageBox.Show("Import failed because the tool was unable to locate a required MTL file for this model.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }


        private void addTexture_Click(object sender, EventArgs e)
        {
            //depreciated
        }
        private void removeTexture_Click(object sender, EventArgs e)
        {
            //depreciated
        }
    }
}
