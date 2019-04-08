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
    public partial class Model_Importer_MaterialList : Form
    {
        JObject model_material_config = new JObject();
        List<JToken> material_tokens = new List<JToken>();
        UsefulFunctions common_functions = new UsefulFunctions();
        string model_directory = "";
        string model_name = "";
        public Model_Importer_MaterialList(string _name)
        {
            model_name = _name.ToUpper();
            model_directory = "DATA/MODELS/" + model_name + "/";
            model_material_config = JObject.Parse(File.ReadAllText(model_directory + "model_materials.json"));
            InitializeComponent();
        }

        private void Model_Importer_pt2_Load(object sender, EventArgs e)
        {
            //Parse material data on load
            foreach (var material_object in model_material_config)
            {
                materialList.Items.Add(material_object.Key);
                material_tokens.Add(model_material_config[material_object.Key]);
            }
        }

        private void materialList_SelectedIndexChanged(object sender, EventArgs e)
        {
            //Only act if material selected
            int index = materialList.SelectedIndex;
            if (index == -1)
            {
                return;
            }

            //Update previews of config
            JToken this_token = material_tokens.ElementAt(index);
            isTrack.Checked = this_token["MARIOKART"]["is_on_track"].Value<bool>();
            isOffTrack.Checked = this_token["MARIOKART"]["is_off_track"].Value<bool>();
            isBoostPad.Checked = this_token["MARIOKART"]["is_boost_pad"].Value<bool>();

            //Try find and show our material preview.
            common_functions.loadMaterialPreview(this_token, materialPreview);
        }

        private void editMaterial_Click(object sender, EventArgs e)
        {
            //Only act if material selected
            int index = materialList.SelectedIndex;
            if (index == -1)
            {
                return;
            }

            using (var editor = new Model_Importer_MaterialEditor(material_tokens.ElementAt(index), materialList.SelectedItem.ToString()))
            {
                editor.ShowDialog();
                if (editor.DialogResult == DialogResult.OK)
                {
                    //refresh jobject
                }
            }
        }
    }
}
