using NAudio.Gui;
using NAudio.Wave;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Media;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Integration;

namespace EditorTool
{
    /* A struct for storing a queue of assets to modify the useage tags on */
    public struct UseageQueue
    {
        public bool removing; //true if removing / false if adding
        public AssetType asset_type;
        public string asset_name;
    }

    /* Common functionality library */
    class UsefulFunctions
    {
        /*
         * 
         * Asset Previewing Functionality
         * 
         */

        /* Load an image preview from an asset list to a picture box */
        public bool loadImagePreview(ListBox assetList, PictureBox imagePreview)
        {
            imagePreview.BackColor = Color.Transparent;
            if (assetList.SelectedIndex == -1)
            {
                imagePreview.Image = null;
                return false;
            }
            imagePreview.Visible = true;

            string file_path_without_extension = getFolder(AssetType.IMAGE) + assetList.SelectedItem.ToString() + ".";
            string file_path_with_extension = "";
            if (File.Exists(file_path_without_extension + "PNG"))
            {
                file_path_with_extension = file_path_without_extension + "PNG";
            }
            else if (File.Exists(file_path_without_extension + "JPG"))
            {
                file_path_with_extension = file_path_without_extension + "JPG";
            }
            else if (File.Exists(file_path_without_extension + "JPEG"))
            {
                file_path_with_extension = file_path_without_extension + "JPEG";
            }
            else
            {
                return false;
            }

            using (var tempPreviewImg = new Bitmap(file_path_with_extension))
            {
                imagePreview.Image = new Bitmap(tempPreviewImg);
            }
            return true;
        }

        /* Try find our material preview, show it if we find it */
        public void loadMaterialPreview(JToken this_token, PictureBox materialPreview, string model_path = "")
        {
            string[] map_types = { "map_Kd", "map_Ks", "map_Kn", "norm", "map_Ke", "map_emissive" };
            string path_to_mat = "";
            foreach (string map_type in map_types)
            {
                if (this_token[map_type].Value<string>() == "")
                {
                    continue;
                }
                if (this_token[map_type].Type == JTokenType.String)
                {
                    path_to_mat = this_token[map_type].Value<string>();
                    break;
                }
            }

            if (path_to_mat == "" || (!File.Exists(path_to_mat) && !File.Exists(model_path + path_to_mat)))
            {
                return;
            }
            if (!File.Exists(path_to_mat))
            {
                path_to_mat = model_path + path_to_mat;
            }

            using (var tempPreviewImg = new Bitmap(path_to_mat))
            {
                materialPreview.Image = new Bitmap(tempPreviewImg);
            }
        }

        /* Load colour preview of material properties */
        public bool loadMaterialColourPreview(JToken material_config, string property, PictureBox preview)
        {
            if (material_config[property] != null)
            {
                string[] colour = material_config[property].Value<string>().Split(' ');
                preview.BackColor = Color.FromArgb(
                    255,
                    Convert.ToInt32(Convert.ToDouble(colour[0]) * 255),
                    Convert.ToInt32(Convert.ToDouble(colour[1]) * 255),
                    Convert.ToInt32(Convert.ToDouble(colour[2]) * 255));
                return true;
            }
            return false;
        }

        /* Load a model preview from asset list into a model viewer */
        public bool loadModelPreview(ListBox assetList, ElementHost modelPreview)
        {
            if (assetList.SelectedIndex == -1)
            {
                modelPreview.Child = new ModelViewer("");
                return false;
            }

            modelPreview.Visible = true;
            modelPreview.Child = new ModelViewer(getFolder(AssetType.MODEL) + assetList.SelectedItem.ToString() + "/" + assetList.SelectedItem.ToString() + ".OBJ");
            return true;
        }

