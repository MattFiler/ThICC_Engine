using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
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
    public partial class Create_Cup : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();

        /* If editing, load data */
        JObject maps_json_config = null;
        string map_json_key = "";
        bool edit_mode = false;
        public Create_Cup(JObject json_data = null, string object_key = "")
        {
            maps_json_config = json_data;
            map_json_key = object_key;
            edit_mode = (json_data != null && object_key != "");

            InitializeComponent();
        }

        /* On load */
        private void Create_Cup_Load(object sender, EventArgs e)
        {
            if (edit_mode)
            {
                // If in edit mode, load existing config
                cupCodename.Text = map_json_key;
                cupCodename.ReadOnly = true;
                cupName.Text = maps_json_config[map_json_key]["friendly_name"].Value<string>();
                cupPreviewImage.Text = maps_json_config[map_json_key]["menu_sprite"].Value<string>();
            }
        }

        /* Select assets */
        private void loadString_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(cupName, AssetType.STRING);
        }
        private void selectImageAsset_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(cupPreviewImage, AssetType.IMAGE);
        }

        /* Save to config */
        private void saveCup_Click(object sender, EventArgs e)
        {
            //All inputs required
            if (cupCodename.Text == "" || cupName.Text == "" || cupPreviewImage.Text == "")
            {
                MessageBox.Show("Please complete all fields before trying to save.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update tags
            function_libary.executeUseageQueueForTag(function_libary.getUseageTagFor(AssetCompType.CUP));

            //Remove old json config or load existing (depends on mode)
            if (edit_mode)
            {
                maps_json_config.Remove(map_json_key);
            }
            else
            {
                maps_json_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/CUP_CONFIG.JSON"));
            }

            string map_name = cupCodename.Text;

            //Add map to config
            maps_json_config[map_name] = JObject.Parse("{}");
            maps_json_config[map_name]["friendly_name"] = cupName.Text;
            maps_json_config[map_name]["menu_sprite"] = cupPreviewImage.Text;

            //Save back out
            File.WriteAllText("DATA/CONFIGS/CUP_CONFIG.JSON", maps_json_config.ToString(Formatting.Indented));

            //Done
            MessageBox.Show("Saved cup configuration!", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }
    }
}
