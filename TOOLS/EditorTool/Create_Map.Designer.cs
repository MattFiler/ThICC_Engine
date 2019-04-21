namespace EditorTool
{
    partial class Create_Map
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Create_Map));
            this.saveMap = new System.Windows.Forms.Button();
            this.mapPreviewImage = new System.Windows.Forms.TextBox();
            this.selectImageAsset = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.selectMapModel = new System.Windows.Forms.Button();
            this.mapModelAsset = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.selectSoundtrackIntro = new System.Windows.Forms.Button();
            this.soundtrackIntro = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.selectSoundtrackLoop = new System.Windows.Forms.Button();
            this.soundtrackIntroLoop = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.selectFinalLapIntro = new System.Windows.Forms.Button();
            this.finalLapIntro = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.selectFinalLapLoop = new System.Windows.Forms.Button();
            this.finalLapLoop = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.loadString = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.mapName = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.mapCodename = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // saveMap
            // 
            this.saveMap.Location = new System.Drawing.Point(11, 383);
            this.saveMap.Name = "saveMap";
            this.saveMap.Size = new System.Drawing.Size(327, 23);
            this.saveMap.TabIndex = 0;
            this.saveMap.Text = "Create";
            this.saveMap.UseVisualStyleBackColor = true;
            this.saveMap.Click += new System.EventHandler(this.saveMap_Click);
            // 
            // mapPreviewImage
            // 
            this.mapPreviewImage.Location = new System.Drawing.Point(9, 111);
            this.mapPreviewImage.Name = "mapPreviewImage";
            this.mapPreviewImage.ReadOnly = true;
            this.mapPreviewImage.Size = new System.Drawing.Size(229, 20);
            this.mapPreviewImage.TabIndex = 1;
            // 
            // selectImageAsset
            // 
            this.selectImageAsset.Location = new System.Drawing.Point(244, 110);
            this.selectImageAsset.Name = "selectImageAsset";
            this.selectImageAsset.Size = new System.Drawing.Size(75, 23);
            this.selectImageAsset.TabIndex = 2;
            this.selectImageAsset.Text = "Load";
            this.selectImageAsset.UseVisualStyleBackColor = true;
            this.selectImageAsset.Click += new System.EventHandler(this.selectImageAsset_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 95);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Preview Image";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 134);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(60, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Map Model";
            // 
            // selectMapModel
            // 
            this.selectMapModel.Location = new System.Drawing.Point(244, 149);
            this.selectMapModel.Name = "selectMapModel";
            this.selectMapModel.Size = new System.Drawing.Size(75, 23);
            this.selectMapModel.TabIndex = 6;
            this.selectMapModel.Text = "Load";
            this.selectMapModel.UseVisualStyleBackColor = true;
            this.selectMapModel.Click += new System.EventHandler(this.selectMapModel_Click);
            // 
            // mapModelAsset
            // 
            this.mapModelAsset.Location = new System.Drawing.Point(9, 150);
            this.mapModelAsset.Name = "mapModelAsset";
            this.mapModelAsset.ReadOnly = true;
            this.mapModelAsset.Size = new System.Drawing.Size(229, 20);
            this.mapModelAsset.TabIndex = 5;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 19);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(112, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Main Soundtrack Intro";
            // 
            // selectSoundtrackIntro
            // 
            this.selectSoundtrackIntro.Location = new System.Drawing.Point(244, 34);
            this.selectSoundtrackIntro.Name = "selectSoundtrackIntro";
            this.selectSoundtrackIntro.Size = new System.Drawing.Size(75, 23);
            this.selectSoundtrackIntro.TabIndex = 9;
            this.selectSoundtrackIntro.Text = "Load";
            this.selectSoundtrackIntro.UseVisualStyleBackColor = true;
            this.selectSoundtrackIntro.Click += new System.EventHandler(this.selectSoundtrackIntro_Click);
            // 
            // soundtrackIntro
            // 
            this.soundtrackIntro.Location = new System.Drawing.Point(9, 35);
            this.soundtrackIntro.Name = "soundtrackIntro";
            this.soundtrackIntro.ReadOnly = true;
            this.soundtrackIntro.Size = new System.Drawing.Size(229, 20);
            this.soundtrackIntro.TabIndex = 8;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 58);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(115, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "Main Soundtrack Loop";
            // 
            // selectSoundtrackLoop
            // 
            this.selectSoundtrackLoop.Location = new System.Drawing.Point(244, 73);
            this.selectSoundtrackLoop.Name = "selectSoundtrackLoop";
            this.selectSoundtrackLoop.Size = new System.Drawing.Size(75, 23);
            this.selectSoundtrackLoop.TabIndex = 12;
            this.selectSoundtrackLoop.Text = "Load";
            this.selectSoundtrackLoop.UseVisualStyleBackColor = true;
            this.selectSoundtrackLoop.Click += new System.EventHandler(this.selectSoundtrackLoop_Click);
            // 
            // soundtrackIntroLoop
            // 
            this.soundtrackIntroLoop.Location = new System.Drawing.Point(9, 74);
            this.soundtrackIntroLoop.Name = "soundtrackIntroLoop";
            this.soundtrackIntroLoop.ReadOnly = true;
            this.soundtrackIntroLoop.Size = new System.Drawing.Size(229, 20);
            this.soundtrackIntroLoop.TabIndex = 11;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 97);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(74, 13);
            this.label6.TabIndex = 16;
            this.label6.Text = "Final Lap Intro";
            // 
            // selectFinalLapIntro
            // 
            this.selectFinalLapIntro.Location = new System.Drawing.Point(244, 112);
            this.selectFinalLapIntro.Name = "selectFinalLapIntro";
            this.selectFinalLapIntro.Size = new System.Drawing.Size(75, 23);
            this.selectFinalLapIntro.TabIndex = 15;
            this.selectFinalLapIntro.Text = "Load";
            this.selectFinalLapIntro.UseVisualStyleBackColor = true;
            this.selectFinalLapIntro.Click += new System.EventHandler(this.selectFinalLapIntro_Click);
            // 
            // finalLapIntro
            // 
            this.finalLapIntro.Location = new System.Drawing.Point(9, 113);
            this.finalLapIntro.Name = "finalLapIntro";
            this.finalLapIntro.ReadOnly = true;
            this.finalLapIntro.Size = new System.Drawing.Size(229, 20);
            this.finalLapIntro.TabIndex = 14;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 136);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(77, 13);
            this.label7.TabIndex = 19;
            this.label7.Text = "Final Lap Loop";
            // 
            // selectFinalLapLoop
            // 
            this.selectFinalLapLoop.Location = new System.Drawing.Point(244, 151);
            this.selectFinalLapLoop.Name = "selectFinalLapLoop";
            this.selectFinalLapLoop.Size = new System.Drawing.Size(75, 23);
            this.selectFinalLapLoop.TabIndex = 18;
            this.selectFinalLapLoop.Text = "Load";
            this.selectFinalLapLoop.UseVisualStyleBackColor = true;
            this.selectFinalLapLoop.Click += new System.EventHandler(this.selectFinalLapLoop_Click);
            // 
            // finalLapLoop
            // 
            this.finalLapLoop.Location = new System.Drawing.Point(9, 152);
            this.finalLapLoop.Name = "finalLapLoop";
            this.finalLapLoop.ReadOnly = true;
            this.finalLapLoop.Size = new System.Drawing.Size(229, 20);
            this.finalLapLoop.TabIndex = 17;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.soundtrackIntro);
            this.groupBox1.Controls.Add(this.selectFinalLapLoop);
            this.groupBox1.Controls.Add(this.selectSoundtrackIntro);
            this.groupBox1.Controls.Add(this.finalLapLoop);
            this.groupBox1.Controls.Add(this.soundtrackIntroLoop);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.selectSoundtrackLoop);
            this.groupBox1.Controls.Add(this.selectFinalLapIntro);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.finalLapIntro);
            this.groupBox1.Location = new System.Drawing.Point(11, 197);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(327, 180);
            this.groupBox1.TabIndex = 20;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Map Audio Tracks";
            // 
            // groupBox2
            // 
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
            this.groupBox2.Size = new System.Drawing.Size(327, 179);
            this.groupBox2.TabIndex = 21;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Core Map Elements";
            // 
            // loadString
            // 
            this.loadString.Location = new System.Drawing.Point(244, 71);
            this.loadString.Name = "loadString";
            this.loadString.Size = new System.Drawing.Size(75, 23);
            this.loadString.TabIndex = 11;
            this.loadString.Text = "Load";
            this.loadString.UseVisualStyleBackColor = true;
            this.loadString.Click += new System.EventHandler(this.loadString_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(27, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Title";
            // 
            // mapName
            // 
            this.mapName.Location = new System.Drawing.Point(9, 72);
            this.mapName.Name = "mapName";
            this.mapName.ReadOnly = true;
            this.mapName.Size = new System.Drawing.Size(229, 20);
            this.mapName.TabIndex = 8;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 17);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(63, 13);
            this.label8.TabIndex = 23;
            this.label8.Text = "Code Name";
            // 
            // mapCodename
            // 
            this.mapCodename.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.mapCodename.Location = new System.Drawing.Point(9, 33);
            this.mapCodename.Name = "mapCodename";
            this.mapCodename.Size = new System.Drawing.Size(310, 20);
            this.mapCodename.TabIndex = 22;
            // 
            // Create_Map
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(349, 415);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.saveMap);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Create_Map";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Create New Map";
            this.Load += new System.EventHandler(this.Create_Map_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button saveMap;
        private System.Windows.Forms.TextBox mapPreviewImage;
        private System.Windows.Forms.Button selectImageAsset;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button selectMapModel;
        private System.Windows.Forms.TextBox mapModelAsset;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button selectSoundtrackIntro;
        private System.Windows.Forms.TextBox soundtrackIntro;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button selectSoundtrackLoop;
        private System.Windows.Forms.TextBox soundtrackIntroLoop;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button selectFinalLapIntro;
        private System.Windows.Forms.TextBox finalLapIntro;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button selectFinalLapLoop;
        private System.Windows.Forms.TextBox finalLapLoop;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox mapName;
        private System.Windows.Forms.Button loadString;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox mapCodename;
    }
}