using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DeadRisingLauncher
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            // If this is the first run after an update upgrade any previous settings.
            if (Properties.Settings.Default.UpdateSettings)
            {
                Properties.Settings.Default.Upgrade();
                Properties.Settings.Default.UpdateSettings = false;
                Properties.Settings.Default.Save();
            }

            // Set the update flavor and save the settings file.
#if DEBUG
            Properties.Settings.Default.UpdateFlavor = UpdateFlavor.Beta.ToString();
#else
            Properties.Settings.Default.UpdateFlavor = UpdateFlavor.Release.ToString();
#endif
            Properties.Settings.Default.Save();

            try
            {
                // Delete the old updated application if found.
                File.Delete(Application.StartupPath + "\\Updater.exe");
            }
            catch (Exception e)
            { }

            // Check if the -silent argument was provided.
            if (args.Length > 0 && args[0] == "-silent")
            {
                // Launch the game with the DeadRisingEx dll.
                if (DeadRisingEx.LaunchDeadRisingEx(Application.StartupPath) == false)
                {
                    // Display an error to the user.
                    MessageBox.Show("Failed to start DeadRisingEx!");
                    return;
                }
            }
            else
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new Form1());
            }
        }
    }
}
