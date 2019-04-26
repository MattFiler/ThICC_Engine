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
    public partial class Game_Config_Misc : Form
    {
        UsefulFunctions useful_functions = new UsefulFunctions();
        string original_font = "";
        public Game_Config_Misc()
        {
            InitializeComponent();
            
            //Populate imported fonts
            string[] files = Directory.GetFiles(useful_functions.getFolder(AssetType.FONT), "*.SPRITEFONT", SearchOption.AllDirectories);
            foreach (string file in files)
            {
                engineFont.Items.Add(Path.GetFileNameWithoutExtension(file));
            }

            //Original config
            JToken game_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/GAME_CORE.JSON"));
            playerCount.Value = game_config["player_count"].Value<decimal>();
            engineFont.SelectedItem = game_config["font"].Value<string>().ToUpper();
            original_font = engineFont.SelectedItem.ToString();
        }

        /* Save config */
        private void saveConfig_Click(object sender, EventArgs e)
        {
            if (engineFont.SelectedIndex == -1)
            {
                MessageBox.Show("Please select a font!", "No font selected!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (!(playerCount.Value >= 1 && playerCount.Value <= 4))
            {
                MessageBox.Show("Player count must be between 1-4!", "Invalid player count!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Update font useages
            useful_functions.removeUseageTag(AssetType.FONT, original_font, useful_functions.getUseageTagFor(AssetCompType.ENGINE_FONT));
            useful_functions.addUseageTag(AssetType.FONT, engineFont.SelectedItem.ToString(), useful_functions.getUseageTagFor(AssetCompType.ENGINE_FONT));

            //Update config
            JToken game_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/GAME_CORE.JSON"));
            game_config["player_count"] = playerCount.Value;
            game_config["font"] = engineFont.SelectedItem.ToString();
            File.WriteAllText("DATA/CONFIGS/GAME_CORE.JSON", game_config.ToString(Formatting.Indented));
            MessageBox.Show("Configuration saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Close();
        }
    }
}
