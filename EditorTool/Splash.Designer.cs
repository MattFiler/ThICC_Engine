namespace EditorTool
{
    partial class Splash
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
            this.openMapManager = new System.Windows.Forms.Button();
            this.openAssetManager = new System.Windows.Forms.Button();
            this.openUiEditor = new System.Windows.Forms.Button();
            this.openKeybindEditor = new System.Windows.Forms.Button();
            this.openLocalisationEditor = new System.Windows.Forms.Button();
            this.openItemManager = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.compileAssets = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label14 = new System.Windows.Forms.Label();
            this.DEBUG_PLAYERCOUNT = new System.Windows.Forms.NumericUpDown();
            this.DEBUG_SAVE = new System.Windows.Forms.Button();
            this.label15 = new System.Windows.Forms.Label();
            this.DEBUG_DEFAULTTRACK = new System.Windows.Forms.ComboBox();
            this.debug_button = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DEBUG_PLAYERCOUNT)).BeginInit();
            this.SuspendLayout();
            // 
            // openMapManager
            // 
            this.openMapManager.Location = new System.Drawing.Point(6, 19);
            this.openMapManager.Name = "openMapManager";
            this.openMapManager.Size = new System.Drawing.Size(270, 31);
            this.openMapManager.TabIndex = 0;
            this.openMapManager.Text = "Map Manager";
            this.openMapManager.UseVisualStyleBackColor = true;
            this.openMapManager.Click += new System.EventHandler(this.openMapManager_Click);
            // 
            // openAssetManager
            // 
            this.openAssetManager.Location = new System.Drawing.Point(6, 19);
            this.openAssetManager.Name = "openAssetManager";
            this.openAssetManager.Size = new System.Drawing.Size(270, 31);
            this.openAssetManager.TabIndex = 1;
            this.openAssetManager.Text = "Asset Manager";
            this.openAssetManager.UseVisualStyleBackColor = true;
            this.openAssetManager.Click += new System.EventHandler(this.openAssetManager_Click);
            // 
            // openUiEditor
            // 
            this.openUiEditor.Location = new System.Drawing.Point(6, 19);
            this.openUiEditor.Name = "openUiEditor";
            this.openUiEditor.Size = new System.Drawing.Size(270, 31);
            this.openUiEditor.TabIndex = 2;
            this.openUiEditor.Text = "UI Editor";
            this.openUiEditor.UseVisualStyleBackColor = true;
            this.openUiEditor.Click += new System.EventHandler(this.openUiEditor_Click);
            // 
            // openKeybindEditor
            // 
            this.openKeybindEditor.Location = new System.Drawing.Point(6, 56);
            this.openKeybindEditor.Name = "openKeybindEditor";
            this.openKeybindEditor.Size = new System.Drawing.Size(270, 31);
            this.openKeybindEditor.TabIndex = 3;
            this.openKeybindEditor.Text = "Keybinds";
            this.openKeybindEditor.UseVisualStyleBackColor = true;
            this.openKeybindEditor.Click += new System.EventHandler(this.openKeybindEditor_Click);
            // 
            // openLocalisationEditor
            // 
            this.openLocalisationEditor.Enabled = false;
            this.openLocalisationEditor.Location = new System.Drawing.Point(6, 93);
            this.openLocalisationEditor.Name = "openLocalisationEditor";
            this.openLocalisationEditor.Size = new System.Drawing.Size(270, 31);
            this.openLocalisationEditor.TabIndex = 4;
            this.openLocalisationEditor.Text = "Localisation";
            this.openLocalisationEditor.UseVisualStyleBackColor = true;
            // 
            // openItemManager
            // 
            this.openItemManager.Enabled = false;
            this.openItemManager.Location = new System.Drawing.Point(6, 56);
            this.openItemManager.Name = "openItemManager";
            this.openItemManager.Size = new System.Drawing.Size(270, 31);
            this.openItemManager.TabIndex = 5;
            this.openItemManager.Text = "In-Game Item Manager";
            this.openItemManager.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.openMapManager);
            this.groupBox1.Controls.Add(this.openItemManager);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(281, 93);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Grouped Asset Configurations";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.compileAssets);
            this.groupBox2.Controls.Add(this.openAssetManager);
            this.groupBox2.Location = new System.Drawing.Point(12, 111);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(281, 92);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Global Asset Management";
            // 
            // compileAssets
            // 
            this.compileAssets.Location = new System.Drawing.Point(6, 56);
            this.compileAssets.Name = "compileAssets";
            this.compileAssets.Size = new System.Drawing.Size(270, 31);
            this.compileAssets.TabIndex = 2;
            this.compileAssets.Text = "Compile Assets";
            this.compileAssets.UseVisualStyleBackColor = true;
            this.compileAssets.Click += new System.EventHandler(this.compileAssets_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.openUiEditor);
            this.groupBox3.Controls.Add(this.openKeybindEditor);
            this.groupBox3.Controls.Add(this.openLocalisationEditor);
            this.groupBox3.Location = new System.Drawing.Point(12, 209);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(281, 129);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Additional Game Editors";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label14);
            this.groupBox4.Controls.Add(this.DEBUG_PLAYERCOUNT);
            this.groupBox4.Controls.Add(this.DEBUG_SAVE);
            this.groupBox4.Controls.Add(this.label15);
            this.groupBox4.Controls.Add(this.DEBUG_DEFAULTTRACK);
            this.groupBox4.Location = new System.Drawing.Point(12, 344);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(281, 105);
            this.groupBox4.TabIndex = 35;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Debug Configurations";
            this.groupBox4.Enter += new System.EventHandler(this.groupBox4_Enter);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(4, 19);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(72, 13);
            this.label14.TabIndex = 15;
            this.label14.Text = "Default Track";
            // 
            // DEBUG_PLAYERCOUNT
            // 
            this.DEBUG_PLAYERCOUNT.Location = new System.Drawing.Point(7, 79);
            this.DEBUG_PLAYERCOUNT.Maximum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.DEBUG_PLAYERCOUNT.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.DEBUG_PLAYERCOUNT.Name = "DEBUG_PLAYERCOUNT";
            this.DEBUG_PLAYERCOUNT.Size = new System.Drawing.Size(101, 20);
            this.DEBUG_PLAYERCOUNT.TabIndex = 32;
            this.DEBUG_PLAYERCOUNT.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // DEBUG_SAVE
            // 
            this.DEBUG_SAVE.Location = new System.Drawing.Point(156, 62);
            this.DEBUG_SAVE.Name = "DEBUG_SAVE";
            this.DEBUG_SAVE.Size = new System.Drawing.Size(120, 37);
            this.DEBUG_SAVE.TabIndex = 33;
            this.DEBUG_SAVE.Text = "Save";
            this.DEBUG_SAVE.UseVisualStyleBackColor = true;
            this.DEBUG_SAVE.Click += new System.EventHandler(this.DEBUG_SAVE_Click);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(4, 62);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(67, 13);
            this.label15.TabIndex = 17;
            this.label15.Text = "Player Count";
            // 
            // DEBUG_DEFAULTTRACK
            // 
            this.DEBUG_DEFAULTTRACK.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.DEBUG_DEFAULTTRACK.FormattingEnabled = true;
            this.DEBUG_DEFAULTTRACK.Items.AddRange(new object[] {
            "Models",
            "Meshes",
            "Sounds",
            "Images",
            "Fonts"});
            this.DEBUG_DEFAULTTRACK.Location = new System.Drawing.Point(7, 35);
            this.DEBUG_DEFAULTTRACK.Name = "DEBUG_DEFAULTTRACK";
            this.DEBUG_DEFAULTTRACK.Size = new System.Drawing.Size(268, 21);
            this.DEBUG_DEFAULTTRACK.TabIndex = 16;
            // 
            // debug_button
            // 
            this.debug_button.Location = new System.Drawing.Point(19, 463);
            this.debug_button.Name = "debug_button";
            this.debug_button.Size = new System.Drawing.Size(120, 37);
            this.debug_button.TabIndex = 34;
            this.debug_button.Text = "debug lol";
            this.debug_button.UseVisualStyleBackColor = true;
            this.debug_button.Click += new System.EventHandler(this.debug_button_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(145, 463);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(120, 37);
            this.button1.TabIndex = 36;
            this.button1.Text = "debug lol 2";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Splash
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(304, 512);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.debug_button);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Splash";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mario Kart Toolkit";
            this.Load += new System.EventHandler(this.Splash_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DEBUG_PLAYERCOUNT)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button openMapManager;
        private System.Windows.Forms.Button openAssetManager;
        private System.Windows.Forms.Button openUiEditor;
        private System.Windows.Forms.Button openKeybindEditor;
        private System.Windows.Forms.Button openLocalisationEditor;
        private System.Windows.Forms.Button openItemManager;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button compileAssets;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.ComboBox DEBUG_DEFAULTTRACK;
        private System.Windows.Forms.NumericUpDown DEBUG_PLAYERCOUNT;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Button DEBUG_SAVE;
        private System.Windows.Forms.Button debug_button;
        private System.Windows.Forms.Button button1;
    }
}