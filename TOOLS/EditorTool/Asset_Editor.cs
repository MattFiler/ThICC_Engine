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
            //These should match up to the ModelType enum!
            modelType.Items.Add("Map");
            modelType.Items.Add("Item");
            modelType.Items.Add("Player");
            modelType.Items.Add("Prop");

            loadAssetType.SelectedIndex = (int)on_load;
        }

        /* When closing, clear any lingering stuff */
        private void Landing_Close(object sender, EventArgs e)
        {
            function_libary.closeLingeringSoundStreams();
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
                    /*
                    Model_Importer_Wrapper modelImporter = new Model_Importer_Wrapper();
                    modelImporter.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    modelImporter.Show();
                    */
                    using (var form = new Model_Importer_Preselect())
                    {
                        var result = form.ShowDialog();
                        if (result == DialogResult.OK)
                        {
                            Model_Importer_AssetSelector modelimporter = new Model_Importer_AssetSelector(form.selected_model_type);
                            modelimporter.FormClosed += new FormClosedEventHandler(refreshOnClose); //needs to use the wrapper, this closes too early
                            modelimporter.Show();
                        }
                    }
                    break;
                case "Images":
                case "Sounds":
                case "Fonts":
                    Generic_Importer multipurposeImporter = new Generic_Importer((AssetType)loadAssetType.SelectedIndex);
                    multipurposeImporter.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    multipurposeImporter.Show();
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
                    path = function_libary.getFolder(AssetType.MODEL);
                    extension = "*.SDKMESH";
                    break;
                case "Images":
                    path = function_libary.getFolder(AssetType.IMAGE);
                    extension = "*.DDS";
                    break;
                case "Sounds":
                    path = function_libary.getFolder(AssetType.SOUND);
                    extension = "*.WAV";
                    break;
                case "Fonts":
                    path = function_libary.getFolder(AssetType.FONT);
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
            
            //Get name
            string selected_file_name = "";
            if (loadAssetType.SelectedItem.ToString() != "Strings")
            {
                selected_file_name = full_loaded_filenames.ElementAt(assetList.SelectedIndex);
            }
            else
            {
                selected_file_name = assetList.SelectedItem.ToString();
            }

            //Get type
            AssetType selected_type;
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    selected_type = AssetType.MODEL;
                    break;
                case "Fonts":
                    selected_type = AssetType.FONT;
                    break;
                case "Images":
                    selected_type = AssetType.IMAGE;
                    break;
                case "Strings":
                    selected_type = AssetType.STRING;
                    break;
                default:
                    selected_type = AssetType.SOUND;
                    break;
            }

            //Check to see if asset is in use somewhere
            List<string> useages = function_libary.getUseages(selected_type, assetList.SelectedItem.ToString());
            if (useages.Count > 0)
            {
                string message = "Cannot delete! In use in: ";
                foreach (string use in useages)
                {
                    message += use + ", ";
                }
                message = message.Substring(0, message.Length - 2);
                MessageBox.Show(message + ".", "Asset is in use!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Clear any item preview so we can delete it
            assetList.SelectedIndex = -1; 
            function_libary.closeLingeringSoundStreams();

            //Confirmation
            DialogResult showErrorInfo = MessageBox.Show("Are you sure you want to delete this asset?", "About to delete selected asset...", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (showErrorInfo != DialogResult.Yes)
            {
                return;
            }

            //Delete selected asset
            switch (selected_type)
            {
                case AssetType.MODEL:
                    Directory.Delete(Path.GetDirectoryName(selected_file_name), true);
                    break;
                case AssetType.FONT:
                    File.Delete(selected_file_name);
                    try
                    {
                        File.Delete(selected_file_name.Substring(0, selected_file_name.Length - 10) + "JSON");
                        File.Delete(selected_file_name.Substring(0, selected_file_name.Length - 10) + "BMP");
                    }
                    catch { }
                    break;
                case AssetType.IMAGE:
                    string[] files = Directory.GetFiles(function_libary.getFolder(AssetType.IMAGE), "*", SearchOption.AllDirectories);
                    foreach (string file in files)
                    {
                        if (Path.GetFileNameWithoutExtension(file) == Path.GetFileNameWithoutExtension(selected_file_name))
                        {
                            try { File.Delete(file); } catch { }
                        }
                    }
                    break;
                case AssetType.STRING:
                    localisation_config["ENGLISH"][selected_file_name].Parent.Remove();
                    File.WriteAllText("DATA/CONFIGS/LOCALISATION.JSON", localisation_config.ToString(Formatting.Indented));
                    JObject inuse_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/LOCALISATION_INUSE.JSON"));
                    inuse_config[selected_file_name].Parent.Remove();
                    File.WriteAllText("DATA/CONFIGS/LOCALISATION_INUSE.JSON", inuse_config.ToString(Formatting.Indented));
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
            depreciationWarning.Visible = false;
            assetUseageGroup.Visible = false;

            //Get type
            AssetType selected_type;
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    selected_type = AssetType.MODEL;
                    break;
                case "Fonts":
                    selected_type = AssetType.FONT;
                    break;
                case "Images":
                    selected_type = AssetType.IMAGE;
                    break;
                case "Strings":
                    selected_type = AssetType.STRING;
                    break;
                case "Sounds":
                    selected_type = AssetType.SOUND;
                    break;
                default:
                    return;
            }

            //Act appropriately for selected asset type
            switch (selected_type)
            {
                case AssetType.MODEL:
                    if (function_libary.loadModelPreview(assetList, modelPreview))
                    {
                        //If preview loads properly, load config
                        getConfigPathForSelectedAsset();
                        JToken asset_json = JToken.Parse(File.ReadAllText(path_to_current_config));

                        if (asset_json["model_type"].Type != JTokenType.Integer)
                        {
                            //Old config, don't enable editing
                            modelConfigs.Visible = false;
                            depreciationWarning.Visible = true;
                            return;
                        }

                        //Fill up config
                        modelType.SelectedIndex = asset_json["model_type"].Value<int>();
                        model_world_x.Text = asset_json["start_x"].Value<string>();
                        model_world_y.Text = asset_json["start_y"].Value<string>();
                        model_world_z.Text = asset_json["start_z"].Value<string>();
                        model_rot_x.Text = asset_json["rot_x"].Value<string>();
                        model_rot_y.Text = asset_json["rot_y"].Value<string>();
                        model_rot_z.Text = asset_json["rot_z"].Value<string>();
                        model_scale.Text = asset_json["modelscale"].Value<string>();
                        model_segmentsize.Value = asset_json["segment_size"].Value<decimal>();

                        modelConfigs.Visible = true;
                    }
                    break;
                case AssetType.IMAGE:
                    function_libary.loadImagePreview(assetList, imagePreview);
                    break;
                case AssetType.SOUND:
                    function_libary.loadSoundPreview(assetList, sound_player, soundPreview, playSoundPreview);
                    break;
                case AssetType.FONT:
                    function_libary.loadFontPreview(assetList, imagePreview);
                    break;
                case AssetType.STRING:
                    function_libary.loadStringPreview(assetList, localisationPreview, localisation_config);
                    break;
                default:
                    return;
            }
            if (assetList.SelectedIndex == -1)
            {
                return;
            }

            //List useages
            assetUseages.Items.Clear();
            foreach (string useage in function_libary.getUseages(selected_type, assetList.SelectedItem.ToString()))
            {
                assetUseages.Items.Add(useage);
            }
            assetUseageGroup.Visible = true;

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
                case "Models":
                    //Create importer resource & configure paths
                    Model_Importer_Common common_importer = new Model_Importer_Common();
                    common_importer.configureAssetPaths(assetList.SelectedItem.ToString());

                    //Only continue if config is present
                    if (!File.Exists(common_importer.fileName(importer_file.IMPORTER_CONFIG)))
                    {
                        MessageBox.Show("Cannot edit this file.\nIt was imported with an older version of the toolkit.", "Error.", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    //Pull model type (this is forced)
                    JObject asset_json = JObject.Parse(File.ReadAllText(common_importer.fileName(importer_file.CONFIG)));
                    common_importer.setModelType((ModelType)asset_json["model_type"].Value<int>());

                    //Set as edit mode - this ignores the track config requirement
                    common_importer.setEditMode(true);

                    //Load editor
                    Model_Importer_MaterialList modelEditor = new Model_Importer_MaterialList(common_importer);
                    modelEditor.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    modelEditor.Show();
                    break;
                case "Strings":
                    Localisation_Editor stringEditor = new Localisation_Editor(assetList.SelectedItem.ToString());
                    stringEditor.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    stringEditor.Show();
                    break;
                default:
                    MessageBox.Show("This asset has no properties to edit.", "No properties to edit.", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
                    asset_json["model_type"] = modelType.SelectedIndex; //For this change to actually take effect, the model will need to be "edited" - improvement needed here
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
