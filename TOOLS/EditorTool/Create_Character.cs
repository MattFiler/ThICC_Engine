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
    public partial class Create_Character : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();

        /* If editing, load data */
        JObject maps_json_config = null;
        string map_json_key = "";
        bool edit_mode = false;
        public Create_Character(JObject json_data = null, string object_key = "")
        {
            maps_json_config = json_data;
            map_json_key = object_key;
            edit_mode = (json_data != null && object_key != "");

            InitializeComponent();
        }

        /* On Load */
        private void Create_Character_Load(object sender, EventArgs e)
        {
            if (edit_mode)
            {
                // If in edit mode, load existing config
                mapCodename.Text = map_json_key;
                mapCodename.ReadOnly = true;
                mapName.Text = maps_json_config[map_json_key]["friendly_name"].Value<string>();
                mapPreviewImage.Text = maps_json_config[map_json_key]["menu_sprite"].Value<string>();
                mapModelAsset.Text = maps_json_config[map_json_key]["model"].Value<string>();
                characterSound.Text = maps_json_config[map_json_key]["audio"].Value<string>();
            }
        }
        
        /* Select assets */
        private void loadString_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(mapName, AssetType.STRING);
        }
        private void selectImageAsset_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(mapPreviewImage, AssetType.IMAGE);
        }
        private void selectMapModel_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(mapModelAsset, AssetType.MODEL);
        }
        private void loadSound_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(characterSound, AssetType.SOUND);
        }

        /* Save character to config */
        private void saveMap_Click(object sender, EventArgs e)
        {
            //All inputs required
            if (mapCodename.Text == "" || mapName.Text == "" || mapPreviewImage.Text == "" || mapModelAsset.Text == "" || characterSound.Text == "")
            {
                MessageBox.Show("Please complete all fields before trying to save.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update tags
            function_libary.executeUseageQueueForTag(function_libary.getUseageTagFor(AssetCompType.CHARACTER));

            //Remove old json config or load existing (depends on mode)
            if (edit_mode)
            {
                maps_json_config.Remove(map_json_key);
            }
            else
            {
                maps_json_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/CHARACTER_CONFIG.JSON"));
            }

            string map_name = mapCodename.Text;

            //Add to config
            maps_json_config[map_name] = JObject.Parse("{}");
            maps_json_config[map_name]["friendly_name"] = mapName.Text;
            maps_json_config[map_name]["menu_sprite"] = mapPreviewImage.Text;
            maps_json_config[map_name]["model"] = mapModelAsset.Text;
            maps_json_config[map_name]["audio"] = characterSound.Text;

            //Save back out
            File.WriteAllText("DATA/CONFIGS/CHARACTER_CONFIG.JSON", maps_json_config.ToString(Formatting.Indented));

            //Done
            MessageBox.Show("Saved character configuration!", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }

    }
}
