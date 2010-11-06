using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DTO;
using DAO;
using System.Collections;

namespace BUS
{
    public class MomentumOscillatorBUS
    {
        #region Attributes
        private int _numDaysPredicted;  // số ngày sau đó cần được dự đoán
        private int _numDaysPeriod;     // số ngày trong 1 period
        private double[] _periodsMomen; // mảng độ dao động qua từng period 

        private int _numBoughts;
        private int _numSales;
        private int _numTotals;
        private double _precision;

        #endregion

        #region Properties

        public int NumDaysPredicted
        {
            get { return _numDaysPredicted; }
            set { _numDaysPredicted = value; }
        }

        public int NumDaysPeriod
        {
            get { return _numDaysPeriod; }
            set { _numDaysPeriod = value; }
        }

        public double[] PeriodsMomen
        {
            get { return _periodsMomen; }
            set { _periodsMomen = value; }
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
        #endregion

        #region Method
        public MomentumOscillatorBUS()
        {
            
        }

        public int Decide(ArrayList entries, int currentIndex)
        {
            if (currentIndex + NumDaysPeriod * PeriodsMomen.Length < entries.Count)
            {
                int iTemp = currentIndex;
                for (int i = 0; i < PeriodsMomen.Length; i++)
                {
                    EntryDTO entry1 = (EntryDTO)entries[iTemp];
                    EntryDTO entry2 = (EntryDTO)entries[iTemp + NumDaysPeriod];
                    PeriodsMomen[i] = entry1.ClosePrice - entry2.ClosePrice;
                    iTemp += NumDaysPeriod;
                }
                double dblMaxMomen = 0;
                for (int i = 0; i < PeriodsMomen.Length; i++)
                {
                    if (Math.Abs(PeriodsMomen[i]) > dblMaxMomen)
                    {
                        dblMaxMomen = Math.Abs(PeriodsMomen[i]);
                    }
                }

                for (int i = 0; i < PeriodsMomen.Length; i++)
                {
                    PeriodsMomen[i] = PeriodsMomen[i] / dblMaxMomen;
                }

                if (PeriodsMomen[0] > 0.8)
                {
                    _numSales++;
                    return 1;   // bán
                }
                else if (PeriodsMomen[0] < -0.8)
                {
                    _numBoughts++;
                    return -1;  // mua
                }
                else
                {
                    return 0;   // giữ hoặc ko mua
                }
            }
            return -2;
        }

        public int CheckPrecision(ArrayList entries, int currentIndex)
        {
            int result = Decide(entries, currentIndex);
            if (result == 1)
            {
                EntryDTO curEntry = (EntryDTO)entries[currentIndex];
                EntryDTO predictEntry = (EntryDTO)entries[currentIndex - NumDaysPredicted];
                if (curEntry.ClosePrice > predictEntry.ClosePrice)
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
            else if (result == -1)
            {
                EntryDTO curEntry = (EntryDTO)entries[currentIndex];
                EntryDTO predictEntry = (EntryDTO)entries[currentIndex - NumDaysPredicted];
                if (curEntry.ClosePrice < predictEntry.ClosePrice)
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                return result;
            }

        }

        public void Test(ArrayList entries, int nDaysPeriod, int nPeriods, int nDaysPredicted)
        {
            NumDaysPeriod = nDaysPeriod;
            NumDaysPredicted = nDaysPredicted;     // mặc định số ngày dự đoán bằng số ngày trong 1 period
            PeriodsMomen = new double[nPeriods];

            _numTotals = 0;
            _numSales = 0;
            _numBoughts = 0;

            int iCorrectCase = 0;
            int iCurIndex = NumDaysPredicted;
            int iResult = CheckPrecision(entries, iCurIndex);
            while (iResult != -2)
            {
                if (iResult == 1)
                {
                    iCorrectCase++;
                }
                _numTotals++;
                iCurIndex += NumDaysPredicted;
                iResult = CheckPrecision(entries, iCurIndex);
            }
            _precision = (double)iCorrectCase / (double)_numTotals;
        }
        #endregion
    }
}
