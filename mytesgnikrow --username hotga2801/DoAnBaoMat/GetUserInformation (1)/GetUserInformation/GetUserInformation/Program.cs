using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace GetUserInformation
{
    class Program
    {
        [DllImport("netapi32.dll", SetLastError = true)]
        static extern int NetWkstaUserGetInfo(
            [MarshalAs(UnmanagedType.LPWStr)]string reserved,
            int level, out IntPtr lpBuffer);

        [DllImport("Netapi32.dll", CharSet = CharSet.Unicode, ExactSpelling = true)]
        private extern static int NetUserGetInfo(
            [MarshalAs(UnmanagedType.LPWStr)] string ServerName,
            [MarshalAs(UnmanagedType.LPWStr)] string UserName,
            int level,
            out IntPtr BufPtr);

        [DllImport("netapi32.dll", SetLastError = true)]
        static extern int NetApiBufferFree(IntPtr Buffer);

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct USER_INFO_1
        {
            [MarshalAs(UnmanagedType.LPWStr)]
            public string sUsername;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string sPassword;
            public uint uiPasswordAge;
            public uint uiPriv;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string sHome_Dir;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string sComment;
            public uint uiFlags;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string sScript_Path;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct WKSTA_USER_INFO_1
        {
            [MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPWStr)]
            public string wkui1_username;

            [MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPWStr)]
            public string wkui1_logon_domain;

            [MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPWStr)]
            public string wkui1_oth_domains;

            [MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPWStr)]
            public string wkui1_logon_server;
        }

        public static string AccountGetName()
        {
            string sUsername = "";
            IntPtr buffer = new IntPtr();
            int result;
            WKSTA_USER_INFO_1 wksInfo;

            result = NetWkstaUserGetInfo(null, 1, out buffer);
            if (result == 0)
            {
                Int32 pointer = buffer.ToInt32();

                wksInfo = (WKSTA_USER_INFO_1)Marshal.PtrToStructure(
                    new IntPtr(pointer), typeof(WKSTA_USER_INFO_1));

                sUsername = wksInfo.wkui1_username;
                NetApiBufferFree(buffer);

            }

            return sUsername;

        }

        public static USER_INFO_1 AccountGetPassword(string sAccountName)
        {
            //string sPassword = "";

            USER_INFO_1 objUserInfo1 = new USER_INFO_1();
            IntPtr bufPtr; // because it's an OUT, we don't need to Alloc

            int lngReturn = NetUserGetInfo(null, sAccountName, 1, out bufPtr);
            if (lngReturn == 0)
            {
                objUserInfo1 = (USER_INFO_1)Marshal.PtrToStructure(bufPtr, typeof(USER_INFO_1));
                //sPassword = objUserInfo1.sPassword;
            }
            NetApiBufferFree(bufPtr);

            return objUserInfo1;
        }

        static void Main(string[] args)
        {
            string sPassword;
            string sUserName;
            USER_INFO_1 objUserInfo1;
            sUserName = AccountGetName();
            objUserInfo1 = AccountGetPassword(sUserName);

            System.Console.WriteLine("UserName: {0}", sUserName);
            System.Console.WriteLine("Password: {0}", objUserInfo1.sPassword);
            System.Console.WriteLine("Password Age: {0}", objUserInfo1.uiPasswordAge);
            System.Console.WriteLine("home dir : {0}", objUserInfo1.sHome_Dir);
            System.Console.WriteLine("Comment : {0}", objUserInfo1.sComment);
            System.Console.WriteLine("Script Path : {0}", objUserInfo1.sScript_Path);
        }
    }
}
