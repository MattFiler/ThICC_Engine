namespace EditorTool
{
    partial class Create_Cup
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Create_Cup));
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.loadString = new System.Windows.Forms.Button();
            this.cupCodename = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.cupName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cupPreviewImage = new System.Windows.Forms.TextBox();
            this.selectImageAsset = new System.Windows.Forms.Button();
            this.saveCup = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.loadString);
            this.groupBox2.Controls.Add(this.cupCodename);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.cupName);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.cupPreviewImage);
            this.groupBox2.Controls.Add(this.selectImageAsset);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(327, 140);
            this.groupBox2.TabIndex = 22;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Cup Configuration";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 17);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(63, 13);
            this.label8.TabIndex = 23;
            this.label8.Text = "Code Name";
            // 
            // loadString
            // 
            this.loadString.Location = new System.Drawing.Point(244, 71);
            this.loadString.Name = "loadString";
            this.loadString.Size = new System.Drawing.Size(75, 23);
            this.loadString.TabIndex = 11;
            this.loadString.Text = "Load";
            this.loadString.UseVisualStyleBackColor = true;
            this.loadString.Click += new System.EventHandler(this.loadString_Click);
            // 
            // cupCodename
            // 
            this.cupCodename.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.cupCodename.Location = new System.Drawing.Point(9, 33);
            this.cupCodename.Name = "cupCodename";
            this.cupCodename.Size = new System.Drawing.Size(310, 20);
            this.cupCodename.TabIndex = 22;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(27, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Title";
            // 
            // cupName
            // 
            this.cupName.Location = new System.Drawing.Point(9, 72);
            this.cupName.Name = "cupName";
            this.cupName.ReadOnly = true;
            this.cupName.Size = new System.Drawing.Size(229, 20);
            this.cupName.TabIndex = 8;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 95);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Preview Image";
            // 
            // cupPreviewImage
            // 
            this.cupPreviewImage.Location = new System.Drawing.Point(9, 111);
            this.cupPreviewImage.Name = "cupPreviewImage";
            this.cupPreviewImage.ReadOnly = true;
            this.cupPreviewImage.Size = new System.Drawing.Size(229, 20);
            this.cupPreviewImage.TabIndex = 1;
            // 
            // selectImageAsset
            // 
            this.selectImageAsset.Location = new System.Drawing.Point(244, 110);
            this.selectImageAsset.Name = "selectImageAsset";
            this.selectImageAsset.Size = new System.Drawing.Size(75, 23);
            this.selectImageAsset.TabIndex = 2;
            this.selectImageAsset.Text = "Load";
            this.selectImageAsset.UseVisualStyleBackColor = true;
            this.selectImageAsset.Click += new System.EventHandler(this.selectImageAsset_Click);
            // 
            // saveCup
            // 
            this.saveCup.Location = new System.Drawing.Point(12, 158);
            this.saveCup.Name = "saveCup";
            this.saveCup.Size = new System.Drawing.Size(327, 23);
            this.saveCup.TabIndex = 23;
            this.saveCup.Text = "Create";
            this.saveCup.UseVisualStyleBackColor = true;
            this.saveCup.Click += new System.EventHandler(this.saveCup_Click);
            // 
            // Create_Cup
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(350, 191);
            this.Controls.Add(this.saveCup);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Create_Cup";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Create New Cup";
            this.Load += new System.EventHandler(this.Create_Cup_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button loadString;
        private System.Windows.Forms.TextBox cupCodename;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox cupName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox cupPreviewImage;
        private System.Windows.Forms.Button selectImageAsset;
        private System.Windows.Forms.Button saveCup;
    }
}