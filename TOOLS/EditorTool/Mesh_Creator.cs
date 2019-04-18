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
    public partial class Mesh_Creator: Form
    {
        public Mesh_Creator()
        {
            InitializeComponent();
        }

        private void Mesh_Creator_Load(object sender, EventArgs e)
        {
            /* 
             * Mesh explanation...
             * First line is "C" or "T", with "C" being a coloured mesh, "T" being a textured mesh
             * If textured, second line is the name of the image to use as the texture.
             * Next line is the count of verts
             * All following line are vert definitions:
             *  - for coloured meshes, the verts are X, Y, Z (or maybe X, Z, Y) followed by R, G, B, A
             *  - for textured meshes, the verts are X, Y, Z (or maybe X, Z, Y) followed by X, Y texture coords
             * Then, you have the count of faces - these are triangulated
             * The face definitions are similar to OBJ just without the extra normal data index, etc.
             * */
        }
    }
}
