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

namespace EditorTool
{
    public partial class Landing : Form
    {
        List<string> fullLoadedFileNames = new List<string>(); //Used for saving full list item file paths
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
            fullLoadedFileNames.Clear();
            assetList.Items.Clear();
            foreach (string file in files)
            {
                fullLoadedFileNames.Add(file);
                assetList.Items.Add(Path.GetFileNameWithoutExtension(file));
            }
        }

        /* REFRESH LIST WHEN IMPORTER CLOSED */
        private void refreshOnClose(object sender, EventArgs e)
        {
            refreshList();
        }

        /* EDIT ASSET CONFIG */
        private void editSelected_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select an asset to edit!", "No asset selected!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            MessageBox.Show(fullLoadedFileNames.ElementAt(assetList.SelectedIndex));
        }

        /* DELETE ASSET */
        private void deleteAsset_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select an asset to delete!", "No asset selected!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            DialogResult showErrorInfo = MessageBox.Show("Are you sure you want to delete this asset?", "About to delete selected asset...", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (showErrorInfo != DialogResult.Yes)
            {
                return;
            }

            //Delete selected asset
            string selected_file_name = fullLoadedFileNames.ElementAt(assetList.SelectedIndex);
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    string[] files = Directory.GetFiles(Path.GetDirectoryName(selected_file_name), "*");
                    foreach (string file in files)
                    {
                        File.Delete(file);
                    }
                    Directory.Delete(Path.GetDirectoryName(selected_file_name));
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
            DialogResult showErrorInfo = MessageBox.Show("Is the game running?", "About to compile assets...", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (showErrorInfo != DialogResult.No)
            {
                MessageBox.Show("The game must not be running when compiling assets.", "Please close game.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

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
            ignored_extensions.Add(".exe");
            ignored_extensions.Add(".obj");
            ignored_extensions.Add(".bmp");
            DirectoryCopy("DATA/", output_directory, true, ignored_extensions);
        }


        private void assetList_SelectedIndexChanged(object sender, EventArgs e)
        {

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
                    if (file.Extension == ignored_extension)
                    {
                        should_copy = false;
                        break;
                    }
                }
                if (should_copy)
                {
                    string temppath = Path.Combine(destDirName, file.Name);
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
    }
}
