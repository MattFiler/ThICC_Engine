using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Model_Importer_MaterialList : Form
    {
        JObject model_material_config = new JObject();
        List<JToken> material_tokens = new List<JToken>();
        UsefulFunctions common_functions = new UsefulFunctions();
        JToken extra_json = JToken.Parse("{}");
        Model_Importer_Common importer_common;
        public Model_Importer_MaterialList(Model_Importer_Common _importer_conf)
        {
            importer_common = _importer_conf;
            model_material_config = JObject.Parse(File.ReadAllText(importer_common.fileName(importer_file.IMPORTER_CONFIG)));
            InitializeComponent();
        }

        private void Model_Importer_pt2_Load(object sender, EventArgs e)
        {
            //Parse material data on load
            foreach (var material_object in model_material_config)
            {
                materialList.Items.Add(material_object.Key);
                material_tokens.Add(model_material_config[material_object.Key]);
            }

            //Collision properties only visible for map
            if (importer_common.getModelType() != ModelType.MAP)
            {
                configPreview.Visible = false;
                editMaterial.Location = new Point(272, 125);
                autoDetect.Visible = false;
            }
        }

        private void materialList_SelectedIndexChanged(object sender, EventArgs e)
        {
            //Only act if material selected
            int index = materialList.SelectedIndex;
            if (index == -1)
            {
                return;
            }

            //Update previews of config - this is kinda hard coded, needs to be updated if enum is changed (UI!)
            JToken this_token = material_tokens.ElementAt(index);
            isTrack.Checked = this_token["MARIOKART_COLLISION"]["0"].Value<bool>();
            isOffTrack.Checked = this_token["MARIOKART_COLLISION"]["1"].Value<bool>();
            isBoostPad.Checked = this_token["MARIOKART_COLLISION"]["2"].Value<bool>();
            isWall.Checked = this_token["MARIOKART_COLLISION"]["3"].Value<bool>();

            //Try find and show our material preview.
            common_functions.loadMaterialPreview(this_token, materialPreview, importer_common.importDir());
        }

        private void editMaterial_Click(object sender, EventArgs e)
        {
            //Only act if material selected
            int index = materialList.SelectedIndex;
            if (index == -1)
            {
                return;
            }

            using (var editor = new Model_Importer_MaterialEditor(material_tokens.ElementAt(index), importer_common.getModelType(), importer_common.importDir()))
            {
                editor.ShowDialog();
                if (editor.DialogResult == DialogResult.OK)
                {
                    //refresh jobject
                }
            }
        }

        private void SaveMaterials_Click(object sender, EventArgs e)
        {
            //Save json config back
            File.WriteAllText(importer_common.fileName(importer_file.IMPORTER_CONFIG), model_material_config.ToString(Formatting.Indented));

            //------

            //Rewrite MTL from json
            List<string> new_mtl = new List<string>();
            new_mtl.Add("### CREATED BY THE MARIO KART TOOLKIT ###");
            new_mtl.Add("");
            foreach (var this_material_config in model_material_config)
            {
                foreach (JProperty material_prop in model_material_config[this_material_config.Key])
                {
                    //Ignore MarioKart config
                    if (material_prop.Name != "MARIOKART_COLLISION")
                    {
                        //Fix transparency issue
                        if (material_prop.Name == "d" && material_prop.Value.Value<string>() == "1.000000")
                        {
                            material_prop.Value = "0.999999";
                        }
                        //Fix filepath issue
                        if (material_prop.Name.Contains("map"))
                        {
                            material_prop.Value = Path.GetFileName(material_prop.Value.Value<string>());
                        }
                        //Write config to mtl list
                        new_mtl.Add(material_prop.Name + " " + material_prop.Value);
                    }
                }
                new_mtl.Add("");
            }
            File.Delete(importer_common.fileName(importer_file.MATERIAL));
            File.WriteAllLines(importer_common.fileName(importer_file.MATERIAL), new_mtl);

            //------

            //Make sure our MTL is uncommented in the OBJ
            int obj_index = 0;
            string[] obj_file = File.ReadAllLines(importer_common.fileName(importer_file.OBJ_MODEL));
            foreach (string line in obj_file)
            {
                if (line.Contains("mtllib"))
                {
                    if (line.Substring(0, 1) == "#")
                    {
                        obj_file[obj_index] = obj_file[obj_index].Substring(1);
                    }
                    break;
                }
                obj_index++;
            }
            File.WriteAllLines(importer_common.fileName(importer_file.OBJ_MODEL), obj_file);

            //------

            //If we're in edit mode, delete the old files
            if (importer_common.getEditMode())
            {
                if (File.Exists(importer_common.fileName(importer_file.COLLMAP)))
                {
                    File.Delete(importer_common.fileName(importer_file.COLLMAP));
                }
                if (File.Exists(importer_common.fileName(importer_file.SDK_MESH)))
                {
                    File.Delete(importer_common.fileName(importer_file.SDK_MESH));
                }
            }

            //------

            //Run the model converter to swap our OBJ into an SDKMESH
            string conv_args = "\"" + Path.GetFileName(importer_common.fileName(importer_file.OBJ_MODEL)) + "\" -sdkmesh -nodds -y";
            if (shouldFlipUVs.Checked)
            {
                conv_args += " -flipv";
            }
            this.Cursor = Cursors.WaitCursor;
            ProcessStartInfo meshConverter = new ProcessStartInfo();
            meshConverter.WorkingDirectory = importer_common.importDir();
            meshConverter.FileName = "DATA/MODELS/meshconvert.exe";
            meshConverter.Arguments = conv_args;
            meshConverter.UseShellExecute = false;
            meshConverter.RedirectStandardOutput = true;
            meshConverter.CreateNoWindow = true;
            Process converterProcess = Process.Start(meshConverter);
            StreamReader reader = converterProcess.StandardOutput;
            converterProcess.WaitForExit();
            this.Cursor = Cursors.Default;

            //Capture write info from converter
            string writeInfo = "";
            foreach (string line in reader.ReadToEnd().Split('\n'))
            {
                if (line.Contains("written"))
                {
                    writeInfo = line.Substring(0, line.Length - 2);
                    break;
                }
            }

            //Make sure our SDKMESH is in caps!
            string lowercase_sdkmesh = importer_common.fileName(importer_file.SDK_MESH).Substring(0, importer_common.fileName(importer_file.SDK_MESH).Length - 7) + "sdkmesh";
            if (File.Exists(lowercase_sdkmesh)) {
                File.Move(lowercase_sdkmesh, importer_common.fileName(importer_file.SDK_MESH));
            }

            //Make sure the conversion succeeded
            if (!File.Exists(importer_common.fileName(importer_file.SDK_MESH)))
            {
                //Failed! Show full output from DXTK converter if requested.
                DialogResult showErrorInfo = MessageBox.Show("Model import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                if (showErrorInfo == DialogResult.Yes)
                {
                    MessageBox.Show(reader.ReadToEnd(), "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }

            //------

            //Vertex operations
            if (handleVertexOperations())
            {
                //Handle success of collision
            }

            //------

            //Create JSON data
            JToken asset_json = extra_json;
            asset_json["asset_name"] = importer_common.modelName();
            asset_json["asset_type"] = "Models";
            asset_json["model_type"] = (int)importer_common.getModelType();
            asset_json["visible"] = true;
            asset_json["start_x"] = 0;
            asset_json["start_y"] = 0;
            asset_json["start_z"] = 0;
            asset_json["modelscale"] = 1.0;
            asset_json["rot_x"] = 0;
            asset_json["rot_y"] = 0;
            asset_json["rot_z"] = 0;
            asset_json["segment_size"] = 10;

            //If model type is track, parse config
            if (importer_common.getModelType() == ModelType.MAP)
            {
                JArray camera_array = new JArray();
                JArray waypoint_array = new JArray();
                JArray spawnpoint_array = new JArray();
                JArray finishline_array = new JArray();
                JArray itembox_array = new JArray();
                if (File.Exists(importer_common.getModelConfigPath()))
                {
                    JToken model_blender_data = JToken.Parse(File.ReadAllText(importer_common.getModelConfigPath()));
                    foreach (JToken data in model_blender_data["cams"])
                    {
                        camera_array.Add(data);
                    }
                    foreach (JToken data in model_blender_data["waypoints"])
                    {
                        waypoint_array.Add(data["pos"]);
                    }
                    foreach (JToken data in model_blender_data["spawns"])
                    {
                        spawnpoint_array.Add(data["pos"]);
                    }
                    foreach (JToken data in model_blender_data["finish_line"])
                    {
                        finishline_array.Add(data);
                    }
                    foreach (JToken data in model_blender_data["item_boxes"])
                    {
                        itembox_array.Add(data);
                    }
                }
                asset_json["map_cameras"] = camera_array;
                asset_json["map_waypoints"] = waypoint_array;
                asset_json["map_spawnpoints"] = spawnpoint_array;
                asset_json["map_finishline"] = finishline_array;
                asset_json["map_itemboxes"] = itembox_array;
            }

            //Save JSON data if not in edit mode
            if (!importer_common.getEditMode())
            {
                File.WriteAllText(importer_common.fileName(importer_file.CONFIG), asset_json.ToString(Formatting.Indented));
            }

            //------
            
            //Comment out mtllib in OBJ for asset previewer
            obj_index = 0;
            foreach (string line in obj_file)
            {
                if (line.Contains("mtllib"))
                {
                    obj_file[obj_index] = "#" + obj_file[obj_index];
                    break;
                }
                obj_index++;
            }
            File.WriteAllLines(importer_common.fileName(importer_file.OBJ_MODEL), obj_file);

            //------

            //Done
            //File.Delete(importer_common.fileName(importer_file.IMPORTER_CONFIG));
            MessageBox.Show("Model import complete.", "Imported.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }


        /* Vertex operations (collision) */
        private bool handleVertexOperations()
        {
            int collision_fix_count = 0;

            //Output face vertex data for generating our collmap
            List<List<double>> model_vertices_raw = new List<List<double>>();
            List<List<List<int>>> model_face_indexes = new List<List<List<int>>>();
            List<string> final_collmap_data = new List<string>();
            string[] obj_file = File.ReadAllLines(importer_common.fileName(importer_file.OBJ_MODEL));
            for (int i = 0; i < (int)CollisionType.NUM_OF_TYPES; i++)
            {
                model_face_indexes.Add(new List<List<int>>());
            }

            //Use cases
            bool collision_enabled = false;
            CollisionType collision_type = (CollisionType)0;

            //Grab all vertices and face vert indexes from our OBJ
            foreach (string line in obj_file)
            {
                //Capture all vertices
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
                    collision_enabled = false;
                    try
                    {
                        JToken collision_config = model_material_config[line.Substring(7)]["MARIOKART_COLLISION"];
                        for (int i = 0; i < (int)CollisionType.NUM_OF_TYPES; i++)
                        {
                            if (collision_config[i.ToString()].Value<bool>())
                            {
                                collision_enabled = true;
                                collision_type = (CollisionType)i;
                            }
                        }
                    }
                    catch
                    {
                        //This will be hit if a material is missing in the JSON file.
                        //Would be caused by an issue in the MTL to JSON script in the asset selector.
                        int do_breakpoint_here = 0;
                    }
                }
                //Capture face data per use case to link to vertices
                if (line.Length > 2 && line.Substring(0, 2) == "f " && collision_enabled)
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
                    if (collision_enabled)
                    {
                        model_face_indexes.ElementAt((int)collision_type).Add(face_array_parsed);
                    }
                    continue;
                }
            }

            //Calculate box collider (applies to everything BUT maps)
            if (importer_common.getModelType() != ModelType.MAP)
            {
                //Order the data
                double[] biggest_vert = { 0, 0, 0 };
                double[] smallest_vert = { 0, 0, 0 };
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

                //Write the data to a json token
                JToken collision_info = JToken.Parse(
                    "{\"front_top_left\": [" + biggest_vert[0] + "," + biggest_vert[1] + "," + biggest_vert[2] + "], " +
                    "\"front_top_right\": [" + smallest_vert[0] + "," + biggest_vert[1] + "," + biggest_vert[2] + "], " +
                    "\"front_bottom_right\": [" + smallest_vert[0] + "," + smallest_vert[1] + "," + biggest_vert[2] + "], " +
                    "\"front_bottom_left\": [" + biggest_vert[0] + "," + smallest_vert[1] + "," + biggest_vert[2] + "], " +
                    "\"back_top_left\": [" + biggest_vert[0] + "," + biggest_vert[1] + "," + smallest_vert[2] + "], " +
                    "\"back_top_right\": [" + smallest_vert[0] + "," + biggest_vert[1] + "," + smallest_vert[2] + "], " +
                    "\"back_bottom_right\": [" + smallest_vert[0] + "," + smallest_vert[1] + "," + smallest_vert[2] + "], " +
                    "\"back_bottom_left\": [" + biggest_vert[0] + "," + smallest_vert[1] + "," + smallest_vert[2] + "]}");
                extra_json["has_box_collider"] = true;
                extra_json["collision_box"] = collision_info;

                //Debug box for collision
                string debug_material = Path.GetFileNameWithoutExtension(importer_common.fileName(importer_file.MATERIAL)) + "_DEBUG.MTL";
                string debug_model = Path.GetFileNameWithoutExtension(importer_common.fileName(importer_file.OBJ_MODEL)) + " DEBUG.OBJ";

                //Clear up old debug box
                if (importer_common.getEditMode())
                {
                    File.Delete(importer_common.importDir() + debug_model);
                    File.Delete(importer_common.importDir() + debug_material);
                    File.Delete(importer_common.importDir() + "collision_debug.png");
                }

                //Write out a model for debugging collision box
                List<string> box_collider_mesh = new List<string>();
                box_collider_mesh.Add("# AUTOGENERATED MESH TO DEMONSTRATE BOX COLLIDER");
                box_collider_mesh.Add("mtllib " + debug_material);
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
                File.WriteAllLines(importer_common.importDir() + debug_model, box_collider_mesh);

                //Write out material for debug box
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
                File.WriteAllLines(importer_common.importDir() + debug_material, box_collider_mesh);
                File.Copy(importer_common.importDir() + "../collision_debug.png", importer_common.importDir() + "collision_debug.png");

                //Convert debug box to SDKMESH
                ProcessStartInfo meshConverter2 = new ProcessStartInfo();
                meshConverter2.WorkingDirectory = importer_common.importDir();
                meshConverter2.FileName = "DATA/MODELS/meshconvert.exe";
                meshConverter2.Arguments = "\"" + debug_model + "\" -sdkmesh -nodds -y";
                meshConverter2.UseShellExecute = false;
                meshConverter2.RedirectStandardOutput = true;
                Process converterProcess2 = Process.Start(meshConverter2);
                converterProcess2.WaitForExit();

                //Tidy up
                File.Delete(importer_common.importDir() + debug_model);
                File.Delete(importer_common.importDir() + debug_material);
                if (File.Exists(importer_common.importDir() + Path.GetFileNameWithoutExtension(debug_model) + ".sdkmesh"))
                {
                    File.Move(importer_common.importDir() + Path.GetFileNameWithoutExtension(debug_model) + ".sdkmesh",
                              importer_common.importDir() + Path.GetFileNameWithoutExtension(debug_model) + ".SDKMESH");
                }

                //Exit out here, it's box collider OR mesh collider!
                return true;
            }

            //Build up total model verts for collmap reader from parsed data
            List<List<double>> all_verts = new List<List<double>>();
            for (int i = 0; i < (int)CollisionType.NUM_OF_TYPES; i++)
            {
                all_verts.Add(new List<double>());
            }

            //Compile collision data
            int vert_index_i = 0;
            foreach (List<List<int>> these_face_indexes in model_face_indexes)
            {
                foreach (List<int> vert_index_list in these_face_indexes)
                {
                    if (!parseVertices(vert_index_list, model_vertices_raw, collision_fix_count, all_verts.ElementAt(vert_index_i)))
                    {
                        //Model is not triangulated - exit here
                        return false;
                    }
                }
                vert_index_i++;
            }

            //Output vertex data as a binary file
            using (BinaryWriter writer = new BinaryWriter(File.Open(importer_common.fileName(importer_file.COLLMAP), FileMode.Create)))
            {
                writer.Write(all_verts.Count); //Number of collision types to split to
                foreach (List<double> these_verts in all_verts)
                {
                    writer.Write(these_verts.Count); //Number of verts to expect
                }
                foreach (List<double> these_verts in all_verts)
                { 
                    foreach (double vert in these_verts)
                    {
                        writer.Write(vert); //Each bit of collision data
                    }
                }
            }

            return true;
        }

        /* Here we split out our vertex positions to X,Y,Z lists and check for any degenerate triangles */
        private bool parseVertices(List<int> vert_index_list, List<List<double>> model_vertices_raw, int collision_fix_count, List<double> all_verts)
        {
            //Split out verts into nicer X,Y,Z lists
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
                return false;
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

            //Compile for binary writing
            for (int i = 0; i < this_face_vert_count; i++) // this_face_vert_count = 3
            {
                all_verts.Add(vert_x_list.ElementAt(i));
                all_verts.Add(vert_y_list.ElementAt(i));
                all_verts.Add(vert_z_list.ElementAt(i));
            }

            return true;
        }

        /* Auto detect material names and base collision/transparency properties on that */
        private void autoDetect_Click(object sender, EventArgs e)
        {
            //Confirmation
            DialogResult areYouSure = MessageBox.Show("This process will auto-detect the best transparency and colision properties based on material metadata.\nThis will potentially overwrite existing configurations.\nAre you sure you wish to continue?", "Are you sure?", MessageBoxButtons.YesNo, MessageBoxIcon.Information);
            if (areYouSure != DialogResult.Yes)
            {
                return;
            }

            //Loop through every material and base on name if we are transparent or collision
            foreach (var this_material_config in model_material_config)
            {
                JToken this_token = model_material_config[this_material_config.Key];

                //Materials starting with "ef_" are usually boost pads
                if (this_material_config.Key.ToUpper().Substring(0, 3) == "EF_")
                {
                    setCollisionParam(CollisionType.BOOST_PAD, this_token);
                }

                //Materials containing "road" are usually road!
                if (this_material_config.Key.ToUpper().Contains("ROAD"))
                {
                    setCollisionParam(CollisionType.ON_TRACK, this_token);
                }

                //Materials containing "wall" are usually road!
                if (this_material_config.Key.ToUpper().Contains("WALL"))
                {
                    setCollisionParam(CollisionType.WALL, this_token);
                }

                //Materials containing "grass", "suna", or "shiba" are usually off-road!
                if (this_material_config.Key.ToUpper().Contains("GRASS") || this_material_config.Key.ToUpper().Contains("SHIBA") || this_material_config.Key.ToUpper().Contains("SUNA"))
                {
                    setCollisionParam(CollisionType.OFF_TRACK, this_token);
                }

                //Materials containing "nuki" are usually transparent!
                if (this_material_config.Key.ToUpper().Contains("NUKI"))
                {
                    this_token["d"] = "0.999999";
                }
                else
                {
                    this_token["d"] = "0.000000";
                }
            }
        }

        /* Set collision parameter in material config */
        private void setCollisionParam(CollisionType _type, JToken _token)
        {
            for (int i = 0; i < (int)CollisionType.NUM_OF_TYPES; i++)
            {
                bool to_set = false;
                if (i == (int)_type)
                {
                    to_set = true;
                }
                _token["MARIOKART_COLLISION"][i.ToString()] = to_set;
            }
        }
    }
}
