using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Landing : Form
    {
        public Landing()
        {
            InitializeComponent();
        }

        private void importModel_Click(object sender, EventArgs e)
        {
            Model_Importer modelimporter = new Model_Importer();
            modelimporter.Show();
        }

        private void importSound_Click(object sender, EventArgs e)
        {
            Sound_Importer soundimporter = new Sound_Importer();
            soundimporter.Show();
        }

        private void importImage_Click(object sender, EventArgs e)
        {
            Image_Importer imageimporter = new Image_Importer();
            imageimporter.Show();
        }
    }
}
