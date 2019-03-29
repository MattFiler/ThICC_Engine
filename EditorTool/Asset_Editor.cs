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
using System.Windows.Forms.Integration;
using System.Media;
using NAudio.Wave;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Xml.Linq;

namespace EditorTool
{
    public partial class Asset_Editor : Form
    {
        List<string> full_loaded_filenames = new List<string>(); //Used for saving full list item file paths
        SoundPlayer sound_player = new SoundPlayer(); //for previewing sounds
        UsefulFunctions function_libary = new UsefulFunctions();
        JObject localisation_config;
        string path_to_current_config = "";
        AssetType on_load = AssetType.MODEL;
        public Asset_Editor(AssetType type_to_load_to = AssetType.MODEL)
        {
            on_load = type_to_load_to;
            InitializeComponent();
        }

        /* ON LOAD */
        private void Landing_Load(object sender, EventArgs e)
        {
            loadAssetType.SelectedIndex = (int)on_load;
        }
        
        private void DEBUG_SAVE_Click(object sender, EventArgs e)
        {
        }

        /* LOAD ASSET LIST */
        private void loadAssetType_SelectedIndexChanged(object sender, EventArgs e)
        {
            refreshList();
        }
        
        /* IMPORT ASSET */
        private void refreshList_Click(object sender, EventArgs e)
        {
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    using (var form = new Model_Importer_Preselect())
                    {
                        var result = form.ShowDialog();
                        if (result == DialogResult.OK)
                        {
                            Model_Importer modelimporter = new Model_Importer(form.selected_model_type);
                            modelimporter.FormClosed += new FormClosedEventHandler(refreshOnClose);
                            modelimporter.Show();
                        }
                    }
                    break;
                case "Meshes":
                    MessageBox.Show("This functionality is coming soon.");
                    /*
                    Mesh_Creator meshcreator = new Mesh_Creator();
                    meshcreator.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    meshcreator.Show();
                    */
                    break;
                case "Images":
                    Image_Importer imageimporter = new Image_Importer();
                    imageimporter.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    imageimporter.Show();
                    break;
                case "Sounds":
                    Sound_Importer soundimporter = new Sound_Importer();
                    soundimporter.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    soundimporter.Show();
                    break;
                case "Fonts":
                    Font_Importer fontimporter = new Font_Importer();
                    fontimporter.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    fontimporter.Show();
                    break;
                case "Strings":
                    Localisation_Editor stringEditor = new Localisation_Editor();
                    stringEditor.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    stringEditor.Show();
                    break;
            }
        }

        /* REFRESH CURRENT LIST */
        void refreshList()
        {
            assetList.SelectedIndex = -1;

            full_loaded_filenames.Clear();
            assetList.Items.Clear();

            string path = "";
            string extension = "";
            bool should_go_further = true;
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    path = "DATA/MODELS/";
                    extension = "*.SDKMESH";
                    break;
                case "Meshes":
                    path = "DATA/MODELS/";
                    extension = "*.TXTMESH";
                    break;
                case "Images":
                    path = "DATA/IMAGES/";
                    extension = "*.DDS";
                    break;
                case "Sounds":
                    path = "DATA/SOUNDS/";
                    extension = "*.WAV";
                    break;
                case "Fonts":
                    path = "DATA/FONTS/";
                    extension = "*.SPRITEFONT";
                    break;
                case "Strings":
                    should_go_further = false;
                    localisation_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/LOCALISATION.JSON"));
                    foreach (var localised_string in localisation_config["ENGLISH"].ToObject<JObject>()) //Only working with English for now
                    {
                        assetList.Items.Add(localised_string.Key);
                    }
                    break;
            }

