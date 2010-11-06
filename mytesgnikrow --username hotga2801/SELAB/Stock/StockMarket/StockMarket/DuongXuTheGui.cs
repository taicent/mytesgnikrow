using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace StockMarket
{
    public partial class DuongXuTheGui : Form
    {
        XuLyDuLieuVao a = new XuLyDuLieuVao();
        ArrayList dlck;

        public DuongXuTheGui()
        {
            
            InitializeComponent();
            
        }

        public DuongXuTheGui(ArrayList DLDK)
        {
            this.dlck = DLDK;
            InitializeComponent();

        }

        private void DuongXuTheGui_Load(object sender, EventArgs e)
        {
            DTO.DuLieuNgay dln1,dln2;

            dln1 = (DTO.DuLieuNgay)dlck[dlck.Count - 1];
            dln2 = (DTO.DuLieuNgay)dlck[0];


            for (int i = 1; i < 32; i++)
            {
                DuongXuTheGui_T1_Ngay.Items.Add(Convert.ToString(i));
            }

            for (int i = 1; i < 13; i++)
            {
                DuongXuTheGui_T1_Thang.Items.Add(Convert.ToString(i));
            }

            for (int i = dln1.Ngay.Year ; i <= dln2.Ngay.Year ; i++)
            {
                DuongXuTheGui_T1_Nam.Items.Add(Convert.ToString(i));
            }

            for (int i = 1; i < 32; i++)
            {
                DuongXuTheGui_T2_Ngay.Items.Add(Convert.ToString(i));
            }

            for (int i = 1; i < 13; i++)
            {
                DuongXuTheGui_T2_Thang.Items.Add(Convert.ToString(i));
            }

            for (int i = dln1.Ngay.Year; i <= dln2.Ngay.Year; i++)
            {
                DuongXuTheGui_T2_Nam.Items.Add(Convert.ToString(i));
            }

            
           

            label11.Text = dln1.Ngay.Day.ToString() + "\\" +dln1.Ngay.Month.ToString() + "\\" + dln1.Ngay.Year.ToString() ;
            label12.Text = dln2.Ngay.Day.ToString() + "\\" + dln2.Ngay.Month.ToString() + "\\" + dln2.Ngay.Year.ToString();

           
        }

        private void DuongXuTheGui_BT_ThucHien_Click(object sender, EventArgs e)
        {
            DateTime t1, t2;
            DTO.DuLieuNgay dln = new DTO.DuLieuNgay();
            BUS.DuongXuThe dxt = new BUS.DuongXuThe();
            


            t1 = new DateTime(Convert.ToInt32(DuongXuTheGui_T1_Nam.Text),Convert.ToInt32(DuongXuTheGui_T1_Thang.Text),Convert.ToInt32(DuongXuTheGui_T1_Ngay.Text));
            t2 = new DateTime(Convert.ToInt32(DuongXuTheGui_T2_Nam.Text), Convert.ToInt32(DuongXuTheGui_T2_Thang.Text), Convert.ToInt32(DuongXuTheGui_T2_Ngay.Text));

            if (t2 > t1)
            {

                dln = (DTO.DuLieuNgay)dlck[dlck.Count - 1];

                if (a.KiemTraNhoHonNgayBatDau(t1, dln.Ngay))
                {
                    MessageBox.Show("Lỗi! Ngày bắt đầu nhỏ hơn thời gian tối thiểu." + Convert.ToString(dln.Ngay.Day) + "\\" + Convert.ToString(dln.Ngay.Month) + "\\" + Convert.ToString(dln.Ngay.Year));
                }
                else
                {
                    dln = (DTO.DuLieuNgay)dlck[0];

                    if (a.KiemTraLonHonNgayKetThuc(t2, dln.Ngay))
                    {
                        MessageBox.Show("Lỗi! Ngày kết thúc lớn hơn thời gian tối thiểu." + Convert.ToString(dln.Ngay.Day) + "\\" + Convert.ToString(dln.Ngay.Month) + "\\" + Convert.ToString(dln.Ngay.Year));
                    }
                    else
                    {

                    }
                }


                if (dxt.XacDinhXuTheTangGiam(t1, t2, dlck) == 1)
                {
                    DXTGui_TB.Text = "Xu thế tăng và không đổi chiều!";
                }
                if (dxt.XacDinhXuTheTangGiam(t1, t2, dlck) == 2)
                {
                    DXTGui_TB.Text = "Xu thế giảm và không đổi chiều!";
                }
                if (dxt.XacDinhXuTheTangGiam(t1, t2, dlck) == 3)
                {
                    DXTGui_TB.Text = "Xu thế tăng và có đổi chiều!";
                }
                if (dxt.XacDinhXuTheTangGiam(t1, t2, dlck) == 4)
                {
                    DXTGui_TB.Text = "Xu thế giảm và có đổi chiều!";
                }
            }
            else
            {
                MessageBox.Show("Lỗi! Ngày kết thúc bé hơn hoặc bằng ngày bắt đầu.");
            }

        }

        private void DuongXuTheGui_BT_Thoat_Click(object sender, EventArgs e)
        {
            Close();
        }


    }
}