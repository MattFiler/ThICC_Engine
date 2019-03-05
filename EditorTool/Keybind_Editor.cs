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
    public partial class Keybind_Editor : Form
    {
        public Keybind_Editor()
        {
            InitializeComponent();
        }
        
        List<TextBox> textboxes = new List<TextBox>();
        List<ComboBox> dropdowns = new List<ComboBox>();
        int y_offset = 70;
        private void Keybind_Editor_Load(object sender, EventArgs e)
        {
            JObject keybind_json = JObject.Parse(File.ReadAllText("DATA/CONFIGS/KEYBINDS.JSON"));

            //Load existing config
            foreach (var keybind in keybind_json)
            {
                addNewInputs(keybind.Key, keybind.Value.ToString());
            }

            if (y_offset == 70)
            {
                //Should never get here, but adds a default keybind if we have none.
                addNewInputs();
            }
        }

        /* Add Input */
        private void button1_Click(object sender, EventArgs e)
        {
            addNewInputs();
        }

        /* Remove Input */
        private void button2_Click(object sender, EventArgs e)
        {
            if (textboxes.Count() > 1)
            {
                Controls.Remove(textboxes[textboxes.Count() - 1]);
                Controls.Remove(dropdowns[dropdowns.Count() - 1]);
                textboxes.Remove(textboxes[textboxes.Count() - 1]);
                dropdowns.Remove(dropdowns[dropdowns.Count() - 1]);
                y_offset -= 25;
                resizeWindow();
            }
            else
            {
                MessageBox.Show("You must have at least one keybind!", "Error.", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /* Save Keybinds */
        private void saveKeybinds_Click(object sender, EventArgs e)
        {
            //Remove existing binds
            JObject keybind_json = JObject.Parse("{}");

            //Add new binds
            bool encounteredError = false;
            List<string> bind_names = new List<string>();
            for (int i=0; i<textboxes.Count(); i++)
            {
                if (dropdowns[i].SelectedIndex == -1 || textboxes[i].Text == "")
                {
                    MessageBox.Show("Please fill out all added keybinds!", "Error.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    encounteredError = true;
                    break;
                }
                else
                {
                    bool should_save = true;
                    foreach (string bind in bind_names)
                    {
                        if (bind == textboxes[i].Text)
                        {
                            should_save = false;
                        }
                    }
                    if (should_save)
                    {
                        keybind_json[textboxes[i].Text] = dropdowns[i].Items[dropdowns[i].SelectedIndex].ToString();
                        bind_names.Add(textboxes[i].Text);
                    }
                    else
                    {
                        MessageBox.Show("All keybind names must be unique!", "Error.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        encounteredError = true;
                        break;
                    }
                }
            }

            //If no errors, save
            if (!encounteredError)
            {
                File.WriteAllText("DATA/CONFIGS/KEYBINDS.JSON", keybind_json.ToString(Formatting.Indented));
                MessageBox.Show("Keybinds saved!", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.Close();
            }
        }

        /* Add new keybind inputs */
        void addNewInputs(string keybind_name = "", string keybind_selected = "")
        {
            //New textbox
            TextBox newTextBox = new TextBox();
            newTextBox.Name = "ActionName_" + textboxes.Count();
            newTextBox.Size = new Size(243, 20);
            newTextBox.Location = new Point(12, y_offset);
            newTextBox.Text = keybind_name;
            textboxes.Add(newTextBox);
            Controls.Add(newTextBox);

            //New dropdown
            ComboBox newDropdown = new ComboBox();
            newDropdown.Name = "InputName_" + dropdowns.Count();
            newDropdown.Size = new Size(158, 21);
            newDropdown.Location = new Point(264, y_offset);
            newDropdown.DropDownStyle = ComboBoxStyle.DropDownList;
            newDropdown.Items.AddRange(new object[] {
                "None", "Back", "Tab", "Enter", "Pause", "CapsLock", "Kana", "Kanji", "Escape", "ImeConvert", "ImeNoConvert",
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
            newDropdown.SelectedItem = keybind_selected;
            dropdowns.Add(newDropdown);
            Controls.Add(newDropdown);

            y_offset += 25;
            resizeWindow();
        }

        /* Resize Window */
        void resizeWindow()
        {
            Size = new Size(449, y_offset + 90);
            saveKeybinds.Location = new Point(12, y_offset + 15);
        }
    }
}
