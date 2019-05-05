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

        /* Open Cup Manager */
        private void cupManager_Click(object sender, EventArgs e)
        {
            openManager(AssetCompType.CUP);
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

        /* Open Global Sound Manager */
        private void openSoundManager_Click(object sender, EventArgs e)
        {
            Create_Global_Sounds globalSoundEditor = new Create_Global_Sounds();
            globalSoundEditor.Show();
            Close();
        }

        /* Open Common Model Manager */
        private void commonModelManager_Click(object sender, EventArgs e)
        {
            Create_InGame_Models_Common commonModelEditor = new Create_InGame_Models_Common();
            commonModelEditor.Show();
            Close();
        }

        /* Open Gamepad Icon Manager */
        private void gamepadIconManager_Click(object sender, EventArgs e)
        {
            Create_Gamepad_Icons gamepadIconEditor = new Create_Gamepad_Icons();
            gamepadIconEditor.Show();
            Close();
        }

        /* Open A Manager */
        private void openManager(AssetCompType type)
        {
            Asset_Comp_Manager manager = new Asset_Comp_Manager(type);
            manager.Show();
            this.Close();
        }
    }
}
