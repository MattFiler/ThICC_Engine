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
            this.loadAssetType = new System.Windows.Forms.ComboBox();
            this.assetList = new System.Windows.Forms.ListBox();
            this.deleteAsset = new System.Windows.Forms.Button();
            this.importAsset = new System.Windows.Forms.Button();
            this.editSelected = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.domainUpDown1 = new System.Windows.Forms.DomainUpDown();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // loadAssetType
            // 
            this.loadAssetType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.loadAssetType.FormattingEnabled = true;
            this.loadAssetType.Items.AddRange(new object[] {
            "Models",
            "Sounds",
            "Images",
            "Fonts"});
            this.loadAssetType.Location = new System.Drawing.Point(6, 19);
            this.loadAssetType.Name = "loadAssetType";
            this.loadAssetType.Size = new System.Drawing.Size(202, 21);
            this.loadAssetType.TabIndex = 2;
            this.loadAssetType.SelectedIndexChanged += new System.EventHandler(this.loadAssetType_SelectedIndexChanged);
            // 
            // assetList
            // 
            this.assetList.FormattingEnabled = true;
            this.assetList.Location = new System.Drawing.Point(6, 46);
            this.assetList.Name = "assetList";
            this.assetList.Size = new System.Drawing.Size(202, 277);
            this.assetList.TabIndex = 3;
            // 
            // deleteAsset
            // 
            this.deleteAsset.Location = new System.Drawing.Point(145, 329);
            this.deleteAsset.Name = "deleteAsset";
            this.deleteAsset.Size = new System.Drawing.Size(63, 23);
            this.deleteAsset.TabIndex = 4;
            this.deleteAsset.Text = "Delete";
            this.deleteAsset.UseVisualStyleBackColor = true;
            this.deleteAsset.Click += new System.EventHandler(this.deleteAsset_Click);
            // 
            // importAsset
            // 
            this.importAsset.Location = new System.Drawing.Point(6, 329);
            this.importAsset.Name = "importAsset";
            this.importAsset.Size = new System.Drawing.Size(63, 23);
            this.importAsset.TabIndex = 5;
            this.importAsset.Text = "New";
            this.importAsset.UseVisualStyleBackColor = true;
            this.importAsset.Click += new System.EventHandler(this.refreshList_Click);
            // 
            // editSelected
            // 
            this.editSelected.Location = new System.Drawing.Point(76, 329);
            this.editSelected.Name = "editSelected";
            this.editSelected.Size = new System.Drawing.Size(63, 23);
            this.editSelected.TabIndex = 6;
            this.editSelected.Text = "Edit";
            this.editSelected.UseVisualStyleBackColor = true;
            this.editSelected.Click += new System.EventHandler(this.editSelected_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.loadAssetType);
            this.groupBox2.Controls.Add(this.editSelected);
            this.groupBox2.Controls.Add(this.deleteAsset);
            this.groupBox2.Controls.Add(this.assetList);
            this.groupBox2.Controls.Add(this.importAsset);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(215, 364);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Asset Browser";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(66, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Model Scale";
            // 
            // domainUpDown1
            // 
            this.domainUpDown1.Location = new System.Drawing.Point(21, 46);
            this.domainUpDown1.Name = "domainUpDown1";
            this.domainUpDown1.Size = new System.Drawing.Size(120, 20);
            this.domainUpDown1.TabIndex = 10;
            this.domainUpDown1.Text = "domainUpDown1";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.domainUpDown1);
            this.groupBox3.Location = new System.Drawing.Point(233, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(409, 364);
            this.groupBox3.TabIndex = 11;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Asset Configuration";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(165, 180);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(84, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "COMING SOON";
            // 
            // Landing
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(653, 384);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Landing";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mario Kart Asset Manager";
            this.Load += new System.EventHandler(this.Landing_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ComboBox loadAssetType;
        private System.Windows.Forms.ListBox assetList;
        private System.Windows.Forms.Button deleteAsset;
        private System.Windows.Forms.Button importAsset;
        private System.Windows.Forms.Button editSelected;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DomainUpDown domainUpDown1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label2;
    }
}