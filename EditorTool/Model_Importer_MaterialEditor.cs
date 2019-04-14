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
    public partial class Model_Importer_MaterialEditor : Form
    {
        /*
         * 
         * The CollisionType stuff is hard-coded here for the UI, whereas the rest of the tool will scale to the UI.
         * Don't forget to update this UI if changing CollisionTypes.
         * 
         */

        JToken material_config;
        UsefulFunctions common_functions = new UsefulFunctions();
        string this_model_folder;
        ModelType model_type;
        public Model_Importer_MaterialEditor(JToken _config, ModelType _type, string _folder)
        {
            material_config = _config;
            model_type = _type;
            this_model_folder = _folder;
            InitializeComponent();
        }

        private void Model_Importer_MaterialEditor_Load(object sender, EventArgs e)
        {
            //Name and material preview
            materialName.Text = material_config["newmtl"].Value<string>();
            common_functions.loadMaterialPreview(material_config, materialPreview, this_model_folder);

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
                else if (material_config["MARIOKART_COLLISION"]["3"].Value<bool>())
                {
                    isWall.Checked = true;
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
            if (!common_functions.loadMaterialColourPreview(material_config, "Ka", ambientColour))
            {
                ambientColour.BackColor = Color.White;
            }

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
                specularMap.Text = material_config["map_Ks"].Value<string>();
            }
            else
            {
                if (material_config["map_Ns"] != null)
                {
                    specularMap.Text = material_config["map_Ns"].Value<string>();
                }
            }
            common_functions.loadMaterialColourPreview(material_config, "Ks", specularColour);

            //Emissive
            common_functions.loadMaterialColourPreview(material_config, "Ke", emissiveColour);

            //Transparency
            transparencySlider.Value = Convert.ToInt32(material_config["d"].Value<float>() * 10);
            transparencyValue.Text = sliderToString(transparencySlider, 10);

            //Specular exponent
            specExSlider.Value = Convert.ToInt32(material_config["Ns"].Value<float>());
            specExValue.Text = sliderToString(specExSlider);

            //Illumination model
            illumModel.SelectedIndex = material_config["illum"].Value<int>();
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
            isWall.Checked = false;
            isWall.Enabled = enabled;
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
            if (diffuseMap.Text == "")
            {
                MessageBox.Show("All materials must have a diffuse map.", "Could not save changes.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Ambient
            material_config["map_Ka"] = ambientMap.Text;
            copyNewMat(ambientMap.Text);
            colourToJSON("Ka", ambientColour);

            //Diffuse
            material_config["map_Kd"] = diffuseMap.Text;
            material_config["map_d"] = diffuseMap.Text;
            copyNewMat(diffuseMap.Text);
            colourToJSON("Kd", diffuseColour);

            //Specular
            material_config["map_Ks"] = specularMap.Text;
            material_config["map_Ns"] = specularMap.Text;
            copyNewMat(specularMap.Text);
            colourToJSON("Ks", specularColour);

            //Emissive
            colourToJSON("Ke", emissiveColour);

            //Specular exponent
            material_config["Ns"] = specExSlider.Value.ToString("0.000000");
            
            //Illumination configuration
            material_config["illum"] = illumModel.SelectedIndex;

            //Transparency
            material_config["d"] = (transparencySlider.Value / 10).ToString("0.000000");
            
            //Collision config
            if (inPlayableArea.Checked)
            {
                material_config["MARIOKART_COLLISION"]["0"] = onTrack.Checked;
                material_config["MARIOKART_COLLISION"]["1"] = offTrack.Checked;
                material_config["MARIOKART_COLLISION"]["2"] = boostPad.Checked;
                material_config["MARIOKART_COLLISION"]["3"] = isWall.Checked;
            }
            else
            {
                material_config["MARIOKART_COLLISION"]["0"] = false;
                material_config["MARIOKART_COLLISION"]["1"] = false;
                material_config["MARIOKART_COLLISION"]["2"] = false;
                material_config["MARIOKART_COLLISION"]["3"] = false;
            }

            MessageBox.Show("Material edits saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            DialogResult = DialogResult.OK; 
            this.Close();
        }
        
        /* Slider value to string for UI */
        private string sliderToString(TrackBar slider, int modifier = 1)
        {
            return (Convert.ToDouble(slider.Value) / modifier).ToString("0.0");
        }

        /* Colour box to JSON data */
        private void colourToJSON(string key, PictureBox colour)
        {
            material_config[key] = (colour.BackColor.R/255).ToString("0.000000") + " " + (colour.BackColor.G/255).ToString("0.000000") + " " + (colour.BackColor.B/255).ToString("0.000000");
        }
        
        /* Copy new material to our model's directory */
        private void copyNewMat(string file_path)
        {
            if (file_path != "")
            {
                string copy_to = this_model_folder + Path.GetFileName(file_path);
                if (File.Exists(copy_to))
                {
                    File.Delete(copy_to);
                }
                File.Copy(file_path, copy_to);
            }
        }



        // stuff to be deleted
        private void label5_Click(object sender, EventArgs e)
        {

        }
        private void label15_Click(object sender, EventArgs e)
        {

        }
        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }
        private void saveMaterial_Click(object sender, EventArgs e)
        {
        }
    }
}
