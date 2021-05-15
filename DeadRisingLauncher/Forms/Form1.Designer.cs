namespace DeadRisingLauncher
{
    partial class Form1
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
            this.btnPlay = new System.Windows.Forms.Panel();
            this.btnSettings = new System.Windows.Forms.Panel();
            this.btnExit = new System.Windows.Forms.Panel();
            this.pnlWhiteboard = new System.Windows.Forms.Panel();
            this.lblVersion = new System.Windows.Forms.Label();
            this.lblStatus = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnPlay
            // 
            this.btnPlay.BackColor = System.Drawing.Color.Transparent;
            this.btnPlay.BackgroundImage = global::DeadRisingLauncher.Properties.Resources.PlayButton;
            this.btnPlay.Location = new System.Drawing.Point(405, 256);
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(104, 54);
            this.btnPlay.TabIndex = 0;
            this.btnPlay.MouseClick += new System.Windows.Forms.MouseEventHandler(this.btnPlay_MouseClick);
            this.btnPlay.MouseEnter += new System.EventHandler(this.PanelMouseEnterEvent);
            this.btnPlay.MouseLeave += new System.EventHandler(this.PanelMouseLeaveEvent);
            // 
            // btnSettings
            // 
            this.btnSettings.BackColor = System.Drawing.Color.Transparent;
            this.btnSettings.BackgroundImage = global::DeadRisingLauncher.Properties.Resources.SettingsButton;
            this.btnSettings.Location = new System.Drawing.Point(534, 252);
            this.btnSettings.Name = "btnSettings";
            this.btnSettings.Size = new System.Drawing.Size(272, 64);
            this.btnSettings.TabIndex = 1;
            this.btnSettings.MouseClick += new System.Windows.Forms.MouseEventHandler(this.btnSettings_MouseClick);
            this.btnSettings.MouseEnter += new System.EventHandler(this.PanelMouseEnterEvent);
            this.btnSettings.MouseLeave += new System.EventHandler(this.PanelMouseLeaveEvent);
            // 
            // btnExit
            // 
            this.btnExit.BackColor = System.Drawing.Color.Transparent;
            this.btnExit.BackgroundImage = global::DeadRisingLauncher.Properties.Resources.ExitButton;
            this.btnExit.Location = new System.Drawing.Point(825, 253);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(101, 65);
            this.btnExit.TabIndex = 2;
            this.btnExit.MouseClick += new System.Windows.Forms.MouseEventHandler(this.btnExit_MouseClick);
            this.btnExit.MouseEnter += new System.EventHandler(this.PanelMouseEnterEvent);
            this.btnExit.MouseLeave += new System.EventHandler(this.PanelMouseLeaveEvent);
            // 
            // pnlWhiteboard
            // 
            this.pnlWhiteboard.BackColor = System.Drawing.Color.Transparent;
            this.pnlWhiteboard.Location = new System.Drawing.Point(382, 101);
            this.pnlWhiteboard.Name = "pnlWhiteboard";
            this.pnlWhiteboard.Size = new System.Drawing.Size(563, 253);
            this.pnlWhiteboard.TabIndex = 3;
            // 
            // lblVersion
            // 
            this.lblVersion.AutoSize = true;
            this.lblVersion.BackColor = System.Drawing.Color.Transparent;
            this.lblVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblVersion.ForeColor = System.Drawing.Color.Red;
            this.lblVersion.Location = new System.Drawing.Point(35, 475);
            this.lblVersion.Name = "lblVersion";
            this.lblVersion.Size = new System.Drawing.Size(67, 20);
            this.lblVersion.TabIndex = 4;
            this.lblVersion.Text = "Version:";
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.BackColor = System.Drawing.Color.Transparent;
            this.lblStatus.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblStatus.ForeColor = System.Drawing.Color.Red;
            this.lblStatus.Location = new System.Drawing.Point(35, 498);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(172, 20);
            this.lblStatus.TabIndex = 5;
            this.lblStatus.Text = "Checking for updates...";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::DeadRisingLauncher.Properties.Resources.DeadRisingExBackground;
            this.ClientSize = new System.Drawing.Size(1006, 527);
            this.Controls.Add(this.lblStatus);
            this.Controls.Add(this.lblVersion);
            this.Controls.Add(this.btnSettings);
            this.Controls.Add(this.btnPlay);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.pnlWhiteboard);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel btnPlay;
        private System.Windows.Forms.Panel btnSettings;
        private System.Windows.Forms.Panel btnExit;
        private System.Windows.Forms.Panel pnlWhiteboard;
        private System.Windows.Forms.Label lblVersion;
        private System.Windows.Forms.Label lblStatus;
    }
}

