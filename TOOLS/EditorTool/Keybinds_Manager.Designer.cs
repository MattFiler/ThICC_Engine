namespace EditorTool
{
    partial class Keybinds_Manager
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Keybinds_Manager));
            this.editKeybind = new System.Windows.Forms.Button();
            this.deleteKeybind = new System.Windows.Forms.Button();
            this.newKeybind = new System.Windows.Forms.Button();
            this.assetList = new System.Windows.Forms.ListBox();
            this.keybindPreview = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.kbPreview = new System.Windows.Forms.Label();
            this.gpPreview = new System.Windows.Forms.Label();
            this.arPreview = new System.Windows.Forms.Label();
            this.isDebug = new System.Windows.Forms.CheckBox();
            this.keybindPreview.SuspendLayout();
            this.SuspendLayout();
            // 
            // editKeybind
            // 
            this.editKeybind.Location = new System.Drawing.Point(220, 142);
            this.editKeybind.Name = "editKeybind";
            this.editKeybind.Size = new System.Drawing.Size(253, 25);
            this.editKeybind.TabIndex = 12;
            this.editKeybind.Text = "Edit Selected Keybind";
            this.editKeybind.UseVisualStyleBackColor = true;
            this.editKeybind.Click += new System.EventHandler(this.editKeybind_Click);
            // 
            // deleteKeybind
            // 
            this.deleteKeybind.Location = new System.Drawing.Point(220, 173);
            this.deleteKeybind.Name = "deleteKeybind";
            this.deleteKeybind.Size = new System.Drawing.Size(253, 25);
            this.deleteKeybind.TabIndex = 11;
            this.deleteKeybind.Text = "Delete Selected Keybind";
            this.deleteKeybind.UseVisualStyleBackColor = true;
            this.deleteKeybind.Click += new System.EventHandler(this.deleteKeybind_Click);
            // 
            // newKeybind
            // 
            this.newKeybind.Location = new System.Drawing.Point(220, 111);
            this.newKeybind.Name = "newKeybind";
            this.newKeybind.Size = new System.Drawing.Size(253, 25);
            this.newKeybind.TabIndex = 10;
            this.newKeybind.Text = "Create New Keybind";
            this.newKeybind.UseVisualStyleBackColor = true;
            this.newKeybind.Click += new System.EventHandler(this.newKeybind_Click);
            // 
            // assetList
            // 
            this.assetList.FormattingEnabled = true;
            this.assetList.Location = new System.Drawing.Point(12, 12);
            this.assetList.Name = "assetList";
            this.assetList.Size = new System.Drawing.Size(202, 186);
            this.assetList.TabIndex = 9;
            this.assetList.SelectedIndexChanged += new System.EventHandler(this.assetList_SelectedIndexChanged);
            // 
            // keybindPreview
            // 
            this.keybindPreview.Controls.Add(this.isDebug);
            this.keybindPreview.Controls.Add(this.arPreview);
            this.keybindPreview.Controls.Add(this.gpPreview);
            this.keybindPreview.Controls.Add(this.kbPreview);
            this.keybindPreview.Controls.Add(this.label3);
            this.keybindPreview.Controls.Add(this.label2);
            this.keybindPreview.Controls.Add(this.label1);
            this.keybindPreview.Location = new System.Drawing.Point(220, 6);
            this.keybindPreview.Name = "keybindPreview";
            this.keybindPreview.Size = new System.Drawing.Size(253, 99);
            this.keybindPreview.TabIndex = 13;
            this.keybindPreview.TabStop = false;
            this.keybindPreview.Text = "Current Setting";
            this.keybindPreview.Visible = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 54);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Arcade: ";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Gamepad: ";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(55, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Keyboard:";
            // 
            // kbPreview
            // 
            this.kbPreview.AutoSize = true;
            this.kbPreview.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.kbPreview.Location = new System.Drawing.Point(67, 20);
            this.kbPreview.Name = "kbPreview";
            this.kbPreview.Size = new System.Drawing.Size(34, 13);
            this.kbPreview.TabIndex = 3;
            this.kbPreview.Text = "temp";
            // 
            // gpPreview
            // 
            this.gpPreview.AutoSize = true;
            this.gpPreview.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gpPreview.Location = new System.Drawing.Point(71, 37);
            this.gpPreview.Name = "gpPreview";
            this.gpPreview.Size = new System.Drawing.Size(34, 13);
            this.gpPreview.TabIndex = 4;
            this.gpPreview.Text = "temp";
            // 
            // arPreview
            // 
            this.arPreview.AutoSize = true;
            this.arPreview.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.arPreview.Location = new System.Drawing.Point(59, 54);
            this.arPreview.Name = "arPreview";
            this.arPreview.Size = new System.Drawing.Size(34, 13);
            this.arPreview.TabIndex = 5;
            this.arPreview.Text = "temp";
            // 
            // isDebug
            // 
            this.isDebug.AutoSize = true;
            this.isDebug.Location = new System.Drawing.Point(9, 73);
            this.isDebug.Name = "isDebug";
            this.isDebug.Size = new System.Drawing.Size(82, 17);
            this.isDebug.TabIndex = 6;
            this.isDebug.Text = "Debug Only";
            this.isDebug.UseVisualStyleBackColor = true;
            // 
            // Keybinds_Manager
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(482, 209);
            this.Controls.Add(this.keybindPreview);
            this.Controls.Add(this.editKeybind);
            this.Controls.Add(this.deleteKeybind);
            this.Controls.Add(this.newKeybind);
            this.Controls.Add(this.assetList);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Keybinds_Manager";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Keybinds";
            this.Load += new System.EventHandler(this.Keybind_Editor_New_Load);
            this.keybindPreview.ResumeLayout(false);
            this.keybindPreview.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button editKeybind;
        private System.Windows.Forms.Button deleteKeybind;
        private System.Windows.Forms.Button newKeybind;
        private System.Windows.Forms.ListBox assetList;
        private System.Windows.Forms.GroupBox keybindPreview;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label arPreview;
        private System.Windows.Forms.Label gpPreview;
        private System.Windows.Forms.Label kbPreview;
        private System.Windows.Forms.CheckBox isDebug;
    }
}