        /* Load a font preview from asset list into a picture box */
        public bool loadFontPreview(ListBox assetList, PictureBox imagePreview)
        {
            imagePreview.BackColor = Color.Black;
            if (assetList.SelectedIndex == -1)
            {
                imagePreview.Image = null;
                return false;
            }
            imagePreview.Visible = true;

            using (var tempPreviewImg = new Bitmap(getFolder(AssetType.FONT) + assetList.SelectedItem.ToString() + ".BMP"))
            {
                imagePreview.Image = new Bitmap(tempPreviewImg);
            }
            return true;
        }

        /* Load a string preview */
        public bool loadStringPreview(ListBox assetList, TextBox localisationPreview, JObject localisation_config)
        { 
            if (assetList.SelectedIndex == -1)
            {
                return false;
            }
            localisationPreview.Visible = true;
            localisationPreview.Text = localisation_config["ENGLISH"][assetList.SelectedItem.ToString()].Value<string>();
            return true;
        }

        /* Load a sound preview from asset list into a wave viewer */
        public bool loadSoundPreview(ListBox assetList, SoundPlayer sound_player, WaveViewer soundPreview, Button playSoundPreview)
        {
            sound_player.Stop();
            if (assetList.SelectedIndex == -1)
            {
                closeSoundStream();
                soundPreview.WaveStream = null;
                sound_player.Stream = null;
                return false;
            }
            playSoundPreview.Visible = true;
            soundPreview.Visible = true;
            openSoundStream(assetList);
            soundPreview.WaveStream = new WaveFileReader(sound_stream);
            soundPreview.SamplesPerPixel = 150;
            return true;
        }

        /* Play a sound preview currently being previewed */
        public bool playSoundPreview(ListBox assetList, SoundPlayer sound_player)
        {
            openSoundStream(assetList);
            sound_player.Stream = sound_stream;
            try { sound_player.Play(); }
            catch { MessageBox.Show("An error ocurred while trying to play this sound - it may have imported incorrectly.", "Error.", MessageBoxButtons.OK, MessageBoxIcon.Error); }
            return true;
        }

        ///////////////////////////////////////////////
        ///////////////////////////////////////////////

        /* 
         * 
         * Sound Streaming Functionality
         * 
         */

        /* A public handle to close lingering sound streams */
        public bool closeLingeringSoundStreams()
        {
            closeSoundStream();
            return true;
        }

        /* Update sound stream to use for any previews */
        Stream sound_stream;
        private void openSoundStream(ListBox assetList)
        {
            closeSoundStream();
            sound_stream = File.Open(getFolder(AssetType.SOUND) + assetList.SelectedItem.ToString() + ".WAV", FileMode.Open, FileAccess.Read);
        }
        private void closeSoundStream()
        {
            if (sound_stream != null)
            {
                sound_stream.Close();
            }
            sound_stream = null;
        }

        ///////////////////////////////////////////////
        ///////////////////////////////////////////////

        /* 
         * 
         * File/Colour Picker
         * 
         */

        /* Allow user to locate a file */
        public string userLocatedFile(string filter)
        {
            OpenFileDialog filePicker = new OpenFileDialog();
            filePicker.Filter = filter;
            if (filePicker.ShowDialog() == DialogResult.OK)
            {
                return filePicker.FileName;
            }
            return "";
        }

        /* Load a colour picker with default colour */
        public void userChosenColour(PictureBox preview_window)
        {
            using (var colour_popup = new ColorDialog())
            {
                colour_popup.Color = preview_window.BackColor;
                colour_popup.FullOpen = true;
                colour_popup.ShowDialog();
                preview_window.BackColor = colour_popup.Color;
            }
        }

        ///////////////////////////////////////////////
        ///////////////////////////////////////////////

        /* 
         * 
         * Config Parsing Functionality
         * 
         */

        /* Get a localised string */
        public string getLocalisedString(string request, string language = "ENGLISH")
        {
            JObject string_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/LOCALISATION.JSON"));
            return string_config[language][request].Value<string>();
        }

