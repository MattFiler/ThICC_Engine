using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
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

namespace EditorTool
{
    public partial class Splash : Form
    {
        public Splash()
        {
            InitializeComponent();
        }

        /* Open Asset Manager */
        private void openAssetManager_Click(object sender, EventArgs e)
        {
            Landing assetManager = new Landing();
            assetManager.Show();
        }

        /* Compile Assets */
        private void compileAssets_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                //Fix VS debugging directory config
                File.WriteAllText("Scarle2019/Scarle2019.vcxproj.user", "<?xml version=\"1.0\" encoding=\"utf-8\"?><Project ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\"><LocalDebuggerWorkingDirectory>$(SolutionDir)$(Configuration)\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments></PropertyGroup><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\"><LocalDebuggerWorkingDirectory>$(SolutionDir)$(Configuration)\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments></PropertyGroup></Project>");

                //Copy to debug folder
                if (Directory.Exists("Debug"))
                {
                    copyAssets("Debug/DATA/");
                    if (File.Exists("Debug/Launcher.exe"))
                    {
                        File.Delete("Debug/Launcher.exe");
                    }
                    File.Copy("DATA/MarioKartLauncher.exe", "Debug/Launcher.exe");
                }

                //Copy to release folder
                if (Directory.Exists("Release"))
                {
                    copyAssets("Release/DATA/");
                    if (File.Exists("Release/Launcher.exe"))
                    {
                        File.Delete("Release/Launcher.exe");
                    }
                    File.Copy("DATA/MarioKartLauncher.exe", "Release/Launcher.exe");
                }

                Cursor.Current = Cursors.Default;
                MessageBox.Show("Assets successfully compiled.", "Compiled assets.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch
            {
                Cursor.Current = Cursors.Default;
                MessageBox.Show("An error occured while compiling assets.\nMake sure that the game is closed and no files are open.", "Asset compile failed.", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /* Copy All Asssets To Folder */
        void copyAssets(string output_directory)
        {
            if (Directory.Exists(output_directory))
            {
                Directory.Delete(output_directory, true);
            }
            List<string> ignored_extensions = new List<string>();
            ignored_extensions.Add(".vcxproj");
            ignored_extensions.Add(".filters");
            ignored_extensions.Add(".user");
            ignored_extensions.Add(".exe");
            ignored_extensions.Add(".obj");
            ignored_extensions.Add(".mtl");
            ignored_extensions.Add(".bmp");
            ignored_extensions.Add(".config");
            ignored_extensions.Add(".pdb");
            ignored_extensions.Add(".xml");
            ignored_extensions.Add(".dll");
            DirectoryCopy("DATA/", output_directory, true, ignored_extensions);
        }

        /* Copy a Directory */
        //Modified from: https://docs.microsoft.com/en-us/dotnet/standard/io/how-to-copy-directories
        private static void DirectoryCopy(string sourceDirName, string destDirName, bool copySubDirs, List<string> ignoreExtensions)
        {
            // Get the subdirectories for the specified directory.
            DirectoryInfo dir = new DirectoryInfo(sourceDirName);

            if (!dir.Exists)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + sourceDirName);
            }

            DirectoryInfo[] dirs = dir.GetDirectories();
            // If the destination directory doesn't exist, create it.
            if (!Directory.Exists(destDirName))
            {
                Directory.CreateDirectory(destDirName);
            }

            // Get the files in the directory and copy them to the new location.
            FileInfo[] files = dir.GetFiles();
            foreach (FileInfo file in files)
            {
                bool should_copy = true;
                foreach (string ignored_extension in ignoreExtensions)
                {
                    if (file.Extension.ToUpper() == ignored_extension.ToUpper())
                    {
                        should_copy = false;
                        break;
                    }
                }
                if (should_copy)
                {
                    string temppath = Path.Combine(destDirName, file.Name);
                    if (Path.GetFileName(Path.GetDirectoryName(temppath)).ToUpper() == "IMAGES" && (
                        Path.GetExtension(temppath).ToUpper() == ".JPG" ||
                        Path.GetExtension(temppath).ToUpper() == ".JPEG" ||
                        Path.GetExtension(temppath).ToUpper() == ".PNG"))
                    {
                        //A little manual override to stop original images in the IMAGES folder
                        continue;
                    }
                    file.CopyTo(temppath, false);
                }
            }

            // If copying subdirectories, copy them and their contents to new location.
            if (copySubDirs)
            {
                foreach (DirectoryInfo subdir in dirs)
                {
                    string temppath = Path.Combine(destDirName, subdir.Name);
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs, ignoreExtensions);
                }
            }
        }

        /* Open UI Editor */
        private void openUiEditor_Click(object sender, EventArgs e)
        {
            UI_Editor uieditor = new UI_Editor();
            uieditor.Show();
        }

        /* Open Keybind Editor */
        private void openKeybindEditor_Click(object sender, EventArgs e)
        {
            Keybind_Editor keybindeditor = new Keybind_Editor();
            keybindeditor.Show();
        }

        private void groupBox4_Enter(object sender, EventArgs e)
        {

        }

        /* On Load */
        private void Splash_Load(object sender, EventArgs e)
        {
            /* LOAD DEBUG CONFIGURATIONS */
            REFRESH_DEBUG_LIST();
            JToken game_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/GAME_CORE.JSON"));
            DEBUG_DEFAULTTRACK.SelectedItem = game_config["default_track"].Value<string>();
            DEBUG_PLAYERCOUNT.Value = game_config["player_count"].Value<decimal>();
        }

        /* SAVE DEBUG CONFIG */
        private void DEBUG_SAVE_Click(object sender, EventArgs e)
        {
            JToken game_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/GAME_CORE.JSON"));
            game_config["default_track"] = DEBUG_DEFAULTTRACK.SelectedItem.ToString();
            game_config["player_count"] = DEBUG_PLAYERCOUNT.Value;
            File.WriteAllText("DATA/CONFIGS/GAME_CORE.JSON", game_config.ToString(Formatting.Indented));
            MessageBox.Show("Configuration saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /* UPDATE DEBUG LEVEL CHOICES */
        void REFRESH_DEBUG_LIST()
        {
            string[] levels = Directory.GetFiles("DATA/MODELS/", "*.SDKMESH", SearchOption.AllDirectories);
            DEBUG_DEFAULTTRACK.Items.Clear();
            foreach (string level in levels)
            {
                DEBUG_DEFAULTTRACK.Items.Add(Path.GetFileNameWithoutExtension(level));
            }
        }

        /* Open Map Manager */
        private void openMapManager_Click(object sender, EventArgs e)
        {
            Map_Manager mapManager = new Map_Manager();
            mapManager.Show();
        }
    }
}
