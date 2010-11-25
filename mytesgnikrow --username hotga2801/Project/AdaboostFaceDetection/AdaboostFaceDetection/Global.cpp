#include "stdafx.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <float.h>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "Label.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString gSetup_Filename = _T("C:\\options.txt");

int gTotal_fp;

CString gTrainset_Filename;
CString gValidationset_Filename;
CString gClassifier_Filename;
CString gAda_Log_Filename;
CString gCascade_Filename;

CString gBootstrap_Database_Filename;
CString gBackup_Directory_Name;
CString gTestSet_Filename;
CString gTestSet_Label;
int gSx; //height
int gSy; //width
int gTrain_Method;
int linear_classifier;
int bootstrap_level;
int max_bootstrap_level;
vector<REAL> bootstrap_resizeratio;
vector<int> bootstrap_increment;
int gTotalFeatures;
int gMaxNumFiles;
REAL gNode_fp_Goal;

int gMaxNumNodes;
vector<int> gNof;
int gStartingNode;
int gFaceCount, gValidFaceCount;

IntImage* gTrainSet = NULL;
IntImage* gValidationSet = NULL;
int gTotalCount;
int gValidationCount;	

CascadeClassifier* gCascade = NULL;
REAL* gWeights= NULL;
int** gTable= NULL;
WeakClassifier* gClassifiers = NULL;

REAL* gFeatures=NULL;
int* gLabels=NULL;

int* gFileUsed = NULL;
int gBootstrapSize;
CString* gBootstrap_Filenames = NULL;

REAL gMean_Min, gMean_Max, gSq_Min, gSq_Max, gVar_Min, gVar_Max;

vector<CLabel> gFaceLabels;
int gTotalLabel;
int gNumRightLabel;
int gNumWrongDetectedFace;
IplImage* gOrgImg = NULL;
CString gHomePath;

void WriteRangeFile(void)
// images are already integral images.
{
	CString filename;
	ofstream f;

	filename = gHomePath + gCascade_Filename + _T(".range");
	f.open(filename);
	gMean_Min = REAL(gSx*gSy*256); gSq_Min = REAL(gSx*gSy*256*256); gVar_Min = 256;
	gMean_Max = 0; gSq_Max = 0; gVar_Max = -256;
	for(int i=0;i<gFaceCount;i++)
	{
		REAL ex, sq, v;

		ex = sq = 0;
		for(int j=1;j<gTrainSet[i].m_iHeight;j++) 
			for(int k=1;k<gTrainSet[i].m_iWidth;k++)
			{
				v = gTrainSet[i].m_Data[j][k]+gTrainSet[i].m_Data[j-1][k-1]-gTrainSet[i].m_Data[j-1][k]-gTrainSet[i].m_Data[j][k-1];
				sq += (v*v);
			}
		ex = gTrainSet[i].m_Data[gSx][gSy];
		gMean_Min = min(gMean_Min,ex);
		gMean_Max = max(gMean_Max,ex);
		gSq_Min = min(gSq_Min,sq);
		gSq_Max = max(gSq_Max,sq);
		gVar_Min = min(gVar_Min,gTrainSet[i].m_rVariance);
		gVar_Max = max(gVar_Max,gTrainSet[i].m_rVariance);
	}
	gMean_Min *= REAL(0.90);
	gMean_Max *= REAL(1.10);
	gSq_Min *= REAL(0.90);
	gSq_Max *= REAL(1.10);
	gVar_Min *= REAL(0.90);
	gVar_Max *= REAL(1.10);
	f<<gMean_Min<<endl<<gMean_Max<<endl<<gSq_Min<<endl<<gSq_Max<<endl<<gVar_Min<<endl<<gVar_Max<<endl;
	f.close();
}

void ReadRangeFile(void)
{
	CString filename;
	ifstream f;

	filename = gHomePath + gCascade_Filename + _T(".range");
	f.open(filename);
	f>>gMean_Min>>gMean_Max>>gSq_Min>>gSq_Max>>gVar_Min>>gVar_Max;
	f.close();
}

