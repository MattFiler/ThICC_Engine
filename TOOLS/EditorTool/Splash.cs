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
        UsefulFunctions common_functions = new UsefulFunctions();
        List<string> ignored_extensions = new List<string>();
        int compiled_asset_count = 0;
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
            ignored_extensions.Add(".jpg");
            ignored_extensions.Add(".jpeg");
            ignored_extensions.Add(".png");
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
            fixVS();

            if (autoCompileAssets(true) == -1)
            {
                Cursor.Current = Cursors.Default;
                MessageBox.Show("An error occured while compiling assets.\nMake sure that the game is closed and no files are open.", "Asset compile failed.", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Cursor.Current = Cursors.Default;
            MessageBox.Show("Finished compiling " + compiled_asset_count + " asset(s).", "Compiled assets.", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
                    if (File.Exists(temppath))
                    {
                        if (Path.GetFileName(Path.GetDirectoryName(temppath)).ToUpper() == "IMAGES" && (
                            Path.GetExtension(temppath).ToUpper() == ".JPG" ||
                            Path.GetExtension(temppath).ToUpper() == ".JPEG" ||
                            Path.GetExtension(temppath).ToUpper() == ".PNG"))
                        {
                            continue; //A little manual override to stop original images in the IMAGES folder
                        }
                        if (file.Name == "IMPORTER_CONFIG.JSON") //Check to see we're in MODELS?
                        {
                            continue; //Another override to stop importer files getting copied
                        }
                        FileInfo temppathinfo = new FileInfo(temppath);
                        if (file.LastWriteTime == temppathinfo.LastWriteTime && file.Length == temppathinfo.Length)
                        {
                            continue; //If mod time and length are the same, we can be confident the file doesn't need to be copied
                        }
                        File.Delete(temppath);
                    }
                    file.CopyTo(temppath, false);
                    compiled_asset_count++;
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

        /* Open asset group preselect */
        private void openMapManager_Click(object sender, EventArgs e)
        {
            Asset_Manager_Preselect assetManager = new Asset_Manager_Preselect();
            assetManager.Show();
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
            Keybinds_Manager keybindeditor = new Keybinds_Manager();
            keybindeditor.Show();
        }

        /* Open Engine Configs */
        private void openEngineConfigs_Click(object sender, EventArgs e)
        {
            Game_Config_Preselect engineConfigs = new Game_Config_Preselect();
            engineConfigs.Show();
        }

        /* Open texture map animator */
        private void openMapAnimator_Click(object sender, EventArgs e)
        {
            Texture_Map_Animator mapAnimator = new Texture_Map_Animator();
            mapAnimator.Show();
        }

        /* Open VS Project */
        private void openProject_Click(object sender, EventArgs e)
        {
            fixVS();
            Process.Start(Directory.GetCurrentDirectory() + "/SOURCE/ThICC_Engine.sln");
        }

        /* Compile assets */
        public int autoCompileAssets(bool show_notifs, string path_mod = "")
        {
            try
            {
                //Copy to output folders if needed
                buildAssets("BUILDS/Debug", path_mod);
                buildAssets("BUILDS/Release", path_mod);
                buildAssets("BUILDS/Arcade", path_mod);
                return compiled_asset_count;
            }
            catch
            {
                return -1;
            }
        }

        /* Build assets for... */
        private int buildAssets(string output, string path_mod)
        {
            if (Directory.Exists(path_mod + output))
            {
                //Copy all
                DirectoryCopy(path_mod + "DATA/", path_mod + output + "/DATA/", true);
                CopyToOutput(path_mod, output, "MarioKartLauncher.exe", "Mario Kart Launcher.exe");
                CopyToOutput(path_mod, output, "DEMO.exe", "DEMO.exe");
                return 1;
            }
            return -1;
        }

        /* Fix VS config */
        public void fixVS()
        {
            File.WriteAllText(Directory.GetCurrentDirectory() + "/SOURCE/ThICC_Engine.vcxproj.user", "<?xml version=\"1.0\" encoding=\"utf-8\"?><Project ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\"><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments><LocalDebuggerWorkingDirectory>$(SolutionDir)..\\\\BUILDS\\$(Configuration)\\\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor></PropertyGroup><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\"><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments><LocalDebuggerWorkingDirectory>$(SolutionDir)..\\\\BUILDS\\$(Configuration)\\\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor></PropertyGroup><PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Arcade|Win32'\"><LocalDebuggerCommandArguments>Launcher_Auth</LocalDebuggerCommandArguments><LocalDebuggerWorkingDirectory>$(SolutionDir)..\\\\BUILDS\\$(Configuration)\\\\</LocalDebuggerWorkingDirectory><DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor></PropertyGroup></Project>");
        }

        /* Copy to output */
        private void CopyToOutput(string path_mod, string output, string input, string copyto)
        {
            if (File.Exists(path_mod + output + "/" + copyto))
            {
                FileInfo input_file = new FileInfo(path_mod + "DATA/" + input);
                FileInfo output_file = new FileInfo(path_mod + output + "/" + copyto);
                if (input_file.LastWriteTime == output_file.LastWriteTime &&
                    input_file.Length == output_file.Length)
                {
                    return; //Don't continue if it exists and is the same!
                }
                File.Delete(path_mod + output + "/" + copyto);
            }
            File.Copy(path_mod + "DATA/" + input, path_mod + output + "/" + copyto);
            compiled_asset_count++;
        }




        private void debug_btn_Click(object sender, EventArgs e)
        {
            Model_Importer importer = new Model_Importer(ModelType.PROP);
            importer.Show();
        }

        private void Splash_Load(object sender, EventArgs e)
        {
        }

        private void DEBUG_SAVE_Click(object sender, EventArgs e)
        {
        }
        private void groupBox4_Enter(object sender, EventArgs e)
        {

        }
        private void reloadDebugList_Click(object sender, EventArgs e)
        {
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
        private void openVehicleManager_Click(object sender, EventArgs e)
        {
        }
        private void openCharacterManager_Click(object sender, EventArgs e)
        {
        }
    }
}
