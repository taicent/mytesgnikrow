using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DTO;
using DAO;
using System.Collections;

namespace BUS
{
    public class MACDBUS
    {
        #region Attributes
        private int _numDaysPredicted;  // số ngày sau đó cần được dự đoán
        private List<double> _listEMA12; //List chỉ số EMA 12 ngày của giá đóng cửa
        private List<double> _listEMA26; //List chỉ số EMA 26 ngày của giá đóng cửa
        private List<double> _listMACD; //List chỉ số MACD = EMA12 - EMA26
        private List<double> _listSignalLine; //List chỉ số EMA 9 ngày của chính MACD
        private List<double> _listMACDHistogram; //MACD Histogram = MACD - SignalLine

        private List<double> _closePrice; //Giá đóng cửa của tất cả các ngày giao dịch
        private List<double> _listEMADaysPredicted; //Chỉ số EMA theo số ngày dự đoán

        //Các tin hiệu báo giá sắp tăng (các tín hiệu Bull) hoặc giảm (các tín hiệu Bear)
        private List<int> _listBullMAC; //Tín hiệu Bullish Moving Average Crossover
        private List<int> _listBullCC; //Tín hiệu Bullish Centerline Crossover
        private List<int> _listBearMAC; //Tín hiệu Bearish Moving Average Crossover
        private List<int> _listBearCC; //Tín hiệu Bearish Centerline Crossover

        private int _numBoughts;
        private int _numSales;
        private int _numTotals;
        private int _numTotalBuy;
        private int _numTotalSell;
        private double _precision;
        #endregion

        #region Properties
        public int NumDaysPredicted
        {
            get { return _numDaysPredicted; }
            set { _numDaysPredicted = value; }
        }

        public List<double> ListEMA12
        {
            get { return _listEMA12; }
            set { _listEMA12 = value; }
        }

        public List<double> ListEMA26
        {
            get { return _listEMA26; }
            set { _listEMA26 = value; }
        }

        public List<double> ListMACD
        {
            get { return _listMACD; }
            set { _listMACD = value; }
        }

        public List<double> ListSignalLine
        {
            get { return _listSignalLine; }
            set { _listSignalLine = value; }
        }

        public List<double> ListMACDHistogram
        {
            get { return _listMACDHistogram; }
            set { _listMACDHistogram = value; }
        }

        public List<int> ListBullMAC
        {
            get { return _listBullMAC; }
            set { _listBullMAC = value; }
        }

        public List<int> ListBearMAC
        {
            get { return _listBearMAC; }
            set { _listBearMAC = value; }
        }

        public List<int> ListBullCC
        {
            get { return _listBullCC; }
            set { _listBullCC = value; }
        }

        public List<int> ListBearCC
        {
            get { return _listBearCC; }
            set { _listBearCC = value; }
        }

        public int NumBoughts
        {
            get { return _numBoughts; }
            set { _numBoughts = value; }
        }

        public int NumSales
        {
            get { return _numSales; }
            set { _numSales = value; }
        }

        public int NumTotals
        {
            get { return _numTotals; }
            set { _numTotals = value; }
        }

        public double Precision
        {
            get { return _precision; }
            set { _precision = value; }
        }

        public List<double> ClosePrice
        {
            get { return _closePrice; }
            set { _closePrice = value; }
        }

        public List<double> ListEMADaysPredicted
        {
            get { return _listEMADaysPredicted; }
            set { _listEMADaysPredicted = value; }
        }

        public int NumTotalBuy
        {
            get { return _numTotalBuy; }
            set { _numTotalBuy = value; }
        }

        public int NumTotalSell
        {
            get { return _numTotalSell; }
            set { _numTotalSell = value; }
        }
        #endregion

        #region Method
        public MACDBUS()
        {
            
        }

        private List<double> CalculateEMA(List<double> listValue, int numDaysPeriod)
        {
            List<double> listEMA = new List<double>();

            double alpha = 1 - 2 * 1.0d / (numDaysPeriod + 1);

            for (int i = 0; i < listValue.Count - numDaysPeriod; i++)
            {
                double tu = 0;
                double mau = 0;
                int exp = 0;
                for (int j = i; j < i + numDaysPeriod; j++)
                {
                    mau += Math.Pow(alpha, exp);
                    tu += listValue[j] * Math.Pow(alpha, exp);
                    exp++;
                }
                listEMA.Add(tu / mau);
            }

            for (int i = 0; i < numDaysPeriod; i++)
            {
                listEMA.Add(0d);
            }

            return listEMA;
        }

        private List<double> CalculateSMA(List<double> listValue, int numDaysPeriod)
        {
            List<double> listSMA = new List<double>();

            for (int i = 0; i < listValue.Count - numDaysPeriod; i++)
            {
                double sum = 0;
                for (int j = i; j < i + numDaysPeriod; j++)
                {
                    sum += listValue[j];
                }
                listSMA.Add(sum / numDaysPeriod);
            }

            for (int i = 0; i < numDaysPeriod; i++)
            {
                listSMA.Add(0d);
            }

            return listSMA;
        }

        private void CalculateMACD()
        {
            ListEMA12 = CalculateEMA(ClosePrice, 12);
            ListEMA26 = CalculateEMA(ClosePrice, 26);

            ListMACD = new List<double>();
            for (int i = 0; i < ClosePrice.Count; i++)
            {
                ListMACD.Add(ListEMA12[i] - ListEMA26[i]);
            }

            ListSignalLine = CalculateEMA(ListMACD, 9);

            ListMACDHistogram = new List<double>();
            for (int i = 0; i < ClosePrice.Count; i++)
            {
                ListMACDHistogram.Add(ListMACD[i] - ListSignalLine[i]);
            }
        }

