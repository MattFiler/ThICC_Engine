namespace EditorTool
{
    partial class Generic_Importer
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.assetNameText = new System.Windows.Forms.Label();
            this.assetName = new System.Windows.Forms.TextBox();
            this.importAsset = new System.Windows.Forms.Button();
            this.assetImportDesc = new System.Windows.Forms.Label();
            this.filePath = new System.Windows.Forms.TextBox();
            this.browseToFile = new System.Windows.Forms.Button();
            this.dropdownOption = new System.Windows.Forms.ComboBox();
            this.numericOption = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.numericOption)).BeginInit();
            this.SuspendLayout();
            // 
            // assetNameText
            // 
            this.assetNameText.AutoSize = true;
            this.assetNameText.Location = new System.Drawing.Point(12, 9);
            this.assetNameText.Name = "assetNameText";
            this.assetNameText.Size = new System.Drawing.Size(64, 13);
            this.assetNameText.TabIndex = 19;
            this.assetNameText.Text = "Asset Name";
            // 
            // assetName
            // 
            this.assetName.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.assetName.Location = new System.Drawing.Point(15, 25);
            this.assetName.Name = "assetName";
            this.assetName.Size = new System.Drawing.Size(264, 20);
            this.assetName.TabIndex = 18;
            // 
            // importAsset
            // 
            this.importAsset.Location = new System.Drawing.Point(15, 91);
            this.importAsset.Name = "importAsset";
            this.importAsset.Size = new System.Drawing.Size(264, 28);
            this.importAsset.TabIndex = 17;
            this.importAsset.Text = "Import";
            this.importAsset.UseVisualStyleBackColor = true;
            this.importAsset.Click += new System.EventHandler(this.importAsset_Click);
            // 
            // assetImportDesc
            // 
            this.assetImportDesc.AutoSize = true;
            this.assetImportDesc.Location = new System.Drawing.Point(12, 48);
            this.assetImportDesc.Name = "assetImportDesc";
            this.assetImportDesc.Size = new System.Drawing.Size(0, 13);
            this.assetImportDesc.TabIndex = 16;
            // 
            // filePath
            // 
            this.filePath.Location = new System.Drawing.Point(15, 64);
            this.filePath.Name = "filePath";
            this.filePath.ReadOnly = true;
            this.filePath.Size = new System.Drawing.Size(183, 20);
            this.filePath.TabIndex = 15;
            // 
            // browseToFile
            // 
            this.browseToFile.Location = new System.Drawing.Point(204, 63);
            this.browseToFile.Name = "browseToFile";
            this.browseToFile.Size = new System.Drawing.Size(75, 22);
            this.browseToFile.TabIndex = 14;
            this.browseToFile.Text = "Browse";
            this.browseToFile.UseVisualStyleBackColor = true;
            this.browseToFile.Click += new System.EventHandler(this.browseToFile_Click);
            // 
            // dropdownOption
            // 
            this.dropdownOption.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.dropdownOption.FormattingEnabled = true;
            this.dropdownOption.Location = new System.Drawing.Point(15, 25);
            this.dropdownOption.Name = "dropdownOption";
            this.dropdownOption.Size = new System.Drawing.Size(264, 21);
            this.dropdownOption.TabIndex = 20;
            this.dropdownOption.Visible = false;
            // 
            // numericOption
            // 
            this.numericOption.Location = new System.Drawing.Point(15, 64);
            this.numericOption.Name = "numericOption";
            this.numericOption.Size = new System.Drawing.Size(264, 20);
            this.numericOption.TabIndex = 21;
            this.numericOption.Value = new decimal(new int[] {
            25,
            0,
            0,
            0});
            this.numericOption.Visible = false;
            // 
            // Generic_Importer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(294, 130);
            this.Controls.Add(this.numericOption);
            this.Controls.Add(this.dropdownOption);
            this.Controls.Add(this.assetNameText);
            this.Controls.Add(this.assetName);
            this.Controls.Add(this.importAsset);
            this.Controls.Add(this.assetImportDesc);
            this.Controls.Add(this.filePath);
            this.Controls.Add(this.browseToFile);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Generic_Importer";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Load += new System.EventHandler(this.Generic_Importer_Load);
            ((System.ComponentModel.ISupportInitialize)(this.numericOption)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label assetNameText;
        private System.Windows.Forms.TextBox assetName;
        private System.Windows.Forms.Button importAsset;
        private System.Windows.Forms.Label assetImportDesc;
        private System.Windows.Forms.TextBox filePath;
        private System.Windows.Forms.Button browseToFile;
        private System.Windows.Forms.ComboBox dropdownOption;
        private System.Windows.Forms.NumericUpDown numericOption;
    }
}