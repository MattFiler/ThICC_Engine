namespace Launcher
{
    partial class Form1
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
            this.resolutionSelector = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.launcherImage = new System.Windows.Forms.PictureBox();
            this.label2 = new System.Windows.Forms.Label();
            this.po_language = new System.Windows.Forms.ComboBox();
            this.playButton = new System.Windows.Forms.Button();
            this.quitButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.launcherImage)).BeginInit();
            this.SuspendLayout();
            // 
            // resolutionSelector
            // 
            this.resolutionSelector.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.resolutionSelector.Enabled = false;
            this.resolutionSelector.FormattingEnabled = true;
            this.resolutionSelector.Items.AddRange(new object[] {
            "2560x1440",
            "1920x1080",
            "1280x720",
            "1024x576",
            "768x432"});
            this.resolutionSelector.Location = new System.Drawing.Point(12, 183);
            this.resolutionSelector.Name = "resolutionSelector";
            this.resolutionSelector.Size = new System.Drawing.Size(154, 21);
            this.resolutionSelector.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(172, 186);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Screen resolution";
            // 
            // launcherImage
            // 
            this.launcherImage.Location = new System.Drawing.Point(12, 12);
            this.launcherImage.Name = "launcherImage";
            this.launcherImage.Size = new System.Drawing.Size(432, 163);
            this.launcherImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.launcherImage.TabIndex = 2;
            this.launcherImage.TabStop = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(172, 213);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(55, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Language";
            // 
            // po_language
            // 
            this.po_language.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.po_language.Enabled = false;
            this.po_language.FormattingEnabled = true;
            this.po_language.Items.AddRange(new object[] {
            "English",
            "French",
            "Spanish"});
            this.po_language.Location = new System.Drawing.Point(12, 210);
            this.po_language.Name = "po_language";
            this.po_language.Size = new System.Drawing.Size(154, 21);
            this.po_language.TabIndex = 2;
            // 
            // playButton
            // 
            this.playButton.Location = new System.Drawing.Point(322, 183);
            this.playButton.Name = "playButton";
            this.playButton.Size = new System.Drawing.Size(122, 23);
            this.playButton.TabIndex = 5;
            this.playButton.Text = "Play!";
            this.playButton.UseVisualStyleBackColor = true;
            this.playButton.Click += new System.EventHandler(this.playButton_Click);
            // 
            // quitButton
            // 
            this.quitButton.Location = new System.Drawing.Point(322, 210);
            this.quitButton.Name = "quitButton";
            this.quitButton.Size = new System.Drawing.Size(122, 23);
            this.quitButton.TabIndex = 6;
            this.quitButton.Text = "Quit";
            this.quitButton.UseVisualStyleBackColor = true;
            this.quitButton.Click += new System.EventHandler(this.quitButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(453, 239);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.quitButton);
            this.Controls.Add(this.po_language);
            this.Controls.Add(this.playButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.resolutionSelector);
            this.Controls.Add(this.launcherImage);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mario Kart Launcher";
            this.Load += new System.EventHandler(this.Launcher_Load);
            ((System.ComponentModel.ISupportInitialize)(this.launcherImage)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox resolutionSelector;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox launcherImage;
        private System.Windows.Forms.Button playButton;
        private System.Windows.Forms.Button quitButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox po_language;
    }
}

