using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using libsvm;
using System.Collections;

namespace BUS
{
    public class svm_modelSelection
    {
        //Để ý là ta đang làm với epsilon-intensitive SVR với Gaussian Kernel
        #region Attributes
        private svm_train _trainer;
        private bool _log;  //Có ghi lại log hay không
        private double _X;  //X = log2(C)
        private double _Y;  //Y = log2(gamma) với gamma: exp(-gamma*|u-v|^2)
        private double _Z;  //Z = log2(epsilon)
        private double _cvResult;   //Cross validation result
		private List<double[]> _passedPoints = new List<double[]>();   //Danh sách các điểm đã được tìm kiếm. Lưu lại các điểm này là để giảm số lần thực thi cross-validation.
        // Phần grid search
        private double _startX; // điểm bắt đầu của C
        private double _startY; // điểm bắt đầu của gamma
        private double _startZ; // điểm bắt đầu của epsilon
        // Dùng phát hiện những bộ grid nào đã được chạy cross rồi
        private ArrayList _arrX;    // chứa tất cả điểm X đã được dùng
        private ArrayList _arrY;    // chứa tất cả điểm Y đã được dùng
        private ArrayList _arrZ;    // chứa tất cả điểm Z đã được dùng
       
        private int _interval;  // số điểm X, Y, Z ==> tổng số grid sẽ là interval^3
        private double _increment; // độ tăng của 1 điểm
        private double _minErr;

        #endregion

