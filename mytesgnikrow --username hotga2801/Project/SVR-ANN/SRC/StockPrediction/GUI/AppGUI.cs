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

namespace GUI
{
    public partial class AppGUI : Form
    {
        private StockRecordDTO _stockRecordDTO;
        private StockRecordBUS _stockRecordBUS;
        private string strPath;

        public AppGUI()
        {
            _stockRecordBUS = new StockRecordBUS();
            _stockRecordDTO = null;
            strPath = "";
            InitializeComponent();
        }

        private void btnPredict_Click(object sender, EventArgs e)
        {
            //Dùng tạm số node input = 10, sau này bổ sung hàm đọc file số node input
            int inputNode = 10;

            //Tìm ngày cần dự đoán
            DateTime PredictDay = Convert.ToDateTime(dtpPredictDay.Text);
            int i;
            for (i = _stockRecordDTO.Entries.Count - 1; i >= inputNode; i--)
            {
                EntryDTO curEntry = (EntryDTO)_stockRecordDTO.Entries[i - inputNode];
                if (PredictDay.CompareTo(curEntry.Date) == 0)
                {
                    break;
                }
            }
            
            //Ghi bộ input đầu vào cho ngày dự đoán
            double[] dblSource = new double[inputNode + 1];
            if (i >= inputNode)
            {
                //1. Đọc dữ liệu vào mảng
                for (int j = 0; j < inputNode; j++)
                { 
                    EntryDTO curEntry = (EntryDTO)_stockRecordDTO.Entries[i - j];
                    dblSource[j] = curEntry.ClosePrice;
                }

                PreprocessBUS preprocessBUS = new PreprocessBUS();
                preprocessBUS.FindMinMax(dblSource);
                dblSource = preprocessBUS.PreprocessByMinMax(dblSource);

                //2. Chuyển sang định dạng của LibSVM (dựa vào số node đầu vào)
                SVMConverterBUS converter = new SVMConverterBUS();
                int iPos = strPath.LastIndexOf('\\');
                string strFolderPath = strPath.Remove(iPos + 1);
                string strTotalFile = strFolderPath + _stockRecordDTO.ID + ".txt";
                int iNumInputNode = inputNode;
                int numDaysPredicted = 1;
                int iNumLine = 0;
                converter.Convert(iNumInputNode, dblSource.Length, dblSource, strTotalFile, out iNumLine);

                //3. Từ file chứa toàn bộ dữ liệu ta phân phối vào 2 file train và test (dựa vào tỉ lệ bộ train)
                string strTestFile = strFolderPath + _stockRecordDTO.ID + "_" + numDaysPredicted + "_test.txt";
                StreamReader reader = new StreamReader(strTotalFile);
                StreamWriter testWriter = new StreamWriter(strTestFile);

                //Ghi phương thức xử lý vào dòng đầu tiên của file test
                //Mục đích là để ta có thể chuyển về dữ liệu nguyên thủy
                testWriter.WriteLine("ScaleByMinMax" + " " + preprocessBUS.Min.ToString() + " " + preprocessBUS.Max.ToString());
              
                for (i = 0; i < iNumLine; i++)
                {
                    string strLine = reader.ReadLine();
                    testWriter.WriteLine(strLine);
                }

                testWriter.Close();
                reader.Close();
            }
        }

        private void AppGUI_Load(object sender, EventArgs e)
        {
            //Load tất cả mã chứng khoán lên combobox
            string[] fileNames = Directory.GetFiles("Excel");
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

        private void cmbStockID_SelectedIndexChanged(object sender, EventArgs e)
        {
            CreateGraph(zg1);
            SetSize();

            if (cmbStockID.SelectedItem != null)
            {
                strPath = "Excel" + "\\" + cmbStockID.SelectedItem.ToString() + ".csv";
                _stockRecordDTO = _stockRecordBUS.LoadData(strPath);
            }
        }
    }
}
