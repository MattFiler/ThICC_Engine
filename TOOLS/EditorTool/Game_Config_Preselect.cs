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
    public partial class Game_Config_Preselect : Form
    {
        public Game_Config_Preselect()
        {
            InitializeComponent();
        }

        /* Open character config */
        private void openCharacterConfig_Click(object sender, EventArgs e)
        {
            Game_Config_Character characterConfig = new Game_Config_Character();
            characterConfig.Show();
            Close();
        }

        /* Open camera config */
        private void openCameraConfig_Click(object sender, EventArgs e)
        {
            Game_Config_Camera cameraConfig = new Game_Config_Camera();
            cameraConfig.Show();
            Close();
        }

        /* Open item config */
        private void itemConfigEditor_Click(object sender, EventArgs e)
        {
            ItemConfigEditor itemConfigs = new ItemConfigEditor();
            itemConfigs.Show();
            Close();
        }

        /* Open misc config */
        private void miscConfigs_Click(object sender, EventArgs e)
        {
            Game_Config_Misc miscConfig = new Game_Config_Misc();
            miscConfig.Show();
            Close();
        }



        private void openItemConfig_Click(object sender, EventArgs e)
        {
        }

    }
}