void ReadOneTrainingSample(ifstream& is,IntImage& image)
{
	int i,j;
	char buf[256];

	ASSERT(gSx<=256 && gSy<=256);
	is>>image.m_iLabel; is.ignore(256,'\n');
	ASSERT( (image.m_iLabel == 0) || (image.m_iLabel == 1) );

	is>>image.m_iHeight>>image.m_iWidth; is.ignore(256,'\n');
	ASSERT(image.m_iHeight==gSx); 
	ASSERT(image.m_iWidth==gSy);

	image.SetSize(CSize(image.m_iHeight+1,image.m_iWidth+1));
	for(i=0;i<image.m_iHeight;i++) image.m_Data[i][0] = 0;
	for(i=0;i<image.m_iWidth;i++) image.m_Data[0][i] = 0;
	for(i=1;i<image.m_iHeight;i++)
	{
		is.read(buf,image.m_iWidth-1);
		for(j=1;j<image.m_iWidth;j++) 
		{
			image.m_Data[i][j] = REAL(int(unsigned char(buf[j-1])));
			ASSERT(image.m_Data[i][j]>=0 && image.m_Data[i][j] <= 255);
		}
	}
	is.ignore(256,'\n');
}

void GetFeatureValues0(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y3)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].m_Data;
		f1 =   data[x1][y3] - data[x1][y1] + data[x3][y3] - data[x3][y1]
			 + 2*(data[x2][y1] - data[x2][y3]);
		features[i] = f1 / gTrainSet[i].m_rVariance;
	}
}

void GetFeatureValues1(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].m_Data;
		f1 =   data[x3][y1] + data[x3][y3] - data[x1][y1] - data[x1][y3]
			 + 2*(data[x1][y2] - data[x3][y2]);
		features[i] = f1 / gTrainSet[i].m_rVariance;
	}
}

void GetFeatureValues2(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int x4,const int y1,const int y3)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].m_Data;
		f1 =   data[x1][y1] -data[x1][y3] + data[x4][y3] - data[x4][y1]
			 + 3*(data[x2][y3] - data[x2][y1] + data[x3][y1] - data[x3][y3]);
		features[i] = f1 / gTrainSet[i].m_rVariance;
	}
}

void GetFeatureValues3(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3,const int y4)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].m_Data;
		f1 =   data[x1][y1] - data[x1][y4] + data[x3][y4] - data[x3][y1]
			 + 3*(data[x3][y2] - data[x3][y3] + data[x1][y3] - data[x1][y2] );
		features[i] = f1 / gTrainSet[i].m_rVariance;
	}
}

void GetFeatureValues4(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y2,const int y3)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].m_Data;
		f1 =   data[x1][y1] + data[x1][y3] + data[x3][y1] + data[x3][y3]
			 - 2*(data[x2][y1] + data[x2][y3] + data[x1][y2] + data[x3][y2])
			 + 4*data[x2][y2];
		features[i] = f1 / gTrainSet[i].m_rVariance;
	}
}

void QuickSort(REAL* values,int* labels,const int lo,const int hi)
{
    int i=lo, j=hi;
	REAL v; int l;
    REAL x = values[(lo+hi)/2];

    do
    {    
        while (values[i]<x) i++; 
        while (values[j]>x) j--;
        if (i<=j)
        {
            v=values[i]; values[i]=values[j]; values[j]=v;
			l=labels[i]; labels[i]=labels[j]; labels[j]=l;
            i++; j--;
        }
    } while (i<=j);

    if (lo<j) QuickSort(values,labels,lo, j);
    if (i<hi) QuickSort(values,labels,i, hi);
}

