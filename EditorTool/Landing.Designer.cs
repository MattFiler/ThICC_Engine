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
            this.importSound = new System.Windows.Forms.Button();
            this.importImage = new System.Windows.Forms.Button();
            this.importFont = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // importModel
            // 
            this.importModel.Location = new System.Drawing.Point(6, 19);
            this.importModel.Name = "importModel";
            this.importModel.Size = new System.Drawing.Size(255, 29);
            this.importModel.TabIndex = 0;
            this.importModel.Text = "Import Models";
            this.importModel.UseVisualStyleBackColor = true;
            this.importModel.Click += new System.EventHandler(this.importModel_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.importFont);
            this.groupBox1.Controls.Add(this.importImage);
            this.groupBox1.Controls.Add(this.importSound);
            this.groupBox1.Controls.Add(this.importModel);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(267, 159);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Import Assets";
            // 
            // importSound
            // 
            this.importSound.Location = new System.Drawing.Point(6, 54);
            this.importSound.Name = "importSound";
            this.importSound.Size = new System.Drawing.Size(255, 29);
            this.importSound.TabIndex = 1;
            this.importSound.Text = "Import Sounds";
            this.importSound.UseVisualStyleBackColor = true;
            this.importSound.Click += new System.EventHandler(this.importSound_Click);
            // 
            // importImage
            // 
            this.importImage.Location = new System.Drawing.Point(6, 89);
            this.importImage.Name = "importImage";
            this.importImage.Size = new System.Drawing.Size(255, 29);
            this.importImage.TabIndex = 2;
            this.importImage.Text = "Import Images";
            this.importImage.UseVisualStyleBackColor = true;
            this.importImage.Click += new System.EventHandler(this.importImage_Click);
            // 
            // importFont
            // 
            this.importFont.Location = new System.Drawing.Point(6, 124);
            this.importFont.Name = "importFont";
            this.importFont.Size = new System.Drawing.Size(255, 29);
            this.importFont.TabIndex = 3;
            this.importFont.Text = "Import Font";
            this.importFont.UseVisualStyleBackColor = true;
            this.importFont.Click += new System.EventHandler(this.importFont_Click);
            // 
            // Landing
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 183);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Landing";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mario Kart Toolkit";
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button importModel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button importImage;
        private System.Windows.Forms.Button importSound;
        private System.Windows.Forms.Button importFont;
    }
}