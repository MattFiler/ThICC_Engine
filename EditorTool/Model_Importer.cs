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
    public partial class Model_Importer : Form
    {
        public Model_Importer()
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
            filePicker.Multiselect = true;
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                foreach (string file in filePicker.FileNames)
                {
                    bool canCopyWithoutConflict = true;
                    foreach (string texture in textureList.Items)
                    {
                        if (texture == file || Path.GetFileName(texture) == Path.GetFileName(file))
                        {
                            canCopyWithoutConflict = false;
                            break;
                        }
                    }
                    if (canCopyWithoutConflict)
                    {
                        textureList.Items.Add(file);
                    }
                    else
                    {
                        MessageBox.Show("Texture name conflict:\n" + file, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
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
            if (File.Exists("Models/" + Path.GetFileName(modelPath.Text)) || modelPath.Text == "")
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
                    int mtl_index = 0;
                    string[] mtl_file = File.ReadAllLines("Models/" + Path.GetFileName(pathWithoutExtension + ".mtl"));
                    foreach (string line in mtl_file)
                    {
                        if (line.Contains("map"))
                        {
                            string[] map_split = new string[2];
                            map_split = line.Split(' ');
                            map_split[1] = Path.GetFileName(map_split[1]);
                            mtl_file[mtl_index] = map_split[0] + " " + map_split[1];
                        }
                        mtl_index++;
                    }
                    File.WriteAllLines("Models/" + Path.GetFileName(pathWithoutExtension + ".mtl"), mtl_file);
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

                File.Delete("Models/" + Path.GetFileName(modelPath.Text));
                if (File.Exists("Models/" + Path.GetFileName(pathWithoutExtension + ".mtl")))
                {
                    File.Delete("Models/" + Path.GetFileName(pathWithoutExtension + ".mtl"));
                }

                modelPath.Text = "";
                textureList.Items.Clear();

                MessageBox.Show("Model Import Complete", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
