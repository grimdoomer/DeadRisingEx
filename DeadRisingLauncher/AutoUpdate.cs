using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DeadRisingLauncher
{
    public enum UpdateFlavor
    {
        Release,
        Beta
    }

    public class AutoUpdate
    {
        public static bool CheckForUpdates(out Version newVersion, out string changeLog, out string downloadUrl)
        {
            // Satisfy the compiler.
            newVersion = null;
            changeLog = "";
            downloadUrl = "";

            try
            {
                // Create a new HTTP request to pull the version file based on the update flavor preference.
                HttpWebRequest webRequest = HttpWebRequest.CreateHttp(string.Format("https://icode4.coffee/files/DeadRisingEx/Version_{0}.txt", Properties.Settings.Default.UpdateFlavor));
                webRequest.Timeout = 5000;

                // Submit the GET request and get the version file.
                using (WebResponse webResponse = webRequest.GetResponse())
                {
                    // Open the response stream in a text reader so we can parse the version and changelog.
                    using (StreamReader reader = new StreamReader(webResponse.GetResponseStream()))
                    {
                        // First line is the version.
                        string version = reader.ReadLine();
                        newVersion = new Version(version);

                        // Compare the versions to see if we should download the update.
                        if (newVersion > Assembly.GetExecutingAssembly().GetName().Version)
                        {
                            // Read the change log and return that there is an update.
                            changeLog = reader.ReadToEnd();
                            downloadUrl = string.Format("https://icode4.coffee/files/DeadRisingArcTool/DeadRisingEx_{0}_{1}.zip", version, Properties.Settings.Default.UpdateFlavor);
                            return true;
                        }
                    }
                }
            }
            catch (Exception e) { }

            // We either failed to check for the update or there is no newer version.
            return false;
        }

        public static async Task<bool> ExtractUpdaterApp(string updateZip)
        {
            try
            {
                // Open the zip file and extract the update exe.
                using (ZipArchive zipFile = ZipFile.Open(updateZip, ZipArchiveMode.Read))
                {
                    // Get the file entry for the update exe and extract it.
                    ZipArchiveEntry updateFileEntry = zipFile.GetEntry("Updater.exe");
                    updateFileEntry.ExtractToFile(Application.StartupPath + "\\Updater.exe", true);

                    // Wait for the file system to catch up.
                    await Task.Delay(1500);
                }

                // Updater successfully extracted.
                return true;
            }
            catch (Exception exception)
            {
                // Failed to extract the updater application.
                return false;
            }
        }
    }
}
