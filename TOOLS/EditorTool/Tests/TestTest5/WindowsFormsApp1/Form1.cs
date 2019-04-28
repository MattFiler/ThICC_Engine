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

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string test = "HELLO WORLD";
            using (BinaryWriter writer = new BinaryWriter(File.Open("bytes.bin", FileMode.Create)))
            {
                writer.Write(test.Length);
                writer.Write(test);
            }
        }
    }
}
