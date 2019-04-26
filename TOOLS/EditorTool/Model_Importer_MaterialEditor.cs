using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
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
        string path_to_rma = "";
        int[] default_rma_vals = { 0, 0, 0 };
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

            //Ambient Colour (RGB)
            common_functions.loadMaterialColourPreview(material_config, "Ka", ambientColour);

            //Diffuse Colour (RGB)
            common_functions.loadMaterialColourPreview(material_config, "Kd", diffuseColour);

            //Specular Colour (RGB)
            common_functions.loadMaterialColourPreview(material_config, "Ks", specularColour);

            //Emissive Colour (RGB)
            common_functions.loadMaterialColourPreview(material_config, "Ke", emissiveColour);

            //Transparency (1 = completely invisible)
            transparencySlider.Value = Convert.ToInt32(Convert.ToSingle(material_config["Tr"].Value<string>()) * 10);
            transparencyValue.Text = sliderToString(transparencySlider, 10);

            //Shininess (0-1000)
            specExSlider.Value = Convert.ToInt32(Convert.ToSingle(material_config["Ns"].Value<string>()));
            specExValue.Text = sliderToString(specExSlider);

            //Specular on/off
            hasSpec.Checked = (material_config["illum"].Value<string>() == "2");

            //Diffuse Texture
            diffuseMap.Text = material_config["map_Kd"].Value<string>();

            //Specular Texture
            specularMap.Text = material_config["map_Ks"].Value<string>();
            if (specularMap.Text == "spec_placeholder.png")
            {
                specularMap.Text = "";
            }

            //Normal Texture
            normalMap.Text = material_config["map_Kn"].Value<string>();
            if (normalMap.Text == "")
            {
                normalMap.Text = material_config["norm"].Value<string>();
            }
            if (normalMap.Text == "norm_placeholder.png")
            {
                normalMap.Text = "";
            }

            //Emissive Texture
            emissiveMap.Text = material_config["map_Ke"].Value<string>();
            if (emissiveMap.Text == "")
            {
                emissiveMap.Text = material_config["map_emissive"].Value<string>();
            }
            if (emissiveMap.Text == "emm_placeholder.png")
            {
                emissiveMap.Text = "";
            }

            //RMA values
            path_to_rma = material_config["map_RMA"].Value<string>();
            if (path_to_rma == "")
            {
                path_to_rma = material_config["map_occlusionRoughnessMetallic"].Value<string>();
                if (path_to_rma == "")
                {
                    path_to_rma = "rma_placeholder.png";
                }
            }
            Bitmap rma_image = new Bitmap(this_model_folder + path_to_rma);
            var rma_pixel = rma_image.GetPixel(0, 0);
            metalnessSlider.Value = rma_pixel.B;
            roughnessSlider.Value = rma_pixel.G;
            ambientocclusionSlider.Value = rma_pixel.R;
            default_rma_vals[0] = metalnessSlider.Value;
            default_rma_vals[1] = roughnessSlider.Value;
            default_rma_vals[2] = ambientocclusionSlider.Value;

            //Before getting to the engine config, update any depreciated stuff
            if (material_config["ThICC_COLLISION"][((int)CollisionType.ON_TRACK_NO_AI).ToString()] == null)
            {
                material_config["ThICC_COLLISION"][((int)CollisionType.ON_TRACK_NO_AI).ToString()] = false;
            }
            if (material_config["ThICC_COLLISION"][((int)CollisionType.ANTIGRAV_PAD).ToString()] == null)
            {
                material_config["ThICC_COLLISION"][((int)CollisionType.ANTIGRAV_PAD).ToString()] = false;
            }
            if (material_config["ThICC_COLLISION"][((int)CollisionType.JUMP_PAD).ToString()] == null)
            {
                material_config["ThICC_COLLISION"][((int)CollisionType.JUMP_PAD).ToString()] = false;
            }

            /* Engine Config */
            if (model_type == ModelType.MAP)
            {
                //Collision config
                if (material_config["ThICC_COLLISION"][((int)CollisionType.ON_TRACK).ToString()].Value<bool>())
                {
                    onTrack.Checked = true;
                }
                else if (material_config["ThICC_COLLISION"][((int)CollisionType.ON_TRACK_NO_AI).ToString()].Value<bool>())
                {
                    onTrack.Checked = true;
                    onTrackNoAI.Checked = true;
                }
                else if (material_config["ThICC_COLLISION"][((int)CollisionType.OFF_TRACK).ToString()].Value<bool>())
                {
                    offTrack.Checked = true;
                }
                else if (material_config["ThICC_COLLISION"][((int)CollisionType.BOOST_PAD).ToString()].Value<bool>())
                {
                    boostPad.Checked = true;
                }
                else if (material_config["ThICC_COLLISION"][((int)CollisionType.ANTIGRAV_PAD).ToString()].Value<bool>())
                {
                    antiGravPad.Checked = true;
                }
                else if (material_config["ThICC_COLLISION"][((int)CollisionType.JUMP_PAD).ToString()].Value<bool>())
                {
                    jumpPad.Checked = true;
                }
                else if (material_config["ThICC_COLLISION"][((int)CollisionType.WALL).ToString()].Value<bool>())
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
            //Metallic
            isMetallic.Checked = material_config["ThICC_METALLIC"].Value<bool>();
        }

        /* Collision selection */
        private void inPlayableArea_CheckedChanged(object sender, EventArgs e)
        {
            collisionCheckChanged(inPlayableArea.Checked);
        }
        private void collisionCheckChanged(bool enabled)
        {
            onTrackNoAI.Checked = false;
            onTrackNoAI.Enabled = false;
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
            emissiveMap.Text = common_functions.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
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

            //Ambient Colour (RGB)
            colourToJSON("Ka", ambientColour);

            //Diffuse Colour (RGB)
            colourToJSON("Kd", diffuseColour);

            //Specular Colour (RGB)
            colourToJSON("Ks", specularColour);

            //Emissive Colour (RGB)
            colourToJSON("Ke", emissiveColour);

            /* Some more work needs to be done to work out the effects of transparency config and its issue with alphas */
            /* Maybe just resort back to how the auto-detect button handles this? */

            //Alpha (0.5+ = has alpha)
            //material_config["d"] = (common_functions.hasTransparency(diffuseMap.Text) ? "0.999999" : "0.000000");
            /* ^ this is now done later to save performance */

            //Transparency (1 = completely invisible)
            material_config["Tr"] = (Convert.ToSingle(transparencySlider.Value) / 10.0f).ToString("0.000000");

            //Shininess (0-1000)
            material_config["Ns"] = specExSlider.Value.ToString("0.000000");

            //Specular on/off (2=on)
            material_config["illum"] = (hasSpec.Checked ? "2" : "0");

            //Diffuse Texture
            material_config["map_Kd"] = Path.GetFileName(diffuseMap.Text);
            copyNewMat(diffuseMap.Text);

            //Specular Texture
            material_config["map_Ks"] = Path.GetFileName(specularMap.Text);
            copyNewMat(specularMap.Text);

            //Normal Texture
            material_config["map_Kn"] = Path.GetFileName(normalMap.Text);
            material_config["norm"] = Path.GetFileName(normalMap.Text);
            copyNewMat(normalMap.Text);
            
            //Emissive Texture
            material_config["map_Ke"] = Path.GetFileName(emissiveMap.Text);
            material_config["map_emissive"] = Path.GetFileName(emissiveMap.Text);
            copyNewMat(emissiveMap.Text);

            //RMA Texture
            string rma = path_to_rma;
            if (metalnessSlider.Value != default_rma_vals[0] || 
                ambientocclusionSlider.Value != default_rma_vals[2] || 
                roughnessSlider.Value != default_rma_vals[1])
            {
                rma = createRMA();
            }
            material_config["map_RMA"] = rma;
            material_config["map_occlusionRoughnessMetallic"] = rma;

            //Collision config
            material_config["ThICC_COLLISION"][((int)CollisionType.ON_TRACK).ToString()] = (inPlayableArea.Checked ? (onTrack.Checked ? !onTrackNoAI.Checked : false) : false);
            material_config["ThICC_COLLISION"][((int)CollisionType.ON_TRACK_NO_AI).ToString()] = (inPlayableArea.Checked ? (onTrack.Checked ? onTrackNoAI.Checked : false) : false);
            material_config["ThICC_COLLISION"][((int)CollisionType.OFF_TRACK).ToString()] = (inPlayableArea.Checked ? offTrack.Checked : false);
            material_config["ThICC_COLLISION"][((int)CollisionType.BOOST_PAD).ToString()] = (inPlayableArea.Checked ? boostPad.Checked : false);
            material_config["ThICC_COLLISION"][((int)CollisionType.ANTIGRAV_PAD).ToString()] = (inPlayableArea.Checked ? antiGravPad.Checked : false);
            material_config["ThICC_COLLISION"][((int)CollisionType.JUMP_PAD).ToString()] = (inPlayableArea.Checked ? jumpPad.Checked : false);
            material_config["ThICC_COLLISION"][((int)CollisionType.WALL).ToString()] = (inPlayableArea.Checked ? isWall.Checked : false);

            //Metallic
            material_config["ThICC_METALLIC"] = isMetallic.Checked;

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
            material_config[key] = (Convert.ToSingle(colour.BackColor.R) / 255.0f).ToString("0.000000") + " " + (Convert.ToSingle(colour.BackColor.G) / 255.0f).ToString("0.000000") + " " + (Convert.ToSingle(colour.BackColor.B) / 255.0f).ToString("0.000000");
        }
        
        /* Copy new material to our model's directory */
        private void copyNewMat(string file_path)
        {
            if (file_path != "")
            {
                string copy_to = this_model_folder + Path.GetFileName(file_path);
                if (!File.Exists(copy_to))
                {
                    File.Copy(file_path, copy_to);
                }
            }
        }

        /* Create our RMA material from slider values (keeping the option to define a pre-made RMA would be nice) */
        private string createRMA()
        {
            Bitmap temp_rma = new Bitmap(1, 1);
            temp_rma.SetPixel(0, 0, Color.FromArgb(255, ambientocclusionSlider.Value, roughnessSlider.Value, metalnessSlider.Value));
            string rma_mat = materialName.Text.Replace(' ', '_') + "_occlusionRoughnessMetallic.png";
            if (File.Exists(this_model_folder + rma_mat))
            {
                File.Delete(this_model_folder + rma_mat);
            }
            temp_rma.Save(this_model_folder + rma_mat, ImageFormat.Png);
            return rma_mat;
        }

        /* Enable "navmesh" toggle for on-track mats */
        private void onTrack_CheckedChanged(object sender, EventArgs e)
        {
            onTrackNoAI.Checked = false;
            onTrackNoAI.Enabled = onTrack.Checked;
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
        private void RMAMapBrowse_Click(object sender, EventArgs e)
        {

        }

    }
}
