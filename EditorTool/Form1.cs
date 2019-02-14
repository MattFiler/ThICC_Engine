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

        private void importModel_Click(object sender, EventArgs e)
        {
            if (File.Exists("Models/" + Path.GetFileName(modelPath.Text)))
            {
                MessageBox.Show("Import Failed!", "Couldn't import model, a model with the same name already exists.", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                string pathWithoutExtension = modelPath.Text.Substring(0, modelPath.Text.Length - Path.GetExtension(modelPath.Text).Length);

                File.Copy(pathWithoutExtension + ".obj", "Models/" + Path.GetFileName(modelPath.Text));
                if (File.Exists(pathWithoutExtension + ".mtl"))
                {
                    File.Copy(pathWithoutExtension + ".mtl", "Models/" + Path.GetFileNameWithoutExtension(modelPath.Text) + ".mtl");
                }

                ProcessStartInfo meshConverter = new ProcessStartInfo();
                meshConverter.WorkingDirectory = "Models";
                meshConverter.FileName = "meshconvert.exe";
                meshConverter.Arguments = Path.GetFileName(modelPath.Text) + " -sdkmesh -nodds -y";
                meshConverter.CreateNoWindow = true;
                Process converterProcess = Process.Start(meshConverter);
                converterProcess.WaitForExit();
                modelPath.Text = "";

                MessageBox.Show("Imported!", "Model Import Complete", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
