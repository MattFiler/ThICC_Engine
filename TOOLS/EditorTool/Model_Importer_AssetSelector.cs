using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Model_Importer_AssetSelector : Form
    {
        UsefulFunctions function_library = new UsefulFunctions();
        public Model_Importer_Common importer_common { get; set; }
        public Model_Importer_AssetSelector(ModelType model_type)
        {
            importer_common = new Model_Importer_Common();
            importer_common.setModelType(model_type);
            InitializeComponent();
        }

        /* Form changes depending on model type */
        private void Model_Importer_AssetSelector_Load(object sender, EventArgs e)
        {
            if (importer_common.getModelType() == ModelType.MAP)
            {
                label3.Show();
                trackConfig.Show();
                locateTrackConfig.Show();
            }
            else
            {
                importModel.Location = new Point(15, 91);
                this.Size = new Size(370, 166);
            }
        }

        /* Browse to model file */
        private void browseToModel_Click(object sender, EventArgs e)
        {
            modelPath.Text = function_library.userLocatedFile("OBJ Model (OBJ)|*.OBJ");

            //Provide a template asset name if user hasn't already
            if (assetName.Text == "")
            {
                assetName.Text = Path.GetFileNameWithoutExtension(modelPath.Text).Replace(' ', '_');
            }
        }

        /* Browse to config file (track only) */
        private void locateTrackConfig_Click(object sender, EventArgs e)
        {
            trackConfig.Text = function_library.userLocatedFile("Track Config (JSON)|*.JSON");
        }

        /* Copy model files and continue */
        private void importModel_Click(object sender, EventArgs e)
        {
            //Check for all inputs & type validation
            if (assetName.Text == "" || modelPath.Text == "" || (importer_common.getModelType() == ModelType.MAP && trackConfig.Text == ""))
            {
                MessageBox.Show("Please fill out all inputs to continue!", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (!Regex.IsMatch(assetName.Text, "^[_a-zA-Z0-9\x20]+$"))
            {
                MessageBox.Show("Your asset name cannot contain any special characters.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (assetName.Text.Length > 5 && assetName.Text.Substring(assetName.Text.Length - 5) == "DEBUG")
            {
                MessageBox.Show("Your asset name conflicts with a system name, please choose another.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //------

            //Setup asset paths
            importer_common.configureAssetPaths(assetName.Text.ToUpper().Replace(' ', '_'));
            importer_common.setModelConfigPath(trackConfig.Text);

            //Check to see if model name conflicts before going further
            if (Directory.Exists(importer_common.importDir()))
            {
                MessageBox.Show("Couldn't import model, a model with the same name already exists.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                importer_common = new Model_Importer_Common(); //Reset
                return;
            }

            //------

            //Material info
            int expected_mat_count = 0;
            List<string> material_names = new List<string>();

            //Copy model and create directory
            Directory.CreateDirectory(importer_common.importDir());
            File.Copy(modelPath.Text, importer_common.fileName(importer_file.OBJ_MODEL));

            //Calculate the number of materials for the OBJ & store names
            string[] obj_file = File.ReadAllLines(importer_common.fileName(importer_file.OBJ_MODEL));
            foreach (string line in obj_file)
            {
                if (line.Length > 7 && line.Substring(0, 7) == "usemtl ")
                {
                    expected_mat_count++;
                    material_names.Add(line.Substring(7));
                }
            }

            //Trawl through our model to find the original MTL name
            string old_mtl_path = "";
            for (int i = 0; i < obj_file.Length; i++)
            {
                if (obj_file[i].Contains("mtllib"))
                {
                    old_mtl_path = obj_file[i].Substring(7);
                    obj_file[i] = "mtllib " + Path.GetFileName(importer_common.fileName(importer_file.MATERIAL));
                    break;
                }
            }
            File.WriteAllLines(importer_common.fileName(importer_file.OBJ_MODEL), obj_file);

            //------

            //Find and copy MTL info
            if (old_mtl_path == "")
            {
                //The model has no MTL file - this can be handled, but fail for now.
                MessageBox.Show("This model has no materials.\nThe new importer doesn't handle this yet!", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                importer_common = new Model_Importer_Common(); //Reset
                return;
            }
            else
            {
                if (File.Exists(old_mtl_path))
                {
                    //File path is global
                    File.Copy(old_mtl_path, importer_common.fileName(importer_file.MATERIAL));
                }
                else
                {
                    old_mtl_path = Path.GetFullPath(Path.GetDirectoryName(modelPath.Text)) + "/" + old_mtl_path;
                    if (File.Exists(old_mtl_path))
                    {
                        //File path is local, correct that
                        File.Copy(old_mtl_path, importer_common.fileName(importer_file.MATERIAL));
                    }
                    else
                    {
                        //No idea where the file is! Ideally here we'll show a file picker as a last resort, or do some further logic.
                        MessageBox.Show("Import failed because the tool was unable to locate a required MTL file for this model.", "Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        importer_common = new Model_Importer_Common(); //Reset
                        return;
                    }
                }
            }

            //Trawl MTL for info & copy materials
            int mtl_index = 0;
            int mat_start = 0;
            string[] mtl_file = File.ReadAllLines(importer_common.fileName(importer_file.MATERIAL));
            List<string> referenced_materials = new List<string>();
            List<int> material_prop_count = new List<int>();
            List<string> material_props = new List<string>();
            string map_Kd = "";
            foreach (string line in mtl_file)
            {
                /* Start of new material */
                if (line.Contains("newmtl "))
                {
                    if (referenced_materials.Count != 0)
                    {
                        //Copy all found props across
                        int prop_count = mtl_index - mat_start;
                        bool has_spm = false;
                        bool has_nrm = false;
                        bool has_emm = false;
                        for (int i = 0; i < prop_count; i++)
                        {
                            material_props.Add(mtl_file[mat_start + i]);

                            string prop = mtl_file[mat_start + i];
                            string[] prop_split = prop.Split(new[] { ' ' }, 2);
                            if (prop_split[0] == "map_Ks") { has_spm = true; }
                            if (prop_split[0] == "map_Kn" || prop_split[0] == "norm") { has_nrm = true; }
                            if (prop_split[0] == "map_Ke" || prop_split[0] == "map_emissive") { has_emm = true; }
                        }
                        //Try and find some of our own using common names
                        if (!has_spm)
                        {
                            //Specular map
                            string copied_to = findAndCopyMatVariant("spm", map_Kd);
                            if (copied_to != "")
                            {
                                material_props.Add("map_Ks " + Path.GetFileName(copied_to));
                                prop_count++;
                            }
                        }
                        if (!has_nrm)
                        {
                            //Normal map
                            string copied_to = findAndCopyMatVariant("nrm", map_Kd);
                            if (copied_to != "")
                            {
                                material_props.Add("map_Kn " + Path.GetFileName(copied_to));
                                material_props.Add("norm " + Path.GetFileName(copied_to));
                                prop_count += 2;
                            }
                        }
                        if (!has_emm)
                        {
                            //Emissive map
                            string copied_to = findAndCopyMatVariant("emm", map_Kd);
                            if (copied_to != "")
                            {
                                material_props.Add("map_Ke " + Path.GetFileName(copied_to));
                                material_props.Add("map_emissive " + Path.GetFileName(copied_to));
                                prop_count += 2;
                            }
                        }
                        material_prop_count.Add(prop_count);
                    }
                    mat_start = mtl_index;
                    referenced_materials.Add(line.Substring(7));
                }
                /* Material file */
                else if (line.Contains("map"))
                {
                    string[] line_split = line.Split(new[] { ' ' }, 2);
                    string material_path = line_split[1];
                    string import_path = importer_common.importDir() + Path.GetFileName(material_path).Replace(' ', '_');
                    if (File.Exists(material_path))
                    {
                        //File path is global
                        if (!File.Exists(import_path))
                        {
                            File.Copy(material_path, import_path);
                        }
                        if (line_split[0] == "map_Kd" || line_split[0] == "map_d")
                        {
                            map_Kd = material_path;
                        }
                        mtl_file[mtl_index] = line_split[0] + " " + Path.GetFileName(import_path);
                    }
                    else
                    {
                        material_path = Path.GetFullPath(Path.GetDirectoryName(old_mtl_path)) + "/" + material_path;
                        if (File.Exists(material_path))
                        {
                            //File path is local
                            if (!File.Exists(import_path))
                            {
                                File.Copy(material_path, import_path);
                            }
                            if (line_split[0] == "map_Kd" || line_split[0] == "map_d")
                            {
                                map_Kd = material_path;
                            }
                            mtl_file[mtl_index] = line_split[0] + " " + Path.GetFileName(import_path);
                        }
                        else
                        {
                            //Need to fail here as the engine hates missing textures!
                            Directory.Delete(importer_common.importDir(), true);
                            MessageBox.Show("Could not find all required materials!\nTry and re-export your model.", "Error.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            importer_common = new Model_Importer_Common();
                            return;
                        }
                    }
                }
                mtl_index++;
            }
            //Final properties for last material in the list
            int final_prop_count = mtl_index - mat_start;
            for (int i = 0; i < final_prop_count; i++)
            {
                material_props.Add(mtl_file[mat_start + i]);
            }
            material_prop_count.Add(final_prop_count);
            importer_common.import_stats.material_count = referenced_materials.Count; //Stats

            //Write out changes to material
            File.WriteAllLines(importer_common.fileName(importer_file.MATERIAL), mtl_file);

            //Copy placeholder materials (PBR IS FUN!)
            File.Copy(importer_common.importDir() + "../rma_placeholder.png", importer_common.importDir() + "rma_placeholder.png"); //RMA
            File.Copy(importer_common.importDir() + "../emm_placeholder.png", importer_common.importDir() + "emm_placeholder.png"); //Emissive
            File.Copy(importer_common.importDir() + "../spec_placeholder.png", importer_common.importDir() + "spec_placeholder.png"); //Specular
            File.Copy(importer_common.importDir() + "../norm_placeholder.png", importer_common.importDir() + "norm_placeholder.png"); //Normals

            //------

            //Rewrite MTL info as JSON for us
            JObject material_config = new JObject();
            JObject mariokart_properties = new JObject();
            for (int i = 0; i < (int)CollisionType.NUM_OF_TYPES; i++)
            {
                mariokart_properties[i.ToString()] = false; //All collision off as default
            }
            int prop_index = 0;
            for (int i = 0; i < referenced_materials.Count(); i++)
            {
                int prop_count = material_prop_count[i];
                JObject this_mat_jobject = new JObject();
                List<string> props = new List<string>();

                //Add all already existing material info
                for (int x = prop_index; x < prop_index + prop_count; x++)
                {
                    if (material_props[x] == "")
                    {
                        continue;
                    }
                    string[] mat_prop = material_props[x].Split(new[] { ' ' }, 2);
                    if (mat_prop[0] == "map_d")
                    {
                        mat_prop[0] = "map_Kd"; //"map_d" is unsupported, but often used instead of "map_Kd" - correct that
                    }
                    this_mat_jobject[mat_prop[0]] = mat_prop[1];
                    props.Add(mat_prop[0]);
                }

                //Below are all default (and handled) material definitions
                //We'll add them all if they don't exist!
                addPropIfNotAlready("Ka", "1.000000 1.000000 1.000000", this_mat_jobject, props); //Ambient Colour (RGB)
                addPropIfNotAlready("Kd", "1.000000 1.000000 1.000000", this_mat_jobject, props); //Diffuse Colour (RGB)
                addPropIfNotAlready("Ks", "1.000000 1.000000 1.000000", this_mat_jobject, props); //Specular Colour (RGB)
                addPropIfNotAlready("Ke", "0.000000 0.000000 0.000000", this_mat_jobject, props); //Emissive Colour (RGB)
                addPropIfNotAlready("d", "1.000000", this_mat_jobject, props); //Alpha (0.5+ = has alpha)
                addPropIfNotAlready("Tr", "0.000000", this_mat_jobject, props); //Transparency (1 = completely invisible)
                addPropIfNotAlready("Ns", "100.000000", this_mat_jobject, props); //Shininess (0-1000)
                addPropIfNotAlready("illum", "2", this_mat_jobject, props); //Specular on/off (2=on)
                addPropIfNotAlready("map_Kd", "", this_mat_jobject, props); //Diffuse Texture
                addPropIfNotAlready("map_Ks", "", this_mat_jobject, props); //Specular Texture
                addPropIfNotAlready("map_Kn", "", this_mat_jobject, props); //Normal Texture
                addPropIfNotAlready("norm", "", this_mat_jobject, props); //Normal Texture (alt def)
                addPropIfNotAlready("map_Ke", "", this_mat_jobject, props); //Emissive Texture
                addPropIfNotAlready("map_emissive", "", this_mat_jobject, props); //Emissive Texture (alt def)
                addPropIfNotAlready("map_RMA", "", this_mat_jobject, props); //RMA Texture
                addPropIfNotAlready("map_occlusionRoughnessMetallic", "", this_mat_jobject, props); //RMA Texture (alt def)

                // Add in placeholders for our custom properties
                this_mat_jobject["ThICC_COLLISION"] = mariokart_properties; 

                material_config[referenced_materials[i]] = this_mat_jobject;
                prop_index += prop_count;
            }
            File.WriteAllText(importer_common.fileName(importer_file.IMPORTER_CONFIG), material_config.ToString(Formatting.Indented));

            //------

            //Progress to next form
            Model_Importer_MaterialList nextForm = new Model_Importer_MaterialList(importer_common);
            nextForm.Show();
            this.Close();
        }

        /* Add material property if it doesn't already exist */
        private void addPropIfNotAlready(string prop_name, string prop_val, JObject prop_json, List<string> props)
        {
            if (!props.Contains(prop_name))
            {
                prop_json[prop_name] = prop_val;
            }
        }

        /* Try to find a material variant using common naming conventions */
        private string findAndCopyMatVariant(string variant, string map_Kd)
        {
            string mat_template = map_Kd.Substring(0, map_Kd.Length - Path.GetExtension(map_Kd).Length - 3);
            string proposed_file = mat_template + variant + Path.GetExtension(map_Kd);
            if (File.Exists(proposed_file))
            {
                string copy_to = importer_common.importDir() + Path.GetFileName(proposed_file).Replace(' ', '_');
                if (!File.Exists(copy_to))
                {
                    File.Copy(proposed_file, copy_to);
                    return copy_to;
                }
            }
            return "";
        }
    }
}
