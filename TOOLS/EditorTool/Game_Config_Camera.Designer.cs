namespace EditorTool
{
    partial class Game_Config_Camera
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
            this.loadMenuLoop = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.farClip = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.nearClip = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.rotationSpeed = new System.Windows.Forms.NumericUpDown();
            this.label10 = new System.Windows.Forms.Label();
            this.movementSpeed = new System.Windows.Forms.NumericUpDown();
            this.label11 = new System.Windows.Forms.Label();
            this.timeOut = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.spinAmount = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.posLerp = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.rotLerp = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.fovPrev = new System.Windows.Forms.Label();
            this.fov = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.lookatX = new System.Windows.Forms.NumericUpDown();
            this.lookatY = new System.Windows.Forms.NumericUpDown();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.lookatZ = new System.Windows.Forms.NumericUpDown();
            this.label15 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label16 = new System.Windows.Forms.Label();
            this.targetX = new System.Windows.Forms.NumericUpDown();
            this.label17 = new System.Windows.Forms.Label();
            this.targetY = new System.Windows.Forms.NumericUpDown();
            this.label18 = new System.Windows.Forms.Label();
            this.targetZ = new System.Windows.Forms.NumericUpDown();
            this.label19 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label28 = new System.Windows.Forms.Label();
            this.lookOffsetX = new System.Windows.Forms.NumericUpDown();
            this.label29 = new System.Windows.Forms.Label();
            this.lookOffsetY = new System.Windows.Forms.NumericUpDown();
            this.label30 = new System.Windows.Forms.Label();
            this.lookOffsetZ = new System.Windows.Forms.NumericUpDown();
            this.label31 = new System.Windows.Forms.Label();
            this.label32 = new System.Windows.Forms.Label();
            this.camOffX = new System.Windows.Forms.NumericUpDown();
            this.label33 = new System.Windows.Forms.Label();
            this.camOffY = new System.Windows.Forms.NumericUpDown();
            this.label34 = new System.Windows.Forms.Label();
            this.camOffZ = new System.Windows.Forms.NumericUpDown();
            this.label35 = new System.Windows.Forms.Label();
            this.saveConfig = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.farClip)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nearClip)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.rotationSpeed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.movementSpeed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.timeOut)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.spinAmount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.posLerp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.rotLerp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fov)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookatX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookatY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookatZ)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.targetX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.targetY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.targetZ)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.lookOffsetX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookOffsetY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookOffsetZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.camOffX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.camOffY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.camOffZ)).BeginInit();
            this.SuspendLayout();
            // 
            // loadMenuLoop
            // 
            this.loadMenuLoop.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.loadMenuLoop.FormattingEnabled = true;
            this.loadMenuLoop.Items.AddRange(new object[] {
            "FOLLOW",
            "ORBIT",
            "CINEMATIC",
            "FIRST",
            "INDEPENDENT",
            "BACK_FACING",
            "DEBUG_CAM"});
            this.loadMenuLoop.Location = new System.Drawing.Point(15, 95);
            this.loadMenuLoop.Name = "loadMenuLoop";
            this.loadMenuLoop.Size = new System.Drawing.Size(307, 21);
            this.loadMenuLoop.TabIndex = 1;
            this.loadMenuLoop.SelectedIndexChanged += new System.EventHandler(this.camType_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 79);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Camera Type";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.farClip);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.nearClip);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(15, 11);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(307, 64);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Shared";
            // 
            // farClip
            // 
            this.farClip.Location = new System.Drawing.Point(156, 35);
            this.farClip.Maximum = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            this.farClip.Name = "farClip";
            this.farClip.Size = new System.Drawing.Size(141, 20);
            this.farClip.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(153, 19);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(62, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Far Clipping";
            // 
            // nearClip
            // 
            this.nearClip.Location = new System.Drawing.Point(9, 35);
            this.nearClip.Name = "nearClip";
            this.nearClip.Size = new System.Drawing.Size(141, 20);
            this.nearClip.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 19);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(70, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Near Clipping";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.rotationSpeed);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.movementSpeed);
            this.groupBox2.Controls.Add(this.label11);
            this.groupBox2.Controls.Add(this.timeOut);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.spinAmount);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.posLerp);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.rotLerp);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.fovPrev);
            this.groupBox2.Controls.Add(this.fov);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Location = new System.Drawing.Point(15, 122);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(307, 202);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "General";
            // 
            // rotationSpeed
            // 
            this.rotationSpeed.DecimalPlaces = 2;
            this.rotationSpeed.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.rotationSpeed.Location = new System.Drawing.Point(156, 174);
            this.rotationSpeed.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.rotationSpeed.Name = "rotationSpeed";
            this.rotationSpeed.Size = new System.Drawing.Size(141, 20);
            this.rotationSpeed.TabIndex = 14;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(153, 158);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(81, 13);
            this.label10.TabIndex = 13;
            this.label10.Text = "Rotation Speed";
            // 
            // movementSpeed
            // 
            this.movementSpeed.DecimalPlaces = 2;
            this.movementSpeed.Location = new System.Drawing.Point(9, 174);
            this.movementSpeed.Name = "movementSpeed";
            this.movementSpeed.Size = new System.Drawing.Size(141, 20);
            this.movementSpeed.TabIndex = 12;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(6, 158);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(91, 13);
            this.label11.TabIndex = 11;
            this.label11.Text = "Movement Speed";
            // 
            // timeOut
            // 
            this.timeOut.Location = new System.Drawing.Point(156, 135);
            this.timeOut.Maximum = new decimal(new int[] {
            50,
            0,
            0,
            0});
            this.timeOut.Name = "timeOut";
            this.timeOut.Size = new System.Drawing.Size(141, 20);
            this.timeOut.TabIndex = 10;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(153, 119);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(45, 13);
            this.label9.TabIndex = 9;
            this.label9.Text = "Timeout";
            // 
            // spinAmount
            // 
            this.spinAmount.Location = new System.Drawing.Point(9, 135);
            this.spinAmount.Maximum = new decimal(new int[] {
            50,
            0,
            0,
            0});
            this.spinAmount.Name = "spinAmount";
            this.spinAmount.Size = new System.Drawing.Size(141, 20);
            this.spinAmount.TabIndex = 8;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 119);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(67, 13);
            this.label8.TabIndex = 7;
            this.label8.Text = "Spin Amount";
            // 
            // posLerp
            // 
            this.posLerp.DecimalPlaces = 2;
            this.posLerp.Increment = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.posLerp.Location = new System.Drawing.Point(156, 96);
            this.posLerp.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.posLerp.Name = "posLerp";
            this.posLerp.Size = new System.Drawing.Size(141, 20);
            this.posLerp.TabIndex = 6;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(153, 80);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(68, 13);
            this.label7.TabIndex = 5;
            this.label7.Text = "Position Lerp";
            // 
            // rotLerp
            // 
            this.rotLerp.DecimalPlaces = 2;
            this.rotLerp.Increment = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.rotLerp.Location = new System.Drawing.Point(9, 96);
            this.rotLerp.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.rotLerp.Name = "rotLerp";
            this.rotLerp.Size = new System.Drawing.Size(141, 20);
            this.rotLerp.TabIndex = 4;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 80);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(71, 13);
            this.label6.TabIndex = 3;
            this.label6.Text = "Rotation Lerp";
            // 
            // fovPrev
            // 
            this.fovPrev.AutoSize = true;
            this.fovPrev.Location = new System.Drawing.Point(282, 24);
            this.fovPrev.Name = "fovPrev";
            this.fovPrev.Size = new System.Drawing.Size(19, 13);
            this.fovPrev.TabIndex = 2;
            this.fovPrev.Text = "90";
            // 
            // fov
            // 
            this.fov.Location = new System.Drawing.Point(6, 40);
            this.fov.Maximum = 130;
            this.fov.Minimum = 30;
            this.fov.Name = "fov";
            this.fov.Size = new System.Drawing.Size(295, 45);
            this.fov.TabIndex = 1;
            this.fov.Value = 30;
            this.fov.Scroll += new System.EventHandler(this.fov_Scroll);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 24);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(28, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "FOV";
            // 
            // lookatX
            // 
            this.lookatX.DecimalPlaces = 1;
            this.lookatX.Location = new System.Drawing.Point(43, 37);
            this.lookatX.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.lookatX.Name = "lookatX";
            this.lookatX.Size = new System.Drawing.Size(58, 20);
            this.lookatX.TabIndex = 15;
            // 
            // lookatY
            // 
            this.lookatY.DecimalPlaces = 1;
            this.lookatY.Location = new System.Drawing.Point(126, 37);
            this.lookatY.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.lookatY.Name = "lookatY";
            this.lookatY.Size = new System.Drawing.Size(58, 20);
            this.lookatY.TabIndex = 16;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(25, 40);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(17, 13);
            this.label12.TabIndex = 18;
            this.label12.Text = "X:";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(107, 40);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(17, 13);
            this.label13.TabIndex = 19;
            this.label13.Text = "Y:";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(190, 40);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(17, 13);
            this.label14.TabIndex = 21;
            this.label14.Text = "Z:";
            // 
            // lookatZ
            // 
            this.lookatZ.DecimalPlaces = 1;
            this.lookatZ.Location = new System.Drawing.Point(209, 37);
            this.lookatZ.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.lookatZ.Name = "lookatZ";
            this.lookatZ.Size = new System.Drawing.Size(58, 20);
            this.lookatZ.TabIndex = 20;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(25, 21);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(84, 13);
            this.label15.TabIndex = 22;
            this.label15.Text = "Look At Position";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label16);
            this.groupBox3.Controls.Add(this.targetX);
            this.groupBox3.Controls.Add(this.label17);
            this.groupBox3.Controls.Add(this.targetY);
            this.groupBox3.Controls.Add(this.label18);
            this.groupBox3.Controls.Add(this.targetZ);
            this.groupBox3.Controls.Add(this.label19);
            this.groupBox3.Controls.Add(this.label15);
            this.groupBox3.Controls.Add(this.lookatX);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.lookatY);
            this.groupBox3.Controls.Add(this.label12);
            this.groupBox3.Controls.Add(this.lookatZ);
            this.groupBox3.Controls.Add(this.label13);
            this.groupBox3.Location = new System.Drawing.Point(15, 330);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(307, 111);
            this.groupBox3.TabIndex = 23;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Positions";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(25, 60);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(78, 13);
            this.label16.TabIndex = 29;
            this.label16.Text = "Target Position";
            // 
            // targetX
            // 
            this.targetX.DecimalPlaces = 1;
            this.targetX.Location = new System.Drawing.Point(43, 76);
            this.targetX.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.targetX.Name = "targetX";
            this.targetX.Size = new System.Drawing.Size(58, 20);
            this.targetX.TabIndex = 23;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(190, 79);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(17, 13);
            this.label17.TabIndex = 28;
            this.label17.Text = "Z:";
            // 
            // targetY
            // 
            this.targetY.DecimalPlaces = 1;
            this.targetY.Location = new System.Drawing.Point(126, 76);
            this.targetY.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.targetY.Name = "targetY";
            this.targetY.Size = new System.Drawing.Size(58, 20);
            this.targetY.TabIndex = 24;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(25, 79);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(17, 13);
            this.label18.TabIndex = 25;
            this.label18.Text = "X:";
            // 
            // targetZ
            // 
            this.targetZ.DecimalPlaces = 1;
            this.targetZ.Location = new System.Drawing.Point(209, 76);
            this.targetZ.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.targetZ.Name = "targetZ";
            this.targetZ.Size = new System.Drawing.Size(58, 20);
            this.targetZ.TabIndex = 27;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(107, 79);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(17, 13);
            this.label19.TabIndex = 26;
            this.label19.Text = "Y:";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label28);
            this.groupBox4.Controls.Add(this.lookOffsetX);
            this.groupBox4.Controls.Add(this.label29);
            this.groupBox4.Controls.Add(this.lookOffsetY);
            this.groupBox4.Controls.Add(this.label30);
            this.groupBox4.Controls.Add(this.lookOffsetZ);
            this.groupBox4.Controls.Add(this.label31);
            this.groupBox4.Controls.Add(this.label32);
            this.groupBox4.Controls.Add(this.camOffX);
            this.groupBox4.Controls.Add(this.label33);
            this.groupBox4.Controls.Add(this.camOffY);
            this.groupBox4.Controls.Add(this.label34);
            this.groupBox4.Controls.Add(this.camOffZ);
            this.groupBox4.Controls.Add(this.label35);
            this.groupBox4.Location = new System.Drawing.Point(15, 447);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(307, 111);
            this.groupBox4.TabIndex = 30;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Offsets";
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(25, 60);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(75, 13);
            this.label28.TabIndex = 29;
            this.label28.Text = "Look At Offset";
            // 
            // lookOffsetX
            // 
            this.lookOffsetX.DecimalPlaces = 1;
            this.lookOffsetX.Location = new System.Drawing.Point(43, 76);
            this.lookOffsetX.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.lookOffsetX.Name = "lookOffsetX";
            this.lookOffsetX.Size = new System.Drawing.Size(58, 20);
            this.lookOffsetX.TabIndex = 23;
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(190, 79);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(17, 13);
            this.label29.TabIndex = 28;
            this.label29.Text = "Z:";
            // 
            // lookOffsetY
            // 
            this.lookOffsetY.DecimalPlaces = 1;
            this.lookOffsetY.Location = new System.Drawing.Point(126, 76);
            this.lookOffsetY.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.lookOffsetY.Name = "lookOffsetY";
            this.lookOffsetY.Size = new System.Drawing.Size(58, 20);
            this.lookOffsetY.TabIndex = 24;
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(25, 79);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(17, 13);
            this.label30.TabIndex = 25;
            this.label30.Text = "X:";
            // 
            // lookOffsetZ
            // 
            this.lookOffsetZ.DecimalPlaces = 1;
            this.lookOffsetZ.Location = new System.Drawing.Point(209, 76);
            this.lookOffsetZ.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.lookOffsetZ.Name = "lookOffsetZ";
            this.lookOffsetZ.Size = new System.Drawing.Size(58, 20);
            this.lookOffsetZ.TabIndex = 27;
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(107, 79);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(17, 13);
            this.label31.TabIndex = 26;
            this.label31.Text = "Y:";
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(25, 21);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(74, 13);
            this.label32.TabIndex = 22;
            this.label32.Text = "Camera Offset";
            // 
            // camOffX
            // 
            this.camOffX.DecimalPlaces = 1;
            this.camOffX.Location = new System.Drawing.Point(43, 37);
            this.camOffX.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.camOffX.Name = "camOffX";
            this.camOffX.Size = new System.Drawing.Size(58, 20);
            this.camOffX.TabIndex = 15;
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(190, 40);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(17, 13);
            this.label33.TabIndex = 21;
            this.label33.Text = "Z:";
            // 
            // camOffY
            // 
            this.camOffY.DecimalPlaces = 1;
            this.camOffY.Location = new System.Drawing.Point(126, 37);
            this.camOffY.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.camOffY.Name = "camOffY";
            this.camOffY.Size = new System.Drawing.Size(58, 20);
            this.camOffY.TabIndex = 16;
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.Location = new System.Drawing.Point(25, 40);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(17, 13);
            this.label34.TabIndex = 18;
            this.label34.Text = "X:";
            // 
            // camOffZ
            // 
            this.camOffZ.DecimalPlaces = 1;
            this.camOffZ.Location = new System.Drawing.Point(209, 37);
            this.camOffZ.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.camOffZ.Name = "camOffZ";
            this.camOffZ.Size = new System.Drawing.Size(58, 20);
            this.camOffZ.TabIndex = 20;
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Location = new System.Drawing.Point(107, 40);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(17, 13);
            this.label35.TabIndex = 19;
            this.label35.Text = "Y:";
            // 
            // saveConfig
            // 
            this.saveConfig.Location = new System.Drawing.Point(15, 564);
            this.saveConfig.Name = "saveConfig";
            this.saveConfig.Size = new System.Drawing.Size(307, 23);
            this.saveConfig.TabIndex = 31;
            this.saveConfig.Text = "Save";
            this.saveConfig.UseVisualStyleBackColor = true;
            this.saveConfig.Click += new System.EventHandler(this.saveConfig_Click);
            // 
            // Game_Config_Camera
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(337, 596);
            this.Controls.Add(this.saveConfig);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.loadMenuLoop);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Game_Config_Camera";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Camera Config Editor";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.farClip)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nearClip)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.rotationSpeed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.movementSpeed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.timeOut)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.spinAmount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.posLerp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.rotLerp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fov)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookatX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookatY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookatZ)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.targetX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.targetY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.targetZ)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.lookOffsetX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookOffsetY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lookOffsetZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.camOffX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.camOffY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.camOffZ)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox loadMenuLoop;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown farClip;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown nearClip;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown rotationSpeed;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.NumericUpDown movementSpeed;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.NumericUpDown timeOut;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown spinAmount;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown posLerp;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown rotLerp;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label fovPrev;
        private System.Windows.Forms.TrackBar fov;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown lookatX;
        private System.Windows.Forms.NumericUpDown lookatY;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.NumericUpDown lookatZ;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.NumericUpDown targetX;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.NumericUpDown targetY;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.NumericUpDown targetZ;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.NumericUpDown lookOffsetX;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.NumericUpDown lookOffsetY;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.NumericUpDown lookOffsetZ;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.NumericUpDown camOffX;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.NumericUpDown camOffY;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.NumericUpDown camOffZ;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.Button saveConfig;
    }
}