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
    public partial class Asset_Manager_Preselect : Form
    {
        public Asset_Manager_Preselect()
        {
            InitializeComponent();
        }
        
        /* Open Map Manager */
        private void openMapManager_Click(object sender, EventArgs e)
        {
            openManager(AssetCompType.MAP);
        }

        /* Open Vehicle Manager */
        private void openVehicleManager_Click(object sender, EventArgs e)
        {
            openManager(AssetCompType.VEHICLE);
        }

        /* Open Character Manager */
        private void openCharacterManager_Click(object sender, EventArgs e)
        {
            openManager(AssetCompType.CHARACTER);
        }

        /* Open A Manager */
        private void openManager(AssetCompType type)
        {
            Asset_Comp_Manager manager = new Asset_Comp_Manager(type);
            manager.Show();
        }
    }
}
