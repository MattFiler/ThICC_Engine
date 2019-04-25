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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Splash));
            this.openGroupManager = new System.Windows.Forms.Button();
            this.openAssetManager = new System.Windows.Forms.Button();
            this.openUiEditor = new System.Windows.Forms.Button();
            this.openKeybindEditor = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.compileAssets = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.openEngineConfigs = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.openProject = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // openGroupManager
            // 
            this.openGroupManager.Location = new System.Drawing.Point(6, 56);
            this.openGroupManager.Name = "openGroupManager";
            this.openGroupManager.Size = new System.Drawing.Size(270, 31);
            this.openGroupManager.TabIndex = 2;
            this.openGroupManager.Text = "Asset Group Manager";
            this.openGroupManager.UseVisualStyleBackColor = true;
            this.openGroupManager.Click += new System.EventHandler(this.openMapManager_Click);
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
            this.openUiEditor.TabIndex = 4;
            this.openUiEditor.Text = "UI Editor";
            this.openUiEditor.UseVisualStyleBackColor = true;
            this.openUiEditor.Click += new System.EventHandler(this.openUiEditor_Click);
            // 
            // openKeybindEditor
            // 
            this.openKeybindEditor.Location = new System.Drawing.Point(6, 56);
            this.openKeybindEditor.Name = "openKeybindEditor";
            this.openKeybindEditor.Size = new System.Drawing.Size(270, 31);
            this.openKeybindEditor.TabIndex = 5;
            this.openKeybindEditor.Text = "Keybinds";
            this.openKeybindEditor.UseVisualStyleBackColor = true;
            this.openKeybindEditor.Click += new System.EventHandler(this.openKeybindEditor_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.openGroupManager);
            this.groupBox2.Controls.Add(this.compileAssets);
            this.groupBox2.Controls.Add(this.openAssetManager);
            this.groupBox2.Location = new System.Drawing.Point(11, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(281, 130);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Global Asset Management";
            // 
            // compileAssets
            // 
            this.compileAssets.Location = new System.Drawing.Point(6, 93);
            this.compileAssets.Name = "compileAssets";
            this.compileAssets.Size = new System.Drawing.Size(270, 31);
            this.compileAssets.TabIndex = 3;
            this.compileAssets.Text = "Compile Assets";
            this.compileAssets.UseVisualStyleBackColor = true;
            this.compileAssets.Click += new System.EventHandler(this.compileAssets_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.openEngineConfigs);
            this.groupBox3.Controls.Add(this.openUiEditor);
            this.groupBox3.Controls.Add(this.openKeybindEditor);
            this.groupBox3.Location = new System.Drawing.Point(11, 148);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(281, 130);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Additional Editors";
            // 
            // openEngineConfigs
            // 
            this.openEngineConfigs.Location = new System.Drawing.Point(6, 93);
            this.openEngineConfigs.Name = "openEngineConfigs";
            this.openEngineConfigs.Size = new System.Drawing.Size(270, 31);
            this.openEngineConfigs.TabIndex = 6;
            this.openEngineConfigs.Text = "Engine Configurations";
            this.openEngineConfigs.UseVisualStyleBackColor = true;
            this.openEngineConfigs.Click += new System.EventHandler(this.openEngineConfigs_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.openProject);
            this.groupBox5.Location = new System.Drawing.Point(11, 284);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(281, 56);
            this.groupBox5.TabIndex = 0;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Code Editors";
            // 
            // openProject
            // 
            this.openProject.Location = new System.Drawing.Point(6, 19);
            this.openProject.Name = "openProject";
            this.openProject.Size = new System.Drawing.Size(270, 31);
            this.openProject.TabIndex = 0;
            this.openProject.Text = "Open Project";
            this.openProject.UseVisualStyleBackColor = true;
            this.openProject.Click += new System.EventHandler(this.openProject_Click);
            // 
            // Splash
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(301, 350);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Splash";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ThICC Toolkit";
            this.Load += new System.EventHandler(this.Splash_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button openGroupManager;
        private System.Windows.Forms.Button openAssetManager;
        private System.Windows.Forms.Button openUiEditor;
        private System.Windows.Forms.Button openKeybindEditor;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button compileAssets;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button openProject;
        private System.Windows.Forms.Button openEngineConfigs;
    }
}