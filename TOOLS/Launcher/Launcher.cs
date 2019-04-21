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
            launcherImage.Image = new Bitmap(Properties.Resources.beta_banner);
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
                tabPage2.Controls.Add(makeNewDropdown(keybind.Value.ToString()));
            }

            //Load previous res from config
            resolutionSelector.SelectedItem = game_config_json["window_width"] + "x" + game_config_json["window_height"];

            //Load previous language
            string loadedLanguage = game_config_json["language"].ToString();
            po_language.SelectedItem = loadedLanguage.Substring(0,1) + loadedLanguage.Substring(1).ToLower();
        }

        /* Launcher Close */
        private void PO_Launcher_FormClosing1(object sender, FormClosingEventArgs e)
        {
            //Update internal config with new keybinds
            for (int i = 0; i < labels.Count; i++) {
                keybind_config_json[labels[i].Text] = dropdowns[i].Items[dropdowns[i].SelectedIndex].ToString();
            }

            //Update internal config with new resolution
            string[] new_resolution = resolutionSelector.Items[resolutionSelector.SelectedIndex].ToString().Split('x');
            game_config_json["window_width"] = Convert.ToInt32(new_resolution[0]);
            game_config_json["window_height"] = Convert.ToInt32(new_resolution[1]);
            game_config_json["ui_scale"] = Convert.ToDouble(new_resolution[0]) / 1280.0f;

            //Update internal config with new language
            game_config_json["language"] = po_language.SelectedItem.ToString().ToUpper();

            //Save back out
            File.WriteAllText("DATA/CONFIGS/GAME_CORE.JSON", game_config_json.ToString(Formatting.Indented));
            File.WriteAllText("DATA/CONFIGS/KEYBINDS.JSON", keybind_config_json.ToString(Formatting.Indented));

            //Launch game if requested (must do this AFTER saving config!)
            if (launch_game)
            {
                Process.Start("Mario Kart.exe", "Launcher_Auth");
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
            input_dropdown.Items.AddRange(new object[] {
                "Back", "Tab", "Enter", "Pause", "CapsLock", "Kana", "Kanji", "Escape", "ImeConvert", "ImeNoConvert",
                "Space", "PageUp", "PageDown", "End", "Home", "Left", "Up", "Right", "Down", "Select", "Print", "Execute",
                "PrintScreen", "Insert", "Delete", "Help", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "A",
                "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W",
                "X", "Y", "Z", "LeftWindows", "RightWindows", "Apps", "Sleep", "NumPad0", "NumPad1", "NumPad2", "NumPad3",
                "NumPad4", "NumPad5", "NumPad6", "NumPad7", "NumPad8", "NumPad9", "Multiply", "Add", "Separator", "Subtract",
                "Decimal", "Divide", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14",
                "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "NumLock", "Scroll", "LeftShift", "RightShift",
                "LeftControl", "RightControl", "LeftAlt", "RightAlt", "BrowserBack", "BrowserForward", "BrowserRefresh", "BrowserStop",
                "BrowserSearch", "BrowserFavorites", "BrowserHome", "VolumeMute", "VolumeDown", "VolumeUp", "MediaNextTrack",
                "MediaPreviousTrack", "MediaStop", "MediaPlayPause", "LaunchMail", "SelectMedia", "LaunchApplication1",
                "LaunchApplication2", "OemSemicolon", "OemPlus", "OemComma", "OemMinus", "OemPeriod", "OemQuestion", "OemTilde",
                "OemOpenBrackets", "OemPipe", "OemCloseBrackets", "OemQuotes", "Oem8", "OemBackslash", "ProcessKey", "OemCopy",
                "OemAuto", "OemEnlW", "Attn", "Crsel", "Exsel", "EraseEof", "Play", "Zoom", "Pa1", "OemClear"});
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
    }
}
