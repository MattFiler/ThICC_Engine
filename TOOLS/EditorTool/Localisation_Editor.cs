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
    public partial class Localisation_Editor : Form
    {
        JObject localisation_config;
        string edit_key = "";
        string success_message = "String successfully created.";
        public Localisation_Editor(string string_to_edit = "")
        {
            edit_key = string_to_edit;
            InitializeComponent();
            localisation_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/LOCALISATION.JSON"));
        }

        /* Load for editor */
        private void Localisation_Editor_Load(object sender, EventArgs e)
        {
            if (edit_key != "")
            {
                stringIdentifier.Text = edit_key;
                stringIdentifier.ReadOnly = true;
                stringContent.Text = localisation_config["ENGLISH"][edit_key].Value<string>();
                createNewString.Text = "Save";
                success_message = "String successfully edited!";
            }
        }

        /* Save new string */
        private void createNewString_Click(object sender, EventArgs e)
        {
            localisation_config["ENGLISH"][stringIdentifier.Text] = stringContent.Text;
            File.WriteAllText("DATA/CONFIGS/LOCALISATION.JSON", localisation_config.ToString(Formatting.Indented));
            MessageBox.Show(success_message, "Created!", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }
    }
}
