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
    public partial class Item_Asset_Editor : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();
        JToken controller_config;
        JToken controller_config_sounds;
        string selected_item = "GREEN_SHELL";
        public Item_Asset_Editor()
        {
            InitializeComponent();
            controller_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/ITEM_CONFIG.JSON"));
            controller_config_sounds = JToken.Parse(File.ReadAllText("DATA/CONFIGS/SOUNDS_CONFIG.JSON"));
        }

        /* On Load */
        private void ItemAssetEditor_Load(object sender, EventArgs e)
        {
            modelImage.Text = controller_config[selected_item]["ui_sprite"].Value<string>();
            model.Text = controller_config[selected_item]["model"].Value<string>();
            soundEffect.Text = controller_config_sounds["item_sounds"][selected_item].Value<string>();

            itemSelect.SelectedIndex = 0;
        }

        /* Selecting a new item */
        private void itemSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (modelImage.Text == "" || model.Text == "" || soundEffect.Text == "")
            {
                itemSelect.SelectedItem = selected_item;
                MessageBox.Show("Please complete all fields before switching item.", "Can't continue.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Save
            controller_config[selected_item]["ui_sprite"] = modelImage.Text;
            controller_config[selected_item]["model"] = model.Text;
            controller_config_sounds["item_sounds"][selected_item] = soundEffect.Text;
            
            selected_item = itemSelect.SelectedItem.ToString();

            //Update
            modelImage.Text = controller_config[selected_item]["ui_sprite"].Value<string>();
            model.Text = controller_config[selected_item]["model"].Value<string>();
            soundEffect.Text = controller_config_sounds["item_sounds"][selected_item].Value<string>();
        }

        /* Load assets */
        private void loadMenuImage_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(modelImage, AssetType.IMAGE);
        }
        private void loadModel_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(model, AssetType.MODEL);
        }
        private void loadSoundEffect_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(soundEffect, AssetType.SOUND);
        }

        /* Save config */
        private void saveConfig_Click(object sender, EventArgs e)
        {
            if (modelImage.Text == "" || model.Text == "" || soundEffect.Text == "")
            {
                MessageBox.Show("Please complete all fields before saving.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Change index to force a config update
            if (itemSelect.SelectedIndex < 12)
            {
                itemSelect.SelectedIndex = itemSelect.SelectedIndex + 1;
            }
            else
            {
                itemSelect.SelectedIndex = 0;
            }

            //Update tags
            function_libary.executeUseageQueueForTag(function_libary.getUseageTagFor(AssetCompType.ITEMS));
            
            //Save
            File.WriteAllText("DATA/CONFIGS/ITEM_CONFIG.JSON", controller_config.ToString(Formatting.Indented));
            File.WriteAllText("DATA/CONFIGS/SOUNDS_CONFIG.JSON", controller_config_sounds.ToString(Formatting.Indented));
            MessageBox.Show("Saved item asset configuration!", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }
    }
}
