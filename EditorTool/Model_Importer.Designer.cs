namespace EditorTool
{
    partial class Model_Importer
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
            this.label1 = new System.Windows.Forms.Label();
            this.textureList = new System.Windows.Forms.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.addTexture = new System.Windows.Forms.Button();
            this.removeTexture = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // importModel
            // 
            this.importModel.Location = new System.Drawing.Point(11, 165);
            this.importModel.Name = "importModel";
            this.importModel.Size = new System.Drawing.Size(264, 28);
            this.importModel.TabIndex = 0;
            this.importModel.Text = "Import";
            this.importModel.UseVisualStyleBackColor = true;
            this.importModel.Click += new System.EventHandler(this.importModel_Click);
            // 
            // browseToModel
            // 
            this.browseToModel.Location = new System.Drawing.Point(200, 24);
            this.browseToModel.Name = "browseToModel";
            this.browseToModel.Size = new System.Drawing.Size(75, 22);
            this.browseToModel.TabIndex = 1;
            this.browseToModel.Text = "Browse";
            this.browseToModel.UseVisualStyleBackColor = true;
            this.browseToModel.Click += new System.EventHandler(this.browseToModel_Click);
            // 
            // modelPath
            // 
            this.modelPath.Location = new System.Drawing.Point(11, 25);
            this.modelPath.Name = "modelPath";
            this.modelPath.ReadOnly = true;
            this.modelPath.Size = new System.Drawing.Size(183, 20);
            this.modelPath.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Model File (OBJ)";
            // 
            // textureList
            // 
            this.textureList.FormattingEnabled = true;
            this.textureList.Location = new System.Drawing.Point(11, 64);
            this.textureList.Name = "textureList";
            this.textureList.Size = new System.Drawing.Size(183, 95);
            this.textureList.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(137, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Model Textures (PNG/JPG)";
            // 
            // addTexture
            // 
            this.addTexture.Location = new System.Drawing.Point(200, 64);
            this.addTexture.Name = "addTexture";
            this.addTexture.Size = new System.Drawing.Size(75, 22);
            this.addTexture.TabIndex = 6;
            this.addTexture.Text = "Add";
            this.addTexture.UseVisualStyleBackColor = true;
            this.addTexture.Click += new System.EventHandler(this.addTexture_Click);
            // 
            // removeTexture
            // 
            this.removeTexture.Location = new System.Drawing.Point(200, 92);
            this.removeTexture.Name = "removeTexture";
            this.removeTexture.Size = new System.Drawing.Size(75, 22);
            this.removeTexture.TabIndex = 7;
            this.removeTexture.Text = "Remove";
            this.removeTexture.UseVisualStyleBackColor = true;
            this.removeTexture.Click += new System.EventHandler(this.removeTexture_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 203);
            this.Controls.Add(this.removeTexture);
            this.Controls.Add(this.addTexture);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textureList);
            this.Controls.Add(this.label1);
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
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox textureList;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button addTexture;
        private System.Windows.Forms.Button removeTexture;
    }
}

