namespace EditorTool
{
    partial class Model_Importer_MaterialEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Model_Importer_MaterialEditor));
            this.materialPreview = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.materialName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.transparencySlider = new System.Windows.Forms.TrackBar();
            this.inPlayableArea = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.isMetallic = new System.Windows.Forms.CheckBox();
            this.label15 = new System.Windows.Forms.Label();
            this.collisionGroup = new System.Windows.Forms.GroupBox();
            this.boostPad = new System.Windows.Forms.RadioButton();
            this.antiGravPad = new System.Windows.Forms.RadioButton();
            this.jumpPad = new System.Windows.Forms.RadioButton();
            this.onTrack = new System.Windows.Forms.RadioButton();
            this.offTrack = new System.Windows.Forms.RadioButton();
            this.isWall = new System.Windows.Forms.RadioButton();
            this.onTrackNoAI = new System.Windows.Forms.CheckBox();
            this.label4 = new System.Windows.Forms.Label();
            this.transparencyValue = new System.Windows.Forms.Label();
            this.hasSpec = new System.Windows.Forms.CheckBox();
            this.label6 = new System.Windows.Forms.Label();
            this.specExValue = new System.Windows.Forms.Label();
            this.specExSlider = new System.Windows.Forms.TrackBar();
            this.saveMatConfig = new System.Windows.Forms.Button();
            this.emissiveColourBrowse = new System.Windows.Forms.Button();
            this.label14 = new System.Windows.Forms.Label();
            this.emissiveColour = new System.Windows.Forms.PictureBox();
            this.specularColourBrowse = new System.Windows.Forms.Button();
            this.label13 = new System.Windows.Forms.Label();
            this.specularColour = new System.Windows.Forms.PictureBox();
            this.ambientColourBrowse = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.ambientColour = new System.Windows.Forms.PictureBox();
            this.diffuseColourBrowse = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.diffuseColour = new System.Windows.Forms.PictureBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.normalMap = new System.Windows.Forms.TextBox();
            this.normalMapBrowse = new System.Windows.Forms.Button();
            this.specularMapBrowse = new System.Windows.Forms.Button();
            this.specularMap = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.emissiveMapBrowse = new System.Windows.Forms.Button();
            this.emissiveMap = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.metalnessSlider = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.roughnessSlider = new System.Windows.Forms.TrackBar();
            this.ambientocclusionSlider = new System.Windows.Forms.TrackBar();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.diffuseMapList = new System.Windows.Forms.ListBox();
            this.addNewDiffuse = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.isDiffuseAnimated = new System.Windows.Forms.CheckBox();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.diffuseAnimTime = new System.Windows.Forms.NumericUpDown();
            this.removeSelectedDiffuse = new System.Windows.Forms.Button();
            this.groupBox10 = new System.Windows.Forms.GroupBox();
            this.groupBox11 = new System.Windows.Forms.GroupBox();
            ((System.ComponentModel.ISupportInitialize)(this.materialPreview)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.transparencySlider)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.collisionGroup.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.specExSlider)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.emissiveColour)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.specularColour)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ambientColour)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.diffuseColour)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.metalnessSlider)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.roughnessSlider)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ambientocclusionSlider)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox9.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.diffuseAnimTime)).BeginInit();
            this.groupBox10.SuspendLayout();
            this.groupBox11.SuspendLayout();
            this.SuspendLayout();
            // 
            // materialPreview
            // 
            this.materialPreview.Location = new System.Drawing.Point(416, 12);
            this.materialPreview.Name = "materialPreview";
            this.materialPreview.Size = new System.Drawing.Size(300, 300);
            this.materialPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.materialPreview.TabIndex = 15;
            this.materialPreview.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(75, 13);
            this.label1.TabIndex = 16;
            this.label1.Text = "Material Name";
            // 
            // materialName
            // 
            this.materialName.Location = new System.Drawing.Point(9, 36);
            this.materialName.Name = "materialName";
            this.materialName.ReadOnly = true;
            this.materialName.Size = new System.Drawing.Size(285, 20);
            this.materialName.TabIndex = 17;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(6, 282);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "Transparency";
            // 
            // transparencySlider
            // 
            this.transparencySlider.LargeChange = 1;
            this.transparencySlider.Location = new System.Drawing.Point(9, 300);
            this.transparencySlider.Name = "transparencySlider";
            this.transparencySlider.Size = new System.Drawing.Size(285, 45);
            this.transparencySlider.TabIndex = 20;
            this.transparencySlider.TickStyle = System.Windows.Forms.TickStyle.None;
            this.transparencySlider.Scroll += new System.EventHandler(this.transparencySlider_Scroll);
            // 
            // inPlayableArea
            // 
            this.inPlayableArea.AutoSize = true;
            this.inPlayableArea.Checked = true;
            this.inPlayableArea.CheckState = System.Windows.Forms.CheckState.Checked;
            this.inPlayableArea.Location = new System.Drawing.Point(6, 19);
            this.inPlayableArea.Name = "inPlayableArea";
            this.inPlayableArea.Size = new System.Drawing.Size(114, 17);
            this.inPlayableArea.TabIndex = 22;
            this.inPlayableArea.Text = "Is In Playable Area";
            this.inPlayableArea.UseVisualStyleBackColor = true;
            this.inPlayableArea.CheckedChanged += new System.EventHandler(this.inPlayableArea_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.isMetallic);
            this.groupBox1.Controls.Add(this.label15);
            this.groupBox1.Controls.Add(this.collisionGroup);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.transparencyValue);
            this.groupBox1.Controls.Add(this.materialName);
            this.groupBox1.Controls.Add(this.transparencySlider);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.hasSpec);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.specExValue);
            this.groupBox1.Controls.Add(this.specExSlider);
            this.groupBox1.Location = new System.Drawing.Point(416, 316);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(300, 353);
            this.groupBox1.TabIndex = 25;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "General Properties";
            // 
            // isMetallic
            // 
            this.isMetallic.AutoSize = true;
            this.isMetallic.Location = new System.Drawing.Point(9, 169);
            this.isMetallic.Name = "isMetallic";
            this.isMetallic.Size = new System.Drawing.Size(73, 17);
            this.isMetallic.TabIndex = 35;
            this.isMetallic.Text = "Is Metallic";
            this.isMetallic.UseVisualStyleBackColor = true;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(206, 326);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(88, 13);
            this.label15.TabIndex = 29;
            this.label15.Text = "TRANSPARENT";
            // 
            // collisionGroup
            // 
            this.collisionGroup.Controls.Add(this.boostPad);
            this.collisionGroup.Controls.Add(this.onTrackNoAI);
            this.collisionGroup.Controls.Add(this.antiGravPad);
            this.collisionGroup.Controls.Add(this.inPlayableArea);
            this.collisionGroup.Controls.Add(this.onTrack);
            this.collisionGroup.Controls.Add(this.offTrack);
            this.collisionGroup.Controls.Add(this.isWall);
            this.collisionGroup.Controls.Add(this.jumpPad);
            this.collisionGroup.Location = new System.Drawing.Point(9, 62);
            this.collisionGroup.Name = "collisionGroup";
            this.collisionGroup.Size = new System.Drawing.Size(285, 89);
            this.collisionGroup.TabIndex = 30;
            this.collisionGroup.TabStop = false;
            this.collisionGroup.Text = "Collision Config";
            // 
            // boostPad
            // 
            this.boostPad.AutoSize = true;
            this.boostPad.Location = new System.Drawing.Point(6, 65);
            this.boostPad.Name = "boostPad";
            this.boostPad.Size = new System.Drawing.Size(74, 17);
            this.boostPad.TabIndex = 29;
            this.boostPad.TabStop = true;
            this.boostPad.Text = "Boost Pad";
            this.boostPad.UseVisualStyleBackColor = true;
            // 
            // antiGravPad
            // 
            this.antiGravPad.AutoSize = true;
            this.antiGravPad.Location = new System.Drawing.Point(86, 65);
            this.antiGravPad.Name = "antiGravPad";
            this.antiGravPad.Size = new System.Drawing.Size(101, 17);
            this.antiGravPad.TabIndex = 31;
            this.antiGravPad.TabStop = true;
            this.antiGravPad.Text = "Anti-Gravity Pad";
            this.antiGravPad.UseVisualStyleBackColor = true;
            // 
            // jumpPad
            // 
            this.jumpPad.AutoSize = true;
            this.jumpPad.Location = new System.Drawing.Point(193, 65);
            this.jumpPad.Name = "jumpPad";
            this.jumpPad.Size = new System.Drawing.Size(72, 17);
            this.jumpPad.TabIndex = 32;
            this.jumpPad.TabStop = true;
            this.jumpPad.Text = "Jump Pad";
            this.jumpPad.UseVisualStyleBackColor = true;
            // 
            // onTrack
            // 
            this.onTrack.AutoSize = true;
            this.onTrack.Location = new System.Drawing.Point(6, 42);
            this.onTrack.Name = "onTrack";
            this.onTrack.Size = new System.Drawing.Size(70, 17);
            this.onTrack.TabIndex = 27;
            this.onTrack.TabStop = true;
            this.onTrack.Text = "On Track";
            this.onTrack.UseVisualStyleBackColor = true;
            this.onTrack.CheckedChanged += new System.EventHandler(this.onTrack_CheckedChanged);
            // 
            // offTrack
            // 
            this.offTrack.AutoSize = true;
            this.offTrack.Location = new System.Drawing.Point(82, 42);
            this.offTrack.Name = "offTrack";
            this.offTrack.Size = new System.Drawing.Size(70, 17);
            this.offTrack.TabIndex = 28;
            this.offTrack.TabStop = true;
            this.offTrack.Text = "Off-Track";
            this.offTrack.UseVisualStyleBackColor = true;
            // 
            // isWall
            // 
            this.isWall.AutoSize = true;
            this.isWall.Location = new System.Drawing.Point(158, 42);
            this.isWall.Name = "isWall";
            this.isWall.Size = new System.Drawing.Size(46, 17);
            this.isWall.TabIndex = 30;
            this.isWall.TabStop = true;
            this.isWall.Text = "Wall";
            this.isWall.UseVisualStyleBackColor = true;
            // 
            // onTrackNoAI
            // 
            this.onTrackNoAI.AutoSize = true;
            this.onTrackNoAI.Location = new System.Drawing.Point(126, 19);
            this.onTrackNoAI.Name = "onTrackNoAI";
            this.onTrackNoAI.Size = new System.Drawing.Size(148, 17);
            this.onTrackNoAI.TabIndex = 33;
            this.onTrackNoAI.Text = "Exclude from AI Navmesh";
            this.onTrackNoAI.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 326);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 13);
            this.label4.TabIndex = 28;
            this.label4.Text = "OPAQUE";
            // 
            // transparencyValue
            // 
            this.transparencyValue.AutoSize = true;
            this.transparencyValue.Location = new System.Drawing.Point(272, 285);
            this.transparencyValue.Name = "transparencyValue";
            this.transparencyValue.Size = new System.Drawing.Size(22, 13);
            this.transparencyValue.TabIndex = 26;
            this.transparencyValue.Text = "0.0";
            this.transparencyValue.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // hasSpec
            // 
            this.hasSpec.AutoSize = true;
            this.hasSpec.Location = new System.Drawing.Point(9, 192);
            this.hasSpec.Name = "hasSpec";
            this.hasSpec.Size = new System.Drawing.Size(104, 17);
            this.hasSpec.TabIndex = 34;
            this.hasSpec.Text = "Enable Specular";
            this.hasSpec.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(6, 230);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(49, 13);
            this.label6.TabIndex = 18;
            this.label6.Text = "Specular";
            // 
            // specExValue
            // 
            this.specExValue.AutoSize = true;
            this.specExValue.Location = new System.Drawing.Point(254, 230);
            this.specExValue.Name = "specExValue";
            this.specExValue.Size = new System.Drawing.Size(40, 13);
            this.specExValue.TabIndex = 24;
            this.specExValue.Text = "1000.0";
            this.specExValue.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.specExValue.Click += new System.EventHandler(this.label5_Click);
            // 
            // specExSlider
            // 
            this.specExSlider.LargeChange = 100;
            this.specExSlider.Location = new System.Drawing.Point(9, 246);
            this.specExSlider.Maximum = 1000;
            this.specExSlider.Name = "specExSlider";
            this.specExSlider.Size = new System.Drawing.Size(285, 45);
            this.specExSlider.SmallChange = 100;
            this.specExSlider.TabIndex = 20;
            this.specExSlider.TickStyle = System.Windows.Forms.TickStyle.None;
            this.specExSlider.Scroll += new System.EventHandler(this.specExSlider_Scroll);
            // 
            // saveMatConfig
            // 
            this.saveMatConfig.Location = new System.Drawing.Point(416, 676);
            this.saveMatConfig.Name = "saveMatConfig";
            this.saveMatConfig.Size = new System.Drawing.Size(300, 31);
            this.saveMatConfig.TabIndex = 21;
            this.saveMatConfig.Text = "Save";
            this.saveMatConfig.UseVisualStyleBackColor = true;
            this.saveMatConfig.Click += new System.EventHandler(this.button1_Click);
            // 
            // emissiveColourBrowse
            // 
            this.emissiveColourBrowse.Location = new System.Drawing.Point(317, 160);
            this.emissiveColourBrowse.Name = "emissiveColourBrowse";
            this.emissiveColourBrowse.Size = new System.Drawing.Size(75, 22);
            this.emissiveColourBrowse.TabIndex = 49;
            this.emissiveColourBrowse.Text = "Browse";
            this.emissiveColourBrowse.UseVisualStyleBackColor = true;
            this.emissiveColourBrowse.Click += new System.EventHandler(this.emissiveColourBrowse_Click);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(3, 144);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(81, 13);
            this.label14.TabIndex = 48;
            this.label14.Text = "Emissive Colour";
            // 
            // emissiveColour
            // 
            this.emissiveColour.Location = new System.Drawing.Point(6, 160);
            this.emissiveColour.Name = "emissiveColour";
            this.emissiveColour.Size = new System.Drawing.Size(305, 22);
            this.emissiveColour.TabIndex = 47;
            this.emissiveColour.TabStop = false;
            // 
            // specularColourBrowse
            // 
            this.specularColourBrowse.Location = new System.Drawing.Point(317, 119);
            this.specularColourBrowse.Name = "specularColourBrowse";
            this.specularColourBrowse.Size = new System.Drawing.Size(75, 22);
            this.specularColourBrowse.TabIndex = 46;
            this.specularColourBrowse.Text = "Browse";
            this.specularColourBrowse.UseVisualStyleBackColor = true;
            this.specularColourBrowse.Click += new System.EventHandler(this.specularColourBrowse_Click);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(3, 103);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(82, 13);
            this.label13.TabIndex = 45;
            this.label13.Text = "Specular Colour";
            // 
            // specularColour
            // 
            this.specularColour.Location = new System.Drawing.Point(6, 119);
            this.specularColour.Name = "specularColour";
            this.specularColour.Size = new System.Drawing.Size(305, 22);
            this.specularColour.TabIndex = 44;
            this.specularColour.TabStop = false;
            // 
            // ambientColourBrowse
            // 
            this.ambientColourBrowse.Location = new System.Drawing.Point(317, 78);
            this.ambientColourBrowse.Name = "ambientColourBrowse";
            this.ambientColourBrowse.Size = new System.Drawing.Size(75, 22);
            this.ambientColourBrowse.TabIndex = 43;
            this.ambientColourBrowse.Text = "Browse";
            this.ambientColourBrowse.UseVisualStyleBackColor = true;
            this.ambientColourBrowse.Click += new System.EventHandler(this.ambientColourBrowse_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(3, 62);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(78, 13);
            this.label12.TabIndex = 42;
            this.label12.Text = "Ambient Colour";
            // 
            // ambientColour
            // 
            this.ambientColour.Location = new System.Drawing.Point(6, 78);
            this.ambientColour.Name = "ambientColour";
            this.ambientColour.Size = new System.Drawing.Size(305, 22);
            this.ambientColour.TabIndex = 41;
            this.ambientColour.TabStop = false;
            // 
            // diffuseColourBrowse
            // 
            this.diffuseColourBrowse.Location = new System.Drawing.Point(317, 36);
            this.diffuseColourBrowse.Name = "diffuseColourBrowse";
            this.diffuseColourBrowse.Size = new System.Drawing.Size(75, 22);
            this.diffuseColourBrowse.TabIndex = 40;
            this.diffuseColourBrowse.Text = "Browse";
            this.diffuseColourBrowse.UseVisualStyleBackColor = true;
            this.diffuseColourBrowse.Click += new System.EventHandler(this.diffuseColourBrowse_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(3, 20);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(73, 13);
            this.label11.TabIndex = 39;
            this.label11.Text = "Diffuse Colour";
            // 
            // diffuseColour
            // 
            this.diffuseColour.Location = new System.Drawing.Point(6, 36);
            this.diffuseColour.Name = "diffuseColour";
            this.diffuseColour.Size = new System.Drawing.Size(305, 22);
            this.diffuseColour.TabIndex = 38;
            this.diffuseColour.TabStop = false;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(3, 96);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(64, 13);
            this.label10.TabIndex = 36;
            this.label10.Text = "Normal Map";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(3, 18);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(73, 13);
            this.label9.TabIndex = 33;
            this.label9.Text = "Specular Map";
            // 
            // normalMap
            // 
            this.normalMap.Location = new System.Drawing.Point(6, 112);
            this.normalMap.Name = "normalMap";
            this.normalMap.ReadOnly = true;
            this.normalMap.Size = new System.Drawing.Size(305, 20);
            this.normalMap.TabIndex = 37;
            // 
            // normalMapBrowse
            // 
            this.normalMapBrowse.Location = new System.Drawing.Point(317, 111);
            this.normalMapBrowse.Name = "normalMapBrowse";
            this.normalMapBrowse.Size = new System.Drawing.Size(75, 22);
            this.normalMapBrowse.TabIndex = 35;
            this.normalMapBrowse.Text = "Browse";
            this.normalMapBrowse.UseVisualStyleBackColor = true;
            this.normalMapBrowse.Click += new System.EventHandler(this.normalMapBrowse_Click);
            // 
            // specularMapBrowse
            // 
            this.specularMapBrowse.Location = new System.Drawing.Point(317, 33);
            this.specularMapBrowse.Name = "specularMapBrowse";
            this.specularMapBrowse.Size = new System.Drawing.Size(75, 22);
            this.specularMapBrowse.TabIndex = 32;
            this.specularMapBrowse.Text = "Browse";
            this.specularMapBrowse.UseVisualStyleBackColor = true;
            this.specularMapBrowse.Click += new System.EventHandler(this.specularMapBrowse_Click);
            // 
            // specularMap
            // 
            this.specularMap.Location = new System.Drawing.Point(6, 34);
            this.specularMap.Name = "specularMap";
            this.specularMap.ReadOnly = true;
            this.specularMap.Size = new System.Drawing.Size(305, 20);
            this.specularMap.TabIndex = 34;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(3, 57);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(72, 13);
            this.label8.TabIndex = 30;
            this.label8.Text = "Emissive Map";
            // 
            // emissiveMapBrowse
            // 
            this.emissiveMapBrowse.Location = new System.Drawing.Point(317, 72);
            this.emissiveMapBrowse.Name = "emissiveMapBrowse";
            this.emissiveMapBrowse.Size = new System.Drawing.Size(75, 22);
            this.emissiveMapBrowse.TabIndex = 29;
            this.emissiveMapBrowse.Text = "Browse";
            this.emissiveMapBrowse.UseVisualStyleBackColor = true;
            this.emissiveMapBrowse.Click += new System.EventHandler(this.ambientMapBrowse_Click);
            // 
            // emissiveMap
            // 
            this.emissiveMap.Location = new System.Drawing.Point(6, 73);
            this.emissiveMap.Name = "emissiveMap";
            this.emissiveMap.ReadOnly = true;
            this.emissiveMap.Size = new System.Drawing.Size(305, 20);
            this.emissiveMap.TabIndex = 31;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(3, 19);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(67, 13);
            this.label7.TabIndex = 27;
            this.label7.Text = "Texture Map";
            // 
            // metalnessSlider
            // 
            this.metalnessSlider.LargeChange = 1;
            this.metalnessSlider.Location = new System.Drawing.Point(9, 34);
            this.metalnessSlider.Maximum = 255;
            this.metalnessSlider.Name = "metalnessSlider";
            this.metalnessSlider.Size = new System.Drawing.Size(383, 45);
            this.metalnessSlider.TabIndex = 35;
            this.metalnessSlider.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(6, 18);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(55, 13);
            this.label3.TabIndex = 35;
            this.label3.Text = "Metalness";
            // 
            // roughnessSlider
            // 
            this.roughnessSlider.LargeChange = 1;
            this.roughnessSlider.Location = new System.Drawing.Point(9, 136);
            this.roughnessSlider.Maximum = 255;
            this.roughnessSlider.Name = "roughnessSlider";
            this.roughnessSlider.Size = new System.Drawing.Size(383, 45);
            this.roughnessSlider.TabIndex = 36;
            this.roughnessSlider.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // ambientocclusionSlider
            // 
            this.ambientocclusionSlider.LargeChange = 1;
            this.ambientocclusionSlider.Location = new System.Drawing.Point(9, 85);
            this.ambientocclusionSlider.Maximum = 255;
            this.ambientocclusionSlider.Name = "ambientocclusionSlider";
            this.ambientocclusionSlider.Size = new System.Drawing.Size(383, 45);
            this.ambientocclusionSlider.TabIndex = 37;
            this.ambientocclusionSlider.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label18.Location = new System.Drawing.Point(6, 69);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(95, 13);
            this.label18.TabIndex = 38;
            this.label18.Text = "Ambient Occlusion";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label19.Location = new System.Drawing.Point(6, 120);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(61, 13);
            this.label19.TabIndex = 39;
            this.label19.Text = "Roughness";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label18);
            this.groupBox2.Controls.Add(this.metalnessSlider);
            this.groupBox2.Controls.Add(this.label19);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.ambientocclusionSlider);
            this.groupBox2.Controls.Add(this.roughnessSlider);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(398, 185);
            this.groupBox2.TabIndex = 40;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "RMA Values";
            // 
            // diffuseMapList
            // 
            this.diffuseMapList.FormattingEnabled = true;
            this.diffuseMapList.HorizontalScrollbar = true;
            this.diffuseMapList.Location = new System.Drawing.Point(6, 35);
            this.diffuseMapList.Name = "diffuseMapList";
            this.diffuseMapList.Size = new System.Drawing.Size(260, 121);
            this.diffuseMapList.TabIndex = 44;
            // 
            // addNewDiffuse
            // 
            this.addNewDiffuse.Location = new System.Drawing.Point(272, 105);
            this.addNewDiffuse.Name = "addNewDiffuse";
            this.addNewDiffuse.Size = new System.Drawing.Size(120, 23);
            this.addNewDiffuse.TabIndex = 46;
            this.addNewDiffuse.Text = "Add New";
            this.addNewDiffuse.UseVisualStyleBackColor = true;
            this.addNewDiffuse.Click += new System.EventHandler(this.addNewDiffuse_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(269, 63);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(113, 13);
            this.label5.TabIndex = 47;
            this.label5.Text = "Secs Between Frames";
            // 
            // isDiffuseAnimated
            // 
            this.isDiffuseAnimated.AutoCheck = false;
            this.isDiffuseAnimated.AutoSize = true;
            this.isDiffuseAnimated.Location = new System.Drawing.Point(272, 35);
            this.isDiffuseAnimated.Name = "isDiffuseAnimated";
            this.isDiffuseAnimated.Size = new System.Drawing.Size(81, 17);
            this.isDiffuseAnimated.TabIndex = 48;
            this.isDiffuseAnimated.Text = "Is Animated";
            this.isDiffuseAnimated.UseVisualStyleBackColor = true;
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.diffuseAnimTime);
            this.groupBox9.Controls.Add(this.removeSelectedDiffuse);
            this.groupBox9.Controls.Add(this.diffuseMapList);
            this.groupBox9.Controls.Add(this.label7);
            this.groupBox9.Controls.Add(this.isDiffuseAnimated);
            this.groupBox9.Controls.Add(this.addNewDiffuse);
            this.groupBox9.Controls.Add(this.label5);
            this.groupBox9.Location = new System.Drawing.Point(12, 203);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(398, 164);
            this.groupBox9.TabIndex = 49;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Diffuse Texture(s)";
            // 
            // diffuseAnimTime
            // 
            this.diffuseAnimTime.DecimalPlaces = 3;
            this.diffuseAnimTime.Location = new System.Drawing.Point(272, 79);
            this.diffuseAnimTime.Name = "diffuseAnimTime";
            this.diffuseAnimTime.Size = new System.Drawing.Size(120, 20);
            this.diffuseAnimTime.TabIndex = 52;
            // 
            // removeSelectedDiffuse
            // 
            this.removeSelectedDiffuse.Location = new System.Drawing.Point(272, 134);
            this.removeSelectedDiffuse.Name = "removeSelectedDiffuse";
            this.removeSelectedDiffuse.Size = new System.Drawing.Size(120, 23);
            this.removeSelectedDiffuse.TabIndex = 49;
            this.removeSelectedDiffuse.Text = "Remove Selected";
            this.removeSelectedDiffuse.UseVisualStyleBackColor = true;
            this.removeSelectedDiffuse.Click += new System.EventHandler(this.removeSelectedDiffuse_Click);
            // 
            // groupBox10
            // 
            this.groupBox10.Controls.Add(this.emissiveColourBrowse);
            this.groupBox10.Controls.Add(this.specularColourBrowse);
            this.groupBox10.Controls.Add(this.label14);
            this.groupBox10.Controls.Add(this.ambientColourBrowse);
            this.groupBox10.Controls.Add(this.emissiveColour);
            this.groupBox10.Controls.Add(this.diffuseColour);
            this.groupBox10.Controls.Add(this.label12);
            this.groupBox10.Controls.Add(this.label13);
            this.groupBox10.Controls.Add(this.diffuseColourBrowse);
            this.groupBox10.Controls.Add(this.specularColour);
            this.groupBox10.Controls.Add(this.label11);
            this.groupBox10.Controls.Add(this.ambientColour);
            this.groupBox10.Location = new System.Drawing.Point(12, 518);
            this.groupBox10.Name = "groupBox10";
            this.groupBox10.Size = new System.Drawing.Size(398, 189);
            this.groupBox10.TabIndex = 50;
            this.groupBox10.TabStop = false;
            this.groupBox10.Text = "Colours";
            // 
            // groupBox11
            // 
            this.groupBox11.Controls.Add(this.normalMapBrowse);
            this.groupBox11.Controls.Add(this.emissiveMap);
            this.groupBox11.Controls.Add(this.normalMap);
            this.groupBox11.Controls.Add(this.specularMap);
            this.groupBox11.Controls.Add(this.label10);
            this.groupBox11.Controls.Add(this.emissiveMapBrowse);
            this.groupBox11.Controls.Add(this.label9);
            this.groupBox11.Controls.Add(this.label8);
            this.groupBox11.Controls.Add(this.specularMapBrowse);
            this.groupBox11.Location = new System.Drawing.Point(12, 373);
            this.groupBox11.Name = "groupBox11";
            this.groupBox11.Size = new System.Drawing.Size(398, 139);
            this.groupBox11.TabIndex = 51;
            this.groupBox11.TabStop = false;
            this.groupBox11.Text = "Texture Maps";
            // 
            // Model_Importer_MaterialEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(727, 717);
            this.Controls.Add(this.groupBox11);
            this.Controls.Add(this.groupBox10);
            this.Controls.Add(this.groupBox9);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.materialPreview);
            this.Controls.Add(this.saveMatConfig);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Model_Importer_MaterialEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Material Editor";
            this.Load += new System.EventHandler(this.Model_Importer_MaterialEditor_Load);
            ((System.ComponentModel.ISupportInitialize)(this.materialPreview)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.transparencySlider)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.collisionGroup.ResumeLayout(false);
            this.collisionGroup.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.specExSlider)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.emissiveColour)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.specularColour)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ambientColour)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.diffuseColour)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.metalnessSlider)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.roughnessSlider)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ambientocclusionSlider)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.diffuseAnimTime)).EndInit();
            this.groupBox10.ResumeLayout(false);
            this.groupBox10.PerformLayout();
            this.groupBox11.ResumeLayout(false);
            this.groupBox11.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox materialPreview;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox materialName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar transparencySlider;
        private System.Windows.Forms.CheckBox inPlayableArea;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button saveMatConfig;
        private System.Windows.Forms.Label specExValue;
        private System.Windows.Forms.Button emissiveColourBrowse;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.PictureBox emissiveColour;
        private System.Windows.Forms.Button specularColourBrowse;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.PictureBox specularColour;
        private System.Windows.Forms.Button ambientColourBrowse;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.PictureBox ambientColour;
        private System.Windows.Forms.Button diffuseColourBrowse;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.PictureBox diffuseColour;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button normalMapBrowse;
        private System.Windows.Forms.TextBox normalMap;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button specularMapBrowse;
        private System.Windows.Forms.TextBox specularMap;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button emissiveMapBrowse;
        private System.Windows.Forms.TextBox emissiveMap;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.RadioButton boostPad;
        private System.Windows.Forms.RadioButton offTrack;
        private System.Windows.Forms.RadioButton onTrack;
        private System.Windows.Forms.GroupBox collisionGroup;
        private System.Windows.Forms.Label transparencyValue;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar specExSlider;
        private System.Windows.Forms.RadioButton isWall;
        private System.Windows.Forms.CheckBox hasSpec;
        private System.Windows.Forms.TrackBar metalnessSlider;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TrackBar roughnessSlider;
        private System.Windows.Forms.TrackBar ambientocclusionSlider;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox isMetallic;
        private System.Windows.Forms.RadioButton jumpPad;
        private System.Windows.Forms.RadioButton antiGravPad;
        private System.Windows.Forms.CheckBox onTrackNoAI;
        private System.Windows.Forms.ListBox diffuseMapList;
        private System.Windows.Forms.Button addNewDiffuse;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox isDiffuseAnimated;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.Button removeSelectedDiffuse;
        private System.Windows.Forms.GroupBox groupBox10;
        private System.Windows.Forms.GroupBox groupBox11;
        private System.Windows.Forms.NumericUpDown diffuseAnimTime;
    }
}