        #region Default Constructor
        public svm_modelSelection()
        {
            Trainer = new svm_train();
            Log = true;

            StartX = Math.Pow(2, -6);
            StartY = Math.Pow(2, -7);
            StartZ = Math.Pow(2, -10);

            ArrX = new ArrayList();
            ArrY = new ArrayList();
            ArrZ = new ArrayList();

            Interval = 7;
            Increment = Math.Pow(2, 2);

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
        public double CvResult
        {
            get { return _cvResult; }
            set { _cvResult = value; }
        }
        private List<double[]> PassedPoints
        {
            get { return _passedPoints; }
            set { _passedPoints = value; }
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
        public ArrayList ArrX
        {
            get { return _arrX; }
            set { _arrX = value; }
        }
        public ArrayList ArrY
        {
            get { return _arrY; }
            set { _arrY = value; }
        }
        public ArrayList ArrZ
        {
            get { return _arrZ; }
            set { _arrZ = value; }
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
        public double MinErr
        {
            get { return _minErr; }
            set { _minErr = value; }
        }

        public bool Log
        {
            get { return _log; }
            set { _log = value; }
        }

        #endregion

        #region Methods
        public bool CheckPass(double[] point)   //Kiểm xem điểm đã từng được tìm kiếm hay chưa
        {
            int i;
            int j;
            for (i = 0; i < PassedPoints.Count; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    if (point[j] != PassedPoints[i][j])
                    {
                        break;
                    }
                }
                if (j == 3)
                {
                    return true;
                }
            }
            //Điểm chưa từng được tìm kiếm --> đưa vào danh sách
            double[] dblPassedPoint = new double[3] { point[0], point[1], point[2] };
            PassedPoints.Add(dblPassedPoint);
            return false;
        }
        public void PatternSearch(string logPath, int measureType, int crossValidationType)
        {
            //B1: Khởi tạo
            StreamWriter writer = null;
            int iTrueCounter = 0;
            int iLoopCounter = 0;
            int iValidationCounter = 0;
            if (Log)
            {
                writer = new StreamWriter(logPath);
            }

            PassedPoints = new List<double[]>();

            int[,] iIdentityMatrix = new int[3, 3] { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
            double dblSearchStep = 1;
            double dblThreshold = 0.04;
            //Chọn ngẫu nhiên trung tâm của pattern
            Random random = new Random();
            X = -4 + random.NextDouble() * 12;  //X thuộc [-4, 8]
            Y = -9 + random.NextDouble() * 12;  //Y thuộc [-9, 3]
            Z = -14 + random.NextDouble() * 12;  //Z thuộc [-14, -2]
            //X = 6; 
            //Y = -7; 
            //Z = -10;
            CvResult = ObjectiveFunc(X, Y, Z, false, measureType, crossValidationType);
            double[] dblPassedPoint = new double[3] { X, Y, Z };
            PassedPoints.Add(dblPassedPoint);
            double[] dblMinPoint = new double[3] { X, Y, Z };

            iValidationCounter++;
            #region Ghi log
            if (writer != null)
            {
                writer.WriteLine("Init: X = " + X.ToString() + " Y = " + Y.ToString() + " Z = " + Z.ToString());
                writer.WriteLine(iLoopCounter.ToString() + " " + CvResult.ToString());
            }
            #endregion

            //B2: Vòng lặp
            while (dblSearchStep >= dblThreshold)
            {
                double[] dblSk = new double[3] { 0, 0, 0 };
                for (int i = 0; i < 3; i++)
                {
                    double[] dblSik = new double[3];
                    for (int j = 0; j < 3; j++)
                    {
                        dblSik[j] = dblSk[j] + dblSearchStep * iIdentityMatrix[i, j];
                    }

                    double[] dblXik = new double[3];
                    dblXik[0] = X + dblSik[0];
                    dblXik[1] = Y + dblSik[1];
                    dblXik[2] = Z + dblSik[2];

                    double dblValue;
                    if (CheckPass(dblXik) == false)
                    {
                        dblValue = ObjectiveFunc(dblXik[0], dblXik[1], dblXik[2], false, measureType, crossValidationType);
                        iValidationCounter++;////////////
                    }
                    else
                    {
                        iTrueCounter++;
                        dblValue = CvResult + 1;    //Nhằm bỏ qua câu lệnh  if (dblValue < CvResult)
                    }

                    if (dblValue < CvResult)
                    {
                        CvResult = dblValue;

                        dblSk[0] = dblSik[0];
                        dblSk[1] = dblSik[1];
                        dblSk[2] = dblSik[2];

                        dblMinPoint[0] = dblXik[0];
                        dblMinPoint[1] = dblXik[1];
                        dblMinPoint[2] = dblXik[2];
                    }
                    else
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            dblSik[j] = dblSk[j] - dblSearchStep * iIdentityMatrix[i, j];
                        }

                        dblXik[0] = X + dblSik[0];
                        dblXik[1] = Y + dblSik[1];
                        dblXik[2] = Z + dblSik[2];

                        if (CheckPass(dblXik) == false)
                        {
                            dblValue = ObjectiveFunc(dblXik[0], dblXik[1], dblXik[2], false, measureType, crossValidationType);
                            iValidationCounter++;////////////
                        }
                        else
                        {
                            iTrueCounter++;
                            dblValue = CvResult + 1;    //Nhằm bỏ qua câu lệnh  if (dblValue < CvResult)
                        }

                        if (dblValue < CvResult)
                        {
                            CvResult = dblValue;

                            dblSk[0] = dblSik[0];
                            dblSk[1] = dblSik[1];
                            dblSk[2] = dblSik[2];

                            dblMinPoint[0] = dblXik[0];
                            dblMinPoint[1] = dblXik[1];
                            dblMinPoint[2] = dblXik[2];
                        }
                    }
                }

                if (X == dblMinPoint[0] && Y == dblMinPoint[1] && Z == dblMinPoint[2])
                {
                    dblSearchStep /= 2;
                }
                else
                {
                    X = dblMinPoint[0];
                    Y = dblMinPoint[1];
                    Z = dblMinPoint[2];
                }
                iLoopCounter++;
                #region Ghi log
                if (writer != null)
                {
                    writer.WriteLine(iLoopCounter.ToString() + " " + CvResult.ToString());
                }
                #endregion
            }
            #region Ghi log
            if (writer != null)
            {
                writer.WriteLine(iValidationCounter.ToString());
                writer.WriteLine(iTrueCounter.ToString());
                writer.WriteLine("Final: X = " + X.ToString() + " Y = " + Y.ToString() + " Z = " + Z.ToString());
                writer.Close();
            }
            #endregion
        }
        public double ObjectiveFunc(double x, double y, double z, bool isGridSearch, int measureType, int crossValidationType)   //Hàm mục tiêu. Dùng 10 folds cross validation với độ lỗi MSE
        {
            if (isGridSearch)
            {
                Trainer.Param.C = x;
                Trainer.Param.gamma = y;
                Trainer.Param.p = z;  //p: epsilon
            }
            else
            {
                Trainer.Param.C = Math.Pow(2, x);
                Trainer.Param.gamma = Math.Pow(2, y);
                Trainer.Param.p = Math.Pow(2, z);  //p: epsilon
            }

            return Trainer.do_cross_validation(measureType, crossValidationType);
        }
        public void GridSearch(string logPath, int measureType, int crossValidationType)
        {
            StreamWriter writer = null;
            if (Log)
            {
                writer = new StreamWriter(logPath);
            }
            MinErr = ObjectiveFunc(StartX, StartY, StartZ, true, measureType, crossValidationType);
            X = StartX;
            Y = StartY;
            Z = StartZ;
            if (writer != null)
            {
                writer.WriteLine("First val for err: " + MinErr.ToString());
                writer.WriteLine(" C: " + StartX.ToString());
                writer.WriteLine(" Gamma: " + StartY.ToString());
                writer.WriteLine(" Epsilon: " + StartZ.ToString());
            }
            // đánh dấu đã cross
            ArrX.Add(StartX);
            ArrY.Add(StartY);
            ArrZ.Add(StartZ);
            DoSearch(measureType, crossValidationType);
            if (writer != null)
            {
                writer.WriteLine("After loose search, val for err: " + MinErr.ToString());
                writer.WriteLine(" C: " + X.ToString());
                writer.WriteLine(" Gamma: " + Y.ToString());
                writer.WriteLine(" Epsilon: " + Z.ToString());
            }
            ChangeParam(0.25);
            DoSearch(measureType, crossValidationType);
            if (writer != null)
            {
                writer.WriteLine("After fine search, val for err: " + MinErr.ToString());
                writer.WriteLine(" C: " + X.ToString());
                writer.WriteLine(" Gamma: " + Y.ToString());
                writer.WriteLine(" Epsilon: " + Z.ToString());
                writer.Close();
            }

        }
        // Dùng để thu hẹp miền giá trị sau khi đã search thưa
        private void ChangeParam(double incPow)
        {
            Increment = Math.Pow(2, incPow);
            double dblBias = Math.Pow(2, (double)(Interval / 2) * incPow);
            StartX = X / dblBias;
            StartY = Y / dblBias;
            StartZ = Z / dblBias;
        }
        private void DoSearch(int measureType, int crossValidationType)
        {
            double dblCurX = StartX;
            double dblCurY = StartY;
            double dblCurZ = StartZ;

            for (int i = 0; i < Interval; i++)
            {
                dblCurY = StartY;
                for (int j = 0; j < Interval; j++)
                {
                    dblCurZ = StartZ;
                    for (int k = 0; k < Interval; k++)
                    {
                        
                        if (!IsPointUsed(dblCurX, dblCurY, dblCurZ))
                        {
                            double dblErr = ObjectiveFunc(dblCurX, dblCurY, dblCurZ, true, measureType, crossValidationType);
                            if (dblErr < MinErr)
                            {
                                MinErr = dblErr;
                                X = dblCurX;
                                Y = dblCurY;
                                Z = dblCurZ;
                            }
                            ArrX.Add(dblCurX);
                        }
                        dblCurZ = dblCurZ * Increment;
                    }
                    ArrY.Add(dblCurY);
                    dblCurY = dblCurY * Increment;
                }
                ArrZ.Add(dblCurZ);
                dblCurX = dblCurX * Increment;
            }
        }
        public bool IsPointUsed(double x, double y, double z)
        {
            for (int i = 0; i < ArrZ.Count; i++)
            {
                if (z.Equals((double)ArrZ[i]))
                {
                    for (int j = 0; j < ArrY.Count; j++)
                    {
                        if (y.Equals((double)ArrY[j]))
                        {
                            for (int k = 0; k < ArrX.Count; k++)
                            {
                                if (x.Equals(ArrX[k]))
                                {
                                    return true;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            return false;
        }
        public void Run(string[] args, string logPath, bool isGridSearch, int measureType, int crossValidationType)
        {
            Trainer.parse_command_line(args);
            Trainer.read_problem();
            if (isGridSearch)
            {
                GridSearch(logPath, measureType, crossValidationType);
            }
            else
            {
                PatternSearch(logPath, measureType, crossValidationType);
            }
        }
        #endregion
        
    }
}
