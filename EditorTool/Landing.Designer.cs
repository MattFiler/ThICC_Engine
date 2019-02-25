namespace EditorTool
{
    partial class Landing
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
            this.importModel = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.importFont = new System.Windows.Forms.Button();
            this.importImage = new System.Windows.Forms.Button();
            this.importSound = new System.Windows.Forms.Button();
            this.loadAssetType = new System.Windows.Forms.ComboBox();
            this.assetList = new System.Windows.Forms.ListBox();
            this.deleteAsset = new System.Windows.Forms.Button();
            this.refreshList = new System.Windows.Forms.Button();
            this.editSelected = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.domainUpDown1 = new System.Windows.Forms.DomainUpDown();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // importModel
            // 
            this.importModel.Location = new System.Drawing.Point(6, 19);
            this.importModel.Name = "importModel";
            this.importModel.Size = new System.Drawing.Size(150, 29);
            this.importModel.TabIndex = 0;
            this.importModel.Text = "New Model";
            this.importModel.UseVisualStyleBackColor = true;
            this.importModel.Click += new System.EventHandler(this.importModel_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.importFont);
            this.groupBox1.Controls.Add(this.importImage);
            this.groupBox1.Controls.Add(this.importSound);
            this.groupBox1.Controls.Add(this.importModel);
            this.groupBox1.Location = new System.Drawing.Point(12, 382);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(630, 54);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Import Assets";
            // 
            // importFont
            // 
            this.importFont.Location = new System.Drawing.Point(474, 19);
            this.importFont.Name = "importFont";
            this.importFont.Size = new System.Drawing.Size(150, 29);
            this.importFont.TabIndex = 3;
            this.importFont.Text = "New Font";
            this.importFont.UseVisualStyleBackColor = true;
            this.importFont.Click += new System.EventHandler(this.importFont_Click);
            // 
            // importImage
            // 
            this.importImage.Location = new System.Drawing.Point(318, 19);
            this.importImage.Name = "importImage";
            this.importImage.Size = new System.Drawing.Size(150, 29);
            this.importImage.TabIndex = 2;
            this.importImage.Text = "New Images";
            this.importImage.UseVisualStyleBackColor = true;
            this.importImage.Click += new System.EventHandler(this.importImage_Click);
            // 
            // importSound
            // 
            this.importSound.Location = new System.Drawing.Point(162, 19);
            this.importSound.Name = "importSound";
            this.importSound.Size = new System.Drawing.Size(150, 29);
            this.importSound.TabIndex = 1;
            this.importSound.Text = "New Sounds";
            this.importSound.UseVisualStyleBackColor = true;
            this.importSound.Click += new System.EventHandler(this.importSound_Click);
            // 
            // loadAssetType
            // 
            this.loadAssetType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.loadAssetType.FormattingEnabled = true;
            this.loadAssetType.Items.AddRange(new object[] {
            "Models",
            "Sounds",
            "Images",
            "Fonts"});
            this.loadAssetType.Location = new System.Drawing.Point(6, 19);
            this.loadAssetType.Name = "loadAssetType";
            this.loadAssetType.Size = new System.Drawing.Size(202, 21);
            this.loadAssetType.TabIndex = 2;
            this.loadAssetType.SelectedIndexChanged += new System.EventHandler(this.loadAssetType_SelectedIndexChanged);
            // 
            // assetList
            // 
            this.assetList.FormattingEnabled = true;
            this.assetList.Location = new System.Drawing.Point(6, 46);
            this.assetList.Name = "assetList";
            this.assetList.Size = new System.Drawing.Size(202, 277);
            this.assetList.TabIndex = 3;
            // 
            // deleteAsset
            // 
            this.deleteAsset.Location = new System.Drawing.Point(6, 329);
            this.deleteAsset.Name = "deleteAsset";
            this.deleteAsset.Size = new System.Drawing.Size(63, 23);
            this.deleteAsset.TabIndex = 4;
            this.deleteAsset.Text = "Delete";
            this.deleteAsset.UseVisualStyleBackColor = true;
            this.deleteAsset.Click += new System.EventHandler(this.deleteAsset_Click);
            // 
            // refreshList
            // 
            this.refreshList.Location = new System.Drawing.Point(145, 329);
            this.refreshList.Name = "refreshList";
            this.refreshList.Size = new System.Drawing.Size(63, 23);
            this.refreshList.TabIndex = 5;
            this.refreshList.Text = "Refresh";
            this.refreshList.UseVisualStyleBackColor = true;
            this.refreshList.Click += new System.EventHandler(this.refreshList_Click);
            // 
            // editSelected
            // 
            this.editSelected.Location = new System.Drawing.Point(75, 329);
            this.editSelected.Name = "editSelected";
            this.editSelected.Size = new System.Drawing.Size(63, 23);
            this.editSelected.TabIndex = 6;
            this.editSelected.Text = "Edit";
            this.editSelected.UseVisualStyleBackColor = true;
            this.editSelected.Click += new System.EventHandler(this.editSelected_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.loadAssetType);
            this.groupBox2.Controls.Add(this.editSelected);
            this.groupBox2.Controls.Add(this.assetList);
            this.groupBox2.Controls.Add(this.refreshList);
            this.groupBox2.Controls.Add(this.deleteAsset);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(215, 364);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Asset Browser";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(66, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Model Scale";
            // 
            // domainUpDown1
            // 
            this.domainUpDown1.Location = new System.Drawing.Point(21, 46);
            this.domainUpDown1.Name = "domainUpDown1";
            this.domainUpDown1.Size = new System.Drawing.Size(120, 20);
            this.domainUpDown1.TabIndex = 10;
            this.domainUpDown1.Text = "domainUpDown1";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.domainUpDown1);
            this.groupBox3.Location = new System.Drawing.Point(233, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(409, 364);
            this.groupBox3.TabIndex = 11;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Asset Configuration";
            // 
            // Landing
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(653, 445);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Landing";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mario Kart Asset Manager";
            this.Load += new System.EventHandler(this.Landing_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button importModel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button importImage;
        private System.Windows.Forms.Button importSound;
        private System.Windows.Forms.Button importFont;
        private System.Windows.Forms.ComboBox loadAssetType;
        private System.Windows.Forms.ListBox assetList;
        private System.Windows.Forms.Button deleteAsset;
        private System.Windows.Forms.Button refreshList;
        private System.Windows.Forms.Button editSelected;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DomainUpDown domainUpDown1;
        private System.Windows.Forms.GroupBox groupBox3;
    }
}