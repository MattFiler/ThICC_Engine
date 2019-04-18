using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Text;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class Generic_Importer : Form
    {
        AssetType import_type;
        string asset_picker_filter = "";
        UsefulFunctions function_library = new UsefulFunctions();
        public Generic_Importer(AssetType type_to_import)
        {
            import_type = type_to_import;
            InitializeComponent();
        }

        /* Configure form for asset type on load */
        private void Generic_Importer_Load(object sender, EventArgs e)
        {
            switch (import_type)
            {
                case AssetType.SOUND:
                    this.Text = "Import Sound";
                    assetImportDesc.Text = "Sound File (WAV/MP3/OGG)";
                    asset_picker_filter = "Sound (WAV/MP3/OGG)|*.WAV;*.MP3;*.OGG";
                    break;
                case AssetType.IMAGE:
                    this.Text = "Import Image";
                    assetImportDesc.Text = "Image File (PNG/JPG/JPEG)";
                    asset_picker_filter = "Image (PNG/JPG/JPEG)|*.PNG;*.JPG;*.JPEG";
                    break;
                case AssetType.FONT:
                    this.Text = "Import Font";
                    assetNameText.Text = "Font Name";
                    assetImportDesc.Text = "Default Font Size";
                    configureFontImport(); //Some tweaks we need to import fonts
                    break;
            }
        }

        /* Browse to asset file */
        private void browseToFile_Click(object sender, EventArgs e)
        {
            function_library.userLocatedFile(asset_picker_filter);
        }

        /* Import asset if valid */
        private void importAsset_Click(object sender, EventArgs e)
        {
            switch (import_type)
            {
                case AssetType.SOUND:
                    if (function_library.importSound(assetName.Text, filePath.Text))
                    {
                        this.Close();
                    }
                    break;
                case AssetType.IMAGE:
                    if (function_library.importImage(assetName.Text, filePath.Text))
                    {
                        this.Close();
                    }
                    break;
                case AssetType.FONT:
                    if (function_library.importFont(dropdownOption, numericOption.Value))
                    {
                        this.Close();
                    }
                    break;
            }
        }

        /* Configure form for font imports */
        void configureFontImport(bool swap = true)
        {
            //Tweak UI
            numericOption.Visible = true;
            dropdownOption.Visible = true;
            assetName.Visible = false;
            browseToFile.Visible = false;
            filePath.Visible = false;

            //Load all system fonts
            //Thanks: https://stackoverflow.com/a/8657854/3798962
            using (InstalledFontCollection fontsCollection = new InstalledFontCollection())
            {
                FontFamily[] fontFamilies = fontsCollection.Families;
                foreach (FontFamily font in fontFamilies)
                {
                    dropdownOption.Items.Add(font.Name);
                }
            }
        }
    }
}
