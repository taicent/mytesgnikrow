using System;
using System.Collections.Generic;
using System.Text;

namespace BUS
{
    public class Stock
    {
        #region 1.atributes
        private float Close;
        private float High;
        private float Low;
        private float Volume;
        private float Open;
        private DateTime Date;
        #endregion
        #region 2.properties
        public float Close1
        {
            get { return Close; }
            set { Close = value; }
        }
       

        public float High1
        {
            get { return High; }
            set { High = value; }
        }
        
        public float Low1
        {
            get { return Low; }
            set { Low = value; }
        }
        

        public float Volume1
        {
            get { return Volume; }
            set { Volume = value; }
        }
        

        public float Open1
        {
            get { return Open; }
            set { Open = value; }
        }
        

        public DateTime Date1
        {
            get { return Date; }
            set { Date = value; }
        }
        #endregion
        #region 3.constructor
        public Stock()
        {
            this.Close = 0;
            this.High = 0;
            this.Low = 0;
            this.Volume = 0;
            this.Open = 0;
            this.Date = DateTime.Today;
        }
        public Stock(float Close , float High , float Low, float Volume, float Open, DateTime Date)
        {
            this.Close = Close;
            this.High = High;
            this.Low = Low;
            this.Volume = Volume;
            this.Open = Open;
            this.Date = Date;
        }
        #endregion
    /*    #region 4.destructor
        public ~Stock()
        {
            
        }
        #endregion*/
    }

    public class Multi_Stock
    {
        #region 1.atributes
        private List<Stock> MultiStock;

        
        private int size;
        private float Highest;
        private float Lowest;
        
        #endregion
        #region 2.properties
        public List<Stock> MultiStock1
        {
            get { return MultiStock; }
            set { MultiStock = value; }
        }


        public int Size
        {
            get { return size; }
            set { size = value; }
        }


        public float Highest1
        {
            get { return Highest; }
            set { Highest = value; }
        }


        public float Lowest1
        {
            get { return Lowest; }
            set { Lowest = value; }
        }
        #endregion
        #region 3.constructor
        public Multi_Stock()
        {
            this.MultiStock = new List<Stock> ();
            this.size = 0;
            this.Highest = 0;
            this.Lowest = 0;
        }
        public Multi_Stock(Stock Temp)
        {
            this.MultiStock.Add(Temp);
            this.size++;
            this.Highest = Temp.High1;
            this.Lowest = Temp.Low1;
        }
        #endregion
    /*    #region 4.destructor
        public ~Multi_Stock()
        {
               
        }
        #endregion*/
        #region 5.Inserting
        public void Insert(params Stock[] Temp)
        {

            foreach (Stock i in Temp)
            {
                if (this.Highest < i.High1)
                {
                    this.Highest = i.High1;
                }
                if (this.Lowest > i.Low1)
                {
                    this.Lowest = i.Low1;
                }

                this.MultiStock.Add(i);
                this.size++;
            }
          
        }
        #endregion

        public void ConvertFromDuLieuMaCk(DTO.DuLieuMaCK Temp)
        {
            Stock StockTemp; 
            foreach (DTO.DuLieuNgay i in Temp.LayDuLieu())
            {
                if (this.Highest < i.GiaCaoNhat)
                {
                    this.Highest = (float)i.GiaCaoNhat;
                }
                if (this.Lowest > i.GiaThapNhat)
                {
                    this.Lowest = (float)i.GiaThapNhat;
                }
                StockTemp = new Stock(); 

                StockTemp.High1 = (float)i.GiaCaoNhat;
                StockTemp.Low1 = (float)i.GiaThapNhat;
                StockTemp.Volume1 = (float)i.KhoiLuongGD;
                StockTemp.Open1 = (float)i.GiaMoCua;
                StockTemp.Close1 = (float)i.GiaDongCua;
                StockTemp.Date1 = i.Ngay;

                
                this.MultiStock.Add(StockTemp);
                this.size++;
            }
        }
    }

    public class SMA
    {
        #region 1.atribute
        private List<float> fVals;              // danh sach gia tri SMA sau khi tinh toan 
        private List<DateTime> dDate;
        private Multi_Stock MS;                 // du lieu de tinh SMA
        private int time;                       // thoi gian tinh toan chi so SMA , tinh theo so ngay (5,10,20,50 ... ngay)
                                        
                    
        #endregion
        #region 2.properties

        public List<DateTime> DDate
        {
            get { return dDate; }
            set { dDate = value; }
        }
        public List<float> FVals1
        {
            get { return fVals; }
            set { fVals = value; }
        }
        public int Time
        {
            get { return time; }
            set { time = value; }
        }

