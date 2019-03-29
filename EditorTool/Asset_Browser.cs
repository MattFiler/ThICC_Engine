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
    public partial class Asset_Browser : Form
    {
        List<string> full_loaded_filenames = new List<string>(); 
        string file_path = "";
        string file_extension = "";

        public string selected_file_path { get; set; }

        /* Configure asset list filtering */
        public Asset_Browser(AssetType type_to_select_from)
        {
            switch (type_to_select_from)
            {
                case AssetType.MODEL:
                    file_path = "DATA/MODELS/";
                    file_extension = "*.SDKMESH";
                    break;
                case AssetType.MESH:
                    file_path = "DATA/MODELS/";
                    file_extension = "*.TXTMESH";
                    break;
                case AssetType.IMAGE:
                    file_path = "DATA/IMAGES/";
                    file_extension = "*.DDS";
                    break;
                case AssetType.SOUND:
                    file_path = "DATA/SOUNDS/";
                    file_extension = "*.WAV";
                    break;
                case AssetType.FONT:
                    file_path = "DATA/FONTS/";
                    file_extension = "*.SPRITEFONT";
                    break;
                default:
                    this.Close();
                    break;
            }

            InitializeComponent();
        }

        /* Fetch asset list on form load */
        private void Asset_Browser_Load(object sender, EventArgs e)
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
            }
        }

        /* Select the highlighted asset */
        private void selectAsset_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select an asset from the list.");
                return;
            }
            this.DialogResult = DialogResult.OK;
            selected_file_path = assetList.SelectedItem.ToString();
            this.Close();
        }
    }
}
