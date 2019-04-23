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
    public partial class Keybinds_Manager : Form
    {
        UsefulFunctions common_functions = new UsefulFunctions();
        JObject comp_json_config;
        public Keybinds_Manager()
        {
            InitializeComponent();

            refreshList();
        }

        /* On load */
        private void Keybind_Editor_New_Load(object sender, EventArgs e)
        {

        }

        /* Create new */
        private void newKeybind_Click(object sender, EventArgs e)
        {
            Keybinds_Editor new_editor = new Keybinds_Editor();
            new_editor.FormClosed += new FormClosedEventHandler(refreshList);
            new_editor.Show();
        }

        /* Edit selected */
        private void editKeybind_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select a keybind from the list to edit.", "No keybind selected.", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            Keybinds_Editor new_editor = new Keybinds_Editor(comp_json_config, assetList.SelectedItem.ToString());
            new_editor.FormClosed += new FormClosedEventHandler(refreshList);
            new_editor.Show();
        }

        /* Delete selected */
        private void deleteKeybind_Click(object sender, EventArgs e)
        {
            if (assetList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select a keybind from the list to delete.", "No keybind selected.", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            //Confirm
            DialogResult showErrorInfo = MessageBox.Show("Are you sure you want to delete this keybind?", "About to delete selected keybind...", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (showErrorInfo != DialogResult.Yes)
            {
                return;
            }

            //Remove JToken from config
            comp_json_config.Remove(assetList.SelectedItem.ToString());
            File.WriteAllText("DATA/CONFIGS/KEYBINDS_CONFIG.JSON", comp_json_config.ToString(Formatting.Indented));
            MessageBox.Show("Keybind successfully deleted!", "Deleted.", MessageBoxButtons.OK, MessageBoxIcon.Information);

            //Refresh
            refreshList();
        }

        /* Refresh list */
        private void refreshList(object sender, EventArgs e)
        {
            refreshList();
        }
        private void refreshList()
        {
            comp_json_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/KEYBINDS_CONFIG.JSON"));
            assetList.Items.Clear();
            foreach (var config_entry in comp_json_config)
            {
                assetList.Items.Add(config_entry.Key.ToString());
            }
        }

        /* Show preview on selection */
        private void assetList_SelectedIndexChanged(object sender, EventArgs e)
        {
            keybindPreview.Hide();
            if (assetList.SelectedIndex == -1)
            {
                return;
            }
            keybindPreview.Show();
            
            kbPreview.Text = comp_json_config[assetList.SelectedItem]["Keyboard"].Value<string>();
            gpPreview.Text = comp_json_config[assetList.SelectedItem]["Gamepad"].Value<string>();
            arPreview.Text = comp_json_config[assetList.SelectedItem]["Arcade"].Value<string>();
            isDebug.Checked = comp_json_config[assetList.SelectedItem]["is_debug"].Value<bool>();
        }
    }
}
