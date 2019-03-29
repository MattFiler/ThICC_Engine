namespace EditorTool
{
    partial class Asset_Browser
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
            this.assetList = new System.Windows.Forms.ListBox();
            this.selectAsset = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.playSoundPreview = new System.Windows.Forms.Button();
            this.modelPreview = new System.Windows.Forms.Integration.ElementHost();
            this.soundPreview = new NAudio.Gui.WaveViewer();
            this.imagePreview = new System.Windows.Forms.PictureBox();
            this.localisationPreview = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imagePreview)).BeginInit();
            this.SuspendLayout();
            // 
            // assetList
            // 
            this.assetList.FormattingEnabled = true;
            this.assetList.Location = new System.Drawing.Point(10, 177);
            this.assetList.Name = "assetList";
            this.assetList.Size = new System.Drawing.Size(299, 303);
            this.assetList.TabIndex = 5;
            this.assetList.SelectedIndexChanged += new System.EventHandler(this.assetList_SelectedIndexChanged);
            // 
            // selectAsset
            // 
            this.selectAsset.Location = new System.Drawing.Point(10, 486);
            this.selectAsset.Name = "selectAsset";
            this.selectAsset.Size = new System.Drawing.Size(299, 35);
            this.selectAsset.TabIndex = 6;
            this.selectAsset.Text = "Select";
            this.selectAsset.UseVisualStyleBackColor = true;
            this.selectAsset.Click += new System.EventHandler(this.selectAsset_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.localisationPreview);
            this.groupBox1.Controls.Add(this.playSoundPreview);
            this.groupBox1.Controls.Add(this.modelPreview);
            this.groupBox1.Controls.Add(this.soundPreview);
            this.groupBox1.Controls.Add(this.imagePreview);
            this.groupBox1.Location = new System.Drawing.Point(10, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(299, 159);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Asset Preview";
            // 
            // playSoundPreview
            // 
            this.playSoundPreview.Location = new System.Drawing.Point(239, 129);
            this.playSoundPreview.Name = "playSoundPreview";
            this.playSoundPreview.Size = new System.Drawing.Size(54, 23);
            this.playSoundPreview.TabIndex = 17;
            this.playSoundPreview.Text = "Play";
            this.playSoundPreview.UseVisualStyleBackColor = true;
            this.playSoundPreview.Visible = false;
            this.playSoundPreview.Click += new System.EventHandler(this.playSoundPreview_Click);
            // 
            // modelPreview
            // 
            this.modelPreview.Location = new System.Drawing.Point(6, 19);
            this.modelPreview.Name = "modelPreview";
            this.modelPreview.Size = new System.Drawing.Size(287, 133);
            this.modelPreview.TabIndex = 12;
            this.modelPreview.Text = "elementHost1";
            this.modelPreview.Child = null;
            // 
            // soundPreview
            // 
            this.soundPreview.Location = new System.Drawing.Point(6, 19);
            this.soundPreview.Name = "soundPreview";
            this.soundPreview.SamplesPerPixel = 128;
            this.soundPreview.Size = new System.Drawing.Size(287, 133);
            this.soundPreview.StartPosition = ((long)(0));
            this.soundPreview.TabIndex = 16;
            this.soundPreview.Visible = false;
            this.soundPreview.WaveStream = null;
            // 
            // imagePreview
            // 
            this.imagePreview.Location = new System.Drawing.Point(6, 19);
            this.imagePreview.Name = "imagePreview";
            this.imagePreview.Size = new System.Drawing.Size(287, 133);
            this.imagePreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.imagePreview.TabIndex = 15;
            this.imagePreview.TabStop = false;
            this.imagePreview.Visible = false;
            // 
            // localisationPreview
            // 
            this.localisationPreview.Location = new System.Drawing.Point(6, 19);
            this.localisationPreview.Multiline = true;
            this.localisationPreview.Name = "localisationPreview";
            this.localisationPreview.ReadOnly = true;
            this.localisationPreview.Size = new System.Drawing.Size(287, 134);
            this.localisationPreview.TabIndex = 19;
            this.localisationPreview.Visible = false;
            // 
            // Asset_Browser
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(318, 528);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.selectAsset);
            this.Controls.Add(this.assetList);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Asset_Browser";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Asset Browser";
            this.Load += new System.EventHandler(this.Asset_Browser_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imagePreview)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox assetList;
        private System.Windows.Forms.Button selectAsset;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button playSoundPreview;
        private System.Windows.Forms.Integration.ElementHost modelPreview;
        private NAudio.Gui.WaveViewer soundPreview;
        private System.Windows.Forms.PictureBox imagePreview;
        private System.Windows.Forms.TextBox localisationPreview;
    }
}