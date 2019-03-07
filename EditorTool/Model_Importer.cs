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
using System.Collections;

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
            if (filePicker.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            //Update form with file path
            modelPath.Text = filePicker.FileName;
            if (assetName.Text == "")
            {
                assetName.Text = Path.GetFileNameWithoutExtension(modelPath.Text);
            }

            //Find all materials in model to group collmap by - sort alphabetically
            string[] obj_file = File.ReadAllLines(filePicker.FileName);
            ArrayList material_array = new ArrayList();
            foreach (string line in obj_file)
            {
                if (line.Length > 7 && line.Substring(0, 7) == "usemtl ")
                {
                    material_array.Add(line.Substring(7));
                }
            }
            material_array.Sort();
            itemMaterialCategories.Items.Clear();
            foreach (string material in material_array)
            {
                itemMaterialCategories.Items.Add(material);
            }
        }
        
        /* Import model and textures */
        private void importModel_Click(object sender, EventArgs e)
        {
            string import_directory = "DATA/MODELS/" + assetName.Text.ToUpper() + "/";
            int mat_check_count = 0;
            for (int i = 0; i < itemMaterialCategories.Items.Count; i++)
            {
                if (itemMaterialCategories.GetItemChecked(i))
                {
                    mat_check_count++;
                }
            }

            if (Directory.Exists(import_directory) || modelPath.Text == "" || assetName.Text == "" || !Regex.IsMatch(assetName.Text, "^[_a-zA-Z0-9\x20]+$") || (mat_check_count == 0 && shouldGenerateCollmap.Checked) || (assetName.Text.Length > 5 && assetName.Text.Substring(assetName.Text.Length - 5) == "DEBUG"))
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
                else if (itemMaterialCategories.SelectedItems.Count == 0 && shouldGenerateCollmap.Checked)
                {
                    MessageBox.Show("You must select at least one model section to generate the collision map from.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else if (assetName.Text.Length > 5 && assetName.Text.Substring(assetName.Text.Length - 5) == "DEBUG")
                {
                    MessageBox.Show("Your asset name conflicts with a system name, please choose another.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                MessageBox.Show("Couldn't import model, a model with the same name already exists.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                string pathWithoutExtension = modelPath.Text.Substring(0, modelPath.Text.Length - Path.GetExtension(modelPath.Text).Length);
                string importedMTL = "";
                string original_mtl_path = "";

                bool didHaveUglyPath = false; //Local paths to MTL files are somewhat unhandled, so worth keeping track of that.
                bool didFindMTL = false;
                bool vertexOrderIsBusted = false; //Some model files will invert the vertex index, which is totally unhandled and will cause an error.
                
                int material_count = 0;
                List<string> referenced_materials = new List<string>();
                int found_count = 0;
                int lost_count = 0;

                string json_extras = ", \"has_box_collider\": false";

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
                    string conv_args = "\"" + Path.GetFileName(modelPath.Text) + "\" -sdkmesh -nodds -y";
                    if (shouldFlipUV.Checked)
                    {
                        conv_args += " -flipv";
                    }
                    ProcessStartInfo meshConverter = new ProcessStartInfo();
                    meshConverter.WorkingDirectory = import_directory;
                    meshConverter.FileName = "DATA/MODELS/meshconvert.exe";
                    meshConverter.Arguments = conv_args;
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

                    //See if our vertex order is busted (unhandled issue, but helpful error message)
                    string[] final_obj_file = File.ReadAllLines(import_directory + Path.GetFileName(modelPath.Text));
                    foreach (string line in final_obj_file)
                    {
                        if(line.Length > 2 && line.Substring(0, 2) == "f ") {
                            if (line.Substring(2, 1) == "-")
                            {
                                vertexOrderIsBusted = true;
                                break;
                            }
                        }
                    }

                    if (File.Exists(import_directory + Path.GetFileNameWithoutExtension(modelPath.Text) + ".sdkmesh"))
                    {
                        string final_asset_path = import_directory + assetName.Text + ".sdkmesh";
                        final_asset_path = final_asset_path.ToUpper();

                        bool model_supports_collision = true;
                        int collision_fix_count = 0;
                        if (shouldGenerateCollmap.Checked || shouldCreateBoxCollider.Checked)
                        {
                            //Output face vertex data for generating our collmap
                            List<List<double>> model_vertices_raw = new List<List<double>>();
                            List<List<int>> model_face_vert_index = new List<List<int>>();
                            List<string> final_collmap_data = new List<string>();
                            bool should_listen_for_faces = false;
                            //Grab all vertices and face vert indexes from our OBJ
                            foreach (string line in final_obj_file)
                            {
                                //Vertices
                                if (line.Length > 2 && line.Substring(0, 2) == "v ")
                                {
                                    string[] vert_array = line.Substring(2).Split(' ');
                                    List<double> this_vertex = new List<double>();
                                    foreach (string vert in vert_array)
                                    {
                                        this_vertex.Add(Convert.ToDouble(vert));
                                    }
                                    model_vertices_raw.Add(this_vertex);
                                    continue;
                                }
                                //Check to see if we should start listening for faces
                                //This may seem odd, but as the OBJ is always written/read in a logical order, this should work 100% of the time
                                if (line.Length > 7 && line.Substring(0, 7) == "usemtl ")
                                {
                                    should_listen_for_faces = false;
                                    for (int i = 0; i < itemMaterialCategories.Items.Count; i++)
                                    {
                                        if (itemMaterialCategories.GetItemChecked(i) && (line.Substring(7) == itemMaterialCategories.Items[i].ToString()))
                                        {
                                            should_listen_for_faces = true;
                                            break;
                                        }
                                    }
                                    continue;
                                }
                                //Faces
                                if (line.Length > 2 && line.Substring(0, 2) == "f " && should_listen_for_faces)
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
                                    continue;
                                }
                            }

                            //Calculate center of mesh for our box collider script
                            double[] biggest_vert = { 0, 0, 0 };
                            double[] smallest_vert = { 0, 0, 0 };
                            if (shouldCreateBoxCollider.Checked)
                            {
                                foreach (List<double> vertex in model_vertices_raw)
                                {
                                    for (int i = 0; i < 3; i++)
                                    {
                                        if (biggest_vert[i] < vertex.ElementAt(i))
                                        {
                                            biggest_vert[i] = vertex.ElementAt(i);
                                        }
                                        if (smallest_vert[i] > vertex.ElementAt(i))
                                        {
                                            smallest_vert[i] = vertex.ElementAt(i);
                                        }
                                    }
                                }
                            }

                            //Build up total model verts for collmap reader from parsed data
                            if (shouldGenerateCollmap.Checked)
                            {
                                foreach (List<int> vert_index_list in model_face_vert_index)
                                {
                                    string this_face_complete = "";
                                    int this_face_vert_count = 0;
                                    List<double> vert_x_list = new List<double>();
                                    List<double> vert_y_list = new List<double>();
                                    List<double> vert_z_list = new List<double>();
                                    foreach (int vert_index in vert_index_list)
                                    {
                                        vert_x_list.Add(model_vertices_raw.ElementAt(vert_index - 1).ElementAt(0));
                                        vert_y_list.Add(model_vertices_raw.ElementAt(vert_index - 1).ElementAt(1));
                                        vert_z_list.Add(model_vertices_raw.ElementAt(vert_index - 1).ElementAt(2));

                                        this_face_vert_count++;
                                    }
                                    if (this_face_vert_count != 3)
                                    {
                                        //Model must be triangulated to generate collision map
                                        model_supports_collision = false;
                                        break;
                                    }

                                    //Fix conflicts if any are present
                                    if (vert_x_list.ElementAt(0) == vert_x_list.ElementAt(1) && vert_y_list.ElementAt(0) == vert_y_list.ElementAt(1) && vert_z_list.ElementAt(0) == vert_z_list.ElementAt(1))
                                    {
                                        vert_x_list[0] = vert_x_list.ElementAt(0) + 0.1;
                                        vert_y_list[0] = vert_y_list.ElementAt(0) + 0.1;
                                        vert_z_list[0] = vert_z_list.ElementAt(0) + 0.1;
                                        collision_fix_count++;
                                    }
                                    if (vert_x_list.ElementAt(1) == vert_x_list.ElementAt(2) && vert_y_list.ElementAt(1) == vert_y_list.ElementAt(2) && vert_z_list.ElementAt(1) == vert_z_list.ElementAt(2))
                                    {
                                        vert_x_list[1] = vert_x_list.ElementAt(1) + 0.1;
                                        vert_y_list[1] = vert_y_list.ElementAt(1) + 0.1;
                                        vert_z_list[1] = vert_z_list.ElementAt(1) + 0.1;
                                        collision_fix_count++;
                                    }
                                    if (vert_x_list.ElementAt(0) == vert_x_list.ElementAt(2) && vert_y_list.ElementAt(0) == vert_y_list.ElementAt(2) && vert_z_list.ElementAt(0) == vert_z_list.ElementAt(2))
                                    {
                                        vert_x_list[2] = vert_x_list.ElementAt(2) + 0.1;
                                        vert_y_list[2] = vert_y_list.ElementAt(2) + 0.1;
                                        vert_z_list[2] = vert_z_list.ElementAt(2) + 0.1;
                                        collision_fix_count++;
                                    }

                                    //Compile this data now we know it is correct
                                    for (int i = 0; i < this_face_vert_count; i++) // this_face_vert_count = 3
                                    {
                                        this_face_complete += "(" + vert_x_list.ElementAt(i) + ", " + vert_y_list.ElementAt(i) + ", " + vert_z_list.ElementAt(i) + "), ";
                                    }
                                    this_face_complete = this_face_complete.Substring(0, this_face_complete.Length - 2);

                                    final_collmap_data.Add(this_face_complete);
                                }
                            }
                            if (shouldGenerateCollmap.Checked && model_supports_collision)
                            {
                                //Output full collision map
                                File.WriteAllLines(import_directory + Path.GetFileNameWithoutExtension(final_asset_path) + ".COLLMAP", final_collmap_data);
                            }
                            if (shouldCreateBoxCollider.Checked)
                            {
                                //Save box data to add to json file
                                json_extras = ",\"has_box_collider\": true,\"collision_box\": { " +
                                    "\"front_top_left\": [" + biggest_vert[0] + "," + biggest_vert[1] + "," + biggest_vert[2] + "], " +
                                    "\"front_top_right\": [" + smallest_vert[0] + "," + biggest_vert[1] + "," + biggest_vert[2] + "], " +
                                    "\"front_bottom_right\": [" + smallest_vert[0] + "," + smallest_vert[1] + "," + biggest_vert[2] + "], " +
                                    "\"front_bottom_left\": [" + biggest_vert[0] + "," + smallest_vert[1] + "," + biggest_vert[2] + "], " +
                                    "\"back_top_left\": [" + biggest_vert[0] + "," + biggest_vert[1] + "," + smallest_vert[2] + "], " +
                                    "\"back_top_right\": [" + smallest_vert[0] + "," + biggest_vert[1] + "," + smallest_vert[2] + "], " +
                                    "\"back_bottom_right\": [" + smallest_vert[0] + "," + smallest_vert[1] + "," + smallest_vert[2] + "], " +
                                    "\"back_bottom_left\": [" + biggest_vert[0] + "," + smallest_vert[1] + "," + smallest_vert[2] + "] }";

                                //Write out a model for debugging collision box
                                List<string> box_collider_mesh = new List<string>();
                                box_collider_mesh.Add("# AUTOGENERATED MESH TO DEMONSTRATE BOX COLLIDER");
                                box_collider_mesh.Add("mtllib " + Path.GetFileNameWithoutExtension(final_asset_path).Replace(' ', '_') + "_DEBUG.MTL");
                                box_collider_mesh.Add("v " + biggest_vert[0] + " " + biggest_vert[1] + " " + biggest_vert[2]);
                                box_collider_mesh.Add("v " + smallest_vert[0] + " " + biggest_vert[1] + " " + biggest_vert[2]);
                                box_collider_mesh.Add("v " + smallest_vert[0] + " " + smallest_vert[1] + " " + biggest_vert[2]);
                                box_collider_mesh.Add("v " + biggest_vert[0] + " " + smallest_vert[1] + " " + biggest_vert[2]);
                                box_collider_mesh.Add("v " + biggest_vert[0] + " " + biggest_vert[1] + " " + smallest_vert[2]);
                                box_collider_mesh.Add("v " + smallest_vert[0] + " " + biggest_vert[1] + " " + smallest_vert[2]);
                                box_collider_mesh.Add("v " + smallest_vert[0] + " " + smallest_vert[1] + " " + smallest_vert[2]);
                                box_collider_mesh.Add("v " + biggest_vert[0] + " " + smallest_vert[1] + " " + smallest_vert[2]);
                                box_collider_mesh.Add("vt 0.0000 0.0000\nvt 1.0000 0.0000\nvt 1.0000 1.0000\nvt 0.0000 1.0000\n" +
                                    "vt 0.0000 0.0000\nvt 1.0000 0.0000\nvt 1.0000 1.0000\nvt 0.0000 1.0000\nvt 1.0000 0.0000\n" +
                                    "vt 1.0000 1.0000\nvt 0.0000 1.0000\nvt 0.0000 0.0000\nvt 1.0000 0.0000\nvt 1.0000 1.0000\n" +
                                    "vt 1.0000 0.0000\nvt 0.0000 1.0000\nvt 0.0000 0.0000\nvt 1.0000 0.0000\nvt 0.0000 1.0000\n" +
                                    "vn 0.0000 -0.0000 1.0000\nvn 0.0000 1.0000 0.0000\nvn 0.0000 -1.0000 -0.0000\nvn 1.0000 0.0000 0.0000");
                                box_collider_mesh.Add("usemtl debug_mat");
                                box_collider_mesh.Add("s off");
                                box_collider_mesh.Add("f 1/1/1 2/2/1 3/3/1 4/4/1");
                                box_collider_mesh.Add("f 5/5/1 6/6/1 7/7/1 8/8/1");
                                box_collider_mesh.Add("f 1/1/2 5/9/2 6/10/2 2/11/2");
                                box_collider_mesh.Add("f 3/12/3 7/13/3 8/14/3 4/4/3");
                                box_collider_mesh.Add("f 1/1/4 4/15/4 8/14/4 5/16/4");
                                box_collider_mesh.Add("f 2/17/4 3/18/4 7/7/4 6/19/4");
                                File.WriteAllLines(import_directory + Path.GetFileNameWithoutExtension(final_asset_path) + " DEBUG.OBJ", box_collider_mesh);

                                box_collider_mesh.Clear();
                                box_collider_mesh.Add("newmtl debug_mat");
                                box_collider_mesh.Add("Ns 96.000000");
                                box_collider_mesh.Add("Ka 1.000000 1.000000 1.000000");
                                box_collider_mesh.Add("Kd 0.005002 0.640000 0.000000");
                                box_collider_mesh.Add("Ks 0.500000 0.500000 0.500000");
                                box_collider_mesh.Add("Ke 0.000000 0.000000 0.000000");
                                box_collider_mesh.Add("Ni 1.000000");
                                box_collider_mesh.Add("d 0.400000");
                                box_collider_mesh.Add("illum 2");
                                box_collider_mesh.Add("map_Kd collision_debug.png");
                                File.WriteAllLines(import_directory + Path.GetFileNameWithoutExtension(final_asset_path).Replace(' ', '_') + "_DEBUG.MTL", box_collider_mesh);
                                File.Copy(import_directory + "../collision_debug.png", import_directory + "collision_debug.png");

                                ProcessStartInfo meshConverter2 = new ProcessStartInfo();
                                meshConverter2.WorkingDirectory = import_directory;
                                meshConverter2.FileName = "DATA/MODELS/meshconvert.exe";
                                meshConverter2.Arguments = "\"" + Path.GetFileNameWithoutExtension(final_asset_path) + " DEBUG.OBJ" + "\" -sdkmesh -nodds -y";
                                meshConverter2.UseShellExecute = false;
                                meshConverter2.RedirectStandardOutput = true;
                                Process converterProcess2 = Process.Start(meshConverter2);
                                converterProcess2.WaitForExit();

                                File.Delete(import_directory + Path.GetFileNameWithoutExtension(final_asset_path) + " DEBUG.OBJ");
                                File.Delete(import_directory + Path.GetFileNameWithoutExtension(final_asset_path).Replace(' ', '_') + "_DEBUG.MTL");
                                if (File.Exists(import_directory + Path.GetFileNameWithoutExtension(final_asset_path) + " DEBUG.sdkmesh"))
                                {
                                    File.Move(import_directory + Path.GetFileNameWithoutExtension(final_asset_path) + " DEBUG.sdkmesh", 
                                        import_directory + Path.GetFileNameWithoutExtension(final_asset_path) + " DEBUG.SDKMESH");
                                }
                            }
                        }

                        //Conversion complete, delete MTL and fix up OBJ for model previewer
                        obj_index = 0;
                        foreach (string line in final_obj_file)
                        {
                            if (line.Contains("mtllib"))
                            {
                                final_obj_file[obj_index] = "# MTLLIB REMOVED FOR MARIO KART ASSET MANAGER";
                                break;
                            }
                            obj_index++;
                        }
                        File.Delete(import_directory + Path.GetFileName(modelPath.Text));
                        File.WriteAllLines(import_directory + Path.GetFileNameWithoutExtension(final_asset_path) + ".OBJ", final_obj_file);
                        if (importedMTL != "")
                        {
                            File.Delete(import_directory + importedMTL);
                        }

                        //Create JSON data
                        JToken asset_json = JToken.Parse("{\"asset_name\": \"" + assetName.Text + "\", \"asset_type\": \"Models\"" + json_extras + ", \"visible\": true, \"start_x\": 0, \"start_y\": 0, \"start_z\": 0, \"modelscale\": 1.0, \"rot_x\": 0, \"rot_y\": 0, \"rot_z\": 0}");
                        File.WriteAllText(final_asset_path.Substring(0, final_asset_path.Length - 7) + "JSON", asset_json.ToString(Formatting.Indented));

                        //Move new SDKMESH to the correct requested filename
                        File.Move(import_directory + Path.GetFileNameWithoutExtension(modelPath.Text) + ".sdkmesh", final_asset_path);
                        string final_confirmation = "Model imported with" + writeInfo + ".";
                        if (found_count > 0)
                        {
                            final_confirmation += "\nFound " + found_count + " material(s).";
                        }
                        if (shouldCreateBoxCollider.Checked)
                        {
                            final_confirmation += "\nSuccessfully generated data for box collider.";
                        }
                        if (shouldGenerateCollmap.Checked && !model_supports_collision)
                        {
                            final_confirmation += "\nERROR: Mesh collider was not generated as this model is not triangulated.";
                        }
                        else if (shouldGenerateCollmap.Checked)
                        {
                            final_confirmation += "\nSuccessfully generated mesh collider for " + mat_check_count + " model sections.";
                            if (collision_fix_count > 0)
                            {
                                final_confirmation += "\nWARNING: Automatically fixed " + collision_fix_count + " mesh collision issues.";
                            }
                        }
                        if (lost_count > 0)
                        {
                            final_confirmation += "\nWARNING: Failed to find " + lost_count + " materials.";
                        }
                        MessageBox.Show(final_confirmation, "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        //Conversion must've failed - GAH!
                        Directory.Delete(import_directory, true);
                        if (didHaveUglyPath)
                        {
                            //Failed because I haven't implemented a handler for ugly MTL paths
                            MessageBox.Show("Model import failed due to an incorrect local path to the MTL file.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        else if (vertexOrderIsBusted)
                        {
                            //Failed because the vertex order is busted and I haven't added a fix for this
                            MessageBox.Show("Model import failed due to an inverted vertex index list.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                    Directory.Delete(import_directory, true);
                    MessageBox.Show("Import failed because the tool was unable to locate a required MTL file for this model.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        //Restyle form based on if we should generate a collmap or not
        private void shouldGenerateCollmap_CheckedChanged_1(object sender, EventArgs e)
        {
            if (shouldGenerateCollmap.Checked)
            {
                importModel.Location = new Point(19, 234);
                itemMaterialCategories.Visible = true;
                this.Size = new Size(310, 311);
            }
            else
            {
                importModel.Location = new Point(19, 115);
                itemMaterialCategories.Visible = false;
                this.Size = new Size(310, 191);
            }
        }

        //On first load, resize form to non-collmap view
        private void Model_Importer_Load(object sender, EventArgs e)
        {
            importModel.Location = new Point(19, 113);
            itemMaterialCategories.Visible = false;
            this.Size = new Size(310, 191);
        }


        private void addTexture_Click(object sender, EventArgs e)
        {
            //depreciated
        }
        private void removeTexture_Click(object sender, EventArgs e)
        {
            //depreciated
        }
        private void shouldGenerateCollmap_CheckedChanged(object sender, EventArgs e)
        {
            //depreciated
        }
    }
}
