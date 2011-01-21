using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DTO
{
    public class EntryDTO
    {
        #region Attributes

        private DateTime _date;
        private double _openPrice;
        private double _highPrice;
        private double _lowPrice;
        private double _closePrice;
        //private long _volume;
        //private double _change;

        #endregion

        #region Properties

        public DateTime Date
        {
            get { return _date; }
            set { _date = value; }
        }
        public double OpenPrice
        {
            get { return _openPrice; }
            set { _openPrice = value; }
        }
        public double HighPrice
        {
            get { return _highPrice; }
            set { _highPrice = value; }
        }
        public double LowPrice
        {
            get { return _lowPrice; }
            set { _lowPrice = value; }
        }
        public double ClosePrice
        {
            get { return _closePrice; }
            set { _closePrice = value; }
        }
        //public long Volume
        //{
        //    get { return _volume; }
        //    set { _volume = value; }
        //}

        //public double Change
        //{
        //    get { return _change; }
        //    set { _change = value; }
        //}

        #endregion

        #region Method

        public void GetData(string raw)
        {
            string[] strItems = new string[15];
            strItems = raw.Split(',');
            Date = Convert.ToDateTime(strItems[1]);
            OpenPrice = Convert.ToDouble(strItems[2]);
            HighPrice = Convert.ToDouble(strItems[3]);
            LowPrice = Convert.ToDouble(strItems[4]);
            ClosePrice = Convert.ToDouble(strItems[5]);
            //Volume = Convert.ToInt64(strItems[6]);
            //Change = Convert.ToDouble(strItems[10]);
        }
       
        #endregion
    }
}
