using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DeadRisingLauncher
{
    public partial class SettingsDialog : Form
    {
        // Config file instance.
        DeadRisingExConfig configData;

        public SettingsDialog()
        {
            InitializeComponent();

            // Initialize the config file data instance.
            configData = new DeadRisingExConfig(Application.StartupPath + "\\DeadRisingEx.ini");
            configData.ReadFromFile();
        }

        private void SettingsDialog_Load(object sender, EventArgs e)
        {
            // Get a list of all mod files in the mods directory.
            string modsDirectory = Application.StartupPath + "\\nativeWin64\\Mods";
            string[] modFilesFound = GetArchivesFromModsFolder();

            // Loop through all of the mod files specified in the config file and add them first.
            for (int i = 0; i < this.configData.ModFileLoadOrder.Length; i++)
            {
                // Create a new list view item for the mod file.
                ListViewItem item = new ListViewItem(this.configData.ModFileLoadOrder[i]);
                item.Checked = true;

                // If the archive was not found in the mods directory color it in red.
                if (modFilesFound.Contains(this.configData.ModFileLoadOrder[i], StringComparer.InvariantCultureIgnoreCase) == false)
                {
                    // Flag the archive as missing.
                    item.BackColor = Color.PaleVioletRed;
                    item.ToolTipText = "Archive file missing!";
                }

                // Add the item to the list view.
                this.lstModArchives.Items.Add(item);
            }

            // Get a list of archives that were found but not specified in the config file.
            string[] disabledMods = modFilesFound.Except(this.configData.ModFileLoadOrder, StringComparer.InvariantCultureIgnoreCase).ToArray();
            for (int i = 0; i < disabledMods.Length; i++)
            {
                // Create a new list view item.
                ListViewItem item = new ListViewItem(disabledMods[i]);
                item.Checked = false;
                this.lstModArchives.Items.Add(item);
            }

            // Setup the game settings tab.
            this.chkDebugLog.Checked = this.configData.DebugLog;
            this.chkRecursiveGrenade.Checked = this.configData.RecursiveGrenade;

            // Setup the graphics settings tab.
            this.chkDynamicGraphicsMemory.Checked = this.configData.DynamicGraphicsMemory;
        }

        private string[] GetArchivesFromModsFolder()
        {
            // Make sure the mods folder exists.
            string modsDirectory = Application.StartupPath + "\\nativeWin64\\Mods";
            if (Directory.Exists(modsDirectory) == false)
            {
                // No archives found.
                return new string[0];
            }

            // Get all of the archive files recursively, and remove the mods directory path from the file path.
            return Directory.GetFiles(modsDirectory, "*.arc", SearchOption.AllDirectories).Select(p => p.Substring(modsDirectory.Length + 1)).ToArray();
        }

        private void btnLoadOrderUp_Click(object sender, EventArgs e)
        {
            // Make sure an item is selected and it is not the first item in the list.
            if (this.lstModArchives.SelectedIndices.Count == 0 || this.lstModArchives.SelectedIndices[0] == 0)
                return;

            // Move the selected item up in the list order.
            int index = this.lstModArchives.SelectedIndices[0];
            ListViewItem oldItem = this.lstModArchives.Items[index - 1];
            this.lstModArchives.Items.RemoveAt(index - 1);
            this.lstModArchives.Items.Insert(index, oldItem);
        }

        private void btnLoadOrderDown_Click(object sender, EventArgs e)
        {
            // Make sure the item selected is not the last item in the list.
            if (this.lstModArchives.SelectedIndices.Count == 0 || this.lstModArchives.SelectedIndices[0] == this.lstModArchives.Items.Count - 1)
                return;

            // Move the selected item down in the list order.
            int index = this.lstModArchives.SelectedIndices[0];
            ListViewItem item = this.lstModArchives.Items[index];
            this.lstModArchives.Items.RemoveAt(index);
            this.lstModArchives.Items.Insert(index + 1, item);
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            // Get the new mod file load order.
            List<string> modFiles = new List<string>();
            for (int i = 0; i < this.lstModArchives.Items.Count; i++)
            {
                // Check if the item is checked or not.
                if (this.lstModArchives.Items[i].Checked == true)
                {
                    // Add the mod file to the list.
                    modFiles.Add(this.lstModArchives.Items[i].Text);
                }
            }

            // Update mod file load order.
            this.configData.ModFileLoadOrder = modFiles.ToArray();

            // Update game settings.
            this.configData.DebugLog = this.chkDebugLog.Checked;
            this.configData.RecursiveGrenade = this.chkRecursiveGrenade.Checked;

            // Update graphics settings.
            this.configData.DynamicGraphicsMemory = this.chkDynamicGraphicsMemory.Checked;

            // Write the config file.
            if (this.configData.WriteToFile() == false)
            {
                // Display an error to the user.
                MessageBox.Show("Failed to write config file!");
                return;
            }

            // Close the dialog.
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            // Close the dialog.
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
