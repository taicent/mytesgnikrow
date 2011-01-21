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
using ZedGraph;

namespace GUI
{
    public partial class MainGUI : Form
    {
        #region Attributes
        public const int MSE = 1;
        public const int NMSE = 2;
        public const int RMSE = 3;
        public const int SIGN = 4;

        private StockRecordDTO _stockRecordDTO;
        private StockRecordBUS _stockRecordBUS;
        private string strStockPath;
        private double pricePredict = 0;
        private double trendPredict = 0;
        #endregion

        public MainGUI()
        {
            InitializeComponent();
        }

        private void HandleMeasure(string fileName, double []actual, double []forecast)
        {
            StreamWriter writer = new StreamWriter(fileName);
            MeasureBUS measureBUS = new MeasureBUS();
            CheckedListBox.CheckedItemCollection checkedItems = clbMeasures.CheckedItems;
            for (int i = 0; i < checkedItems.Count; i++)
            {
                switch (checkedItems[i].ToString())
                {
                    case "MSE":
                        double dblMSE = measureBUS.MSE(actual, forecast);
                        writer.WriteLine("MSE " + dblMSE.ToString());
                        break;
                    case "NMSE":
                        double dblNMSE = measureBUS.NMSE(actual, forecast);
                        writer.WriteLine("NMSE " + dblNMSE.ToString());
                        break;
                    case "RMSE":
                        double dblRMSE = measureBUS.RMSE(actual, forecast);
                        writer.WriteLine("RMSE " + dblRMSE.ToString());
                        break;
                    case "APE":
                        double dblAPE = measureBUS.APE(actual, forecast);
                        writer.WriteLine("APE " + dblAPE.ToString());
                        break;
                    case "MAPE":
                        double dblMAPE = measureBUS.MAPE(actual, forecast);
                        writer.WriteLine("MAPE " + dblMAPE.ToString());
                        break;
                    case "DS":
                        double dblMeasure = measureBUS.DS(actual, forecast);
                        writer.WriteLine("DS " + dblMeasure.ToString());
                        break;
                    case "WDS":
                        double dblWDS = measureBUS.WDS(actual, forecast);
                        writer.WriteLine("WDS " + dblWDS.ToString());
                        break;
                    case "Sign":
                        double dblSign = measureBUS.Sign(actual, forecast);
                        writer.WriteLine("Sign " + dblSign.ToString());
                        break;
                    case "DM4Price":
                        double[] dblResult = measureBUS.DirectionMeasure4Price(actual, forecast);
                        writer.WriteLine("Direction measure for price:");
                        writer.WriteLine("\t+ Right: "+dblResult[0]+"%");
                        writer.WriteLine("\t+ Wrong: " + dblResult[1] + "%");
                        writer.WriteLine("\t+ Cannot measure: " + dblResult[2] + "%");
                        break;
                }
            }

            writer.Close();
        }

