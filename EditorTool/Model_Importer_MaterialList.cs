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

namespace EditorTool
{
    public partial class Model_Importer_MaterialList : Form
    {
        JObject model_material_config = new JObject();
        List<JToken> material_tokens = new List<JToken>();
        UsefulFunctions common_functions = new UsefulFunctions();
        Model_Importer_Common importer_common;
        public Model_Importer_MaterialList(Model_Importer_Common _importer_conf)
        {
            importer_common = _importer_conf;
            model_material_config = JObject.Parse(File.ReadAllText(importer_common.fileName(importer_file.CONFIG)));
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

            //Collision properties only visible for map
            if (importer_common.getModelType() != ModelType.MAP)
            {
                configPreview.Visible = false;
                editMaterial.Location = new Point(272, 125);
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

            using (var editor = new Model_Importer_MaterialEditor(material_tokens.ElementAt(index), importer_common.getModelType()))
            {
                editor.ShowDialog();
                if (editor.DialogResult == DialogResult.OK)
                {
                    //refresh jobject
                }
            }
        }

        private void SaveMaterials_Click(object sender, EventArgs e)
        {
            //Save json config back
            File.WriteAllText(importer_common.fileName(importer_file.CONFIG), model_material_config.ToString(Formatting.Indented));

            //------

            //Rewrite MTL from json
            List<string> new_mtl = new List<string>();
            new_mtl.Add("### CREATED BY THE MARIO KART TOOLKIT ###");
            new_mtl.Add("");
            foreach (var this_material_config in model_material_config)
            {
                foreach (JProperty material_prop in model_material_config[this_material_config.Key])
                {
                    //MarioKart config
                    if (material_prop.Name == "MARIOKART")
                    {
                        //add to some kind of array for getting trongle data
                    }
                    //Material config to write
                    else
                    {
                        //Fix transparency issue
                        if (material_prop.Name == "d" && material_prop.Value.Value<string>() == "1.000000")
                        {
                            material_prop.Value = "0.999999";
                        }
                        //Write config to mtl list
                        new_mtl.Add(material_prop.Name + " " + material_prop.Value);
                    }
                }
                new_mtl.Add("");
            }
            File.Delete(importer_common.fileName(importer_file.MATERIAL));
            File.WriteAllLines(importer_common.fileName(importer_file.MATERIAL), new_mtl);

            //------

            //Run the model converter to swap our OBJ into an SDKMESH
            string conv_args = "\"" + Path.GetFileName(importer_common.fileName(importer_file.MODEL)) + "\" -sdkmesh -nodds -y";
            if (shouldFlipUVs.Checked)
            {
                conv_args += " -flipv";
            }
            this.Cursor = Cursors.WaitCursor;
            ProcessStartInfo meshConverter = new ProcessStartInfo();
            meshConverter.WorkingDirectory = importer_common.importDir();
            meshConverter.FileName = "DATA/MODELS/meshconvert.exe";
            meshConverter.Arguments = conv_args;
            meshConverter.UseShellExecute = false;
            meshConverter.RedirectStandardOutput = true;
            meshConverter.CreateNoWindow = true;
            Process converterProcess = Process.Start(meshConverter);
            StreamReader reader = converterProcess.StandardOutput;
            converterProcess.WaitForExit();
            this.Cursor = Cursors.Default;

            //Capture write info from converter
            string output = reader.ReadToEnd();
            string[] outputArray = output.Split('\n');
            string writeInfo = "";
            foreach (string line in outputArray)
            {
                if (line.Contains("written"))
                {
                    writeInfo = line.Substring(0, line.Length - 2);
                    break;
                }
            }

            //------

            //Done
            //File.Delete(importer_common.fileName(importer_file.CONFIG));
            MessageBox.Show("Model import complete.", "Imported.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }
    }
}
