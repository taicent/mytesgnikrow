/*
 * Vision.NET Computer Vision Library
 * Copyright (C) 2007 Matthew JOhnson
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
using System;
using System.Collections.Generic;
using System.Text;

namespace SVMDemo
{
    /// <summary>
    /// Class representing the second derivative of a Gaussian distribution in one dimension.
    /// </summary>
    public class GaussianSecondDerivative
    {
        private double _mean;
        private double _variance;
        private double _stddev;

        private double _a;
        private double _b;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="mean">Desired mean</param>
        /// <param name="stddev">Desired standard deviation of the mean</param>
        public GaussianSecondDerivative(float mean, float stddev)
        {
            _mean = mean;
            _stddev = stddev;
            _variance = stddev * stddev;

            _a = 1 / (_variance * _variance * Math.Sqrt(2 * Math.PI * _variance));
            _b = -1 / (2 * _variance);
        }

        /// <summary>
        /// Computes the value of the function at the position indicated.
        /// </summary>
        /// <param name="x">The position to evaluate the function at</param>
        /// <returns>The value of the function at x</returns>
        public float Compute(float x)
        {
            double dx = x - _mean;
            return (float)((dx*dx - _variance) * _a * Math.Exp(dx * dx * _b));
        }
    }

    /// <summary>
    /// Class representing the first derivative of a Gaussian distribution in one dimension.
    /// </summary>
    public class GaussianFirstDerivative
    {
        private double _mean;
        private double _variance;
        private double _stddev;

        private double _a;
        private double _b;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="mean">Desired mean</param>
        /// <param name="stddev">Desired standard deviation of the mean</param>
        public GaussianFirstDerivative(float mean, float stddev)
        {
            _mean = mean;
            _variance = stddev * stddev;
            _stddev = stddev;

            _a = -1 / (_variance*Math.Sqrt(2 * Math.PI * _variance));
            _b = -1/(2 * _variance);
        }

        /// <summary>
        /// Computes the value of this function at the position indicated.
        /// </summary>
        /// <param name="x">The position to evalute at</param>
        /// <returns>The value of the function at x</returns>
        public float Compute(float x)
        {
            double dx = x - _mean;
            return (float)(dx*_a*Math.Exp(dx*dx*_b));
        }
    }

    /// <summary>
    /// Class representing a Gaussian (or Normal) distribution in one dimension.
    /// </summary>
    public class Gaussian
    {
        private double _mean;
        private double _variance;
        private double _stddev;
        private Random _rand = new Random();

        private double _a;
        private double _b;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="mean">Desired mean of the Gaussian</param>
        /// <param name="stdDev">Desired standard deviation of the mean.</param>
        public Gaussian(float mean, float stdDev)
        {
            _mean = mean;
            _variance = stdDev*stdDev;;
            _stddev = stdDev;

            _a = 1 / (Math.Sqrt(2 * Math.PI) * _stddev);
            _b = -1/(2 * _variance);
        }

        /// <summary>
        /// Randomly samples the Gaussian distribution.
        /// </summary>
        /// <returns>A sample from the Gaussian distribution</returns>
        public float Sample()
        {
            double x1, x2, w, y1, y2;

            do
            {
                x1 = 2.0 * _rand.NextDouble() - 1.0;
                x2 = 2.0 * _rand.NextDouble() - 1.0;
                w = x1 * x1 + x2 * x2;
            } while (w >= 1.0);

            w = Math.Sqrt((-2.0 * Math.Log(w)) / w);
            y1 = x1 * w;
            y2 = x2 * w;

            return (float)(y1 * _stddev + _mean);
        }
        
        /// <summary>
        /// Evaluates the Gaussian at x.
        /// </summary>
        /// <param name="x">The value to evaluate at</param>
        /// <returns>The Gaussian evaluated at x</returns>
        public float Compute(float x)
        {
            double dx = x - _mean;
            return (float)(_a * Math.Exp(_b * dx * dx));
        }

        /// <summary>
        /// Computes a Gaussian kernel, takes the form {center value, value 1 pixel from center, value 2 pixels from center, etc.}.
        /// </summary>
        /// <param name="sigma">The standard deviation of the Gaussian</param>
        /// <returns>The kernel</returns>
        public static float[] ComputeKernel(float sigma)
        {
            int size = (int)Math.Ceiling(sigma * 3);
            float[] kernel = new float[size];
            float sum = 0;
            float half_inverse_squared = -.5f / (sigma * sigma);
            for (int i = 0; i < kernel.Length; i++)
            {
                kernel[i] = (float)Math.Exp(i * i * half_inverse_squared);
                sum += kernel[i];
                if (i > 0)
                    sum += kernel[i];
            }
            float[] result = new float[size];
            for (int i = 0; i < kernel.Length; i++)
                result[i] = kernel[i] / sum;
            return result;
        }
    }
}
