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
    public partial class Create_Global_Sounds : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();
        JToken controller_config;
        public Create_Global_Sounds()
        {
            InitializeComponent();

            //Original config
            controller_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/SOUNDS_CONFIG.JSON"));
            menuLoop.Text = controller_config["menu_sounds"]["MENU_LOOP"].Value<string>();
            titlescreenStart.Text = controller_config["menu_sounds"]["TITLE_START"].Value<string>();
            titlescreenLoop.Text = controller_config["menu_sounds"]["TITLE_LOOP"].Value<string>();
            levelIntro.Text = controller_config["misc_sounds"]["COURSE_INTRO"].Value<string>();
            countdownIntro.Text = controller_config["misc_sounds"]["PRE_COUNTDOWN"].Value<string>();
            countdown.Text = controller_config["misc_sounds"]["COUNTDOWN"].Value<string>();
            itemboxHit.Text = controller_config["misc_sounds"]["ITEMBOX_HIT"].Value<string>();
            finalLapIntro.Text = controller_config["misc_sounds"]["FINAL_LAP_SOUND"].Value<string>();
        }

        /* Select assets */
        private void loadMenuLoop_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(menuLoop, AssetType.SOUND);
        }
        private void loadTitlescreenStart_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(titlescreenStart, AssetType.SOUND);
        }
        private void loadTitlescreenLoop_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(titlescreenLoop, AssetType.SOUND);
        }
        private void loadLevelIntro_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(levelIntro, AssetType.SOUND);
        }
        private void loadCountdownIntro_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(countdownIntro, AssetType.SOUND);
        }
        private void loadCountdown_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(countdown, AssetType.SOUND);
        }
        private void loadItemboxHit_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(itemboxHit, AssetType.SOUND);
        }
        private void loadFinalLapIntro_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(finalLapIntro, AssetType.SOUND);
        }

        /* Save */
        private void saveSounds_Click(object sender, EventArgs e)
        {
            //All inputs required
            if (menuLoop.Text == "" || titlescreenStart.Text == "" || titlescreenLoop.Text == "" ||
                levelIntro.Text == "" || countdownIntro.Text == "" ||
                countdown.Text == "" || itemboxHit.Text == "" ||
                finalLapIntro.Text == "")
            {
                MessageBox.Show("Please complete all fields before trying to save.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update tags
            function_libary.executeUseageQueueForTag(function_libary.getUseageTagFor(AssetCompType.GLOBAL_SOUNDS));

            //Update config
            controller_config["menu_sounds"]["MENU_LOOP"] = menuLoop.Text;
            controller_config["menu_sounds"]["TITLE_START"] = titlescreenStart.Text;
            controller_config["menu_sounds"]["TITLE_LOOP"] = titlescreenLoop.Text;
            controller_config["misc_sounds"]["COURSE_INTRO"] = levelIntro.Text;
            controller_config["misc_sounds"]["PRE_COUNTDOWN"] = countdownIntro.Text;
            controller_config["misc_sounds"]["COUNTDOWN"] = countdown.Text;
            controller_config["misc_sounds"]["ITEMBOX_HIT"] = itemboxHit.Text;
            controller_config["misc_sounds"]["FINAL_LAP_SOUND"] = finalLapIntro.Text;

            //Save
            File.WriteAllText("DATA/CONFIGS/SOUNDS_CONFIG.JSON", controller_config.ToString(Formatting.Indented));
            MessageBox.Show("Configuration saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Close();
        }





        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }
    }
}
