using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.IO;
using System.Collections;
using System.Windows.Forms;

namespace StockMarket
{
    public partial class SMA : Form
    {
        string TenFile;
        BUS.Multi_Stock ms;
        ArrayList dldk;
        
        public SMA()
        {
            InitializeComponent();
        }

        public SMA(DTO.DuLieuMaCK DLDK)
        {
            InitializeComponent();
            ms = new BUS.Multi_Stock();
            ms.ConvertFromDuLieuMaCk(DLDK);
            dldk = DLDK.LayDuLieu();
        }

        private void SMA_SaveFile_Click(object sender, EventArgs e)
        {
            SMA_SaveFileDlg.ShowDialog();
            TenFile = SMA_SaveFileDlg.FileName;
            SMA_FilePath.Text = TenFile;

        }

        private void SMA_Ok_Click(object sender, EventArgs e)
        {
            BUS.Guess_SMA gSMA = new BUS.Guess_SMA();
            BUS.Guess_SMA gSMA2 = new BUS.Guess_SMA();
            BUS.CCI Cci = new BUS.CCI();
            BUS.SMA sma5 = new BUS.SMA();
            BUS.SMA sma10 = new BUS.SMA();
            BUS.SMA sma20 = new BUS.SMA();
            BUS.SMA sma50 = new BUS.SMA();

            ArrayList fSma = new ArrayList();

            Cci.MS1 = ms;

            Cci.Calculate_CCI();

            sma5.MS1 = ms;
            sma5.SMA_Calculation(5);
            gSMA.Sm5 = sma5;
            gSMA2.Sm5 = sma5;
            fSma.Add(sma5.FVals1);

            sma10.MS1 = ms;
            sma10.SMA_Calculation(10);
            gSMA.Sm10 = sma10;
            gSMA2.Sm10 = sma10;
            fSma.Add(sma10.FVals1);

            sma20.MS1 = ms;
            sma20.SMA_Calculation(20);
            gSMA.Sm20 = sma20;
            gSMA2.Sm20 = sma20;
            fSma.Add(sma20.FVals1);

            sma50.MS1 = ms;
            sma50.SMA_Calculation(50);
            gSMA.Sm50 = sma50;
            gSMA2.Sm50 = sma50;
            fSma.Add(sma50.FVals1);

            gSMA.Guess_use_SMA5_SMA20();
            gSMA2.Guess_use_SMA10_SMA50();


            WriteFile(sma5, sma10, sma20, sma50, gSMA, gSMA2,Cci);

            WriteFileGuessCCI(Cci);

            Close();
        }

        
        private void WriteFile(BUS.SMA sma5, BUS.SMA sma10, BUS.SMA sma20, BUS.SMA sma50, BUS.Guess_SMA gSMA5_20, BUS.Guess_SMA gSMA10_50, BUS.CCI Cci)
        {
            string buff;
            DTO.DuLieuNgay dln;
            int i5, i20, i10, i50, igSma5_20, igSma10_50,iCCI;

            i5 = 0;
            i50 = 0;
            i10 = 0;
            i20 = 0;
            igSma5_20 = 0;
            igSma10_50 = 0;
            iCCI = 0;
           
            StreamWriter sw = new StreamWriter(TenFile);
            sw.WriteLine("MaCK,Ngay,GiaMoCua,GiaCaoNhat,GiaThapNhat,GiaDongCua,KhoiLuongGD,GiaTran,GiaSan,GiaThamChieu,TangGiam,%,GDThoaThuan,NNMua,NNBanCOM,SMA5,SMA10,SMA20,SMA50,GUESS_SMA5_SMA20,GUESS_SMA10_SMA50,CCI");
            for (int i = 0; i < dldk.Count; i++ )
            {
                dln = (DTO.DuLieuNgay)dldk[i];
                buff = dln.MACK.ToString() + ",";
                buff = buff + dln.Ngay.ToString() + ",";
                buff = buff + dln.GiaMoCua.ToString() + ",";
                buff = buff + dln.GiaCaoNhat.ToString() + ",";
                buff = buff + dln.GiaThapNhat.ToString() + ",";
                buff = buff + dln.GiaDongCua.ToString() + ",";
                buff = buff + dln.KhoiLuongGD.ToString() + ",";
                buff = buff + dln.GiaTran.ToString() + ",";
                buff = buff + dln.GiaSan.ToString() + ",";
                buff = buff + dln.GiaThamChieu.ToString() + ",";
                buff = buff + dln.TangGiam.ToString() + ",";
                buff = buff + dln.PhanTram.ToString() + ",";
                buff = buff + dln.GDThoaThuan.ToString() + ",";
                buff = buff + dln.NNMua.ToString() + ",";
                buff = buff + dln.NNBan.ToString() + ",";

                if ((i5 < sma5.DDate.Count) && (sma5.DDate[i5] == dln.Ngay))
                {
                    buff = buff + sma5.FVals1[i5].ToString() + ",";
                    i5++;
                }
                else
                {
                    buff = buff + "null,";
                }

                if ((i10 < sma10.DDate.Count) && (sma10.DDate[i10] == dln.Ngay))
                {
                    buff = buff + sma10.FVals1[i10].ToString() + ",";
                    i10++;
                }
                else
                {
                    buff = buff + "null,";
                }

                if ((i20 < sma20.DDate.Count) && (sma20.DDate[i20] == dln.Ngay))
                {
                    buff = buff + sma20.FVals1[i20].ToString() + ",";
                    i20++;
                }
                else
                {
                    buff = buff + "null,";
                }

                if ((i50 < sma50.DDate.Count) && (sma50.DDate[i50] == dln.Ngay))
                {
                    buff = buff + sma50.FVals1[i50].ToString() + ",";
                    i50++;
                }
                else
                {
                    buff = buff + "null,";
                }

                if ((igSma5_20 < gSMA5_20.Gds1.Count) && (gSMA5_20.Gds1[igSma5_20] == dln.Ngay))
                {
                    buff = buff + gSMA5_20.G1[igSma5_20].ToString() + ",";
                    igSma5_20++;
                }
                else
                {
                    buff = buff + "null,";
                }

                if ((igSma10_50 < gSMA10_50.Gds1.Count) && (gSMA10_50.Gds1[igSma10_50] == dln.Ngay))
                {
                    buff = buff + gSMA10_50.G1[igSma10_50].ToString() + ",";
                    igSma10_50++;
                   
                }
                else
                {
                    buff = buff + "null,";
                }

                if ((iCCI < Cci.Date.Count) && (Cci.Date[iCCI] == dln.Ngay))
                {
                    buff = buff + Cci.CCI1[iCCI].ToString();
                    iCCI++;

                }
                else
                {
                    buff = buff + "null";
                }
                
                sw.WriteLine(buff);
               
            }
            sw.Close();
        }