            if (should_go_further)
            {
                string[] files = Directory.GetFiles(path, extension, SearchOption.AllDirectories);
                foreach (string file in files)
                {
                    if (Path.GetFileName(file).Length > 14 && Path.GetFileName(file).Substring(Path.GetFileName(file).Length - 13).ToUpper() == "DEBUG.SDKMESH")
                    {
                        continue; //Skip over collision debug meshes
                    }
                    full_loaded_filenames.Add(file);
                    assetList.Items.Add(Path.GetFileNameWithoutExtension(file));
                }
            }
        }

        /* REFRESH LIST WHEN IMPORTER CLOSED */
        private void refreshOnClose(object sender, EventArgs e)
        {
            refreshList();
        }

        //Try generate a config path for our selected asset - stored in global path_to_current_config (eek)
        void getConfigPathForSelectedAsset()
        {
            try
            {
                string asset_config_path = full_loaded_filenames.ElementAt(assetList.SelectedIndex);
                asset_config_path = asset_config_path.Substring(0, asset_config_path.Length - Path.GetExtension(asset_config_path).Length);
                asset_config_path += ".JSON";
                path_to_current_config = asset_config_path;
            }
            catch
            {
                path_to_current_config = "";
            }
        }

        /* DELETE ASSET */
        private void deleteAsset_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select an asset to delete!", "No asset selected!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string selected_file_name = "";
            if (loadAssetType.SelectedItem.ToString() != "Strings")
            {
                selected_file_name = full_loaded_filenames.ElementAt(assetList.SelectedIndex);
            }
            else
            {
                selected_file_name = assetList.SelectedItem.ToString();
            }
            assetList.SelectedIndex = -1; //Clear any item preview so we can delete it
            function_libary.closeLingeringSoundStreams();

            DialogResult showErrorInfo = MessageBox.Show("Are you sure you want to delete this asset?", "About to delete selected asset...", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (showErrorInfo != DialogResult.Yes)
            {
                return;
            }

            //Delete selected asset
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                case "Meshes":
                    Directory.Delete(Path.GetDirectoryName(selected_file_name), true);
                    break;
                case "Fonts":
                    File.Delete(selected_file_name);
                    try
                    {
                        File.Delete(selected_file_name.Substring(0, selected_file_name.Length - 10) + "JSON");
                        File.Delete(selected_file_name.Substring(0, selected_file_name.Length - 10) + "BMP");
                    }
                    catch { }
                    break;
                case "Images":
                    string[] files = Directory.GetFiles("DATA/IMAGES/", "*", SearchOption.AllDirectories);
                    foreach (string file in files)
                    {
                        if (Path.GetFileNameWithoutExtension(file) == Path.GetFileNameWithoutExtension(selected_file_name))
                        {
                            try { File.Delete(file); } catch { }
                        }
                    }
                    break;
                case "Strings":
                    localisation_config["ENGLISH"][selected_file_name].Parent.Remove();
                    File.WriteAllText("DATA/CONFIGS/LOCALISATION.JSON", localisation_config.ToString(Formatting.Indented));
                    break;
                default:
                    File.Delete(selected_file_name);
                    File.Delete(selected_file_name.Substring(0, selected_file_name.Length - 3) + "JSON");
                    break;
            }
            refreshList();
            MessageBox.Show("Asset successfully deleted.", "Asset deleted!", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        
        private void compileAssets_Click(object sender, EventArgs e)
        {
        }

        /* LOAD ASSET PREVIEW & CONFIG ON CLICK */
        private void assetList_SelectedIndexChanged(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;

            //Hide all possible previewers
            modelPreview.Visible = false;
            imagePreview.Visible = false;
            soundPreview.Visible = false;
            playSoundPreview.Visible = false;
            localisationPreview.Visible = false;
            localisationPreview.Text = "";
            sound_player.Stop();

            //Hide all configs
            modelConfigs.Visible = false;

            //Act appropriately for selected asset type
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    if (function_libary.loadModelPreview(assetList, modelPreview))
                    {
                        //If preview loads properly, load config
                        getConfigPathForSelectedAsset();
                        JToken asset_json = JToken.Parse(File.ReadAllText(path_to_current_config));
                        modelType.SelectedItem = asset_json["model_type"].Value<string>();
                        model_world_x.Text = asset_json["start_x"].Value<string>();
                        model_world_y.Text = asset_json["start_y"].Value<string>();
                        model_world_z.Text = asset_json["start_z"].Value<string>();
                        model_rot_x.Text = asset_json["rot_x"].Value<string>();
                        model_rot_y.Text = asset_json["rot_y"].Value<string>();
                        model_rot_z.Text = asset_json["rot_z"].Value<string>();
                        model_scale.Text = asset_json["modelscale"].Value<string>();
                        model_segmentsize.Value = asset_json["segment_size"].Value<decimal>();
                    }
                    break;
                case "Meshes":
                    //WIP
                    break;
                case "Images":
                    function_libary.loadImagePreview(assetList, imagePreview);
                    break;
                case "Sounds":
                    function_libary.loadSoundPreview(assetList, sound_player, soundPreview, playSoundPreview);
                    break;
                case "Fonts":
                    function_libary.loadFontPreview(assetList, imagePreview);
                    break;
                case "Strings":
                    function_libary.loadStringPreview(assetList, localisationPreview, localisation_config);
                    break;
            }
            Cursor.Current = Cursors.Default;
        }

        /* Edit selected asset */
        private void editAsset_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select an asset to edit!", "No asset selected!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            
            switch (loadAssetType.SelectedItem)
            {
                case "Strings":
                    Localisation_Editor stringEditor = new Localisation_Editor(assetList.SelectedItem.ToString());
                    stringEditor.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    stringEditor.Show();
                    break;
                default:
                    break;
            }
        }

        //Play sound when requested
        private void playSoundPreview_Click(object sender, EventArgs e)
        {
            function_libary.playSoundPreview(assetList, sound_player);
        }

        /* SAVE CONFIG FOR ASSET */
        private void saveAssetConfig_Click(object sender, EventArgs e)
        {
            if (path_to_current_config == "")
            {
                //Should never get here
                MessageBox.Show("Asset config could not be read!\nHave you selected another asset?", "No asset selected!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    JToken asset_json = JToken.Parse(File.ReadAllText(path_to_current_config));
                    asset_json["model_type"] = modelType.SelectedItem.ToString();
                    asset_json["start_x"] = Convert.ToDouble(model_world_x.Text);
                    asset_json["start_y"] = Convert.ToDouble(model_world_y.Text);
                    asset_json["start_z"] = Convert.ToDouble(model_world_z.Text);
                    asset_json["rot_x"] = Convert.ToDouble(model_rot_x.Text);
                    asset_json["rot_y"] = Convert.ToDouble(model_rot_y.Text);
                    asset_json["rot_z"] = Convert.ToDouble(model_rot_z.Text);
                    asset_json["modelscale"] = Convert.ToDouble(model_scale.Text);
                    asset_json["segment_size"] = Convert.ToDouble(model_segmentsize.Value);
                    File.WriteAllText(path_to_current_config, asset_json.ToString(Formatting.Indented));
                    MessageBox.Show("Configuration saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    break;
                default:
                    //Should never get here
                    MessageBox.Show("There are no properties to edit for the selected asset.", "Nothing to edit.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    break;
            }
        }

        //Force numeric in config inputs
        private void model_world_x_TextChanged(object sender, KeyPressEventArgs e)
        {
            forceNumeric(e);
        }
        private void model_world_y_TextChanged(object sender, KeyPressEventArgs e)
        {
            forceNumeric(e);
        }
        private void model_world_z_TextChanged(object sender, KeyPressEventArgs e)
        {
            forceNumeric(e);
        }
        private void model_rot_x_TextChanged(object sender, KeyPressEventArgs e)
        {
            forceNumeric(e);
        }
        private void model_rot_y_TextChanged(object sender, KeyPressEventArgs e)
        {
            forceNumeric(e);
        }
        private void model_rot_z_TextChanged(object sender, KeyPressEventArgs e)
        {
            forceNumeric(e);
        }
        void forceNumeric(KeyPressEventArgs e)
        {
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar) && e.KeyChar != '.' && e.KeyChar != '-')
            {
                e.Handled = true;
            }
        }
        
        private void openKeybindEditor_Click(object sender, EventArgs e)
        {
        }
        
        private void openUiEditor_Click(object sender, EventArgs e)
        {
        }
        
        private void openItemConfig_Click(object sender, EventArgs e)
        {
        }

        // Open localiation editor
        private void openLocalisationEditor_Click(object sender, EventArgs e)
        {
            Localisation_Editor localisationeditor = new Localisation_Editor();
            localisationeditor.Show();
        }

        private void importModel_Click(object sender, EventArgs e)
        {
            //depreciated
        }
        private void importSound_Click(object sender, EventArgs e)
        {
            //depreciated
        }
        private void importImage_Click(object sender, EventArgs e)
        {
            //depreciated
        }
        private void importFont_Click(object sender, EventArgs e)
        {
            //depreciated
        }
        private void editSelected_Click(object sender, EventArgs e)
        {
            //depreciated
        }
    }
}
