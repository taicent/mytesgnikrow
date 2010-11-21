This package contains the version 1.3 of the frontal face detector code.
Main changes:
1. The floating point now can be in 'double' type of 'float' type. The default version uses 'float'. If you want to use
'double' instead, uncomment the first line in IntImage.h file.
2. The bilinear resize function in 1.21 is wrong (IntImage::Resize function). It is corrected now.
3. The CascadeClassifier::ApplyOriginalSize function is revised to make the testing speed faster. The GetOneFeatureTranspose
function (now renamed to GetOneFeatureTranslation) is now directly unwrapped into ApplyOriginalSize.
4. One data file cascade.txt.range is used. This file contains the min/max value of an image patch, etc. These values are
used to accelerate the testing process.
5. Two trained cascade classifiers are attached. One is trained fully automatically using this code and AdaBoost, stored in
data/cascade_Ada.txt. Another cascade is much faster, stored in cascade_faster.txt. The first node of it is manually 
designed with only 1 feature. All other nodes are trained fully automatically using this code and FFS algorithm.

[Readme from version 1.21]
This package contains the version 1.21 of the frontal face detector code. 

Most of the codes remain the same as version 1.2. The main difference is a user interface (the option dialog) that lets users to specify key parameters of the algorithm using a graphical user interface. 

For details of this package, please read the 'Readme' for version 1.2 (below). Here we summary how to compile and run this package.

Requirements:
  1. About 300 MB hard disk space in C:\
  2. At least 750 MB main memory.

How to compile this package
  1. The source code is written with Microsoft Visual C++ .net. Please open the FaceDetect.sln to open the project.
  2. This package requires the OpenCV 4.0 library. Please install OpenCV AND add OpenCV include/library directories to Visual C++'s environment and the system path.
  3. Extract the source code to any directory you want.
  4. You are ready to compile/link this project.

Preparations before you do any training
  1. Extract the zip file that contains non-faces. Please use the directory from the package. It should be extracted as C:\WuJX\SkinColor\non-skin-images\...
  2. Make sure the data/cascade.txt file contains only 1 character '0', if you want to train a new cascade.
  3. Make sure to copy datasetMike.dat (the original training set) in the source code directory into the 'data' directory.
  4. Remember to do step 2 and 3 whenever you want to train a cascade.

Run the code
  1. from the IDE. Just click the execute button of the IDE.
  2. outside the IDE. You need to copy the executable file to the source code directory. (For example, from 'Release' directory up one level).

Train
  1. Run the code. Choose the 'Train a cascade' sub-menu from the 'Train' menu. The option dialog will appear.
  2. Choose your feature selection method (AdaBoost, AsymBoost or FFS)
  3. Choose your linear classifier (original, LAC, or FDA)
  4. If you want to use AsymBoost, after choosing AdaBoost in step 2, specify the asymmetry level. The value 1.0268 is recommended for AsymBoost. If you leave the asymmetry level to 1 (the default value), it is AdaBoost.
  5. If you have manually stopped the training process before, you have the choice to resume from where you stopped. In this case, the 'starting training from which node?' box will show where you were stopped before. This vale is read from the data/cascade.txt. Note that in this case, you do not need to do step 2 and 3 in 'Preparations before you do any training'.
  6. If the number in 'Starting training from which node?' is bigger than 0, but you do NOT want to resume -- just want to train a new cascade, in this case you need to click the 'Cancel' button, do the step 2 and 3 in 'Preparations before you do any training', and then train again.

Test
   1. To test a single image (.jpg, .gif, .bmp support), click 'File'->'Open' to choose the image you want to test. The detector will run automatically, and after it finishes, the detection result will show in the main window.
   2. If the image is big, the detection could be slow. If the image is too big to show in the main window, a saved result image could be viewed. For example, if X is the filename (including path name) of input image, a file named X_result.jpg will contain the detection result.

