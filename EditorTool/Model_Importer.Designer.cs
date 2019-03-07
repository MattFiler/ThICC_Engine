using System.Windows.Forms;

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
            this.itemMaterialCategories = new System.Windows.Forms.CheckedListBox();
            this.shouldFlipUV = new System.Windows.Forms.CheckBox();
            this.shouldCreateBoxCollider = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.itemMaterialCategoriesTrans = new System.Windows.Forms.CheckedListBox();
            this.enableTransparency = new System.Windows.Forms.CheckBox();
            this.resizedCollisionGroup = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.resizedCollisionGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // importModel
            // 
            this.importModel.Location = new System.Drawing.Point(19, 418);
            this.importModel.Name = "importModel";
            this.importModel.Size = new System.Drawing.Size(264, 28);
            this.importModel.TabIndex = 0;
            this.importModel.Text = "Import";
            this.importModel.UseVisualStyleBackColor = true;
            this.importModel.Click += new System.EventHandler(this.importModel_Click);
            // 
            // browseToModel
            // 
            this.browseToModel.Location = new System.Drawing.Point(208, 65);
            this.browseToModel.Name = "browseToModel";
            this.browseToModel.Size = new System.Drawing.Size(75, 22);
            this.browseToModel.TabIndex = 1;
            this.browseToModel.Text = "Browse";
            this.browseToModel.UseVisualStyleBackColor = true;
            this.browseToModel.Click += new System.EventHandler(this.browseToModel_Click);
            // 
            // modelPath
            // 
            this.modelPath.Location = new System.Drawing.Point(19, 66);
            this.modelPath.Name = "modelPath";
            this.modelPath.ReadOnly = true;
            this.modelPath.Size = new System.Drawing.Size(183, 20);
            this.modelPath.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 50);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Model File (OBJ)";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 11);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Asset Name";
            // 
            // assetName
            // 
            this.assetName.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.assetName.Location = new System.Drawing.Point(19, 27);
            this.assetName.Name = "assetName";
            this.assetName.Size = new System.Drawing.Size(264, 20);
            this.assetName.TabIndex = 4;
            // 
            // shouldGenerateCollmap
            // 
            this.shouldGenerateCollmap.AutoSize = true;
            this.shouldGenerateCollmap.Location = new System.Drawing.Point(6, 18);
            this.shouldGenerateCollmap.Name = "shouldGenerateCollmap";
            this.shouldGenerateCollmap.Size = new System.Drawing.Size(89, 17);
            this.shouldGenerateCollmap.TabIndex = 6;
            this.shouldGenerateCollmap.Text = "Mesh Collider";
            this.shouldGenerateCollmap.UseVisualStyleBackColor = true;
            this.shouldGenerateCollmap.CheckedChanged += new System.EventHandler(this.shouldGenerateCollmap_CheckedChanged_1);
            // 
            // itemMaterialCategories
            // 
            this.itemMaterialCategories.FormattingEnabled = true;
            this.itemMaterialCategories.Location = new System.Drawing.Point(6, 41);
            this.itemMaterialCategories.Name = "itemMaterialCategories";
            this.itemMaterialCategories.Size = new System.Drawing.Size(252, 109);
            this.itemMaterialCategories.TabIndex = 7;
            // 
            // shouldFlipUV
            // 
            this.shouldFlipUV.AutoSize = true;
            this.shouldFlipUV.Checked = true;
            this.shouldFlipUV.CheckState = System.Windows.Forms.CheckState.Checked;
            this.shouldFlipUV.Location = new System.Drawing.Point(139, 19);
            this.shouldFlipUV.Name = "shouldFlipUV";
            this.shouldFlipUV.Size = new System.Drawing.Size(65, 17);
            this.shouldFlipUV.TabIndex = 8;
            this.shouldFlipUV.Text = "Flip UVs";
            this.shouldFlipUV.UseVisualStyleBackColor = true;
            // 
            // shouldCreateBoxCollider
            // 
            this.shouldCreateBoxCollider.AutoSize = true;
            this.shouldCreateBoxCollider.Location = new System.Drawing.Point(101, 18);
            this.shouldCreateBoxCollider.Name = "shouldCreateBoxCollider";
            this.shouldCreateBoxCollider.Size = new System.Drawing.Size(81, 17);
            this.shouldCreateBoxCollider.TabIndex = 9;
            this.shouldCreateBoxCollider.Text = "Box Collider";
            this.shouldCreateBoxCollider.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.itemMaterialCategoriesTrans);
            this.groupBox1.Controls.Add(this.enableTransparency);
            this.groupBox1.Controls.Add(this.shouldFlipUV);
            this.groupBox1.Location = new System.Drawing.Point(19, 93);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(264, 157);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Model Materials";
            // 
            // itemMaterialCategoriesTrans
            // 
            this.itemMaterialCategoriesTrans.Enabled = false;
            this.itemMaterialCategoriesTrans.FormattingEnabled = true;
            this.itemMaterialCategoriesTrans.Location = new System.Drawing.Point(6, 42);
            this.itemMaterialCategoriesTrans.Name = "itemMaterialCategoriesTrans";
            this.itemMaterialCategoriesTrans.Size = new System.Drawing.Size(252, 109);
            this.itemMaterialCategoriesTrans.TabIndex = 12;
            this.itemMaterialCategoriesTrans.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.itemMaterialCategoriesTrans_SelectedIndexChanged);
            this.itemMaterialCategoriesTrans.SelectedIndexChanged += new System.EventHandler(this.itemMaterialCategoriesTrans_SelectedIndexChanged_1);
            // 
            // enableTransparency
            // 
            this.enableTransparency.AutoSize = true;
            this.enableTransparency.Location = new System.Drawing.Point(6, 19);
            this.enableTransparency.Name = "enableTransparency";
            this.enableTransparency.Size = new System.Drawing.Size(127, 17);
            this.enableTransparency.TabIndex = 10;
            this.enableTransparency.Text = "Enable Transparency";
            this.enableTransparency.UseVisualStyleBackColor = true;
            this.enableTransparency.CheckedChanged += new System.EventHandler(this.enableTransparency_CheckedChanged);
            // 
            // resizedCollisionGroup
            // 
            this.resizedCollisionGroup.Controls.Add(this.itemMaterialCategories);
            this.resizedCollisionGroup.Controls.Add(this.shouldGenerateCollmap);
            this.resizedCollisionGroup.Controls.Add(this.shouldCreateBoxCollider);
            this.resizedCollisionGroup.Location = new System.Drawing.Point(19, 256);
            this.resizedCollisionGroup.Name = "resizedCollisionGroup";
            this.resizedCollisionGroup.Size = new System.Drawing.Size(264, 156);
            this.resizedCollisionGroup.TabIndex = 12;
            this.resizedCollisionGroup.TabStop = false;
            this.resizedCollisionGroup.Text = "Model Collision";
            // 
            // Model_Importer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 454);
            this.Controls.Add(this.resizedCollisionGroup);
            this.Controls.Add(this.groupBox1);
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
            this.Load += new System.EventHandler(this.Model_Importer_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.resizedCollisionGroup.ResumeLayout(false);
            this.resizedCollisionGroup.PerformLayout();
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
        private System.Windows.Forms.CheckedListBox itemMaterialCategories;
        private System.Windows.Forms.CheckBox shouldFlipUV;
        private System.Windows.Forms.CheckBox shouldCreateBoxCollider;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckedListBox itemMaterialCategoriesTrans;
        private System.Windows.Forms.GroupBox resizedCollisionGroup;
        private CheckBox enableTransparency;
    }
}

