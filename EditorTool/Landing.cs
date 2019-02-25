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
        public Landing()
        {
            InitializeComponent();
        }

        private void importModel_Click(object sender, EventArgs e)
        {
            Model_Importer modelimporter = new Model_Importer();
            modelimporter.Show();
        }

        private void importSound_Click(object sender, EventArgs e)
        {
            Sound_Importer soundimporter = new Sound_Importer();
            soundimporter.Show();
        }

        private void importImage_Click(object sender, EventArgs e)
        {
            Image_Importer imageimporter = new Image_Importer();
            imageimporter.Show();
        }

        private void importFont_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Coming soon!", "Coming soon!", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void loadAssetType_SelectedIndexChanged(object sender, EventArgs e)
        {
            reloadAssetList();
        }

        void reloadAssetList()
        {
            string path = "";
            string extension = "";
            switch (loadAssetType.SelectedItem)
            {
                case "Models":
                    path = "Models/";
                    extension = "*.sdkmesh";
                    break;
                case "Images":
                    path = "DDS/";
                    extension = "*.dds";
                    break;
                case "Sounds":
                    path = "Sounds/";
                    extension = "*.wav";
                    break;
                default:
                    MessageBox.Show("Coming soon!", "Coming soon!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    break;
            }

            string[] files = Directory.GetFiles(path, extension);
            assetList.Items.Clear();
            foreach (string file in files)
            {
                assetList.Items.Add(file);
            }
        }

        private void Landing_Load(object sender, EventArgs e)
        {
            loadAssetType.SelectedIndex = 0;
        }

        private void refreshList_Click(object sender, EventArgs e)
        {
            reloadAssetList();
        }

        private void editSelected_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Coming soon!", "Coming soon!", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void deleteAsset_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Coming soon!", "Coming soon!", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}
