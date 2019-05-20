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
    public partial class Cubemap_Creator : Form
    {
        UsefulFunctions function_library = new UsefulFunctions();

        public Cubemap_Creator()
        {
            InitializeComponent();
        }

        private void Cubemap_Creator_Load(object sender, EventArgs e)
        {
            /*
             * 
             * The default setup is DIFFUSE = RADIANCE, SPECULAR = IRRADIANCE - however I think it looks better the other way around.
             * 
             */
        }

        /* Select and preview faces */
        private void loadPX_Click(object sender, EventArgs e)
        {
            pathPX.Text = function_library.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
            function_library.loadGenericImagePreview(pathPX.Text, previewPX);
        }
        private void loadNX_Click(object sender, EventArgs e)
        {
            pathNX.Text = function_library.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
            function_library.loadGenericImagePreview(pathNX.Text, previewNX);
        }
        private void loadPY_Click(object sender, EventArgs e)
        {
            pathPY.Text = function_library.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
            function_library.loadGenericImagePreview(pathPY.Text, previewPY);
        }
        private void loadNY_Click(object sender, EventArgs e)
        {
            pathNY.Text = function_library.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
            function_library.loadGenericImagePreview(pathNY.Text, previewNY);
        }
        private void loadPZ_Click(object sender, EventArgs e)
        {
            pathPZ.Text = function_library.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
            function_library.loadGenericImagePreview(pathPZ.Text, previewPZ);
        }
        private void loadNZ_Click(object sender, EventArgs e)
        {
            pathNZ.Text = function_library.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
            function_library.loadGenericImagePreview(pathNZ.Text, previewNZ);
        }

        /* Create the cubemap */
        private void createCubemap_Click(object sender, EventArgs e)
        {
            if (pathPX.Text == "" || pathNX.Text == "" || pathPY.Text == "" || pathNY.Text == "" || pathPZ.Text == "" || pathNZ.Text == "")
            {
                MessageBox.Show("Please complete all fields.", "Can't continue.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //All faces
            List<string> faces = new List<string>();
            faces.Add(pathPX.Text);
            faces.Add(pathNX.Text);
            faces.Add(pathPY.Text);
            faces.Add(pathNY.Text);
            faces.Add(pathPZ.Text);
            faces.Add(pathNZ.Text);

            //Create
            if (function_library.createCubemap(asset_name.Text, faces))
            {
                this.Close();
            }
        }
    }
}
