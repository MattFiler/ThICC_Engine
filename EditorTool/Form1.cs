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
using System.Diagnostics;

namespace EditorTool
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void browseToModel_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "OBJ Model (OBJ)|*.OBJ";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                modelPath.Text = filePicker.FileName;
            }
        }

        private void addTexture_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "Model Texture (JPG/PNG)|*.JPG;*.PNG";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                bool canCopyWithoutConflict = true;
                foreach (string texture in textureList.Items)
                {
                    if (texture == filePicker.FileName || Path.GetFileName(texture) == Path.GetFileName(filePicker.FileName))
                    {
                        canCopyWithoutConflict = false;
                        break;
                    }
                }
                if (canCopyWithoutConflict)
                {
                    textureList.Items.Add(filePicker.FileName);
                }
                else
                {
                    MessageBox.Show("This texture's filename conflicts with another.", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void removeTexture_Click(object sender, EventArgs e)
        {
            if (textureList.SelectedIndex == -1)
            {
                MessageBox.Show("Please select a texture from the list.", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                textureList.Items.Remove(textureList.Items[textureList.SelectedIndex]);
            }
        }

        private void importModel_Click(object sender, EventArgs e)
        {
            if (File.Exists("Models/" + Path.GetFileName(modelPath.Text)))
            {
                MessageBox.Show("Couldn't import model, a model with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                string pathWithoutExtension = modelPath.Text.Substring(0, modelPath.Text.Length - Path.GetExtension(modelPath.Text).Length);

                File.Copy(pathWithoutExtension + ".obj", "Models/" + Path.GetFileName(modelPath.Text));
                if (File.Exists(pathWithoutExtension + ".mtl"))
                {
                    File.Copy(pathWithoutExtension + ".mtl", "Models/" + Path.GetFileName(pathWithoutExtension + ".mtl"));
                }
                foreach (string texture in textureList.Items)
                {
                    if (!File.Exists("Models/" + Path.GetFileName(texture)))
                    {
                        File.Copy(texture, "Models/" + Path.GetFileName(texture));
                    }
                }

                ProcessStartInfo meshConverter = new ProcessStartInfo();
                meshConverter.WorkingDirectory = "Models";
                meshConverter.FileName = "meshconvert.exe";
                meshConverter.Arguments = Path.GetFileName(modelPath.Text) + " -sdkmesh -nodds -y";
                meshConverter.CreateNoWindow = true;
                Process converterProcess = Process.Start(meshConverter);
                converterProcess.WaitForExit();

                modelPath.Text = "";
                textureList.Items.Clear();

                MessageBox.Show("Model Import Complete", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
