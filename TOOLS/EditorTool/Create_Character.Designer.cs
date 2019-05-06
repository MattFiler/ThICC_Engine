namespace EditorTool
{
    partial class Create_Character
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
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.loadSound = new System.Windows.Forms.Button();
            this.characterSound = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.loadString = new System.Windows.Forms.Button();
            this.mapCodename = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.mapName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.mapPreviewImage = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.selectImageAsset = new System.Windows.Forms.Button();
            this.selectMapModel = new System.Windows.Forms.Button();
            this.mapModelAsset = new System.Windows.Forms.TextBox();
            this.saveMap = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.loadSound);
            this.groupBox2.Controls.Add(this.characterSound);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.loadString);
            this.groupBox2.Controls.Add(this.mapCodename);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.mapName);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.mapPreviewImage);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.selectImageAsset);
            this.groupBox2.Controls.Add(this.selectMapModel);
            this.groupBox2.Controls.Add(this.mapModelAsset);
            this.groupBox2.Location = new System.Drawing.Point(11, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(327, 224);
            this.groupBox2.TabIndex = 22;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Character Elements";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 176);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(38, 13);
            this.label4.TabIndex = 26;
            this.label4.Text = "Sound";
            // 
            // loadSound
            // 
            this.loadSound.Location = new System.Drawing.Point(244, 191);
            this.loadSound.Name = "loadSound";
            this.loadSound.Size = new System.Drawing.Size(75, 23);
            this.loadSound.TabIndex = 25;
            this.loadSound.Text = "Load";
            this.loadSound.UseVisualStyleBackColor = true;
            this.loadSound.Click += new System.EventHandler(this.loadSound_Click);
            // 
            // characterSound
            // 
            this.characterSound.Location = new System.Drawing.Point(9, 192);
            this.characterSound.Name = "characterSound";
            this.characterSound.ReadOnly = true;
            this.characterSound.Size = new System.Drawing.Size(229, 20);
            this.characterSound.TabIndex = 24;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 20);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(63, 13);
            this.label8.TabIndex = 23;
            this.label8.Text = "Code Name";
            // 
            // loadString
            // 
            this.loadString.Location = new System.Drawing.Point(244, 74);
            this.loadString.Name = "loadString";
            this.loadString.Size = new System.Drawing.Size(75, 23);
            this.loadString.TabIndex = 11;
            this.loadString.Text = "Load";
            this.loadString.UseVisualStyleBackColor = true;
            this.loadString.Click += new System.EventHandler(this.loadString_Click);
            // 
            // mapCodename
            // 
            this.mapCodename.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.mapCodename.Location = new System.Drawing.Point(9, 36);
            this.mapCodename.Name = "mapCodename";
            this.mapCodename.Size = new System.Drawing.Size(310, 20);
            this.mapCodename.TabIndex = 22;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 59);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(78, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "In-Game Name";
            // 
            // mapName
            // 
            this.mapName.Location = new System.Drawing.Point(9, 75);
            this.mapName.Name = "mapName";
            this.mapName.ReadOnly = true;
            this.mapName.Size = new System.Drawing.Size(229, 20);
            this.mapName.TabIndex = 8;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 98);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Preview Image";
            // 
            // mapPreviewImage
            // 
            this.mapPreviewImage.Location = new System.Drawing.Point(9, 114);
            this.mapPreviewImage.Name = "mapPreviewImage";
            this.mapPreviewImage.ReadOnly = true;
            this.mapPreviewImage.Size = new System.Drawing.Size(229, 20);
            this.mapPreviewImage.TabIndex = 1;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 137);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(36, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Model";
            // 
            // selectImageAsset
            // 
            this.selectImageAsset.Location = new System.Drawing.Point(244, 113);
            this.selectImageAsset.Name = "selectImageAsset";
            this.selectImageAsset.Size = new System.Drawing.Size(75, 23);
            this.selectImageAsset.TabIndex = 2;
            this.selectImageAsset.Text = "Load";
            this.selectImageAsset.UseVisualStyleBackColor = true;
            this.selectImageAsset.Click += new System.EventHandler(this.selectImageAsset_Click);
            // 
            // selectMapModel
            // 
            this.selectMapModel.Location = new System.Drawing.Point(244, 152);
            this.selectMapModel.Name = "selectMapModel";
            this.selectMapModel.Size = new System.Drawing.Size(75, 23);
            this.selectMapModel.TabIndex = 6;
            this.selectMapModel.Text = "Load";
            this.selectMapModel.UseVisualStyleBackColor = true;
            this.selectMapModel.Click += new System.EventHandler(this.selectMapModel_Click);
            // 
            // mapModelAsset
            // 
            this.mapModelAsset.Location = new System.Drawing.Point(9, 153);
            this.mapModelAsset.Name = "mapModelAsset";
            this.mapModelAsset.ReadOnly = true;
            this.mapModelAsset.Size = new System.Drawing.Size(229, 20);
            this.mapModelAsset.TabIndex = 5;
            // 
            // saveMap
            // 
            this.saveMap.Location = new System.Drawing.Point(11, 242);
            this.saveMap.Name = "saveMap";
            this.saveMap.Size = new System.Drawing.Size(327, 23);
            this.saveMap.TabIndex = 23;
            this.saveMap.Text = "Create";
            this.saveMap.UseVisualStyleBackColor = true;
            this.saveMap.Click += new System.EventHandler(this.saveMap_Click);
            // 
            // Create_Character
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(349, 277);
            this.Controls.Add(this.saveMap);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Create_Character";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Create New Character";
            this.Load += new System.EventHandler(this.Create_Character_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button loadString;
        private System.Windows.Forms.TextBox mapCodename;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox mapName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox mapPreviewImage;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button selectImageAsset;
        private System.Windows.Forms.Button selectMapModel;
        private System.Windows.Forms.TextBox mapModelAsset;
        private System.Windows.Forms.Button saveMap;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button loadSound;
        private System.Windows.Forms.TextBox characterSound;
    }
}