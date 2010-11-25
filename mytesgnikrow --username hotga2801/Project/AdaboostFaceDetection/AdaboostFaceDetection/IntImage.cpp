#include "stdafx.h"
#include "IntImage.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IntImage::IntImage():m_iHeight(0),m_iWidth(0),m_Data(NULL),m_Buf(NULL),m_rVariance(0.0),m_iLabel(-1)
{
}

IntImage::~IntImage()
{
	Clear();
}

void IntImage::Clear(void)
{
	if(m_Data == NULL)
		ASSERT(m_Buf == NULL);
	else
	{
		ASSERT(m_Buf != NULL);
		for(int i=0;i<m_iHeight;i++)	m_Data[i] = NULL;
		delete[] m_Data;	m_Data = NULL;
		delete[] m_Buf;  	m_Buf = NULL;
		m_iHeight = m_iWidth = 0;
		m_rVariance = 0.0;
		m_iLabel = -1;
	}
}

void IntImage::Copy(const IntImage& source)
// the ONLY way to make a copy of 'source' to this image
{
	ASSERT(source.m_iHeight > 0);
	ASSERT(source.m_iWidth > 0);
	if(&source == this)	return;
	SetSize(CSize(source.m_iHeight,source.m_iWidth));
	memcpy(m_Buf,source.m_Buf,sizeof(REAL)*m_iHeight*m_iWidth);
}

void IntImage::Load(const char* filename)
{
	IplImage* img;

	img = cvLoadImage(filename, 0);
	gOrgImg = cvLoadImage(filename);

	cvWaitKey(0);
	cvDestroyWindow("Detected");
	SetSize(CSize(img->height,img->width));
	for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
	{
		REAL* pdata = m_Data[i];
		unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData + img->widthStep*i);
		for(int j=0;j<iw;j++) 
			pdata[j] = pimg[j];
	}
	cvReleaseImage(&img);
}

void IntImage::Save(const char* filename) const
{
	IplImage* img;

	img = cvCreateImage(cvSize(m_iWidth,m_iHeight),IPL_DEPTH_8U,1);
	for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
	{
		REAL* pdata = m_Data[i];
		unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData+img->widthStep*i);
		for(int j=0;j<iw;j++) pimg[j] = (unsigned char)pdata[j];
	}
	cvSaveImage(filename,img);
	cvReleaseImage(&img);	
}

void IntImage::SetSize(const CSize size)
// 'size' is the new size of the image, if necessary, memory is reallocated
// size.cx is the new height and size.cy is the new width
{
	if((size.cx == m_iHeight) && (size.cy == m_iWidth) && (m_Buf != NULL) && (m_Data != NULL) ) return; 
	ASSERT(size.cx >= 0); ASSERT(size.cy >= 0);
	Clear();

	m_iHeight = size.cx;	
	m_iWidth = size.cy;

	m_Buf = new REAL[m_iHeight*m_iWidth]; ASSERT(m_Buf != NULL);
	m_Data = new REAL*[m_iHeight];	ASSERT(m_Data != NULL);
	for(int i=0;i<m_iHeight;i++)	m_Data[i] = &m_Buf[i*m_iWidth];
}

IntImage& IntImage::operator=(const IntImage& source)
{
	SetSize(CSize(source.m_iHeight,source.m_iWidth));
	memcpy(m_Buf,source.m_Buf,sizeof(*m_Buf)*m_iHeight*m_iWidth);
	m_iLabel = source.m_iLabel;
	m_rVariance = source.m_rVariance;

	return *this;
}

void IntImage::Resize(IntImage &result, REAL ratio) const
{
	result.SetSize(CSize(int(m_iHeight*ratio),int(m_iWidth*ratio)));
	ratio = 1/ratio;
	for(int i=0,rh=result.m_iHeight,rw=result.m_iWidth;i<rh;i++)
		for(int j=0;j<rw;j++) {
			int x0,y0;
			REAL x,y,fx0,fx1;
			x = j*ratio; y = i*ratio;
			x0 = (int)(x);
			y0 = (int)(y);

			//by Jianxin Wu  
			//1. The conversion of float to int in C is towards to 0 point, i.e. the floor function for positive numbers, and ceiling function for negative numbers.
			//2. We only make use of ratio<1 in this applicaiton, and all numbers involved are positive.
			//Using these, we have 0<=x<=height-1 and 0<=y<=width-1. Thus, boundary conditions check is not necessary.
			//In languages other than C/C++ or ratio>=1, take care. 
			if (x0 == m_iWidth-1) x0--;
			if (y0 == m_iHeight-1) y0--;

			x = x - x0; y = y - y0;

			fx0 = m_Data[y0][x0] + x*(m_Data[y0][x0+1]-m_Data[y0][x0]);
			fx1 = m_Data[y0+1][x0] + x*(m_Data[y0+1][x0+1]-m_Data[y0+1][x0]);

			result.m_Data[i][j] = fx0 + y*(fx1-fx0);
		}
}

void IntImage::CalculateVarianceAndIntegralImageInPlace(void)
// We pad a zero column and a zero row, so 24*24 image will be 25*25 in size
{
	REAL partialsum;
	REAL ex,ex2;
	REAL size,var;

	ex = 0;
	ex2 = 0;
	for(int i=0,size=m_iHeight*m_iWidth;i<size;i++)
	{
		ex += m_Buf[i];
		ex2 += (m_Buf[i]*m_Buf[i]);
	}
	size = REAL(m_iHeight*m_iWidth);
	var = ex2/size - (ex/size)*(ex/size);
	ASSERT(var >= 0);
	if(var>0)
		m_rVariance = sqrt(var);
	else
		m_rVariance = 1.0;


	for(int i=1;i<m_iWidth;i++)
	{
		m_Data[0][i] = m_Data[0][i-1] + m_Data[0][i];
	}
	for(int i=1;i<m_iHeight;i++)
	{
		partialsum = 0;
		for(int j=0;j<m_iWidth;j++)
		{
			partialsum += m_Data[i][j];
			m_Data[i][j] = m_Data[i-1][j] + partialsum;
		}
	}
}

void IntImage::CalcSquareAndIntegral(IntImage& square, IntImage& image) const
{
	REAL partialsum,partialsum2;

	square.SetSize(CSize(m_iHeight+1,m_iWidth+1));
	image.SetSize(CSize(m_iHeight+1,m_iWidth+1));

	for(int i=0;i<=m_iWidth+1;i++) square.m_Buf[i]=image.m_Buf[i]=0;
	for(int i=1;i<=m_iHeight;i++)
	{
		partialsum = partialsum2 = 0;
		square.m_Data[i][0] = 0;
		image.m_Data[i][0] = 0;
		for(int j=1;j<=m_iWidth;j++)
		{
			partialsum += (m_Data[i-1][j-1]*m_Data[i-1][j-1]);
			partialsum2 += m_Data[i-1][j-1];
			square.m_Data[i][j] = square.m_Data[i-1][j] + partialsum;
			image.m_Data[i][j] = image.m_Data[i-1][j] + partialsum2;
		}
	}
}

void SwapIntImage(IntImage& i1,IntImage& i2)
{
	std::swap(i1.m_iHeight,i2.m_iHeight);
	std::swap(i1.m_iWidth,i2.m_iWidth);
	std::swap(i1.m_Buf,i2.m_Buf);
	std::swap(i1.m_Data,i2.m_Data);
	std::swap(i1.m_rVariance,i2.m_rVariance);
	std::swap(i1.m_iLabel,i2.m_iLabel);
}