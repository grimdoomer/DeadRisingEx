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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::DeadRisingLauncher.Properties.Resources.DeadRisingExBackground;
            this.ClientSize = new System.Drawing.Size(1006, 527);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.btnSettings);
            this.Controls.Add(this.btnPlay);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel btnPlay;
        private System.Windows.Forms.Panel btnSettings;
        private System.Windows.Forms.Panel btnExit;
    }
}

