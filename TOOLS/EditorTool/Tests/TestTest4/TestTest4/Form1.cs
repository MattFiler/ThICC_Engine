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

namespace TestTest4
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
            filePicker.Filter = "Model |*.OBJ";
            if (filePicker.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            string[] model_obj = File.ReadAllLines(filePicker.FileName);
            List<string> mat_names = new List<string>();
            int index = 0;
            foreach (string line in model_obj)
            {
                if (line.Contains("usemtl "))
                {
                    string line_i = line.Replace("\t", string.Empty);
                    line_i = line_i.Replace("    ", string.Empty);
                    mat_names.Add(line_i.Substring(7));
                }
                index++;
            }
            File.WriteAllLines("parsed.txt", mat_names);
        }
    }
}
