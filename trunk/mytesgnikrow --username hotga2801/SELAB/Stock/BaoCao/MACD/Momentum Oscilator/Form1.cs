using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using DTO;
using BUS;

namespace Momentum_Oscilator
{
    public partial class Form1 : Form
    {
        private string _path;
        private StockRecordDTO _stockRecordDTO;
        private StockRecordBUS _stockRecordBUS;

        public Form1()
        {
            _stockRecordBUS = new StockRecordBUS();
            _stockRecordDTO = null;
            InitializeComponent();
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            _path = null;
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                _path = dlg.SelectedPath;
                string[] fileNames = Directory.GetFiles(_path);
                cmbStockID.Items.Clear();
                for (int i = 0; i < fileNames.Length; i++)
                {
                    string strTemp = fileNames[i].Substring(fileNames[i].LastIndexOf('\\') + 1);
                    strTemp = strTemp.Remove(strTemp.IndexOf('.')).ToUpper();

                    cmbStockID.Items.Add(strTemp);
                }

                if (cmbStockID.Items.Count > 0)
                {
                    cmbStockID.SelectedIndex = 0;
                }
            }

            
        }

        private void cmbStockID_SelectedIndexChanged(object sender, EventArgs e)
        { 
            if (cmbStockID.SelectedItem != null)
            {
                _stockRecordDTO = _stockRecordBUS.LoadData(_path + "\\" + cmbStockID.SelectedItem.ToString() + ".csv");
            }
        }

