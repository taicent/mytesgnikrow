using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using libsvm;

namespace BUS
{
    public class svm_modelSelection
    {
        //Để ý là ta đang làm với epsilon-intensitive SVR với Gaussian Kernel
        #region Attributes
        private svm_train _trainer;
        private double _X;  //X = ln(C)
        private double _Y;  //Y = ln(sigma) với sigma là phương sai của hàm Gauss
        private double _Z;  //Z = ln(1/epsilon)
        // Phần grid search
        private double _startX; // điểm bắt đầu của C
        private double _startY; // điểm bắt đầu của gamma
        private double _startZ; // điểm bắt đầu của epsilon
        private int _interval;  // số điểm X, Y, Z ==> tổng số grid sẽ là interval^3
        private double _increment; // độ tăng của 1 điểm

        #endregion

        #region Default Constructor
        public svm_modelSelection()
        {
            Trainer = new svm_train();
            StartX = 2E-5;
            StartY = 2E-15;
            StartZ = 2E-5;
            Interval = 10;
            Increment = 2;
        }
        #endregion

        #region Properties
        public svm_train Trainer
        {
            get { return _trainer; }
            set { _trainer = value; }
        }
        public double X
        {
            get { return _X; }
            set { _X = value; }
        }
        public double Y
        {
            get { return _Y; }
            set { _Y = value; }
        }
        public double Z
        {
            get { return _Z; }
            set { _Z = value; }
        }
        public double StartX
        {
            get { return _startX; }
            set { _startX = value; }
        }

        public double StartY
        {
            get { return _startY; }
            set { _startY = value; }
        }

        public double StartZ
        {
            get { return _startZ; }
            set { _startZ = value; }
        }

        public int Interval
        {
            get { return _interval; }
            set { _interval = value; }
        }

        public double Increment
        {
            get { return _increment; }
            set { _increment = value; }
        }
        #endregion

        #region Methods
        public void PatternSearch()
        {
            //B1: Khởi tạo
            int[,] iMatrixP = new int[3, 6] { { 1, 0, 0, -1, 0, 0 }, { 0, 1, 0, 0, -1, 0 }, { 0, 0, 1, 0, 0, -1 } };
            double dblSearchStep = 1;
            double dblThreshold = 0.04;
            Random random = new Random();
            //Chọn ngẫu nhiên trung tâm của pattern
            X = -2 + random.NextDouble() * 12;  //X thuộc [-2, 10]
            Y = -2 + random.NextDouble() * 12;  //Y thuộc [-2, 10]
            Z = 1 + random.NextDouble() * 9;  //X thuộc [1, 9]
            double dblMin = ObjectiveFunc(X, Y, Z);
            //B2: Vòng lặp
            while (dblSearchStep >= dblThreshold)
            {
                for (int i = 0; i < 6; i++)
                {
                    double[] dblSurroundPoint = new double[3];
                    dblSurroundPoint[0] = X + dblSearchStep * iMatrixP[0, i];
                    dblSurroundPoint[1] = X + dblSearchStep * iMatrixP[1, i];
                    dblSurroundPoint[2] = X + dblSearchStep * iMatrixP[2, i];
                    double dblValue = ObjectiveFunc(dblSurroundPoint[0], dblSurroundPoint[1], dblSurroundPoint[2]);
                    if (dblValue < dblMin)
                    {
                        dblMin = dblValue;
                        X = dblSurroundPoint[0];
                        Y = dblSurroundPoint[1];
                        Z = dblSurroundPoint[2];
                        i = -1; //Chạy lại vòng lặp mới
                    }
                }
                dblSearchStep /= 2;
            }
            StreamWriter writer = new StreamWriter("PatternSearch.txt");
            writer.WriteLine(dblMin.ToString());
            writer.Close();
        }
        public double ObjectiveFunc(double x, double y, double z)   //Hàm mục tiêu. Dùng 10 folds cross validation với độ lỗi MSE
        {
            //Trainer.Nr_fold = 10;
            Trainer.Param.C = Math.Pow(Math.E, x);
            Trainer.Param.gamma = 1 / (2.0 * Math.Pow(Math.E, y) * Math.Pow(Math.E, y));
            Trainer.Param.p = 1 / Math.Pow(Math.E, z);  //p: epsilon
            //Trainer.Param.svm_type = svm_parameter.EPSILON_SVR;
            //Trainer.Param.kernel_type = svm_parameter.RBF;

            return Trainer.do_cross_validation();
        }
        public void GridSearch()
        {

        }
        public void Run(string[] args)
        {
            Trainer.parse_command_line(args);
            Trainer.read_problem();
            //error_msg = svm.svm_check_parameter(prob, param);

            //if ((System.Object)error_msg != null)
            //{
            //    //System.Console.Error.Write("Error: " + error_msg + "\n");
            //    System.Environment.Exit(1);
            //}

            PatternSearch();
        }
        #endregion
        
    }
}
