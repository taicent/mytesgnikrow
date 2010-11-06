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
            MomentumOscillatorBUS moBUS = new MomentumOscillatorBUS();
            if (_stockRecordDTO != null)
            {
                try
                {
                    moBUS.Test(_stockRecordDTO.Entries, (int)nmNumDaysPeriod.Value, (int)nmNumPeriods.Value, (int)nmNumDaysPredicted.Value);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Lỗi");
                }
            }
            string strResult = "Tổng số lần dự đoán: " + moBUS.NumTotals.ToString() + "\r\n"
                          + "Số lần dự đoán bán: " + moBUS.NumSales.ToString() + "\r\n"
                          + "Số lần dự đoán mua: " + moBUS.NumBoughts.ToString() + "\r\n"
                          + "Độ chính xác: " + moBUS.Precision.ToString() + "\r\n";

            tbxResult.Text = strResult;
        }
    }
}
