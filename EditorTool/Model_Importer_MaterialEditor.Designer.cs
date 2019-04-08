namespace EditorTool
{
    partial class Model_Importer_MaterialEditor
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
            this.materialPreview = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.materialName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.isTransparent = new System.Windows.Forms.CheckBox();
            this.transparencySlider = new System.Windows.Forms.TrackBar();
            this.saveMaterial = new System.Windows.Forms.Button();
            this.isOffTrack = new System.Windows.Forms.CheckBox();
            this.isBoostPad = new System.Windows.Forms.CheckBox();
            this.isTrack = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.domainUpDown1 = new System.Windows.Forms.DomainUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.label5 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.materialPreview)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.transparencySlider)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            this.SuspendLayout();
            // 
            // materialPreview
            // 
            this.materialPreview.Location = new System.Drawing.Point(12, 12);
            this.materialPreview.Name = "materialPreview";
            this.materialPreview.Size = new System.Drawing.Size(300, 300);
            this.materialPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.materialPreview.TabIndex = 15;
            this.materialPreview.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(75, 13);
            this.label1.TabIndex = 16;
            this.label1.Text = "Material Name";
            // 
            // materialName
            // 
            this.materialName.Location = new System.Drawing.Point(9, 36);
            this.materialName.Name = "materialName";
            this.materialName.ReadOnly = true;
            this.materialName.Size = new System.Drawing.Size(297, 20);
            this.materialName.TabIndex = 17;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 214);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "Transparency";
            // 
            // isTransparent
            // 
            this.isTransparent.AutoSize = true;
            this.isTransparent.Location = new System.Drawing.Point(223, 214);
            this.isTransparent.Name = "isTransparent";
            this.isTransparent.Size = new System.Drawing.Size(83, 17);
            this.isTransparent.TabIndex = 19;
            this.isTransparent.Text = "Transparent";
            this.isTransparent.UseVisualStyleBackColor = true;
            // 
            // transparencySlider
            // 
            this.transparencySlider.LargeChange = 1;
            this.transparencySlider.Location = new System.Drawing.Point(9, 233);
            this.transparencySlider.Name = "transparencySlider";
            this.transparencySlider.Size = new System.Drawing.Size(297, 45);
            this.transparencySlider.TabIndex = 20;
            // 
            // saveMaterial
            // 
            this.saveMaterial.Location = new System.Drawing.Point(231, 275);
            this.saveMaterial.Name = "saveMaterial";
            this.saveMaterial.Size = new System.Drawing.Size(75, 23);
            this.saveMaterial.TabIndex = 21;
            this.saveMaterial.Text = "Save";
            this.saveMaterial.UseVisualStyleBackColor = true;
            this.saveMaterial.Click += new System.EventHandler(this.saveMaterial_Click);
            // 
            // isOffTrack
            // 
            this.isOffTrack.AutoSize = true;
            this.isOffTrack.Location = new System.Drawing.Point(80, 62);
            this.isOffTrack.Name = "isOffTrack";
            this.isOffTrack.Size = new System.Drawing.Size(82, 17);
            this.isOffTrack.TabIndex = 24;
            this.isOffTrack.Text = "Is Off-Track";
            this.isOffTrack.UseVisualStyleBackColor = true;
            // 
            // isBoostPad
            // 
            this.isBoostPad.AutoSize = true;
            this.isBoostPad.Location = new System.Drawing.Point(168, 62);
            this.isBoostPad.Name = "isBoostPad";
            this.isBoostPad.Size = new System.Drawing.Size(86, 17);
            this.isBoostPad.TabIndex = 23;
            this.isBoostPad.Text = "Is Boost Pad";
            this.isBoostPad.UseVisualStyleBackColor = true;
            // 
            // isTrack
            // 
            this.isTrack.AutoSize = true;
            this.isTrack.Location = new System.Drawing.Point(9, 62);
            this.isTrack.Name = "isTrack";
            this.isTrack.Size = new System.Drawing.Size(65, 17);
            this.isTrack.TabIndex = 22;
            this.isTrack.Text = "Is Track";
            this.isTrack.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.domainUpDown1);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.isOffTrack);
            this.groupBox1.Controls.Add(this.materialName);
            this.groupBox1.Controls.Add(this.isBoostPad);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.isTrack);
            this.groupBox1.Controls.Add(this.isTransparent);
            this.groupBox1.Controls.Add(this.saveMaterial);
            this.groupBox1.Controls.Add(this.transparencySlider);
            this.groupBox1.Location = new System.Drawing.Point(318, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(314, 306);
            this.groupBox1.TabIndex = 25;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "groupBox1";
            // 
            // domainUpDown1
            // 
            this.domainUpDown1.Location = new System.Drawing.Point(9, 122);
            this.domainUpDown1.Name = "domainUpDown1";
            this.domainUpDown1.Size = new System.Drawing.Size(153, 20);
            this.domainUpDown1.TabIndex = 25;
            this.domainUpDown1.Text = "domainUpDown1";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 106);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 13);
            this.label3.TabIndex = 26;
            this.label3.Text = "Material Name";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.trackBar2);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.button1);
            this.groupBox2.Controls.Add(this.trackBar1);
            this.groupBox2.Location = new System.Drawing.Point(12, 318);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(314, 306);
            this.groupBox2.TabIndex = 27;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "groupBox2";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(97, 13);
            this.label6.TabIndex = 18;
            this.label6.Text = "Specular Exponent";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(231, 275);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 21;
            this.button1.Text = "Save";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // trackBar1
            // 
            this.trackBar1.LargeChange = 1;
            this.trackBar1.Location = new System.Drawing.Point(9, 39);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(297, 45);
            this.trackBar1.TabIndex = 20;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 87);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(97, 13);
            this.label4.TabIndex = 22;
            this.label4.Text = "Specular Exponent";
            // 
            // trackBar2
            // 
            this.trackBar2.LargeChange = 1;
            this.trackBar2.Location = new System.Drawing.Point(9, 106);
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Size = new System.Drawing.Size(297, 45);
            this.trackBar2.TabIndex = 23;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(209, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(97, 13);
            this.label5.TabIndex = 24;
            this.label5.Text = "Specular Exponent";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // Model_Importer_MaterialEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(641, 638);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.materialPreview);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Model_Importer_MaterialEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Material Editor";
            this.Load += new System.EventHandler(this.Model_Importer_MaterialEditor_Load);
            ((System.ComponentModel.ISupportInitialize)(this.materialPreview)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.transparencySlider)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox materialPreview;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox materialName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox isTransparent;
        private System.Windows.Forms.TrackBar transparencySlider;
        private System.Windows.Forms.Button saveMaterial;
        private System.Windows.Forms.CheckBox isOffTrack;
        private System.Windows.Forms.CheckBox isBoostPad;
        private System.Windows.Forms.CheckBox isTrack;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.DomainUpDown domainUpDown1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar trackBar2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Label label5;
    }
}