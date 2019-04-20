namespace EditorTool
{
    partial class Model_Importer_AssetSelector
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Model_Importer_AssetSelector));
            this.trackConfig = new System.Windows.Forms.TextBox();
            this.locateTrackConfig = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.assetName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.modelPath = new System.Windows.Forms.TextBox();
            this.browseToModel = new System.Windows.Forms.Button();
            this.importModel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // trackConfig
            // 
            this.trackConfig.Location = new System.Drawing.Point(15, 105);
            this.trackConfig.Name = "trackConfig";
            this.trackConfig.ReadOnly = true;
            this.trackConfig.Size = new System.Drawing.Size(244, 20);
            this.trackConfig.TabIndex = 24;
            this.trackConfig.Visible = false;
            // 
            // locateTrackConfig
            // 
            this.locateTrackConfig.Location = new System.Drawing.Point(265, 104);
            this.locateTrackConfig.Name = "locateTrackConfig";
            this.locateTrackConfig.Size = new System.Drawing.Size(75, 22);
            this.locateTrackConfig.TabIndex = 23;
            this.locateTrackConfig.Text = "Browse";
            this.locateTrackConfig.UseVisualStyleBackColor = true;
            this.locateTrackConfig.Visible = false;
            this.locateTrackConfig.Click += new System.EventHandler(this.locateTrackConfig_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(122, 13);
            this.label3.TabIndex = 22;
            this.label3.Text = "Track Properties (JSON)";
            this.label3.Visible = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 21;
            this.label2.Text = "Asset Name";
            // 
            // assetName
            // 
            this.assetName.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.assetName.Location = new System.Drawing.Point(15, 25);
            this.assetName.Name = "assetName";
            this.assetName.Size = new System.Drawing.Size(325, 20);
            this.assetName.TabIndex = 20;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 48);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 13);
            this.label1.TabIndex = 19;
            this.label1.Text = "Model File (OBJ)";
            // 
            // modelPath
            // 
            this.modelPath.Location = new System.Drawing.Point(15, 64);
            this.modelPath.Name = "modelPath";
            this.modelPath.ReadOnly = true;
            this.modelPath.Size = new System.Drawing.Size(244, 20);
            this.modelPath.TabIndex = 18;
            // 
            // browseToModel
            // 
            this.browseToModel.Location = new System.Drawing.Point(265, 63);
            this.browseToModel.Name = "browseToModel";
            this.browseToModel.Size = new System.Drawing.Size(75, 22);
            this.browseToModel.TabIndex = 17;
            this.browseToModel.Text = "Browse";
            this.browseToModel.UseVisualStyleBackColor = true;
            this.browseToModel.Click += new System.EventHandler(this.browseToModel_Click);
            // 
            // importModel
            // 
            this.importModel.Location = new System.Drawing.Point(15, 132);
            this.importModel.Name = "importModel";
            this.importModel.Size = new System.Drawing.Size(325, 28);
            this.importModel.TabIndex = 25;
            this.importModel.Text = "Continue";
            this.importModel.UseVisualStyleBackColor = true;
            this.importModel.Click += new System.EventHandler(this.importModel_Click);
            // 
            // Model_Importer_AssetSelector
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(354, 171);
            this.Controls.Add(this.importModel);
            this.Controls.Add(this.trackConfig);
            this.Controls.Add(this.locateTrackConfig);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.assetName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.modelPath);
            this.Controls.Add(this.browseToModel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Model_Importer_AssetSelector";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Model Importer";
            this.Load += new System.EventHandler(this.Model_Importer_AssetSelector_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox trackConfig;
        private System.Windows.Forms.Button locateTrackConfig;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox assetName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox modelPath;
        private System.Windows.Forms.Button browseToModel;
        private System.Windows.Forms.Button importModel;
    }
}