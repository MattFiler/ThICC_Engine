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
            filePicker.Filter = "Image (PNG)|*.PNG";
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
                File.Copy(imagePath.Text, "DDS/" + Path.GetFileName(imagePath.Text));

                ProcessStartInfo imageConverter = new ProcessStartInfo();
                imageConverter.WorkingDirectory = "DDS";
                imageConverter.FileName = "texconv.exe";
                imageConverter.Arguments = "\""+Path.GetFileName(imagePath.Text)+ "\"";
                imageConverter.CreateNoWindow = true;
                Process converterProcess = Process.Start(imageConverter);
                converterProcess.WaitForExit();

                File.Delete("DDS/" + Path.GetFileName(imagePath.Text));
                if (File.Exists("DDS/" + Path.GetFileNameWithoutExtension(imagePath.Text) + ".DDS"))
                {
                    File.Move("DDS/" + Path.GetFileNameWithoutExtension(imagePath.Text) + ".DDS", "DDS/" + Path.GetFileNameWithoutExtension(imagePath.Text) + ".dds");
                }

                imagePath.Text = "";

                MessageBox.Show("Image Import Complete", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
