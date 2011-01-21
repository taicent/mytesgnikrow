using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.IO;

namespace BUS
{
    public class ANNModelBUS
    {
        #region Attributes
        private double[][] _weightIH;
        private double[] _weightHO;

        private double[] _outInputLayer;
        private double[] _outHiddenLayer;
        private double _outOutputLayer;

        private double[][] _deltaWeightIH;
        private double[] _deltaWeightHO;

        private static string _annModelFile;
        #endregion

        #region Constructors
        public ANNModelBUS()
        {
            OutInputLayer = new double[ANNParameterBUS.InputNode];
            OutHiddenLayer = new double[ANNParameterBUS.HiddenNode];

            WeightIH = new double[ANNParameterBUS.InputNode][];
            WeightHO = new double[ANNParameterBUS.HiddenNode];

            DeltaWeightIH = new double[ANNParameterBUS.InputNode][];
            DeltaWeightHO = new double[ANNParameterBUS.HiddenNode];
            for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
            {
                DeltaWeightHO[j] = 0;
            }

            for (int i = 0; i < ANNParameterBUS.InputNode; i++)
            {
                WeightIH[i] = new double[ANNParameterBUS.HiddenNode];
                DeltaWeightIH[i] = new double[ANNParameterBUS.HiddenNode];

                for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
                {
                    DeltaWeightIH[i][j] = 0;
                }
            }
        }
        #endregion

        #region Properties
        public double[][] WeightIH
        {
            get { return _weightIH; }
            set { _weightIH = value; }
        }

        public double[] WeightHO
        {
            get { return _weightHO; }
            set { _weightHO = value; }
        }

        public double[] OutInputLayer
        {
            get { return _outInputLayer; }
            set { _outInputLayer = value; }
        }

        public double[] OutHiddenLayer
        {
            get { return _outHiddenLayer; }
            set { _outHiddenLayer = value; }
        }

        public double OutOutputLayer
        {
            get { return _outOutputLayer; }
            set { _outOutputLayer = value; }
        }

        public double[][] DeltaWeightIH
        {
            get { return _deltaWeightIH; }
            set { _deltaWeightIH = value; }
        }

        public double[] DeltaWeightHO
        {
            get { return _deltaWeightHO; }
            set { _deltaWeightHO = value; }
        }

        public static string AnnModelFile
        {
            get { return ANNModelBUS._annModelFile; }
            set { ANNModelBUS._annModelFile = value; }
        }
        #endregion

        #region Methods
        public void InitialWeight()
        {
            Random random = new Random();

            //Khởi tạo ngẫu nhiên bộ trọng số liên kết lớp Input - Hidden
            for (int i = 0; i < ANNParameterBUS.InputNode; i++)
            {
                for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
                {
                    WeightIH[i][j] = random.NextDouble();
                }
            }

            //Khởi tạo ngẫu nhiên bộ trọng số liên kết lớp Hidden - Output
            for (int i = 0; i < ANNParameterBUS.HiddenNode; i++)
            {
                WeightHO[i] = random.NextDouble();
            }
        }

        public double[] CalculateOutputOfHiddenLayer(double[] arr)
        {
            double[] result = new double[ANNParameterBUS.HiddenNode];
            for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
            {
                result[j] = ANNParameterBUS.Bias;
                for (int i = 0; i < ANNParameterBUS.InputNode; i++)
                {
                    result[j] += (WeightIH[i][j] * arr[i]);
                }

                result[j] = PreprocessBUS.NormalizeBySigmoid(result[j]);
            }

            return result;
        }

        public double CalculateOutputOfOutputLayer(double[] arr)
        {
            double result = ANNParameterBUS.Bias;
            for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
            {
                result += (WeightHO[j] * arr[j]);
            }

            return result;
        }

        public double FeedforwardTraining()
        {
            OutHiddenLayer = CalculateOutputOfHiddenLayer(OutInputLayer);
            OutOutputLayer = CalculateOutputOfOutputLayer(OutHiddenLayer);

            return OutOutputLayer;
        }

