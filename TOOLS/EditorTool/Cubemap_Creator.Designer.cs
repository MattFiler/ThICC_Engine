namespace EditorTool
{
    partial class Cubemap_Creator
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
            this.loadPX = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.pathPX = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.loadNZ = new System.Windows.Forms.Button();
            this.pathNZ = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.loadPZ = new System.Windows.Forms.Button();
            this.pathPZ = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.loadNY = new System.Windows.Forms.Button();
            this.pathNY = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.loadPY = new System.Windows.Forms.Button();
            this.pathPY = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.loadNX = new System.Windows.Forms.Button();
            this.pathNX = new System.Windows.Forms.TextBox();
            this.createCubemap = new System.Windows.Forms.Button();
            this.previewNX = new System.Windows.Forms.PictureBox();
            this.previewPZ = new System.Windows.Forms.PictureBox();
            this.previewNZ = new System.Windows.Forms.PictureBox();
            this.previewPX = new System.Windows.Forms.PictureBox();
            this.previewNY = new System.Windows.Forms.PictureBox();
            this.previewPY = new System.Windows.Forms.PictureBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.asset_name = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.previewNX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewPZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewNZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewPX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewNY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewPY)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // loadPX
            // 
            this.loadPX.Location = new System.Drawing.Point(244, 33);
            this.loadPX.Name = "loadPX";
            this.loadPX.Size = new System.Drawing.Size(75, 23);
            this.loadPX.TabIndex = 14;
            this.loadPX.Text = "Load";
            this.loadPX.UseVisualStyleBackColor = true;
            this.loadPX.Click += new System.EventHandler(this.loadPX_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 18);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(54, 13);
            this.label2.TabIndex = 13;
            this.label2.Text = "Positive-X";
            // 
            // pathPX
            // 
            this.pathPX.Location = new System.Drawing.Point(9, 34);
            this.pathPX.Name = "pathPX";
            this.pathPX.ReadOnly = true;
            this.pathPX.Size = new System.Drawing.Size(229, 20);
            this.pathPX.TabIndex = 12;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.loadNZ);
            this.groupBox1.Controls.Add(this.pathNZ);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.loadPZ);
            this.groupBox1.Controls.Add(this.pathPZ);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.loadNY);
            this.groupBox1.Controls.Add(this.pathNY);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.loadPY);
            this.groupBox1.Controls.Add(this.pathPY);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.loadNX);
            this.groupBox1.Controls.Add(this.pathNX);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.loadPX);
            this.groupBox1.Controls.Add(this.pathPX);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(327, 258);
            this.groupBox1.TabIndex = 15;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Faces";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 213);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(60, 13);
            this.label6.TabIndex = 28;
            this.label6.Text = "Negative-Z";
            // 
            // loadNZ
            // 
            this.loadNZ.Location = new System.Drawing.Point(244, 228);
            this.loadNZ.Name = "loadNZ";
            this.loadNZ.Size = new System.Drawing.Size(75, 23);
            this.loadNZ.TabIndex = 29;
            this.loadNZ.Text = "Load";
            this.loadNZ.UseVisualStyleBackColor = true;
            this.loadNZ.Click += new System.EventHandler(this.loadNZ_Click);
            // 
            // pathNZ
            // 
            this.pathNZ.Location = new System.Drawing.Point(9, 229);
            this.pathNZ.Name = "pathNZ";
            this.pathNZ.ReadOnly = true;
            this.pathNZ.Size = new System.Drawing.Size(229, 20);
            this.pathNZ.TabIndex = 27;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 174);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(54, 13);
            this.label5.TabIndex = 25;
            this.label5.Text = "Positive-Z";
            // 
            // loadPZ
            // 
            this.loadPZ.Location = new System.Drawing.Point(244, 189);
            this.loadPZ.Name = "loadPZ";
            this.loadPZ.Size = new System.Drawing.Size(75, 23);
            this.loadPZ.TabIndex = 26;
            this.loadPZ.Text = "Load";
            this.loadPZ.UseVisualStyleBackColor = true;
            this.loadPZ.Click += new System.EventHandler(this.loadPZ_Click);
            // 
            // pathPZ
            // 
            this.pathPZ.Location = new System.Drawing.Point(9, 190);
            this.pathPZ.Name = "pathPZ";
            this.pathPZ.ReadOnly = true;
            this.pathPZ.Size = new System.Drawing.Size(229, 20);
            this.pathPZ.TabIndex = 24;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 135);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 13);
            this.label4.TabIndex = 22;
            this.label4.Text = "Negative-Y";
            // 
            // loadNY
            // 
            this.loadNY.Location = new System.Drawing.Point(244, 150);
            this.loadNY.Name = "loadNY";
            this.loadNY.Size = new System.Drawing.Size(75, 23);
            this.loadNY.TabIndex = 23;
            this.loadNY.Text = "Load";
            this.loadNY.UseVisualStyleBackColor = true;
            this.loadNY.Click += new System.EventHandler(this.loadNY_Click);
            // 
            // pathNY
            // 
            this.pathNY.Location = new System.Drawing.Point(9, 151);
            this.pathNY.Name = "pathNY";
            this.pathNY.ReadOnly = true;
            this.pathNY.Size = new System.Drawing.Size(229, 20);
            this.pathNY.TabIndex = 21;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 13);
            this.label3.TabIndex = 19;
            this.label3.Text = "Positive-Y";
            // 
            // loadPY
            // 
            this.loadPY.Location = new System.Drawing.Point(244, 111);
            this.loadPY.Name = "loadPY";
            this.loadPY.Size = new System.Drawing.Size(75, 23);
            this.loadPY.TabIndex = 20;
            this.loadPY.Text = "Load";
            this.loadPY.UseVisualStyleBackColor = true;
            this.loadPY.Click += new System.EventHandler(this.loadPY_Click);
            // 
            // pathPY
            // 
            this.pathPY.Location = new System.Drawing.Point(9, 112);
            this.pathPY.Name = "pathPY";
            this.pathPY.ReadOnly = true;
            this.pathPY.Size = new System.Drawing.Size(229, 20);
            this.pathPY.TabIndex = 18;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 57);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 16;
            this.label1.Text = "Negative-X";
            // 
            // loadNX
            // 
            this.loadNX.Location = new System.Drawing.Point(244, 72);
            this.loadNX.Name = "loadNX";
            this.loadNX.Size = new System.Drawing.Size(75, 23);
            this.loadNX.TabIndex = 17;
            this.loadNX.Text = "Load";
            this.loadNX.UseVisualStyleBackColor = true;
            this.loadNX.Click += new System.EventHandler(this.loadNX_Click);
            // 
            // pathNX
            // 
            this.pathNX.Location = new System.Drawing.Point(9, 73);
            this.pathNX.Name = "pathNX";
            this.pathNX.ReadOnly = true;
            this.pathNX.Size = new System.Drawing.Size(229, 20);
            this.pathNX.TabIndex = 15;
            // 
            // createCubemap
            // 
            this.createCubemap.Location = new System.Drawing.Point(12, 276);
            this.createCubemap.Name = "createCubemap";
            this.createCubemap.Size = new System.Drawing.Size(544, 23);
            this.createCubemap.TabIndex = 16;
            this.createCubemap.Text = "Create Cubemap";
            this.createCubemap.UseVisualStyleBackColor = true;
            this.createCubemap.Click += new System.EventHandler(this.createCubemap_Click);
            // 
            // previewNX
            // 
            this.previewNX.Location = new System.Drawing.Point(6, 75);
            this.previewNX.Name = "previewNX";
            this.previewNX.Size = new System.Drawing.Size(50, 50);
            this.previewNX.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.previewNX.TabIndex = 17;
            this.previewNX.TabStop = false;
            // 
            // previewPZ
            // 
            this.previewPZ.Location = new System.Drawing.Point(55, 75);
            this.previewPZ.Name = "previewPZ";
            this.previewPZ.Size = new System.Drawing.Size(50, 50);
            this.previewPZ.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.previewPZ.TabIndex = 18;
            this.previewPZ.TabStop = false;
            // 
            // previewNZ
            // 
            this.previewNZ.Location = new System.Drawing.Point(153, 75);
            this.previewNZ.Name = "previewNZ";
            this.previewNZ.Size = new System.Drawing.Size(50, 50);
            this.previewNZ.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.previewNZ.TabIndex = 20;
            this.previewNZ.TabStop = false;
            // 
            // previewPX
            // 
            this.previewPX.Location = new System.Drawing.Point(104, 75);
            this.previewPX.Name = "previewPX";
            this.previewPX.Size = new System.Drawing.Size(50, 50);
            this.previewPX.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.previewPX.TabIndex = 19;
            this.previewPX.TabStop = false;
            // 
            // previewNY
            // 
            this.previewNY.Location = new System.Drawing.Point(55, 124);
            this.previewNY.Name = "previewNY";
            this.previewNY.Size = new System.Drawing.Size(50, 50);
            this.previewNY.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.previewNY.TabIndex = 21;
            this.previewNY.TabStop = false;
            // 
            // previewPY
            // 
            this.previewPY.Location = new System.Drawing.Point(55, 26);
            this.previewPY.Name = "previewPY";
            this.previewPY.Size = new System.Drawing.Size(50, 50);
            this.previewPY.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.previewPY.TabIndex = 22;
            this.previewPY.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.previewPY);
            this.groupBox2.Controls.Add(this.previewNX);
            this.groupBox2.Controls.Add(this.previewNY);
            this.groupBox2.Controls.Add(this.previewPZ);
            this.groupBox2.Controls.Add(this.previewNZ);
            this.groupBox2.Controls.Add(this.previewPX);
            this.groupBox2.Location = new System.Drawing.Point(345, 78);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(211, 192);
            this.groupBox2.TabIndex = 23;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Preview";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.asset_name);
            this.groupBox3.Location = new System.Drawing.Point(345, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(211, 60);
            this.groupBox3.TabIndex = 24;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Misc";
            // 
            // asset_name
            // 
            this.asset_name.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.asset_name.Location = new System.Drawing.Point(6, 34);
            this.asset_name.Name = "asset_name";
            this.asset_name.Size = new System.Drawing.Size(197, 20);
            this.asset_name.TabIndex = 0;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(3, 18);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(35, 13);
            this.label7.TabIndex = 30;
            this.label7.Text = "Name";
            // 
            // Cubemap_Creator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(565, 307);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.createCubemap);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Cubemap_Creator";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Cubemap Creator";
            this.Load += new System.EventHandler(this.Cubemap_Creator_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.previewNX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewPZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewNZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewPX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewNY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.previewPY)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button loadPX;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox pathPX;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button createCubemap;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button loadNZ;
        private System.Windows.Forms.TextBox pathNZ;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button loadPZ;
        private System.Windows.Forms.TextBox pathPZ;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button loadNY;
        private System.Windows.Forms.TextBox pathNY;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button loadPY;
        private System.Windows.Forms.TextBox pathPY;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button loadNX;
        private System.Windows.Forms.TextBox pathNX;
        private System.Windows.Forms.PictureBox previewNX;
        private System.Windows.Forms.PictureBox previewPZ;
        private System.Windows.Forms.PictureBox previewNZ;
        private System.Windows.Forms.PictureBox previewPX;
        private System.Windows.Forms.PictureBox previewNY;
        private System.Windows.Forms.PictureBox previewPY;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox asset_name;
    }
}