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
    public partial class Asset_Comp_Manager : Form
    {
        UsefulFunctions common_functions = new UsefulFunctions();
        JObject comp_json_config;
        AssetCompType this_comp_type;
        string comp_displayname = "";
        public Asset_Comp_Manager(AssetCompType comp_type)
        {
            //Dynamic comp-specific stuff
            this_comp_type = comp_type;
            switch (this_comp_type)
            {
                case AssetCompType.CHARACTER:
                    comp_displayname = "character";
                    break;
                case AssetCompType.MAP:
                    comp_displayname = "map";
                    break;
                case AssetCompType.VEHICLE:
                    comp_displayname = "vehicle";
                    break;
            }

            InitializeComponent();

            //Populate dynamic elements text
            this.Text = "ThICC " + common_functions.makeCapitalised(comp_displayname) + " Manager";
            newMap.Text = "Create New " + common_functions.makeCapitalised(comp_displayname);
            editMap.Text = "Edit Selected " + common_functions.makeCapitalised(comp_displayname);
            deleteMap.Text = "Delete Selected " + common_functions.makeCapitalised(comp_displayname);

            //Load config
            comp_json_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/" + comp_displayname.ToUpper() + "_CONFIG.JSON"));
            foreach (var config_entry in comp_json_config)
            {
                assetList.Items.Add(config_entry.Key.ToString());
            }
        }

        /* Open creator window per comp type */
        private void newMap_Click(object sender, EventArgs e)
        {
            switch (this_comp_type)
            {
                case AssetCompType.CHARACTER:
                    Create_Character createChar = new Create_Character();
                    //needs to refresh
                    createChar.Show();
                    break;
                case AssetCompType.MAP:
                    Create_Map createMap = new Create_Map();
                    //needs to refresh
                    createMap.Show();
                    break;
                case AssetCompType.VEHICLE:
                    Create_Vehicle createVehicle = new Create_Vehicle();
                    //needs to refresh
                    createVehicle.Show();
                    break;
            }
        }

        /* Act on selection */
        private void assetList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                return;
            }

            string file_path_without_extension = common_functions.getFolder(AssetType.IMAGE) + comp_json_config[assetList.SelectedItem.ToString()]["menu_sprite"].Value<string>() + ".";
            string file_path_with_extension = "";
            if (File.Exists(file_path_without_extension + "PNG"))
            {
                file_path_with_extension = file_path_without_extension + "PNG";
            }
            else if (File.Exists(file_path_without_extension + "JPG"))
            {
                file_path_with_extension = file_path_without_extension + "JPG";
            }
            else if (File.Exists(file_path_without_extension + "JPEG"))
            {
                file_path_with_extension = file_path_without_extension + "JPEG";
            }
            else
            {
                previewImage.Image = null;
                return;
            }

            using (var tempPreviewImg = new Bitmap(file_path_with_extension))
            {
                previewImage.Image = new Bitmap(tempPreviewImg);
            }
        }

        /* Open creator window for editing */
        private void editMap_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select a " + comp_displayname + " from the list to edit.", "No " + comp_displayname + " selected.", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            switch (this_comp_type)
            {
                case AssetCompType.CHARACTER:
                    //Create_Map createChar = new Create_Map();
                    //needs to refresh
                    //createChar.Show();
                    break;
                case AssetCompType.MAP:
                    Create_Map editMap = new Create_Map(comp_json_config, assetList.SelectedItem.ToString());
                    //needs to refresh
                    editMap.Show();
                    break;
                case AssetCompType.VEHICLE:
                    //Create_Map createVehicle = new Create_Map();
                    //needs to refresh
                    //createVehicle.Show();
                    break;
            }
        }

        /* Delete selected */
        private void deleteMap_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select a " + comp_displayname + " from the list to delete.", "No " + comp_displayname + " selected.", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            //Confirm
            DialogResult showErrorInfo = MessageBox.Show("Are you sure you want to delete this " + comp_displayname + "?", "About to delete selected " + comp_displayname + "...", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (showErrorInfo != DialogResult.Yes)
            {
                return;
            }

            //Remove useage tags
            JToken data_block = comp_json_config[assetList.SelectedItem.ToString()];
            common_functions.removeUseageTag(AssetType.STRING, data_block["friendly_name"].Value<string>(), common_functions.getUseageTagFor(this_comp_type));
            common_functions.removeUseageTag(AssetType.IMAGE, data_block["menu_sprite"].Value<string>(), common_functions.getUseageTagFor(this_comp_type));
            common_functions.removeUseageTag(AssetType.MODEL, data_block["model"].Value<string>(), common_functions.getUseageTagFor(this_comp_type));
            if (this_comp_type == AssetCompType.MAP)
            {
                common_functions.removeUseageTag(AssetType.SOUND, data_block["audio"]["background_start"].Value<string>(), common_functions.getUseageTagFor(this_comp_type));
                common_functions.removeUseageTag(AssetType.SOUND, data_block["audio"]["background"].Value<string>(), common_functions.getUseageTagFor(this_comp_type));
                common_functions.removeUseageTag(AssetType.SOUND, data_block["audio"]["final_lap_start"].Value<string>(), common_functions.getUseageTagFor(this_comp_type));
                common_functions.removeUseageTag(AssetType.SOUND, data_block["audio"]["final_lap"].Value<string>(), common_functions.getUseageTagFor(this_comp_type));
            }

            //Remove JToken from config
            comp_json_config.Remove(assetList.SelectedItem.ToString());
            File.WriteAllText("DATA/CONFIGS/" + comp_displayname.ToUpper() + "_CONFIG.JSON", comp_json_config.ToString(Formatting.Indented));
            MessageBox.Show(common_functions.makeCapitalised(comp_displayname) + " successfully deleted!", "Deleted.", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}
