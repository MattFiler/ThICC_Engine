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
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Image_Importer : Form
    {
        public Image_Importer()
        {
            InitializeComponent();
        }

        /* Browse to asset */
        private void browseToImage_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                imagePath.Text = filePicker.FileName;
                if (assetName.Text == "")
                {
                    assetName.Text = Path.GetFileNameWithoutExtension(imagePath.Text);
                }
            }
        }

        /* Import image */
        private void importModel_Click(object sender, EventArgs e)
        {
            string asset_path = "DDS/" + assetName.Text + ".dds";
            string asset_path_orig_ext = "DDS/" + assetName.Text + Path.GetExtension(imagePath.Text);

            if (File.Exists(asset_path) || imagePath.Text == "" || assetName.Text == "" || !Regex.IsMatch(assetName.Text, "^[_a-zA-Z0-9\x20]+$"))
            {
                if (imagePath.Text == "" || assetName.Text == "")
                {
                    MessageBox.Show("Please fill out all required inputs.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else if (!Regex.IsMatch(assetName.Text, "^[_a-zA-Z0-9\x20]+$"))
                {
                    MessageBox.Show("Your asset name cannot contain any special characters.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                MessageBox.Show("Couldn't import asset, an image with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                //Copy file to "working directory"
                File.Copy(imagePath.Text, asset_path_orig_ext);

                //Convert copied image to DDS
                ProcessStartInfo imageConverter = new ProcessStartInfo();
                imageConverter.WorkingDirectory = "DDS";
                imageConverter.FileName = "DDS/texconv.exe";
                imageConverter.Arguments = "\"" + asset_path_orig_ext.Substring(4) + "\"";
                imageConverter.UseShellExecute = false;
                imageConverter.RedirectStandardOutput = true;
                Process converterProcess = Process.Start(imageConverter);
                StreamReader reader = converterProcess.StandardOutput;
                converterProcess.WaitForExit();

                //Capture DDS convert output incase we errored
                string output = reader.ReadToEnd();

                //Get dimensions then delete original file
                int image_width = 0;
                int image_height = 0;
                using (Image img = Image.FromFile(asset_path_orig_ext))
                {
                    image_width = img.Width;
                    image_height = img.Height;
                }
                File.Delete(asset_path_orig_ext);

                //Lowercase extension pls
                if (File.Exists(asset_path.Substring(0, asset_path.Length - 3) + "DDS"))
                {
                    File.Move(asset_path.Substring(0, asset_path.Length - 3) + "DDS", asset_path);
                }

                if (!File.Exists(asset_path))
                {
                    //Import failed, show reason if requested
                    DialogResult showErrorInfo = MessageBox.Show("Image import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                    if (showErrorInfo == DialogResult.Yes)
                    {
                        MessageBox.Show(output, "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else
                {
                    //Create JSON data
                    JToken asset_json = JToken.Parse("{\"asset_name\": \"" + assetName.Text + "\", \"asset_type\": \"Images\", \"visible\": true, \"is_2d\": true, \"res_x\": " + image_width + ", \"res_y\": " + image_height + ", \"x_pos\": 0, \"y_pos\": 0}");
                    File.WriteAllText(asset_path.Substring(0, asset_path.Length - 3) + "json", asset_json.ToString(Formatting.Indented));

                    //Import success
                    MessageBox.Show("Image successfully imported.", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }

                this.Close();
            }
        }
    }
}
