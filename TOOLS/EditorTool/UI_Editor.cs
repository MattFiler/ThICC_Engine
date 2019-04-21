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
    public partial class UI_Editor : Form
    {
        UsefulFunctions common_functions = new UsefulFunctions();

        List<PictureBox> all_images = new List<PictureBox>();
        List<Label> all_texts = new List<Label>();
        PictureBox background;

        public UI_Editor()
        {
            InitializeComponent();
        }

        private void UI_Editor_Load(object sender, EventArgs e)
        {
            background = new PictureBox();
            background.Image = loadBMP(common_functions.getFolder(AssetType.IMAGE) + "BLANK_UI_CANVAS.JPG");
            background.Location = new Point(0, 0);
            background.Size = new Size(1280, 720);
            viewportGroup.Controls.Add(background);
        }

        /* Browse to new image */
        private void imageBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = "Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG";
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                imageSrc.Text = filePicker.FileName;

                PictureBox selected_image = all_images.ElementAt(getSelectedItemIndex());
                selected_image.Image = loadBMP(filePicker.FileName);
                selected_image.Location = new Point(0,0);
                selected_image.Size = new Size(selected_image.Image.Size.Width, selected_image.Image.Size.Height);
                selected_image.BringToFront();
            }
        }

        /* Update element text */
        private void elementText_TextChanged(object sender, EventArgs e)
        {
            Label selected_text = all_texts.ElementAt(getSelectedItemIndex());
            selected_text.Text = elementText.Text;
            selected_text.BackColor = Color.Transparent;
            selected_text.BringToFront();
        }

        /* Parse selected list item */
        int getSelectedItemIndex()
        {
            string[] selected_item = elementList.SelectedItem.ToString().Split('-');
            return Convert.ToInt32(selected_item[1]);
        }
        string getSelectedItem()
        {
            string[] selected_item = elementList.SelectedItem.ToString().Split('-');
            return selected_item[0];
        }

        /* Add new element to canvas */
        private void addElement_Click(object sender, EventArgs e)
        {
            switch (elementType.SelectedItem)
            {
                case "Image":
                    PictureBox new_image = new PictureBox();
                    all_images.Add(new_image);
                    viewportGroup.Controls.Add(new_image);
                    elementList.Items.Add("IMAGE-" + Convert.ToString(all_images.Count - 1));
                    break;
                case "Text":
                    Label new_text = new Label();
                    new_text.Font = new Font("Arial", 10);
                    new_text.Parent = background;
                    all_texts.Add(new_text);
                    viewportGroup.Controls.Add(new_text);
                    elementList.Items.Add("TEXT-" + Convert.ToString(all_texts.Count - 1));
                    break;
            }
        }

        /* Load a bitmap safely */
        Bitmap loadBMP(string filepath, int x = 0, int y = 0)
        {
            if (x == 0 && y == 0)
            {
                using (var tempPreviewImg = new Bitmap(filepath))
                {
                    return new Bitmap(tempPreviewImg);
                }
            }
            else
            {
                using (var tempPreviewImg = new Bitmap(filepath))
                {
                    return new Bitmap(tempPreviewImg, x, y);
                }
            }
        }

        /* Show editor controls for selected item */
        private void elementList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (elementList.SelectedIndex != -1)
            {
                switch (getSelectedItem())
                {
                    case "IMAGE":
                        imageElementControl.Visible = true;
                        textElementControl.Visible = false;
                        break;
                    case "TEXT":
                        imageElementControl.Visible = false;
                        textElementControl.Visible = true;
                        break;
                }
                return;
            }
            imageElementControl.Visible = false;
            textElementControl.Visible = false;
        }

        private void fontSize_ValueChanged(object sender, EventArgs e)
        {
            Label selected_text = all_texts.ElementAt(getSelectedItemIndex());
            selected_text.Font = new Font("Arial", Convert.ToInt32(fontSize.Value));
        }
    }
}
