namespace EditorTool
{
    partial class Form1
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
            this.importModel = new System.Windows.Forms.Button();
            this.browseToModel = new System.Windows.Forms.Button();
            this.modelPath = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // importModel
            // 
            this.importModel.Location = new System.Drawing.Point(11, 39);
            this.importModel.Name = "importModel";
            this.importModel.Size = new System.Drawing.Size(264, 28);
            this.importModel.TabIndex = 0;
            this.importModel.Text = "Import";
            this.importModel.UseVisualStyleBackColor = true;
            this.importModel.Click += new System.EventHandler(this.importModel_Click);
            // 
            // browseToModel
            // 
            this.browseToModel.Location = new System.Drawing.Point(201, 12);
            this.browseToModel.Name = "browseToModel";
            this.browseToModel.Size = new System.Drawing.Size(75, 22);
            this.browseToModel.TabIndex = 1;
            this.browseToModel.Text = "Browse";
            this.browseToModel.UseVisualStyleBackColor = true;
            this.browseToModel.Click += new System.EventHandler(this.browseToModel_Click);
            // 
            // modelPath
            // 
            this.modelPath.Location = new System.Drawing.Point(12, 13);
            this.modelPath.Name = "modelPath";
            this.modelPath.ReadOnly = true;
            this.modelPath.Size = new System.Drawing.Size(183, 20);
            this.modelPath.TabIndex = 2;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(287, 76);
            this.Controls.Add(this.modelPath);
            this.Controls.Add(this.browseToModel);
            this.Controls.Add(this.importModel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Import Model";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button importModel;
        private System.Windows.Forms.Button browseToModel;
        private System.Windows.Forms.TextBox modelPath;
    }
}

