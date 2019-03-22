using System.Windows.Forms;

namespace EditorTool
{
    partial class Landing
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
            this.loadAssetType = new System.Windows.Forms.ComboBox();
            this.assetList = new System.Windows.Forms.ListBox();
            this.deleteAsset = new System.Windows.Forms.Button();
            this.importAsset = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.compileAssets = new System.Windows.Forms.Button();
            this.modelConfigs = new System.Windows.Forms.GroupBox();
            this.label16 = new System.Windows.Forms.Label();
            this.modelType = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.model_segmentsize = new System.Windows.Forms.NumericUpDown();
            this.label12 = new System.Windows.Forms.Label();
            this.model_scale = new System.Windows.Forms.NumericUpDown();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.saveAssetConfig = new System.Windows.Forms.Button();
            this.model_rot_z = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.model_rot_y = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.model_rot_x = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.model_world_z = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.model_world_y = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.model_world_x = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.modelPreview = new System.Windows.Forms.Integration.ElementHost();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.imagePreview = new System.Windows.Forms.PictureBox();
            this.playSoundPreview = new System.Windows.Forms.Button();
            this.soundPreview = new NAudio.Gui.WaveViewer();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.openItemConfig = new System.Windows.Forms.Button();
            this.openKeybindEditor = new System.Windows.Forms.Button();
            this.openLocalisationEditor = new System.Windows.Forms.Button();
            this.openUiEditor = new System.Windows.Forms.Button();
            this.label14 = new System.Windows.Forms.Label();
            this.DEBUG_DEFAULTTRACK = new System.Windows.Forms.ComboBox();
            this.label15 = new System.Windows.Forms.Label();
            this.DEBUG_PLAYERCOUNT = new System.Windows.Forms.NumericUpDown();
            this.DEBUG_SAVE = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.modelConfigs.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.model_segmentsize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.model_scale)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imagePreview)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DEBUG_PLAYERCOUNT)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // loadAssetType
            // 
            this.loadAssetType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.loadAssetType.FormattingEnabled = true;
            this.loadAssetType.Items.AddRange(new object[] {
            "Models",
            "Meshes",
            "Sounds",
            "Images",
            "Fonts"});
            this.loadAssetType.Location = new System.Drawing.Point(6, 19);
            this.loadAssetType.Name = "loadAssetType";
            this.loadAssetType.Size = new System.Drawing.Size(202, 21);
            this.loadAssetType.TabIndex = 2;
            this.loadAssetType.SelectedIndexChanged += new System.EventHandler(this.loadAssetType_SelectedIndexChanged);
            // 
            // assetList
            // 
            this.assetList.FormattingEnabled = true;
            this.assetList.Location = new System.Drawing.Point(6, 46);
            this.assetList.Name = "assetList";
            this.assetList.Size = new System.Drawing.Size(202, 303);
            this.assetList.TabIndex = 3;
            this.assetList.SelectedIndexChanged += new System.EventHandler(this.assetList_SelectedIndexChanged);
            // 
            // deleteAsset
            // 
            this.deleteAsset.Location = new System.Drawing.Point(108, 355);
            this.deleteAsset.Name = "deleteAsset";
            this.deleteAsset.Size = new System.Drawing.Size(100, 23);
            this.deleteAsset.TabIndex = 4;
            this.deleteAsset.Text = "Delete";
            this.deleteAsset.UseVisualStyleBackColor = true;
            this.deleteAsset.Click += new System.EventHandler(this.deleteAsset_Click);
            // 
            // importAsset
            // 
            this.importAsset.Location = new System.Drawing.Point(6, 355);
            this.importAsset.Name = "importAsset";
            this.importAsset.Size = new System.Drawing.Size(100, 23);
            this.importAsset.TabIndex = 5;
            this.importAsset.Text = "New";
            this.importAsset.UseVisualStyleBackColor = true;
            this.importAsset.Click += new System.EventHandler(this.refreshList_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.loadAssetType);
            this.groupBox2.Controls.Add(this.deleteAsset);
            this.groupBox2.Controls.Add(this.compileAssets);
            this.groupBox2.Controls.Add(this.assetList);
            this.groupBox2.Controls.Add(this.importAsset);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(215, 415);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Asset Browser";
            // 
            // compileAssets
            // 
            this.compileAssets.Location = new System.Drawing.Point(6, 379);
            this.compileAssets.Name = "compileAssets";
            this.compileAssets.Size = new System.Drawing.Size(202, 30);
            this.compileAssets.TabIndex = 0;
            this.compileAssets.Text = "Compile Assets and Configs";
            this.compileAssets.UseVisualStyleBackColor = true;
            this.compileAssets.Click += new System.EventHandler(this.compileAssets_Click);
            // 
            // modelConfigs
            // 
            this.modelConfigs.Controls.Add(this.label16);
            this.modelConfigs.Controls.Add(this.modelType);
            this.modelConfigs.Controls.Add(this.label13);
            this.modelConfigs.Controls.Add(this.model_segmentsize);
            this.modelConfigs.Controls.Add(this.label12);
            this.modelConfigs.Controls.Add(this.model_scale);
            this.modelConfigs.Controls.Add(this.label11);
            this.modelConfigs.Controls.Add(this.label10);
            this.modelConfigs.Controls.Add(this.label1);
            this.modelConfigs.Controls.Add(this.saveAssetConfig);
            this.modelConfigs.Controls.Add(this.model_rot_z);
            this.modelConfigs.Controls.Add(this.label6);
            this.modelConfigs.Controls.Add(this.model_rot_y);
            this.modelConfigs.Controls.Add(this.label7);
            this.modelConfigs.Controls.Add(this.model_rot_x);
            this.modelConfigs.Controls.Add(this.label8);
            this.modelConfigs.Controls.Add(this.model_world_z);
            this.modelConfigs.Controls.Add(this.label5);
            this.modelConfigs.Controls.Add(this.model_world_y);
            this.modelConfigs.Controls.Add(this.label4);
            this.modelConfigs.Controls.Add(this.model_world_x);
            this.modelConfigs.Controls.Add(this.label3);
            this.modelConfigs.Controls.Add(this.label2);
            this.modelConfigs.Controls.Add(this.label9);
            this.modelConfigs.Location = new System.Drawing.Point(233, 177);
            this.modelConfigs.Name = "modelConfigs";
            this.modelConfigs.Size = new System.Drawing.Size(226, 250);
            this.modelConfigs.TabIndex = 11;
            this.modelConfigs.TabStop = false;
            this.modelConfigs.Text = "Model Configuration";
            this.modelConfigs.Visible = false;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label16.Location = new System.Drawing.Point(7, 18);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(73, 13);
            this.label16.TabIndex = 34;
            this.label16.Text = "Model Type";
            // 
            // modelType
            // 
            this.modelType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modelType.FormattingEnabled = true;
            this.modelType.Items.AddRange(new object[] {
            "Track",
            "Player",
            "Prop"});
            this.modelType.Location = new System.Drawing.Point(10, 34);
            this.modelType.Name = "modelType";
            this.modelType.Size = new System.Drawing.Size(210, 21);
            this.modelType.TabIndex = 33;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(116, 180);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(84, 13);
            this.label13.TabIndex = 32;
            this.label13.Text = "Segment Size";
            // 
            // model_segmentsize
            // 
            this.model_segmentsize.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.model_segmentsize.Location = new System.Drawing.Point(119, 195);
            this.model_segmentsize.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.model_segmentsize.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.model_segmentsize.Name = "model_segmentsize";
            this.model_segmentsize.Size = new System.Drawing.Size(101, 20);
            this.model_segmentsize.TabIndex = 31;
            this.model_segmentsize.Value = new decimal(new int[] {
            100,
            0,
            0,
            65536});
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(5, 179);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(77, 13);
            this.label12.TabIndex = 30;
            this.label12.Text = "Model Scale";
            // 
            // model_scale
            // 
            this.model_scale.DecimalPlaces = 5;
            this.model_scale.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.model_scale.Location = new System.Drawing.Point(8, 195);
            this.model_scale.Name = "model_scale";
            this.model_scale.Size = new System.Drawing.Size(101, 20);
            this.model_scale.TabIndex = 29;
            this.model_scale.Value = new decimal(new int[] {
            10,
            0,
            0,
            65536});
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(7, 141);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(181, 13);
            this.label11.TabIndex = 28;
            this.label11.Text = "Default starting rotation of this model.";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(7, 87);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(213, 13);
            this.label10.TabIndex = 27;
            this.label10.Text = "Other models use this as their initial position.";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 73);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(215, 13);
            this.label1.TabIndex = 26;
            this.label1.Text = "For tracks, this specifies the spawn location.";
            // 
            // saveAssetConfig
            // 
            this.saveAssetConfig.Location = new System.Drawing.Point(145, 221);
            this.saveAssetConfig.Name = "saveAssetConfig";
            this.saveAssetConfig.Size = new System.Drawing.Size(75, 23);
            this.saveAssetConfig.TabIndex = 25;
            this.saveAssetConfig.Text = "Save";
            this.saveAssetConfig.UseVisualStyleBackColor = true;
            this.saveAssetConfig.Click += new System.EventHandler(this.saveAssetConfig_Click);
            // 
            // model_rot_z
            // 
            this.model_rot_z.Location = new System.Drawing.Point(168, 157);
            this.model_rot_z.Name = "model_rot_z";
            this.model_rot_z.Size = new System.Drawing.Size(52, 20);
            this.model_rot_z.TabIndex = 22;
            this.model_rot_z.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.model_rot_z_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(153, 160);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(17, 13);
            this.label6.TabIndex = 23;
            this.label6.Text = "Z:";
            // 
            // model_rot_y
            // 
            this.model_rot_y.Location = new System.Drawing.Point(95, 157);
            this.model_rot_y.Name = "model_rot_y";
            this.model_rot_y.Size = new System.Drawing.Size(52, 20);
            this.model_rot_y.TabIndex = 20;
            this.model_rot_y.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.model_rot_y_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(79, 160);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(17, 13);
            this.label7.TabIndex = 21;
            this.label7.Text = "Y:";
            // 
            // model_rot_x
            // 
            this.model_rot_x.Location = new System.Drawing.Point(21, 157);
            this.model_rot_x.Name = "model_rot_x";
            this.model_rot_x.Size = new System.Drawing.Size(52, 20);
            this.model_rot_x.TabIndex = 19;
            this.model_rot_x.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.model_rot_x_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(7, 126);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(55, 13);
            this.label8.TabIndex = 18;
            this.label8.Text = "Rotation";
            // 
            // model_world_z
            // 
            this.model_world_z.Location = new System.Drawing.Point(168, 103);
            this.model_world_z.Name = "model_world_z";
            this.model_world_z.Size = new System.Drawing.Size(52, 20);
            this.model_world_z.TabIndex = 16;
            this.model_world_z.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.model_world_z_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(153, 106);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(17, 13);
            this.label5.TabIndex = 17;
            this.label5.Text = "Z:";
            // 
            // model_world_y
            // 
            this.model_world_y.Location = new System.Drawing.Point(95, 103);
            this.model_world_y.Name = "model_world_y";
            this.model_world_y.Size = new System.Drawing.Size(52, 20);
            this.model_world_y.TabIndex = 14;
            this.model_world_y.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.model_world_y_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(80, 106);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(17, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Y:";
            // 
            // model_world_x
            // 
            this.model_world_x.Location = new System.Drawing.Point(21, 103);
            this.model_world_x.Name = "model_world_x";
            this.model_world_x.Size = new System.Drawing.Size(52, 20);
            this.model_world_x.TabIndex = 12;
            this.model_world_x.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.model_world_x_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 106);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(17, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "X:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(7, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "World Position";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(7, 160);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(17, 13);
            this.label9.TabIndex = 24;
            this.label9.Text = "X:";
            // 
            // modelPreview
            // 
            this.modelPreview.Location = new System.Drawing.Point(6, 19);
            this.modelPreview.Name = "modelPreview";
            this.modelPreview.Size = new System.Drawing.Size(214, 133);
            this.modelPreview.TabIndex = 12;
            this.modelPreview.Text = "elementHost1";
            this.modelPreview.Child = null;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.imagePreview);
            this.groupBox1.Controls.Add(this.playSoundPreview);
            this.groupBox1.Controls.Add(this.modelPreview);
            this.groupBox1.Controls.Add(this.soundPreview);
            this.groupBox1.Location = new System.Drawing.Point(233, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(226, 159);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Asset Preview";
            // 
            // imagePreview
            // 
            this.imagePreview.Location = new System.Drawing.Point(6, 19);
            this.imagePreview.Name = "imagePreview";
            this.imagePreview.Size = new System.Drawing.Size(215, 133);
            this.imagePreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.imagePreview.TabIndex = 15;
            this.imagePreview.TabStop = false;
            this.imagePreview.Visible = false;
            // 
            // playSoundPreview
            // 
            this.playSoundPreview.Location = new System.Drawing.Point(167, 129);
            this.playSoundPreview.Name = "playSoundPreview";
            this.playSoundPreview.Size = new System.Drawing.Size(54, 23);
            this.playSoundPreview.TabIndex = 17;
            this.playSoundPreview.Text = "Play";
            this.playSoundPreview.UseVisualStyleBackColor = true;
            this.playSoundPreview.Visible = false;
            this.playSoundPreview.Click += new System.EventHandler(this.playSoundPreview_Click);
            // 
            // soundPreview
            // 
            this.soundPreview.Location = new System.Drawing.Point(6, 19);
            this.soundPreview.Name = "soundPreview";
            this.soundPreview.SamplesPerPixel = 128;
            this.soundPreview.Size = new System.Drawing.Size(215, 133);
            this.soundPreview.StartPosition = ((long)(0));
            this.soundPreview.TabIndex = 16;
            this.soundPreview.Visible = false;
            this.soundPreview.WaveStream = null;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.openItemConfig);
            this.groupBox4.Controls.Add(this.openKeybindEditor);
            this.groupBox4.Controls.Add(this.openLocalisationEditor);
            this.groupBox4.Controls.Add(this.openUiEditor);
            this.groupBox4.Location = new System.Drawing.Point(12, 433);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(447, 55);
            this.groupBox4.TabIndex = 14;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Editors";
            // 
            // openItemConfig
            // 
            this.openItemConfig.Enabled = false;
            this.openItemConfig.Location = new System.Drawing.Point(341, 19);
            this.openItemConfig.Name = "openItemConfig";
            this.openItemConfig.Size = new System.Drawing.Size(100, 30);
            this.openItemConfig.TabIndex = 4;
            this.openItemConfig.Text = "Items";
            this.openItemConfig.UseVisualStyleBackColor = true;
            this.openItemConfig.Click += new System.EventHandler(this.openItemConfig_Click);
            // 
            // openKeybindEditor
            // 
            this.openKeybindEditor.Location = new System.Drawing.Point(117, 19);
            this.openKeybindEditor.Name = "openKeybindEditor";
            this.openKeybindEditor.Size = new System.Drawing.Size(100, 30);
            this.openKeybindEditor.TabIndex = 3;
            this.openKeybindEditor.Text = "Keybinds";
            this.openKeybindEditor.UseVisualStyleBackColor = true;
            this.openKeybindEditor.Click += new System.EventHandler(this.openKeybindEditor_Click);
            // 
            // openLocalisationEditor
            // 
            this.openLocalisationEditor.Enabled = false;
            this.openLocalisationEditor.Location = new System.Drawing.Point(230, 19);
            this.openLocalisationEditor.Name = "openLocalisationEditor";
            this.openLocalisationEditor.Size = new System.Drawing.Size(100, 30);
            this.openLocalisationEditor.TabIndex = 2;
            this.openLocalisationEditor.Text = "Localisation";
            this.openLocalisationEditor.UseVisualStyleBackColor = true;
            this.openLocalisationEditor.Click += new System.EventHandler(this.openLocalisationEditor_Click);
            // 
            // openUiEditor
            // 
            this.openUiEditor.Location = new System.Drawing.Point(7, 19);
            this.openUiEditor.Name = "openUiEditor";
            this.openUiEditor.Size = new System.Drawing.Size(99, 30);
            this.openUiEditor.TabIndex = 1;
            this.openUiEditor.Text = "UI";
            this.openUiEditor.UseVisualStyleBackColor = true;
            this.openUiEditor.Click += new System.EventHandler(this.openUiEditor_Click);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(4, 19);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(72, 13);
            this.label14.TabIndex = 15;
            this.label14.Text = "Default Track";
            // 
            // DEBUG_DEFAULTTRACK
            // 
            this.DEBUG_DEFAULTTRACK.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.DEBUG_DEFAULTTRACK.FormattingEnabled = true;
            this.DEBUG_DEFAULTTRACK.Items.AddRange(new object[] {
            "Models",
            "Meshes",
            "Sounds",
            "Images",
            "Fonts"});
            this.DEBUG_DEFAULTTRACK.Location = new System.Drawing.Point(7, 35);
            this.DEBUG_DEFAULTTRACK.Name = "DEBUG_DEFAULTTRACK";
            this.DEBUG_DEFAULTTRACK.Size = new System.Drawing.Size(202, 21);
            this.DEBUG_DEFAULTTRACK.TabIndex = 16;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(212, 18);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(67, 13);
            this.label15.TabIndex = 17;
            this.label15.Text = "Player Count";
            // 
            // DEBUG_PLAYERCOUNT
            // 
            this.DEBUG_PLAYERCOUNT.Location = new System.Drawing.Point(215, 35);
            this.DEBUG_PLAYERCOUNT.Maximum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.DEBUG_PLAYERCOUNT.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.DEBUG_PLAYERCOUNT.Name = "DEBUG_PLAYERCOUNT";
            this.DEBUG_PLAYERCOUNT.Size = new System.Drawing.Size(101, 20);
            this.DEBUG_PLAYERCOUNT.TabIndex = 32;
            this.DEBUG_PLAYERCOUNT.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // DEBUG_SAVE
            // 
            this.DEBUG_SAVE.Location = new System.Drawing.Point(322, 19);
            this.DEBUG_SAVE.Name = "DEBUG_SAVE";
            this.DEBUG_SAVE.Size = new System.Drawing.Size(120, 37);
            this.DEBUG_SAVE.TabIndex = 33;
            this.DEBUG_SAVE.Text = "Save";
            this.DEBUG_SAVE.UseVisualStyleBackColor = true;
            this.DEBUG_SAVE.Click += new System.EventHandler(this.DEBUG_SAVE_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.DEBUG_SAVE);
            this.groupBox3.Controls.Add(this.DEBUG_DEFAULTTRACK);
            this.groupBox3.Controls.Add(this.DEBUG_PLAYERCOUNT);
            this.groupBox3.Controls.Add(this.label15);
            this.groupBox3.Location = new System.Drawing.Point(12, 494);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(447, 64);
            this.groupBox3.TabIndex = 34;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Debug Configurations";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(147, 564);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(168, 23);
            this.button1.TabIndex = 35;
            this.button1.Text = "debug track importer";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Landing
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(469, 594);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.modelConfigs);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Landing";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mario Kart Asset Manager";
            this.Load += new System.EventHandler(this.Landing_Load);
            this.groupBox2.ResumeLayout(false);
            this.modelConfigs.ResumeLayout(false);
            this.modelConfigs.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.model_segmentsize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.model_scale)).EndInit();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.imagePreview)).EndInit();
            this.groupBox4.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DEBUG_PLAYERCOUNT)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ComboBox loadAssetType;
        private System.Windows.Forms.ListBox assetList;
        private System.Windows.Forms.Button deleteAsset;
        private System.Windows.Forms.Button importAsset;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox modelConfigs;
        private System.Windows.Forms.Integration.ElementHost modelPreview;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button saveAssetConfig;
        private System.Windows.Forms.TextBox model_rot_z;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox model_rot_y;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox model_rot_x;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox model_world_z;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox model_world_y;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox model_world_x;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button openLocalisationEditor;
        private System.Windows.Forms.Button openUiEditor;
        private System.Windows.Forms.Button compileAssets;
        private System.Windows.Forms.PictureBox imagePreview;
        private NAudio.Gui.WaveViewer soundPreview;
        private System.Windows.Forms.Button playSoundPreview;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.NumericUpDown model_scale;
        private Button openKeybindEditor;
        private Label label13;
        private NumericUpDown model_segmentsize;
        private Label label14;
        private ComboBox DEBUG_DEFAULTTRACK;
        private Label label15;
        private NumericUpDown DEBUG_PLAYERCOUNT;
        private Button DEBUG_SAVE;
        private GroupBox groupBox3;
        private Label label10;
        private Label label1;
        private Label label16;
        private ComboBox modelType;
        private Button openItemConfig;
        private Button button1;
    }
}