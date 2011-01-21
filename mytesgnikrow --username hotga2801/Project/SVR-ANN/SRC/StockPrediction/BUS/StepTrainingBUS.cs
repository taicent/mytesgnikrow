using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using DAO;
using DTO;

namespace BUS
{
    public class StepTrainingBUS
    {
        #region Attributes
        private StockRecordDTO _wholeData;
        private int _currentDateIndex;    //Chỉ số của ngày "hiện tại" (ngày liền trước ngày cần dự đoán)
        private int _trainingSize;  //Kích thước (số ngày) của tập train
        private int _numInputNode;  //Số ngày (trước ngày dự đoán) cần lấy thông tin để dự đoán   

        private string _preprocess;
        private int _numPredictedDay;   //Dành cho tiền xử lý return của xu hướng
        private string _modelSelection;
        private string _numFold;   //Số fold cross-validation. Qui ước: 1: Leave one out
        private int _trainingMeasure;
        private int _crossValidationType;
        private bool _improvedDirection;
        #endregion

        #region Properties
        public StockRecordDTO WholeData
        {
            get { return _wholeData; }
            set { _wholeData = value; }
        }
        public int CurrentDateIndex
        {
            get { return _currentDateIndex; }
            set { _currentDateIndex = value; }
        }

        public int TrainingSize
        {
            get { return _trainingSize; }
            set { _trainingSize = value; }
        }

        public int NumInputNode
        {
            get { return _numInputNode; }
            set { _numInputNode = value; }
        }

        public string Preprocess
        {
            get { return _preprocess; }
            set { _preprocess = value; }
        }

        public int NumPredictedDay
        {
            get { return _numPredictedDay; }
            set { _numPredictedDay = value; }
        }

        public string ModelSelection
        {
            get { return _modelSelection; }
            set { _modelSelection = value; }
        }

        public string NumFold
        {
            get { return _numFold; }
            set { _numFold = value; }
        }

        public int TrainingMeasure
        {
            get { return _trainingMeasure; }
            set { _trainingMeasure = value; }
        }

        public int CrossValidationType
        {
            get { return _crossValidationType; }
            set { _crossValidationType = value; }
        }

        public bool ImprovedDirection
        {
            get { return _improvedDirection; }
            set { _improvedDirection = value; }
        }

        #endregion

        #region Methods
        public void LoadWholdeData(string fileName)
        {
            StockRecordBUS stockRecordBUS = new StockRecordBUS();
            WholeData = stockRecordBUS.LoadData(fileName);
        }
        public double[] PickNeededData()
        {
            double[] dblResult = new double[TrainingSize];
            for (int i = 0; i < TrainingSize; i++)
            {
                EntryDTO entryDTO = (EntryDTO)WholeData.Entries[CurrentDateIndex - TrainingSize + 1 + i];
                dblResult[i] = entryDTO.ClosePrice;
            }
            return dblResult;
        }
        public int FindIndex(DateTime date) //Tìm chỉ số dựa vào ngày
        {
            int i = 0;
            foreach (EntryDTO entry in WholeData.Entries)
            {
                if(entry.Date == date)
                {
                    return i;
                }
                i++;
            }
            return -1;  //Tìm không thấy
        }

