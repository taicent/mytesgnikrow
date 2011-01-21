using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BUS
{
    public class MeasureBUS
    {
        #region Attributes
        #endregion

        #region Constructors
        #endregion

        #region Properties
        #endregion

        #region Methods

        //Mean Square Error
        public double MSE(double[] actualValues, double[] forecastValues)
        {
            double dblMse = 0.0;
            for (int i = 0; i < actualValues.Length; i++)
            {
                dblMse = dblMse + Math.Pow((actualValues[i] - forecastValues[i]), 2);
            }
            dblMse = dblMse / (2 * actualValues.Length);
            return dblMse;
        }

        //Normalized Mean Square Error
        public double NMSE(double[] actualValues, double[] forecastValues)
        {
            double dblNmse = 0.0;
            double dblAvgValue = 0.0;
            double dblSigma2 = 0.0;

            //Tính giá trị trung bình
            for (int i = 0; i < forecastValues.Length; i++)
            {
                dblAvgValue = dblAvgValue + forecastValues[i];
            }
            dblAvgValue = dblAvgValue / forecastValues.Length;

            //Tính giá trị sigma2
            for (int i = 0; i < actualValues.Length; i++)
            {
                double dblTemp = Math.Pow((actualValues[i] - dblAvgValue), 2);
                dblSigma2 = dblSigma2 + dblTemp;
            }
            dblSigma2 = dblSigma2 / (actualValues.Length - 1);

            //Tính độ lỗi NMSE
            for (int i = 0; i < actualValues.Length; i++)
            {
                double dblTemp =  Math.Pow((actualValues[i] - forecastValues[i]), 2);
                dblNmse = dblNmse + dblTemp;
            }
            dblNmse = dblNmse / (dblSigma2 * actualValues.Length);
            return dblNmse;
        }

        //Root Mean Square Error
        public double RMSE(double[] actualValues, double[] forecastValues)
        {
            double dblRmse = 0.0;
            for (int i = 0; i < actualValues.Length; i++)
            {
                dblRmse = dblRmse + Math.Pow((actualValues[i] - forecastValues[i]), 2);
            }
            dblRmse = dblRmse / (2 * actualValues.Length);
            dblRmse = Math.Sqrt(dblRmse);
            return dblRmse;
        }

        //Mean Absolute Error
        public double APE(double[] actualValues, double[] forecastValues)
        {
            double dblApe = 0.0;
            for (int i = 0; i < actualValues.Length; i++)
            {
                double dblTemp = Math.Abs((actualValues[i] - forecastValues[i]));
                dblApe = dblApe + dblTemp;
            }
            dblApe = dblApe / actualValues.Length;
            return dblApe;
        }

        //Mean Absolute Percentage Error 
        public double MAPE(double[] actualValues, double[] forecastValues)
        {
            double dblMape = 0.0;
            for (int i = 0; i < actualValues.Length; i++)
            {
                double dblTemp = Math.Abs((actualValues[i] - forecastValues[i])/actualValues[i]) * 100;
                dblMape = dblMape + dblTemp;
            }
            dblMape = dblMape / actualValues.Length;
            return dblMape;          
        }

        //Directional Symmetry
        public double DS(double[] actualValues, double[] forecastValues)
        {
            double dblDs = 0.0;
            for (int i = 1; i < actualValues.Length; i++)
            {
                int id;
                double dblTemp = (actualValues[i] - actualValues[i-1])*(forecastValues[i] - forecastValues[i-1]);

                if (dblTemp >= 0)
                {
                    id = 1;
                }
                else
                {
                    id = 0;
                }

                dblDs = dblDs + id;
            }
            dblDs = (dblDs * 100)/(actualValues.Length -1);            
            return dblDs;
        }

        //Weighted Directional  Symmetry
        public double WDS(double[] actualValues, double[] forecastValues)
        {
            double dblWds = 0.0;

            double dblG = 1.5;//Hằng Số
            double dblH = 0.5;//Hằng Số

            for (int i = 1; i < actualValues.Length ; i++)
            {
                double id;
                double dblTemp = (actualValues[i] - actualValues[i - 1]) * (forecastValues[i] - forecastValues[i - 1]);

                if (dblTemp <= 0)
                {
                    id = dblG;
                }
                else
                {
                    id = dblH;
                }

                dblWds = dblWds + id * (actualValues[i] - forecastValues[i]);
            }
            dblWds = (dblWds * 100) / (actualValues.Length-1);
            return dblWds;
        }

        public double Sign(double[] actualValues, double[] forecastValues)
        {
            int iTruePositive = 0;
            double dblResult = 0;

            for (int i = 1; i < actualValues.Length; i++)
            {
                if (actualValues[i]* forecastValues[i] > 0)
                {
                    iTruePositive++;
                }
            }
            dblResult = 100 * iTruePositive / actualValues.Length;
            return dblResult;
        }

        //Direction Measure For Price
        public double [] DirectionMeasure4Price(double[] actualValues, double[] forecastValues)
        {
            //Mảng kết quả gồm 3 phần tử:
            //+Phần tử 0: Tỉ lệ % dự đoán đúng.
            //+Phần tử 1: Tỉ lệ % dự đoán sai.
            //+Phần tử 2: Tỉ lệ % không đánh giá được mô hình (giá thực không thay đổi.)
            double []dblResult = new double[]{0.0, 0.0, 0.0};
            for (int i = 1; i < actualValues.Length; i++)
            {
                double dblTemp = (actualValues[i] - actualValues[i - 1]) * (forecastValues[i] - actualValues[i - 1]);

                if (dblTemp > 0)    //Dự đoán đúng
                {
                    dblResult[0] += 1;
                }
                else if (dblTemp < 0)  //Dự đoán sai
                {
                    dblResult[1] += 1;
                }
                else   //Không đánh giá được
                {
                    dblResult[2] += 1;
                }
            }
            dblResult[0] = (dblResult[0] * 100) / (actualValues.Length - 1);
            dblResult[1] = (dblResult[1] * 100) / (actualValues.Length - 1);
            dblResult[2] = (dblResult[2] * 100) / (actualValues.Length - 1);
            return dblResult;
        }
        #endregion
    }
}