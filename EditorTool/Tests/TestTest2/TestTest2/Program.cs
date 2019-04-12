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

            /* READ OUT */
            /*
            string[] numbers_file = File.ReadAllLines("numbers.bin");
            List<float> all_verts = new List<float>();
            foreach (string line in numbers_file)
            {
                string[] split_by_comma = line.Split(',');
                for (int i = 0; i < numbers_per_line; i++)
                {
                    all_verts.Add(Convert.ToSingle(split_by_comma[i]));
                }
            }
            using (BinaryWriter writer = new BinaryWriter(File.Open("bytes.bin", FileMode.Create)))
            {
                writer.Write(1);
                writer.Write(2);
                writer.Write(3);
                foreach (float vert in all_verts)
                {
                    writer.Write(vert);
                }
            }
            */

            Console.WriteLine(sizeof(double));

            /* READ BACK IN */
            double[] csv_line = new double[numbers_per_line];
            List<string> csv_out_line = new List<string>();
            using (BinaryReader reader = new BinaryReader(File.Open("MKS_TEST.COLLMAP", FileMode.Open)))
            {
                int len = reader.ReadInt32();

                Console.WriteLine(len);

                List<int> num = new List<int>();
                for (int i = len; i > 0; i--)
                {
                    int number = reader.ReadInt32();
                    Console.WriteLine(number);
                    number /= sizeof(double);
                    for (int x = 0; x < len - i; x++)
                    {
                        number += num.ElementAt(x);
                    }
                    num.Add(number);
                }


                int index = 0;
                int reader_index = 0;
                try
                {
                    for (int i = 0; i < 999999999; i++)
                    {
                        if (index == numbers_per_line)
                        {
                            string this_vec_line = "";
                            for (int x = 0; x < numbers_per_line; x++)
                            {
                                this_vec_line += csv_line[x].ToString() + ",";
                            }
                            this_vec_line = this_vec_line.Substring(0, this_vec_line.Length - 1);
                            csv_out_line.Add(this_vec_line);
                            index = 0;
                        }
                        csv_line[index] = reader.ReadDouble();
                        index++;
                        
                        if (i == num.ElementAt(reader_index))
                        {
                            csv_out_line.Add("---");
                            reader_index++;
                        }
                    }
                }
                catch
                {
                    File.WriteAllLines("numbers_out.bin", csv_out_line);
                }
            }
            Console.ReadLine();
        }
    }
}
