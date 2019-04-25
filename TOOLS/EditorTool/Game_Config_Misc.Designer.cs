namespace EditorTool
{
    partial class Game_Config_Misc
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
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.playerCount = new System.Windows.Forms.NumericUpDown();
            this.saveConfig = new System.Windows.Forms.Button();
            this.label15 = new System.Windows.Forms.Label();
            this.engineFont = new System.Windows.Forms.ComboBox();
            this.text2 = new System.Windows.Forms.Label();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.playerCount)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.text2);
            this.groupBox4.Controls.Add(this.engineFont);
            this.groupBox4.Controls.Add(this.playerCount);
            this.groupBox4.Controls.Add(this.label15);
            this.groupBox4.Location = new System.Drawing.Point(12, 12);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(281, 118);
            this.groupBox4.TabIndex = 4;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Core Engine Configs";
            // 
            // playerCount
            // 
            this.playerCount.Location = new System.Drawing.Point(6, 39);
            this.playerCount.Maximum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.playerCount.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.playerCount.Name = "playerCount";
            this.playerCount.Size = new System.Drawing.Size(101, 20);
            this.playerCount.TabIndex = 6;
            this.playerCount.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // saveConfig
            // 
            this.saveConfig.Location = new System.Drawing.Point(12, 136);
            this.saveConfig.Name = "saveConfig";
            this.saveConfig.Size = new System.Drawing.Size(281, 23);
            this.saveConfig.TabIndex = 7;
            this.saveConfig.Text = "Save";
            this.saveConfig.UseVisualStyleBackColor = true;
            this.saveConfig.Click += new System.EventHandler(this.saveConfig_Click);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(3, 22);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(90, 13);
            this.label15.TabIndex = 17;
            this.label15.Text = "Max Player Count";
            // 
            // engineFont
            // 
            this.engineFont.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.engineFont.FormattingEnabled = true;
            this.engineFont.Location = new System.Drawing.Point(6, 81);
            this.engineFont.Name = "engineFont";
            this.engineFont.Size = new System.Drawing.Size(269, 21);
            this.engineFont.TabIndex = 18;
            // 
            // text2
            // 
            this.text2.AutoSize = true;
            this.text2.Location = new System.Drawing.Point(6, 65);
            this.text2.Name = "text2";
            this.text2.Size = new System.Drawing.Size(28, 13);
            this.text2.TabIndex = 19;
            this.text2.Text = "Font";
            // 
            // Game_Config_Misc
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(306, 170);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.saveConfig);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Game_Config_Misc";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Core Engine Configs";
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.playerCount)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.NumericUpDown playerCount;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Button saveConfig;
        private System.Windows.Forms.Label text2;
        private System.Windows.Forms.ComboBox engineFont;
    }
}