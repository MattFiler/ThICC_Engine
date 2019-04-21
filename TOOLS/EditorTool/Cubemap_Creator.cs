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
    }
}