        /* Get a useage tag string (this unifies all tags in a simple way) */
        public string getUseageTagFor(AssetCompType comp_type)
        {
            switch (comp_type)
            {
                case AssetCompType.CHARACTER:
                    return "Character";
                case AssetCompType.MAP:
                    return "Map";
                case AssetCompType.VEHICLE:
                    return "Vehicle";
            }
            return "";
        }

        /* Add something to the useage queue to batch delete */
        List<UseageQueue> useage_queue = new List<UseageQueue>();
        public void pushToUseageQueue(UseageQueue new_item)
        {
            useage_queue.Add(new_item);
        }

        /* Batch handle the queue */
        public void executeUseageQueueForTag(string tag)
        {
            foreach (UseageQueue useage in useage_queue)
            {
                if (useage.removing)
                {
                    removeUseageTag(useage.asset_type, useage.asset_name, tag);
                }
                else
                {
                    addUseageTag(useage.asset_type, useage.asset_name, tag);
                }
            }
        }

        /* Get the useages for the asset */
        public List<string> getUseages(AssetType type, string asset_name)
        {
            string path = getConfigPath(type, asset_name);
            JObject inuse_config = JObject.Parse(File.ReadAllText(path));
            JArray array;
            if (type == AssetType.STRING)
            {
                array = inuse_config[asset_name]["in_use_in"].Value<JArray>();
            }
            else
            {
                array = inuse_config["in_use_in"].Value<JArray>();
            }
            List<string> to_return = new List<string>();
            foreach (JToken useage in array)
            {
                to_return.Add(useage.Value<string>());
            }
            return to_return;
        }

        /* Add a "in_use_in" tag to a config */
        public void addUseageTag(AssetType type, string asset_name, string new_tag)
        {
            string path = getConfigPath(type, asset_name);
            JObject config = JObject.Parse(File.ReadAllText(path));
            if (type == AssetType.STRING)
            {
                config[asset_name]["in_use_in"].Value<JArray>().Add(new_tag);
            }
            else
            {
                config["in_use_in"].Value<JArray>().Add(new_tag);
            }
            File.WriteAllText(path, config.ToString(Formatting.Indented));
        }

        /* Remove a "in_use_in" tag from a config */
        public void removeUseageTag(AssetType type, string asset_name, string old_tag)
        {
            string path = getConfigPath(type, asset_name);
            JObject config = JObject.Parse(File.ReadAllText(path));
            if (type == AssetType.STRING)
            {
                RemoveValue(config[asset_name]["in_use_in"].Value<JArray>(), old_tag);
            }
            else
            {
                RemoveValue(config["in_use_in"].Value<JArray>(), old_tag);
            }
            File.WriteAllText(path, config.ToString(Formatting.Indented));
        }
        //Fixing removal issue
        private void RemoveValue(JArray array, string to_delete)
        {
            foreach (JToken thing in array)
            {
                if (thing.Value<string>() == to_delete)
                {
                    thing.Remove();
                    break;
                }
            }
        }

        ///////////////////////////////////////////////
        ///////////////////////////////////////////////

        /* 
         * 
         * Asset Selection
         * 
         */

        /* Launch asset selector */
        public string selectAsset(AssetType asset_type, string existing_option = "")
        {
            string asset_name = "";
            using (var form = new Asset_Browser(asset_type, existing_option))
            {
                form.ShowDialog();
                if (form.DialogResult == DialogResult.OK)
                {
                    asset_name = form.selected_asset;

                    UseageQueue new_queue_item = new UseageQueue();
                    new_queue_item.asset_name = form.selected_asset;
                    new_queue_item.asset_type = asset_type;
                    new_queue_item.removing = false;
                    useage_queue.Add(new_queue_item);
                }
            }
            return asset_name;
        }

