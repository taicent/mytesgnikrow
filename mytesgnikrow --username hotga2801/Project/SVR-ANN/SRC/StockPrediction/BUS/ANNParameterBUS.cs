using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace BUS
{
    public class ANNParameterBUS
    {
        #region Attributes
        private static int _inputNode;
        private static int _hiddenNode;
        private static int _outputNode;

        private static double _learningRate;
        private static double _momentum;
        private static int _maxEpoch;
        private static double _bias;
        
        private static int _trainingSize;

        private static double _accuracy;
        private static string _measureType;

        #endregion

        #region Properties
        public static int InputNode
        {
            get { return ANNParameterBUS._inputNode; }
            set { ANNParameterBUS._inputNode = value; }
        }

        public static int HiddenNode
        {
            get { return ANNParameterBUS._hiddenNode; }
            set { ANNParameterBUS._hiddenNode = value; }
        }

        public static int OutputNode
        {
            get { return ANNParameterBUS._outputNode; }
            set { ANNParameterBUS._outputNode = value; }
        }

        public static double LearningRate
        {
            get { return ANNParameterBUS._learningRate; }
            set { ANNParameterBUS._learningRate = value; }
        }

        public static double Momentum
        {
            get { return ANNParameterBUS._momentum; }
            set { ANNParameterBUS._momentum = value; }
        }

        public static int MaxEpoch
        {
            get { return ANNParameterBUS._maxEpoch; }
            set { ANNParameterBUS._maxEpoch = value; }
        }

        public static double Bias
        {
            get { return ANNParameterBUS._bias; }
            set { ANNParameterBUS._bias = value; }
        }

        public static int TrainingSize
        {
            get { return ANNParameterBUS._trainingSize; }
            set { ANNParameterBUS._trainingSize = value; }
        }

        public static double Accuracy
        {
            get { return ANNParameterBUS._accuracy; }
            set { ANNParameterBUS._accuracy = value; }
        }

        public static string MeasureType
        {
            get { return ANNParameterBUS._measureType; }
            set { ANNParameterBUS._measureType = value; }
        }
        #endregion

        #region Methods
        public static void LoadParameter()
        {
            try
            {
                TextReader reader = new StreamReader(ANNModelBUS.AnnModelFile);

                ANNParameterBUS.InputNode = int.Parse(reader.ReadLine());
                ANNParameterBUS.HiddenNode = int.Parse(reader.ReadLine());

                reader.Close();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        #endregion
    }
}
