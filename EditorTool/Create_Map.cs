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

        /* Select assets */
        private void selectImageAsset_Click(object sender, EventArgs e)
        {
            mapPreviewImage.Text = selectAsset(AssetType.IMAGE);
        }
        private void selectMapModel_Click(object sender, EventArgs e)
        {
            mapModelAsset.Text = selectAsset(AssetType.MODEL);
        }
        private void selectSoundtrackIntro_Click(object sender, EventArgs e)
        {
            soundtrackIntro.Text = selectAsset(AssetType.SOUND);
        }
        private void selectSoundtrackLoop_Click(object sender, EventArgs e)
        {
            soundtrackIntroLoop.Text = selectAsset(AssetType.SOUND);
        }
        private void selectFinalLapIntro_Click(object sender, EventArgs e)
        {
            finalLapIntro.Text = selectAsset(AssetType.SOUND);
        }
        private void selectFinalLapLoop_Click(object sender, EventArgs e)
        {
            finalLapLoop.Text = selectAsset(AssetType.SOUND);
        }
    }
}