        /* Asset selection handler */
        public void assetSelectHandler(TextBox textbox, AssetType type)
        {
            if (textbox.Text != "")
            {
                UseageQueue new_queue_item = new UseageQueue();
                new_queue_item.asset_name = textbox.Text;
                new_queue_item.asset_type = type;
                new_queue_item.removing = true;
                useage_queue.Add(new_queue_item);
            }
            string selection = selectAsset(type, textbox.Text);
            if (selection == "")
            {
                return;
            }
            textbox.Text = selection;
        }

        ///////////////////////////////////////////////
        ///////////////////////////////////////////////

        /* 
         * 
         * Filepath Generator
         * 
         */

        /* Get a folder path */
        public string getFolder(AssetType type, string asset_name = "")
        {
            switch (type)
            {
                case AssetType.FONT:
                    return "DATA/FONTS/";
                case AssetType.IMAGE:
                    return "DATA/IMAGES/";
                case AssetType.MODEL:
                    if (asset_name == "")
                    {
                        return "DATA/MODELS/";
                    }
                    return "DATA/MODELS/" + asset_name + "/";
                case AssetType.SOUND:
                    return "DATA/SOUNDS/";
                case AssetType.CUBEMAP:
                    return "DATA/CUBEMAPS/";
            }
            throw new InvalidOperationException("Unhandled AssetType.");
        }

        /* Get an extension */
        public string getExtension(AssetType type)
        {
            switch (type)
            {
                case AssetType.FONT:
                    return ".SPRITEFONT";
                case AssetType.IMAGE:
                    return ".DDS";
                case AssetType.MODEL:
                    return ".SDKMESH";
                case AssetType.SOUND:
                    return ".WAV";
                case AssetType.CUBEMAP:
                    return ".DDS"; //"_SPECULAR.DDS" for irradiance, "_DIFFUSE.DDS" for radiance
            }
            throw new InvalidOperationException("Unhandled AssetType.");
        }

        /* Get config path */
        public string getConfigPath(AssetType type, string asset_name = "")
        {
            if (type == AssetType.STRING)
            {
                return "DATA/CONFIGS/LOCALISATION_INUSE.JSON";
            }
            else
            {
                return getFolder(type, asset_name) + asset_name + ".JSON";
            }
        }

        ///////////////////////////////////////////////
        ///////////////////////////////////////////////

        /* 
         * 
         * Asset Import Functionality
         * 
         */

