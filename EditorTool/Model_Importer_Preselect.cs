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
    public partial class Model_Importer_Preselect : Form
    {
        public ModelType selected_model_type { get; set; }

        public Model_Importer_Preselect()
        {
            InitializeComponent();
        }

        /* Select what type of model we should import */
        private void isMap_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            selected_model_type = ModelType.MAP;
        }
        private void isItem_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            selected_model_type = ModelType.ITEM;
        }
        private void isPlayer_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            selected_model_type = ModelType.PLAYER;
        }
        private void isProp_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            selected_model_type = ModelType.PROP;
        }
    }
}
