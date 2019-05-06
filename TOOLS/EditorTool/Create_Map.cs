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
        bool hide_popup = false;
        public Create_Map(JObject json_data = null, string object_key = "")
        {
            maps_json_config = json_data;
            map_json_key = object_key;
            edit_mode = (json_data != null && object_key != "");

            InitializeComponent();
            
            //Populate cups
            foreach (var config_entry in JObject.Parse(File.ReadAllText("DATA/CONFIGS/CUP_CONFIG.JSON")))
            {
                mapCup.Items.Add(config_entry.Key.ToString());
            }
        }

        /* On Load */
        private void Create_Map_Load(object sender, EventArgs e)
        {
            if (edit_mode)
            {
                hide_popup = maps_json_config[map_json_key]["arcade_only"].Value<bool>();

                // If in edit mode, load existing config
                mapCodename.Text = map_json_key;
                mapCodename.ReadOnly = true;
                mapName.Text = maps_json_config[map_json_key]["friendly_name"].Value<string>();
                mapPreviewImage.Text = maps_json_config[map_json_key]["menu_sprite"].Value<string>();
                mapModelAsset.Text = maps_json_config[map_json_key]["model"].Value<string>();
                decorationModel.Text = maps_json_config[map_json_key]["decoration_model"].Value<string>();
                cubemapChoice.Text = maps_json_config[map_json_key]["cubemap"].Value<string>();
                skyboxChoice.Text = maps_json_config[map_json_key]["skybox"].Value<string>();
                mapCup.SelectedItem = maps_json_config[map_json_key]["cup"].Value<string>();
                isArcadeOnly.Checked = maps_json_config[map_json_key]["arcade_only"].Value<bool>();
                isOnPC.Checked = !isArcadeOnly.Checked;
                soundtrackIntro.Text = maps_json_config[map_json_key]["audio"]["background_start"].Value<string>();
                soundtrackIntroLoop.Text = maps_json_config[map_json_key]["audio"]["background"].Value<string>();
                finalLapIntro.Text = maps_json_config[map_json_key]["audio"]["final_lap_start"].Value<string>();
                finalLapLoop.Text = maps_json_config[map_json_key]["audio"]["final_lap"].Value<string>();
            }
        }

        /* Select assets */
        private void selectImageAsset_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(mapPreviewImage, AssetType.IMAGE);
        }
        private void selectMapModel_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(mapModelAsset, AssetType.MODEL);
        }
        private void loadDecorationModel_Click(object sender, EventArgs e)
        {
            //This is a somewhat hacky fix for the out-of-memory exception when loading large maps.
            //For the degree show, this will be refactored massively with an update to the model importer supporting auto submesh generation.
            //Ran out of time to do that for the deadline however, so this works good enough to get larger maps imported.
            function_libary.assetSelectHandler(decorationModel, AssetType.MODEL);
        }
        private void loadRadiance_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(cubemapChoice, AssetType.CUBEMAP);
        }
        private void loadSkybox_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(skyboxChoice, AssetType.IMAGE);
        }
        private void selectSoundtrackIntro_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(soundtrackIntro, AssetType.SOUND);
        }
        private void selectSoundtrackLoop_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(soundtrackIntroLoop, AssetType.SOUND);
        }
        private void selectFinalLapIntro_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(finalLapIntro, AssetType.SOUND);
        }
        private void selectFinalLapLoop_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(finalLapLoop, AssetType.SOUND);
        }
        private void loadString_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(mapName, AssetType.STRING);
        }

        /* Make map exclusive to Arcade */
        private void isArcadeOnly_CheckedChanged(object sender, EventArgs e)
        {
            if (isArcadeOnly.Checked)
            {
                isOnPC.Checked = false;
                if (!hide_popup)
                {
                    DialogResult showErrorInfo = MessageBox.Show("This option will make the map arcade exclusive.\nExclusivity is a potentially destructive action.\nAre you sure this map should be exclusive to the arcade?", "Are you sure?", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    if (showErrorInfo == DialogResult.Yes)
                    {
                        return;
                    }
                    isArcadeOnly.Checked = false;
                    isOnPC.Checked = true;
                }
                hide_popup = false;
            }
        }
        private void isOnPC_CheckedChanged(object sender, EventArgs e)
        {
            isArcadeOnly.Checked = !isOnPC.Checked;
        }

        /* Save map to config */
        private void saveMap_Click(object sender, EventArgs e)
        {
            //All inputs required
            if (mapCodename.Text == "" || mapName.Text == "" || mapPreviewImage.Text == "" || 
                mapModelAsset.Text == "" || cubemapChoice.Text == "" || decorationModel.Text == "" ||
                soundtrackIntro.Text == "" || soundtrackIntroLoop.Text == "" || 
                finalLapIntro.Text == "" || finalLapLoop.Text == "" || mapCup.SelectedIndex == -1)
            {
                MessageBox.Show("Please complete all fields before trying to save.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update tags
            function_libary.executeUseageQueueForTag(function_libary.getUseageTagFor(AssetCompType.MAP));

            //Load existing config if not editing (would already be loaded)
            if (!edit_mode)
            {
                maps_json_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/MAP_CONFIG.JSON"));
            }

            string map_name = mapCodename.Text;

            //Add map to config
            if (!edit_mode) { maps_json_config[map_name] = JObject.Parse("{}"); }
            maps_json_config[map_name]["friendly_name"] = mapName.Text;
            maps_json_config[map_name]["menu_sprite"] = mapPreviewImage.Text;
            maps_json_config[map_name]["model"] = mapModelAsset.Text;
            maps_json_config[map_name]["decoration_model"] = decorationModel.Text;
            maps_json_config[map_name]["cubemap"] = cubemapChoice.Text;
            maps_json_config[map_name]["skybox"] = skyboxChoice.Text;
            maps_json_config[map_name]["cup"] = mapCup.SelectedItem.ToString();
            maps_json_config[map_name]["arcade_only"] = isArcadeOnly.Checked;
            maps_json_config[map_name]["audio"] = JObject.Parse("{}");
            maps_json_config[map_name]["audio"]["background_start"] = soundtrackIntro.Text;
            maps_json_config[map_name]["audio"]["background"] = soundtrackIntroLoop.Text;
            maps_json_config[map_name]["audio"]["final_lap_start"] = finalLapIntro.Text;
            maps_json_config[map_name]["audio"]["final_lap"] = finalLapLoop.Text;

            //Save back out
            File.WriteAllText("DATA/CONFIGS/MAP_CONFIG.JSON", maps_json_config.ToString(Formatting.Indented));

            //Done
            MessageBox.Show("Saved map configuration!", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }



        private void loadIrradiance_Click(object sender, EventArgs e)
        {
            //depreciated
        }
    }
}
