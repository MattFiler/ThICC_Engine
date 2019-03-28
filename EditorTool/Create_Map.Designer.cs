namespace EditorTool
{
    partial class Create_Map
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
            this.button1 = new System.Windows.Forms.Button();
            this.mapPreviewImage = new System.Windows.Forms.TextBox();
            this.selectImageAsset = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(721, 496);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // mapPreviewImage
            // 
            this.mapPreviewImage.Location = new System.Drawing.Point(12, 26);
            this.mapPreviewImage.Name = "mapPreviewImage";
            this.mapPreviewImage.ReadOnly = true;
            this.mapPreviewImage.Size = new System.Drawing.Size(229, 20);
            this.mapPreviewImage.TabIndex = 1;
            // 
            // selectImageAsset
            // 
            this.selectImageAsset.Location = new System.Drawing.Point(247, 25);
            this.selectImageAsset.Name = "selectImageAsset";
            this.selectImageAsset.Size = new System.Drawing.Size(75, 23);
            this.selectImageAsset.TabIndex = 2;
            this.selectImageAsset.Text = "Load";
            this.selectImageAsset.UseVisualStyleBackColor = true;
            this.selectImageAsset.Click += new System.EventHandler(this.selectImageAsset_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Preview Image";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(732, 462);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "label2";
            // 
            // Create_Map
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(975, 699);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.selectImageAsset);
            this.Controls.Add(this.mapPreviewImage);
            this.Controls.Add(this.button1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Create_Map";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Create New Map";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox mapPreviewImage;
        private System.Windows.Forms.Button selectImageAsset;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}