void FillTheTable(int* const row,const WeakClassifier& sc)
{
	int i;
	int x1,x2,x3,x4,y1,y2,y3,y4;
	
	x1 = sc.x1;	y1 = sc.y1;
	x2 = sc.x2;	y2 = sc.y2;
	x3 = sc.x3; y3 = sc.y3;
	x4 = sc.x4;	y4 = sc.y4;
	switch(sc.m_iType)
	{
		case 0:GetFeatureValues0(gFeatures,0,gTotalCount,x1,x2,x3,y1,y3);break;
		case 1:GetFeatureValues1(gFeatures,0,gTotalCount,x1,x3,y1,y2,y3);break;
		case 2:GetFeatureValues2(gFeatures,0,gTotalCount,x1,x2,x3,x4,y1,y3);break;
		case 3:GetFeatureValues3(gFeatures,0,gTotalCount,x1,x3,y1,y2,y3,y4);break;
		case 4:GetFeatureValues4(gFeatures,0,gTotalCount,x1,x2,x3,y1,y2,y3);break;
	}
	for(i=0;i<gTotalCount;i++) row[i] = i;
	QuickSort(gFeatures,row,0,gTotalCount-1);
}

const int CountTrainFaces()
{
	int i,count;

	count = 0;
	for(i=0;i<gTotalCount;i++)
		count += gTrainSet[i].m_iLabel;
	return count;
}

const int CountValidFaces()
{
	int i,count;

	count = 0;
	for(i=0;i<gValidationCount;i++)
		count += gValidationSet[i].m_iLabel;
	return count;
}

void InitializeWeights()
{
	int i;

	for(i=0;i<gFaceCount;i++) gWeights[i] = REAL(0.5)/gFaceCount;
	for(i=gFaceCount;i<gTotalCount;i++) gWeights[i] = REAL(0.5)/(gTotalCount-gFaceCount);
}

void ReadClassifiers()
{
	ifstream f;
	int i;

	f.open(gHomePath + gClassifier_Filename);
	for(i=0;i<gTotalFeatures;i++) gClassifiers[i].ReadFromFile(f);
	f.close();
}

void IgnoreComments(ifstream& f)
{
	f.ignore(65536,'\n');
}

void ReadlnString(ifstream& f, CString& s)
{
	char buf[256];
	f.getline(buf,255,'\n');
	s = buf;
}

void LoadOptions()
{
	ifstream f;
	int i;

	f.open(gSetup_Filename);

	IgnoreComments(f); ReadlnString(f,gTrainset_Filename);
	IgnoreComments(f); ReadlnString(f,gValidationset_Filename);
	IgnoreComments(f); ReadlnString(f,gClassifier_Filename);
	IgnoreComments(f); ReadlnString(f,gAda_Log_Filename);
	IgnoreComments(f); ReadlnString(f,gCascade_Filename);

	IgnoreComments(f); ReadlnString(f,gBootstrap_Database_Filename);
	gBackup_Directory_Name = _T("temp\\");
	IgnoreComments(f); ReadlnString(f,gTestSet_Filename);

	IgnoreComments(f); f>>gSx; IgnoreComments(f);
	IgnoreComments(f); f>>gSy; IgnoreComments(f);

	IgnoreComments(f); f>>bootstrap_level; IgnoreComments(f);
	IgnoreComments(f); f>>max_bootstrap_level; IgnoreComments(f);
	bootstrap_resizeratio.resize(max_bootstrap_level);
	IgnoreComments(f); for(i=0;i<max_bootstrap_level;i++) f>>bootstrap_resizeratio[i]; IgnoreComments(f);
	bootstrap_increment.resize(max_bootstrap_level);
	IgnoreComments(f); for(i=0;i<max_bootstrap_level;i++) f>>bootstrap_increment[i]; IgnoreComments(f);
	IgnoreComments(f); f>>gTotalFeatures; IgnoreComments(f);
	IgnoreComments(f); f>>gMaxNumFiles; IgnoreComments(f);
	IgnoreComments(f); f>>gNode_fp_Goal; IgnoreComments(f);
	IgnoreComments(f); f>>gMaxNumNodes; IgnoreComments(f);
	gNof.resize(gMaxNumNodes);
	IgnoreComments(f); for(i=0;i<gMaxNumNodes;i++) f>>gNof[i]; 	IgnoreComments(f);
	f.close();
}

