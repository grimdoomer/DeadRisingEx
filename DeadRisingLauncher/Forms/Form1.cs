using DeadRisingLauncher.Forms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DeadRisingLauncher
{
    public partial class Form1 : Form
    {
        enum UpdateStatus
        {
            Failed,
            NoUpdate,
            UpdateAvailable
        }

        #region Native API

        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;

        [DllImportAttribute("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);

        [DllImportAttribute("user32.dll")]
        public static extern bool ReleaseCapture();

        #endregion

        // Background worker for async operations.
        BackgroundWorker updateWorker = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Run bugfix routine and fix the unicode encoding on the settings file.
            //BugFix_ConvertSettingsToAscii();

            // Determine the build flavor.
#if DEBUG
            string buildFlavor = "Beta";
#else
            string buildFlavor = "Release";
#endif

            // Set the version string.
            this.lblVersion.Text = "Version: " + Assembly.GetExecutingAssembly().GetName().Version.ToString() + " " + buildFlavor;

            // Setup the update worker.
            this.updateWorker = new BackgroundWorker();
            this.updateWorker.DoWork += UpdateWorker_DoWork;
            this.updateWorker.ProgressChanged += UpdateWorker_ProgressChanged;
            this.updateWorker.RunWorkerCompleted += UpdateWorker_RunWorkerCompleted;
            this.updateWorker.WorkerReportsProgress = true;

            // Kick off the update worker to check for updates.
            this.updateWorker.RunWorkerAsync();
        }

#region UpdateWorker

        private void UpdateWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            // Check the progress value and handle accordingly.
            UpdateStatus updateStatus = (UpdateStatus)e.UserState;
            if (updateStatus == UpdateStatus.Failed)
            {
                lblStatus.Text = "Failed to contact update server";
            }
            else if (updateStatus == UpdateStatus.NoUpdate)
            {
                lblStatus.Text = "";
                lblVersion.Text += " (latest)";
                lblVersion.ForeColor = Color.Green;
            }
            else if (updateStatus == UpdateStatus.UpdateAvailable)
            {
                lblStatus.Text = "Update available for download";
            }
        }

        private void UpdateWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            // Get the worker instance from the sender object.
            BackgroundWorker worker = (BackgroundWorker)sender;

            // Check to see if there is an update available.
            if (AutoUpdate.CheckForUpdates(out Version newVersion, out string changeLog, out string downloadUrl) == true)
            {
                // There is an update available, set the worker result.
                (Version, string, string) updateInfo = (newVersion, changeLog, downloadUrl);
                e.Result = updateInfo;

                // Update UI.
                worker.ReportProgress(0, UpdateStatus.UpdateAvailable);
            }
            else if (newVersion != null)
            {
                // Successfully checked for updates but did not find a new version.
                worker.ReportProgress(0, UpdateStatus.NoUpdate);
            }
            else
            {
                // An error occurred while contacting the update server.
                worker.ReportProgress(0, UpdateStatus.Failed);
            }
        }

        private void UpdateWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            // If the result is valid get the update info and pop a update dialog.
            if (e.Result != null)
            {
                // Get the update info tuple.
                (Version newVersion, string changeLog, string downloadUrl) updateInfo = ((Version, string, string))e.Result;

                // Prompt the user to install the update.
                UpdateDialog updateDialog = new UpdateDialog(updateInfo.newVersion, updateInfo.changeLog);
                if (updateDialog.ShowDialog() == DialogResult.OK)
                {
                    // Format local file path for the update download.
                    string updateFilePath = Application.StartupPath + "\\Update.zip";

                    // Initialize the loading dialog and display it.
                    LoadingDialog downloadDialog = new LoadingDialog("Downloading...");

                    // Create the web client to download the update file.
                    WebClient downloadClient = new WebClient();

                    downloadClient.DownloadProgressChanged += new DownloadProgressChangedEventHandler(delegate (object _sender, DownloadProgressChangedEventArgs _e)
                    {
                        // Update download progress.
                        downloadDialog.UpdateProgress(100, _e.ProgressPercentage);
                    });

                    downloadClient.DownloadFileCompleted += new AsyncCompletedEventHandler(async delegate (object _sender, AsyncCompletedEventArgs _e)
                    {
                        // Check if the download was successfull.
                        if (_e.Cancelled == true || _e.Error != null)
                        {
                            // Failed to download the update.
                            MessageBox.Show("Failed to download update. Please try again later or manually download the update at: " + updateInfo.downloadUrl);
                            downloadDialog.Close();
                            return;
                        }

                        // Extract the updater application from the update zip.
                        if (await AutoUpdate.ExtractUpdaterApp(updateFilePath) == false)
                        {
                            // Failed to extract the updater application.
                            MessageBox.Show("Failed to extract updater application from update file.\nPlease manually download the update at: " + updateInfo.downloadUrl);
                            return;
                        }

                        // Run the updater application and quit.
                        Process.Start(Application.StartupPath + "\\Updater.exe");
                        Application.Exit();
                    });

                    // Start the file download.
                    downloadClient.DownloadFileAsync(new Uri(updateInfo.downloadUrl), updateFilePath);

                    // Show the loading dialog.
                    downloadDialog.ShowDialog();
                }
            }
        }

#endregion

        private void BugFix_ConvertSettingsToAscii()
        {
            // Check if the settings file exists.
            if (File.Exists(Application.StartupPath + "\\DeadRisingEx.ini") == true)
            {
                // Open the file for reading.
                BinaryReader reader = new BinaryReader(new FileStream(Application.StartupPath + "\\DeadRisingEx.ini", FileMode.Open, FileAccess.Read, FileShare.Read));

                // Read the first byte of the file and close the reader.
                byte unicMarking = reader.ReadByte();
                reader.Close();

                // Check if the file is unicode encoded or not.
                if (unicMarking == 0xEF)
                {
                    // Parse the config file and then write it back in ascii.
                    DeadRisingExConfig configData = new DeadRisingExConfig(Application.StartupPath + "\\DeadRisingEx.ini");
                    configData.ReadFromFile();

                    // Write back in ascii encoding.
                    configData.WriteToFile();
                }
            }
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

        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            // Check if the left button is being held.
            if (e.Button == MouseButtons.Left)
            {
                // If the mouse is not hovering the whiteboard adjust the window position based on mouse movement.
                if ((e.X >= this.pnlWhiteboard.Location.X && e.X <= this.pnlWhiteboard.Location.X + this.pnlWhiteboard.Size.Width &&
                    e.Y >= this.pnlWhiteboard.Location.Y && e.Y <= this.pnlWhiteboard.Location.Y + this.pnlWhiteboard.Size.Height) == false)
                {
                    ReleaseCapture();
                    SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
                }
            }
        }
    }
}
