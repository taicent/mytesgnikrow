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
        bool PaintGrapFlag;
        DateTime t1, t2;
        int iMax;
        int iMin;
        string stockCode;

        public DuongXuTheGui()
        {
            
            InitializeComponent();
            
        }

        public DuongXuTheGui(ArrayList DLDK, string StockCode)
        {
            this.dlck = DLDK;
            PaintGrapFlag = false;
            stockCode = StockCode;
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
            label14.Text = stockCode;
           
        }

        private void DuongXuTheGui_BT_ThucHien_Click(object sender, EventArgs e)
        {

            DTO.DataDuongXuThe dataDXT = new DTO.DataDuongXuThe();

            DTO.DuLieuNgay dln = new DTO.DuLieuNgay();
            BUS.DuongXuThe dxt = new BUS.DuongXuThe();

            PaintGrapFlag = true;

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

                dataDXT = dxt.XacDinhXuTheTangGiam(t1, t2, dlck);


                switch (dataDXT.value)
                { 
                    case 1:
                        DXTGui_TB.Text = "Xu thế tăng mạnh và không có xu hướng đổi chiều!";
                        break;
                    case 2:
                        DXTGui_TB.Text = "Xu thế tăng vừa và không có xu hướng đổi chiều!";
                        break;
                    case 3:
                        DXTGui_TB.Text = "Xu thế tăng yếu và không có xu hướng đổi chiều!";
                        break;
                    case 4:
                        DXTGui_TB.Text = "Xu thế giãm và không có xu hướng đổi chiều!";
                        break;
                    case 5:
                        DXTGui_TB.Text = "Xu thế tăng mạnh và có xu hướng đổi chiều!";
                        break;
                    case 6:
                        DXTGui_TB.Text = "Xu thế tăng vừa và có xu hướng đổi chiều!";
                        break;
                    case 7:
                        DXTGui_TB.Text = "Xu thế tăng yếu và có xu hướng đổi chiều!";
                        break;
                    case 8:
                        DXTGui_TB.Text = "Xu thế giãm và không có xu hướng đổi chiều!";
                        break;
                        
                }

            }
            else
            {
                MessageBox.Show("Lỗi! Ngày kết thúc bé hơn hoặc bằng ngày bắt đầu.");
            }


            iMax = dataDXT.iMax;
            iMin = dataDXT.iMin;

            Invalidate();

        }

        private void DuongXuTheGui_BT_Thoat_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void DuongXuTheGui_Paint(object sender, PaintEventArgs e)
        {

            Bitmap myBitmap = new Bitmap(365, 200);
            Graphics g = Graphics.FromImage(myBitmap);
            Brush myBlueBrush = new SolidBrush(Color.Blue);
            Pen myBluePen = new Pen(myBlueBrush);
            e.Graphics.DrawRectangle(myBluePen, new Rectangle(96, 240, 366, 201));
            if (PaintGrapFlag)
            {
                PaintGrapFlag = false;
                g.Clear(Color.White);
                PaintGrap(g);
                e.Graphics.DrawImage(myBitmap, 97, 241);
                g.Dispose();
                myBluePen.Dispose();
                myBlueBrush.Dispose();
            }
        }

        void ChangePos(ref Point pos)
        {
            pos.Y = 220 - 2*pos.Y;
        }

        void PaintGrap(Graphics g)
        {
            DTO.DuLieuNgay oldDln,newDln;
            int index;

            Point MaxPos, MinPos;

            Brush myBlueBrush = new SolidBrush(Color.Blue);
            Pen myBluePen = new Pen(myBlueBrush);

            Brush myRedBrush = new SolidBrush(Color.Red);
            Pen myRedPen = new Pen(myRedBrush);

            TimeSpan tempTimeSpan = t2.Subtract(t1);

            int NumberOfDay = tempTimeSpan.Days;

            int DistanceTime = 365 / (NumberOfDay - 1);

            Point OldPoint,NewPoint;

            NewPoint = new Point();
            MaxPos = new Point(NewPoint.X, NewPoint.Y);
            MinPos = new Point(NewPoint.X, NewPoint.Y);

            for (int i = dlck.Count - 1; ; i--)
            {
                newDln = (DTO.DuLieuNgay)dlck[i];

                if (t1 <= newDln.Ngay)
                {
                    index = i;
                    break;
                }
            }

            tempTimeSpan = newDln.Ngay.Subtract(t1);
            NewPoint.X = 0;
            NewPoint.Y = (int)newDln.GiaDongCua;
            OldPoint = NewPoint;
            ChangePos(ref OldPoint);
            NewPoint.X = tempTimeSpan.Days * DistanceTime;
            NewPoint.Y = (int)newDln.GiaDongCua;
            ChangePos(ref NewPoint);
            if (index == iMax)
            {
                MaxPos.X = NewPoint.X;
                MaxPos.Y = NewPoint.Y;
            }

            if (index == iMin)
            {
                MinPos.X = NewPoint.X;
                MinPos.Y = NewPoint.Y;
            }

            g.DrawLine(myBluePen, OldPoint, NewPoint);

            for (int i = index-1; ; i--)
            {
                oldDln = newDln;
                newDln = (DTO.DuLieuNgay)dlck[i];

                if (t2 < newDln.Ngay)
                    break;

                tempTimeSpan = newDln.Ngay.Subtract(oldDln.Ngay);

                OldPoint = NewPoint;
                NewPoint.Y = (int)newDln.GiaDongCua;
                NewPoint.X = NewPoint.X + DistanceTime*tempTimeSpan.Days;
                ChangePos(ref NewPoint);

                if(i == iMax)
                {
                    MaxPos.X = NewPoint.X;
                    MaxPos.Y = NewPoint.Y;
                }

                if (i == iMin)
                {
                    MinPos.X = NewPoint.X;
                    MinPos.Y = NewPoint.Y;
                }


                g.DrawLine(myBluePen, OldPoint, NewPoint);


            }

            g.DrawLine(myRedPen, MaxPos, MinPos);

            myRedPen.Dispose();
            myRedBrush.Dispose();
            myBluePen.Dispose();
            myBlueBrush.Dispose();
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            base.OnPaintBackground(e);
        }
    }
}