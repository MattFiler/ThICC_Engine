namespace EditorTool
{
    partial class Model_Importer_Preselect
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
            this.isMap = new System.Windows.Forms.Button();
            this.isProp = new System.Windows.Forms.Button();
            this.isItem = new System.Windows.Forms.Button();
            this.isPlayer = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // isMap
            // 
            this.isMap.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.isMap.Location = new System.Drawing.Point(12, 12);
            this.isMap.Name = "isMap";
            this.isMap.Size = new System.Drawing.Size(233, 43);
            this.isMap.TabIndex = 0;
            this.isMap.Text = "Map";
            this.isMap.UseVisualStyleBackColor = true;
            this.isMap.Click += new System.EventHandler(this.isMap_Click);
            // 
            // isProp
            // 
            this.isProp.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.isProp.Location = new System.Drawing.Point(12, 159);
            this.isProp.Name = "isProp";
            this.isProp.Size = new System.Drawing.Size(233, 43);
            this.isProp.TabIndex = 1;
            this.isProp.Text = "Prop";
            this.isProp.UseVisualStyleBackColor = true;
            this.isProp.Click += new System.EventHandler(this.isProp_Click);
            // 
            // isItem
            // 
            this.isItem.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.isItem.Location = new System.Drawing.Point(12, 61);
            this.isItem.Name = "isItem";
            this.isItem.Size = new System.Drawing.Size(233, 43);
            this.isItem.TabIndex = 2;
            this.isItem.Text = "Item";
            this.isItem.UseVisualStyleBackColor = true;
            this.isItem.Click += new System.EventHandler(this.isItem_Click);
            // 
            // isPlayer
            // 
            this.isPlayer.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.isPlayer.Location = new System.Drawing.Point(12, 110);
            this.isPlayer.Name = "isPlayer";
            this.isPlayer.Size = new System.Drawing.Size(233, 43);
            this.isPlayer.TabIndex = 3;
            this.isPlayer.Text = "Player";
            this.isPlayer.UseVisualStyleBackColor = true;
            this.isPlayer.Click += new System.EventHandler(this.isPlayer_Click);
            // 
            // Model_Importer_Preselect
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(258, 213);
            this.Controls.Add(this.isPlayer);
            this.Controls.Add(this.isItem);
            this.Controls.Add(this.isProp);
            this.Controls.Add(this.isMap);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Model_Importer_Preselect";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Import as...";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button isMap;
        private System.Windows.Forms.Button isProp;
        private System.Windows.Forms.Button isItem;
        private System.Windows.Forms.Button isPlayer;
    }
}