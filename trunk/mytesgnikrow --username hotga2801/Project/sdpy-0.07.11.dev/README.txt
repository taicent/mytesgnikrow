sdpy - SharkDolphin's Computer Science modules for Python
Copyright (C) 2007  Minh-Tri Pham and Viet-Dung D. Hoang
Copyright (C) 2007  Nanyang Technological University
============================================================

1. FEATURES
The main features of sdpy are:
 - Interface:
   - A Python interface to OpenCV (by Gary Bishop)
 - Computer Science:
   - Computer Geometry:
     - Smallest enclosing ball -- to be used later in CVM (Tsang et. al. JMLR'05)
   - Computer Vision:
     - Haar-like feature and its generalized Sparse Rectangular feature (by me)
     - Integral image
     - Cascade for Object detection
     - Face detection
     - Fast training of face detection (Pham-Cham ICCV'07)
   - Linear Algebra:
     - Symmetric eigen values and eigen vectors of A' * A
   - Machine Learning:
     - Classification:
       - Boosting:
         - Offline:
           - AdaBoost
           - Viola-Jones' AdaBoost (Viola-Jones NIPS'02)
           - AdaBoost with Skewness balancing (Pham-Cham CVPR'07)
         - Online:
           - Online Boosting (Oza-Russel '05)
           - Asymmetric Online Boosting (Pham-Cham CVPR'07)
       - Linear Discriminant Analysis (LDA)
       - Naive Bayes classifier
       - Stump classifiers with/without univariate Gaussian assumptions
     - Regression:
       - Linear regression
   - Optimization:
     - Differential Evolution (by Robert Kent)
   - Statistics:
     - Moments/statistics for univariates and multivariates

2. DOWNLOAD
The most current development version is always available from:

http://pmtri.sharkdolphin.com

3. INSTALLATION
sdpy requires:
 - A standard C++ compiler, e.g. msvc, icc, or gcc
 - OpenCV
 - SciPy, implying Python and NumPy is installed as well
 - cpuidpy, a small and simple Python package to detect CPU's capabilities

sdpy can optionally utilize:
 - BLAS or ATLAS installed along with NumPy + SciPy

To install:

python setup.py install

4. TESTING
After installation, tests can be run with

python -c 'import sdpy; sdpy.test()'

5. CONTACT ME
Send me an email at mtpham@ntu.edu.sg
