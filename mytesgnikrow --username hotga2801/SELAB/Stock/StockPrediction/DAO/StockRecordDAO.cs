using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DTO;
using System.IO;
using System.Collections;

namespace DAO
{
    public class StockRecordDAO
    {
        public StockRecordDTO LoadData(string fileName)
        {
            EntryDTO entryDTO = null;
            StockRecordDTO stockRecordDTO = new StockRecordDTO();
            string strTemp;
            StreamReader stream = null;

            stockRecordDTO.ID = fileName.Substring(fileName.LastIndexOf('\\') + 1);
            stockRecordDTO.ID = stockRecordDTO.ID.Remove(stockRecordDTO.ID.IndexOf('.'));
            try
            {
                stream = new StreamReader(fileName);
                stream.ReadLine();  // bỏ dòng title ở đầu

                while (true)
                {
                    strTemp = stream.ReadLine();
                    if (stream.EndOfStream)
                    {
                        break;
                    }
                    entryDTO = new EntryDTO();
                    entryDTO.GetData(strTemp);
                    stockRecordDTO.Entries.Add(entryDTO);
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }

            return stockRecordDTO;
        }
    }
}
