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
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Model_Importer_AssetSelector : Form
    {
        ModelType selected_model_type = ModelType.PROP;
        UsefulFunctions function_library = new UsefulFunctions();
        Model_Importer_Common importer_common = new Model_Importer_Common();
        bool generate_box_collider = false;
        public Model_Importer_AssetSelector(ModelType model_type)
        {
            selected_model_type = model_type;
            InitializeComponent();
        }

        /* Form changes depending on model type */
        private void Model_Importer_AssetSelector_Load(object sender, EventArgs e)
        {
            if (selected_model_type == ModelType.MAP)
            {
                label3.Show();
                trackConfig.Show();
                locateTrackConfig.Show();
            }
            else
            {
                importModel.Location = new Point(15, 91);
                this.Size = new Size(370, 166);
                generate_box_collider = true; //Everything but track generates a box collider
            }
        }

        /* Browse to model file */
        private void browseToModel_Click(object sender, EventArgs e)
        {
            modelPath.Text = function_library.userLocatedFile("OBJ Model (OBJ)|*.OBJ");
        }

        /* Browse to config file (track only) */
        private void locateTrackConfig_Click(object sender, EventArgs e)
        {
            trackConfig.Text = function_library.userLocatedFile("Track Config (JSON)|*.JSON");
        }

        /* Copy model files and continue */
        private void importModel_Click(object sender, EventArgs e)
        {
            //Setup asset paths
            importer_common.configureAssetPaths(assetName.Text.ToUpper());

            //------

            //Material info
            int expected_mat_count = 0;
            List<string> material_names = new List<string>();

            //Copy model and create directory
            Directory.CreateDirectory(importer_common.importDir());
            File.Copy(modelPath.Text, importer_common.fileName(importer_file.MODEL));

            //Calculate the number of materials for the OBJ & store names
            string[] obj_file = File.ReadAllLines(importer_common.fileName(importer_file.MODEL));
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

                    //Correct MTL name to what we'll be using
                    obj_file[i] = "mtllib " + Path.GetFileName(importer_common.fileName(importer_file.MATERIAL));
                    File.WriteAllLines(importer_common.fileName(importer_file.MODEL), obj_file);

                    break;
                }
            }

            //------

            //Find and copy MTL info
            if (old_mtl_path == "")
            {
                //no mtl
                MessageBox.Show("Model has no MTL! This is unhandled for now.");
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
                        //No idea where the file is - show file window
                        MessageBox.Show("Could not locate MTL file.\nIdeally there will be a file window here!");
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
            int mat_couldnt_find = 0;
            foreach (string line in mtl_file)
            {
                /* Start of new material */
                if (line.Contains("newmtl "))
                {
                    if (referenced_materials.Count != 0)
                    {
                        int prop_count = mtl_index - mat_start;
                        for (int i = 0; i < prop_count; i++)
                        {
                            material_props.Add(mtl_file[mat_start + i]);
                        }
                        material_prop_count.Add(prop_count);
                    }
                    mat_start = mtl_index;
                    referenced_materials.Add(line.Substring(7));
                    //Seems to miss last material?!
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
                        mtl_file[mtl_index] = line_split[0] + " " + import_path;
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
                            mtl_file[mtl_index] = line_split[0] + " " + import_path;
                        }
                        else
                        {
                            //No idea where the file is
                            mat_couldnt_find++;
                            //Need to fail here as the game will crash =)
                        }
                    }
                }
                mtl_index++;
            }
            File.WriteAllLines(importer_common.fileName(importer_file.MATERIAL), mtl_file);

            //------

            //Rewrite MTL info as JSON
            JObject material_config = new JObject();
            JObject mariokart_properties = JObject.Parse("{\"is_boost_pad\": false, \"is_on_track\": false, \"is_off_track\": false}");
            int prop_index = 0;
            for (int i = 0; i < referenced_materials.Count()-1; i++)
            {
                int prop_count = material_prop_count[i];
                JObject this_mat_jobject = new JObject();
                for (int x = prop_index; x < prop_index + prop_count; x++)
                {
                    if (material_props[x] == "")
                    {
                        continue;
                    }
                    string[] mat_prop = material_props[x].Split(new[] { ' ' }, 2);
                    this_mat_jobject[mat_prop[0]] = mat_prop[1];
                }

                this_mat_jobject["MARIOKART"] = mariokart_properties; // Add in placeholders for our custom properties
                material_config[referenced_materials[i]] = this_mat_jobject;
                prop_index += prop_count;
            }
            File.WriteAllText(importer_common.fileName(importer_file.CONFIG), material_config.ToString(Formatting.Indented));

            //------

            //Progress to next form
            Model_Importer_MaterialList nextForm = new Model_Importer_MaterialList(importer_common);
            nextForm.Show();
            this.Close();
        }
    }
}
