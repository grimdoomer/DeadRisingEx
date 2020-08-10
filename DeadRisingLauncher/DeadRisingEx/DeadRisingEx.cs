using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace DeadRisingLauncher
{
    public class DeadRisingEx
    {
        [DllImport("DeadRisingEx.dll", CharSet = CharSet.Ansi)]
        public static extern bool LaunchDeadRisingEx(string psGameDirectory);
    }
}
