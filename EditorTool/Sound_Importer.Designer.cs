namespace EditorTool
{
    partial class Sound_Importer
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
            this.importSound = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.soundPath = new System.Windows.Forms.TextBox();
            this.browseToSound = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // importSound
            // 
            this.importSound.Location = new System.Drawing.Point(13, 51);
            this.importSound.Name = "importSound";
            this.importSound.Size = new System.Drawing.Size(264, 28);
            this.importSound.TabIndex = 11;
            this.importSound.Text = "Import";
            this.importSound.UseVisualStyleBackColor = true;
            this.importSound.Click += new System.EventHandler(this.importSound_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(147, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "Sound File (WAV/MP3/OGG)";
            // 
            // soundPath
            // 
            this.soundPath.Location = new System.Drawing.Point(13, 25);
            this.soundPath.Name = "soundPath";
            this.soundPath.ReadOnly = true;
            this.soundPath.Size = new System.Drawing.Size(183, 20);
            this.soundPath.TabIndex = 9;
            // 
            // browseToSound
            // 
            this.browseToSound.Location = new System.Drawing.Point(202, 24);
            this.browseToSound.Name = "browseToSound";
            this.browseToSound.Size = new System.Drawing.Size(75, 22);
            this.browseToSound.TabIndex = 8;
            this.browseToSound.Text = "Browse";
            this.browseToSound.UseVisualStyleBackColor = true;
            this.browseToSound.Click += new System.EventHandler(this.browseToSound_Click);
            // 
            // Sound_Importer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(286, 89);
            this.Controls.Add(this.importSound);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.soundPath);
            this.Controls.Add(this.browseToSound);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Sound_Importer";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Import Sound";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button importSound;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox soundPath;
        private System.Windows.Forms.Button browseToSound;
    }
}