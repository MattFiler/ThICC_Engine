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
        JObject keybind_config_json;

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
            //launcherImage.Image = new Bitmap(Properties.Resources.launcher_banner);
            resolutionSelector.SelectedIndex = 0;
            po_language.SelectedIndex = 0;
            tabPage2.AutoScroll = true;

            //Get current game config 
            game_config_json = JObject.Parse(File.ReadAllText("DATA/CONFIGS/GAME_CORE.JSON"));
            keybind_config_json = JObject.Parse(File.ReadAllText("DATA/CONFIGS/KEYBINDS.JSON"));

            //Add keybinds to list from config
            foreach (var keybind in keybind_config_json)
            {
                tabPage2.Controls.Add(makeNewLabel(keybind.Key));
                tabPage2.Controls.Add(makeNewDropdown(keycodeToKeyname(Convert.ToInt32(keybind.Value))));
            }

            //Load previous res from config
            resolutionSelector.SelectedItem = game_config_json["resolution"]["width"] + "x" + game_config_json["resolution"]["height"];

            //Load previous language
            string loadedLanguage = game_config_json["language"].ToString();
            po_language.SelectedItem = loadedLanguage.Substring(0,1) + loadedLanguage.Substring(1).ToLower();
        }

        /* Launcher Close */
        private void PO_Launcher_FormClosing1(object sender, FormClosingEventArgs e)
        {
            //Update internal config with new keybinds
            for (int i = 0; i < labels.Count; i++) {
                keybind_config_json[labels[i].Text] = keynameToKeycode(dropdowns[i].Items[dropdowns[i].SelectedIndex].ToString());
            }

            //Update internal config with new resolution
            string[] new_resolution = resolutionSelector.Items[resolutionSelector.SelectedIndex].ToString().Split('x');
            game_config_json["resolution"]["width"] = Convert.ToInt32(new_resolution[0]);
            game_config_json["resolution"]["height"] = Convert.ToInt32(new_resolution[1]);

            //Update internal config with new language
            game_config_json["language"] = po_language.SelectedItem.ToString().ToUpper();

            //Save back out
            File.WriteAllText("DATA/CONFIGS/GAME_CORE.JSON", game_config_json.ToString(Formatting.Indented));
            File.WriteAllText("DATA/CONFIGS/KEYBINDS.JSON", keybind_config_json.ToString(Formatting.Indented));

            //Launch game if requested (must do this AFTER saving config!)
            if (launch_game)
            {
                Process.Start("Scarle2019.exe", "Launcher_Auth");
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

        /* Make new label */
        Label makeNewLabel(string labeltext = "Dummy Input Name")
        {
            Label input_label = new Label();
            input_label.AutoSize = true;
            input_label.Text = labeltext;
            input_label.TextAlign = ContentAlignment.MiddleLeft;
            int new_y = 21;
            if (labels.Count - 1 >= 0)
            {
                new_y = labels[labels.Count - 1].Location.Y + 25;
            }
            input_label.Location = new Point(184, new_y);
            labels.Add(input_label);
            return input_label;
        }

        /* Make new input dropdown */
        ComboBox makeNewDropdown(string selecteditem = "KEY_SPACE")
        {
            ComboBox input_dropdown = new ComboBox();
            input_dropdown.DropDownStyle = ComboBoxStyle.DropDownList;
            input_dropdown.FormattingEnabled = true;
            input_dropdown.Size = new Size(160, 21);
            input_dropdown.Items.Add("KEY_SPACE");
            input_dropdown.Items.Add("KEY_APOSTROPHE");
            input_dropdown.Items.Add("KEY_COMMA");
            input_dropdown.Items.Add("KEY_MINUS");
            input_dropdown.Items.Add("KEY_PERIOD");
            input_dropdown.Items.Add("KEY_SLASH");
            input_dropdown.Items.Add("KEY_0");
            input_dropdown.Items.Add("KEY_1");
            input_dropdown.Items.Add("KEY_2");
            input_dropdown.Items.Add("KEY_3");
            input_dropdown.Items.Add("KEY_4");
            input_dropdown.Items.Add("KEY_5");
            input_dropdown.Items.Add("KEY_6");
            input_dropdown.Items.Add("KEY_7");
            input_dropdown.Items.Add("KEY_8");
            input_dropdown.Items.Add("KEY_9");
            input_dropdown.Items.Add("KEY_SEMICOLON");
            input_dropdown.Items.Add("KEY_EQUAL");
            input_dropdown.Items.Add("KEY_A");
            input_dropdown.Items.Add("KEY_B");
            input_dropdown.Items.Add("KEY_C");
            input_dropdown.Items.Add("KEY_D");
            input_dropdown.Items.Add("KEY_E");
            input_dropdown.Items.Add("KEY_F");
            input_dropdown.Items.Add("KEY_G");
            input_dropdown.Items.Add("KEY_H");
            input_dropdown.Items.Add("KEY_I");
            input_dropdown.Items.Add("KEY_J");
            input_dropdown.Items.Add("KEY_K");
            input_dropdown.Items.Add("KEY_L");
            input_dropdown.Items.Add("KEY_M");
            input_dropdown.Items.Add("KEY_N");
            input_dropdown.Items.Add("KEY_O");
            input_dropdown.Items.Add("KEY_P");
            input_dropdown.Items.Add("KEY_Q");
            input_dropdown.Items.Add("KEY_R");
            input_dropdown.Items.Add("KEY_S");
            input_dropdown.Items.Add("KEY_T");
            input_dropdown.Items.Add("KEY_U");
            input_dropdown.Items.Add("KEY_V");
            input_dropdown.Items.Add("KEY_W");
            input_dropdown.Items.Add("KEY_X");
            input_dropdown.Items.Add("KEY_Y");
            input_dropdown.Items.Add("KEY_Z");
            input_dropdown.Items.Add("KEY_LEFT_BRACKET");
            input_dropdown.Items.Add("KEY_BACKSLASH");
            input_dropdown.Items.Add("KEY_RIGHT_BRACKET");
            input_dropdown.Items.Add("KEY_GRAVE_ACCENT");
            input_dropdown.Items.Add("KEY_WORLD_1");
            input_dropdown.Items.Add("KEY_WORLD_2");
            input_dropdown.Items.Add("KEY_ESCAPE");
            input_dropdown.Items.Add("KEY_ENTER");
            input_dropdown.Items.Add("KEY_TAB");
            input_dropdown.Items.Add("KEY_BACKSPACE");
            input_dropdown.Items.Add("KEY_DELETE");
            input_dropdown.Items.Add("KEY_RIGHT");
            input_dropdown.Items.Add("KEY_LEFT");
            input_dropdown.Items.Add("KEY_DOWN");
            input_dropdown.Items.Add("KEY_UP");
            input_dropdown.SelectedItem = selecteditem;
            int new_y = 18;
            if (dropdowns.Count - 1 >= 0)
            {
                new_y = dropdowns[dropdowns.Count - 1].Location.Y + 25;
            }
            input_dropdown.Location = new Point(18, new_y);
            dropdowns.Add(input_dropdown);
            return input_dropdown;
        }

        /* Convert Key Name to Code */
        int keynameToKeycode(string keyname)
        {
            switch (keyname)
            {
                case "KEY_SPACE": return 32;
                case "KEY_APOSTROPHE": return 39;
                case "KEY_COMMA": return 44;
                case "KEY_MINUS": return 45;
                case "KEY_PERIOD": return 46;
                case "KEY_SLASH": return 47;
                case "KEY_0": return 48;
                case "KEY_1": return 49;
                case "KEY_2": return 50;
                case "KEY_3": return 51;
                case "KEY_4": return 52;
                case "KEY_5": return 53;
                case "KEY_6": return 54;
                case "KEY_7": return 55;
                case "KEY_8": return 56;
                case "KEY_9": return 57;
                case "KEY_SEMICOLON": return 59;
                case "KEY_EQUAL": return 61;
                case "KEY_A": return 65;
                case "KEY_B": return 66;
                case "KEY_C": return 67;
                case "KEY_D": return 68;
                case "KEY_E": return 69;
                case "KEY_F": return 70;
                case "KEY_G": return 71;
                case "KEY_H": return 72;
                case "KEY_I": return 73;
                case "KEY_J": return 74;
                case "KEY_K": return 75;
                case "KEY_L": return 76;
                case "KEY_M": return 77;
                case "KEY_N": return 78;
                case "KEY_O": return 79;
                case "KEY_P": return 80;
                case "KEY_Q": return 81;
                case "KEY_R": return 82;
                case "KEY_S": return 83;
                case "KEY_T": return 84;
                case "KEY_U": return 85;
                case "KEY_V": return 86;
                case "KEY_W": return 87;
                case "KEY_X": return 88;
                case "KEY_Y": return 89;
                case "KEY_Z": return 90;
                case "KEY_LEFT_BRACKET": return 91;
                case "KEY_BACKSLASH": return 92;
                case "KEY_RIGHT_BRACKET": return 93;
                case "KEY_GRAVE_ACCENT": return 96;
                case "KEY_WORLD_1": return 161;
                case "KEY_WORLD_2": return 162;
                case "KEY_ESCAPE": return 256;
                case "KEY_ENTER": return 257;
                case "KEY_TAB": return 258;
                case "KEY_BACKSPACE": return 259;
                case "KEY_DELETE": return 261;
                case "KEY_RIGHT": return 262;
                case "KEY_LEFT": return 263;
                case "KEY_DOWN": return 264;
                case "KEY_UP": return 265;
                default: return 0;
            }
        }

        /* Convert Key Code to Name */
        string keycodeToKeyname(int keycode)
        {
            switch (keycode)
            {
                case 32: return "KEY_SPACE";
                case 39: return "KEY_APOSTROPHE";
                case 44: return "KEY_COMMA";
                case 45: return "KEY_MINUS";
                case 46: return "KEY_PERIOD";
                case 47: return "KEY_SLASH";
                case 48: return "KEY_0";
                case 49: return "KEY_1";
                case 50: return "KEY_2";
                case 51: return "KEY_3";
                case 52: return "KEY_4";
                case 53: return "KEY_5";
                case 54: return "KEY_6";
                case 55: return "KEY_7";
                case 56: return "KEY_8";
                case 57: return "KEY_9";
                case 59: return "KEY_SEMICOLON";
                case 61: return "KEY_EQUAL";
                case 65: return "KEY_A";
                case 66: return "KEY_B";
                case 67: return "KEY_C";
                case 68: return "KEY_D";
                case 69: return "KEY_E";
                case 70: return "KEY_F";
                case 71: return "KEY_G";
                case 72: return "KEY_H";
                case 73: return "KEY_I";
                case 74: return "KEY_J";
                case 75: return "KEY_K";
                case 76: return "KEY_L";
                case 77: return "KEY_M";
                case 78: return "KEY_N";
                case 79: return "KEY_O";
                case 80: return "KEY_P";
                case 81: return "KEY_Q";
                case 82: return "KEY_R";
                case 83: return "KEY_S";
                case 84: return "KEY_T";
                case 85: return "KEY_U";
                case 86: return "KEY_V";
                case 87: return "KEY_W";
                case 88: return "KEY_X";
                case 89: return "KEY_Y";
                case 90: return "KEY_Z";
                case 91: return "KEY_LEFT_BRACKET";
                case 92: return "KEY_BACKSLASH";
                case 93: return "KEY_RIGHT_BRACKET";
                case 96: return "KEY_GRAVE_ACCENT";
                case 161: return "KEY_WORLD_1";
                case 162: return "KEY_WORLD_2";
                case 256: return "KEY_ESCAPE";
                case 257: return "KEY_ENTER";
                case 258: return "KEY_TAB";
                case 259: return "KEY_BACKSPACE";
                case 261: return "KEY_DELETE";
                case 262: return "KEY_RIGHT";
                case 263: return "KEY_LEFT";
                case 264: return "KEY_DOWN";
                case 265: return "KEY_UP";
                default: return "";
            }
        }
    }
}
