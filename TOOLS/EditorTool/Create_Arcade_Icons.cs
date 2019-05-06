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
    public partial class Create_Arcade_Icons : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();
        JToken controller_config;
        public Create_Arcade_Icons()
        {
            InitializeComponent();

            //Original config
            controller_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/CONTROLLER_GLYPH_CONFIG.JSON"));
            joyUp.Text = controller_config["Joystick Up"].Value<string>();
            joyDown.Text = controller_config["Joystick Down"].Value<string>();
            joyLeft.Text = controller_config["Joystick Left"].Value<string>();
            joyRight.Text = controller_config["Joystick Right"].Value<string>();
            btnBlue.Text = controller_config["Blue Button"].Value<string>();
            btnYellow.Text = controller_config["Yellow Button"].Value<string>();
            btnGrey1.Text = controller_config["Grey Top Button"].Value<string>();
            btnGreen.Text = controller_config["Green Button"].Value<string>();
            btnRed.Text = controller_config["Red Button"].Value<string>();
            btnGrey2.Text = controller_config["Grey Bottom Button"].Value<string>();
            btnSide.Text = controller_config["Side Button"].Value<string>();
        }

        /* Select assets */
        private void joyLeftLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(joyLeft, AssetType.IMAGE);
        }
        private void joyRightLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(joyRight, AssetType.IMAGE);
        }
        private void joyUpLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(joyUp, AssetType.IMAGE);
        }
        private void joyDownLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(joyDown, AssetType.IMAGE);
        }
        private void btnBlueLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnBlue, AssetType.IMAGE);
        }
        private void btnYellowLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnYellow, AssetType.IMAGE);
        }
        private void btnGrey1Load_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnGrey1, AssetType.IMAGE);
        }
        private void btnGreenLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnGreen, AssetType.IMAGE);
        }
        private void btnRedLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnRed, AssetType.IMAGE);
        }
        private void btnGrey2Load_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnGrey2, AssetType.IMAGE);
        }
        private void btnSideLoad_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(btnSide, AssetType.IMAGE);
        }

        /* Save character to config */
        private void saveGamepadIcons_Click(object sender, EventArgs e)
        {
            //All inputs required
            if (joyLeft.Text == "" || joyRight.Text == "" || joyUp.Text == "" || joyDown.Text == "" ||
                btnBlue.Text == "" || btnYellow.Text == "" || btnGrey1.Text == "" || btnGreen.Text == "" ||
                btnRed.Text == "" || btnGrey2.Text == "" || btnSide.Text == "")
            {
                MessageBox.Show("Please complete all fields before trying to save.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update tags
            function_libary.executeUseageQueueForTag(function_libary.getUseageTagFor(AssetCompType.ARCADE_GLPYHS));

            //Add to config
            controller_config["Joystick Up"] = joyUp.Text;
            controller_config["Joystick Down"] = joyDown.Text;
            controller_config["Joystick Left"] = joyLeft.Text;
            controller_config["Joystick Right"] = joyRight.Text;
            controller_config["Blue Button"] = btnBlue.Text;
            controller_config["Yellow Button"] = btnYellow.Text;
            controller_config["Grey Top Button"] = btnGrey1.Text;
            controller_config["Green Button"] = btnGreen.Text;
            controller_config["Red Button"] = btnRed.Text;
            controller_config["Grey Bottom Button"] = btnGrey2.Text;
            controller_config["Side Button"] = btnSide.Text;

            //Save
            File.WriteAllText("DATA/CONFIGS/CONTROLLER_GLYPH_CONFIG.JSON", controller_config.ToString(Formatting.Indented));
            MessageBox.Show("Saved arcade icon configuration!", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }
    }
}