        private void btnTrain_Click(object sender, EventArgs e)
        {
            if (tbxTrainFilePath.Text == "")
            {
                MessageBox.Show("Error: You must fill all required inputs!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            int iMeasureType = 0;
            switch (cmbTrainingMeasure.SelectedItem.ToString())
            {
                case "MSE":
                    iMeasureType = MSE;
                    break;
                case "NMSE":
                    iMeasureType = NMSE;
                    break;
                case "RMSE":
                    iMeasureType = RMSE;
                    break;
                case "Sign":
                    iMeasureType = SIGN;
                    break;
            }
            int iCrossValidationType;
            if(rdShuffle.Checked)
            {
                iCrossValidationType = 0;
            }
            else if(rdNotShuffle.Checked)
            {
                iCrossValidationType = 1;
            }
            else
            {
                iCrossValidationType = 2;
            }

            #region Dự đoán giá
            if (rdPricePrediction.Checked)//Dự đoán giá
            {
                int iPos = tbxTrainFilePath.Text.LastIndexOf('_');
                string strMutualPath = tbxTrainFilePath.Text.Remove(iPos + 1);
                string strModelFile = strMutualPath + "model.txt";
                #region SVR
                if (rdSVR.Checked)//Mô hình SVR
                {
                    string[] strArgs = new string[8];
                    strArgs[0] = "-g";  //gama. Trong hàm Gauss kernel: exp(-gamma*|u-v|^2)
                    strArgs[2] = "-c";
                    strArgs[4] = "-p";  //epsilon
                    strArgs[6] = tbxTrainFilePath.Text;
                    strArgs[7] = strModelFile;

                    if (cmbModelSelection.SelectedItem.ToString() == "Use default values")
                    {
                        strArgs[1] = "0";   //gama
                        strArgs[3] = "1";   //c
                        strArgs[5] = "0.1";   //epsilon
                        //strArgs[1] = "0.686";   //gama
                        //strArgs[3] = "68.4717";   //c
                        //strArgs[5] = "0.00012124";   //epsilon
                        //strArgs[1] = "2.5";   //gama
                        //strArgs[3] = "0.01";   //c
                        //strArgs[5] = "0.3";   //epsilon
                        svm_train.Main(strArgs);
                    }
                    else if (cmbModelSelection.SelectedItem.ToString() == "Pattern search")
                    {
                        string[] strArgsModel = new string[4];
                        strArgsModel[0] = "-v";
                        strArgsModel[1] = tbxNumFold.Text;
                        strArgsModel[2] = tbxTrainFilePath.Text;
                        strArgsModel[3] = strModelFile;

                        int iNumModel = 10;
                        StreamWriter writer = new StreamWriter(strModelFile);
                        writer.WriteLine("Multiple");
                        writer.WriteLine(iNumModel.ToString());
                        StreamWriter patternTime = new StreamWriter(strMutualPath + "PatternTime.txt");
                        double dblTotalTime = 0;
                        for (int i = 0; i < iNumModel; i++)
                        {
                            svm_modelSelection modelSelection = new svm_modelSelection();
                            DateTime start = DateTime.Now;
                            modelSelection.Run(strArgsModel, strMutualPath + "PatternSearchLog" + (i + 1).ToString() + ".txt", false, iMeasureType, iCrossValidationType);
                            DateTime end = DateTime.Now;
                            TimeSpan elapsedTime = end - start;
                            dblTotalTime += elapsedTime.TotalMinutes;
                            patternTime.WriteLine((i + 1).ToString() + ": " + elapsedTime.TotalMinutes.ToString() + " minutes");

                            strArgs[1] = (Math.Pow(2, modelSelection.Y)).ToString();   //gama
                            strArgs[3] = (Math.Pow(2, modelSelection.X)).ToString();   //c
                            strArgs[5] = (Math.Pow(2, modelSelection.Z)).ToString();   //epsilon
                            //strArgs[3] = (Math.Pow(2, 6.09743805699862)).ToString();
                            //strArgs[1] = (Math.Pow(2, -3.86541887296616)).ToString();
                            //strArgs[5] = (Math.Pow(2, -13.0097538228192)).ToString(); 
                            strArgs[7] = strMutualPath + "model" + (i + 1).ToString() + ".txt";
                            writer.WriteLine(strArgs[7]);
                            svm_train.Main(strArgs);
                        }
                        writer.Close();
                        patternTime.WriteLine("Total: " + dblTotalTime.ToString() + " minutes");
                        patternTime.Close();
                    }
                    else if (cmbModelSelection.SelectedItem.ToString() == "Grid search")
                    {
                        svm_modelSelection modelSelection = new svm_modelSelection();
                        string[] strArgsModel = new string[4];
                        strArgsModel[0] = "-v";
                        strArgsModel[1] = tbxNumFold.Text;
                        strArgsModel[2] = tbxTrainFilePath.Text;
                        strArgsModel[3] = strModelFile;
                        DateTime start = DateTime.Now;
                        modelSelection.Run(strArgsModel, strMutualPath + "GridSearchLog.txt", true, iMeasureType, iCrossValidationType);
                        DateTime end = DateTime.Now;
                        TimeSpan elapsedTime = end - start;
                        StreamWriter gridTime = new StreamWriter(strMutualPath + "GridTime.txt");
                        gridTime.WriteLine(elapsedTime.TotalMinutes.ToString() + " minutes");
                        gridTime.Close();

                        strArgs[1] = modelSelection.Y.ToString();   //gama
                        strArgs[3] = modelSelection.X.ToString();   //c
                        strArgs[5] = modelSelection.Z.ToString();   //epsilon
                        svm_train.Main(strArgs);
                    }
                    MessageBox.Show("Finish!");
                }
                #endregion
                #region ANN
                else//Mô hình ANN
                {
                    ANNParameterBUS.InputNode = int.Parse(tbxNumInputNode.Text);
                    ANNParameterBUS.HiddenNode = int.Parse(tbxANNHiddenNode.Text);
                    ANNParameterBUS.OutputNode = 1;
                    ANNParameterBUS.MaxEpoch = int.Parse(tbxMaxLoops.Text);
                    ANNParameterBUS.LearningRate = double.Parse(tbxLearningRate.Text);
                    ANNParameterBUS.Momentum = double.Parse(tbxMomentum.Text);
                    ANNParameterBUS.Bias = double.Parse(tbxBias.Text);

                    ANNModelBUS.AnnModelFile = strModelFile;
                    ANNTrainBUS annTrain = new ANNTrainBUS();
                    annTrain.LoadDataSet(tbxTrainFilePath.Text);
                    annTrain.Main();
                    MessageBox.Show("Finish!");
                }
                #endregion
            }
            #endregion
            #region Dự đoán xu hướng
            else//Dự đoán xu hướng
            {
                if (rdSVR.Checked)//Mô hình SVR
                {
                    int iPos = tbxTrainFilePath.Text.LastIndexOf('_');
                    string strMutualPath = tbxTrainFilePath.Text.Remove(iPos + 1);
                    string strModelFile = strMutualPath + "model.txt";
                    string[] strArgs = new string[8];
                    strArgs[0] = "-g";  //gama. Trong hàm Gauss kernel: exp(-gamma*|u-v|^2)
                    strArgs[2] = "-c";
                    strArgs[4] = "-p";  //epsilon
                    strArgs[6] = tbxTrainFilePath.Text;
                    strArgs[7] = strModelFile;

                    if (cmbModelSelection.SelectedItem.ToString() == "Use default values")
                    {
                        //strArgs[1] = "0";   //gama
                        //strArgs[3] = "1";   //c
                        //strArgs[5] = "0.1";   //epsilon
                        strArgs[1] = "0.0110485434560398";   //gama
                        strArgs[3] = "304.437021440697";   //c
                        strArgs[5] = "0.000290333768311211";   //epsilon
                        //strArgs[1] = "2.5";   //gama
                        //strArgs[3] = "0.01";   //c
                        //strArgs[5] = "0.3";   //epsilon
                        svm_train.Main(strArgs);
                    }
                    else if (cmbModelSelection.SelectedItem.ToString() == "Pattern search")
                    {
                        string[] strArgsModel = new string[4];
                        strArgsModel[0] = "-v";
                        strArgsModel[1] = tbxNumFold.Text;
                        strArgsModel[2] = tbxTrainFilePath.Text;
                        strArgsModel[3] = strModelFile;

                        int iNumModel = 10;
                        StreamWriter writer = new StreamWriter(strModelFile);
                        writer.WriteLine("Multiple");
                        writer.WriteLine(iNumModel.ToString());
                        for (int i = 0; i < iNumModel; i++)
                        {
                            svm_modelSelection modelSelection = new svm_modelSelection();
                            modelSelection.Run(strArgsModel, strMutualPath + "PatternSearchLog" + (i + 1).ToString() + ".txt", false, iMeasureType, iCrossValidationType);
                            strArgs[1] = (Math.Pow(2, modelSelection.Y)).ToString();   //gama
                            strArgs[3] = (Math.Pow(2, modelSelection.X)).ToString();   //c
                            strArgs[5] = (Math.Pow(2, modelSelection.Z)).ToString();   //epsilon
                            //strArgs[3] = (Math.Pow(2, 6.09743805699862)).ToString();
                            //strArgs[1] = (Math.Pow(2, -3.86541887296616)).ToString();
                            //strArgs[5] = (Math.Pow(2, -13.0097538228192)).ToString(); 
                            strArgs[7] = strMutualPath + "model" + (i + 1).ToString() + ".txt";
                            writer.WriteLine(strArgs[7]);
                            svm_train.Main(strArgs);
                        }
                        writer.Close();
                    }
                    else if (cmbModelSelection.SelectedItem.ToString() == "Grid search")
                    {
                        svm_modelSelection modelSelection = new svm_modelSelection();
                        string[] strArgsModel = new string[4];
                        strArgsModel[0] = "-v";
                        strArgsModel[1] = tbxNumFold.Text;
                        strArgsModel[2] = tbxTrainFilePath.Text;
                        strArgsModel[3] = strModelFile;
                        modelSelection.Run(strArgsModel, strMutualPath + "GridSearchLog.txt", true, iMeasureType, iCrossValidationType);
                        strArgs[1] = modelSelection.Y.ToString();   //gama
                        strArgs[3] = modelSelection.X.ToString();   //c
                        strArgs[5] = modelSelection.Z.ToString();   //epsilon
                        svm_train.Main(strArgs);
                    }
                    MessageBox.Show("Finish!");
                }
                else//Mô hình ANN
                {
                    int iPos = tbxTrainFilePath.Text.LastIndexOf('_');
                    string strModelFile = tbxTrainFilePath.Text.Remove(iPos + 1) + "model.txt";

                    //khởi tạo các tham số cho mạng
                    ANNParameterBUS.InputNode = int.Parse(tbxNumInputNode.Text);
                    ANNParameterBUS.HiddenNode = int.Parse(tbxANNHiddenNode.Text);
                    ANNParameterBUS.OutputNode = 1;
                    ANNParameterBUS.MaxEpoch = int.Parse(tbxMaxLoops.Text);
                    ANNParameterBUS.LearningRate = double.Parse(tbxLearningRate.Text);
                    ANNParameterBUS.Momentum = double.Parse(tbxMomentum.Text);
                    ANNParameterBUS.Bias = double.Parse(tbxBias.Text);

                    ANNParameterBUS.Accuracy = double.Parse(tbxAccuracy.Text);
                    ANNParameterBUS.MeasureType = cmbTrainingMeasure.SelectedItem.ToString();

                    //Tiến hành train
                    ANNModelBUS.AnnModelFile = strModelFile;
                    ANNTrainBUS annTrain = new ANNTrainBUS();
                    annTrain.LoadDataSet(tbxTrainFilePath.Text);
                    annTrain.Main(iMeasureType);
                    MessageBox.Show("Finish!");
                }
            #endregion
            }
        }

        private void btnBrowser_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDlg = new OpenFileDialog();
            openFileDlg.Filter = "(*.csv)|*.csv";
            if (openFileDlg.ShowDialog() == DialogResult.OK)
            {
                tbxCsvFilePath.Text = openFileDlg.FileName;
                tbxTrainFilePath.Text = openFileDlg.FileName.Replace(".csv", "") + "_1_train.txt";
                tbxTestFilePath.Text = openFileDlg.FileName.Replace(".csv", "") + "_1_test.txt";
                tbxModelFilePath.Text = openFileDlg.FileName.Replace(".csv", "") + "_1_model.txt";
            }
        }

        private void btnTest_Click(object sender, EventArgs e)
        {
            if (tbxTestFilePath.Text == "" || tbxModelFilePath.Text == "")
            {
                MessageBox.Show("Error: You must fill all required inputs!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            #region Phần chung
            //Ma trận với dòng thứ 1 chứa các giá trị thực và dòng thứ 2 chứa các giá trị dự đoán.
            double[][] dblActual_Forecast = new double[2][];
            dblActual_Forecast[0] = null;
            dblActual_Forecast[1] = null;

            int iPos = tbxTestFilePath.Text.LastIndexOf('\\');
            string strFolderPath = tbxTestFilePath.Text.Remove(iPos+1);
            #endregion

            #region Phần riêng
            #region Dự đoán giá
            if (rdPricePrediction.Checked)//Dự đoán giá
            {
                iPos = tbxTestFilePath.Text.LastIndexOf('_');
                string strMutualPath = tbxTestFilePath.Text.Remove(iPos + 1);
                string strPredictedFile = strMutualPath + "predict.txt";
                #region SVR
                if (rdSVR.Checked)//Mô hình SVR
                {
                    string[] strArgs = new string[3];
                    strArgs[0] = tbxTestFilePath.Text;
                    strArgs[1] = tbxModelFilePath.Text;
                    strArgs[2] = strPredictedFile;
                    StreamReader reader = new StreamReader(tbxModelFilePath.Text);
                    string strLine = reader.ReadLine();
                    if (strLine == "Multiple")  //Bag Pattern Search
                    {
                        StreamWriter measure = new StreamWriter(strMutualPath + "SingleModelMeasure.txt");
                        strLine = reader.ReadLine();    //Số mô hình
                        int iNumModel = Convert.ToInt32(strLine);
                        double[][][] dblActual_ForecastSeries = new double[iNumModel][][];
                        for (int i = 0; i < iNumModel; i++)
                        {
                            dblActual_ForecastSeries[i] = new double[2][];
                            dblActual_ForecastSeries[i][0] = null;
                            dblActual_ForecastSeries[i][1] = null;

                            strLine = reader.ReadLine();    //Đường dẫn đến file model
                            strArgs[1] = strLine;
                            dblActual_ForecastSeries[i] = svm_predict.MainProcess(strArgs);
                            MeasureBUS measureBUS = new MeasureBUS();
                            double dblMSE = measureBUS.MSE(dblActual_ForecastSeries[i][0], dblActual_ForecastSeries[i][1]);
                            double dblNMSE = measureBUS.NMSE(dblActual_ForecastSeries[i][0], dblActual_ForecastSeries[i][1]);
                            double dblDS = measureBUS.DS(dblActual_ForecastSeries[i][0], dblActual_ForecastSeries[i][1]);
                            measure.WriteLine((i + 1).ToString() + " " + dblMSE.ToString() + " " + dblNMSE.ToString() + " " + dblDS.ToString());
                        }
                        measure.Close();

                        //Để có kết quả sau cùng, ta sẽ lấy trung bình kết quả dự đoán của tất cả các mô hình lại
                        int iCounter = dblActual_ForecastSeries[0][0].Count();
                        dblActual_Forecast[0] = new double[iCounter];
                        dblActual_Forecast[1] = new double[iCounter];
                        for (int i = 0; i < iCounter; i++)
                        {
                            //Giá thực
                            dblActual_Forecast[0][i] = dblActual_ForecastSeries[0][0][i];
                            //Giá dự đoán
                            dblActual_Forecast[1][i] = 0;
                            for (int j = 0; j < iNumModel; j++)
                            {
                                dblActual_Forecast[1][i] += dblActual_ForecastSeries[j][1][i];
                            }
                            dblActual_Forecast[1][i] /= iNumModel;
                        }

                        //Ghi kết quả cuối cùng ra file
                        StreamWriter finalResult = new StreamWriter(strPredictedFile);
                        for (int i = 0; i < iCounter; i++)
                        {
                            finalResult.WriteLine(dblActual_Forecast[0][i].ToString() + " " + dblActual_Forecast[1][i].ToString());
                        }
                        finalResult.Close();
                        reader.Close();
                    }
                    else
                    {
                        reader.Close();
                        dblActual_Forecast = svm_predict.MainProcess(strArgs);
                    }
                }
                #endregion
                #region ANN
                else//Mô hình ANN
                {
                    ANNModelBUS.AnnModelFile = tbxModelFilePath.Text;

                    ANNParameterBUS.LoadParameter();

                    ANNPredictBUS annPredict = new ANNPredictBUS();
                    annPredict.LoadDataSet(tbxTestFilePath.Text);
                    dblActual_Forecast = annPredict.MainProcess();
                    //dblActual_Forecast = annPredict.StepTrainingMethod();
                    annPredict.WritePredictPrice(strPredictedFile);
                }
                #endregion
            }
            #endregion
            #region Dự đoán xu hướng
            else//Dự đoán xu hướng
            {
                if (rdSVR.Checked)//Mô hình SVR
                {
                    iPos = tbxTestFilePath.Text.LastIndexOf('_');
                    string strMutualPath = tbxTestFilePath.Text.Remove(iPos + 1);
                    string strPredictedFile = strMutualPath + "predict.txt";
                    string[] strArgs = new string[3];
                    strArgs[0] = tbxTestFilePath.Text;
                    strArgs[1] = tbxModelFilePath.Text;
                    strArgs[2] = strPredictedFile;
                    StreamReader reader = new StreamReader(tbxModelFilePath.Text);
                    string strLine = reader.ReadLine();
                    if (strLine == "Multiple")  //Bag Pattern Search
                    {
                        StreamWriter measure = new StreamWriter(strMutualPath + "SingleModelMeasure.txt");
                        strLine = reader.ReadLine();    //Số mô hình
                        int iNumModel = Convert.ToInt32(strLine);
                        double[][][] dblActual_ForecastSeries = new double[iNumModel][][];
                        for (int i = 0; i < iNumModel; i++)
                        {
                            dblActual_ForecastSeries[i] = new double[2][];
                            dblActual_ForecastSeries[i][0] = null;
                            dblActual_ForecastSeries[i][1] = null;

                            strLine = reader.ReadLine();    //Đường dẫn đến file model
                            strArgs[1] = strLine;
                            dblActual_ForecastSeries[i] = svm_predict.MainProcess(strArgs);
                            MeasureBUS measureBUS = new MeasureBUS();
                            double dblNMSE = measureBUS.NMSE(dblActual_ForecastSeries[i][0], dblActual_ForecastSeries[i][1]);
                            measure.WriteLine((i + 1).ToString() + " " + dblNMSE.ToString());
                        }
                        measure.Close();

                        //Để có kết quả sau cùng, ta sẽ lấy trung bình kết quả dự đoán của tất cả các mô hình lại
                        int iCounter = dblActual_ForecastSeries[0][0].Count();
                        dblActual_Forecast[0] = new double[iCounter];
                        dblActual_Forecast[1] = new double[iCounter];
                        for (int i = 0; i < iCounter; i++)
                        {
                            //Giá thực
                            dblActual_Forecast[0][i] = dblActual_ForecastSeries[0][0][i];
                            //Giá dự đoán
                            dblActual_Forecast[1][i] = 0;
                            for (int j = 0; j < iNumModel; j++)
                            {
                                dblActual_Forecast[1][i] += dblActual_ForecastSeries[j][1][i];
                            }
                            dblActual_Forecast[1][i] /= iNumModel;
                        }

                        //Ghi kết quả cuối cùng ra file
                        StreamWriter finalResult = new StreamWriter(strPredictedFile);
                        for (int i = 0; i < iCounter; i++)
                        {
                            finalResult.WriteLine(dblActual_Forecast[0][i].ToString() + " " + dblActual_Forecast[1][i].ToString());
                        }
                        finalResult.Close();
                        reader.Close();
                    }
                    else
                    {
                        reader.Close();
                        dblActual_Forecast = svm_predict.MainProcess(strArgs);
                    }
                }
                else//Mô hình ANN
                {
                    iPos = tbxTestFilePath.Text.LastIndexOf('_');
                    string strPredictedFile = tbxTestFilePath.Text.Remove(iPos + 1) + "predict.txt";

                    ANNModelBUS.AnnModelFile = tbxModelFilePath.Text;
                    ANNParameterBUS.LoadParameter();

                    ANNPredictBUS annPredict = new ANNPredictBUS();
                    annPredict.LoadDataSet(tbxTestFilePath.Text);
                    dblActual_Forecast = annPredict.MainProcessTrend();
                    annPredict.WritePredictPrice(strPredictedFile);
                }
            }
            #endregion
            #endregion

            #region Phần chung
            HandleMeasure(strFolderPath+"PerformanceMeasure.txt", dblActual_Forecast[0], dblActual_Forecast[1]);
            MessageBox.Show("Finish!");
            #endregion
            
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (tbxCsvFilePath.Text == "" || cmbPreprocess.SelectedIndex < 0 || tbxNumInputNode.Text == "" || tbxTrainingRatio.Text == "")
            {
                MessageBox.Show("Error: You must fill all required inputs!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            //1. Đọc dữ liệu từ file .csv vào mảng và tiền xử lý
            StockRecordBUS stockRecordBUS = new StockRecordBUS();
            StockRecordDTO stockRecordDTO = stockRecordBUS.LoadData(tbxCsvFilePath.Text);
            double[] dblSource = new double[stockRecordDTO.Entries.Count];
            int i = 0;
            foreach (EntryDTO entryDTO in stockRecordDTO.Entries)
            {
                dblSource[i] = entryDTO.ClosePrice;
                i++;
            }
            PreprocessBUS preprocessBUS = new PreprocessBUS();
            #region Phần riêng
            if (rdPricePrediction.Checked)//Dự đoán giá
            {
                if (rdSVR.Checked)//Mô hình SVR
                {
                    if (cmbPreprocess.SelectedItem.ToString() == "ScaleByMinMax")
                    {
                        preprocessBUS.FindMinMax(dblSource);
                        dblSource = preprocessBUS.PreprocessByMinMax(dblSource);
                    }
                }
                else//Mô hình ANN
                {
                    if (cmbPreprocess.SelectedItem.ToString() == "ScaleByMinMax")
                    {
                        preprocessBUS.FindMinMax(dblSource);
                        dblSource = preprocessBUS.PreprocessByMinMax(dblSource);
                    }
                }
            }
            else//Dự đoán xu hướng
            {
                if (rdSVR.Checked)//Mô hình SVR
                {
                    if (cmbPreprocess.SelectedItem.ToString() == "Return[-1,1]")
                    {
                        dblSource = preprocessBUS.Scale_SVR_Return(stockRecordDTO.Entries.Count, dblSource, 1, 1);
                    }
                }
                else//Mô hình ANN
                {
                    if (cmbPreprocess.SelectedItem.ToString() == "Return[-1,1]")
                    {
                        dblSource = preprocessBUS.Scale_SVR_Return(stockRecordDTO.Entries.Count, dblSource, 1, 1);
                    }
                }
            }
            #endregion
            
            //2. Chuyển sang định dạng của LibSVM (dựa vào số node đầu vào)
            ConverterBUS converter = new ConverterBUS();
            int iPos = tbxCsvFilePath.Text.LastIndexOf('\\');
            string strFolderPath = tbxCsvFilePath.Text.Remove(iPos+1);
            string strTotalFile = strFolderPath + stockRecordDTO.ID + ".txt";
            int iNumInputNode = Convert.ToInt32(tbxNumInputNode.Text);
            int numDaysPredicted = 1;
            int iNumLine = 0;

            if (cmbPreprocess.SelectedItem.ToString() == "Return[-1,1]")
            {
                if (int.TryParse(tbxNumDaysPredicted.Text,out numDaysPredicted))
                {
                    converter.ConvertForTrend(int.Parse(tbxNumDaysPredicted.Text), iNumInputNode, dblSource, strTotalFile, out iNumLine, 2, false);
                }
                else
                {
                    MessageBox.Show("Please enter a number");
                    return;
                }  
            }
            else
            {
                if(ckbImproveDirection.Checked)
                {
                    double dblTrainPercent = Convert.ToDouble(tbxTrainingRatio.Text);
                    converter.ConvertWImprovedDirection(iNumInputNode, dblSource, strTotalFile, dblTrainPercent, out iNumLine);
                }
                else
                {
                    converter.Convert(iNumInputNode, dblSource, strTotalFile, out iNumLine);    
                }
            }
            //3. Từ file chứa toàn bộ dữ liệu ta phân phối vào 2 file train và test (dựa vào tỉ lệ bộ train)
            string strTrainFile = strFolderPath + stockRecordDTO.ID + "_" + numDaysPredicted + "_train.txt";
            string strTestFile = strFolderPath + stockRecordDTO.ID + "_" + numDaysPredicted + "_test.txt";
            StreamReader reader = new StreamReader(strTotalFile);
            StreamWriter trainWriter = new StreamWriter(strTrainFile);
            StreamWriter testWriter = new StreamWriter(strTestFile);

            //Ghi phương thức xử lý vào dòng đầu tiên của file test
            //Mục đích là để ta có thể chuyển về dữ liệu nguyên thủy
            testWriter.WriteLine(cmbPreprocess.SelectedItem.ToString() + " " + preprocessBUS.Min.ToString() + " " + preprocessBUS.Max.ToString());

            double dblTrainingSetRatio = Convert.ToDouble(tbxTrainingRatio.Text);
            //int iBound = numDaysPredicted > iNumInputNode ? 2 * numDaysPredicted : numDaysPredicted + iNumInputNode;
            //int iNumLine = dblSource.Length - iBound + 1;
            int iDivideLine = (int)(dblTrainingSetRatio * iNumLine/100);
            for (i = 0; i < iDivideLine; i++)
            {
                string strLine = reader.ReadLine();
                trainWriter.WriteLine(strLine);
            }
            for (; i < iNumLine; i++)
            {
                string strLine = reader.ReadLine();
                testWriter.WriteLine(strLine);
            }

            testWriter.Close();
            trainWriter.Close();
            reader.Close();

            MessageBox.Show("Finish!");
        }

        private void MainGUI_Load(object sender, EventArgs e)
        {
            //Khởi gán
            cmbPreprocess.SelectedIndex = 0;
            tbxNumInputNode.Text = "5";
            tbxTrainingRatio.Text = "80";
            cmbModelSelection.SelectedIndex = 0;
            cmbTrainingMeasure.SelectedIndex = 0;

            //Khởi gán tham số ANN
            tbxNumInputNode.Text = 5.ToString();
            tbxANNHiddenNode.Text = 4.ToString();
            tbxLearningRate.Text = 0.3.ToString();
            tbxMaxLoops.Text = 2000.ToString();
            tbxBias.Text = 0.ToString();
            tbxMomentum.Text = 0.01.ToString();

            if (rdANN.Checked == true)
            {
                gbAnnSetting.Enabled = true;
            }
            else
            {
                gbAnnSetting.Enabled = false;
            }

            _stockRecordBUS = new StockRecordBUS();
            _stockRecordDTO = null;
            strStockPath = "";
        }

        private void CreateGraph(ZedGraphControl zgc)
        {
            GraphPane myPane = zgc.GraphPane;
            myPane.CurveList.Clear();
            
            // Set the titles and axis labels
            myPane.Title.Text = "Stock Chart";
            myPane.XAxis.Title.Text = "Time";
            myPane.YAxis.Title.Text = "Price";

            // Make up some data points from data
            //1. Đọc dữ liệu từ file .csv vào mảng và tiền xử lý
            PointPairList list = new PointPairList();
            DateTime from = dtpFrom.Value;
            DateTime to = dtpTo.Value;

            foreach (EntryDTO entryDTO in _stockRecordDTO.Entries)
            {
                if (entryDTO.Date.Subtract(from).Days >= 0 && entryDTO.Date.Subtract(to).Days <= 0)
                {
                    double x = (double)new XDate(entryDTO.Date);
                    list.Add(x, entryDTO.ClosePrice);
                }
            }
            
            // Generate a blue curve with circle symbols, and "My Curve 2" in the legend
            LineItem myCurve = myPane.AddCurve("Close Price", list, Color.Blue, SymbolType.None);

            // Fill the axis background with a color gradient
            myPane.Chart.Fill = new Fill(Color.White);

            // Fill the pane background with a color gradient
            myPane.Fill = new Fill(Color.White);

            // Set the XAxis to date type
            myPane.XAxis.Type = AxisType.DateAsOrdinal;
            
            // Calculate the Axis Scale Ranges
            zgc.AxisChange();
            zgc.Invalidate();
        }

        private void btnTrainBrowser_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDlg = new OpenFileDialog();
            openFileDlg.Filter = "(*.txt)|*.txt";
            if (openFileDlg.ShowDialog() == DialogResult.OK)
            {
                tbxTrainFilePath.Text = openFileDlg.FileName;
            }
        }

        private void btnTestBrowser_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDlg = new OpenFileDialog();
            openFileDlg.Filter = "(*.txt)|*.txt";
            if (openFileDlg.ShowDialog() == DialogResult.OK)
            {
                tbxTestFilePath.Text = openFileDlg.FileName;
            }
        }

        private void btnModelBrowser_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDlg = new OpenFileDialog();
            openFileDlg.Filter = "(*.txt)|*.txt";
            if (openFileDlg.ShowDialog() == DialogResult.OK)
            {
                tbxModelFilePath.Text = openFileDlg.FileName;
            }
        }

        private void cmbPreprocess_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmbPreprocess.SelectedItem.ToString() == "Return[-1,1]")
            {
                tbxNumDaysPredicted.ReadOnly = false;
            }
            else
            {
                tbxNumDaysPredicted.ReadOnly = true;
            }
        }

