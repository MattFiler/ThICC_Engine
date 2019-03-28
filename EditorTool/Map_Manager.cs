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
    public partial class Map_Manager : Form
    {
        public Map_Manager()
        {
            InitializeComponent();
        }

        /* Open map creator window */
        private void newMap_Click(object sender, EventArgs e)
        {
            Create_Map createMap = new Create_Map();
            createMap.Show();
        }
    }
}
