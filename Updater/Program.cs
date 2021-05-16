using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Updater
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            // If there are no command line arguments then just run the updater as normal.
            if (args.Length == 0)
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new UpdaterForm());
            }
            else
            {
                // Make sure the arguments are correct.
                if (args.Length < 2)
                {
                    // Invalid arguments.
                    return;
                }

                // Get the command line args for packaging the update.
                string buildFolder = args[0];
                string drexDll = args[1];

                // Determine the build flavor.
#if DEBUG
                string buildFlavor = "Beta";
#else
                string buildFlavor = "Release";
#endif

                // Get the build version from the application exe.
                string buildVersion = FileVersionInfo.GetVersionInfo(buildFolder + "DeadRisingLauncher.exe").FileVersion;

                // Remove any old zip files in the build folder.
                string[] oldZipFiles = Directory.GetFiles(buildFolder, "*.zip", SearchOption.TopDirectoryOnly);
                for (int i = 0; i < oldZipFiles.Length; i++)
                {
                    // Delete old zip files.
                    File.Delete(oldZipFiles[i]);
                }

                // Get a recursive list of all files inside the build directory.
                List<string> filesToZip = new List<string>(Directory.GetFiles(buildFolder));
                filesToZip.Add(drexDll);

                // Create a file stream for the zip file.
                string updateZipFile = string.Format("{0}DeadRisingEx_{1}_{2}.zip", buildFolder, buildVersion, buildFlavor);
                using (FileStream zipStream = new FileStream(updateZipFile, FileMode.Create, FileAccess.ReadWrite, FileShare.Read))
                {
                    using (ZipArchive zipFile = new ZipArchive(zipStream, ZipArchiveMode.Create, false))
                    {
                        // Add all of the file entries from the build directory to the zip archive.
                        for (int i = 0; i < filesToZip.Count; i++)
                        {
                            // Add the current file to the zip archive.
                            zipFile.CreateEntryFromFile(filesToZip[i], Path.GetFileName(filesToZip[i]));
                        }

                        // Add the updater application to the zip file.
                        zipFile.CreateEntryFromFile(Application.ExecutablePath, "Updater.exe");
                    }
                }
            }
        }
    }
}
