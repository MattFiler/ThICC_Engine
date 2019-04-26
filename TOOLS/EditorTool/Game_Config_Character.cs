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
    public partial class Game_Config_Character : Form
    {
        UsefulFunctions useful_functions = new UsefulFunctions();
        JToken controller_config;
        public Game_Config_Character()
        {
            InitializeComponent();

            //Original config
            controller_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/PLAYER_CONTROLLER_CONFIG.JSON"));
            movementSpeed.Value = controller_config["movement_speed"].Value<int>();
            turningSpeed.Value = controller_config["turning_speed"].Value<int>();
            driftBoost.Value = Convert.ToInt32(controller_config["drift_boost_multiplier"].Value<double>() * 10);
            turnTime.Value = controller_config["time_for_max_turn"].Value<int>();
            driftTime.Value = controller_config["time_for_max_drift"].Value<int>();
        }

        /* Save */
        private void saveConfig_Click(object sender, EventArgs e)
        {
            //Update config
            controller_config["movement_speed"] = movementSpeed.Value;
            controller_config["turning_speed"] = turningSpeed.Value;
            controller_config["drift_boost_multiplier"] = (Convert.ToSingle(driftBoost.Value)/10.0f);
            controller_config["time_for_max_turn"] = turnTime.Value;
            controller_config["time_for_max_drift"] = driftTime.Value;
            File.WriteAllText("DATA/CONFIGS/PLAYER_CONTROLLER_CONFIG.JSON", controller_config.ToString(Formatting.Indented));
            MessageBox.Show("Configuration saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Close();
        }

        /* Previews */
        private void movementSpeed_Scroll(object sender, EventArgs e)
        {
            movementSpeedPreview.Text = movementSpeed.Value.ToString();
        }
        private void turningSpeed_Scroll(object sender, EventArgs e)
        {
            turnSpeedPreview.Text = turningSpeed.Value.ToString();
        }
        private void driftBoost_Scroll(object sender, EventArgs e)
        {
            driftBoostPreview.Text = (Convert.ToSingle(driftBoost.Value) / 10.0f).ToString();
        }
        private void turnTime_Scroll(object sender, EventArgs e)
        {
            maxTurnPreview.Text = turnTime.Value.ToString();
        }
        private void driftTime_Scroll(object sender, EventArgs e)
        {
            maxDriftPreview.Text = driftTime.Value.ToString();
        }
    }
}
