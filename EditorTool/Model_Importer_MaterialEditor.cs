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
        ModelType model_type;
        public Model_Importer_MaterialEditor(JToken _config, ModelType _type)
        {
            material_config = _config;
            model_type = _type;
            InitializeComponent();
        }

        private void Model_Importer_MaterialEditor_Load(object sender, EventArgs e)
        {
            //Name and material preview
            materialName.Text = material_config["newmtl"].Value<string>();
            common_functions.loadMaterialPreview(material_config, materialPreview);

            if (model_type == ModelType.MAP)
            {
                //Collision config
                if (material_config["MARIOKART_COLLISION"]["0"].Value<bool>())
                {
                    onTrack.Checked = true;
                }
                else if (material_config["MARIOKART_COLLISION"]["1"].Value<bool>())
                {
                    offTrack.Checked = true;
                }
                else if (material_config["MARIOKART_COLLISION"]["2"].Value<bool>())
                {
                    boostPad.Checked = true;
                }
                else
                {
                    inPlayableArea.Checked = false;
                }
            }
            else
            {
                //Hide collision options for non-track models
                collisionGroup.Visible = false;
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

            //Transparency
            if (material_config["d"] != null)
            {
                transparencySlider.Value = Convert.ToInt32(material_config["d"].Value<float>() * 10);
            }
            else
            {
                transparencySlider.Value = 0;
            }
            transparencyValue.Text = sliderToString(transparencySlider, 10);

            //Specular exponent
            if (material_config["Ns"] != null)
            {
                specExSlider.Value = Convert.ToInt32(material_config["Ns"].Value<float>());
            }
            else
            {
                specExSlider.Value = 100;
            }
            specExValue.Text = sliderToString(specExSlider);

            //Illumination model
            if (material_config["illum"] != null)
            {
                illumModel.SelectedIndex = material_config["illum"].Value<int>();
            }
            else
            {
                illumModel.SelectedIndex = 2;
            }
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

        /* Slider interaction */
        private void transparencySlider_Scroll(object sender, EventArgs e)
        {
            transparencyValue.Text = sliderToString(transparencySlider, 10);
        }
        private void specExSlider_Scroll(object sender, EventArgs e)
        {
            specExValue.Text = sliderToString(specExSlider);
        }

        /* Save new config */
        private void button1_Click(object sender, EventArgs e)
        {
            //material_config

            //Collision config
            if (inPlayableArea.Checked)
            {
                material_config["MARIOKART_COLLISION"]["0"] = onTrack.Checked;
                material_config["MARIOKART_COLLISION"]["1"] = offTrack.Checked;
                material_config["MARIOKART_COLLISION"]["2"] = boostPad.Checked;
            }
            else
            {
                material_config["MARIOKART_COLLISION"]["0"] = false;
                material_config["MARIOKART_COLLISION"]["1"] = false;
                material_config["MARIOKART_COLLISION"]["2"] = false;
            }

            MessageBox.Show("Material edits saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        }
        
        /* Slider value to string for UI */
        private string sliderToString(TrackBar slider, int modifier = 1)
        {
            return (Convert.ToDouble(slider.Value) / modifier).ToString("0.0");
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