        private void WriteFileGuessCCI(BUS.CCI Cci)
        {
            TenFile = TenFile.Substring(0, TenFile.Length - 4); 
            StreamWriter sw = new StreamWriter(TenFile + "_GuessCCI.txt");

            int count = 0;                              // nguong gia tri lua chon xu huong tang hay giam , co the dung cach xac dinh khac thay cho count
            for (int i = 0; i < Cci.MS1.Size - 1; i++)
            {
                if (Cci.CCI1[i] * Cci.CCI1[i + 1] <= 0)
                {
                    if (count > 3)
                    {
                        if (Cci.CCI1[i] > 0)
                        {
                            sw.WriteLine("-Tu ngay {0} den ngay {1} co xu huong tang", Cci.Date[i - count], Cci.Date[i]);
                        }
                        if (Cci.CCI1[i] < 0)
                        {
                            sw.WriteLine("-Tu ngay {0} den ngay {1} co xu huong giam", Cci.Date[i - count], Cci.Date[i]);
                        }
                    }
                    count = 0;
                }
                else
                {
                    count++;
                }
                if (Cci.CCI1[i] > 100)
                {
                    sw.WriteLine("-Thi truong ngay {0} di vao vung MUA QUA MUC", Cci.Date[i]);
                }
                if (Cci.CCI1[i] < -100)
                {
                    sw.WriteLine("-Thi truong ngay {0} di vao vung BAN QUA MUC", Cci.Date[i]);
                }
            }

            sw.Close();
        }
    
    }


}