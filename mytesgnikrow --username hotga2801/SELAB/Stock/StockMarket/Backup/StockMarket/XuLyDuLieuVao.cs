using System;
using System.Collections;
using System.Text;

namespace StockMarket
{
    public class XuLyDuLieuVao
    {
        public string LayTenFileKhongCoExtend(string TenFile)
        {
            string Ten;
            int s, e;
            s = TenFile.LastIndexOf('\\');
            e = TenFile.LastIndexOf('.');
            Ten = TenFile.Substring(s+1, e -s-1);
            return Ten;
        }

        public bool KiemTraDuLieuChungKhoangDaTonTai(ArrayList TenFiles, string TenFile_MoiVao)
        {
            for (int i = 0; i < TenFiles.Count; i++)
            {
                if (Convert.ToString(TenFiles[i]) == TenFile_MoiVao)
                    return true;
            }
            return false;
        }

        public bool KiemTraNhoHonNgayBatDau(DateTime T1,DateTime S)
        {
            if (T1 < S)
                return true;
            else
                return false;
        }

        public bool KiemTraLonHonNgayKetThuc(DateTime T2,DateTime E)
        {
            if (T2 > E)
                return true;
            else
                return false;
        }
    }
}
