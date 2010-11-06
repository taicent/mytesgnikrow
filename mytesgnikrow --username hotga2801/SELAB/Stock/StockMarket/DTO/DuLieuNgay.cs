using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace DTO
{
    public class DuLieuNgay
    {
        public string MACK;
        public DateTime Ngay;
        public  Double GiaMoCua;
        public Double GiaCaoNhat;
        public Double GiaThapNhat;
        public Double GiaDongCua;
        public uint KhoiLuongGD;
        public Double GiaTran;
        public Double GiaSan;
        public Double GiaThamChieu;
        public Double TangGiam;
        public Double PhanTram;
        public uint GDThoaThuan;
        public int NNMua;
        public int NNBan;

        public void LayDulieu(string str)
        {
            string[] str2 = new string[15];
            str2 = str.Split(',');
            MACK = str2[0];
            Ngay = Convert.ToDateTime(str2[1]);
            GiaMoCua = Convert.ToDouble(str2[2]);
            GiaCaoNhat = Convert.ToDouble(str2[3]);
            GiaThapNhat = Convert.ToDouble(str2[4]);
            GiaDongCua = Convert.ToDouble(str2[5]);
            KhoiLuongGD = Convert.ToUInt32(str2[6]);
            GiaTran = Convert.ToDouble(str2[7]);
            GiaSan = Convert.ToDouble(str2[8]);
            GiaThamChieu = Convert.ToDouble(str2[9]);
            TangGiam = Convert.ToDouble(str2[10]);
            PhanTram = Convert.ToDouble(str2[11]);
            GDThoaThuan = Convert.ToUInt32(str2[12]);
            NNMua = Convert.ToInt32(str2[13]);
            NNBan = Convert.ToInt32(str2[14]);
        }
    }
}
