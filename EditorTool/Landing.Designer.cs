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
            this.loadAssetType = new System.Windows.Forms.ComboBox();
            this.assetList = new System.Windows.Forms.ListBox();
            this.deleteAsset = new System.Windows.Forms.Button();
            this.importAsset = new System.Windows.Forms.Button();
            this.editSelected = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.domainUpDown1 = new System.Windows.Forms.DomainUpDown();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.modelPreview = new System.Windows.Forms.Integration.ElementHost();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.imagePreview = new System.Windows.Forms.PictureBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.openUiEditor = new System.Windows.Forms.Button();
            this.compileAssets = new System.Windows.Forms.Button();
            this.soundPreview = new NAudio.Gui.WaveViewer();
            this.playSoundPreview = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imagePreview)).BeginInit();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
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
            this.assetList.Size = new System.Drawing.Size(202, 264);
            this.assetList.TabIndex = 3;
            this.assetList.SelectedIndexChanged += new System.EventHandler(this.assetList_SelectedIndexChanged);
            // 
            // deleteAsset
            // 
            this.deleteAsset.Location = new System.Drawing.Point(145, 316);
            this.deleteAsset.Name = "deleteAsset";
            this.deleteAsset.Size = new System.Drawing.Size(63, 23);
            this.deleteAsset.TabIndex = 4;
            this.deleteAsset.Text = "Delete";
            this.deleteAsset.UseVisualStyleBackColor = true;
            this.deleteAsset.Click += new System.EventHandler(this.deleteAsset_Click);
            // 
            // importAsset
            // 
            this.importAsset.Location = new System.Drawing.Point(6, 316);
            this.importAsset.Name = "importAsset";
            this.importAsset.Size = new System.Drawing.Size(63, 23);
            this.importAsset.TabIndex = 5;
            this.importAsset.Text = "New";
            this.importAsset.UseVisualStyleBackColor = true;
            this.importAsset.Click += new System.EventHandler(this.refreshList_Click);
            // 
            // editSelected
            // 
            this.editSelected.Location = new System.Drawing.Point(76, 316);
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
            this.groupBox2.Controls.Add(this.deleteAsset);
            this.groupBox2.Controls.Add(this.assetList);
            this.groupBox2.Controls.Add(this.importAsset);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(215, 347);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Asset Browser";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(6, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Model Scale";
            // 
            // domainUpDown1
            // 
            this.domainUpDown1.Location = new System.Drawing.Point(10, 37);
            this.domainUpDown1.Name = "domainUpDown1";
            this.domainUpDown1.Size = new System.Drawing.Size(120, 20);
            this.domainUpDown1.TabIndex = 10;
            this.domainUpDown1.Text = "domainUpDown1";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.button1);
            this.groupBox3.Controls.Add(this.textBox4);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.textBox5);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.textBox6);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.textBox3);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.textBox2);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.textBox1);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.domainUpDown1);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Location = new System.Drawing.Point(233, 177);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(226, 182);
            this.groupBox3.TabIndex = 11;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Asset Configuration";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(144, 151);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 25;
            this.button1.Text = "Save";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(167, 123);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(52, 20);
            this.textBox4.TabIndex = 22;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(152, 126);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(17, 13);
            this.label6.TabIndex = 23;
            this.label6.Text = "Z:";
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(94, 123);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(52, 20);
            this.textBox5.TabIndex = 20;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(78, 126);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(17, 13);
            this.label7.TabIndex = 21;
            this.label7.Text = "Y:";
            // 
            // textBox6
            // 
            this.textBox6.Location = new System.Drawing.Point(20, 123);
            this.textBox6.Name = "textBox6";
            this.textBox6.Size = new System.Drawing.Size(52, 20);
            this.textBox6.TabIndex = 19;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(6, 107);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(55, 13);
            this.label8.TabIndex = 18;
            this.label8.Text = "Rotation";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(167, 80);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(52, 20);
            this.textBox3.TabIndex = 16;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(152, 83);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(17, 13);
            this.label5.TabIndex = 17;
            this.label5.Text = "Z:";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(94, 80);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(52, 20);
            this.textBox2.TabIndex = 14;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(79, 83);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(17, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Y:";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(21, 80);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(52, 20);
            this.textBox1.TabIndex = 12;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 83);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(17, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "X:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(6, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "World Position";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 126);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(17, 13);
            this.label9.TabIndex = 24;
            this.label9.Text = "X:";
            // 
            // modelPreview
            // 
            this.modelPreview.Location = new System.Drawing.Point(6, 19);
            this.modelPreview.Name = "modelPreview";
            this.modelPreview.Size = new System.Drawing.Size(214, 133);
            this.modelPreview.TabIndex = 12;
            this.modelPreview.Text = "elementHost1";
            this.modelPreview.Child = null;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.playSoundPreview);
            this.groupBox1.Controls.Add(this.imagePreview);
            this.groupBox1.Controls.Add(this.modelPreview);
            this.groupBox1.Controls.Add(this.soundPreview);
            this.groupBox1.Location = new System.Drawing.Point(233, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(226, 159);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Asset Preview";
            // 
            // imagePreview
            // 
            this.imagePreview.Location = new System.Drawing.Point(6, 19);
            this.imagePreview.Name = "imagePreview";
            this.imagePreview.Size = new System.Drawing.Size(215, 133);
            this.imagePreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.imagePreview.TabIndex = 15;
            this.imagePreview.TabStop = false;
            this.imagePreview.Visible = false;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.button4);
            this.groupBox4.Controls.Add(this.openUiEditor);
            this.groupBox4.Controls.Add(this.compileAssets);
            this.groupBox4.Location = new System.Drawing.Point(12, 365);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(448, 55);
            this.groupBox4.TabIndex = 14;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Other Tools";
            // 
            // button4
            // 
            this.button4.Enabled = false;
            this.button4.Location = new System.Drawing.Point(305, 19);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(137, 30);
            this.button4.TabIndex = 2;
            this.button4.Text = "Game Config Editor";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // openUiEditor
            // 
            this.openUiEditor.Enabled = false;
            this.openUiEditor.Location = new System.Drawing.Point(156, 19);
            this.openUiEditor.Name = "openUiEditor";
            this.openUiEditor.Size = new System.Drawing.Size(137, 30);
            this.openUiEditor.TabIndex = 1;
            this.openUiEditor.Text = "UI Editor";
            this.openUiEditor.UseVisualStyleBackColor = true;
            // 
            // compileAssets
            // 
            this.compileAssets.Location = new System.Drawing.Point(6, 19);
            this.compileAssets.Name = "compileAssets";
            this.compileAssets.Size = new System.Drawing.Size(137, 30);
            this.compileAssets.TabIndex = 0;
            this.compileAssets.Text = "Compile Assets";
            this.compileAssets.UseVisualStyleBackColor = true;
            this.compileAssets.Click += new System.EventHandler(this.compileAssets_Click);
            // 
            // soundPreview
            // 
            this.soundPreview.Location = new System.Drawing.Point(6, 19);
            this.soundPreview.Name = "soundPreview";
            this.soundPreview.SamplesPerPixel = 128;
            this.soundPreview.Size = new System.Drawing.Size(215, 133);
            this.soundPreview.StartPosition = ((long)(0));
            this.soundPreview.TabIndex = 16;
            this.soundPreview.Visible = false;
            this.soundPreview.WaveStream = null;
            // 
            // playSoundPreview
            // 
            this.playSoundPreview.Location = new System.Drawing.Point(167, 129);
            this.playSoundPreview.Name = "playSoundPreview";
            this.playSoundPreview.Size = new System.Drawing.Size(54, 23);
            this.playSoundPreview.TabIndex = 17;
            this.playSoundPreview.Text = "Play";
            this.playSoundPreview.UseVisualStyleBackColor = true;
            this.playSoundPreview.Visible = false;
            this.playSoundPreview.Click += new System.EventHandler(this.playSoundPreview_Click);
            // 
            // Landing
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(472, 428);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Landing";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mario Kart Asset Manager";
            this.Load += new System.EventHandler(this.Landing_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.imagePreview)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ComboBox loadAssetType;
        private System.Windows.Forms.ListBox assetList;
        private System.Windows.Forms.Button deleteAsset;
        private System.Windows.Forms.Button importAsset;
        private System.Windows.Forms.Button editSelected;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DomainUpDown domainUpDown1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Integration.ElementHost modelPreview;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button openUiEditor;
        private System.Windows.Forms.Button compileAssets;
        private System.Windows.Forms.PictureBox imagePreview;
        private NAudio.Gui.WaveViewer soundPreview;
        private System.Windows.Forms.Button playSoundPreview;
    }
}