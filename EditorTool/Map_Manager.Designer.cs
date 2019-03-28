namespace EditorTool
{
    partial class Map_Manager
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
            this.newMap = new System.Windows.Forms.Button();
            this.deleteMap = new System.Windows.Forms.Button();
            this.editMap = new System.Windows.Forms.Button();
            this.previewImage = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.previewImage)).BeginInit();
            this.SuspendLayout();
            // 
            // assetList
            // 
            this.assetList.FormattingEnabled = true;
            this.assetList.Location = new System.Drawing.Point(12, 12);
            this.assetList.Name = "assetList";
            this.assetList.Size = new System.Drawing.Size(202, 199);
            this.assetList.TabIndex = 4;
            this.assetList.SelectedIndexChanged += new System.EventHandler(this.assetList_SelectedIndexChanged);
            // 
            // newMap
            // 
            this.newMap.Location = new System.Drawing.Point(220, 125);
            this.newMap.Name = "newMap";
            this.newMap.Size = new System.Drawing.Size(202, 25);
            this.newMap.TabIndex = 5;
            this.newMap.Text = "Create New Map";
            this.newMap.UseVisualStyleBackColor = true;
            this.newMap.Click += new System.EventHandler(this.newMap_Click);
            // 
            // deleteMap
            // 
            this.deleteMap.Location = new System.Drawing.Point(220, 187);
            this.deleteMap.Name = "deleteMap";
            this.deleteMap.Size = new System.Drawing.Size(202, 25);
            this.deleteMap.TabIndex = 6;
            this.deleteMap.Text = "Delete Selected Map";
            this.deleteMap.UseVisualStyleBackColor = true;
            // 
            // editMap
            // 
            this.editMap.Location = new System.Drawing.Point(220, 156);
            this.editMap.Name = "editMap";
            this.editMap.Size = new System.Drawing.Size(202, 25);
            this.editMap.TabIndex = 7;
            this.editMap.Text = "Edit Selected Map";
            this.editMap.UseVisualStyleBackColor = true;
            // 
            // previewImage
            // 
            this.previewImage.Location = new System.Drawing.Point(220, 12);
            this.previewImage.Name = "previewImage";
            this.previewImage.Size = new System.Drawing.Size(202, 108);
            this.previewImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.previewImage.TabIndex = 8;
            this.previewImage.TabStop = false;
            // 
            // Map_Manager
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(432, 221);
            this.Controls.Add(this.previewImage);
            this.Controls.Add(this.editMap);
            this.Controls.Add(this.deleteMap);
            this.Controls.Add(this.newMap);
            this.Controls.Add(this.assetList);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Map_Manager";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mario Kart Map Manager";
            ((System.ComponentModel.ISupportInitialize)(this.previewImage)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox assetList;
        private System.Windows.Forms.Button newMap;
        private System.Windows.Forms.Button deleteMap;
        private System.Windows.Forms.Button editMap;
        private System.Windows.Forms.PictureBox previewImage;
    }
}