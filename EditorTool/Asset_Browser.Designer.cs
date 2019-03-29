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
            this.SuspendLayout();
            // 
            // assetList
            // 
            this.assetList.FormattingEnabled = true;
            this.assetList.Location = new System.Drawing.Point(12, 12);
            this.assetList.Name = "assetList";
            this.assetList.Size = new System.Drawing.Size(299, 303);
            this.assetList.TabIndex = 5;
            // 
            // selectAsset
            // 
            this.selectAsset.Location = new System.Drawing.Point(12, 321);
            this.selectAsset.Name = "selectAsset";
            this.selectAsset.Size = new System.Drawing.Size(299, 35);
            this.selectAsset.TabIndex = 6;
            this.selectAsset.Text = "Select";
            this.selectAsset.UseVisualStyleBackColor = true;
            this.selectAsset.Click += new System.EventHandler(this.selectAsset_Click);
            // 
            // Asset_Browser
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(321, 363);
            this.Controls.Add(this.selectAsset);
            this.Controls.Add(this.assetList);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Asset_Browser";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Asset Browser";
            this.Load += new System.EventHandler(this.Asset_Browser_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox assetList;
        private System.Windows.Forms.Button selectAsset;
    }
}