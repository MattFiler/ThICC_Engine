namespace EditorTool
{
    partial class ItemAssetEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ItemAssetEditor));
            this.loadSoundEffect = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.soundEffect = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.loadModel = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.model = new System.Windows.Forms.TextBox();
            this.loadMenuImage = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.modelImage = new System.Windows.Forms.TextBox();
            this.itemSelect = new System.Windows.Forms.ComboBox();
            this.saveConfig = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // loadSoundEffect
            // 
            this.loadSoundEffect.Location = new System.Drawing.Point(244, 113);
            this.loadSoundEffect.Name = "loadSoundEffect";
            this.loadSoundEffect.Size = new System.Drawing.Size(75, 23);
            this.loadSoundEffect.TabIndex = 14;
            this.loadSoundEffect.Text = "Load";
            this.loadSoundEffect.UseVisualStyleBackColor = true;
            this.loadSoundEffect.Click += new System.EventHandler(this.loadSoundEffect_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 98);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 13);
            this.label2.TabIndex = 13;
            this.label2.Text = "Sound Effect";
            // 
            // soundEffect
            // 
            this.soundEffect.Location = new System.Drawing.Point(9, 114);
            this.soundEffect.Name = "soundEffect";
            this.soundEffect.ReadOnly = true;
            this.soundEffect.Size = new System.Drawing.Size(229, 20);
            this.soundEffect.TabIndex = 12;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.loadSoundEffect);
            this.groupBox2.Controls.Add(this.loadModel);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.soundEffect);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.model);
            this.groupBox2.Controls.Add(this.loadMenuImage);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.modelImage);
            this.groupBox2.Location = new System.Drawing.Point(12, 39);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(326, 144);
            this.groupBox2.TabIndex = 18;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Item Assets";
            // 
            // loadModel
            // 
            this.loadModel.Location = new System.Drawing.Point(244, 74);
            this.loadModel.Name = "loadModel";
            this.loadModel.Size = new System.Drawing.Size(75, 23);
            this.loadModel.TabIndex = 17;
            this.loadModel.Text = "Load";
            this.loadModel.UseVisualStyleBackColor = true;
            this.loadModel.Click += new System.EventHandler(this.loadModel_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 59);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(36, 13);
            this.label3.TabIndex = 16;
            this.label3.Text = "Model";
            // 
            // model
            // 
            this.model.Location = new System.Drawing.Point(9, 75);
            this.model.Name = "model";
            this.model.ReadOnly = true;
            this.model.Size = new System.Drawing.Size(229, 20);
            this.model.TabIndex = 15;
            // 
            // loadMenuImage
            // 
            this.loadMenuImage.Location = new System.Drawing.Point(244, 35);
            this.loadMenuImage.Name = "loadMenuImage";
            this.loadMenuImage.Size = new System.Drawing.Size(75, 23);
            this.loadMenuImage.TabIndex = 14;
            this.loadMenuImage.Text = "Load";
            this.loadMenuImage.UseVisualStyleBackColor = true;
            this.loadMenuImage.Click += new System.EventHandler(this.loadMenuImage_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 20);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(66, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "Menu Image";
            // 
            // modelImage
            // 
            this.modelImage.Location = new System.Drawing.Point(9, 36);
            this.modelImage.Name = "modelImage";
            this.modelImage.ReadOnly = true;
            this.modelImage.Size = new System.Drawing.Size(229, 20);
            this.modelImage.TabIndex = 12;
            // 
            // itemSelect
            // 
            this.itemSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.itemSelect.FormattingEnabled = true;
            this.itemSelect.Items.AddRange(new object[] {
            "GREEN_SHELL",
            "RED_SHELL",
            "BANANA",
            "FAKE_BOX",
            "MUSHROOM",
            "MUSHROOM_3X",
            "BOMB",
            "STAR",
            "MUSHROOM_UNLIMITED",
            "MUSHROOM_GIANT",
            "BULLET_BILL",
            "GREEN_SHELL_3X",
            "BANANA_3X"});
            this.itemSelect.Location = new System.Drawing.Point(12, 12);
            this.itemSelect.Name = "itemSelect";
            this.itemSelect.Size = new System.Drawing.Size(326, 21);
            this.itemSelect.TabIndex = 19;
            this.itemSelect.SelectedIndexChanged += new System.EventHandler(this.itemSelect_SelectedIndexChanged);
            // 
            // saveConfig
            // 
            this.saveConfig.Location = new System.Drawing.Point(12, 189);
            this.saveConfig.Name = "saveConfig";
            this.saveConfig.Size = new System.Drawing.Size(326, 23);
            this.saveConfig.TabIndex = 20;
            this.saveConfig.Text = "Save";
            this.saveConfig.UseVisualStyleBackColor = true;
            this.saveConfig.Click += new System.EventHandler(this.saveConfig_Click);
            // 
            // ItemAssetEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(348, 220);
            this.Controls.Add(this.saveConfig);
            this.Controls.Add(this.itemSelect);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ItemAssetEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Item Asset Editor";
            this.Load += new System.EventHandler(this.ItemAssetEditor_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Button loadSoundEffect;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox soundEffect;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button loadModel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox model;
        private System.Windows.Forms.Button loadMenuImage;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox modelImage;
        private System.Windows.Forms.ComboBox itemSelect;
        private System.Windows.Forms.Button saveConfig;
    }
}