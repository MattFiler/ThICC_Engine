using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestTest1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "Image |*.JPG;*.PNG;*.JPEG";
            if (filePicker.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            bool has_transparency = false;
            using (var tempPreviewImg = new Bitmap(filePicker.FileName))
            {
                pictureBox1.Image = new Bitmap(tempPreviewImg);
                
                for (int x=0; x < tempPreviewImg.Width; x++)
                {
                    for (int y=0; y < tempPreviewImg.Height; y++)
                    {
                        var this_pixel = tempPreviewImg.GetPixel(x, y);
                        if (this_pixel.A != 255)
                        {
                            has_transparency = true;
                            break;
                        }
                    }
                    if (has_transparency) { break; }
                }

                if (has_transparency)
                {
                    MessageBox.Show("Has transparency.");
                }
                else
                {
                    MessageBox.Show("Doesn't have transparency.");
                }
            }
        }
    }
}
