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
    public partial class Landing : Form
    {
        List<string> full_loaded_filenames = new List<string>(); //Used for saving full list item file paths
        SoundPlayer sound_player = new SoundPlayer(); //for previewing sounds
        string path_to_current_config = "";
        public Landing()
        {
            InitializeComponent();
        }

        /* ON LOAD */
        private void Landing_Load(object sender, EventArgs e)
        {
            loadAssetType.SelectedIndex = 0;
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
                    Model_Importer modelimporter = new Model_Importer();
                    modelimporter.FormClosed += new FormClosedEventHandler(refreshOnClose);
                    modelimporter.Show();
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
            }
        }

        /* REFRESH CURRENT LIST */
        void refreshList()
        {
            assetList.SelectedIndex = -1;

            string path = "";
            string extension = "";
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    path = "DATA/MODELS/";
                    extension = "*.SDKMESH";
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
            }

            string[] files = Directory.GetFiles(path, extension, SearchOption.AllDirectories);
            full_loaded_filenames.Clear();
            assetList.Items.Clear();
            foreach (string file in files)
            {
                full_loaded_filenames.Add(file);
                assetList.Items.Add(Path.GetFileNameWithoutExtension(file));
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

            string selected_file_name = full_loaded_filenames.ElementAt(assetList.SelectedIndex);
            assetList.SelectedIndex = -1; //Clear any item preview so we can delete it
            closeSoundStream();

            DialogResult showErrorInfo = MessageBox.Show("Are you sure you want to delete this asset?", "About to delete selected asset...", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (showErrorInfo != DialogResult.Yes)
            {
                return;
            }

            //Delete selected asset
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
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
                default:
                    File.Delete(selected_file_name);
                    File.Delete(selected_file_name.Substring(0, selected_file_name.Length - 3) + "JSON");
                    break;
            }
            refreshList();
            MessageBox.Show("Asset successfully deleted.", "Asset deleted!", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /* COMPILE ASSETS TO BUILD FOLDER */
        private void compileAssets_Click(object sender, EventArgs e)
        {
            try
            {
                //Fix VS debugging directory config
                File.WriteAllText("Scarle2019/Scarle2019.vcxproj.user", "<?xml version=\"1.0\" encoding=\"utf-8\"?><Project ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\"><LocalDebuggerWorkingDirectory>$(SolutionDir)$(Configuration)\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor></PropertyGroup><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\"><LocalDebuggerWorkingDirectory>$(SolutionDir)$(Configuration)\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor></PropertyGroup></Project>");

                //Copy to debug folder
                if (Directory.Exists("Debug"))
                {
                    copyAssets("Debug/DATA/");
                }

                //Copy to release folder
                if (Directory.Exists("Release"))
                {
                    copyAssets("Release/DATA/");
                }
                
                MessageBox.Show("Assets successfully compiled.", "Compiled assets.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch
            {
                MessageBox.Show("An error occured while compiling assets.\nMake sure that the game is closed and no files are open.", "Asset compile failed.", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /* COPY ALL ASSETS TO FOLDER */
        void copyAssets(string output_directory)
        {
            if (Directory.Exists(output_directory))
            {
                Directory.Delete(output_directory, true);
            }
            List<string> ignored_extensions = new List<string>();
            ignored_extensions.Add(".vcxproj");
            ignored_extensions.Add(".filters");
            ignored_extensions.Add(".user");
            ignored_extensions.Add(".exe");
            ignored_extensions.Add(".obj");
            ignored_extensions.Add(".mtl");
            ignored_extensions.Add(".bmp");
            DirectoryCopy("DATA/", output_directory, true, ignored_extensions);
        }

        /* LOAD ASSET PREVIEW & CONFIG ON CLICK */
        private void assetList_SelectedIndexChanged(object sender, EventArgs e)
        {
            //Hide all possible previewers
            modelPreview.Visible = false;
            imagePreview.Visible = false;
            soundPreview.Visible = false;
            playSoundPreview.Visible = false;
            sound_player.Stop();

            //Hide all configs
            modelConfigs.Visible = false;

            //Act appropriately for selected asset type
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    if (assetList.SelectedIndex == -1)
                    {
                        modelPreview.Child = new ModelViewer("");
                        break;
                    }
                    modelConfigs.Visible = true;

                    getConfigPathForSelectedAsset();
                    JToken asset_json = JToken.Parse(File.ReadAllText(path_to_current_config));
                    model_world_x.Text = asset_json["start_x"].Value<string>();
                    model_world_y.Text = asset_json["start_y"].Value<string>();
                    model_world_z.Text = asset_json["start_z"].Value<string>();
                    model_rot_x.Text = asset_json["rot_x"].Value<string>();
                    model_rot_y.Text = asset_json["rot_y"].Value<string>();
                    model_rot_z.Text = asset_json["rot_z"].Value<string>();
                    model_scale.Text = asset_json["modelscale"].Value<string>();

                    modelPreview.Visible = true;
                    modelPreview.Child = new ModelViewer("DATA/MODELS/" + assetList.SelectedItem.ToString() + "/" + assetList.SelectedItem.ToString() + ".OBJ");
                    break;
                case "Images":
                    if (assetList.SelectedIndex == -1)
                    {
                        imagePreview.Image = null;
                        break;
                    }
                    imagePreview.Visible = true;

                    string file_path_without_extension = "DATA/IMAGES/" + assetList.SelectedItem.ToString() + ".";
                    string file_path_with_extension = "";
                    if (File.Exists(file_path_without_extension + "PNG"))
                    {
                        file_path_with_extension = file_path_without_extension + "PNG";
                    }
                    else if (File.Exists(file_path_without_extension + "JPG"))
                    {
                        file_path_with_extension = file_path_without_extension + "JPG";
                    }
                    else if (File.Exists(file_path_without_extension + "JPEG"))
                    {
                        file_path_with_extension = file_path_without_extension + "JPEG";
                    }
                    else
                    {
                        break;
                    }

                    using (var tempPreviewImg = new Bitmap(file_path_with_extension))
                    {
                        imagePreview.Image = new Bitmap(tempPreviewImg);
                    }
                    break;
                case "Sounds":
                    if (assetList.SelectedIndex == -1)
                    {
                        closeSoundStream();
                        soundPreview.WaveStream = null;
                        sound_player.Stream = null;
                        break;
                    }
                    playSoundPreview.Visible = true;
                    soundPreview.Visible = true;
                    openSoundStream();
                    soundPreview.WaveStream = new WaveFileReader(sound_stream);
                    soundPreview.SamplesPerPixel = 150;
                    break;
                case "Fonts":
                    if (assetList.SelectedIndex == -1)
                    {
                        imagePreview.Image = null;
                        break;
                    }
                    imagePreview.Visible = true;

                    using (var tempPreviewImg = new Bitmap("DATA/FONTS/" + assetList.SelectedItem.ToString() + ".BMP"))
                    {
                        imagePreview.Image = new Bitmap(tempPreviewImg);
                    }
                    break;
            }
        }

        //Play sound when requested
        private void playSoundPreview_Click(object sender, EventArgs e)
        {
            openSoundStream();
            sound_player.Stream = sound_stream;
            try { sound_player.Play(); }
            catch { MessageBox.Show("An error ocurred while trying to play this sound - it may have imported incorrectly.", "Error.", MessageBoxButtons.OK, MessageBoxIcon.Error); }
        }

        //Update sound stream to use for any previews
        Stream sound_stream;
        void openSoundStream()
        {
            closeSoundStream();
            sound_stream = File.Open("DATA/SOUNDS/" + assetList.SelectedItem.ToString() + ".WAV", FileMode.Open, FileAccess.Read);
        }
        void closeSoundStream()
        {
            if(sound_stream != null)
            {
                sound_stream.Close();
            }
            sound_stream = null;
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
                    asset_json["start_x"] = Convert.ToDouble(model_world_x.Text);
                    asset_json["start_y"] = Convert.ToDouble(model_world_y.Text);
                    asset_json["start_z"] = Convert.ToDouble(model_world_z.Text);
                    asset_json["rot_x"] = Convert.ToDouble(model_rot_x.Text);
                    asset_json["rot_y"] = Convert.ToDouble(model_rot_y.Text);
                    asset_json["rot_z"] = Convert.ToDouble(model_rot_z.Text);
                    asset_json["modelscale"] = Convert.ToDouble(model_scale.Text);
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

        //Modified from: https://docs.microsoft.com/en-us/dotnet/standard/io/how-to-copy-directories
        private static void DirectoryCopy(string sourceDirName, string destDirName, bool copySubDirs, List<string> ignoreExtensions)
        {
            // Get the subdirectories for the specified directory.
            DirectoryInfo dir = new DirectoryInfo(sourceDirName);

            if (!dir.Exists)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + sourceDirName);
            }

            DirectoryInfo[] dirs = dir.GetDirectories();
            // If the destination directory doesn't exist, create it.
            if (!Directory.Exists(destDirName))
            {
                Directory.CreateDirectory(destDirName);
            }

            // Get the files in the directory and copy them to the new location.
            FileInfo[] files = dir.GetFiles();
            foreach (FileInfo file in files)
            {
                bool should_copy = true;
                foreach (string ignored_extension in ignoreExtensions)
                {
                    if (file.Extension.ToUpper() == ignored_extension.ToUpper())
                    {
                        should_copy = false;
                        break;
                    }
                }
                if (should_copy)
                {
                    string temppath = Path.Combine(destDirName, file.Name);
                    if (Path.GetFileName(Path.GetDirectoryName(temppath)).ToUpper() == "IMAGES" && (
                        Path.GetExtension(temppath).ToUpper() == ".JPG" || 
                        Path.GetExtension(temppath).ToUpper() == ".JPEG" || 
                        Path.GetExtension(temppath).ToUpper() == ".PNG"))
                    {
                        //A little manual override to stop original images in the IMAGES folder
                        continue;
                    }
                    file.CopyTo(temppath, false);
                }
            }

            // If copying subdirectories, copy them and their contents to new location.
            if (copySubDirs)
            {
                foreach (DirectoryInfo subdir in dirs)
                {
                    string temppath = Path.Combine(destDirName, subdir.Name);
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs, ignoreExtensions);
                }
            }
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