        public void ErrorBackpropagationTraining(double actualValue)
        {
            //Tính độ lỗi của Lớp Output
            double errorOut = actualValue - OutOutputLayer;

            //Tính độ lỗi của các neural lớp Hidden
            double[] errorHidden = new double[ANNParameterBUS.HiddenNode];
            for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
            {
                errorHidden[j] = OutHiddenLayer[j] * (1 - OutHiddenLayer[j]) * errorOut * WeightHO[j];

                //Cập nhật trọng số liên kết lớp Hidden - Output
                DeltaWeightHO[j] = ANNParameterBUS.LearningRate * errorOut * OutHiddenLayer[j] + ANNParameterBUS.Momentum * DeltaWeightHO[j];
                WeightHO[j] = WeightHO[j] + DeltaWeightHO[j];

                //Cập nhật trọng số liên kết lớp Input - Hidden
                for (int i = 0; i < ANNParameterBUS.InputNode; i++)
                {
                    DeltaWeightIH[i][j] = ANNParameterBUS.LearningRate * errorHidden[j] * OutInputLayer[i] + ANNParameterBUS.Momentum * DeltaWeightIH[i][j];
                    WeightIH[i][j] = WeightIH[i][j] + DeltaWeightIH[i][j];
                }
            }
        }
        
        public void SaveModelFile()
        {
            try
            {
                TextWriter writer = new StreamWriter(AnnModelFile);

                //Các tham số của mô hình ANN
                writer.WriteLine(ANNParameterBUS.InputNode);
                writer.WriteLine(ANNParameterBUS.HiddenNode);

                //Bộ trọng số liên kết các lớp
                string strLine = "";
                for (int i = 0; i < ANNParameterBUS.InputNode; i++)
                {
                    strLine = "";
                    for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
                    {
                        strLine += (WeightIH[i][j].ToString() + " ");
                    }

                    writer.WriteLine(strLine);
                }

                strLine = "";
                for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
                {
                    strLine += (WeightHO[j].ToString() + " ");
                }

                writer.WriteLine(strLine);

                writer.Close();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void LoadModelFile()
        {
            try
            {
                TextReader reader = new StreamReader(AnnModelFile);

                reader.ReadLine();
                reader.ReadLine();

                //Bộ trọng số liên kết các lớp
                string strLine = "";
                string[] strSplit;
                for (int i = 0; i < ANNParameterBUS.InputNode; i++)
                {
                    strLine = reader.ReadLine();
                    strSplit = strLine.Split(' ');
                    for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
                    {
                        WeightIH[i][j] = double.Parse(strSplit[j]);
                    }
                }

                strLine = reader.ReadLine();
                strSplit = strLine.Split(' ');
                for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
                {
                    WeightHO[j] = double.Parse(strSplit[j]);
                }

                reader.Close();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void SaveError_MaxLoop(double error, int loop)
        {
            try
            {
                string strTemp = AnnModelFile;
                int iPos = strTemp.LastIndexOf('_');

                string strErrMaxFile = strTemp.Remove(iPos + 1) + "Error_Maxloop.txt";
                TextWriter writer = new StreamWriter(strErrMaxFile);

                writer.WriteLine(ANNParameterBUS.MeasureType + ": " + error.ToString());
                writer.WriteLine("Maxloop: " + loop.ToString());

                writer.Close();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void ErrorBackpropagationTrainingImprove(double actualValue)
        {
            //Tính độ lỗi của Lớp Output
            double errorOut = actualValue - OutOutputLayer;

            //Tính độ lỗi của các neural lớp Hidden
            double[] errorHidden = new double[ANNParameterBUS.HiddenNode];
            for (int j = 0; j < ANNParameterBUS.HiddenNode; j++)
            {
                errorHidden[j] = OutHiddenLayer[j] * (1 - OutHiddenLayer[j]) * errorOut * WeightHO[j];

                //Cập nhật trọng số liên kết lớp Hidden - Output
                DeltaWeightHO[j] = ANNParameterBUS.LearningRate * errorOut * OutHiddenLayer[j] + ANNParameterBUS.Momentum * DeltaWeightHO[j];
                WeightHO[j] = WeightHO[j] + DeltaWeightHO[j];

                //Cập nhật trọng số liên kết lớp Input - Hidden
                for (int i = 0; i < ANNParameterBUS.InputNode; i++)
                {
                    DeltaWeightIH[i][j] = ANNParameterBUS.LearningRate * errorHidden[j] * OutInputLayer[i] + ANNParameterBUS.Momentum * DeltaWeightIH[i][j];
                    WeightIH[i][j] = WeightIH[i][j] + DeltaWeightIH[i][j];
                }
            }
        }
        #endregion
    }
}
