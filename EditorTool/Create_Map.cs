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
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace EditorTool
{
    public partial class Create_Map : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();

        /* If editing, load data */
        JObject maps_json_config = null;
        string map_json_key = "";
        bool edit_mode = false;
        public Create_Map(JObject json_data = null, string object_key = "")
        {
            maps_json_config = json_data;
            map_json_key = object_key;
            edit_mode = (json_data != null && object_key != "");

            InitializeComponent();
        }

        /* On Load */
        private void Create_Map_Load(object sender, EventArgs e)
        {
            if (edit_mode)
            {
                // If in edit mode, load existing config
                mapName.Text = maps_json_config[map_json_key]["friendly_name"].Value<string>();
                mapPreviewImage.Text = maps_json_config[map_json_key]["menu_sprite"].Value<string>();
                mapModelAsset.Text = maps_json_config[map_json_key]["model"].Value<string>();
                soundtrackIntro.Text = maps_json_config[map_json_key]["audio"]["background_start"].Value<string>();
                soundtrackIntroLoop.Text = maps_json_config[map_json_key]["audio"]["background"].Value<string>();
                finalLapIntro.Text = maps_json_config[map_json_key]["audio"]["final_lap_start"].Value<string>();
                finalLapLoop.Text = maps_json_config[map_json_key]["audio"]["final_lap"].Value<string>();
            }
        }

        /* Launch asset selector */
        string selectAsset(AssetType asset_type, string existing_option = "")
        {
            string asset_path = "";
            using (var form = new Asset_Browser(asset_type, existing_option))
            {
                form.ShowDialog();
                asset_path = form.selected_file_path;
            }
            return asset_path;
        }

        /* Select assets */
        private void selectImageAsset_Click(object sender, EventArgs e)
        {
            mapPreviewImage.Text = selectAsset(AssetType.IMAGE, mapPreviewImage.Text);
        }
        private void selectMapModel_Click(object sender, EventArgs e)
        {
            mapModelAsset.Text = selectAsset(AssetType.MODEL, mapModelAsset.Text);
        }
        private void selectSoundtrackIntro_Click(object sender, EventArgs e)
        {
            soundtrackIntro.Text = selectAsset(AssetType.SOUND, soundtrackIntro.Text);
        }
        private void selectSoundtrackLoop_Click(object sender, EventArgs e)
        {
            soundtrackIntroLoop.Text = selectAsset(AssetType.SOUND, soundtrackIntroLoop.Text);
        }
        private void selectFinalLapIntro_Click(object sender, EventArgs e)
        {
            finalLapIntro.Text = selectAsset(AssetType.SOUND, finalLapIntro.Text);
        }
        private void selectFinalLapLoop_Click(object sender, EventArgs e)
        {
            finalLapLoop.Text = selectAsset(AssetType.SOUND, finalLapLoop.Text);
        }
        private void loadString_Click(object sender, EventArgs e)
        {
            mapName.Text = selectAsset(AssetType.STRING, mapName.Text);
        }

        /* Save map to config */
        private void saveMap_Click(object sender, EventArgs e)
        {
            //Remove old json config or load existing (depends on mode)
            if (edit_mode)
            {
                maps_json_config.Remove(map_json_key);
            }
            else
            {
                maps_json_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/MAP_CONFIG.JSON"));
            }

            string map_name = function_libary.getLocalisedString(mapName.Text);

            //Add map to config
            maps_json_config[map_name]["friendly_name"] = mapName.Text;
            maps_json_config[map_name]["menu_sprite"] = mapPreviewImage.Text;
            maps_json_config[map_name]["model"] = mapModelAsset.Text;
            maps_json_config[map_name]["audio"]["background_start"] = soundtrackIntro.Text;
            maps_json_config[map_name]["audio"]["background"] = soundtrackIntroLoop.Text;
            maps_json_config[map_name]["audio"]["final_lap_start"] = finalLapIntro.Text;
            maps_json_config[map_name]["audio"]["final_lap"] = finalLapLoop.Text;

            //Save back out
            File.WriteAllText("DATA/CONFIGS/MAP_CONFIG.JSON", maps_json_config.ToString(Formatting.Indented));
        }
    }
}
