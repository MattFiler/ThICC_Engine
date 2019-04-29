namespace EditorTool
{
    partial class Create_InGame_Models_Common
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
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.loadReferee = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.courseReferee = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.glider = new System.Windows.Forms.TextBox();
            this.loadGlider = new System.Windows.Forms.Button();
            this.saveModels = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.loadReferee);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.courseReferee);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.glider);
            this.groupBox2.Controls.Add(this.loadGlider);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(327, 104);
            this.groupBox2.TabIndex = 25;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Common Models";
            // 
            // loadReferee
            // 
            this.loadReferee.Location = new System.Drawing.Point(244, 36);
            this.loadReferee.Name = "loadReferee";
            this.loadReferee.Size = new System.Drawing.Size(75, 23);
            this.loadReferee.TabIndex = 11;
            this.loadReferee.Text = "Load";
            this.loadReferee.UseVisualStyleBackColor = true;
            this.loadReferee.Click += new System.EventHandler(this.loadReferee_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(81, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Course Referee";
            // 
            // courseReferee
            // 
            this.courseReferee.Location = new System.Drawing.Point(9, 37);
            this.courseReferee.Name = "courseReferee";
            this.courseReferee.ReadOnly = true;
            this.courseReferee.Size = new System.Drawing.Size(229, 20);
            this.courseReferee.TabIndex = 8;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 60);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(34, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Glider";
            // 
            // glider
            // 
            this.glider.Location = new System.Drawing.Point(9, 76);
            this.glider.Name = "glider";
            this.glider.ReadOnly = true;
            this.glider.Size = new System.Drawing.Size(229, 20);
            this.glider.TabIndex = 1;
            // 
            // loadGlider
            // 
            this.loadGlider.Location = new System.Drawing.Point(244, 75);
            this.loadGlider.Name = "loadGlider";
            this.loadGlider.Size = new System.Drawing.Size(75, 23);
            this.loadGlider.TabIndex = 2;
            this.loadGlider.Text = "Load";
            this.loadGlider.UseVisualStyleBackColor = true;
            this.loadGlider.Click += new System.EventHandler(this.loadGlider_Click);
            // 
            // saveModels
            // 
            this.saveModels.Location = new System.Drawing.Point(12, 122);
            this.saveModels.Name = "saveModels";
            this.saveModels.Size = new System.Drawing.Size(327, 23);
            this.saveModels.TabIndex = 26;
            this.saveModels.Text = "Save";
            this.saveModels.UseVisualStyleBackColor = true;
            this.saveModels.Click += new System.EventHandler(this.saveModels_Click);
            // 
            // Create_InGame_Models_Common
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(352, 157);
            this.Controls.Add(this.saveModels);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Create_InGame_Models_Common";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Common In-Game Models";
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button loadReferee;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox courseReferee;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox glider;
        private System.Windows.Forms.Button loadGlider;
        private System.Windows.Forms.Button saveModels;
    }
}