        private void btnRun_Click(object sender, EventArgs e)
        {
            //MomentumOscillatorBUS moBUS = new MomentumOscillatorBUS();
            //if (_stockRecordDTO != null)
            //{
            //    try
            //    {
            //        moBUS.Test(_stockRecordDTO.Entries, (int)nmNumDaysPeriod.Value, (int)nmNumPeriods.Value, (int)nmNumDaysPredicted.Value);
            //    }
            //    catch (Exception ex)
            //    {
            //        MessageBox.Show(ex.Message, "Lỗi");
            //    }
            //}
            //string strResult = "Tổng số lần dự đoán: " + moBUS.NumTotals.ToString() + "\r\n"
            //              + "Số lần dự đoán bán: " + moBUS.NumSales.ToString() + "\r\n"
            //              + "Số lần dự đoán mua: " + moBUS.NumBoughts.ToString() + "\r\n"
            //              + "Độ chính xác: " + moBUS.Precision.ToString() + "\r\n";

            //tbxResult.Text = strResult;
            MACDBUS macdBUS = new MACDBUS();

            List<double> ClosePrice = new List<double>();
            for (int i = 0; i < _stockRecordDTO.Entries.Count; i++)
            {
                EntryDTO curEntry = (EntryDTO)_stockRecordDTO.Entries[i];
                ClosePrice.Add(curEntry.ClosePrice);
            }
            macdBUS.ClosePrice = ClosePrice;
            macdBUS.NumDaysPredicted = (int)nmNumDaysPredicted.Value;
            macdBUS.Decide();
            macdBUS.CalculateEMADaysPredicted();

            string strResult = "";
            if (_stockRecordDTO != null)
            {
                try
                {
                    macdBUS.CheckPrecisionMAC();
                    strResult += "Tổng số lần dự đoán theo MAC: " + macdBUS.NumTotals.ToString()
                          + " (" + Math.Round(macdBUS.Precision * 100, 2).ToString() + "%)\r\n"
                          + "Số lần dự đoán bán: " + macdBUS.NumTotalSell.ToString()
                          + " (" + Math.Round(macdBUS.NumSales * 100.0d / macdBUS.NumTotalSell, 2).ToString() + "%)\r\n"
                          + "Số lần dự đoán mua: " + macdBUS.NumTotalBuy.ToString()
                          + " (" + Math.Round(macdBUS.NumBoughts * 100.0d / macdBUS.NumTotalBuy, 2).ToString() + "%)\r\n\r\n";

                    macdBUS.CheckPrecisionCC();
                    strResult += "Tổng số lần dự đoán theo CC: " + macdBUS.NumTotals.ToString()
                          + " (" + Math.Round(macdBUS.Precision * 100, 2).ToString() + "%)\r\n"
                          + "Số lần dự đoán bán: " + macdBUS.NumTotalSell.ToString()
                          + " (" + Math.Round(macdBUS.NumSales * 100.0d / macdBUS.NumTotalSell, 2).ToString() + "%)\r\n"
                          + "Số lần dự đoán mua: " + macdBUS.NumTotalBuy.ToString()
                          + " (" + Math.Round(macdBUS.NumBoughts * 100.0d / macdBUS.NumTotalBuy, 2).ToString() + "%)\r\n\r\n";

                    macdBUS.CheckPrecisionMACCC((int)nmNumDays2Signal.Value);
                    strResult += "Tổng số lần dự đoán theo MAC + CC: " + macdBUS.NumTotals.ToString()
                          + " (" + Math.Round(macdBUS.Precision * 100, 2).ToString() + "%)\r\n"
                          + "Số lần dự đoán bán: " + macdBUS.NumTotalSell.ToString()
                          + " (" + Math.Round(macdBUS.NumSales * 100.0d / macdBUS.NumTotalSell, 2).ToString() + "%)\r\n"
                          + "Số lần dự đoán mua: " + macdBUS.NumTotalBuy.ToString()
                          + " (" + Math.Round(macdBUS.NumBoughts * 100.0d / macdBUS.NumTotalBuy, 2).ToString() + "%)\r\n\r\n";
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Lỗi");
                }
                tbxResult.Text = strResult;

                StreamWriter streamWriter = new StreamWriter("Result.txt");
                strResult = "";
                for (int i = 0; i < cmbStockID.Items.Count; i ++)
                {
                    StockRecordDTO stockRecordDTO = _stockRecordBUS.LoadData(_path + "\\" + cmbStockID.Items[i].ToString() + ".csv");

                    ClosePrice.Clear();
                    for (int j = 0; j < stockRecordDTO.Entries.Count; j++)
                    {
                        EntryDTO curEntry = (EntryDTO)stockRecordDTO.Entries[j];
                        ClosePrice.Add(curEntry.ClosePrice);
                    }
                    macdBUS.ClosePrice = ClosePrice;

                    macdBUS.NumDaysPredicted = (int)nmNumDaysPredicted.Value;
                    macdBUS.Decide();

                    macdBUS.CalculateEMADaysPredicted();

                    strResult = cmbStockID.Items[i].ToString() + "\t";
                    macdBUS.CheckPrecisionMAC();
                    strResult += Math.Round(macdBUS.NumBoughts * 100.0d / macdBUS.NumTotalBuy, 2).ToString() + "\t"
                                + Math.Round(macdBUS.NumSales * 100.0d / macdBUS.NumTotalSell, 2).ToString() + "\t"
                                + Math.Round(macdBUS.Precision * 100, 2).ToString() + "\t";

                    macdBUS.CheckPrecisionCC();
                    strResult += Math.Round(macdBUS.NumBoughts * 100.0d / macdBUS.NumTotalBuy, 2).ToString() + "\t"
                                + Math.Round(macdBUS.NumSales * 100.0d / macdBUS.NumTotalSell, 2).ToString() + "\t"
                                + Math.Round(macdBUS.Precision * 100, 2).ToString() + "\t";

                    macdBUS.CheckPrecisionMACCC((int)nmNumDays2Signal.Value);
                    strResult += Math.Round(macdBUS.NumBoughts * 100.0d / macdBUS.NumTotalBuy, 2).ToString() + "\t"
                                + Math.Round(macdBUS.NumSales * 100.0d / macdBUS.NumTotalSell, 2).ToString() + "\t"
                                + Math.Round(macdBUS.Precision * 100, 2).ToString() + "\t";

                    streamWriter.WriteLine(strResult);
                }
                streamWriter.Close();
            }
        }
    }
}
