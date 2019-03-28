using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Create_Map : Form
    {
        public Create_Map()
        {
            InitializeComponent();
        }

        /* Select a new map image */
        private void selectImageAsset_Click(object sender, EventArgs e)
        {
            mapPreviewImage.Text = selectAsset(AssetType.IMAGE);
        }

        /* Launch asset selector */
        string selectAsset(AssetType asset_type)
        {
            string asset_path = "";
            using (var form = new Asset_Browser(asset_type))
            {
                var result = form.ShowDialog();
                if (result == DialogResult.OK)
                {
                    asset_path = form.selected_file_path;
                }
            }
            return asset_path;
        }
    }
}
