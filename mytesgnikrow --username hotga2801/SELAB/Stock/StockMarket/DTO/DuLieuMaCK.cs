using System;
using System.Collections;
using System.Text;
using System.IO;

namespace DTO
{
    public class DuLieuMaCK
    {
        ArrayList DanhSach_dln = new ArrayList();

        public void DocDuLieu(string TenFile)
        {
            DuLieuNgay dln;
            string str1;
            string[] str2 = new string[15];
            StreamReader rf = new StreamReader(TenFile);
            rf.ReadLine();

            while (true)
            {
                str1 = rf.ReadLine();
                if (rf.EndOfStream)
                    break;
                dln = new DuLieuNgay();
                dln.LayDulieu(str1);
                DanhSach_dln.Add(dln); 
            }
        }

        public void ResetDuLieu()
        {
            DanhSach_dln.Clear();
        }

        public ArrayList LayDuLieu()
        {
            return DanhSach_dln;
        }

        public int LaySoLuong()
        {
            return DanhSach_dln.Count;
        }
    }
}
