using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
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
        List<string> ignored_extensions = new List<string>();
        public Splash(bool shouldInitialiseGUI = true)
        {
            if (shouldInitialiseGUI)
            {
                //Only create GUI if needed (auto-compile launches want no GUI)
                InitializeComponent();
            }

            //Create list of ignored extensions for asset compilation
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
            /* We can now also ignore JPG/JPEG/PNG, but older models still use this format. Begin to depreciate it immediately! */
        }

        /* Open Asset Manager */
        private void openAssetManager_Click(object sender, EventArgs e)
        {
            Asset_Editor assetManager = new Asset_Editor();
            assetManager.Show();
        }

        /* Compile Assets */
        private void compileAssets_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;

            //Fix VS debugging directory config
            File.WriteAllText("Scarle2019/Scarle2019.vcxproj.user", "<?xml version=\"1.0\" encoding=\"utf-8\"?><Project ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\"><LocalDebuggerWorkingDirectory>$(SolutionDir)$(Configuration)\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments></PropertyGroup><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\"><LocalDebuggerWorkingDirectory>$(SolutionDir)$(Configuration)\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments></PropertyGroup></Project>");

            if (!autoCompileAssets())
            {
                Cursor.Current = Cursors.Default;
                MessageBox.Show("An error occured while compiling assets.\nMake sure that the game is closed and no files are open.", "Asset compile failed.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Cursor.Current = Cursors.Default;
            MessageBox.Show("Assets successfully compiled.", "Compiled assets.", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /* Copy All Asssets To Folder */
        void copyAssets(string output_directory, string path_mod)
        {
            if (Directory.Exists(output_directory))
            {
                Directory.Delete(output_directory, true);
            }
            DirectoryCopy(path_mod + "DATA/", output_directory, true);
        }

        /* Copy a Directory */
        //Modified from: https://docs.microsoft.com/en-us/dotnet/standard/io/how-to-copy-directories
        private void DirectoryCopy(string sourceDirName, string destDirName, bool copySubDirs)
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
                foreach (string ignored_extension in ignored_extensions)
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
                    if (file.Name == "IMPORTER_CONFIG.JSON") //Check to see we're in MODELS?
                    {
                        //Another override to stop importer files getting copied
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
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs);
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
            if (DEBUG_DEFAULTTRACK.SelectedIndex == -1)
            {
                MessageBox.Show("No track selected!", "Cannot save.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
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
                string file_name = Path.GetFileName(level);
                if (file_name.Length > 14 && file_name.Substring(file_name.Length - 13).ToUpper() == "DEBUG.SDKMESH")
                {
                    continue; //Skip over collision debug meshes
                }
                JToken game_config = JToken.Parse(File.ReadAllText("DATA/MODELS/" + Path.GetFileNameWithoutExtension(level) + "/" + Path.GetFileNameWithoutExtension(level) + ".JSON"));
                if (game_config["model_type"] == null || game_config["model_type"].Type != JTokenType.Integer || game_config["model_type"].Value<int>() != (int)ModelType.MAP)
                {
                    continue;
                }
                DEBUG_DEFAULTTRACK.Items.Add(Path.GetFileNameWithoutExtension(level));
            }
        }

        /* Open Map Manager */
        private void openMapManager_Click(object sender, EventArgs e)
        {
            Map_Manager mapManager = new Map_Manager();
            mapManager.Show();
        }

        /* Open VS Project */
        private void openProject_Click(object sender, EventArgs e)
        {
            fixVS();
            Process.Start("SOURCE/ThICC_Engine/ThICC_Engine.sln");
        }

        /* Compile assets */
        public bool autoCompileAssets(string path_mod = "")
        {
            //Create cache directory/file if it doesn't exist
            if (!Directory.Exists(path_mod + "CACHE") || !File.Exists(path_mod + "CACHE/DATA_CACHE.BIN"))
            {
                Directory.CreateDirectory(path_mod + "CACHE");
                using (BinaryWriter writer = new BinaryWriter(File.Open(path_mod + "CACHE/DATA_CACHE.BIN", FileMode.Create)))
                {
                    long placeholder = 0;
                    writer.Write(placeholder);
                }
            }

            try
            {
                //Copy to debug folder
                buildAssets("BUILDS/Debug", path_mod);

                //Copy to release folder
                buildAssets("BUILDS/Release", path_mod);

                return true;
            }
            catch
            {
                return false;
            }
        }

        /* Build assets for... */
        private void buildAssets(string output, string path_mod)
        {
            if (Directory.Exists(path_mod + output))
            {
                if (Directory.Exists(path_mod + output + "/DATA/"))
                {
                    //Check to see if we need to copy...
                    DirectoryInfo existing_data = new DirectoryInfo(path_mod + "DATA/");
                    FileInfo[] file_array = existing_data.GetFiles("*.*", SearchOption.AllDirectories);
                    long total_size = 0;
                    foreach (var file in file_array)
                    {
                        bool can_copy = true;
                        foreach (string ignored_extension in ignored_extensions)
                        {
                            if (Path.GetExtension(file.Name) == ignored_extension)
                            {
                                can_copy = false;
                            }
                        }
                        if (can_copy)
                        {
                            total_size += file.Length;
                        }
                    }
                    long orig_size = 0;
                    using (BinaryReader reader = new BinaryReader(File.Open(path_mod + "CACHE/DATA_CACHE.BIN", FileMode.Open)))
                    {
                        orig_size = reader.ReadInt64();
                    }
                    if (orig_size == total_size)
                    {
                        return;
                    }
                    //We do need to copy, save new cache value
                    using (BinaryWriter writer = new BinaryWriter(File.Open(path_mod + "CACHE/DATA_CACHE.BIN", FileMode.Create)))
                    {
                        writer.Write(Convert.ToInt64(total_size));
                    }
                }

                //Copy all
                copyAssets(path_mod + output + "/DATA/", path_mod);
                if (File.Exists(path_mod + output + "/Mario Kart Launcher.exe"))
                {
                    File.Delete(path_mod + output + "/Mario Kart Launcher.exe");
                }
                File.Copy(path_mod + "DATA/MarioKartLauncher.exe", path_mod + output + "/Mario Kart Launcher.exe");
            }
            return;
        }

        /* Fix VS config */
        public void fixVS()
        {
            File.WriteAllText("SOURCE/ThICC_Engine/ThICC_Engine.vcxproj.user", "<?xml version=\"1.0\" encoding=\"utf-8\"?><Project ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\"><LocalDebuggerWorkingDirectory>$(SolutionDir)$(Configuration)\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments></PropertyGroup><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\"><LocalDebuggerWorkingDirectory>$(SolutionDir)$(Configuration)\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments></PropertyGroup></Project>");
        }


        private void debug_btn_Click(object sender, EventArgs e)
        {
            Model_Importer importer = new Model_Importer(ModelType.PROP);
            importer.Show();
        }

        private void reloadDebugList_Click(object sender, EventArgs e)
        {
            REFRESH_DEBUG_LIST();
        }


        private void debug_button_Click(object sender, EventArgs e)
        {

        }
        private void button1_Click(object sender, EventArgs e)
        {

        }
        private void button2_Click(object sender, EventArgs e)
        {

        }
    }
}