        /* Import a sound asset */
        public bool importSound(string asset_name, string sound_path)
        {
            string asset_path = getFolder(AssetType.SOUND) + asset_name.ToUpper() + ".WAV";
            string asset_path_orig_ext = getFolder(AssetType.SOUND) + asset_name.ToUpper() + Path.GetExtension(sound_path);

            if (File.Exists(asset_path) || sound_path == "" || asset_name == "" || !Regex.IsMatch(asset_name, "^[_a-zA-Z0-9\x20]+$"))
            {
                if (sound_path == "" || asset_name == "")
                {
                    MessageBox.Show("Please fill out all required inputs.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                else if (!Regex.IsMatch(asset_name, "^[_a-zA-Z0-9\x20]+$"))
                {
                    MessageBox.Show("Your asset name cannot contain any special characters.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                MessageBox.Show("Couldn't import sound, a sound with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            else
            {
                //Copy file to working directory
                File.Copy(sound_path, asset_path_orig_ext);

                string output = "";
                if (Path.GetExtension(sound_path).ToUpper() != ".WAV")
                {
                    //Convert file to WAV if it isn't already
                    ProcessStartInfo soundConverter = new ProcessStartInfo();
                    soundConverter.WorkingDirectory = getFolder(AssetType.SOUND);
                    soundConverter.FileName = getFolder(AssetType.SOUND) + "ffmpeg.exe";
                    soundConverter.Arguments = "-i \"" + Path.GetFileName(asset_path_orig_ext) + "\" \"" + Path.GetFileName(asset_path) + "\"";
                    soundConverter.UseShellExecute = false;
                    soundConverter.RedirectStandardOutput = true;
                    soundConverter.CreateNoWindow = true;
                    Process converterProcess = Process.Start(soundConverter);
                    StreamReader reader = converterProcess.StandardOutput;
                    converterProcess.WaitForExit();

                    //Capture DDS convert output incase we errored
                    output = reader.ReadToEnd();

                    File.Delete(asset_path_orig_ext);
                }

                if (!File.Exists(asset_path))
                {
                    //Conversion failed, show reason if requested
                    if (File.Exists(asset_path_orig_ext))
                    {
                        File.Delete(asset_path_orig_ext);
                    }
                    DialogResult showErrorInfo = MessageBox.Show("Sound import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                    if (showErrorInfo == DialogResult.Yes)
                    {
                        MessageBox.Show(output, "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    return false;
                }

                //Create JSON data
                JToken asset_json = JToken.Parse("{\"asset_name\": \"" + asset_name + "\", \"asset_type\": \"Sounds\", \"is_looping\": false, \"volume\": 1.0, \"pitch\": 0.0, \"pan\": 0.0, \"in_use_in\": []}");
                File.WriteAllText(asset_path.Substring(0, asset_path.Length - 3) + "JSON", asset_json.ToString(Formatting.Indented));

                //Import success
                MessageBox.Show("Sound successfully imported.", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);

                return true;
            }
        }

        /* Import an image asset */
        public bool importImage(string asset_name, string image_path)
        {
            string asset_path = getFolder(AssetType.IMAGE) + asset_name.ToUpper() + ".DDS";
            string asset_path_orig_ext = getFolder(AssetType.IMAGE) + asset_name.ToUpper() + Path.GetExtension(image_path);

            if (File.Exists(asset_path) || image_path == "" || asset_name == "" || !Regex.IsMatch(asset_name, "^[_a-zA-Z0-9\x20]+$"))
            {
                if (image_path == "" || asset_name == "")
                {
                    MessageBox.Show("Please fill out all required inputs.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                else if (!Regex.IsMatch(asset_name, "^[_a-zA-Z0-9\x20]+$"))
                {
                    MessageBox.Show("Your asset name cannot contain any special characters.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                MessageBox.Show("Couldn't import asset, an image with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            else
            {
                //Copy file to "working directory"
                File.Copy(image_path, asset_path_orig_ext);

                //Convert copied image to DDS
                ProcessStartInfo imageConverter = new ProcessStartInfo();
                imageConverter.WorkingDirectory = getFolder(AssetType.IMAGE);
                imageConverter.FileName = getFolder(AssetType.IMAGE) + "texconv.exe";
                imageConverter.Arguments = "\"" + Path.GetFileName(asset_path_orig_ext) + "\"";
                imageConverter.UseShellExecute = false;
                imageConverter.RedirectStandardOutput = true;
                imageConverter.CreateNoWindow = true;
                Process converterProcess = Process.Start(imageConverter);
                StreamReader reader = converterProcess.StandardOutput;
                converterProcess.WaitForExit();

                //Capture DDS convert output incase we errored
                string output = reader.ReadToEnd();

                //Get dimensions
                int image_width = 0;
                int image_height = 0;
                using (Image img = Image.FromFile(asset_path_orig_ext))
                {
                    image_width = img.Width;
                    image_height = img.Height;
                }
                File.Move(asset_path_orig_ext, asset_path_orig_ext.ToUpper());

                //Uppercase extension pls
                if (File.Exists(asset_path.Substring(0, asset_path.Length - 3) + "dds"))
                {
                    File.Move(asset_path.Substring(0, asset_path.Length - 3) + "dds", asset_path);
                }

                if (!File.Exists(asset_path))
                {
                    //Import failed, show reason if requested
                    if (File.Exists(asset_path_orig_ext))
                    {
                        File.Delete(asset_path_orig_ext);
                    }
                    DialogResult showErrorInfo = MessageBox.Show("Image import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                    if (showErrorInfo == DialogResult.Yes)
                    {
                        MessageBox.Show(output, "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    return false;
                }

                //Create JSON data
                JToken asset_json = JToken.Parse("{\"asset_name\": \"" + asset_name + "\", \"asset_type\": \"Images\", \"visible\": true, \"is_2d\": true, \"res_x\": " + image_width + ", \"res_y\": " + image_height + ", \"x_pos\": 0, \"y_pos\": 0, \"in_use_in\": []}");
                File.WriteAllText(asset_path.Substring(0, asset_path.Length - 3) + "JSON", asset_json.ToString(Formatting.Indented));

                //Import success
                MessageBox.Show("Image successfully imported.", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);

                return true;
            }
        }

        /* Import a system font */
        public bool importFont(ComboBox selected_font, decimal font_size)
        {
            string asset_path = getFolder(AssetType.FONT) + selected_font.SelectedItem.ToString().ToUpper() + ".SPRITEFONT";

            if (File.Exists(asset_path) || selected_font.SelectedItem.ToString() == "" || selected_font.SelectedIndex == -1 || !(font_size > 0))
            {
                if (File.Exists(asset_path))
                {
                    MessageBox.Show("Couldn't import asset, an image with the same name already exists.", "Import Failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                MessageBox.Show("Please fill out all required inputs.", "Import failed!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            else
            {
                //Convert font to SPRITEFONT
                ProcessStartInfo fontImporter = new ProcessStartInfo();
                fontImporter.WorkingDirectory = getFolder(AssetType.FONT);
                fontImporter.FileName = getFolder(AssetType.FONT) + "MakeSpriteFont.exe";
                fontImporter.Arguments = "\"" + selected_font.SelectedItem.ToString() + "\" \"" + selected_font.SelectedItem.ToString().ToUpper() + ".SPRITEFONT\" /FontSize:" + font_size + " /DebugOutputSpriteSheet:\"" + selected_font.SelectedItem.ToString().ToUpper() + ".BMP\"";
                fontImporter.UseShellExecute = false;
                fontImporter.RedirectStandardOutput = true;
                fontImporter.CreateNoWindow = true;
                Process converterProcess = Process.Start(fontImporter);
                StreamReader reader = converterProcess.StandardOutput;
                converterProcess.WaitForExit();

                //Capture output incase we errored
                string output = reader.ReadToEnd();

                if (!File.Exists(asset_path))
                {
                    //Import failed, show reason if requested
                    DialogResult showErrorInfo = MessageBox.Show("Font import failed!\nWould you like error info?", "Import failed!", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                    if (showErrorInfo == DialogResult.Yes)
                    {
                        MessageBox.Show(output, "Error details...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    return false;
                }

                //Create JSON data
                JToken asset_json = JToken.Parse("{\"asset_name\": \"" + selected_font.SelectedItem.ToString() + "\", \"asset_type\": \"Fonts\", \"in_use_in\": []}");
                File.WriteAllText(asset_path.Substring(0, asset_path.Length - 10) + "JSON", asset_json.ToString(Formatting.Indented));

                //Import success
                MessageBox.Show("Font successfully imported.", "Imported!", MessageBoxButtons.OK, MessageBoxIcon.Information);

                return true;
            }
        }

        ///////////////////////////////////////////////
        ///////////////////////////////////////////////

        /* 
         * 
         * Misc Functionality
         * 
         */

        /* Make a string capitalised */
        public string makeCapitalised(string text)
        {
            return text.Substring(0, 1).ToUpper() + text.Substring(1);
        }

        /* Work out if an image has transparency */
        public bool hasTransparency(string image_path)
        {
            if (!File.Exists(image_path))
            {
                return false;
            }
            using (var image = new Bitmap(image_path))
            {
                for (int x = 0; x < image.Width; x++)
                {
                    for (int y = 0; y < image.Height; y++)
                    {
                        var this_pixel = image.GetPixel(x, y);
                        if (this_pixel.A != 255)
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
    }
}
