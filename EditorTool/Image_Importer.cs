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
    public partial class Image_Importer : Form
    {
        public Image_Importer()
        {
            InitializeComponent();
        }

        private void browseToImage_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                imagePath.Text = filePicker.FileName;
            }
        }

        private void importModel_Click(object sender, EventArgs e)
        {
            if (File.Exists("DDS/" + Path.GetFileNameWithoutExtension(imagePath.Text) + ".dds") || imagePath.Text == "")
            {
                MessageBox.Show("Couldn't import image, an image with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                //Copy file to "working directory"
                File.Copy(imagePath.Text, "DDS/" + Path.GetFileName(imagePath.Text));

                //Convert copied image to DDS
                ProcessStartInfo imageConverter = new ProcessStartInfo();
                imageConverter.WorkingDirectory = "DDS";
                imageConverter.FileName = "DDS/texconv.exe";
                imageConverter.Arguments = "\""+Path.GetFileName(imagePath.Text)+ "\"";
                imageConverter.UseShellExecute = false;
                imageConverter.RedirectStandardOutput = true;
                Process converterProcess = Process.Start(imageConverter);
                StreamReader reader = converterProcess.StandardOutput;
                converterProcess.WaitForExit();

                //Capture DDS convert output incase we errored
                string output = reader.ReadToEnd();

                //Lowercase extension pls
                File.Delete("DDS/" + Path.GetFileName(imagePath.Text));
                if (File.Exists("DDS/" + Path.GetFileNameWithoutExtension(imagePath.Text) + ".DDS"))
                {
                    File.Move("DDS/" + Path.GetFileNameWithoutExtension(imagePath.Text) + ".DDS", "DDS/" + Path.GetFileNameWithoutExtension(imagePath.Text) + ".dds");
                }

                if (!File.Exists("DDS/" + Path.GetFileNameWithoutExtension(imagePath.Text) + ".dds"))
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
                    //Import success
                    MessageBox.Show("Image successfully imported.", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }

                imagePath.Text = "";
            }
        }
    }
}
