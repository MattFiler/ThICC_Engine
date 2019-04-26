namespace EditorTool
{
    partial class Game_Config_Character
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.turnSpeedPreview = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.turningSpeed = new System.Windows.Forms.TrackBar();
            this.movementSpeedPreview = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.movementSpeed = new System.Windows.Forms.TrackBar();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.driftBoostPreview = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.driftBoost = new System.Windows.Forms.TrackBar();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.maxDriftPreview = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.driftTime = new System.Windows.Forms.TrackBar();
            this.maxTurnPreview = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.turnTime = new System.Windows.Forms.TrackBar();
            this.saveConfig = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.turningSpeed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.movementSpeed)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.driftBoost)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.driftTime)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.turnTime)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.turnSpeedPreview);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.turningSpeed);
            this.groupBox1.Controls.Add(this.movementSpeedPreview);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.movementSpeed);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(353, 151);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Speed Modifiers";
            // 
            // turnSpeedPreview
            // 
            this.turnSpeedPreview.AutoSize = true;
            this.turnSpeedPreview.Location = new System.Drawing.Point(323, 84);
            this.turnSpeedPreview.Name = "turnSpeedPreview";
            this.turnSpeedPreview.Size = new System.Drawing.Size(19, 13);
            this.turnSpeedPreview.TabIndex = 5;
            this.turnSpeedPreview.Text = "65";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 84);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "Turning Speed";
            // 
            // turningSpeed
            // 
            this.turningSpeed.Location = new System.Drawing.Point(6, 100);
            this.turningSpeed.Maximum = 100;
            this.turningSpeed.Minimum = 20;
            this.turningSpeed.Name = "turningSpeed";
            this.turningSpeed.Size = new System.Drawing.Size(341, 45);
            this.turningSpeed.SmallChange = 5;
            this.turningSpeed.TabIndex = 3;
            this.turningSpeed.Value = 20;
            this.turningSpeed.Scroll += new System.EventHandler(this.turningSpeed_Scroll);
            // 
            // movementSpeedPreview
            // 
            this.movementSpeedPreview.AutoSize = true;
            this.movementSpeedPreview.Location = new System.Drawing.Point(323, 26);
            this.movementSpeedPreview.Name = "movementSpeedPreview";
            this.movementSpeedPreview.Size = new System.Drawing.Size(19, 13);
            this.movementSpeedPreview.TabIndex = 2;
            this.movementSpeedPreview.Text = "50";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Movement Speed";
            // 
            // movementSpeed
            // 
            this.movementSpeed.Location = new System.Drawing.Point(6, 42);
            this.movementSpeed.Maximum = 100;
            this.movementSpeed.Minimum = 20;
            this.movementSpeed.Name = "movementSpeed";
            this.movementSpeed.Size = new System.Drawing.Size(341, 45);
            this.movementSpeed.SmallChange = 5;
            this.movementSpeed.TabIndex = 0;
            this.movementSpeed.Value = 20;
            this.movementSpeed.Scroll += new System.EventHandler(this.movementSpeed_Scroll);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.driftBoostPreview);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.driftBoost);
            this.groupBox2.Location = new System.Drawing.Point(12, 169);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(353, 96);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Multipliers";
            // 
            // driftBoostPreview
            // 
            this.driftBoostPreview.AutoSize = true;
            this.driftBoostPreview.Location = new System.Drawing.Point(323, 26);
            this.driftBoostPreview.Name = "driftBoostPreview";
            this.driftBoostPreview.Size = new System.Drawing.Size(22, 13);
            this.driftBoostPreview.TabIndex = 2;
            this.driftBoostPreview.Text = "0.3";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 26);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(100, 13);
            this.label8.TabIndex = 1;
            this.label8.Text = "Drift Boost Multiplier";
            // 
            // driftBoost
            // 
            this.driftBoost.LargeChange = 1;
            this.driftBoost.Location = new System.Drawing.Point(6, 42);
            this.driftBoost.Name = "driftBoost";
            this.driftBoost.Size = new System.Drawing.Size(341, 45);
            this.driftBoost.TabIndex = 0;
            this.driftBoost.Scroll += new System.EventHandler(this.driftBoost_Scroll);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.maxDriftPreview);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.driftTime);
            this.groupBox3.Controls.Add(this.maxTurnPreview);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.turnTime);
            this.groupBox3.Location = new System.Drawing.Point(12, 271);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(353, 155);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Time Limits";
            // 
            // maxDriftPreview
            // 
            this.maxDriftPreview.AutoSize = true;
            this.maxDriftPreview.Location = new System.Drawing.Point(329, 86);
            this.maxDriftPreview.Name = "maxDriftPreview";
            this.maxDriftPreview.Size = new System.Drawing.Size(13, 13);
            this.maxDriftPreview.TabIndex = 5;
            this.maxDriftPreview.Text = "4";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 86);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(90, 13);
            this.label10.TabIndex = 4;
            this.label10.Text = "Time for Max Drift";
            // 
            // driftTime
            // 
            this.driftTime.LargeChange = 1;
            this.driftTime.Location = new System.Drawing.Point(6, 102);
            this.driftTime.Name = "driftTime";
            this.driftTime.Size = new System.Drawing.Size(341, 45);
            this.driftTime.TabIndex = 3;
            this.driftTime.Scroll += new System.EventHandler(this.driftTime_Scroll);
            // 
            // maxTurnPreview
            // 
            this.maxTurnPreview.AutoSize = true;
            this.maxTurnPreview.Location = new System.Drawing.Point(329, 28);
            this.maxTurnPreview.Name = "maxTurnPreview";
            this.maxTurnPreview.Size = new System.Drawing.Size(13, 13);
            this.maxTurnPreview.TabIndex = 2;
            this.maxTurnPreview.Text = "5";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 28);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(93, 13);
            this.label6.TabIndex = 1;
            this.label6.Text = "Time for Max Turn";
            // 
            // turnTime
            // 
            this.turnTime.LargeChange = 1;
            this.turnTime.Location = new System.Drawing.Point(6, 44);
            this.turnTime.Name = "turnTime";
            this.turnTime.Size = new System.Drawing.Size(341, 45);
            this.turnTime.TabIndex = 0;
            this.turnTime.Scroll += new System.EventHandler(this.turnTime_Scroll);
            // 
            // saveConfig
            // 
            this.saveConfig.Location = new System.Drawing.Point(12, 432);
            this.saveConfig.Name = "saveConfig";
            this.saveConfig.Size = new System.Drawing.Size(353, 23);
            this.saveConfig.TabIndex = 8;
            this.saveConfig.Text = "Save";
            this.saveConfig.UseVisualStyleBackColor = true;
            this.saveConfig.Click += new System.EventHandler(this.saveConfig_Click);
            // 
            // Game_Config_Character
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(377, 465);
            this.Controls.Add(this.saveConfig);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Game_Config_Character";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Character Config Editor";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.turningSpeed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.movementSpeed)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.driftBoost)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.driftTime)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.turnTime)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label turnSpeedPreview;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar turningSpeed;
        private System.Windows.Forms.Label movementSpeedPreview;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar movementSpeed;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label driftBoostPreview;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TrackBar driftBoost;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label maxDriftPreview;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TrackBar driftTime;
        private System.Windows.Forms.Label maxTurnPreview;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TrackBar turnTime;
        private System.Windows.Forms.Button saveConfig;
    }
}