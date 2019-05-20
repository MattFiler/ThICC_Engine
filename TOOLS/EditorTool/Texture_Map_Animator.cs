using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Texture_Map_Animator : Form
    {
        UsefulFunctions function_library = new UsefulFunctions();
        public Texture_Map_Animator()
        {
            InitializeComponent();
        }

        /* Select texture and output folder */
        private void selectTexture_Click(object sender, EventArgs e)
        {
            texture.Text = function_library.userLocatedFile("Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG");
        }
        private void selectOutputFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folder_browser = new FolderBrowserDialog();
            if (folder_browser.ShowDialog() == DialogResult.OK)
            {
                outputFolder.Text = folder_browser.SelectedPath;
            }
        }

        /* Make animation */
        private void makeAnim_Click(object sender, EventArgs e)
        {
            if (texture.Text == "" || outputFolder.Text == "" || !Directory.Exists(outputFolder.Text))
            {
                MessageBox.Show("Please complete all fields.", "Can't continue.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Create a basic animation of pixel-based movement over the Y of the image
            using (var texture_to_animate = new Bitmap(texture.Text))
            {
                for (int i = 0; i < texture_to_animate.Height; i += stepModifier.Value)
                {
                    Bitmap this_map = new Bitmap(texture_to_animate.Width, texture_to_animate.Height);
                    Graphics texture_map = Graphics.FromImage(this_map);
                    texture_map.DrawImage(texture_to_animate, 0, i - texture_to_animate.Height, texture_to_animate.Width, texture_to_animate.Height);
                    texture_map.DrawImage(texture_to_animate, 0, i, texture_to_animate.Width, texture_to_animate.Height);
                    this_map.Save(outputFolder.Text + "/" + Path.GetFileNameWithoutExtension(texture.Text) + "_" + i + Path.GetExtension(texture.Text));
                    texture_map.Dispose();
                }
            }

            //Done
            MessageBox.Show("Saved all animation frames!", "Completed.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Process.Start(outputFolder.Text);
        }
    }
}
