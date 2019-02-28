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
            this.label2 = new System.Windows.Forms.Label();
            this.assetName = new System.Windows.Forms.TextBox();
            this.shouldGenerateCollmap = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // importModel
            // 
            this.importModel.Location = new System.Drawing.Point(15, 113);
            this.importModel.Name = "importModel";
            this.importModel.Size = new System.Drawing.Size(264, 28);
            this.importModel.TabIndex = 0;
            this.importModel.Text = "Import";
            this.importModel.UseVisualStyleBackColor = true;
            this.importModel.Click += new System.EventHandler(this.importModel_Click);
            // 
            // browseToModel
            // 
            this.browseToModel.Location = new System.Drawing.Point(204, 63);
            this.browseToModel.Name = "browseToModel";
            this.browseToModel.Size = new System.Drawing.Size(75, 22);
            this.browseToModel.TabIndex = 1;
            this.browseToModel.Text = "Browse";
            this.browseToModel.UseVisualStyleBackColor = true;
            this.browseToModel.Click += new System.EventHandler(this.browseToModel_Click);
            // 
            // modelPath
            // 
            this.modelPath.Location = new System.Drawing.Point(15, 64);
            this.modelPath.Name = "modelPath";
            this.modelPath.ReadOnly = true;
            this.modelPath.Size = new System.Drawing.Size(183, 20);
            this.modelPath.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 48);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Model File (OBJ)";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Asset Name";
            // 
            // assetName
            // 
            this.assetName.Location = new System.Drawing.Point(15, 25);
            this.assetName.Name = "assetName";
            this.assetName.Size = new System.Drawing.Size(264, 20);
            this.assetName.TabIndex = 4;
            // 
            // shouldGenerateCollmap
            // 
            this.shouldGenerateCollmap.AutoSize = true;
            this.shouldGenerateCollmap.Location = new System.Drawing.Point(15, 90);
            this.shouldGenerateCollmap.Name = "shouldGenerateCollmap";
            this.shouldGenerateCollmap.Size = new System.Drawing.Size(135, 17);
            this.shouldGenerateCollmap.TabIndex = 6;
            this.shouldGenerateCollmap.Text = "Generate Collision Map";
            this.shouldGenerateCollmap.UseVisualStyleBackColor = true;
            // 
            // Model_Importer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(294, 155);
            this.Controls.Add(this.shouldGenerateCollmap);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.assetName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.modelPath);
            this.Controls.Add(this.browseToModel);
            this.Controls.Add(this.importModel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Model_Importer";
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
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox assetName;
        private System.Windows.Forms.CheckBox shouldGenerateCollmap;
    }
}

