namespace EditorTool
{
    partial class Keybind_Editor
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
            this.label1 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.saveKeybinds = new System.Windows.Forms.Button();
            this.addInput = new System.Windows.Forms.Button();
            this.removeInput = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(261, 54);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 13);
            this.label1.TabIndex = 48;
            this.label1.Text = "Input";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(9, 54);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(79, 13);
            this.label5.TabIndex = 46;
            this.label5.Text = "Action Name";
            // 
            // saveKeybinds
            // 
            this.saveKeybinds.Location = new System.Drawing.Point(12, 134);
            this.saveKeybinds.Name = "saveKeybinds";
            this.saveKeybinds.Size = new System.Drawing.Size(410, 26);
            this.saveKeybinds.TabIndex = 42;
            this.saveKeybinds.Text = "Save";
            this.saveKeybinds.UseVisualStyleBackColor = true;
            this.saveKeybinds.Click += new System.EventHandler(this.saveKeybinds_Click);
            // 
            // addInput
            // 
            this.addInput.Location = new System.Drawing.Point(140, 12);
            this.addInput.Name = "addInput";
            this.addInput.Size = new System.Drawing.Size(138, 26);
            this.addInput.TabIndex = 50;
            this.addInput.Text = "Add Input";
            this.addInput.UseVisualStyleBackColor = true;
            this.addInput.Click += new System.EventHandler(this.button1_Click);
            // 
            // removeInput
            // 
            this.removeInput.Location = new System.Drawing.Point(284, 12);
            this.removeInput.Name = "removeInput";
            this.removeInput.Size = new System.Drawing.Size(138, 26);
            this.removeInput.TabIndex = 51;
            this.removeInput.Text = "Remove Input";
            this.removeInput.UseVisualStyleBackColor = true;
            this.removeInput.Click += new System.EventHandler(this.button2_Click);
            // 
            // Keybind_Editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(433, 169);
            this.Controls.Add(this.removeInput);
            this.Controls.Add(this.addInput);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.saveKeybinds);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Keybind_Editor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Keybind Editor";
            this.Load += new System.EventHandler(this.Keybind_Editor_Load);
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion
		
		private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button saveKeybinds;
        private System.Windows.Forms.Button addInput;
        private System.Windows.Forms.Button removeInput;
    }
}