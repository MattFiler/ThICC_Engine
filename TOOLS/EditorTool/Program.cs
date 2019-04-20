using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            if (Environment.GetCommandLineArgs().Count() >= 2 && Environment.GetCommandLineArgs().ElementAt(1).ToString().Substring(1) == "auto_compile")
            {
                //A launch to soley build assets
                Splash splash_content = new Splash(false);
                if (splash_content.autoCompileAssets(false, "../")) //Working directory fix
                {
                    Console.WriteLine("Asset compile finished successfully.");
                }
                else
                {
                    Console.WriteLine("Asset compile FAILED. Files may be open in another process.");
                }
                Application.Exit();
            }
            else
            {
                //A standard application launch
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new Splash());
            }
        }
    }
}
