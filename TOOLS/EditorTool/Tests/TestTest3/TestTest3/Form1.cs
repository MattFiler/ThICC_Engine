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

namespace TestTest3
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

            using (var image = new Bitmap(filePicker.FileName))
            {
                Bitmap channel_R = new Bitmap(image.Width, image.Height);
                Bitmap channel_G = new Bitmap(image.Width, image.Height);
                Bitmap channel_B = new Bitmap(image.Width, image.Height);

                for (int x = 0; x < image.Width; x++)
                {
                    for (int y = 0; y < image.Height; y++)
                    {
                        var this_pixel = image.GetPixel(x, y);
                        channel_R.SetPixel(x, y, Color.FromArgb(this_pixel.R, 255, 255, 255));
                        channel_G.SetPixel(x, y, Color.FromArgb(this_pixel.G, 255, 255, 255));
                        channel_B.SetPixel(x, y, Color.FromArgb(this_pixel.B, 255, 255, 255));

                        if (x == 0 && y == 0)
                        {
                            MessageBox.Show("First pixel! R: " + this_pixel.R + ", G: " + this_pixel.G + ", B: " + this_pixel.B + ".");
                        }
                    }
                }

                string output_template = filePicker.FileName.Substring(0, filePicker.FileName.Length - Path.GetExtension(filePicker.FileName).Length);
                channel_R.Save(output_template + "_R" + Path.GetExtension(filePicker.FileName));
                channel_G.Save(output_template + "_G" + Path.GetExtension(filePicker.FileName));
                channel_B.Save(output_template + "_B" + Path.GetExtension(filePicker.FileName));

                MessageBox.Show("Completed!");
            }
        }
    }
}
