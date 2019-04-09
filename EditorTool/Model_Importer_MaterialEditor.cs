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

            //Collision config
            if (material_config["MARIOKART"]["is_boost_pad"].Value<bool>())
            {
                boostPad.Checked = true;
                inPlayableArea.Checked = true;
            }
            else if (material_config["MARIOKART"]["is_boost_pad"].Value<bool>())
            {
                boostPad.Checked = true;
                inPlayableArea.Checked = true;
            }
            else if (material_config["MARIOKART"]["is_boost_pad"].Value<bool>())
            {
                boostPad.Checked = true;
                inPlayableArea.Checked = true;
            }

            //Ambient
            if (material_config["map_Ka"] != null)
            {
                ambientMap.Text = material_config["map_Ka"].Value<string>();
            }
            common_functions.loadMaterialColourPreview(material_config, "Ka", ambientColour);

            //Diffuse
            if (material_config["map_Kd"] != null)
            {
                diffuseMap.Text = material_config["map_Kd"].Value<string>();
            }
            else
            {
                if (material_config["map_d"] != null)
                {
                    diffuseMap.Text = material_config["map_d"].Value<string>();
                }
            }
            common_functions.loadMaterialColourPreview(material_config, "Kd", diffuseColour);

            //Specular
            if (material_config["map_Ks"] != null)
            {
                ambientMap.Text = material_config["map_Ks"].Value<string>();
            }
            else
            {
                if (material_config["map_Ns"] != null)
                {
                    diffuseMap.Text = material_config["map_Ns"].Value<string>();
                }
            }
            common_functions.loadMaterialColourPreview(material_config, "Ks", specularColour);

            //Emissive
            common_functions.loadMaterialColourPreview(material_config, "Ke", emissiveColour);
        }

        private void saveMaterial_Click(object sender, EventArgs e)
        {
            //material_config
            DialogResult = DialogResult.OK; //Forces main window to only refresh if we save
        }

        /* Collision selection */
        private void inPlayableArea_CheckedChanged(object sender, EventArgs e)
        {
            collisionCheckChanged(inPlayableArea.Checked);
        }
        private void collisionCheckChanged(bool enabled)
        {
            onTrack.Checked = enabled;
            onTrack.Enabled = enabled;
            offTrack.Checked = false;
            offTrack.Enabled = enabled;
            boostPad.Checked = false;
            boostPad.Enabled = enabled;
        }

        /* Colour selection */
        private void diffuseColourBrowse_Click(object sender, EventArgs e)
        {
            common_functions.userChosenColour(diffuseColour);
        }
        private void ambientColourBrowse_Click(object sender, EventArgs e)
        {
            common_functions.userChosenColour(ambientColour);
        }
        private void specularColourBrowse_Click(object sender, EventArgs e)
        {
            common_functions.userChosenColour(specularColour);
        }
        private void emissiveColourBrowse_Click(object sender, EventArgs e)
        {
            common_functions.userChosenColour(emissiveColour);
        }

        /* Map file selection */
        private void diffuseMapBrowse_Click(object sender, EventArgs e)
        {
            diffuseMap.Text = common_functions.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
        }
        private void ambientMapBrowse_Click(object sender, EventArgs e)
        {
            ambientMap.Text = common_functions.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
        }
        private void specularMapBrowse_Click(object sender, EventArgs e)
        {
            specularMap.Text = common_functions.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
        }
        private void normalMapBrowse_Click(object sender, EventArgs e)
        {
            normalMap.Text = common_functions.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
        }



        // stuff to be deleted
        private void label5_Click(object sender, EventArgs e)
        {

        }
        private void label15_Click(object sender, EventArgs e)
        {

        }
    }
}
