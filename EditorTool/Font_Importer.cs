using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Font_Importer : Form
    {
        public Font_Importer()
        {
            InitializeComponent();
        }

        //Form load
        private void Font_Importer_Load(object sender, EventArgs e)
        {
            //Load all system fonts
            //Thanks: https://stackoverflow.com/a/8657854/3798962
            using (InstalledFontCollection fontsCollection = new InstalledFontCollection())
            {
                FontFamily[] fontFamilies = fontsCollection.Families;
                foreach (FontFamily font in fontFamilies)
                {
                    installedFonts.Items.Add(font.Name);
                }
            }
        }

        //Import font button
        private void importFont_Click(object sender, EventArgs e)
        {
            string asset_path = "DATA/FONTS/" + installedFonts.SelectedItem.ToString().Replace(' ', '_').ToUpper() + ".SPRITEFONT";

            if (File.Exists(asset_path) || installedFonts.SelectedItem.ToString() == "" || installedFonts.SelectedIndex == -1 || !(fontSize.Value > 0))
            {
                if (File.Exists(asset_path))
                {
                    MessageBox.Show("Couldn't import asset, an image with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                MessageBox.Show("Please fill out all required inputs.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                //Convert font to SPRITEFONT
                ProcessStartInfo imageConverter = new ProcessStartInfo();
                imageConverter.WorkingDirectory = "DATA/FONTS";
                imageConverter.FileName = "DATA/FONTS/MakeSpriteFont.exe";
                imageConverter.Arguments = "\"" + installedFonts.SelectedItem.ToString() + "\" \"" + installedFonts.SelectedItem.ToString().Replace(' ', '_').ToUpper() + ".SPRITEFONT\" /FontSize:" + fontSize.Value + " /DebugOutputSpriteSheet:\"" + installedFonts.SelectedItem.ToString().Replace(' ', '_').ToUpper() + ".BMP\"";
                imageConverter.UseShellExecute = false;
                imageConverter.RedirectStandardOutput = true;
                Process converterProcess = Process.Start(imageConverter);
                StreamReader reader = converterProcess.StandardOutput;
                converterProcess.WaitForExit();

                //Capture output incase we errored
                string output = reader.ReadToEnd();

                if (!File.Exists(asset_path))
                {
                    //Import failed, show reason if requested
                    DialogResult showErrorInfo = MessageBox.Show("Font import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                    if (showErrorInfo == DialogResult.Yes)
                    {
                        MessageBox.Show(output, "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else
                {
                    //Create JSON data
                    JToken asset_json = JToken.Parse("{\"asset_name\": \"" + installedFonts.SelectedItem.ToString() + "\", \"asset_type\": \"Fonts\"}");
                    File.WriteAllText(asset_path.Substring(0, asset_path.Length - 10) + "JSON", asset_json.ToString(Formatting.Indented));

                    //Import success
                    MessageBox.Show("Font successfully imported.", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }

                this.Close();
            }
        }
    }
}
