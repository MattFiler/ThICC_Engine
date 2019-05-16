namespace EditorTool
{
    partial class Texture_Map_Animator
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Texture_Map_Animator));
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.selectTexture = new System.Windows.Forms.Button();
            this.label16 = new System.Windows.Forms.Label();
            this.texture = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.outputFolder = new System.Windows.Forms.TextBox();
            this.selectOutputFolder = new System.Windows.Forms.Button();
            this.makeAnim = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.stepModifier = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox5.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.stepModifier)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.selectTexture);
            this.groupBox5.Controls.Add(this.label16);
            this.groupBox5.Controls.Add(this.texture);
            this.groupBox5.Controls.Add(this.label17);
            this.groupBox5.Controls.Add(this.outputFolder);
            this.groupBox5.Controls.Add(this.selectOutputFolder);
            this.groupBox5.Location = new System.Drawing.Point(12, 12);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(327, 101);
            this.groupBox5.TabIndex = 29;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Paths";
            // 
            // selectTexture
            // 
            this.selectTexture.Location = new System.Drawing.Point(244, 33);
            this.selectTexture.Name = "selectTexture";
            this.selectTexture.Size = new System.Drawing.Size(75, 23);
            this.selectTexture.TabIndex = 11;
            this.selectTexture.Text = "Load";
            this.selectTexture.UseVisualStyleBackColor = true;
            this.selectTexture.Click += new System.EventHandler(this.selectTexture_Click);
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(6, 18);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(67, 13);
            this.label16.TabIndex = 10;
            this.label16.Text = "Texture Map";
            // 
            // texture
            // 
            this.texture.Location = new System.Drawing.Point(9, 34);
            this.texture.Name = "texture";
            this.texture.ReadOnly = true;
            this.texture.Size = new System.Drawing.Size(229, 20);
            this.texture.TabIndex = 8;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(6, 57);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(71, 13);
            this.label17.TabIndex = 3;
            this.label17.Text = "Output Folder";
            // 
            // outputFolder
            // 
            this.outputFolder.Location = new System.Drawing.Point(9, 73);
            this.outputFolder.Name = "outputFolder";
            this.outputFolder.Size = new System.Drawing.Size(229, 20);
            this.outputFolder.TabIndex = 1;
            // 
            // selectOutputFolder
            // 
            this.selectOutputFolder.Location = new System.Drawing.Point(244, 72);
            this.selectOutputFolder.Name = "selectOutputFolder";
            this.selectOutputFolder.Size = new System.Drawing.Size(75, 23);
            this.selectOutputFolder.TabIndex = 2;
            this.selectOutputFolder.Text = "Load";
            this.selectOutputFolder.UseVisualStyleBackColor = true;
            this.selectOutputFolder.Click += new System.EventHandler(this.selectOutputFolder_Click);
            // 
            // makeAnim
            // 
            this.makeAnim.Location = new System.Drawing.Point(12, 197);
            this.makeAnim.Name = "makeAnim";
            this.makeAnim.Size = new System.Drawing.Size(327, 23);
            this.makeAnim.TabIndex = 26;
            this.makeAnim.Text = "Create";
            this.makeAnim.UseVisualStyleBackColor = true;
            this.makeAnim.Click += new System.EventHandler(this.makeAnim_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.stepModifier);
            this.groupBox1.Location = new System.Drawing.Point(12, 119);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(327, 72);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Modifiers";
            // 
            // stepModifier
            // 
            this.stepModifier.Location = new System.Drawing.Point(6, 19);
            this.stepModifier.Name = "stepModifier";
            this.stepModifier.Size = new System.Drawing.Size(313, 45);
            this.stepModifier.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 51);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "1 Step";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(272, 51);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(49, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "10 Steps";
            // 
            // Texture_Map_Animator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(350, 229);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.makeAnim);
            this.Controls.Add(this.groupBox5);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Texture_Map_Animator";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Texture Map Animator";
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.stepModifier)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button selectTexture;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox texture;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox outputFolder;
        private System.Windows.Forms.Button selectOutputFolder;
        private System.Windows.Forms.Button makeAnim;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar stepModifier;
    }
}