        public void Decide()
        {
            CalculateMACD();

            ListBullMAC = new List<int>();//Tín hiệu Bullish Moving Average Crossover
            ListBearMAC = new List<int>();//Tín hiệu Bearish Moving Average Crossover
            for (int i = NumDaysPredicted; i < ListMACDHistogram.Count - 26; i++)
            {
                if (ListMACDHistogram[i] >= 0 && ListMACDHistogram[i + 1] < 0)
                {
                    ListBullMAC.Add(i); //Khi có tín hiệu này thì giá chuẩn bị tăng
                }
                if (ListMACDHistogram[i] <= 0 && ListMACDHistogram[i + 1] > 0)
                {
                    ListBearMAC.Add(i); //Khi có tín hiệu này thì giá chuẩn bị giảm
                }
            }

            ListBullCC = new List<int>();//Tín hiệu Bullish Centerline Crossover
            ListBearCC = new List<int>();//Tín hiệu Bearish Centerline Crossover
            for (int i = NumDaysPredicted; i < ListMACD.Count - 26; i++)
            {
                if (ListMACD[i] >= 0 && ListMACD[i + 1] < 0)
                {
                    ListBullCC.Add(i); //Khi có tín hiệu này thì giá chuẩn bị tăng
                }
                if (ListMACD[i] <= 0 && ListMACD[i + 1] > 0)
                {
                    ListBearCC.Add(i); //Khi có tín hiệu này thì giá chuẩn bị giảm
                }
            }
        }

        private bool IsSell(int iIndex)
        {
            return ClosePrice[iIndex] > ClosePrice[iIndex - NumDaysPredicted];
            //return ListEMADaysPredicted[iIndex] > ListEMADaysPredicted[iIndex - NumDaysPredicted];
        }

        private bool IsBuy(int iIndex)
        {
            return ClosePrice[iIndex] < ClosePrice[iIndex - NumDaysPredicted];
            //return ListEMADaysPredicted[iIndex] < ListEMADaysPredicted[iIndex - NumDaysPredicted];
        }

        public void CalculateEMADaysPredicted()
        {
            ListEMADaysPredicted = CalculateSMA(ClosePrice, NumDaysPredicted);
        }

        public void CheckPrecisionMAC()
        {
            NumBoughts = 0;
            NumSales = 0;
            NumTotalBuy = ListBullMAC.Count;
            NumTotalSell = ListBearMAC.Count;

            //Kiểm tra với thực tế khi các tín hiệu xuất hiện
            for (int i = 0; i < ListBullMAC.Count; i++ )
            {
                if (IsBuy(ListBullMAC[i]))
                {
                    NumBoughts++;
                }
            }

            for (int i = 0; i < ListBearMAC.Count; i++)
            {
                if (IsSell(ListBearMAC[i]))
                {
                    NumSales++;
                }
            }

            NumTotals = NumTotalBuy + NumTotalSell;
            Precision = (double)(NumBoughts + NumSales) / (double)NumTotals;
        }

        public void CheckPrecisionCC()
        {
            NumBoughts = 0;
            NumSales = 0;
            NumTotalBuy = ListBullCC.Count;
            NumTotalSell = ListBearCC.Count;

            //Kiểm tra với thực tế khi các tín hiệu xuất hiện
            for (int i = 0; i < ListBullCC.Count; i++)
            {
                if (IsBuy(ListBullCC[i]))
                {
                    NumBoughts++;
                }
            }

            for (int i = 0; i < ListBearCC.Count; i++)
            {
                if (IsSell(ListBearCC[i]))
                {
                    NumSales++;
                }
            }

            NumTotals = NumTotalBuy + NumTotalSell;
            Precision = (double)(NumBoughts + NumSales) / (double)NumTotals;
        }

        public void CheckPrecisionMACCC(int nDays2Signal)
        {
            NumBoughts = 0;
            NumSales = 0;
            NumTotals = 0;
            NumTotalBuy = 0;
            NumTotalSell = 0;

            //Kiểm tra với thực tế khi các tín hiệu xuất hiện
            for (int i = 0; i < ListBullMAC.Count; i++)
            {
                for(int j = 0; j < ListBullCC.Count; j++)
                {
                    if (ListBullMAC[i] < ListBullCC[j] && ListBullCC[j] < ListBullMAC[i] + nDays2Signal)
                    {
                        NumTotalBuy++;
                        if (IsBuy(ListBullCC[j]))
                        {
                            NumBoughts++;
                        }
                        break;
                    }
                }
            }

            for (int i = 0; i < ListBearMAC.Count; i++)
            {
                for (int j = 0; j < ListBearCC.Count; j++)
                {
                    if (ListBearMAC[i] < ListBearCC[j] && ListBearCC[j] < ListBearMAC[i] + nDays2Signal)
                    {
                        NumTotalSell++;
                        if (IsSell(ListBearCC[j]))
                        {
                            NumSales++;
                        }
                        break;
                    }
                }
            }

            NumTotals = NumTotalBuy + NumTotalSell;
            Precision = (double)(NumBoughts + NumSales) / (double)NumTotals;
        }
        #endregion
    }
}