        public Multi_Stock MS1
        {
            get { return MS; }
            set { MS = value; }
        }
        #endregion
        #region 3.constructor
        public SMA()
        {
            this.fVals = new List<float> ();
            this.dDate = new List<DateTime> ();
            this.MS = new Multi_Stock();
            this.time = 0;
        }
        public SMA(SMA s)
        {
            this.dDate = s.dDate;
            this.fVals = s.fVals;
            this.MS = s.MS;
            this.time = s.time;
            //s.dDate.CopyTo(this.dDate);
            //s.fVals.CopyTo(this.fVals);
            
        }
        #endregion

        #region 4.SMA calculation
        public void SMA_Calculation(int time)
        {
            this.time = time;
            
            if (MS.Size < this.time)
            {
                //Console.WriteLine("data is not correct");
               
                return;
            }

            this.fVals = new List<float>(MS.Size - this.time + 1);
            this.dDate = new List<DateTime>(MS.Size - this.time + 1);

            for (int i = this.time-1; i < MS.Size; i++)
            {
                float value = 0;
                for (int j = 0; j < this.time; j++)
                {
                    value = value + MS.MultiStock1[i - j].Close1;
                }
                value = value / this.time;
                this.fVals.Add(value);
                this.dDate.Add(MS.MultiStock1[i].Date1);
            }
        }

        public void SMA_Reset()
        {
            time = 0;
            fVals.Clear();
            dDate.Clear();
        }



        #endregion
        #region 5.destuctor
        #endregion
    }

    public class Guess_SMA
    {
        #region 1.atributte
        private SMA sm5;
        private SMA sm10;
        private SMA sm20;
        private SMA sm50;
        private List<bool> G;           // 0 = ban ra ; 1 = mua vao

        public List<bool> G1
        {
            get { return G; }
          
        }
        private List<DateTime> Gds;     // ngay ra chi thi ban ra hoac mua vao ung voi G

        public List<DateTime> Gds1
        {
            get { return Gds; }
           
        }
        #endregion
        #region 2.properties
        public SMA Sm5
        {
            get { return sm5; }
            set { sm5 = value; }
        }
       
        public SMA Sm10
        {
            get { return sm10; }
            set { sm10 = value; }
        }
        
        public SMA Sm20
        {
            get { return sm20; }
            set { sm20 = value; }
        }
        

        public SMA Sm50
        {
            get { return sm50; }
            set { sm50 = value; }
        }
        #endregion
        #region 3.constructor
        public Guess_SMA()
        {
            sm5 = new SMA();
            sm10 = new SMA();
            sm20 = new SMA();
            sm50 = new SMA();
        }
        #endregion
        #region 4. Guess
        public void Guess_use_SMA5_SMA20()
        {
            int i = 0;
            this.G = new List<bool>();
            this.Gds = new List<DateTime>();
            while (sm20.DDate[0] >= sm5.DDate[i])
            {
                i++;
                if (i >= sm5.FVals1.Count)
                {
                    //Console.WriteLine("data is not correct");
                    return ;
                }
            }
            int j = 0;
            while (i < sm5.FVals1.Count-1 && j < sm20.FVals1.Count-1)
            {
                float val = sm5.FVals1[i] - sm20.FVals1[j];
                int count = 0;                                              // nguong~ chon lua vung sau khi mua va ban
                while( sm5.FVals1[i] - sm20.FVals1[j] >= 0)
                {
                    count++;

                    if (i + 1 >= sm5.FVals1.Count || j + 1 >= sm20.FVals1.Count)
                        return;
                    
                    if (sm5.FVals1[i + 1] - sm20.FVals1[j + 1] <= 0 && count >= 2)
                    {
                        bool buy = false;
                        G.Add(buy);
                        Gds.Add(sm5.DDate[i+1]);
                    }
                    

                    i++;
                    j++;

                }
                count = 0;
                while (sm5.FVals1[i] - sm20.FVals1[j] <= 0)
                {
                    count++;

                    if (i + 1 >= sm5.FVals1.Count || j + 1 >= sm20.FVals1.Count)
                        return;
                    
                    if (sm5.FVals1[i + 1] - sm20.FVals1[j + 1] >= 0 && count >= 2)
                    {
                        bool buy = true;
                        G.Add(buy);
                        Gds.Add(sm5.DDate[i + 1]);
                    }
                    i++;
                    j++;
                   
                   
                }
            }
        }
        public void Guess_use_SMA10_SMA50()
        {
            int i = 0;
            this.G = new List<bool>();
            this.Gds = new List<DateTime>();
            while (sm50.DDate[0] >= sm10.DDate[i])
            {
                i++;
                if (i >= sm10.FVals1.Count)
                {
                    //Console.WriteLine("data is not correct");
                    return;
                }
            }
            int j = 0;
            while (i < sm10.FVals1.Count - 1 && j < sm50.FVals1.Count - 1)
            {
                float val = sm10.FVals1[i] - sm50.FVals1[j];
                int count = 0;                                              // nguong~ chon lua vung sau khi mua va ban
                while (sm10.FVals1[i] - sm50.FVals1[j] >= 0)
                {
                    count++;

                    if (i + 1 >= sm10.FVals1.Count || j + 1 >= sm50.FVals1.Count)
                        return;

                    if (sm10.FVals1[i + 1] - sm50.FVals1[j + 1] <= 0 && count >= 4)
                    {
                        bool buy = false;
                        G.Add(buy);
                        Gds.Add(sm10.DDate[i + 1]);
                    }
                   

                    i++;
                    j++;

                }
                count = 0;
                while (sm10.FVals1[i] - sm50.FVals1[j] <= 0)
                {
                    count++;

                    if (i +1 >= sm10.FVals1.Count || j+1 >= sm50.FVals1.Count)
                        return;

                    if (sm10.FVals1[i + 1] - sm50.FVals1[j + 1] >= 0 && count >= 2)
                    {
                        bool buy = true;
                        G.Add(buy);
                        Gds.Add(sm10.DDate[i + 1]);
                    }
                    i++;
                    j++;
                   

                }
            }
        }
        #endregion
    }

