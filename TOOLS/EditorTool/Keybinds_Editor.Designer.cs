namespace EditorTool
{
    partial class Keybinds_Editor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Keybinds_Editor));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.arcadeInput = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.gamepadInput = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.keyboardInput = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.keybindName = new System.Windows.Forms.TextBox();
            this.createKeybind = new System.Windows.Forms.Button();
            this.isDebug = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.arcadeInput);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.gamepadInput);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.keyboardInput);
            this.groupBox1.Location = new System.Drawing.Point(12, 103);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(278, 145);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Inputs";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 99);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(68, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Arcade Input";
            // 
            // arcadeInput
            // 
            this.arcadeInput.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.arcadeInput.FormattingEnabled = true;
            this.arcadeInput.Location = new System.Drawing.Point(9, 115);
            this.arcadeInput.Name = "arcadeInput";
            this.arcadeInput.Size = new System.Drawing.Size(259, 21);
            this.arcadeInput.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 59);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(80, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Gamepad Input";
            // 
            // gamepadInput
            // 
            this.gamepadInput.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.gamepadInput.FormattingEnabled = true;
            this.gamepadInput.Location = new System.Drawing.Point(9, 75);
            this.gamepadInput.Name = "gamepadInput";
            this.gamepadInput.Size = new System.Drawing.Size(259, 21);
            this.gamepadInput.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 19);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Keyboard Input";
            // 
            // keyboardInput
            // 
            this.keyboardInput.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.keyboardInput.FormattingEnabled = true;
            this.keyboardInput.Location = new System.Drawing.Point(9, 35);
            this.keyboardInput.Name = "keyboardInput";
            this.keyboardInput.Size = new System.Drawing.Size(259, 21);
            this.keyboardInput.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(76, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Keybind Name";
            // 
            // keybindName
            // 
            this.keybindName.Location = new System.Drawing.Point(9, 36);
            this.keybindName.Name = "keybindName";
            this.keybindName.Size = new System.Drawing.Size(259, 20);
            this.keybindName.TabIndex = 0;
            // 
            // createKeybind
            // 
            this.createKeybind.Location = new System.Drawing.Point(12, 254);
            this.createKeybind.Name = "createKeybind";
            this.createKeybind.Size = new System.Drawing.Size(279, 23);
            this.createKeybind.TabIndex = 1;
            this.createKeybind.Text = "Create";
            this.createKeybind.UseVisualStyleBackColor = true;
            this.createKeybind.Click += new System.EventHandler(this.button1_Click);
            // 
            // isDebug
            // 
            this.isDebug.AutoSize = true;
            this.isDebug.Location = new System.Drawing.Point(9, 62);
            this.isDebug.Name = "isDebug";
            this.isDebug.Size = new System.Drawing.Size(110, 17);
            this.isDebug.TabIndex = 8;
            this.isDebug.Text = "Is Debug Keybind";
            this.isDebug.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.keybindName);
            this.groupBox2.Controls.Add(this.isDebug);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(278, 85);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Keybind Info";
            // 
            // Keybinds_Editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(303, 287);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.createKeybind);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Keybinds_Editor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Keybind Editor";
            this.Load += new System.EventHandler(this.Keybinds_Editor_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox arcadeInput;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox gamepadInput;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox keyboardInput;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox keybindName;
        private System.Windows.Forms.Button createKeybind;
        private System.Windows.Forms.CheckBox isDebug;
        private System.Windows.Forms.GroupBox groupBox2;
    }
}