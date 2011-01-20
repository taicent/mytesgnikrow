﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace DTO
{
    public class StockRecordDTO
    {
        #region Attributes

        private string _iD;
        private ArrayList _entries;

        #endregion

        #region Properties

        public string ID
        {
            get { return _iD; }
            set { _iD = value; }
        }
        public ArrayList Entries
        {
            get { return _entries; }
            set { _entries = value; }
        }

        #endregion

        #region Method

        public StockRecordDTO()
        {
            Entries = new ArrayList();
            ID = null;
        }

        #endregion
    }
}