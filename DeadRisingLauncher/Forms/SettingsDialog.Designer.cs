namespace DeadRisingLauncher
{
    partial class SettingsDialog
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.btnLoadOrderDown = new System.Windows.Forms.Button();
            this.btnLoadOrderUp = new System.Windows.Forms.Button();
            this.lstModArchives = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.chkEnableDebugger = new System.Windows.Forms.CheckBox();
            this.btnOk = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.chkRecursiveGrenade = new System.Windows.Forms.CheckBox();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(407, 377);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.btnLoadOrderDown);
            this.tabPage1.Controls.Add(this.btnLoadOrderUp);
            this.tabPage1.Controls.Add(this.lstModArchives);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(399, 351);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Mod Files";
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(117, 328);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(97, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Modify Load Order:";
            // 
            // btnLoadOrderDown
            // 
            this.btnLoadOrderDown.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnLoadOrderDown.Location = new System.Drawing.Point(260, 323);
            this.btnLoadOrderDown.Name = "btnLoadOrderDown";
            this.btnLoadOrderDown.Size = new System.Drawing.Size(34, 23);
            this.btnLoadOrderDown.TabIndex = 2;
            this.btnLoadOrderDown.Text = "▼";
            this.btnLoadOrderDown.UseVisualStyleBackColor = true;
            this.btnLoadOrderDown.Click += new System.EventHandler(this.btnLoadOrderDown_Click);
            // 
            // btnLoadOrderUp
            // 
            this.btnLoadOrderUp.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnLoadOrderUp.Location = new System.Drawing.Point(220, 323);
            this.btnLoadOrderUp.Name = "btnLoadOrderUp";
            this.btnLoadOrderUp.Size = new System.Drawing.Size(34, 23);
            this.btnLoadOrderUp.TabIndex = 1;
            this.btnLoadOrderUp.Text = "▲";
            this.btnLoadOrderUp.UseVisualStyleBackColor = true;
            this.btnLoadOrderUp.Click += new System.EventHandler(this.btnLoadOrderUp_Click);
            // 
            // lstModArchives
            // 
            this.lstModArchives.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lstModArchives.CheckBoxes = true;
            this.lstModArchives.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.lstModArchives.FullRowSelect = true;
            this.lstModArchives.GridLines = true;
            this.lstModArchives.HideSelection = false;
            this.lstModArchives.Location = new System.Drawing.Point(0, 0);
            this.lstModArchives.Name = "lstModArchives";
            this.lstModArchives.ShowItemToolTips = true;
            this.lstModArchives.Size = new System.Drawing.Size(399, 317);
            this.lstModArchives.TabIndex = 0;
            this.lstModArchives.UseCompatibleStateImageBehavior = false;
            this.lstModArchives.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "File name";
            this.columnHeader1.Width = 395;
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage2.Controls.Add(this.chkRecursiveGrenade);
            this.tabPage2.Controls.Add(this.chkEnableDebugger);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(399, 351);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Settings";
            // 
            // chkEnableDebugger
            // 
            this.chkEnableDebugger.AutoSize = true;
            this.chkEnableDebugger.Location = new System.Drawing.Point(8, 6);
            this.chkEnableDebugger.Name = "chkEnableDebugger";
            this.chkEnableDebugger.Size = new System.Drawing.Size(138, 17);
            this.chkEnableDebugger.TabIndex = 0;
            this.chkEnableDebugger.Text = "Enable console window";
            this.chkEnableDebugger.UseVisualStyleBackColor = true;
            // 
            // btnOk
            // 
            this.btnOk.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOk.Location = new System.Drawing.Point(7, 383);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(310, 23);
            this.btnOk.TabIndex = 1;
            this.btnOk.Text = "OK";
            this.btnOk.UseVisualStyleBackColor = true;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.Location = new System.Drawing.Point(323, 383);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(77, 23);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // chkRecursiveGrenade
            // 
            this.chkRecursiveGrenade.AutoSize = true;
            this.chkRecursiveGrenade.Location = new System.Drawing.Point(8, 29);
            this.chkRecursiveGrenade.Name = "chkRecursiveGrenade";
            this.chkRecursiveGrenade.Size = new System.Drawing.Size(118, 17);
            this.chkRecursiveGrenade.TabIndex = 1;
            this.chkRecursiveGrenade.Text = "Recursive Grenade";
            this.chkRecursiveGrenade.UseVisualStyleBackColor = true;
            // 
            // SettingsDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(407, 412);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOk);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "SettingsDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Settings";
            this.Load += new System.EventHandler(this.SettingsDialog_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnLoadOrderDown;
        private System.Windows.Forms.Button btnLoadOrderUp;
        private System.Windows.Forms.ListView lstModArchives;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.CheckBox chkEnableDebugger;
        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.CheckBox chkRecursiveGrenade;
    }
}