//Copyright (C) 2007 Matthew Johnson

//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using SVM;

namespace SVMDemo
{
    public enum PointDensity { Single, Sparse, Dense };
    public partial class SVMCanvas : UserControl
    {
        private static readonly Color[] CLASSES = new Color[] { 
            Color.FromArgb(255, 0, 0), 
            Color.FromArgb(0, 255, 0),
            Color.FromArgb(0, 0, 255), 
            Color.FromArgb(0, 255, 255), 
            Color.FromArgb(255, 0, 255), 
            Color.FromArgb(255, 255, 0) 
        };
        private static readonly Color[] CLASS_FILL = new Color[] { 
            Color.FromArgb(127, 0, 0), 
            Color.FromArgb(0, 127, 0),
            Color.FromArgb(0, 0, 127), 
            Color.FromArgb(0, 127, 127), 
            Color.FromArgb(127, 0, 127), 
            Color.FromArgb(127, 127, 0) 
        }; 
        private const int CIRCLE_DIAMETER = 16;

        private PointDensity _density;
        private Bitmap _canvas;
        private List<Node[]> _X;
        private List<double> _Y;
        private Gaussian _sparse, _dense;
        private int _class;
        private Bitmap[] _bullets;

        public SVMCanvas()
        {
            InitializeComponent();

            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.OptimizedDoubleBuffer, true);

            _X = new List<Node[]>();
            _Y = new List<double>();
            _sparse = new Gaussian(0, 20);
            _dense = new Gaussian(0, 5);
            _bullets = new Bitmap[imageList1.Images.Count];
            for (int i = 0; i < imageList1.Images.Count; i++)
                _bullets[i] = new Bitmap(imageList1.Images[i]);
        }

        public void Reset()
        {
            _X.Clear();
            _Y.Clear();
        }

        public void PerformRecognition()
        {
            backgroundWorker.RunWorkerAsync();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            g.FillRectangle(Brushes.White, 0, 0, Width, Height);

            if (_canvas != null)
            {
                Bitmap image;
                lock (this)
                {
                    image = new Bitmap(_canvas);
                }

                g.DrawImage(image, (Width - image.Width) / 2, (Height - image.Height) / 2, image.Width, image.Height);
            }

            for (int i = 0; i < _X.Count; i++)
            {
                Node[] x = _X[i];
                double y = _Y[i];
                int tlx = (int)x[0].Value + Width / 2 - CIRCLE_DIAMETER / 2;
                int tly = (int)x[1].Value + Height / 2 - CIRCLE_DIAMETER / 2;
                g.DrawImage(_bullets[(int)y], tlx, tly, CIRCLE_DIAMETER, CIRCLE_DIAMETER);
            }
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            switch (_density)
            {
                case PointDensity.Single:
                    AddPoint(e.X, e.Y);
                    break;

                case PointDensity.Sparse:
                    for (int i = 0; i < 20; i++)
                    {
                        float x = e.X + _sparse.Sample();
                        float y = e.Y + _sparse.Sample();
                        AddPoint(x, y);
                    }
                    break;

                case PointDensity.Dense:
                    for (int i = 0; i < 20; i++)
                    {
                        float x = e.X + _dense.Sample();
                        float y = e.Y + _dense.Sample();
                        AddPoint(x, y);
                    }
                    break;
            }
            Refresh();
        }

        private void AddPoint(float x, float y)
        {
            _X.Add(new Node[] { new Node(1, x - Width / 2), new Node(2, y - Height / 2) });
            _Y.Add(_class);
            lock (this)
            {
                _canvas = null;
            }
        }

        public static int ClassCount
        {
            get
            {
                return CLASSES.Length;
            }
        }

        public int Class
        {
            get
            {
                return _class;
            }
            set
            {
                _class = value;
            }
        }

        private void backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            Problem problem = new Problem(_X.Count, _Y.ToArray(), _X.ToArray(), 2);
            RangeTransform range = RangeTransform.Compute(problem);
            problem = range.Scale(problem);

            Parameter param = new Parameter();
            param.C = 2;
            param.Gamma = .5;
            Model model = Training.Train(problem, param);
            
            int rows = ClientSize.Height;
            int columns = ClientSize.Width;
            Bitmap image = new Bitmap(columns, rows);
            int centerR = rows / 2;
            int centerC = columns / 2;
            BitmapData buf = image.LockBits(new Rectangle(0, 0, columns, rows), ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);
            unsafe
            {
                byte* ptr = (byte*)buf.Scan0;
                int stride = buf.Stride;

                for (int r = 0; r < rows; r++)
                {
                    byte* scan = ptr;
                    for (int c = 0; c < columns; c++)
                    {
                        int x = c - centerC;
                        int y = r - centerR;
                        Node[] test = new Node[] { new Node(1, x), new Node(2, y) };
                        test = range.Transform(test);
                        int assignment = (int)Prediction.Predict(model, test);
                        *scan++ = CLASS_FILL[assignment].B;
                        *scan++ = CLASS_FILL[assignment].G;
                        *scan++ = CLASS_FILL[assignment].R;
                    }
                    ptr += stride;
                }
            }
            image.UnlockBits(buf);
            lock (this)
            {
                _canvas = new Bitmap(image);
            }
        }

        public PointDensity Density
        {
            get
            {
                return _density;
            }
            set
            {
                _density = value;
            }
        }

        protected override void OnResize(EventArgs e)
        {
            Refresh();
        }

        private void backgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            Refresh();
        }
    }
}
