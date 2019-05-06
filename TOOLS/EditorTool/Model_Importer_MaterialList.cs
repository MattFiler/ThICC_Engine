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
        JToken extra_json;
        bool editing;
        List<List<double>> glider_track = new List<List<double>>();
        Model_Importer_Common importer_common;
        public Model_Importer_MaterialList(Model_Importer_Common _importer_conf, bool is_editing = false)
        {
            editing = is_editing;
            importer_common = _importer_conf;
            extra_json = JToken.Parse(importer_common.getExtraJson());
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

            //If we're editing, check for depreciated configs
            if (editing)
            {
                //Check to see if we have index update
                if (material_tokens.ElementAt(0)["ThICC_INDEX"] == null)
                {
                    //Get all materials in order
                    string[] obj_file = File.ReadAllLines(importer_common.fileName(importer_file.OBJ_MODEL));
                    List<string> material_names = new List<string>();
                    foreach (string line in obj_file)
                    {
                        if (line.Length > 7 && line.Substring(0, 7) == "usemtl ")
                        {
                            material_names.Add(line.Substring(7));
                        }
                    }
                    //Push our order to the config file
                    foreach (var material_object in model_material_config)
                    {
                        int index = 0;
                        foreach (string material in material_names)
                        {
                            if (material == material_object.Key)
                            {
                                break;
                            }
                            index++;
                        }
                        model_material_config[material_object.Key]["ThICC_INDEX"] = index;
                    }
                }
                //Check to see if we have metalness config update
                if (material_tokens.ElementAt(0)["ThICC_METALLIC"] == null)
                {
                    //Push our order to the config file
                    foreach (var material_object in model_material_config)
                    {
                        model_material_config[material_object.Key]["ThICC_METALLIC"] = false;
                    }
                }
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
            if (this_token["ThICC_COLLISION"][((int)CollisionType.ON_TRACK_NO_AI).ToString()] != null) //supporting depreciated configs
            {
                isTrack.Checked = (this_token["ThICC_COLLISION"][((int)CollisionType.ON_TRACK).ToString()].Value<bool>() | this_token["ThICC_COLLISION"][((int)CollisionType.ON_TRACK_NO_AI).ToString()].Value<bool>());
            }
            else
            {
                isTrack.Checked = this_token["ThICC_COLLISION"][((int)CollisionType.ON_TRACK).ToString()].Value<bool>();
            }
            isOffTrack.Checked = this_token["ThICC_COLLISION"][((int)CollisionType.OFF_TRACK).ToString()].Value<bool>();
            isBoostPad.Checked = this_token["ThICC_COLLISION"][((int)CollisionType.BOOST_PAD).ToString()].Value<bool>();
            if (this_token["ThICC_COLLISION"][((int)CollisionType.ANTIGRAV_PAD).ToString()] != null) //supporting depreciated configs
            {
                isAntiGravPad.Checked = this_token["ThICC_COLLISION"][((int)CollisionType.ANTIGRAV_PAD).ToString()].Value<bool>();
            }
            else
            {
                isAntiGravPad.Checked = false;
            }
            if (this_token["ThICC_COLLISION"][((int)CollisionType.JUMP_PAD).ToString()] != null) //supporting depreciated configs
            {
                isJumpPad.Checked = this_token["ThICC_COLLISION"][((int)CollisionType.JUMP_PAD).ToString()].Value<bool>();
            }
            else
            {
                isJumpPad.Checked = false;
            }
            isWall.Checked = this_token["ThICC_COLLISION"][((int)CollisionType.WALL).ToString()].Value<bool>();

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
            Cursor.Current = Cursors.WaitCursor;

            //Save json config back
            File.WriteAllText(importer_common.fileName(importer_file.IMPORTER_CONFIG), model_material_config.ToString(Formatting.Indented));

            //Load OBJ
            string[] obj_file = File.ReadAllLines(importer_common.fileName(importer_file.OBJ_MODEL));

            //------

            //If we're in edit mode, delete the old mesh
            if (importer_common.getEditMode())
            {
                if (File.Exists(importer_common.fileName(importer_file.ENGINE_MESH)))
                {
                    File.Delete(importer_common.fileName(importer_file.ENGINE_MESH));
                }
            }

            //------

            //Rewrite MTL from json
            int diffuse_missing_count = 0;
            List<string> new_mtl = new List<string>();
            new_mtl.Add("### CREATED BY THE ThICC TOOLKIT ###");
            new_mtl.Add("");
            foreach (var this_material_config in model_material_config)
            {
                //Work out if we have alpha in this material & check diffuse map
                string diffuse_map = "";
                foreach (JProperty prop_2 in model_material_config[this_material_config.Key])
                {
                    if (prop_2.Name == "map_d" || prop_2.Name == "map_Kd")
                    {
                        diffuse_map = prop_2.Value.ToString();
                        break;
                    }
                }
                if (diffuse_map == "")
                {
                    diffuse_missing_count++;
                }
                bool has_alpha = common_functions.hasTransparency(importer_common.importDir() + diffuse_map);

                //Parse MTL from JSON
                foreach (JProperty material_prop in model_material_config[this_material_config.Key])
                {
                    //Ignore engine configs
                    if (!(material_prop.Name.Length > 6 && material_prop.Name.Substring(0, 6) == "ThICC_"))
                    {
                        //Auto calculated alpha
                        if (material_prop.Name == "d")
                        {
                            material_prop.Value = (has_alpha ? "0.999999" : "0.000000");
                        }
                        //Transparency hack
                        if (material_prop.Name == "Tr")
                        {
                            if (has_alpha && material_prop.Value.ToString() == "0.000000")
                            {
                                material_prop.Value = "0.000001";
                            }
                        }
                        //Fix filepath issue
                        if (material_prop.Name.Contains("map"))
                        {
                            material_prop.Value = Path.GetFileName(material_prop.Value.Value<string>());
                        }
                        //Include specular placeholder for all blank specular entries
                        if ((material_prop.Name == "map_Ks") && material_prop.Value.Value<string>() == "")
                        {
                            material_prop.Value = "spec_placeholder.png";
                        }
                        //Include normal placeholder for all blank normal entries
                        if ((material_prop.Name == "map_Kn" || material_prop.Name == "norm") && material_prop.Value.Value<string>() == "")
                        {
                            material_prop.Value = "norm_placeholder.png";
                        }
                        //Include emissive placeholder for all blank emissive entries
                        if ((material_prop.Name == "map_Ke" || material_prop.Name == "map_emissive") && material_prop.Value.Value<string>() == "")
                        {
                            material_prop.Value = "emm_placeholder.png";
                        }
                        //Include RMA placeholder for all blank RMA entries
                        if ((material_prop.Name == "map_RMA" || material_prop.Name == "map_occlusionRoughnessMetallic") && material_prop.Value.Value<string>() == "")
                        {
                            material_prop.Value = "rma_placeholder.png";
                        }
                        //Ignore blank props
                        if (material_prop.Value.Value<string>() == "")
                        {
                            continue;
                        }
                        //Write config to mtl list
                        new_mtl.Add(material_prop.Name + " " + material_prop.Value);
                    }
                }
                new_mtl.Add("");
            }

            //Check for missing diffuse maps
            if (diffuse_missing_count > 0)
            {
                MessageBox.Show(diffuse_missing_count + " material(s) are missing diffuse maps.\nImport cannot continue!", "Missing diffuse maps!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update MTL with new info
            File.Delete(importer_common.fileName(importer_file.MATERIAL));
            File.WriteAllLines(importer_common.fileName(importer_file.MATERIAL), new_mtl);

            //------

            //Create metal config
            List<bool> metal_config = new List<bool>();
            for (int i = 0; i < model_material_config.Count + 1; i++)
            {
                foreach (var this_material_config in model_material_config)
                {
                    if (model_material_config[this_material_config.Key]["ThICC_INDEX"].Value<int>() == i)
                    {
                        metal_config.Add(model_material_config[this_material_config.Key]["ThICC_METALLIC"].Value<bool>());
                        break;
                    }
                }
            }
            
            //Output metal config
            using (BinaryWriter writer = new BinaryWriter(File.Open(importer_common.fileName(importer_file.METALLIC_CONFIG), FileMode.Create)))
            {
                addThiccSignature(writer);
                writer.Write(metal_config.Count);
                foreach (bool is_metal in metal_config)
                {
                    writer.Write(is_metal); 
                }
            }

            //------

            //Create animation config
            List<int> anim_mat_index = new List<int>();
            List<List<string>> anim_mat_textures = new List<List<string>>();
            List<float> anim_mat_anim_time = new List<float>();
            for (int i = 0; i < model_material_config.Count + 1; i++)
            {
                foreach (var this_material_config in model_material_config)
                {
                    if (model_material_config[this_material_config.Key]["ThICC_INDEX"].Value<int>() == i)
                    {
                        if (model_material_config[this_material_config.Key]["ThICC_ANIMATION_ENABLED"] != null &&
                            model_material_config[this_material_config.Key]["ThICC_ANIMATION_ENABLED"].Value<bool>() == true)
                        {
                            anim_mat_index.Add(i);
                            List<string> these_textures = new List<string>();
                            foreach (string texture in model_material_config[this_material_config.Key]["ThICC_ANIMATION"].Value<JArray>())
                            {
                                these_textures.Add(texture);
                            }
                            anim_mat_textures.Add(these_textures);
                            anim_mat_anim_time.Add(model_material_config[this_material_config.Key]["ThICC_ANIMATION_TIME"].Value<float>());
                        }
                        break;
                    }
                }
            }

            //Output anim config
            using (BinaryWriter writer = new BinaryWriter(File.Open(importer_common.fileName(importer_file.ANIMATION_CONFIG), FileMode.Create)))
            {
                addThiccSignature(writer);
                writer.Write(anim_mat_index.Count);
                for (int i = 0; i < anim_mat_index.Count; i++)
                {
                    writer.Write(anim_mat_index.ElementAt(i));
                    writer.Write(anim_mat_textures.ElementAt(i).Count);
                    writer.Write(anim_mat_anim_time.ElementAt(i));
                    foreach (string texture in anim_mat_textures.ElementAt(i))
                    {
                        string updated_texture = Path.GetFileNameWithoutExtension(texture) + ".DDS";
                        writer.Write(updated_texture.Length);
                        for (int x = 0; x < updated_texture.Length; x++)
                        {
                            writer.Write(updated_texture[x]);
                        }
                    }
                }
            }

            //------

            //Make sure our MTL is uncommented in the OBJ
            int obj_index = 0;
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

            //Delete old DDS materials and convert new ones
            convertMaterials();

            //Run the model converter to swap our OBJ into our engine's format
            string conv_args = "\"" + Path.GetFileName(importer_common.fileName(importer_file.OBJ_MODEL)) + "\" -sdkmesh2 -y";
            if (shouldFlipUVs.Checked)
            {
                conv_args += " -flipv";
            }
            this.Cursor = Cursors.WaitCursor;
            ProcessStartInfo meshConverter = new ProcessStartInfo();
            meshConverter.WorkingDirectory = importer_common.importDir();
            meshConverter.FileName = common_functions.getFolder(AssetType.MODEL) + "meshconvert.exe";
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
            string readerOutput = reader.ReadToEnd();
            foreach (string line in readerOutput.Split('\n'))
            {
                if (line.Contains("written"))
                {
                    writeInfo = line.Substring(0, line.Length - 2);
                    break;
                }
            }

            //Make sure our SDKMESH is in caps!
            string lowercase_sdkmesh = importer_common.fileName(importer_file.ENGINE_MESH).Substring(0, importer_common.fileName(importer_file.ENGINE_MESH).Length - 7) + "sdkmesh";
            if (File.Exists(lowercase_sdkmesh)) {
                File.Move(lowercase_sdkmesh, importer_common.fileName(importer_file.ENGINE_MESH));
            }

            //Make sure the conversion succeeded
            if (!File.Exists(importer_common.fileName(importer_file.ENGINE_MESH)))
            {
                //Failed! Show full output from DXTK converter if requested.
                DialogResult showErrorInfo = MessageBox.Show("Model import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                if (showErrorInfo == DialogResult.Yes)
                {
                    MessageBox.Show(readerOutput, "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }

            //------

            //Create JSON data
            JToken asset_json = JToken.Parse("{}");
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
            asset_json["in_use_in"] = JToken.Parse("[]");

            //If model type is track, parse config
            if (importer_common.getModelType() == ModelType.MAP)
            {
                JArray camera_array = new JArray();
                JArray lookat_array = new JArray();
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
                    foreach (JToken data in model_blender_data["look_at_points"])
                    {
                        lookat_array.Add(data["pos"]);
                    }
                    foreach (JToken data in model_blender_data["waypoints"])
                    {
                        waypoint_array.Add(data);
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
                    if (model_blender_data["glider_track"] != null)
                    {
                        foreach (JArray data in model_blender_data["glider_track"])
                        {
                            List<double> glider_track_array = new List<double>();
                            foreach (var vert in data)
                            {
                                glider_track_array.Add(vert[0].Value<double>());
                                glider_track_array.Add(vert[1].Value<double>());
                                glider_track_array.Add(vert[2].Value<double>());
                            }
                            glider_track.Add(glider_track_array);
                        }
                    }
                }
                asset_json["map_cameras"] = camera_array;
                asset_json["map_cam_lookat"] = lookat_array;
                asset_json["map_waypoints"] = waypoint_array;
                asset_json["map_spawnpoints"] = spawnpoint_array;
                asset_json["map_finishline"] = finishline_array;
                asset_json["map_itemboxes"] = itembox_array;
            }

            //Vertex operations
            if (!handleVertexOperations())
            {
                //It would be nicer to handle this in Model_Importer_AssetSelector before we get this late into the import process.
                MessageBox.Show("An error occured while generating collision data for this model.\nMake sure the mesh is triangulated.", "Collision error.", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            //Add any extra JSON
            foreach (var data in extra_json)
            {
                asset_json.Last.AddAfterSelf(data);
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

            Cursor.Current = Cursors.Default;

            //Done
            string final_confirmation = "Model imported with" + writeInfo + ".";
            if (importer_common.import_stats.material_count > 0)
            {
                final_confirmation += "\nFound " + importer_common.import_stats.material_count + " material(s).";
            }
            if (importer_common.getModelType() != ModelType.MAP)
            {
                final_confirmation += "\nSuccessfully generated data for box collider.";
            }
            else
            {
                final_confirmation += "\nSuccessfully generated mesh collider with " + importer_common.import_stats.vertices + " vertices.";
                if (importer_common.import_stats.collision_fix_count > 0)
                {
                    final_confirmation += "\nWARNING: Automatically fixed " + importer_common.import_stats.collision_fix_count + " mesh collision issues.";
                }
            }
            MessageBox.Show(final_confirmation, "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }


        /* Vertex operations (collision) */
        private bool handleVertexOperations()
        {
            //Output face vertex data for generating our collmap
            List<List<float>> model_vertices_raw = new List<List<float>>();
            List<List<List<int>>> model_face_indexes = new List<List<List<int>>>();
            List<string> final_collmap_data = new List<string>();
            string[] obj_file = File.ReadAllLines(importer_common.fileName(importer_file.OBJ_MODEL));
            for (int i = 0; i < (int)CollisionType.NUM_OF_TYPES; i++)
            {
                model_face_indexes.Add(new List<List<int>>());
            }
            for (int i = 0; i < obj_file.Length; i++)
            {
                //fix for older tabbed OBJ files
                obj_file[i] = obj_file[i].Replace("\t", string.Empty);
                obj_file[i] = obj_file[i].Replace("    ", string.Empty);
                obj_file[i] = obj_file[i].Replace("  ", string.Empty); //I ACTUALLY SAW THIS ONCE!
            }

            //Use cases
            bool collision_enabled = false;
            extra_json["has_box_collider"] = false;
            CollisionType collision_type = (CollisionType)0;

            //Grab all vertices and face vert indexes from our OBJ
            foreach (string line in obj_file)
            {
                //Capture all vertices
                if (line.Length > 2 && line.Substring(0, 2) == "v ")
                {
                    string[] vert_array = line.Substring(2).Split(' ');
                    List<float> this_vertex = new List<float>();
                    foreach (string vert in vert_array)
                    {
                        this_vertex.Add(Convert.ToSingle(vert));
                    }
                    model_vertices_raw.Add(this_vertex);
                    continue;
                }
                //Check to see if we should start listening for faces
                //This may seem odd, but as the OBJ is always written/read in a logical order, this should work 100% of the time
                if (line.Length > 7 && line.Substring(0, 7) == "usemtl ")
                {
                    collision_enabled = false;
                    JToken collision_config = model_material_config[line.Substring(7)]["ThICC_COLLISION"];
                    for (int i = 0; i < (int)CollisionType.NUM_OF_TYPES; i++)
                    {
                        if (collision_config[i.ToString()] != null) //support for depreciated configs
                        {
                            if (collision_config[i.ToString()].Value<bool>())
                            {
                                collision_enabled = true;
                                collision_type = (CollisionType)i;
                            }
                        }
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
                float[] biggest_vert = { 0, 0, 0 };
                float[] smallest_vert = { 0, 0, 0 };
                foreach (List<float> vertex in model_vertices_raw)
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
                    deleteIfExists(importer_common.importDir() + debug_model);
                    deleteIfExists(importer_common.importDir() + debug_material);
                    deleteIfExists(importer_common.importDir() + "collision_debug.png");
                    deleteIfExists(importer_common.importDir() + "collision_debug.DDS");
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
                box_collider_mesh.Add("map_Ks spec_placeholder.png");
                box_collider_mesh.Add("norm norm_placeholder.png");
                box_collider_mesh.Add("map_Ke emm_placeholder.png");
                box_collider_mesh.Add("map_RMA rma_placeholder.png");
                File.WriteAllLines(importer_common.importDir() + debug_material, box_collider_mesh);
                File.Copy(importer_common.importDir() + "../collision_debug.png", importer_common.importDir() + "collision_debug.png");
                convertMaterial("collision_debug.png");

                //Convert debug box to a format for our engine
                ProcessStartInfo meshConverter2 = new ProcessStartInfo();
                meshConverter2.WorkingDirectory = importer_common.importDir();
                meshConverter2.FileName = common_functions.getFolder(AssetType.MODEL) + "meshconvert.exe";
                meshConverter2.Arguments = "\"" + debug_model + "\" -sdkmesh2 -y";
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
            List<List<float>> all_verts = new List<List<float>>();
            for (int i = 0; i < (int)CollisionType.NUM_OF_TYPES; i++)
            {
                all_verts.Add(new List<float>());
            }

            //Compile collision data from model
            int vert_index_i = 0;
            foreach (List<List<int>> these_face_indexes in model_face_indexes)
            {
                foreach (List<int> vert_index_list in these_face_indexes)
                {
                    if (!parseVertices(vert_index_list, model_vertices_raw, all_verts.ElementAt(vert_index_i)))
                    {
                        //Model is not triangulated - exit here
                        return false;
                    }
                }
                vert_index_i++;
            }

            //If we have glider data, add that
            if (glider_track.Count != 0)
            {
                foreach (List<double> glider_tri in glider_track)
                {
                    foreach (double vert in glider_tri)
                    {
                        all_verts.ElementAt((int)CollisionType.GLIDER_TRACK).Add(Convert.ToSingle(vert));
                    }
                }
            }
            else if (editing)
            {
                using (BinaryReader reader = new BinaryReader(File.Open(importer_common.fileName(importer_file.COLLMAP), FileMode.Open)))
                {
                    int collision_count = reader.ReadInt32();
                    reader.BaseStream.Position = sizeof(int) + (sizeof(int) * (int)CollisionType.GLIDER_TRACK);
                    int glider_vert_count = reader.ReadInt32();
                    if (glider_vert_count > 0) //check we actually have a glider track on this map
                    {
                        int offset = 0;
                        reader.BaseStream.Position = sizeof(int);
                        for (int i = 0; i < (int)CollisionType.GLIDER_TRACK; i++) {
                            offset += reader.ReadInt32();
                        }
                        reader.BaseStream.Position = (sizeof(int) * (collision_count + 1)) + (offset * sizeof(float));
                        for (int i = 0; i < glider_vert_count; i++)
                        {
                            all_verts.ElementAt((int)CollisionType.GLIDER_TRACK).Add(reader.ReadSingle());
                        }
                    }
                }
            }

            //Output vertex data as a binary file (overwrite old one if it exists)
            using (BinaryWriter writer = new BinaryWriter(File.Open(importer_common.fileName(importer_file.COLLMAP), FileMode.Create)))
            {
                writer.Write(all_verts.Count); //Number of collision types to split to
                foreach (List<float> these_verts in all_verts)
                {
                    writer.Write(these_verts.Count); //Number of verts to expect
                    importer_common.import_stats.vertices += these_verts.Count;
                }
                foreach (List<float> these_verts in all_verts)
                { 
                    foreach (float vert in these_verts)
                    {
                        writer.Write(vert); //Each bit of collision data
                    }
                }
            }

            return true;
        }

        /* Here we split out our vertex positions to X,Y,Z lists and check for any degenerate triangles */
        private bool parseVertices(List<int> vert_index_list, List<List<float>> model_vertices_raw, List<float> all_verts)
        {
            //Split out verts into nicer X,Y,Z lists
            int this_face_vert_count = 0;
            List<float> vert_x_list = new List<float>();
            List<float> vert_y_list = new List<float>();
            List<float> vert_z_list = new List<float>();
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
                vert_x_list[0] = vert_x_list.ElementAt(0) + 0.1f;
                vert_y_list[0] = vert_y_list.ElementAt(0) + 0.1f;
                vert_z_list[0] = vert_z_list.ElementAt(0) + 0.1f;
                importer_common.import_stats.collision_fix_count++;
            }
            if (vert_x_list.ElementAt(1) == vert_x_list.ElementAt(2) && vert_y_list.ElementAt(1) == vert_y_list.ElementAt(2) && vert_z_list.ElementAt(1) == vert_z_list.ElementAt(2))
            {
                vert_x_list[1] = vert_x_list.ElementAt(1) + 0.1f;
                vert_y_list[1] = vert_y_list.ElementAt(1) + 0.1f;
                vert_z_list[1] = vert_z_list.ElementAt(1) + 0.1f;
                importer_common.import_stats.collision_fix_count++;
            }
            if (vert_x_list.ElementAt(0) == vert_x_list.ElementAt(2) && vert_y_list.ElementAt(0) == vert_y_list.ElementAt(2) && vert_z_list.ElementAt(0) == vert_z_list.ElementAt(2))
            {
                vert_x_list[2] = vert_x_list.ElementAt(2) + 0.1f;
                vert_y_list[2] = vert_y_list.ElementAt(2) + 0.1f;
                vert_z_list[2] = vert_z_list.ElementAt(2) + 0.1f;
                importer_common.import_stats.collision_fix_count++;
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
            DialogResult areYouSure = MessageBox.Show("This process will auto-detect the best collision properties based on material metadata.\nThis will potentially overwrite existing configurations.\nAre you sure you wish to continue?", "Are you sure?", MessageBoxButtons.YesNo, MessageBoxIcon.Information);
            if (areYouSure != DialogResult.Yes)
            {
                return;
            }

            //Loop through every material and base on name if we are transparent or collision
            foreach (var this_material_config in model_material_config)
            {
                JToken this_token = model_material_config[this_material_config.Key];

                //All materials typically work best when we can see them :)
                this_token["Kd"] = "1.000000 1.000000 1.000000";

                //Materials starting with "ef_" are usually boost/antigrav/jump pads
                if (this_material_config.Key.ToUpper().Substring(0, 3) == "EF_GLIDEBOARD")
                {
                    setCollisionParam(CollisionType.JUMP_PAD, this_token);
                }
                if (this_material_config.Key.ToUpper().Substring(0, 3) == "EF_GRAVITYBOARD")
                {
                    setCollisionParam(CollisionType.ANTIGRAV_PAD, this_token);
                }
                if (this_material_config.Key.ToUpper().Substring(0, 3) == "EF_DASHBOARD")
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
            }
        }

        /* Delete old DDS materials and convert new ones */
        private void convertMaterials()
        {
            DirectoryInfo materials = new DirectoryInfo(importer_common.importDir());
            FileInfo[] file_array = materials.GetFiles();
            foreach (var file in file_array)
            {
                if (file.Extension.ToUpper() == ".DDS")
                {
                    //file.Delete();
                }
            }
            foreach (var file in file_array)
            {
                if (file.Extension.ToUpper() == ".JPG" || file.Extension.ToUpper() == ".PNG" || file.Extension.ToUpper() == ".JPEG")
                {
                    convertMaterial(Path.GetFileName(file.FullName));
                }
            }
        }

        private void convertMaterial(string filename)
        {
            ProcessStartInfo texConv = new ProcessStartInfo();
            texConv.WorkingDirectory = importer_common.importDir();
            texConv.FileName = common_functions.getFolder(AssetType.IMAGE) + "texconv.exe";
            texConv.Arguments = "\"" + filename + "\"";
            texConv.UseShellExecute = false;
            texConv.RedirectStandardOutput = true;
            texConv.CreateNoWindow = true;
            Process converterProcess = Process.Start(texConv);
            converterProcess.WaitForExit();
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
                _token["ThICC_COLLISION"][i.ToString()] = to_set;
            }
        }

        /* Delete a file if it exists (this is safer than just calling delete!) */
        private void deleteIfExists(string file)
        {
            if (File.Exists(file))
            {
                File.Delete(file);
            }
        }

        /* Add the ThICC file signature, this is used by the game to check we're reading an up-to-date file. */
        private void addThiccSignature(BinaryWriter writer)
        {
            writer.Write(common_functions.ThICC_FILE_VERSION);
            string signature = "ThICC";
            foreach (char character in signature)
            {
                writer.Write(character); //This verifies the legitimacy of the file.
            }
        }
    }
}
