using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;


namespace StockMarket
{
    

    public partial class MainGui : Form
    {

        string TenTapTin;
        ArrayList TenFiles = new ArrayList();
        XuLyDuLieuVao a = new XuLyDuLieuVao();
        DTO.DuLieuMaCK dlck = new DTO.DuLieuMaCK();

        public MainGui()
        {
            InitializeComponent();
        }

    

        private void MainGui_Menu_ChucNang_XacDinhDuongXuThe_Click(object sender, EventArgs e)
        {
            if (dlck.LaySoLuong() > 0)
            {
                DuongXuTheGui dxt = new DuongXuTheGui(dlck.LayDuLieu(),MainGui_CB_MACK.Text);
                dxt.ShowDialog();
            }
            else
            {
                MessageBox.Show("Lỗi! Không có dữ liệu.");
            }
        }

        private void MainGui_Menu_TapTin_Thoat_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void MainGui_Load(object sender, EventArgs e)
        {

        }

        private void MainGui_Menu_TapTin_MoTapTin_Click(object sender, EventArgs e)
        {
            while (true)
            {
                MainGui_OFDialog.ShowDialog();

                TenTapTin = MainGui_OFDialog.FileName;
                if (!a.KiemTraDuLieuChungKhoangDaTonTai(TenFiles, TenTapTin))
                {
                    if (TenTapTin != "")
                    {
                        TenFiles.Add(TenTapTin);
                        //dlck.DocDuLieu(Convert.ToString(TenFiles[TenFiles.Count - 1]));
                        MainGui_CB_MACK.Items.Add(a.LayTenFileKhongCoExtend(TenTapTin).ToUpper());
                        MainGui_CB_MACK.Text = a.LayTenFileKhongCoExtend(TenTapTin).ToUpper();
                    }
                    break;
                }
                else
                {
                    MessageBox.Show("Tập tin đã được mở !");
                }
            }
        }

        private void MainGui_CB_MACK_SelectedIndexChanged(object sender, EventArgs e)
        {
            int Pos;
            dlck.ResetDuLieu();

            Pos = MainGui_CB_MACK.SelectedIndex;

            dlck.DocDuLieu(Convert.ToString(TenFiles[Pos]));
        }

        public DTO.DuLieuMaCK LayDuLieu()
        {
            return dlck;
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            
        }

        private void sMAToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SMA Sma = new SMA(dlck);
            Sma.ShowDialog();

        }
    
    }
}