/*
	Author : Binh Nguyen - Bioz
	Last Update : 21 Oct 2008	 
	License : Free
	Official site : www.sectic.com
	Description : detect face by using haar model supplied by OpenCV lib. 
*/

#include <cv.h>

public class OpenCVFaceDetection
{
public:

	int init_cascade(const char *cascade_name);
	CvSeq* detect_face(IplImage* image, double scale, double &time);
	~OpenCVFaceDetection();
	OpenCVFaceDetection();
};