    public class CCI
    {
        #region 1.atributes
        private float[] TP;
        private float[] SMATP;
        private Multi_Stock MS;

        private float[] MeanDev;
        
        private List<float> Cci;
        private List<DateTime> date;
        #endregion
        #region 2. properties

        public float[] TP1
        {
            get { return TP; }
            set { TP = value; }
        }

        public float[] SMATP1
        {
            get { return SMATP; }
            set { SMATP = value; }
        }


        public float[] MeanDev1
        {
            get { return MeanDev; }
            set { MeanDev = value; }
        }
        
        public List<DateTime> Date
        {
            get { return date; }
            set { date = value; }
        }
        
        public Multi_Stock MS1
        {
            get { return MS; }
            set { MS = value; }
        }

        public List<float> CCI1
        {
            get { return Cci; }
            set { Cci = value; }
        }

        #endregion
        #region 3.constructor
        public CCI()
        {
            //TP = new List<float>(); ;
            //SMATP = new List<float>(); ;
            //MS = new Multi_Stock();
            //MeanDev = new List<float>(); ;
            //Cci = new List<float>();
            //date = new List<DateTime>();

        }
        #endregion
        #region 4. calculate CCI
        public void Calculate_CCI()
        {
            if (MS.Size < 20)
            {
                //Console.WriteLine("data is not correct");
                return;
            }
            this.SMATP = new float[MS.Size];
            this.TP = new float[MS.Size];
            this.MeanDev = new float[MS.Size];
            float sum = 0;

            /*tinh tp va smatp20*/
            for (int i = 0; i < MS.Size; i++)
            {
                TP[i] = (MS.MultiStock1[i].Close1 + MS.MultiStock1[i].High1 + MS.MultiStock1[i].Low1) / 3;
                if (i > 19)
                {
                    SMATP[i] = sum / 20;
                    sum = sum - TP[i - 20];
                }
                else
                {
                    SMATP[i] = 0;
                }
                sum = sum + TP[i];
            }
            /*******************************************/

            /*Tinh chi so CCI*/
            sum = 0;
            this.Cci = new List<float>();
            this.date = new List<DateTime>();
            for (int i = 0; i < MS.Size; i++)
            {
                if (i >= 19)
                {
                    MeanDev[i] = (20 * SMATP[i] - sum) / 20;
                    sum = sum - SMATP[i - 19];
                    float c = (float)(TP[i] - SMATP[i]) / (float)(0.15 * MeanDev[i]);
                    this.Cci.Add(c);
                }
                else
                {
                    MeanDev[i] = 0;
                    this.Cci.Add(0);

                }
                this.date.Add(this.MS.MultiStock1[i].Date1);
                sum = sum + SMATP[i];
            }
            /*******************************************/

        }
        #endregion
        #region 5. Guess by CCI
        public void Guess_CCI()
        {

            int count = 0;                              // nguong gia tri lua chon xu huong tang hay giam , co the dung cach xac dinh khac thay cho count
            for (int i = 0; i < this.MS.Size - 1; i++)
            {
                if (this.Cci[i] * this.Cci[i + 1] <= 0)
                {
                    if (count > 3)
                    {
                        if (Cci[i] > 0)
                        {
                            //Console.WriteLine(" tu ngay {0} den ngay {1} co xu huong tang", this.date[i - count], this.date[i]);
                        }
                        if (Cci[i] < 0)
                        {
                            //Console.WriteLine(" tu ngay {0} den ngay {1} co xu huong giam", this.date[i - count], this.date[i]);
                        }
                    }
                    count = 0;
                }
                else
                {
                    count++;
                }
                if (this.Cci[i] > 100)
                {
                    //Console.WriteLine("thi truong ngay {0} di vao vung MUA QUA MUC", this.date[i]);
                }
                if (this.Cci[i] < -100)
                {
                    //Console.WriteLine("thi truong ngay {0} di vao vung BAN QUA MUC", this.date[i]);
                }
            }
        }
        #endregion
    }
}
