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
    public partial class Keybinds_Editor : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();

        /* If editing, load data */
        JObject maps_json_config = null;
        string map_json_key = "";
        bool edit_mode = false;
        public Keybinds_Editor(JObject json_data = null, string object_key = "")
        {
            maps_json_config = json_data;
            map_json_key = object_key;
            edit_mode = (json_data != null && object_key != "");

            InitializeComponent();

            /* (CUT-DOWN) DIRECTX KEYBOARD INPUTS */
            keyboardInput.Items.AddRange(new object[] {
                "DISABLED",
                "Back", "Tab", "Enter", "Pause", "CapsLock", "Escape", "Space", "LeftWindows",
                "Left", "Up", "Right", "Down",
                "PageUp", "PageDown", "End", "Home", "PrintScreen", "Insert", "Delete",
                "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
                "NumPad0", "NumPad1", "NumPad2", "NumPad3", "NumPad4", "NumPad5", "NumPad6", "NumPad7", "NumPad8", "NumPad9",
                "Multiply", "Add", "Separator", "Subtract", "Decimal", "Divide",
                "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14",
                "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24",
                "NumLock", "Scroll",
                "LeftShift", "RightShift",
                "LeftControl", "RightControl",
                "LeftAlt", "RightAlt",
                "OemSemicolon", "OemPlus", "OemComma", "OemMinus", "OemPeriod", "OemQuestion", "OemTilde",
                "OemOpenBrackets", "OemPipe", "OemCloseBrackets", "OemQuotes", "OemBackslash"
            });
            keyboardInput.SelectedIndex = 0;

            /* ARCADE KEYBOARD INPUTS */
            arcadeInput.Items.AddRange(new object[] {
                "DISABLED",
                "Left Joystick Up",               //Maps to keyboard: UP
                "Left Joystick Down",             //Maps to keyboard: DOWN
                "Left Joystick Left",             //Maps to keyboard: LEFT
                "Left Joystick Right",            //Maps to keyboard: RIGHT
                "Left Blue Button",               //Maps to keyboard: LCTRL
                "Left Yellow Button",             //Maps to keyboard: LALT
                "Left Grey Top Button",           //Maps to keyboard: SPACE
                "Left Green Button",              //Maps to keyboard: LSHIFT
                "Left Red Button",                //Maps to keyboard: Z
                "Left Grey Bottom Button",        //Maps to keyboard: X
                "Left Side Button",               //Maps to keyboard: C

                "Right Joystick Up",              //Maps to keyboard: R
                "Right Joystick Down",            //Maps to keyboard: F
                "Right Joystick Left",            //Maps to keyboard: D
                "Right Joystick Right",           //Maps to keyboard: G
                "Right Blue Button",              //Maps to keyboard: A
                "Right Yellow Button",            //Maps to keyboard: S
                "Right Grey Top Button",          //Maps to keyboard: Q
                "Right Green Button",             //Maps to keyboard: W
                "Right Red Button",               //Maps to keyboard: I
                "Right Grey Bottom Button",       //Maps to keyboard: K
                "Right Side Button"               //Maps to keyboard: J
            });
            arcadeInput.SelectedIndex = 0;

            /* DIRECTX GAMEPAD INPUTS */
            gamepadInput.Items.AddRange(new object[] {
                "DISABLED",
                "A", "B", "X", "Y",
                "Left Stick", "Right Stick",
                "Left Shoulder", "Right Shoulder",
                "Back", "Start",
                "D-Pad Up", "D-Pad Down", "D-Pad Left", "D-Pad Right",
                "Left Stick Up", "Left Stick Down", "Left Stick Left", "Left Stick Right",
                "Right Stick Up", "Right Stick Down", "Right Stick Left", "Right Stick Right",
                "Left Trigger", "Right Trigger"
            });
            gamepadInput.SelectedIndex = 0;
        }

        /* On Load */
        private void Keybinds_Editor_Load(object sender, EventArgs e)
        {
            if (edit_mode)
            {
                // If in edit mode, load existing config
                keybindName.Text = map_json_key;
                keybindName.ReadOnly = true;
                keyboardInput.SelectedItem = maps_json_config[map_json_key]["Keyboard"].Value<string>();
                arcadeInput.SelectedItem = maps_json_config[map_json_key]["Arcade"].Value<string>();
                gamepadInput.SelectedItem = maps_json_config[map_json_key]["Gamepad"].Value<string>();
                isDebug.Checked = maps_json_config[map_json_key]["is_debug"].Value<bool>();
            }
        }

        /* Save character to config */
        private void button1_Click(object sender, EventArgs e)
        {
            //All inputs required
            if (keybindName.Text == "" || keyboardInput.SelectedIndex == -1 || arcadeInput.SelectedIndex == -1 || gamepadInput.SelectedIndex == -1 ||
                (keyboardInput.SelectedIndex == 0 && arcadeInput.SelectedIndex == 0 && gamepadInput.SelectedIndex == 0))
            {
                MessageBox.Show("Please complete all fields before trying to save.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Load config if not already (non-edit mode)
            if (!edit_mode)
            {
                maps_json_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/KEYBINDS_CONFIG.JSON"));
            }

            //Format name
            string keybind_name = keybindName.Text.Replace(' ', '_').ToUpper();

            //Check for existing useages
            List<string> keyboard_useages = new List<string>();
            List<string> gamepad_useages = new List<string>();
            List<string> arcade_useages = new List<string>();
            bool name_confict = false;
            foreach (var keybind in maps_json_config)
            {
                if (keybind.Key == keybind_name)
                {
                    if (edit_mode)
                    {
                        continue;
                    }
                    else
                    {
                        name_confict = true;
                    }
                }
                if (keyboardInput.SelectedIndex != 0 && maps_json_config[keybind.Key]["Keyboard"].Value<string>() == keyboardInput.SelectedItem.ToString())
                {
                    keyboard_useages.Add(keybind.Key);
                }
                if (arcadeInput.SelectedIndex != 0 && maps_json_config[keybind.Key]["Arcade"].Value<string>() == arcadeInput.SelectedItem.ToString())
                {
                    arcade_useages.Add(keybind.Key);
                }
                if (gamepadInput.SelectedIndex != 0 && maps_json_config[keybind.Key]["Gamepad"].Value<string>() == gamepadInput.SelectedItem.ToString())
                {
                    gamepad_useages.Add(keybind.Key);
                }
            }

            //Our name already exists, pick another!
            if (name_confict)
            {
                MessageBox.Show("Keybind name already exists, please enter another.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Check if we want to continue if there are existing useages
            if (!useageWarning(keyboard_useages, "keyboard"))
            {
                return;
            }
            if (!useageWarning(gamepad_useages, "gamepad"))
            {
                return;
            }
            if (!useageWarning(arcade_useages, "arcade"))
            {
                return;
            }

            //Add to config
            if (!edit_mode) { maps_json_config[keybind_name] = JObject.Parse("{}"); } 
            maps_json_config[keybind_name]["Keyboard"] = keyboardInput.SelectedItem.ToString();
            maps_json_config[keybind_name]["Keyboard_Code"] = keyboardToKeyCode(keyboardInput.SelectedItem.ToString());
            maps_json_config[keybind_name]["Arcade"] = arcadeInput.SelectedItem.ToString();
            maps_json_config[keybind_name]["Arcade_Code"] = keyboardToKeyCode(arcadeToKeyboard(arcadeInput.SelectedItem.ToString()));
            maps_json_config[keybind_name]["Gamepad"] = gamepadInput.SelectedItem.ToString();
            maps_json_config[keybind_name]["is_debug"] = isDebug.Checked;

            //Save back out
            File.WriteAllText("DATA/CONFIGS/KEYBINDS_CONFIG.JSON", maps_json_config.ToString(Formatting.Indented));

            //Done
            MessageBox.Show("Saved keybind configuration!", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }

        /* Useage warning */
        bool useageWarning(List<string> useages, string useage_name)
        {
            if (useages.Count > 0)
            {
                string warning_msg = "The selected " + useage_name + " input is already in use in:\n";
                foreach (string useage in useages)
                {
                    warning_msg += useage + ", ";
                }
                warning_msg = warning_msg.Substring(0, warning_msg.Length - 2) + ".\nThis may cause conflicts in-game.\nDo you wish to continue?";
                DialogResult showErrorInfo = MessageBox.Show(warning_msg, "Selected " + useage_name + " input already in use!", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                return (showErrorInfo == DialogResult.Yes);
            }
            return true;
        }

        /* Convert an arcade input to a keyboard input */
        string arcadeToKeyboard(string arcade_name)
        {
            switch(arcade_name)
            {
                case "Left Joystick Up":
                    return "Up";
                case "Left Joystick Down":
                    return "Down";
                case "Left Joystick Left":
                    return "Left";
                case "Left Joystick Right":
                    return "Right";
                case "Left Blue Button":
                    return "LeftControl";
                case "Left Yellow Button":
                    return "LeftAlt";
                case "Left Grey Top Button":
                    return "Space";
                case "Left Green Button":
                    return "LeftShift";
                case "Left Red Button":
                    return "Z";
                case "Left Grey Bottom Button":
                    return "X";
                case "Left Side Button":
                    return "C";

                case "Right Joystick Up":
                    return "R";
                case "Right Joystick Down":
                    return "F";
                case "Right Joystick Left":
                    return "D";
                case "Right Joystick Right":
                    return "G";
                case "Right Blue Button":
                    return "A";
                case "Right Yellow Button":
                    return "S";
                case "Right Grey Top Button":
                    return "Q";
                case "Right Green Button":
                    return "W";
                case "Right Red Button":
                    return "I";
                case "Right Grey Bottom Button":
                    return "K";
                case "Right Side Button":
                    return "J";
            }

            return "";
        }

        /* Convert a keyboard key name to keyboard key code */
        int keyboardToKeyCode(string key_name)
        {
            switch(key_name)
            {
                case "Back":
	                return 8;
                case "Tab":
	                return 9;
                case "Enter":
	                return 13;
                case "Pause":
	                return 19;
                case "CapsLock":
	                return 20;
                case "Escape":
	                return 27;
                case "Space":
	                return 32;
                case "LeftWindows":
	                return 91;
                case "Left":
	                return 37;
                case "Up":
	                return 38;
                case "Right":
	                return 39;
                case "Down":
	                return 40;
                case "PageUp":
	                return 33;
                case "PageDown":
	                return 34;
                case "End":
	                return 35;
                case "Home":
	                return 36;
                case "PrintScreen":
	                return 44;
                case "Insert":
	                return 45;
                case "Delete":
	                return 46;
                case "A":
	                return 65;
                case "B":
	                return 66;
                case "C":
	                return 67;
                case "D":
	                return 68;
                case "E":
	                return 69;
                case "F":
	                return 70;
                case "G":
                    return 71;
                case "H":
	                return 72;
                case "I":
	                return 73;
                case "J":
                    return 74;
                case "K":
	                return 75;
                case "L":
	                return 76;
                case "M":
	                return 77;
                case "N":
	                return 78;
                case "O":
	                return 79;
                case "P":
	                return 80;
                case "Q":
	                return 81;
                case "R":
	                return 82;
                case "S":
	                return 83;
                case "T":
	                return 84;
                case "U":
	                return 85;
                case "V":
	                return 86;
                case "W":
	                return 87;
                case "X":
	                return 88;
                case "Y":
	                return 89;
                case "Z":
	                return 90;
                case "0":
                    return 48;
                case "1":
                    return 49;
                case "2":
                    return 50;
                case "3":
                    return 51;
                case "4":
                    return 52;
                case "5":
                    return 53;
                case "6":
                    return 54;
                case "7":
                    return 55;
                case "8":
                    return 56;
                case "9":
                    return 57;
                case "NumPad0":
	                return 96;
                case "NumPad1":
	                return 97;
                case "NumPad2":
	                return 98;
                case "NumPad3":
	                return 99;
                case "NumPad4":
	                return 100;
                case "NumPad5":
	                return 101;
                case "NumPad6":
	                return 102;
                case "NumPad7":
	                return 103;
                case "NumPad8":
	                return 104;
                case "NumPad9":
	                return 105;
                case "Multiply":
	                return 106;
                case "Add":
	                return 107;
                case "Separator":
	                return 108;
                case "Subtract":
	                return 109;
                case "Decimal":
	                return 110;
                case "Divide":
	                return 111;
                case "F1":
	                return 112;
                case "F2":
	                return 113;
                case "F3":
	                return 114;
                case "F4":
	                return 115;
                case "F5":
	                return 116;
                case "F6":
	                return 117;
                case "F7":
	                return 118;
                case "F8":
	                return 119;
                case "F9":
	                return 120;
                case "F10":
	                return 121;
                case "F11":
	                return 122;
                case "F12":
	                return 123;
                case "F13":
	                return 124;
                case "F14":
	                return 125;
                case "F15":
	                return 126;
                case "F16":
	                return 127;
                case "F17":
	                return 128;
                case "F18":
	                return 129;
                case "F19":
	                return 130;
                case "F20":
	                return 131;
                case "F21":
	                return 132;
                case "F22":
	                return 133;
                case "F23":
	                return 134;
                case "F24":
	                return 135;
                case "NumLock":
	                return 144;
                case "Scroll":
	                return 145;
                case "LeftShift":
	                return 160;
                case "RightShift":
	                return 161;
                case "LeftControl":
	                return 162;
                case "RightControl":
	                return 163;
                case "LeftAlt":
	                return 164;
                case "RightAlt":
	                return 165;
                case "OemSemicolon":
	                return 186;
                case "OemPlus":
	                return 187;
                case "OemComma":
	                return 188;
                case "OemMinus":
	                return 189;
                case "OemPeriod":
	                return 190;
                case "OemQuestion":
                    return 191;
                case "OemTilde":
	                return 192;
                case "OemOpenBrackets":
	                return 219;
                case "OemPipe":
	                return 220;
                case "OemCloseBrackets":
	                return 221;
                case "OemQuotes":
	                return 222;
                case "OemBackslash":
                    return 226;
            }

            return -1;
        }
    }
}
