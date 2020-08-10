using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DeadRisingLauncher
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void PanelMouseEnterEvent(object sender, EventArgs e)
        {
            // Get the panel from the sender.
            Panel panel = (Panel)sender;

            // Check the name of the pnael to determine which bitmap to use.
            if (panel.Name == "btnPlay")
                panel.BackgroundImage = Properties.Resources.PlayButtonInvert;
            else if (panel.Name == "btnSettings")
                panel.BackgroundImage = Properties.Resources.SettingsButtonInvert;
            else if (panel.Name == "btnExit")
                panel.BackgroundImage = Properties.Resources.ExitButtonInvert;
        }

        private void PanelMouseLeaveEvent(object sender, EventArgs e)
        {
            // Get the panel from the sender.
            Panel panel = (Panel)sender;

            // Check the name of the pnael to determine which bitmap to use.
            if (panel.Name == "btnPlay")
                panel.BackgroundImage = Properties.Resources.PlayButton;
            else if (panel.Name == "btnSettings")
                panel.BackgroundImage = Properties.Resources.SettingsButton;
            else if (panel.Name == "btnExit")
                panel.BackgroundImage = Properties.Resources.ExitButton;
        }

        private void btnPlay_MouseClick(object sender, MouseEventArgs e)
        {
            // Set the steam appid environment variable. This will be copied into the child game process
            // and is needed so steam does not restart the game process without our dll.
            Environment.SetEnvironmentVariable("SteamAppId", "427190");

            // Launch the game with the DeadRisingEx dll.
            if (DeadRisingEx.LaunchDeadRisingEx(Application.StartupPath) == false)
            {
                // Display an error to the user.
                MessageBox.Show("Failed to start DeadRisingEx!");
                return;
            }

            // Close the application.
            Application.Exit();
        }

        private void btnSettings_MouseClick(object sender, MouseEventArgs e)
        {
            // Display the settings dialog.
            SettingsDialog settings = new SettingsDialog();
            settings.ShowDialog();
        }

        private void btnExit_MouseClick(object sender, MouseEventArgs e)
        {
            // Quit the application.
            Application.Exit();
        }
    }
}
