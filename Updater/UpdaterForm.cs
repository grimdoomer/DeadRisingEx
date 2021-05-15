using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Updater
{
    public partial class UpdaterForm : Form
    {
        BackgroundWorker updateWorker = null;

        public UpdaterForm()
        {
            InitializeComponent();
        }

        private void UpdaterForm_Load(object sender, EventArgs e)
        {
            Process[] processes = null;

            do
            {
                // Check for any launcher processes running.
                processes = Process.GetProcessesByName("DeadRisingLauncher.exe");
                if (processes.Length > 0)
                {
                    // Display an error to the user.
                    MessageBox.Show("Please close all DeadRisingLauncher windows before continuing!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }

                // Check for any running game processes.
                processes = Process.GetProcessesByName("DeadRising.exe");
                if (processes.Length > 0)
                {
                    // Display an error to the user.
                    MessageBox.Show("Please close all DeadRising game windows before continuing!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            while (processes.Length > 0);

            // Initialize the background worker.
            this.updateWorker = new BackgroundWorker();
            this.updateWorker.WorkerReportsProgress = true;
            this.updateWorker.WorkerSupportsCancellation = true;
            this.updateWorker.DoWork += UpdateWorker_DoWork;
            this.updateWorker.ProgressChanged += UpdateWorker_ProgressChanged;
            this.updateWorker.RunWorkerCompleted += UpdateWorker_RunWorkerCompleted;

            // Run the background worker.
            this.updateWorker.RunWorkerAsync();
        }

        private void UpdateWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            // Get the worker instance from the sender param.
            BackgroundWorker worker = (BackgroundWorker)sender;

            try
            {
                // Open the update zip file.
                using (ZipArchive zipFile = ZipFile.Open(Application.StartupPath + "\\Update.zip", ZipArchiveMode.Read))
                {
                    // Report the number of files to be extracted.
                    worker.ReportProgress(zipFile.Entries.Count, null);

                    // Loop through all of the files and extract each one.
                    for (int i = 0; i < zipFile.Entries.Count; i++)
                    {
                        // Check if we should cancel the operation.
                        if (e.Cancel == true)
                        {
                            // Set the result and return.
                            e.Result = false;
                            return;
                        }

                        // Check if this is the updater.exe file and if so skip it.
                        if (zipFile.Entries[i].FullName.Equals("Updater.exe", StringComparison.OrdinalIgnoreCase) == true)
                            continue;

                        // Update the file name being copied.
                        worker.ReportProgress(i, zipFile.Entries[i].Name);

                        // Check if this is a file or directory.
                        string fileName = string.Format("{0}\\{1}", Application.StartupPath, zipFile.Entries[i].FullName.Replace('/', '\\'));
                        if ((zipFile.Entries[i].ExternalAttributes & 0x10) != 0)
                        {
                            // Entry is a directory, create it if it does not already exist.
                            Directory.CreateDirectory(fileName);
                        }
                        else
                        {
                            // Extract the file to the running directory.
                            zipFile.Entries[i].ExtractToFile(fileName, true);
                        }
                    }

                    // Report progress one more time and then wait a second for the file system to finalize any file writes.
                    worker.ReportProgress(zipFile.Entries.Count, "");
                    Task.Delay(1500);

                    // Set the worker result.
                    e.Result = true;
                }

                // Delete the update zip file.
                File.Delete(Application.StartupPath + "\\Update.zip");
            }
            catch (Exception exception)
            {
                // Failed to open the zip file or copy files.
                MessageBox.Show("Failed to install update!", "Update failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
                e.Result = false;
            }
        }

        private void UpdateWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            // Check if the progress being reported is the number of files to copy or not.
            if (e.UserState == null)
            {
                // Set the number of files to copy.
                this.progressBar1.Maximum = e.ProgressPercentage;
            }
            else
            {
                // Update the file currently being copied.
                this.lblFileName.Text = (string)e.UserState;
                this.progressBar1.Value++;
            }
        }

        private void UpdateWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            // Check the update result and handle accordingly.
            if (e.Result != null && (bool)e.Result == true)
            {
                // Start the updated arctool process.
                Process.Start("DeadRisingArcTool.exe");
            }

            // Quit the updated application.
            Application.Exit();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            // Flag the worker to cancel the update.
            this.updateWorker.CancelAsync();
        }
    }
}
