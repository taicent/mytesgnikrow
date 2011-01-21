using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DTO;
using System.IO;
using System.Collections;
using System.Text.RegularExpressions;

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
                strTemp = stream.ReadToEnd();
                stream.Close();

                string[] strLines = Regex.Split(strTemp, "\n");

                for (int i = 0; i < strLines.Length; i++)
                {
                    if (strLines[i] == "")
                    {
                        continue;
                    }

                    strTemp = strLines[i];
                    entryDTO = new EntryDTO();
                    entryDTO.GetData(strTemp);
                    stockRecordDTO.Entries.Add(entryDTO);
                }

                //Sort lại theo ngày tăng dần
                for (int i = 0; i < stockRecordDTO.Entries.Count; i++ )
                {
                    for (int j = i + 1; j < stockRecordDTO.Entries.Count; j++)
                    {
                        EntryDTO entryI = (EntryDTO)stockRecordDTO.Entries[i];
                        EntryDTO entryJ = (EntryDTO)stockRecordDTO.Entries[j];
                        if(entryI.Date.Subtract(entryJ.Date).Days > 0)
                        {
                            EntryDTO temp = entryI;
                            stockRecordDTO.Entries[i] = stockRecordDTO.Entries[j];
                            stockRecordDTO.Entries[j] = temp;
                        }
                    }
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
