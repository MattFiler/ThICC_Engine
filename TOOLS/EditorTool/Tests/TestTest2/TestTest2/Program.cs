using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestTest2
{
    class Program
    {
        static void Main(string[] args)
        {
            int numbers_per_line = 9;
            
            /* READ BACK IN */
            double[] csv_line = new double[numbers_per_line];
            List<string> csv_out_line = new List<string>();
            //using (BinaryReader reader = new BinaryReader(File.Open(args[0].ToString(), FileMode.Open)))
            using (BinaryReader reader = new BinaryReader(File.Open("MARIOKARTSTADIUM.COLLMAP", FileMode.Open)))
            {
                int num_of_groups = reader.ReadInt32();
                List<int> group_size = new List<int>();
                for (int i = 0; i < num_of_groups; i++)
                {
                    group_size.Add(reader.ReadInt32());
                }
                
                List<string> vert_csv = new List<string>();
                for (int i = 0; i < num_of_groups; i++)
                {
                    vert_csv.Add("GROUP " + i.ToString() + " - " + group_size.ElementAt(i).ToString() + " ENTRIES");
                    for (int x = 0; x < group_size.ElementAt(i); x++)
                    {
                        vert_csv.Add(reader.ReadSingle().ToString());
                    }
                    vert_csv.Add("---");
                }

                File.WriteAllLines("output.txt", vert_csv);
            }
            Console.ReadLine();
        }
    }
}
