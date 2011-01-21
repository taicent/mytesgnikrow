using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace BUS
{
    public class ANNPredictBUS
    {
        #region Attributes
        private double[][] dblActual_Forecast;
        private ANNModelBUS _annModel;

        private int _numPattern;
        private double[][] _arrPattern;

        private PreprocessBUS _preprocess;
        #endregion

        #region Constructors
        public ANNPredictBUS()
        {
        }
        #endregion

        #region Properties
        public double[][] DblActual_Forecast
        {
            get { return dblActual_Forecast; }
            set { dblActual_Forecast = value; }
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

        public PreprocessBUS Preprocess
        {
            get { return _preprocess; }
            set { _preprocess = value; }
        }
        #endregion

        #region Methods
        public void LoadDataSet(string strDataFile)
        {
            StreamReader reader = null;

            try
            {
                reader = new StreamReader(strDataFile);

                string strTemp = reader.ReadLine();
                Preprocess = new PreprocessBUS();
                Preprocess.Min = double.Parse(strTemp.Split(' ')[1]);
                Preprocess.Max = double.Parse(strTemp.Split(' ')[2]);

                strTemp = reader.ReadToEnd();
                reader.Close();

                string[] strLines = Regex.Split(strTemp, "\r\n");

                DblActual_Forecast = new double[2][];
                NumPattern = strLines.Length - 1;
                DblActual_Forecast[0] = new double[NumPattern];
                DblActual_Forecast[1] = new double[NumPattern];
                ArrPattern = new double[NumPattern][];

                for (int i = 0; i < NumPattern; i++)
                {
                    string[] strValue = strLines[i].Split(' ');

                    DblActual_Forecast[0][i] = double.Parse(strValue[0]);

                    ArrPattern[i] = new double[ANNParameterBUS.InputNode];
                    for (int j = 0; j < ANNParameterBUS.InputNode; j++)
                    {
                        ArrPattern[i][j] = double.Parse(strValue[j + 1].Split(':')[1]);
                    }
                }

                ANNModel = new ANNModelBUS();
                ANNModel.LoadModelFile();
            }
            catch (Exception ex)
            {
                throw ex;
            }

        }

        public double[][] MainProcess()
        {
            for (int i = 0; i < NumPattern; i++)
            {
                ANNModel.OutInputLayer = ArrPattern[i];
                DblActual_Forecast[1][i] = ANNModel.FeedforwardTraining();
            }

            DblActual_Forecast[0] = Preprocess.DenormalizeByMinMax(DblActual_Forecast[0]);
            DblActual_Forecast[1] = Preprocess.DenormalizeByMinMax(DblActual_Forecast[1]);

            return DblActual_Forecast;
        }
        public double[][] MainProcessTrend()
        {
            for (int i = 0; i < NumPattern; i++)
            {
                ANNModel.OutInputLayer = ArrPattern[i];
                DblActual_Forecast[1][i] = ANNModel.FeedforwardTraining();
            }

            //DblActual_Forecast[0] = Preprocess.DenormalizeByMinMax(DblActual_Forecast[0]);
            //DblActual_Forecast[1] = Preprocess.DenormalizeByMinMax(DblActual_Forecast[1]);

            return DblActual_Forecast;
        }

        public double[][] StepTrainingMethod()
        {
            ANNParameterBUS.TrainingSize = 35;

            PreprocessBUS preprocessBUS = new PreprocessBUS();
            ANNTrainBUS annTrain = new ANNTrainBUS();

            int len = ArrPattern.Length - ANNParameterBUS.TrainingSize;
            double[][] dblActual_Forecast = new double[2][];
            dblActual_Forecast[0] = new double[len];
            dblActual_Forecast[1] = new double[len];

            ArrPattern = Preprocess.DenormalizeByMinMax(ArrPattern);
            DblActual_Forecast[0] = Preprocess.DenormalizeByMinMax(DblActual_Forecast[0]);
            for (int i = 0; i < len; i++)
            {
                double[][] trainingSet = SelectTrainingSet(i);
                preprocessBUS.FindMinMax(trainingSet);
                trainingSet = preprocessBUS.PreprocessByMinMax(trainingSet);
                annTrain.LoadDataSet(trainingSet);
                annTrain.Main();

                ANNModel.LoadModelFile();
                ANNModel.OutInputLayer = preprocessBUS.PreprocessByMinMax(ArrPattern[i + ANNParameterBUS.TrainingSize - 1]);
                dblActual_Forecast[1][i] = preprocessBUS.DenormalizeByMinMax(ANNModel.FeedforwardTraining());
                dblActual_Forecast[0][i] = DblActual_Forecast[0][i + ANNParameterBUS.TrainingSize];
            }

            DblActual_Forecast = dblActual_Forecast;
            NumPattern = len;

            return dblActual_Forecast;
        }

        public double[][] SelectTrainingSet(int index)
        {
            double[][] result = new double[ANNParameterBUS.TrainingSize][];

            for (int i = 0; i < ANNParameterBUS.TrainingSize; i++)
            {
                result[i] = new double[ANNParameterBUS.InputNode + 1];
                for (int j = 0; j < ANNParameterBUS.InputNode; j++ )
                {
                    result[i][j] = ArrPattern[index + i][j];
                }
                result[i][ANNParameterBUS.InputNode] = dblActual_Forecast[0][index + i];
            }

            return result;
        }

        public void WritePredictPrice(string strPredictedFile)
        {
            StreamWriter write = new StreamWriter(strPredictedFile);
            for (int i = 0; i < NumPattern; i++)
            {
                write.WriteLine(DblActual_Forecast[0][i].ToString() + "\t" + DblActual_Forecast[1][i].ToString());
            }
            write.Close();
        }
        #endregion
    }
}
