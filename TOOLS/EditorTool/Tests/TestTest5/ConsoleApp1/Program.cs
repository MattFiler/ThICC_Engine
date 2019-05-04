using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            string test = "HELLO WORLD";
            using (BinaryWriter writer = new BinaryWriter(File.Open("bytes.bin", FileMode.Create)))
            {
                writer.Write(test.Length);
                foreach (char character in test)
                {
                    writer.Write(character);
                }
            }
        }
    }
}
