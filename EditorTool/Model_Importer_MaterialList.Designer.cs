namespace EditorTool
{
    partial class Model_Importer_MaterialList
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Model_Importer_MaterialList));
            this.materialList = new System.Windows.Forms.ListBox();
            this.editMaterial = new System.Windows.Forms.Button();
            this.SaveMaterials = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.materialPreview = new System.Windows.Forms.PictureBox();
            this.isTrack = new System.Windows.Forms.CheckBox();
            this.isBoostPad = new System.Windows.Forms.CheckBox();
            this.isOffTrack = new System.Windows.Forms.CheckBox();
            this.shouldFlipUVs = new System.Windows.Forms.CheckBox();
            this.configPreview = new System.Windows.Forms.GroupBox();
            this.autoDetect = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.materialPreview)).BeginInit();
            this.configPreview.SuspendLayout();
            this.SuspendLayout();
            // 
            // materialList
            // 
            this.materialList.FormattingEnabled = true;
            this.materialList.Location = new System.Drawing.Point(12, 25);
            this.materialList.Name = "materialList";
            this.materialList.Size = new System.Drawing.Size(254, 342);
            this.materialList.TabIndex = 0;
            this.materialList.SelectedIndexChanged += new System.EventHandler(this.materialList_SelectedIndexChanged);
            // 
            // editMaterial
            // 
            this.editMaterial.Location = new System.Drawing.Point(272, 219);
            this.editMaterial.Name = "editMaterial";
            this.editMaterial.Size = new System.Drawing.Size(94, 36);
            this.editMaterial.TabIndex = 2;
            this.editMaterial.Text = "Edit";
            this.editMaterial.UseVisualStyleBackColor = true;
            this.editMaterial.Click += new System.EventHandler(this.editMaterial_Click);
            // 
            // SaveMaterials
            // 
            this.SaveMaterials.Location = new System.Drawing.Point(12, 373);
            this.SaveMaterials.Name = "SaveMaterials";
            this.SaveMaterials.Size = new System.Drawing.Size(354, 29);
            this.SaveMaterials.TabIndex = 3;
            this.SaveMaterials.Text = "Continue";
            this.SaveMaterials.UseVisualStyleBackColor = true;
            this.SaveMaterials.Click += new System.EventHandler(this.SaveMaterials_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Model Materials";
            // 
            // materialPreview
            // 
            this.materialPreview.Location = new System.Drawing.Point(272, 25);
            this.materialPreview.Name = "materialPreview";
            this.materialPreview.Size = new System.Drawing.Size(94, 94);
            this.materialPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.materialPreview.TabIndex = 14;
            this.materialPreview.TabStop = false;
            // 
            // isTrack
            // 
            this.isTrack.AutoCheck = false;
            this.isTrack.AutoSize = true;
            this.isTrack.Location = new System.Drawing.Point(6, 19);
            this.isTrack.Name = "isTrack";
            this.isTrack.Size = new System.Drawing.Size(65, 17);
            this.isTrack.TabIndex = 15;
            this.isTrack.Text = "Is Track";
            this.isTrack.UseVisualStyleBackColor = true;
            // 
            // isBoostPad
            // 
            this.isBoostPad.AutoCheck = false;
            this.isBoostPad.AutoSize = true;
            this.isBoostPad.Location = new System.Drawing.Point(6, 65);
            this.isBoostPad.Name = "isBoostPad";
            this.isBoostPad.Size = new System.Drawing.Size(86, 17);
            this.isBoostPad.TabIndex = 16;
            this.isBoostPad.Text = "Is Boost Pad";
            this.isBoostPad.UseVisualStyleBackColor = true;
            // 
            // isOffTrack
            // 
            this.isOffTrack.AutoCheck = false;
            this.isOffTrack.AutoSize = true;
            this.isOffTrack.Location = new System.Drawing.Point(6, 42);
            this.isOffTrack.Name = "isOffTrack";
            this.isOffTrack.Size = new System.Drawing.Size(82, 17);
            this.isOffTrack.TabIndex = 17;
            this.isOffTrack.Text = "Is Off-Track";
            this.isOffTrack.UseVisualStyleBackColor = true;
            // 
            // shouldFlipUVs
            // 
            this.shouldFlipUVs.AutoSize = true;
            this.shouldFlipUVs.Checked = true;
            this.shouldFlipUVs.CheckState = System.Windows.Forms.CheckState.Checked;
            this.shouldFlipUVs.Location = new System.Drawing.Point(273, 322);
            this.shouldFlipUVs.Name = "shouldFlipUVs";
            this.shouldFlipUVs.Size = new System.Drawing.Size(65, 17);
            this.shouldFlipUVs.TabIndex = 18;
            this.shouldFlipUVs.Text = "Flip UVs";
            this.shouldFlipUVs.UseVisualStyleBackColor = true;
            // 
            // configPreview
            // 
            this.configPreview.Controls.Add(this.isTrack);
            this.configPreview.Controls.Add(this.isBoostPad);
            this.configPreview.Controls.Add(this.isOffTrack);
            this.configPreview.Location = new System.Drawing.Point(272, 125);
            this.configPreview.Name = "configPreview";
            this.configPreview.Size = new System.Drawing.Size(94, 88);
            this.configPreview.TabIndex = 19;
            this.configPreview.TabStop = false;
            this.configPreview.Text = "Config";
            // 
            // autoDetect
            // 
            this.autoDetect.Location = new System.Drawing.Point(272, 345);
            this.autoDetect.Name = "autoDetect";
            this.autoDetect.Size = new System.Drawing.Size(94, 23);
            this.autoDetect.TabIndex = 20;
            this.autoDetect.Text = "Auto Detect";
            this.autoDetect.UseVisualStyleBackColor = true;
            this.autoDetect.Click += new System.EventHandler(this.autoDetect_Click);
            // 
            // Model_Importer_MaterialList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(378, 411);
            this.Controls.Add(this.autoDetect);
            this.Controls.Add(this.configPreview);
            this.Controls.Add(this.shouldFlipUVs);
            this.Controls.Add(this.materialPreview);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.SaveMaterials);
            this.Controls.Add(this.editMaterial);
            this.Controls.Add(this.materialList);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Model_Importer_MaterialList";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Import Model";
            this.Load += new System.EventHandler(this.Model_Importer_pt2_Load);
            ((System.ComponentModel.ISupportInitialize)(this.materialPreview)).EndInit();
            this.configPreview.ResumeLayout(false);
            this.configPreview.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox materialList;
        private System.Windows.Forms.Button editMaterial;
        private System.Windows.Forms.Button SaveMaterials;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox materialPreview;
        private System.Windows.Forms.CheckBox isTrack;
        private System.Windows.Forms.CheckBox isBoostPad;
        private System.Windows.Forms.CheckBox isOffTrack;
        private System.Windows.Forms.CheckBox shouldFlipUVs;
        private System.Windows.Forms.GroupBox configPreview;
        private System.Windows.Forms.Button autoDetect;
    }
}