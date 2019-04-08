using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Model_Importer_MaterialEditor : Form
    {
        JToken material_config;
        UsefulFunctions common_functions = new UsefulFunctions();
        string material_name = "";
        public Model_Importer_MaterialEditor(JToken _config, string _key)
        {
            material_config = _config;
            material_name = _key;
            InitializeComponent();
        }

        private void Model_Importer_MaterialEditor_Load(object sender, EventArgs e)
        {
            materialName.Text = material_name;
            common_functions.loadMaterialPreview(material_config, materialPreview);
        }

        private void saveMaterial_Click(object sender, EventArgs e)
        {
            //material_config
            DialogResult = DialogResult.OK; //Forces main window to only refresh if we save
        }
    }
}