        private void rdANN_CheckedChanged(object sender, EventArgs e)
        {
            if (rdANN.Checked == true)
            {
                gbAnnSetting.Enabled = true;
                gbSVRSetting.Enabled = false;
            }
            else
            {
                gbAnnSetting.Enabled = false;
                gbSVRSetting.Enabled = true;
            }
        }

        private void btnStepTrainAndTest_Click(object sender, EventArgs e)
        {
            try
            {
                bool isSVR = true;
                if (rdANN.Checked == true)
                {
                    isSVR = false;
                    ANNParameterBUS.HiddenNode = Convert.ToInt16(tbxANNHiddenNode.Text);
                    ANNParameterBUS.OutputNode = 1;
                    ANNParameterBUS.MaxEpoch = Convert.ToInt16(tbxMaxLoops.Text);
                    ANNParameterBUS.LearningRate = Convert.ToDouble(tbxLearningRate.Text);
                    ANNParameterBUS.Momentum = Convert.ToDouble(tbxMomentum.Text);
                    ANNParameterBUS.Bias = Convert.ToDouble(tbxBias.Text);
                }

                double[][] dblActual_Forecast = new double[2][];
                StepTrainingBUS stepTrainingBUS = new StepTrainingBUS();
                stepTrainingBUS.NumInputNode = Convert.ToInt16(tbxNumInputNode.Text);
                stepTrainingBUS.TrainingSize = Convert.ToInt16(tbxTrainingSize.Text);
                stepTrainingBUS.Preprocess = cmbPreprocess.SelectedItem.ToString();
                stepTrainingBUS.ModelSelection = cmbModelSelection.SelectedItem.ToString();
                stepTrainingBUS.NumFold = tbxNumFold.Text;
                if(ckbImproveDirection.Checked)
                {
                    stepTrainingBUS.ImprovedDirection = true;
                }
                else
                {
                    stepTrainingBUS.ImprovedDirection = false;
                }
                switch (cmbTrainingMeasure.SelectedItem.ToString())
                {
                    case "MSE":
                        stepTrainingBUS.TrainingMeasure = MSE;
                        break;
                    case "NMSE":
                        stepTrainingBUS.TrainingMeasure = NMSE;
                        break;
                    case "RMSE":
                        stepTrainingBUS.TrainingMeasure = RMSE;
                        break;
                    case "Sign":
                        stepTrainingBUS.TrainingMeasure = SIGN;
                        break;
                }
                stepTrainingBUS.LoadWholdeData(tbxCsvFilePath.Text);

                if (tbxStartDate.Text == "WholeData")
                {
                    stepTrainingBUS.CurrentDateIndex = stepTrainingBUS.TrainingSize - 1;
                }
                else
                {
                    stepTrainingBUS.CurrentDateIndex =
                        stepTrainingBUS.FindIndex(DateTime.ParseExact(tbxStartDate.Text, "d/M/yyyy", null));
                }
                int iEndIndex;
                if (tbxEndDate.Text == "WholeData")
                {
                    iEndIndex = stepTrainingBUS.WholeData.Entries.Count-2;
                }
                else
                {
                    iEndIndex =
                        stepTrainingBUS.FindIndex(DateTime.ParseExact(tbxEndDate.Text, "d/M/yyyy", null));
                }
                if (iEndIndex == -1 || iEndIndex > stepTrainingBUS.WholeData.Entries.Count - 2 || iEndIndex < stepTrainingBUS.CurrentDateIndex)
                {
                    MessageBox.Show("Error: Invalid input!");
                    return;
                }

                int iLen = iEndIndex + 1 - stepTrainingBUS.CurrentDateIndex;
                dblActual_Forecast[0] = new double[iLen];
                dblActual_Forecast[1] = new double[iLen];
                for (int i = 0; i < iLen; i++)
                {
                    EntryDTO entryDTO = (EntryDTO)stepTrainingBUS.WholeData.Entries[stepTrainingBUS.CurrentDateIndex + 1];
                    dblActual_Forecast[0][i] = entryDTO.ClosePrice;
                    dblActual_Forecast[1][i] = stepTrainingBUS.TrainAndPredict(isSVR);
                    stepTrainingBUS.CurrentDateIndex++;
                }

                //Ghi kết quả cuối cùng ra file
                StreamWriter finalResult = new StreamWriter("StepPredict.txt");
                for (int i = 0; i < iLen; i++)
                {
                    finalResult.WriteLine(dblActual_Forecast[0][i].ToString() + "\t" + dblActual_Forecast[1][i].ToString());
                }
                finalResult.Close();

                HandleMeasure("StepMeasure.txt", dblActual_Forecast[0], dblActual_Forecast[1]);
                MessageBox.Show("Finish!");
            }
            catch (Exception ex)
            {

                throw ex;
            }
        }

