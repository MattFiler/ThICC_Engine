namespace EditorTool
{
    partial class Image_Importer
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
            this.label1 = new System.Windows.Forms.Label();
            this.imagePath = new System.Windows.Forms.TextBox();
            this.browseToImage = new System.Windows.Forms.Button();
            this.importModel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Image File (PNG)";
            // 
            // imagePath
            // 
            this.imagePath.Location = new System.Drawing.Point(12, 25);
            this.imagePath.Name = "imagePath";
            this.imagePath.ReadOnly = true;
            this.imagePath.Size = new System.Drawing.Size(183, 20);
            this.imagePath.TabIndex = 5;
            // 
            // browseToImage
            // 
            this.browseToImage.Location = new System.Drawing.Point(201, 24);
            this.browseToImage.Name = "browseToImage";
            this.browseToImage.Size = new System.Drawing.Size(75, 22);
            this.browseToImage.TabIndex = 4;
            this.browseToImage.Text = "Browse";
            this.browseToImage.UseVisualStyleBackColor = true;
            this.browseToImage.Click += new System.EventHandler(this.browseToImage_Click);
            // 
            // importModel
            // 
            this.importModel.Location = new System.Drawing.Point(12, 51);
            this.importModel.Name = "importModel";
            this.importModel.Size = new System.Drawing.Size(264, 28);
            this.importModel.TabIndex = 7;
            this.importModel.Text = "Import";
            this.importModel.UseVisualStyleBackColor = true;
            this.importModel.Click += new System.EventHandler(this.importModel_Click);
            // 
            // Image_Importer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(286, 89);
            this.Controls.Add(this.importModel);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.imagePath);
            this.Controls.Add(this.browseToImage);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Image_Importer";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Import Image";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox imagePath;
        private System.Windows.Forms.Button browseToImage;
        private System.Windows.Forms.Button importModel;
    }
}