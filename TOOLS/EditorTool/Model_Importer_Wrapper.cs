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
    public partial class Model_Importer_Wrapper : Form
    {
        Model_Importer_Common importer_common;

        public Model_Importer_Wrapper()
        {
            InitializeComponent();

            //Model type preselect
            using (var type_preselect = new Model_Importer_Preselect())
            {
                var type_preselect_result = type_preselect.ShowDialog();
                if (type_preselect_result == DialogResult.OK)
                {
                    //Asset selector
                    using (var asset_selector = new Model_Importer_AssetSelector(type_preselect.selected_model_type))
                    {
                        var asset_selector_result = asset_selector.ShowDialog();
                        if (asset_selector_result == DialogResult.OK)
                        {
                            //Material list
                            using (var material_list = new Model_Importer_MaterialList(asset_selector.importer_common))
                            {
                                var material_list_result = material_list.ShowDialog();
                                if (material_list_result == DialogResult.OK)
                                {
                                    this.Close();
                                }
                            }
                        }
                    }
                }
            }
            this.Close();
        }
    }
}
