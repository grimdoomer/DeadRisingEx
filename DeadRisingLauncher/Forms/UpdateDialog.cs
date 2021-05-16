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
    public partial class UpdateDialog : Form
    {
        public UpdateDialog(Version version, string changeLog)
        {
            InitializeComponent();

            // Setup the changelog textbox.
            this.textBox1.Text = string.Format("Version {0}\r\n\r\n{1}", version.ToString(), changeLog.Replace("\n", "\r\n"));
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            // Stop the textbox from selecting all text.
            this.textBox1.SelectionStart = this.textBox1.Text.Length;
            this.textBox1.DeselectAll();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            // Set the dialog result and close.
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            // Set the dialog result and close.
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}
