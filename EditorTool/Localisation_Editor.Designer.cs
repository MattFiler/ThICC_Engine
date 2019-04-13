namespace EditorTool
{
    partial class Localisation_Editor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Localisation_Editor));
            this.label2 = new System.Windows.Forms.Label();
            this.stringIdentifier = new System.Windows.Forms.TextBox();
            this.createNewString = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.stringContent = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 13);
            this.label2.TabIndex = 15;
            this.label2.Text = "String Identifier";
            // 
            // stringIdentifier
            // 
            this.stringIdentifier.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.stringIdentifier.Location = new System.Drawing.Point(15, 25);
            this.stringIdentifier.Name = "stringIdentifier";
            this.stringIdentifier.Size = new System.Drawing.Size(264, 20);
            this.stringIdentifier.TabIndex = 14;
            // 
            // createNewString
            // 
            this.createNewString.Location = new System.Drawing.Point(15, 141);
            this.createNewString.Name = "createNewString";
            this.createNewString.Size = new System.Drawing.Size(264, 28);
            this.createNewString.TabIndex = 13;
            this.createNewString.Text = "Create";
            this.createNewString.UseVisualStyleBackColor = true;
            this.createNewString.Click += new System.EventHandler(this.createNewString_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 48);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(74, 13);
            this.label1.TabIndex = 12;
            this.label1.Text = "String Content";
            // 
            // stringContent
            // 
            this.stringContent.Location = new System.Drawing.Point(15, 64);
            this.stringContent.Multiline = true;
            this.stringContent.Name = "stringContent";
            this.stringContent.Size = new System.Drawing.Size(264, 71);
            this.stringContent.TabIndex = 11;
            // 
            // Localisation_Editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(294, 181);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.stringIdentifier);
            this.Controls.Add(this.createNewString);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.stringContent);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Localisation_Editor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "New String";
            this.Load += new System.EventHandler(this.Localisation_Editor_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox stringIdentifier;
        private System.Windows.Forms.Button createNewString;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox stringContent;
    }
}