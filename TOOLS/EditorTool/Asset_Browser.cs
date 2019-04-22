using NAudio.Wave;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Asset_Browser : Form
    {
        List<string> full_loaded_filenames = new List<string>();
        AssetType file_type = AssetType.FONT;
        string file_path = "";
        string file_extension = "";
        string pre_selected_option = "";
        JObject localisation_config;
        int index_to_default_to = -1;

        SoundPlayer sound_player = new SoundPlayer();
        UsefulFunctions function_libary = new UsefulFunctions();

        public string selected_asset { get; set; }
        public string selected_file_path { get; set; }
        public JObject selected_file_config { get; set; }
        public string selected_file_config_path { get; set; }

        /* Configure asset list filtering */
        public Asset_Browser(AssetType type_to_select_from, string existing_option = "")
        {
            InitializeComponent();

            file_type = type_to_select_from;
            switch (type_to_select_from)
            {
                case AssetType.MODEL:
                    file_path = function_libary.getFolder(AssetType.MODEL);
                    file_extension = "*.SDKMESH";
                    break;
                case AssetType.IMAGE:
                    file_path = function_libary.getFolder(AssetType.IMAGE);
                    file_extension = "*.DDS";
                    break;
                case AssetType.SOUND:
                    file_path = function_libary.getFolder(AssetType.SOUND);
                    file_extension = "*.WAV";
                    break;
                case AssetType.FONT:
                    file_path = function_libary.getFolder(AssetType.FONT);
                    file_extension = "*.SPRITEFONT";
                    break;
                case AssetType.STRING:
                    localisation_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/LOCALISATION.JSON"));
                    foreach (var localised_string in localisation_config["ENGLISH"].ToObject<JObject>()) //Only working with English for now
                    {
                        assetList.Items.Add(localised_string.Key);

                        if (assetList.Items[assetList.Items.Count - 1].ToString() == pre_selected_option)
                        {
                            index_to_default_to = assetList.Items.Count - 1;
                        }
                    }
                    break;
                default:
                    this.Close();
                    break;
            }
            pre_selected_option = existing_option;
        }

        /* When closing, clear any lingering stuff */
        private void Asset_Browser_Close(object sender, EventArgs e)
        {
            function_libary.closeLingeringSoundStreams();
        }

        /* Fetch asset list on form load */
        private void Asset_Browser_Load(object sender, EventArgs e)
        {
            if (file_type != AssetType.STRING)
            {
                string[] files = Directory.GetFiles(file_path, file_extension, SearchOption.AllDirectories);
                full_loaded_filenames.Clear();
                assetList.Items.Clear();
                foreach (string file in files)
                {
                    if (Path.GetFileName(file).Length > 14 && Path.GetFileName(file).Substring(Path.GetFileName(file).Length - 13).ToUpper() == "DEBUG.SDKMESH")
                    {
                        continue; //Skip over collision debug meshes
                    }
                    full_loaded_filenames.Add(file);
                    assetList.Items.Add(Path.GetFileNameWithoutExtension(file));

                    if (assetList.Items[assetList.Items.Count - 1].ToString() == pre_selected_option)
                    {
                        index_to_default_to = assetList.Items.Count - 1;
                    }
                }
            }
            modelPreview.Visible = false;
            imagePreview.Visible = false;
            soundPreview.Visible = false;
            playSoundPreview.Visible = false;
            localisationPreview.Visible = false;

            assetList.SelectedIndex = index_to_default_to;
        }

        /* Select the highlighted asset */
        private void selectAsset_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select an asset from the list.");
                return;
            }
            selected_asset = assetList.SelectedItem.ToString();
            if (file_type == AssetType.STRING)
            {
                selected_file_path = "DATA/CONFIGS/LOCALISATION_INUSE.JSON";
            }
            else
            {
                selected_file_path = function_libary.getFolder(file_type, selected_asset) + selected_asset + function_libary.getExtension(file_type);
            }
            selected_file_config_path = selected_file_path.Substring(0, selected_file_path.Length - Path.GetExtension(selected_file_path).Length) + ".JSON";
            selected_file_config = JObject.Parse(File.ReadAllText(selected_file_config_path));
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        /* Load asset preview and set active variable to return */
        private void assetList_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (file_type)
            {
                case AssetType.SOUND:
                    function_libary.loadSoundPreview(assetList, sound_player, soundPreview, playSoundPreview);
                    break;
                case AssetType.IMAGE:
                    function_libary.loadImagePreview(assetList, imagePreview);
                    break;
                case AssetType.MODEL:
                    function_libary.loadModelPreview(assetList, modelPreview);
                    break;
                case AssetType.FONT:
                    function_libary.loadFontPreview(assetList, imagePreview);
                    break;
                case AssetType.STRING:
                    function_libary.loadStringPreview(assetList, localisationPreview, localisation_config);
                    break;
            }

            selected_file_path = assetList.SelectedItem.ToString();
        }

        /* Play sound preview */
        private void playSoundPreview_Click(object sender, EventArgs e)
        {
            function_libary.playSoundPreview(assetList, sound_player);
        }

        /* Open importer */
        private void createNew_Click(object sender, EventArgs e)
        {
            switch (file_type)
            {
                case AssetType.MODEL:
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
                case AssetType.SOUND:
                case AssetType.IMAGE:
                case AssetType.FONT:
                    Generic_Importer multipurposeImporter = new Generic_Importer(file_type);
                    multipurposeImporter.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    multipurposeImporter.Show();
                    break;
                case AssetType.STRING:
                    Localisation_Editor stringEditor = new Localisation_Editor();
                    stringEditor.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    stringEditor.Show();
                    break;
            }
        }

        /* Reopen when new thing imported */
        private void refreshOnClose(object sender, EventArgs e)
        {
            Asset_Browser new_browser = new Asset_Browser(file_type);
            new_browser.Show();
            this.Close();
        }
    }
}
