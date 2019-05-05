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
    public partial class Create_Gamepad_Icons : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();
        JToken controller_config;
        public Create_Gamepad_Icons(JObject json_data = null, string object_key = "")
        {
            InitializeComponent();

            //Original config
            controller_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/CONTROLLER_GLYPH_CONFIG.JSON"));
            btnA.Text = controller_config["btn_a"].Value<string>();
            btnB.Text = controller_config["btn_b"].Value<string>();
            btnX.Text = controller_config["btn_x"].Value<string>();
            btnY.Text = controller_config["btn_y"].Value<string>();
            btnStart.Text = controller_config["btn_start"].Value<string>();
            btnBack.Text = controller_config["btn_back"].Value<string>();
            dpadLeft.Text = controller_config["dpad_left"].Value<string>();
            dpadRight.Text = controller_config["dpad_right"].Value<string>();
            dpadUp.Text = controller_config["dpad_up"].Value<string>();
            dpadDown.Text = controller_config["dpad_down"].Value<string>();
            dpadGeneric.Text = controller_config["dpad_generic"].Value<string>();
            leftStick.Text = controller_config["stick_left"].Value<string>();
            rightStick.Text = controller_config["stick_right"].Value<string>();
            leftBumper.Text = controller_config["bumper_left"].Value<string>();
            rightBumper.Text = controller_config["bumper_right"].Value<string>();
            leftTrigger.Text = controller_config["trigger_left"].Value<string>();
            rightTrigger.Text = controller_config["trigger_right"].Value<string>();
        }

        /* On Load */
        private void Create_Gamepad_Icons_Load(object sender, EventArgs e)
        {
        }

        /* Select assets */
        private void btnALoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnA, AssetType.IMAGE);
        }
        private void btnBLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnB, AssetType.IMAGE);
        }
        private void btnXLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnX, AssetType.IMAGE);
        }
        private void btnYLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnY, AssetType.IMAGE);
        }
        private void leftStickLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(leftStick, AssetType.IMAGE);
        }
        private void rightStickLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(rightStick, AssetType.IMAGE);
        }
        private void leftTriggerLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(leftTrigger, AssetType.IMAGE);
        }
        private void rightTriggerLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(rightTrigger, AssetType.IMAGE);
        }
        private void leftBumperLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(leftBumper, AssetType.IMAGE);
        }
        private void rightBumperLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(rightBumper, AssetType.IMAGE);
        }
        private void btnBackLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnBack, AssetType.IMAGE);
        }
        private void btnStartLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnStart, AssetType.IMAGE);
        }
        private void dpadLeftLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(dpadLeft, AssetType.IMAGE);
        }
        private void dpadRightLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(dpadRight, AssetType.IMAGE);
        }
        private void dpadUpLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(dpadUp, AssetType.IMAGE);
        }
        private void dpadDownLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(dpadDown, AssetType.IMAGE);
        }
        private void dpadGenericLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(dpadGeneric, AssetType.IMAGE);
        }

        /* Save character to config */
        private void saveGamepadIcons_Click(object sender, EventArgs e)
        {
            //All inputs required
            if (btnA.Text == "" || btnB.Text == "" || btnX.Text == "" || btnY.Text == "" ||
                btnStart.Text == "" || btnBack.Text == "" || dpadLeft.Text == "" || dpadRight.Text == "" ||
                dpadUp.Text == "" || dpadDown.Text == "" || dpadGeneric.Text == "" || leftStick.Text == "" ||
                rightStick.Text == "" || leftBumper.Text == "" || rightBumper.Text == "" || leftTrigger.Text == "" ||
                rightTrigger.Text == "")
            {
                MessageBox.Show("Please complete all fields before trying to save.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update tags
            function_libary.executeUseageQueueForTag(function_libary.getUseageTagFor(AssetCompType.GAMEPAD_GLPYHS));

            //Add to config
            controller_config["btn_a"] = btnA.Text;
            controller_config["btn_b"] = btnB.Text;
            controller_config["btn_x"] = btnX.Text;
            controller_config["btn_y"] = btnY.Text;
            controller_config["btn_start"] = btnStart.Text;
            controller_config["btn_back"] = btnBack.Text;
            controller_config["dpad_left"] = dpadLeft.Text;
            controller_config["dpad_right"] = dpadRight.Text;
            controller_config["dpad_up"] = dpadUp.Text;
            controller_config["dpad_down"] = dpadDown.Text;
            controller_config["dpad_generic"] = dpadGeneric.Text;
            controller_config["stick_left"] = leftStick.Text;
            controller_config["stick_right"] = rightStick.Text;
            controller_config["bumper_left"] = leftBumper.Text;
            controller_config["bumper_right"] = rightBumper.Text;
            controller_config["trigger_left"] = leftTrigger.Text;
            controller_config["trigger_right"] = rightTrigger.Text;

            //Save
            File.WriteAllText("DATA/CONFIGS/CONTROLLER_GLYPH_CONFIG.JSON", controller_config.ToString(Formatting.Indented));
            MessageBox.Show("Saved character configuration!", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }
    }
}
