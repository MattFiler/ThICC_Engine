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
    public partial class Sound_Importer : Form
    {
        public Sound_Importer()
        {
            InitializeComponent();
        }

        private void browseToSound_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "Sound (WAV)|*.WAV";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                soundPath.Text = filePicker.FileName;
            }
        }

        private void importSound_Click(object sender, EventArgs e)
        {
            if (File.Exists("Sounds/" + Path.GetFileName(soundPath.Text)))
            {
                MessageBox.Show("Couldn't import sound, a sound with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                File.Copy(soundPath.Text, "Sounds/" + Path.GetFileName(soundPath.Text));
                soundPath.Text = "";
                MessageBox.Show("Sound Import Complete", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
