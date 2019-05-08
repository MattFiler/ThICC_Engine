using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Launcher
{
    public partial class Form1 : Form
    {
        JObject game_config_json;

        List<Label> labels = new List<Label>();
        List<ComboBox> dropdowns = new List<ComboBox>();

        bool launch_game = false; //ugly global!
        
        public Form1()
        {
            InitializeComponent();
            this.FormClosing += PO_Launcher_FormClosing1;
        }

        /* Launcher Load */
        private void Launcher_Load(object sender, EventArgs e)
        {
            //Load image and select default res
            launcherImage.Image = new Bitmap(Properties.Resources.updated_launcher_banner);
            resolutionSelector.SelectedIndex = 0;
            po_language.SelectedIndex = 0;

            //Get current game config 
            game_config_json = JObject.Parse(File.ReadAllText("DATA/CONFIGS/GAME_CORE.JSON"));

            //Load previous res from config
            resolutionSelector.SelectedItem = game_config_json["window_width"] + "x" + game_config_json["window_height"];

            //Load previous language
            string loadedLanguage = game_config_json["language"].ToString();
            po_language.SelectedItem = loadedLanguage.Substring(0,1) + loadedLanguage.Substring(1).ToLower();
        }

        /* Launcher Close */
        private void PO_Launcher_FormClosing1(object sender, FormClosingEventArgs e)
        {
            //Update internal config with new resolution
            string[] new_resolution = resolutionSelector.Items[resolutionSelector.SelectedIndex].ToString().Split('x');
            game_config_json["window_width"] = Convert.ToInt32(new_resolution[0]);
            game_config_json["window_height"] = Convert.ToInt32(new_resolution[1]);
            game_config_json["ui_scale"] = Convert.ToDouble(new_resolution[0]) / 1280.0f;

            //Update internal config with new language
            game_config_json["language"] = po_language.SelectedItem.ToString().ToUpper();

            //Save back out
            File.WriteAllText("DATA/CONFIGS/GAME_CORE.JSON", game_config_json.ToString(Formatting.Indented));

            //Launch game if requested (must do this AFTER saving config!)
            if (launch_game)
            {
                Process.Start("ThICC_Engine.exe", "Launcher_Auth");
            }
        }

        /* Request Quit Launcher */
        private void quitButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /* Launch Game */
        private void playButton_Click(object sender, EventArgs e)
        {
            launch_game = true;
            this.Close();
        }
        
        /* Perform first time setup */
        private void FirstTimeSetup_Click(object sender, EventArgs e)
        {
            Process.Start("vc_redist.x64.exe");
        }
    }
}
