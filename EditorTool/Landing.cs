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
                default:
                    MessageBox.Show("Coming soon!", "Coming soon!", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                default:
                    MessageBox.Show("Coming soon!", "Coming soon!", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                default:
                    File.Delete(selected_file_name);
                    File.Delete(selected_file_name.Substring(0, selected_file_name.Length - 3) + "json");
                    break;
            }
            refreshList();
            MessageBox.Show("Asset successfully deleted.", "Asset deleted!", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void assetList_SelectedIndexChanged(object sender, EventArgs e)
        {

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