        public double TrainAndPredict(bool isSVR)   //Giá trị trả về là giá dự đoán
        {
            double[][] dblActual_Forecast = null;
            #region Phần chung
            
            //1. Lấy đoạn dữ liệu cần thiết
            double[] dblData = PickNeededData();
            //2. Tiền xử lý
            PreprocessBUS preprocessBUS = new PreprocessBUS();
            if (Preprocess == "ScaleByMinMax")
            {
                preprocessBUS.FindMinMax(dblData);
                dblData = preprocessBUS.PreprocessByMinMax(dblData);
            }
            ConverterBUS converter = new ConverterBUS();
            int iNumLine = 0;
            if(ImprovedDirection)
            {
                converter.ConvertWImprovedDirection(NumInputNode, dblData, "Training.txt", 100, out iNumLine);
            }
            else
            {
                converter.Convert(NumInputNode, dblData, "Training.txt", out iNumLine);    
            }
            
            //Tạo file test. File test này chỉ có 2 dòng:
            //+Dòng 1: Chứa thông tin tiền xử lý
            //+Dòng 2: Giống như 1 dòng của file training, nhưng giá trị đích không biết (để bằng 0)
            StreamWriter writer = new StreamWriter("Test.txt");

            if (Preprocess == "ScaleByMinMax")
            {
                writer.WriteLine("ScaleByMinMax " + preprocessBUS.Min.ToString() + " " + preprocessBUS.Max.ToString());
            }
            string strLine = "0 ";
            for (int i = 0; i < NumInputNode; i++)
            {
                strLine = strLine + (i+1).ToString() + ":" + dblData[TrainingSize - NumInputNode + i].ToString() + " ";
            }
            writer.WriteLine(strLine);

            writer.Close();
            #endregion

            #region Phần riêng
            //3. Huấn luyện và dự đoán
            #region SVR
            if (isSVR == true)   //SVR
            {
                string[] strArgs = new string[8];
                strArgs[0] = "-g";  //gama. Trong hàm Gauss kernel: exp(-gamma*|u-v|^2)
                strArgs[2] = "-c";
                strArgs[4] = "-p";  //epsilon
                strArgs[6] = "Training.txt";
                strArgs[7] = "SVRModel.txt";
                //Tìm bộ tham số (gama, c, epsilon)
                svm_modelSelection modelSelection = new svm_modelSelection();
                modelSelection.Log = false;
                string[] strArgsModel = new string[4];
                strArgsModel[0] = "-v";
                strArgsModel[1] = NumFold;
                strArgsModel[2] = "Training.txt";
                strArgsModel[3] = "SVRModel.txt";
                if (ModelSelection == "Grid search")
                {
                    modelSelection.Run(strArgsModel, "", true, TrainingMeasure, CrossValidationType);
                    strArgs[1] = modelSelection.Y.ToString(); //gama
                    strArgs[3] = modelSelection.X.ToString(); //c
                    strArgs[5] = modelSelection.Z.ToString(); //epsilon
                    //Huấn luyện
                    svm_train.Main(strArgs);
                    //Dự đoán
                    strArgs = new string[3];
                    strArgs[0] = "Test.txt";
                    strArgs[1] = "SVRModel.txt";
                    strArgs[2] = "Prediction.txt";
                    dblActual_Forecast = svm_predict.MainProcess(strArgs);
                }
                else if(ModelSelection == "Pattern search")
                {
                    const int iNumModel = 10;
                    for (int i = 0; i < iNumModel; i++)
                    {
                        modelSelection = new svm_modelSelection {Log = false};
                        strArgsModel[3] = "SVRModel" + i + 1 + ".txt";
                        modelSelection.Run(strArgsModel, "", false, TrainingMeasure, CrossValidationType);
                        strArgs[1] = (Math.Pow(2, modelSelection.Y)).ToString();   //gama
                        strArgs[3] = (Math.Pow(2, modelSelection.X)).ToString();   //c
                        strArgs[5] = (Math.Pow(2, modelSelection.Z)).ToString();   //epsilon
                        strArgs[7] = "SVRModel" + i + 1 + ".txt";
                        //Huấn luyện
                        svm_train.Main(strArgs);
                    }
                    //Dự đoán
                    double[][][] dblTemp = new double[iNumModel][][];
                    dblActual_Forecast = new double[2][];
                    dblActual_Forecast[0] = new double[]{0};
                    dblActual_Forecast[1] = new double[]{0};
                    
                    for (int i = 0; i < iNumModel; i++)
                    {
                        strArgs = new string[3];
                        strArgs[0] = "Test.txt";
                        strArgs[1] = "SVRModel" + i + 1 + ".txt";
                        strArgs[2] = "Prediction.txt";
                        dblTemp[i] = svm_predict.MainProcess(strArgs);
                        dblActual_Forecast[1][0] += dblTemp[i][1][0];
                    }
                    dblActual_Forecast[1][0] /= iNumModel;
                }
            }
            #endregion
            #region ANN
            else   //ANN
            {
                //Huấn luyện
                ANNParameterBUS.InputNode = NumInputNode;

                ANNModelBUS.AnnModelFile = "ANNModel.txt";
                ANNTrainBUS annTrain = new ANNTrainBUS();
                annTrain.LoadDataSet("Training.txt");
                annTrain.Main();

                //Dự đoán
                ANNParameterBUS.LoadParameter();
                ANNPredictBUS annPredict = new ANNPredictBUS();
                annPredict.LoadDataSet("Test.txt");
                dblActual_Forecast = annPredict.MainProcess();
            }
            #endregion
            #endregion
            return dblActual_Forecast[1][0];    //Giá dự đoán
        }
        #endregion
    }
}