void ReadTrainSet(CString filename)
{
	ifstream f;
	int i;

	f.open(filename, ios_base::binary | ios_base::in);
	f>>gTotalCount; f.ignore(256,'\n');
	ASSERT(gTotalCount > 0);
	delete[] gTrainSet; gTrainSet=NULL;
	gTrainSet = new IntImage[gTotalCount]; ASSERT(gTrainSet != NULL);
	for(i=0;i<gTotalCount;i++) ReadOneTrainingSample(f,gTrainSet[i]);
	for(i=0;i<gTotalCount;i++) gTrainSet[i].CalculateVarianceAndIntegralImageInPlace();
	f.close();
	gFaceCount = CountTrainFaces();
}

void ReadValidSet(CString filename)
{
	ifstream f;
	int i;

	f.open(filename, ios_base::binary | ios_base::in);
	f>>gValidationCount; f.ignore(256,'\n');
	ASSERT(gValidationCount > 0);
	delete[] gValidationSet; gValidationSet=NULL;
	gValidationSet = new IntImage[gValidationCount]; ASSERT(gValidationSet != NULL);
	for(i=0;i<gValidationCount;i++) ReadOneTrainingSample(f,gValidationSet[i]);
	for(i=0;i<gValidationCount;i++) gValidationSet[i].CalculateVarianceAndIntegralImageInPlace();
	f.close();
	gValidFaceCount = CountValidFaces();
}

void InitializeAuxiliaryVariables()
{
	int i;

	ASSERT(gTrainSet != NULL);
	ASSERT(gTotalCount > 0);

	delete[] gWeights; gWeights = NULL;
	gWeights = new REAL[__max(gTotalCount,gValidationCount)]; ASSERT(gWeights != NULL);
	InitializeWeights();

	if(gTable!=NULL)
	{
		for(i=0;i<gTotalFeatures;i++)
		{
			delete[] gTable[i]; gTable[i] = NULL;
		}
	}
	delete[] gTable; gTable = NULL;
	gTable = new int*[gTotalFeatures]; ASSERT(gTable != NULL);
	for(i=0;i<gTotalFeatures;i++)
	{
		gTable[i] = new int[gTotalCount]; 
		ASSERT(gTable[i] != NULL);
	}

	delete[] gFeatures; gFeatures = NULL;
	gFeatures = new REAL[__max(gTotalCount,gValidationCount)]; ASSERT(gFeatures != NULL);

	delete[] gLabels; gLabels = NULL;
	gLabels = new int[__max(gTotalCount,gValidationCount)]; ASSERT(gLabels!=NULL);
}

void SaveTrainSet(CString filename)
{
	ofstream f;
	int i;

	f.open(filename,ios_base::out | ios_base::binary);
	f<<gTotalCount<<endl;
	unsigned char writebuf[24*24];
	for(i=0;i<gTotalCount;i++)
	{
		int k,t;
		f<<gTrainSet[i].m_iLabel<<endl;
		f<<gSx<<" "<<gSy<<endl;
		for(k=0;k<gSx;k++)
			for(t=0;t<gSy;t++)
				writebuf[k*gSx+t] = (unsigned char)((int)(gTrainSet[i].m_Data[k+1][t+1]-gTrainSet[i].m_Data[k][t+1]-gTrainSet[i].m_Data[k+1][t]+gTrainSet[i].m_Data[k][t]));
		f.write((char*)writebuf,gSx*gSy);
		f<<endl;
	}
	f.close();
}

void SaveValidSet(CString filename)
{
	ofstream f;
	int i;

	f.open(filename,ios_base::out | ios_base::binary);
	f<<gValidationCount<<endl;
	unsigned char writebuf[24*24];
	for(i=0;i<gValidationCount;i++)
	{
		int k,t;
		f<<gValidationSet[i].m_iLabel<<endl;
		f<<gSx<<" "<<gSy<<endl;
		for(k=0;k<gSx;k++)
			for(t=0;t<gSy;t++)
				writebuf[k*gSx+t] = (unsigned char)((int)(gValidationSet[i].m_Data[k+1][t+1]-gValidationSet[i].m_Data[k][t+1]-gValidationSet[i].m_Data[k+1][t]+gValidationSet[i].m_Data[k][t]));
		f.write((char*)writebuf,gSx*gSy);
		f<<endl;
	}
	f.close();
}

