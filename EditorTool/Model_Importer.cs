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

namespace EditorTool
{
    public partial class Model_Importer : Form
    {
        public Model_Importer()
        {
            InitializeComponent();
        }

        private void browseToModel_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "OBJ Model (OBJ)|*.OBJ";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                modelPath.Text = filePicker.FileName;
            }
        }

        private void addTexture_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "Model Texture (JPG/PNG)|*.JPG;*.PNG";
            filePicker.Multiselect = true;
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                foreach (string file in filePicker.FileNames)
                {
                    bool canCopyWithoutConflict = true;
                    foreach (string texture in textureList.Items)
                    {
                        if (texture == file || Path.GetFileName(texture) == Path.GetFileName(file))
                        {
                            canCopyWithoutConflict = false;
                            break;
                        }
                    }
                    if (canCopyWithoutConflict)
                    {
                        textureList.Items.Add(file);
                    }
                    else
                    {
                        MessageBox.Show("Texture name conflict:\n" + file, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        private void removeTexture_Click(object sender, EventArgs e)
        {
            if (textureList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select a texture from the list.", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                textureList.Items.Remove(textureList.Items[textureList.SelectedIndex]);
            }
        }

        private void importModel_Click(object sender, EventArgs e)
        {
            if (File.Exists("Models/" + Path.GetFileName(modelPath.Text)) || modelPath.Text == "" || File.Exists("Models/" + Path.GetFileNameWithoutExtension(modelPath.Text) + ".sdkmesh"))
            {
                MessageBox.Show("Couldn't import model, a model with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                string pathWithoutExtension = modelPath.Text.Substring(0, modelPath.Text.Length - Path.GetExtension(modelPath.Text).Length);
                string importedMTL = "";

                bool didHaveUglyPath = false;
                bool didFindMTL = false;

                int copied_material_count = 0;
                int material_count = 0;

                //Copy model into our output directory - basically our working directory
                File.Copy(pathWithoutExtension + ".obj", "Models/" + Path.GetFileName(modelPath.Text));

                //Trawl through our model to find the original MTL name
                string[] obj_file = File.ReadAllLines("Models/" + Path.GetFileName(pathWithoutExtension + ".obj"));
                string mtl_suggestedname = "";
                foreach (string line in obj_file)
                {
                    if (line.Contains("mtllib"))
                    {
                        mtl_suggestedname = line.Substring(7);
                        break;
                    }
                }
                
                //Find and fix MTL if one is required
                if (mtl_suggestedname != "")
                {
                    //Check for UGLY paths to original MTL - I'm not writing a handler for this just yet unless we need it
                    if (mtl_suggestedname.Split('/').Length > 1 || mtl_suggestedname.Split('\\').Length > 1)
                    {
                        didHaveUglyPath = true;
                    }

                    //If original MTL exists, copy it over
                    if (File.Exists(Path.GetDirectoryName(modelPath.Text) + @"\" + mtl_suggestedname))
                    {
                        File.Copy(Path.GetDirectoryName(modelPath.Text) + @"\" + mtl_suggestedname, "Models/" + mtl_suggestedname);
                        didFindMTL = true;
                        importedMTL = mtl_suggestedname;
                    }

                    //If we couldn't find our original MTL, attempt to match with our OBJ name
                    if (!didFindMTL && File.Exists(pathWithoutExtension + ".mtl"))
                    {
                        File.Copy(pathWithoutExtension + ".mtl", "Models/" + Path.GetFileName(pathWithoutExtension + ".mtl"));

                        //First fix for crappy DXTK converter - rewrite OBJ to have the correct MTL path
                        int obj_index = 0;
                        foreach (string line in obj_file)
                        {
                            if (line.Contains("mtllib"))
                            {
                                obj_file[obj_index] = "mtllib " + Path.GetFileName(pathWithoutExtension + ".mtl");
                                break;
                            }
                            obj_index++;
                        }
                        File.WriteAllLines("Models/" + Path.GetFileName(pathWithoutExtension + ".obj"), obj_file);

                        didFindMTL = true;
                        importedMTL = Path.GetFileName(pathWithoutExtension + ".mtl");
                    }

                    //Still can't find our MTL file - ask the user to locate it
                    if (!didFindMTL)
                    {
                        MessageBox.Show("Could not find the MTL file for this model.\nPlease locate it.", "Could not auto-locate MTL!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        OpenFileDialog filePicker = new OpenFileDialog();
                        filePicker.Filter = "Model Material Info (MTL)|*.MTL";
                        if (filePicker.ShowDialog() == DialogResult.OK)
                        {
                            File.Copy(filePicker.FileName, "Models/" + Path.GetFileName(filePicker.FileName));

                            //Re-do first fix for crappy DXTK converter now we have our MTL - rewrite OBJ to have the correct MTL path
                            int obj_index = 0;
                            foreach (string line in obj_file)
                            {
                                if (line.Contains("mtllib"))
                                {
                                    obj_file[obj_index] = "mtllib " + Path.GetFileName(filePicker.FileName);
                                    break;
                                }
                                obj_index++;
                            }
                            File.WriteAllLines("Models/" + Path.GetFileName(pathWithoutExtension + ".obj"), obj_file);

                            didFindMTL = true;
                            importedMTL = Path.GetFileName(filePicker.FileName);
                        }
                    }

                    if (didFindMTL)
                    {
                        //Second fix for crappy DXTK converter - rewrite the MTL with proper paths
                        int mtl_index = 0;
                        string[] mtl_file = File.ReadAllLines("Models/" + Path.GetFileName(pathWithoutExtension + ".mtl"));
                        foreach (string line in mtl_file)
                        {
                            if (line.Contains("map"))
                            {
                                string[] map_split = new string[2];
                                map_split = line.Split(' ');
                                map_split[1] = Path.GetFileName(map_split[1]);
                                mtl_file[mtl_index] = map_split[0] + " " + map_split[1];
                                material_count++;
                            }
                            mtl_index++;
                        }
                        File.WriteAllLines("Models/" + Path.GetFileName(pathWithoutExtension + ".mtl"), mtl_file);

                        //Copy over any referenced textures - this should really be handled by trawling the MTL similar to the OBJ trawl earlier
                        foreach (string texture in textureList.Items)
                        {
                            if (!File.Exists("Models/" + Path.GetFileName(texture)))
                            {
                                File.Copy(texture, "Models/" + Path.GetFileName(texture));
                                copied_material_count++;
                            }
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
                    meshConverter.WorkingDirectory = "Models";
                    meshConverter.FileName = "Models/meshconvert.exe";
                    meshConverter.Arguments = Path.GetFileName(modelPath.Text) + " -sdkmesh -nodds -y";
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

                    if (File.Exists("Models/" + Path.GetFileNameWithoutExtension(modelPath.Text) + ".sdkmesh"))
                    {
                        //Conversion complete - delete the OBJ and MTL
                        File.Delete("Models/" + Path.GetFileName(modelPath.Text));
                        if (importedMTL != "")
                        {
                            File.Delete("Models/" + importedMTL);
                        }
                        MessageBox.Show("Model imported with " + writeInfo + ".", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        //Conversion must've failed - GAH!
                        if (didHaveUglyPath)
                        {
                            //Failed because I haven't implemented a handler for ugly MTL paths
                            MessageBox.Show("Model import failed because the original MTL path is UGLY!\nDrop this message to Matt.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        else if (copied_material_count != material_count)
                        {
                            //Failed because some dumbo copied the wrong number of materials
                            //Should never explicitly fail BECAUSE of this, but it's worth a mention
                            MessageBox.Show("Model import failed, and you also copied the wrong number of materials.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
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

                    //Reset our form
                    modelPath.Text = "";
                    textureList.Items.Clear();
                }
                else
                {
                    //Couldn't locate MTL - FAIL!
                    File.Delete("Models/" + Path.GetFileName(modelPath.Text));
                    MessageBox.Show("Import failed because the tool was unable to locate a required MTL file for this model.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
    }
}
