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
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace EditorTool
{
    public partial class Sound_Importer : Form
    {
        public Sound_Importer()
        {
            InitializeComponent();
        }

        /* Browse to asset */
        private void browseToSound_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "Sound (WAV/MP3/OGG)|*.WAV;*.MP3;*.OGG";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                soundPath.Text = filePicker.FileName;
            }
        }

        /* Import sound */
        private void importSound_Click(object sender, EventArgs e)
        {
            string asset_path = "Sounds/" + assetName.Text + ".wav";
            string asset_path_orig_ext = "Sounds/" + assetName.Text + Path.GetExtension(soundPath.Text);

            if (File.Exists("Sounds/" + Path.GetFileName(soundPath.Text)) || soundPath.Text == "")
            {
                MessageBox.Show("Couldn't import sound, a sound with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                //Copy file to working directory
                File.Copy(soundPath.Text, asset_path_orig_ext);

                string output = "";
                if (Path.GetExtension(soundPath.Text).ToUpper() != ".WAV")
                {
                    //Convert file to WAV if it isn't already
                    ProcessStartInfo soundConverter = new ProcessStartInfo();
                    soundConverter.WorkingDirectory = "Sounds";
                    soundConverter.FileName = "Sounds/ffmpeg.exe";
                    soundConverter.Arguments = "-i \"" + asset_path_orig_ext.Substring(7) + "\" \"" + asset_path.Substring(7) + "\"";
                    soundConverter.UseShellExecute = false;
                    soundConverter.RedirectStandardOutput = true;
                    Process converterProcess = Process.Start(soundConverter);
                    StreamReader reader = converterProcess.StandardOutput;
                    converterProcess.WaitForExit();

                    //Capture DDS convert output incase we errored
                    output = reader.ReadToEnd();

                    File.Delete(asset_path_orig_ext);
                }

                if (!File.Exists(asset_path))
                {
                    //Conversion failed, show reason if requested
                    DialogResult showErrorInfo = MessageBox.Show("Sound import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                    if (showErrorInfo == DialogResult.Yes)
                    {
                        MessageBox.Show(output, "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else
                {
                    //Create JSON data
                    JToken asset_json = JToken.Parse("{\"asset_name\": \"" + assetName.Text + "\", \"asset_type\": \"Sounds\", \"is_looping\": false, \"volume\": 1.0, \"pitch\": 0.0, \"pan\": 0.0}");
                    File.WriteAllText(asset_path.Substring(0, asset_path.Length - 3) + "json", asset_json.ToString(Formatting.Indented));

                    //Import success
                    MessageBox.Show("Sound successfully imported.", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }

                soundPath.Text = "";
                assetName.Text = "";
            }
        }
    }
}