void ReadBootstrapFileNames()
{
	ifstream f;
	int i;

	f.open(gHomePath + gBootstrap_Database_Filename);
	f>>gBootstrapSize; IgnoreComments(f);
	gBootstrap_Filenames = new CString[gBootstrapSize]; ASSERT(gBootstrap_Filenames!=NULL);
	for(i=0;i<gBootstrapSize;i++) ReadlnString(f,gBootstrap_Filenames[i]);
	f.close();
}

void InitGlobalData()
{
	int i;

	srand((unsigned)time(NULL));
	LoadOptions();
	ReadTrainSet(gHomePath + gTrainset_Filename);
	ReadValidSet(gHomePath + gValidationset_Filename);
	InitializeAuxiliaryVariables();
	ReadBootstrapFileNames();

	gCascade = new CascadeClassifier;
	ASSERT(gCascade != NULL);
	gCascade->LoadDefaultCascade();
	gStartingNode = gCascade->m_iCount+1;

	gClassifiers = new WeakClassifier[gTotalFeatures]; ASSERT(gClassifiers != NULL);
	ReadClassifiers();

	gFileUsed = new int[gMaxNumFiles]; ASSERT(gFileUsed != NULL);
	for(i=0;i<gMaxNumFiles;i++) gFileUsed[i] = 0;

	ReadRangeFile();
}

void ClearUpGlobalData()
{
	//gSetup_Filename.Empty();
	gTrainset_Filename.Empty();
	gValidationset_Filename.Empty();
	gClassifier_Filename.Empty();
	gAda_Log_Filename.Empty();
	gCascade_Filename.Empty();

	gBootstrap_Database_Filename.Empty();
	gBackup_Directory_Name.Empty();
	gTestSet_Filename.Empty();
	gSx = gSy = 0;
	gTrain_Method = 0;
	linear_classifier = 0;
	bootstrap_level = 0;
	max_bootstrap_level = 0;
	bootstrap_resizeratio.clear();
	bootstrap_increment.clear();
	gMaxNumFiles = 0;
	gNode_fp_Goal = 0.0;
	gMaxNumNodes = 0; 
	gNof.clear();

	if(gTrainSet)
	{
		delete[] gTrainSet;	
		gTrainSet = NULL;
	}
	if(gValidationSet)
	{
		delete[] gValidationSet;  
		gValidationSet = NULL;
	}

	gTotalCount = gValidationCount = 0;

	if(gCascade)
	{
		delete gCascade; 
		gCascade = NULL;
	}
	if(gWeights)
	{
		delete[] gWeights; 
		gWeights = NULL;
	}
	if(gTable)
	{
		for(int i=0;i<gTotalFeatures;i++)
		{
			delete[] gTable[i];	
			gTable[i] = NULL;
		}
		gTotalFeatures = 0;
		delete[] gTable;	
		gTable = NULL;
	}
	if(gClassifiers)
	{
		delete[] gClassifiers; 
		gClassifiers = NULL;
	}

	if(gFeatures)
	{
		delete[] gFeatures; 
		gFeatures = NULL;
	}
	if(gLabels)
	{
		delete[] gLabels; 
		gLabels = NULL;
	}
	if(gFileUsed)
	{
		delete[] gFileUsed; 
		gFileUsed = NULL;
	}

	gBootstrapSize = 0;
	if(gBootstrap_Filenames)
	{
		delete[] gBootstrap_Filenames; 
		gBootstrap_Filenames = NULL;
	}
	if(gOrgImg)
	{
		cvReleaseImage(&gOrgImg);
		gOrgImg = NULL;
	}
}


void NormalizeWeight()
{
	REAL sum;
	int i;

	sum = 0.0;
	for(i=0;i<gTotalCount;i++) sum += gWeights[i];
	for(i=0;i<gTotalCount;i++) gWeights[i] /= sum;
}
