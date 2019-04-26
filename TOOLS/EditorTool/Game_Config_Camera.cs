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
    public partial class Game_Config_Camera : Form
    {
        UsefulFunctions useful_functions = new UsefulFunctions();
        JToken controller_config;
        string cam_type = "";
        public Game_Config_Camera()
        {
            InitializeComponent();

            //Load config
            controller_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/CAMERA_CONFIG.JSON"));

            //Shared vals
            nearClip.Value = controller_config["GENERAL"]["view_near"].Value<int>();
            farClip.Value = controller_config["GENERAL"]["view_far"].Value<int>();

            //Select cam always
            camType.SelectedIndex = 0;
        }

        /* Cam type changed */
        private void camType_SelectedIndexChanged(object sender, EventArgs e)
        {
            /* SAVE CURRENT CONFIG */
            if (cam_type != "")
            {
                //Common
                controller_config[cam_type]["fov"] = fov.Value;
                if (cam_type != "DEBUG_CAM")
                {
                    controller_config[cam_type]["rotation_lerp"] = rotLerp.Value;
                    controller_config[cam_type]["position_lerp"] = posLerp.Value;
                    controller_config[cam_type]["look_at_position"][0] = lookatX.Value;
                    controller_config[cam_type]["look_at_position"][1] = lookatY.Value;
                    controller_config[cam_type]["look_at_position"][2] = lookatZ.Value;
                    controller_config[cam_type]["target_position"][0] = targetX.Value;
                    controller_config[cam_type]["target_position"][1] = targetY.Value;
                    controller_config[cam_type]["target_position"][2] = targetZ.Value;
                    controller_config[cam_type]["camera_offset"][0] = camOffX.Value;
                    controller_config[cam_type]["camera_offset"][1] = camOffY.Value;
                    controller_config[cam_type]["camera_offset"][2] = camOffZ.Value;
                    controller_config[cam_type]["look_at_offset"][0] = lookOffsetX.Value;
                    controller_config[cam_type]["look_at_offset"][1] = lookOffsetY.Value;
                    controller_config[cam_type]["look_at_offset"][2] = lookOffsetZ.Value;
                }

                //Bespoke
                if (spinAmount.Enabled)
                {
                    controller_config[cam_type]["spin_amount"] = spinAmount.Value;
                }
                if (timeOut.Enabled)
                {
                    controller_config[cam_type]["timeout"] = timeOut.Value;
                }
                if (movementSpeed.Enabled)
                {
                    controller_config[cam_type]["camera_speed"] = movementSpeed.Value;
                }
                if (rotationSpeed.Enabled)
                {
                    controller_config[cam_type]["camera_rot_speed"] = rotationSpeed.Value;
                }
            }

            /* LOAD NEW CONFIG */
            cam_type = camType.SelectedItem.ToString();

            //Common
            fov.Value = controller_config[cam_type]["fov"].Value<int>();
            fovPrev.Text = fov.Value.ToString();
            if (cam_type != "DEBUG_CAM")
            {
                rotLerp.Value = controller_config[cam_type]["rotation_lerp"].Value<decimal>();
                posLerp.Value = controller_config[cam_type]["position_lerp"].Value<decimal>();
                lookatX.Value = controller_config[cam_type]["look_at_position"][0].Value<decimal>();
                lookatY.Value = controller_config[cam_type]["look_at_position"][1].Value<decimal>();
                lookatZ.Value = controller_config[cam_type]["look_at_position"][2].Value<decimal>();
                targetX.Value = controller_config[cam_type]["target_position"][0].Value<decimal>();
                targetY.Value = controller_config[cam_type]["target_position"][1].Value<decimal>();
                targetZ.Value = controller_config[cam_type]["target_position"][2].Value<decimal>();
                camOffX.Value = controller_config[cam_type]["camera_offset"][0].Value<decimal>();
                camOffY.Value = controller_config[cam_type]["camera_offset"][1].Value<decimal>();
                camOffZ.Value = controller_config[cam_type]["camera_offset"][2].Value<decimal>();
                lookOffsetX.Value = controller_config[cam_type]["look_at_offset"][0].Value<decimal>();
                lookOffsetY.Value = controller_config[cam_type]["look_at_offset"][1].Value<decimal>();
                lookOffsetZ.Value = controller_config[cam_type]["look_at_offset"][2].Value<decimal>();
            }

            //Bespoke
            if (controller_config[cam_type]["spin_amount"] != null) {
                spinAmount.Value = controller_config[cam_type]["spin_amount"].Value<decimal>();
                spinAmount.Enabled = true;
            }
            else
            {
                spinAmount.Value = 0;
                spinAmount.Enabled = false;
            }
            if (controller_config[cam_type]["timeout"] != null)
            {
                timeOut.Value = controller_config[cam_type]["timeout"].Value<decimal>();
                timeOut.Enabled = true;
            }
            else
            {
                timeOut.Value = 0;
                timeOut.Enabled = false;
            }
            if (controller_config[cam_type]["camera_speed"] != null)
            {
                movementSpeed.Value = controller_config[cam_type]["camera_speed"].Value<decimal>();
                movementSpeed.Enabled = true;
            }
            else
            {
                movementSpeed.Value = 0;
                movementSpeed.Enabled = false;
            }
            if (controller_config[cam_type]["camera_rot_speed"] != null)
            {
                rotationSpeed.Value = controller_config[cam_type]["camera_rot_speed"].Value<decimal>();
                rotationSpeed.Enabled = true;
            }
            else
            {
                rotationSpeed.Value = 0;
                rotationSpeed.Enabled = false;
            }
        }

        /* Preview FOV when changed */
        private void fov_Scroll(object sender, EventArgs e)
        {
            fovPrev.Text = fov.Value.ToString();
        }

        /* Save out */
        private void saveConfig_Click(object sender, EventArgs e)
        {
            //Change index to force a config update
            if (camType.SelectedIndex < 6)
            {
                camType.SelectedIndex = camType.SelectedIndex + 1;
            }
            else
            {
                camType.SelectedIndex = 0;
            }

            //Save shared
            controller_config["GENERAL"]["view_near"] = nearClip.Value;
            controller_config["GENERAL"]["view_far"] = farClip.Value;

            //Save config
            File.WriteAllText("DATA/CONFIGS/CAMERA_CONFIG.JSON", controller_config.ToString(Formatting.Indented));
            MessageBox.Show("Configuration saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Close();
        }
    }
}