Updates: 
   1. A graphical user interface for specifying key parameters is added,
   2. A bug is bootstrapping non-face data is fixed.

[Reame from verions 1.2]

This package contains the version 1.2 of the frontal face detector code, developped by Jianxin Wu (http://www.cc.gatech.edu/~wujx) at the GVU center, Georgia Institute of Technology, with the help of S. Charles Brubaker, Matthew D. Mullin, and James M. Rehg.

This package contains the source code for the entire project. You are free to use it for academic/research or personal purposes. If you use this package in your project, please cite the following reference: 
   Jianxin Wu, James M. Rehg, Matthew D. Mullin. Learning a Rare Event Detection Cascade by Direct Feature Selection, NIPS 16.
If you find a bug, make some improvments, use/cite the code/paper, or having any problem in using it, please feel free to ask me at wujx@cc.gatech.edu.

This is a Microsoft Visual C++ 6 project. It uses the OpenCV 4.0 library. You need to install MSVC and OpenCV 4.0 (NOT previous versions of OpenCV) before you can build and run the project.

The idea behind this package is described in the above paper. We provide 2 ways to train a cascade: using AdaBoost/AsymBoost by Viola and Jones, and the Forward Feature Selection described in the above paper. The code also some fragments of a new algorithms we developed recently, but haven't been published yet.

Besides the source code, I also put a demo executable which is live demo of frontal face detector. This demo requires a USB camera (the OpenCV library has some problems with firewire cameras). 

The rest of this readme file will briefly describe how to use the source code package.

First make sure you have MSVC installed, then install the OpenCV 4.0 library. Make necessary changes to the VC directory options so that OpenCV include files and libraries are accessible from VC. Besides the CV include files, remember to add the cxcore and highgui include file and library file directory.

The main things you need to change in order to run this project is the options.txt file. This file contains all the relavent options to be set. The file itself contains descriptions of every option. It is in the fillowing format
Description of option 1
value of option 1
Description of option 2
Value of option 2
...

Here are some more explanantions for a few important options:
trainset_filename: This is the path/name of the file that contains the training examples. The face training set we use is property of MERL, and we are NOT able to release it. I made a small training set from the car 2001 data set of Caltech vision group (http://www.vision.caltech.edu/html-files/archive.html). You may use this data set to try our code. If you want to build your own training set, please change this option to the correct path/name. The sample car data is in the file datasetMike.dat.

validset_filename: Validation set, mainly used to determine threshold of every node. It is NOT necessary if the goal_method option is not 2. You can just put the training set name here if a validation set is not needed.

classifier_filename: Path/name of the file that contain all rectangle features (for more on rectangle features, please refer to the paper above.)

cascade_filename: The current cascade file. Before you start the trianing process, it should contain only one character '0'. It will be updated by the training process.

Bootstrap_database_filename: A file that contain a list of filenames which do not contain faces. Currently I put a file '0_FileList.txt" in this package to let you know the format of this file. I do not put the bootstrap image files, because the database contains about 8k images, and are too big to be put in my homepage. You may collect your own images.

Backup_directory_name: a directory to store all intermediate information created during the training. You need to create this directory before starting trainging.

train_method: =1 for AdaBoost/AsymBoost, =2 for FFS

goal_method: methods to determine threshold in the ensemble. =1, use the default ensemble value, not suited for a cascade, =2, use the validation set to make the ensemble's detection rate='node_det_goal', =3 make the ensemble's false positve rate='node_fp_goal'

asym_ratio: =1, AdaBoost, >1 AsymBoost

nof: an array contains the nubmer of features in each node of the cascade. If max_nodes=50, then this array MUST contain 50 integers. This requirement is important, otherwise the program will crash.

Note that the implementation of AdaBoost/AsymBoost is very fast (based on the idea from S. Charles Brubaker), to train a face detection cascade took about 8 hours using AdaBoost, and 4 hours using FFS.

