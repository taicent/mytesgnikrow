using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DTO;
using DAO;

namespace BUS
{
    public class StockRecordBUS
    {
        public StockRecordDTO LoadData(string fileName)
        {
            StockRecordDAO stockRecordDAO = new StockRecordDAO();
            if (fileName == "")
            {
                throw new Exception("Invalid file name!");
            }
            return stockRecordDAO.LoadData(fileName);
        }
    }
}
