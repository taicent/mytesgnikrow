using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DTO;
using DAO;
using System.Collections;
using System.IO;
using System.Text.RegularExpressions;

namespace BUS
{
    public class ANNTrainBUS
    {
        #region Attributes
        private double[] _arrActual;
        private double[] _arrPredict;
        private double[] _arrDesiredValue;

        private ANNModelBUS _annModel;

        private int _numPattern;
        private double[][] _arrPattern;

        private double _error;

        public const int MSE = 1;
        public const int NMSE = 2;
        public const int RMSE = 3;
        public const int SIGN = 4;

        #endregion

        #region Constructors
        public ANNTrainBUS()
        {
        }
        #endregion

        #region Properties
        public double[] ArrActual
        {
            get { return _arrActual; }
            set { _arrActual = value; }
        }

        public double[] ArrPredict
        {
            get { return _arrPredict; }
            set { _arrPredict = value; }
        }

        public double[] ArrDesiredValue
        {
            get { return _arrDesiredValue; }
            set { _arrDesiredValue = value; }
        }

        public ANNModelBUS ANNModel
        {
            get { return _annModel; }
            set { _annModel = value; }
        }

        public int NumPattern
        {
            get { return _numPattern; }
            set { _numPattern = value; }
        }

        public double[][] ArrPattern
        {
            get { return _arrPattern; }
            set { _arrPattern = value; }
        }

        public double Error
        {
            get { return _error; }
            set { _error = value; }
        }       
        #endregion

        #region Methods
        public void LoadDataSet(string strDataFile)
        {
            StreamReader reader = null;

            try
            {
                reader = new StreamReader(strDataFile);
                string strTemp = reader.ReadToEnd();
                reader.Close();

                string[] strLines = Regex.Split(strTemp, "\r\n");

                NumPattern = strLines.Length - 1;
                ArrActual = new double[NumPattern];
                ArrPredict = new double[NumPattern];
                ArrPattern = new double[NumPattern][];

                for (int i = 0; i < NumPattern; i++)
                {
                    string[] strValue = strLines[i].Split(' ');

                    ArrActual[i] = double.Parse(strValue[0]);
                    
                    ArrPattern[i] = new double[ANNParameterBUS.InputNode];
                    for (int j = 0; j < ANNParameterBUS.InputNode; j++ )
                    {
                        ArrPattern[i][j] = double.Parse(strValue[j + 1].Split(':')[1]);
                    }
                }

                ANNModel = new ANNModelBUS();
            }
            catch (Exception ex)
            {
                throw ex;
            }
            
        }

        public void LoadDataSet(double[][] trainingSet)
        {
            NumPattern = trainingSet.Length;
            ArrActual = new double[NumPattern];
            ArrPredict = new double[NumPattern];
            ArrPattern = new double[NumPattern][];

            for (int i = 0; i < NumPattern; i++)
            {
                ArrActual[i] = trainingSet[i][ANNParameterBUS.InputNode];

                ArrPattern[i] = new double[ANNParameterBUS.InputNode];
                for (int j = 0; j < ANNParameterBUS.InputNode; j++)
                {
                    ArrPattern[i][j] = trainingSet[i][j];
                }
            }

            ANNModel = new ANNModelBUS();
        }

        public void Main()
        {
            //Khởi tạo bộ trọng số ngẫu nhiên cho mô hình
            ANNModel.InitialWeight();
            TSCFDCostFunction();

            int loop = 1;
            bool bContinue = true;

            MeasureBUS measureBUS = new MeasureBUS();
                        
            while (bContinue)
            {
                for (int i = 0; i < NumPattern; i++)
                {
                    ANNModel.OutInputLayer = ArrPattern[i];
                    ArrPredict[i] = ANNModel.FeedforwardTraining();
                }

                Error = measureBUS.NMSE(ArrActual, ArrPredict);

                if (Error <= 0.01 || loop >= ANNParameterBUS.MaxEpoch)
                {
                    bContinue = false;
                    ANNModel.SaveModelFile();
                    //ghi nhận độ lỗi và số vòng lặp của quá trình train
                    ANNModel.SaveError_MaxLoop(Error, loop);
                }
                else
                {
                    for (int i = 0; i < NumPattern; i++)
                    {
                        ANNModel.OutInputLayer = ArrPattern[i];
                        ANNModel.FeedforwardTraining();
                        ANNModel.ErrorBackpropagationTraining(ArrDesiredValue[i]);
                    }
                }

                loop++;
            }
        }
        
        public void Main(int measureType)
        {
            //Khởi tạo bộ trọng số ngẫu nhiên cho mô hình
            ANNModel.InitialWeight();
            TSCFDCostFunction();

            int loop = 1;
            bool bContinue = true;

            MeasureBUS measureBUS = new MeasureBUS();

            while (bContinue)
            {
                for (int i = 0; i < NumPattern; i++)
                {
                    ANNModel.OutInputLayer = ArrPattern[i];
                    ArrPredict[i] = ANNModel.FeedforwardTraining();
                }
                            
                switch (measureType)
                {
                    case MSE:
                        Error = measureBUS.MSE(ArrActual, ArrPredict);
                        break;
                    case NMSE:
                        Error = measureBUS.NMSE(ArrActual, ArrPredict);
                        break;
                    case RMSE:
                        Error = measureBUS.RMSE(ArrActual, ArrPredict);
                        break;
                    case SIGN:
                        Error = measureBUS.Sign(ArrActual, ArrPredict);
                        break;
                }

                bool bResult = CompareError_Accurancy(Error, ANNParameterBUS.Accuracy, measureType);
                if (bResult == true || loop >= ANNParameterBUS.MaxEpoch)
                {
                    bContinue = false;
                    ANNModel.SaveModelFile();
                    //ghi nhận độ lỗi và số vòng lặp của quá trình train
                    ANNModel.SaveError_MaxLoop(Error, loop);
                }
                else
                {
                    for (int i = 0; i < NumPattern; i++)
                    {
                        ANNModel.OutInputLayer = ArrPattern[i];
                        ANNModel.FeedforwardTraining();
                        ANNModel.ErrorBackpropagationTraining(ArrDesiredValue[i]);
                    }
                }

                loop++;
            }
        }
        
        public void TSCFDCostFunction()
        {
            ArrDesiredValue = ArrActual;

            if (ArrActual.Length > 2)
            {
                double prev = 0;
                double cur = ArrActual[0];
                double next = ArrActual[1];
                for (int i = 1; i < ANNParameterBUS.TrainingSize - 1; i++)
                {
                    prev = cur;
                    cur = next;
                    next = ArrActual[i + 1];

                    if ((cur - prev) * (next - cur) > 0)
                    {
                        //ArrDesiredValue[i] = next;
                    }
                }              
            }
        }

        public bool CompareError_Accurancy(double error, double accuracy, int measureType)
        {
            bool bResult = false;
            switch (measureType)
            {
                case MSE:
                    error = error * 100;
                    if (error <= 100 - accuracy)
                    {
                        bResult = true;
                    }
                    else
                    {
                        bResult = false;
                    }
                    break;
                case NMSE:
                    error = error * 100;
                    if (error <= 100 - accuracy)
                    {
                        bResult = true;
                    }
                    else
                    {
                        bResult = false;
                    }
                    break;
                case RMSE:
                    
                    break;
                case SIGN:
                    if (error >= accuracy)
                    {
                        bResult = true;
                    }
                    else
                    {
                        bResult = false;
                    }
                    break;
            }
            return bResult;
        }
        #endregion
    }
}