        private void cmbStockID_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmbStockID.SelectedItem != null)
            {
                strStockPath = "Excel" + "\\" + cmbStockID.SelectedItem.ToString() + ".csv";
                _stockRecordDTO = _stockRecordBUS.LoadData(strStockPath);
                dtpFrom.Value = ((EntryDTO)_stockRecordDTO.Entries[0]).Date;
                dtpTo.Value = ((EntryDTO)_stockRecordDTO.Entries[_stockRecordDTO.Entries.Count - 1]).Date;
                dtpInputDay.Value = ((EntryDTO)_stockRecordDTO.Entries[_stockRecordDTO.Entries.Count - 1]).Date;
                CreateGraph(zg1);
            }            
        }

        private void btnPredict_Click(object sender, EventArgs e)
        {
            //Ma trận với dòng thứ 1 chứa các giá trị thực và dòng thứ 2 chứa các giá trị dự đoán.
            double[][] dblActual_Forecast = new double[2][];
            dblActual_Forecast[0] = null;
            dblActual_Forecast[1] = null;

            #region Price
            #region Ghi File Test
            //Dùng tạm số node input = 5, sau này bổ sung hàm đọc file số node input
            int numInputNode = 5;

            //Tìm ngày cần dự đoán
            DateTime inputDay = dtpInputDay.Value;
            DateTime Today = ((EntryDTO)_stockRecordDTO.Entries[_stockRecordDTO.Entries.Count - 1]).Date;
            int i;
            for (i = 0; i < _stockRecordDTO.Entries.Count - numInputNode; i++)
            {
                EntryDTO curEntry = (EntryDTO)_stockRecordDTO.Entries[i + numInputNode];
                if (inputDay.Subtract(curEntry.Date).Days <= 0 && inputDay.Subtract(Today).Days <= 0)
                {
                    break;
                }
            }

            //Ghi bộ input đầu vào cho ngày dự đoán
            double[] dblSource = new double[numInputNode + 1];
            if (i < _stockRecordDTO.Entries.Count - numInputNode)
            {
                //1. Đọc dữ liệu vào mảng
                double[] dblTemp = new double[_stockRecordDTO.Entries.Count];
                for (int j = 0; j < _stockRecordDTO.Entries.Count; j++)
                {
                    EntryDTO curEntry = (EntryDTO)_stockRecordDTO.Entries[j];
                    dblTemp[j] = curEntry.ClosePrice;
                }

                for (int j = 0; j < numInputNode; j++)
                {
                    EntryDTO curEntry = (EntryDTO)_stockRecordDTO.Entries[i + j];
                    dblSource[j] = curEntry.ClosePrice;
                }

                PreprocessBUS preprocessBUS = new PreprocessBUS();
                preprocessBUS.FindMinMax(dblTemp);
                dblSource = preprocessBUS.PreprocessByMinMax(dblSource);

                //2. Tạo file test. File test này chỉ có 2 dòng:
                //+Dòng 1: Chứa thông tin tiền xử lý
                //+Dòng 2: Giống như 1 dòng của file training, nhưng giá trị đích không biết (để bằng 0)
                StreamWriter writer = new StreamWriter(@"TestPrice.txt");
                writer.WriteLine("ScaleByMinMax " + preprocessBUS.Min.ToString() + " " + preprocessBUS.Max.ToString());

                string strLine = "0 ";
                for (int j = 0; j < numInputNode; j++)
                {
                    strLine += (j + 1).ToString() + ":" + dblSource[j].ToString() + " ";
                }
                writer.WriteLine(strLine);

                writer.Close();
            }
            else
            {
                MessageBox.Show("Error: Invalid input!");
            }
            #endregion
            #region Dự đoán ANN
            ANNModelBUS.AnnModelFile = @"AppModel\ANNPrice\" + cmbStockID.SelectedItem.ToString() + "_1_model.txt";

            ANNParameterBUS.LoadParameter();

            ANNPredictBUS annPredict = new ANNPredictBUS();
            annPredict.LoadDataSet(@"TestPrice.txt");
            dblActual_Forecast = annPredict.MainProcess();
            pricePredict = dblActual_Forecast[1][0];
            tbxANNPrice.Text = Math.Round(dblActual_Forecast[1][0], 2).ToString();
            #endregion
            string[] strArgs = new string[3];
            strArgs[0] = @"TestPrice.txt";
            strArgs[1] = @"AppModel\SVRPrice\" + cmbStockID.SelectedItem.ToString() + "_1_model.txt";
            strArgs[2] = "price_predicted.txt";
            dblActual_Forecast = svm_predict.MainProcess(strArgs);
            tbxSVMPrice.Text = Math.Round(dblActual_Forecast[1][0], 2).ToString();
            #endregion
            #region Trend
            //Dùng tạm số node input = 5, sau này bổ sung hàm đọc file số node input
            numInputNode = 5;

            //Tìm ngày cần dự đoán
            inputDay = dtpInputDay.Value;
            Today = ((EntryDTO)_stockRecordDTO.Entries[_stockRecordDTO.Entries.Count - 1]).Date;
            for (i = 0; i < _stockRecordDTO.Entries.Count; i++)
            {
                EntryDTO curEntry = (EntryDTO)_stockRecordDTO.Entries[i];
                if (inputDay.Subtract(curEntry.Date).Days <= 0 && inputDay.Subtract(Today).Days <= 0)
                {
                    break;
                }
            }
            int numDaysPredicted = 1; 
            if (!int.TryParse(tbxNumDayTrendPredict.Text, out numDaysPredicted))
            {
                MessageBox.Show("Please enter a number");
                return;
            } 

            //Ghi bộ input đầu vào cho ngày dự đoán
            dblSource = new double[numInputNode * numDaysPredicted + numDaysPredicted + 1];
            if (i < _stockRecordDTO.Entries.Count && i > numInputNode * numDaysPredicted)
            {
                for (int k = 0; k < dblSource.Length; k++)
                {
                    dblSource[k] = 0;
                }
                //1. Đọc dữ liệu vào mảng
                for (int j = 0; j < numInputNode * numDaysPredicted; j++)
                {
                    EntryDTO curEntry = (EntryDTO)_stockRecordDTO.Entries[i - j];
                    dblSource[j] = curEntry.ClosePrice;
                }
                for (int k = 0; k < dblSource.Length; k++)
                {
                    if (dblSource[k] == 0)
                    {
                        dblSource[k] = dblSource[0];
                    }
                }
                PreprocessBUS preprocessBUS = new PreprocessBUS();
                dblSource = preprocessBUS.Scale_SVR_Return(dblSource.Length, dblSource, 1, 1);

                //2. Chuyển sang định dạng của LibSVM (dựa vào số node đầu vào)
                ConverterBUS converter = new ConverterBUS();
                  
                int iNumLine = 0;

                converter.ConvertForTrend(numDaysPredicted, numInputNode, dblSource, "TestTrend.txt", out iNumLine, 2, true);

                #region Dự đoán ANN

                if (numDaysPredicted >= 1 && numDaysPredicted < 5)
                {
                    ANNModelBUS.AnnModelFile = @"AppModel\ANNTrend\" + cmbStockID.SelectedItem.ToString() + "_1_model.txt";
                    strArgs[1] = @"AppModel\SVRTrend\" + cmbStockID.SelectedItem.ToString() + "_1_model.txt";
                }
                else if (numDaysPredicted >= 5 && numDaysPredicted < 10)
                {
                    ANNModelBUS.AnnModelFile = @"AppModel\ANNTrend\" + cmbStockID.SelectedItem.ToString() + "_5_model.txt";
                    strArgs[1] = @"AppModel\SVRTrend\" + cmbStockID.SelectedItem.ToString() + "_5_model.txt";
                }
                else if (numDaysPredicted >= 10 && numDaysPredicted < 30)
                {
                    ANNModelBUS.AnnModelFile = @"AppModel\ANNTrend\" + cmbStockID.SelectedItem.ToString() + "_10_model.txt";
                    strArgs[1] = @"AppModel\SVRTrend\" + cmbStockID.SelectedItem.ToString() + "_10_model.txt";
                }
                else
                {
                    ANNModelBUS.AnnModelFile = @"AppModel\ANNTrend\" + cmbStockID.SelectedItem.ToString() + "_30_model.txt";
                    strArgs[1] = @"AppModel\SVRTrend\" + cmbStockID.SelectedItem.ToString() + "_30_model.txt";
                }

                ANNParameterBUS.LoadParameter();

                ANNPredictBUS annPredictTrend = new ANNPredictBUS();
                annPredictTrend.LoadDataSet(@"TestTrend.txt");
                dblActual_Forecast = annPredictTrend.MainProcessTrend();
                trendPredict = dblActual_Forecast[1][0];
                tbxANNTrend.Text = dblActual_Forecast[1][0] > 0 ? "Tăng" : "Giảm";

                strArgs[0] = @"TestTrend.txt";
                strArgs[2] = "trend_predicted.txt";
                dblActual_Forecast = svm_predict.MainProcess(strArgs);
                tbxSVMTrend.Text = dblActual_Forecast[1][0] > 0 ? "Tăng" : "Giảm";

                #endregion
                
 
              
            }
            else
            {
                MessageBox.Show("Error: Invalid input!");
            }
            
            
            #endregion            
            
        }

        private void tabOption_Selected(object sender, TabControlEventArgs e)
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

        private void dtpFrom_ValueChanged(object sender, EventArgs e)
        {
            CreateGraph(zg1);
        }

        private void dtpTo_ValueChanged(object sender, EventArgs e)
        {
            CreateGraph(zg1);
        }

        private void rdPricePrediction_CheckedChanged(object sender, EventArgs e)
        {
            if(rdPricePrediction.Checked)
            {
                ckbImproveDirection.Enabled = true;
                tbxNumDaysPredicted.ReadOnly = true;
                btnStepTrainAndTest.Enabled = true;
            }
            else
            {
                ckbImproveDirection.Enabled = false;
                tbxNumDaysPredicted.ReadOnly = false;
                btnStepTrainAndTest.Enabled = false;
            }
        }
    }
}
