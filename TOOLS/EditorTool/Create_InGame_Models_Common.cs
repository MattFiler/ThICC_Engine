using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
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
    public partial class Create_InGame_Models_Common : Form
    {
        UsefulFunctions function_libary = new UsefulFunctions();
        JToken controller_config;
        public Create_InGame_Models_Common()
        {
            InitializeComponent();

            //Original config
            controller_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/COMMON_MODELS_CONFIG.JSON"));
            courseReferee.Text = controller_config["referee"].Value<string>();
            glider.Text = controller_config["glider"].Value<string>();
            itemBox.Text = controller_config["itembox"].Value<string>();
            debugMarker.Text = controller_config["debug_marker"].Value<string>();
        }

        /* Select assets */
        private void loadReferee_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(courseReferee, AssetType.MODEL);
        }
        private void loadGlider_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(glider, AssetType.MODEL);
        }
        private void selectItemBox_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(itemBox, AssetType.MODEL);
        }
        private void loadDebugMarker_Click(object sender, EventArgs e)
        {
            function_libary.assetSelectHandler(debugMarker, AssetType.MODEL);
        }

        /* Save */
        private void saveModels_Click(object sender, EventArgs e)
        {
            //All inputs required
            if (courseReferee.Text == "" || glider.Text == "" || itemBox.Text == "")
            {
                MessageBox.Show("Please complete all fields before trying to save.", "Can't save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update tags
            function_libary.executeUseageQueueForTag(function_libary.getUseageTagFor(AssetCompType.GLOBAL_MODELS));

            //Update config
            controller_config["referee"] = courseReferee.Text;
            controller_config["glider"] = glider.Text;
            controller_config["itembox"] = itemBox.Text;
            controller_config["debug_marker"] = debugMarker.Text;

            //Save
            File.WriteAllText("DATA/CONFIGS/COMMON_MODELS_CONFIG.JSON", controller_config.ToString(Formatting.Indented));
            MessageBox.Show("Configuration saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Close();
        }


        
        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }

    }
}
