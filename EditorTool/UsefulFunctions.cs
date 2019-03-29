using NAudio.Gui;
using NAudio.Wave;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Integration;

namespace EditorTool
{
    class UsefulFunctions
    {
        /* Load an image preview from an asset list to a picture box */
        public bool loadImagePreview(ListBox assetList, PictureBox imagePreview)
        {
            if (assetList.SelectedIndex == -1)
            {
                imagePreview.Image = null;
                return false;
            }
            imagePreview.Visible = true;

            string file_path_without_extension = "DATA/IMAGES/" + assetList.SelectedItem.ToString() + ".";
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

        /* Load a model preview from asset list into a model viewer */
        public bool loadModelPreview(ListBox assetList, ElementHost modelPreview)
        {
            if (assetList.SelectedIndex == -1)
            {
                modelPreview.Child = new ModelViewer("");
                return false;
            }

            modelPreview.Visible = true;
            modelPreview.Child = new ModelViewer("DATA/MODELS/" + assetList.SelectedItem.ToString() + "/" + assetList.SelectedItem.ToString() + ".OBJ");
            return true;
        }

        /* Load a font preview from asset list into a picture box */
        public bool loadFontPreview(ListBox assetList, PictureBox imagePreview)
        {
            if (assetList.SelectedIndex == -1)
            {
                imagePreview.Image = null;
                return false;
            }
            imagePreview.Visible = true;

            using (var tempPreviewImg = new Bitmap("DATA/FONTS/" + assetList.SelectedItem.ToString() + ".BMP"))
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
            sound_stream = File.Open("DATA/SOUNDS/" + assetList.SelectedItem.ToString() + ".WAV", FileMode.Open, FileAccess.Read);
        }
        private void closeSoundStream()
        {
            if (sound_stream != null)
            {
                sound_stream.Close();
            }
            sound_stream = null;
        }

        /* Get a localised string */
        public string getLocalisedString(string request, string language = "ENGLISH")
        {
            JObject string_config = JObject.Parse(File.ReadAllText("DATA/CONFIGS/LOCALISATION.JSON"));
            return string_config[language][request].Value<string>();
        }
    }
}
