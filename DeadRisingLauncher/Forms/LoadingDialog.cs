using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DeadRisingLauncher.Forms
{
    public partial class LoadingDialog : Form
    {
        public LoadingDialog(string title)
        {
            InitializeComponent();

            // Set the dialog title.
            this.Text = title;
        }

        public void SetupProgress(int max)
        {
            // Set the progress bar maximum.
            this.progressBar1.Maximum = max;
        }

        public void UpdateProgress(string fileName)
        {
            // Update progress.
            this.lblFileName.Text = fileName;
            this.progressBar1.Value++;
        }

        public void UpdateProgress(int max, int current)
        {
            // Update progress.
            this.progressBar1.Maximum = max;
            this.progressBar1.Value = current;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            // Set the dialog result and close.
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
