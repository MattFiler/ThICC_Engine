namespace EditorTool
{
    partial class Game_Config_Preselect
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
            this.openCharacterConfig = new System.Windows.Forms.Button();
            this.openCameraConfig = new System.Windows.Forms.Button();
            this.openItemConfig = new System.Windows.Forms.Button();
            this.miscConfigs = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // openCharacterConfig
            // 
            this.openCharacterConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.openCharacterConfig.Location = new System.Drawing.Point(12, 12);
            this.openCharacterConfig.Name = "openCharacterConfig";
            this.openCharacterConfig.Size = new System.Drawing.Size(270, 31);
            this.openCharacterConfig.TabIndex = 2;
            this.openCharacterConfig.Text = "Character Controller";
            this.openCharacterConfig.UseVisualStyleBackColor = true;
            this.openCharacterConfig.Click += new System.EventHandler(this.openCharacterConfig_Click);
            // 
            // openCameraConfig
            // 
            this.openCameraConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.openCameraConfig.Location = new System.Drawing.Point(12, 49);
            this.openCameraConfig.Name = "openCameraConfig";
            this.openCameraConfig.Size = new System.Drawing.Size(270, 31);
            this.openCameraConfig.TabIndex = 3;
            this.openCameraConfig.Text = "Camera Configurations";
            this.openCameraConfig.UseVisualStyleBackColor = true;
            this.openCameraConfig.Click += new System.EventHandler(this.openCameraConfig_Click);
            // 
            // openItemConfig
            // 
            this.openItemConfig.Enabled = false;
            this.openItemConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.openItemConfig.Location = new System.Drawing.Point(12, 86);
            this.openItemConfig.Name = "openItemConfig";
            this.openItemConfig.Size = new System.Drawing.Size(270, 31);
            this.openItemConfig.TabIndex = 4;
            this.openItemConfig.Text = "Item Configurations";
            this.openItemConfig.UseVisualStyleBackColor = true;
            this.openItemConfig.Click += new System.EventHandler(this.openItemConfig_Click);
            // 
            // miscConfigs
            // 
            this.miscConfigs.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.miscConfigs.Location = new System.Drawing.Point(12, 123);
            this.miscConfigs.Name = "miscConfigs";
            this.miscConfigs.Size = new System.Drawing.Size(270, 31);
            this.miscConfigs.TabIndex = 5;
            this.miscConfigs.Text = "Core Engine Configurations";
            this.miscConfigs.UseVisualStyleBackColor = true;
            this.miscConfigs.Click += new System.EventHandler(this.miscConfigs_Click);
            // 
            // Game_Config_Preselect
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(294, 164);
            this.Controls.Add(this.miscConfigs);
            this.Controls.Add(this.openItemConfig);
            this.Controls.Add(this.openCameraConfig);
            this.Controls.Add(this.openCharacterConfig);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Game_Config_Preselect";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Engine Configurations";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button openCharacterConfig;
        private System.Windows.Forms.Button openCameraConfig;
        private System.Windows.Forms.Button openItemConfig;
        private System.Windows.Forms.Button miscConfigs;
    }
}