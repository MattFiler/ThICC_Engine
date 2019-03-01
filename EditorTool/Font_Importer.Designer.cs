namespace EditorTool
{
    partial class Font_Importer
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
            this.installedFonts = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.importFont = new System.Windows.Forms.Button();
            this.fontSize = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.fontSize)).BeginInit();
            this.SuspendLayout();
            // 
            // installedFonts
            // 
            this.installedFonts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.installedFonts.FormattingEnabled = true;
            this.installedFonts.Location = new System.Drawing.Point(15, 25);
            this.installedFonts.Name = "installedFonts";
            this.installedFonts.Size = new System.Drawing.Size(264, 21);
            this.installedFonts.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Font Name";
            // 
            // importFont
            // 
            this.importFont.Location = new System.Drawing.Point(15, 91);
            this.importFont.Name = "importFont";
            this.importFont.Size = new System.Drawing.Size(264, 28);
            this.importFont.TabIndex = 13;
            this.importFont.Text = "Import";
            this.importFont.UseVisualStyleBackColor = true;
            this.importFont.Click += new System.EventHandler(this.importFont_Click);
            // 
            // fontSize
            // 
            this.fontSize.Location = new System.Drawing.Point(15, 65);
            this.fontSize.Name = "fontSize";
            this.fontSize.Size = new System.Drawing.Size(264, 20);
            this.fontSize.TabIndex = 16;
            this.fontSize.Value = new decimal(new int[] {
            25,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 49);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(88, 13);
            this.label4.TabIndex = 17;
            this.label4.Text = "Default Font Size";
            // 
            // Font_Importer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(293, 132);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.fontSize);
            this.Controls.Add(this.importFont);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.installedFonts);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Font_Importer";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Import Font";
            this.Load += new System.EventHandler(this.Font_Importer_Load);
            ((System.ComponentModel.ISupportInitialize)(this.fontSize)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox installedFonts;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button importFont;
        private System.Windows.Forms.NumericUpDown fontSize;
        private System.Windows.Forms.Label label4;
    }
}