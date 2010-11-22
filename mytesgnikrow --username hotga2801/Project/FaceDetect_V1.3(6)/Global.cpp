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
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString gSetup_Filename = "options.txt";

int gTotal_fp;

CString gTrainset_Filename;
CString gValidationset_Filename;
CString gClassifier_Filename;
CString gAda_Log_Filename;
CString gCascade_Filename;
CString FFS_WeakClassifiers_filename;
CString FFS_log_filename;
CString FileUsage_log_filename;
CString gBootstrap_Database_Filename;
CString gBackup_Directory_Name;
CString gTestSet_Filename;
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
int gGoal_Method;
REAL gNode_Det_Goal;
REAL gNode_fp_Goal;
int gFirst_Feature;
REAL asym_ratio;
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
SimpleClassifier* gClassifiers = NULL;

REAL* gFeatures=NULL;
int* gLabels=NULL;

int* gFileUsed = NULL;
int gBootstrapSize;
CString* gBootstrap_Filenames;

REAL gMean_Min, gMean_Max, gSq_Min, gSq_Max, gVar_Min, gVar_Max;

void WriteRangeFile(void)
// images are already integral images.
{
	CString filename;
	ofstream f;

	filename = gCascade_Filename + ".range";
	f.open(filename);
	gMean_Min = REAL(gSx*gSy*256); gSq_Min = REAL(gSx*gSy*256*256); gVar_Min = 256;
	gMean_Max = 0; gSq_Max = 0; gVar_Max = -256;
	for(int i=0;i<gFaceCount;i++)
	{
		REAL ex, sq, v;

		ex = sq = 0;
		for(int j=1;j<gTrainSet[i].height;j++) 
			for(int k=1;k<gTrainSet[i].width;k++)
			{
				v = gTrainSet[i].data[j][k]+gTrainSet[i].data[j-1][k-1]-gTrainSet[i].data[j-1][k]-gTrainSet[i].data[j][k-1];
				sq += (v*v);
			}
		ex = gTrainSet[i].data[gSx][gSy];
		gMean_Min = min(gMean_Min,ex);
		gMean_Max = max(gMean_Max,ex);
		gSq_Min = min(gSq_Min,sq);
		gSq_Max = max(gSq_Max,sq);
		gVar_Min = min(gVar_Min,gTrainSet[i].variance);
		gVar_Max = max(gVar_Max,gTrainSet[i].variance);
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

	filename = gCascade_Filename + ".range";
	f.open(filename);
	f>>gMean_Min>>gMean_Max>>gSq_Min>>gSq_Max>>gVar_Min>>gVar_Max;
	f.close();
}

void ReadOneTrainingSample(ifstream& is,IntImage& image)
{
	int i,j;
	char buf[256];

	ASSERT(gSx<=256 && gSy<=256);
	is>>image.label; is.ignore(256,'\n');
	ASSERT( (image.label == 0) || (image.label == 1) );

	is>>image.height>>image.width; is.ignore(256,'\n');
	ASSERT(image.height==gSx); 
	ASSERT(image.width==gSy);

	image.SetSize(CSize(image.height+1,image.width+1));
	for(i=0;i<image.height;i++) image.data[i][0] = 0;
	for(i=0;i<image.width;i++) image.data[0][i] = 0;
	for(i=1;i<image.height;i++)
	{
		is.read(buf,image.width-1);
		for(j=1;j<image.width;j++) 
		{
			image.data[i][j] = REAL(int(unsigned char(buf[j-1])));
			ASSERT(image.data[i][j]>=0 && image.data[i][j] <= 255);
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
		data = gTrainSet[i].data;
		f1 =   data[x1][y3] - data[x1][y1] + data[x3][y3] - data[x3][y1]
			 + 2*(data[x2][y1] - data[x2][y3]);
		features[i] = f1 / gTrainSet[i].variance;
	}
}

void GetFeatureValues1(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].data;
		f1 =   data[x3][y1] + data[x3][y3] - data[x1][y1] - data[x1][y3]
			 + 2*(data[x1][y2] - data[x3][y2]);
		features[i] = f1 / gTrainSet[i].variance;
	}
}

void GetFeatureValues2(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int x4,const int y1,const int y3)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].data;
		f1 =   data[x1][y1] -data[x1][y3] + data[x4][y3] - data[x4][y1]
			 + 3*(data[x2][y3] - data[x2][y1] + data[x3][y1] - data[x3][y3]);
		features[i] = f1 / gTrainSet[i].variance;
	}
}

void GetFeatureValues3(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3,const int y4)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].data;
		f1 =   data[x1][y1] - data[x1][y4] + data[x3][y4] - data[x3][y1]
			 + 3*(data[x3][y2] - data[x3][y3] + data[x1][y3] - data[x1][y2] );
		features[i] = f1 / gTrainSet[i].variance;
	}
}

void GetFeatureValues4(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y2,const int y3)
{
	int i;
	REAL f1;
	REAL** data;

	for(i=from;i<to;i++)
	{
		data = gTrainSet[i].data;
		f1 =   data[x1][y1] + data[x1][y3] + data[x3][y1] + data[x3][y3]
			 - 2*(data[x2][y1] + data[x2][y3] + data[x1][y2] + data[x3][y2])
			 + 4*data[x2][y2];
		features[i] = f1 / gTrainSet[i].variance;
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

void QuickSort(REAL* values,int* labels,REAL* weights,int* parity,const int lo,const int hi)
{
    int i=lo, j=hi;
	REAL v; int l; REAL w; int p;
    REAL x = values[(lo+hi)/2];

    do
    {    
        while (values[i]<x) i++; 
        while (values[j]>x) j--;
        if (i<=j)
        {
            v=values[i]; values[i]=values[j]; values[j]=v;
			l=labels[i]; labels[i]=labels[j]; labels[j]=l;
			w=weights[i];weights[i]=weights[j];weights[j]=w;
			p=parity[i];parity[i]=parity[j];parity[j]=p;
            i++; j--;
        }
    } while (i<=j);

    if (lo<j) QuickSort(values,labels,weights,parity, lo, j);
    if (i<hi) QuickSort(values,labels,weights,parity, i, hi);
}

void FillTheTable(int* const row,const SimpleClassifier& sc)
{
	int i;
	int x1,x2,x3,x4,y1,y2,y3,y4;
	
	x1 = sc.x1;	y1 = sc.y1;
	x2 = sc.x2;	y2 = sc.y2;
	x3 = sc.x3; y3 = sc.y3;
	x4 = sc.x4;	y4 = sc.y4;
	switch(sc.type)
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
		count += gTrainSet[i].label;
	return count;
}

const int CountValidFaces()
{
	int i,count;

	count = 0;
	for(i=0;i<gValidationCount;i++)
		count += gValidationSet[i].label;
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

	f.open(gClassifier_Filename);
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
	IgnoreComments(f); ReadlnString(f,FFS_WeakClassifiers_filename);
	IgnoreComments(f); ReadlnString(f,FFS_log_filename);
	IgnoreComments(f); ReadlnString(f,FileUsage_log_filename);
	IgnoreComments(f); ReadlnString(f,gBootstrap_Database_Filename);
	IgnoreComments(f); ReadlnString(f,gBackup_Directory_Name);
	IgnoreComments(f); ReadlnString(f,gTestSet_Filename);

	IgnoreComments(f); f>>gSx; IgnoreComments(f);
	IgnoreComments(f); f>>gSy; IgnoreComments(f);
	IgnoreComments(f); f>>gTrain_Method; IgnoreComments(f);
	IgnoreComments(f); f>>linear_classifier; IgnoreComments(f);
	IgnoreComments(f); f>>bootstrap_level; IgnoreComments(f);
	IgnoreComments(f); f>>max_bootstrap_level; IgnoreComments(f);
	bootstrap_resizeratio.resize(max_bootstrap_level);
	IgnoreComments(f); for(i=0;i<max_bootstrap_level;i++) f>>bootstrap_resizeratio[i]; IgnoreComments(f);
	bootstrap_increment.resize(max_bootstrap_level);
	IgnoreComments(f); for(i=0;i<max_bootstrap_level;i++) f>>bootstrap_increment[i]; IgnoreComments(f);
	IgnoreComments(f); f>>gTotalFeatures; IgnoreComments(f);
	IgnoreComments(f); f>>gMaxNumFiles; IgnoreComments(f);
	IgnoreComments(f); f>>gGoal_Method; IgnoreComments(f);
	IgnoreComments(f); f>>gNode_Det_Goal; IgnoreComments(f);
	IgnoreComments(f); f>>gNode_fp_Goal; IgnoreComments(f);
	IgnoreComments(f); f>>gFirst_Feature; IgnoreComments(f);
	IgnoreComments(f); f>>asym_ratio; IgnoreComments(f);
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
		f<<gTrainSet[i].label<<endl;
		f<<gSx<<" "<<gSy<<endl;
		for(k=0;k<gSx;k++)
			for(t=0;t<gSy;t++)
				writebuf[k*gSx+t] = (unsigned char)((int)(gTrainSet[i].data[k+1][t+1]-gTrainSet[i].data[k][t+1]-gTrainSet[i].data[k+1][t]+gTrainSet[i].data[k][t]));
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
		f<<gValidationSet[i].label<<endl;
		f<<gSx<<" "<<gSy<<endl;
		for(k=0;k<gSx;k++)
			for(t=0;t<gSy;t++)
				writebuf[k*gSx+t] = (unsigned char)((int)(gValidationSet[i].data[k+1][t+1]-gValidationSet[i].data[k][t+1]-gValidationSet[i].data[k+1][t]+gValidationSet[i].data[k][t]));
		f.write((char*)writebuf,gSx*gSy);
		f<<endl;
	}
	f.close();
}

void ReadBootstrapFileNames()
{
	ifstream f;
	int i;

	f.open(gBootstrap_Database_Filename);
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
	ReadTrainSet(gTrainset_Filename);
	ReadValidSet(gValidationset_Filename);
	InitializeAuxiliaryVariables();
	ReadBootstrapFileNames();

	gCascade = new CascadeClassifier;
	ASSERT(gCascade != NULL);
	gCascade->LoadDefaultCascade();
	gStartingNode = gCascade->count+1;

	gClassifiers = new SimpleClassifier[gTotalFeatures]; ASSERT(gClassifiers != NULL);
	ReadClassifiers();

	gFileUsed = new int[gMaxNumFiles]; ASSERT(gFileUsed != NULL);
	for(i=0;i<gMaxNumFiles;i++) gFileUsed[i] = 0;

	ReadRangeFile();
}

void ClearUpGlobalData()
{
	int i;

	gSetup_Filename.Empty();
	gTrainset_Filename.Empty();
	gValidationset_Filename.Empty();
	gClassifier_Filename.Empty();
	gAda_Log_Filename.Empty();
	gCascade_Filename.Empty();
	FFS_WeakClassifiers_filename.Empty();
	FFS_log_filename = "rate_FFS.txt";
	FileUsage_log_filename.Empty();
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
	gFirst_Feature = 0;
	gMaxNumFiles = 0;
	gGoal_Method = 0;
	gNode_Det_Goal = 0.0;
	gNode_fp_Goal = 0.0;
	asym_ratio = 0.0;
	gMaxNumNodes = 0; 
	gNof.clear();

	delete[] gTrainSet;	gTrainSet = NULL;
	delete[] gValidationSet;  gValidationSet = NULL;
	gTotalCount = gValidationCount = 0;

	delete gCascade; gCascade = NULL;

	delete[] gWeights; gWeights = NULL;
	for(i=0;i<gTotalFeatures;i++)
	{
		delete[] gTable[i];	gTable[i] = NULL;
	}
	gTotalFeatures = 0;
	delete[] gTable;	gTable = NULL;
	delete[] gClassifiers; gClassifiers = NULL;

	delete[] gFeatures; gFeatures = NULL;
	delete[] gLabels; gLabels = NULL;

	delete[] gFileUsed; gFileUsed = NULL;

	gBootstrapSize = 0;
	delete[] gBootstrap_Filenames; gBootstrap_Filenames = NULL;
}

void SkewWeight(const REAL skew_ratio)
{
	int i;

	for(i=0;i<gTotalCount;i++)
		if(gTrainSet[i].label==1)
			gWeights[i] *= skew_ratio;

	NormalizeWeight();
}

void NormalizeWeight()
{
	REAL sum;
	int i;

	sum = 0.0;
	for(i=0;i<gTotalCount;i++) sum += gWeights[i];
	for(i=0;i<gTotalCount;i++) gWeights[i] /= sum;
//	for(i=0;i<totalcount;i++) if(weights[i]<1e-8) weights[i] = 1e-8;
}

void ValidateTheThreshold(AdaBoostClassifier& ada,ofstream& f)
{
	int i,j;
	int falsepos,falseneg;

	for(i=0;i<gValidFaceCount;i++) gFeatures[i] = 0.0;
	for(i=0;i<gValidFaceCount;i++)
		for(j=0;j<ada.count;j++) 
			if(ada.scs[j].Apply(gValidationSet[i])!=0)
				gFeatures[i] += ada.alphas[j];
	nth_element(gFeatures,gFeatures+int(gValidFaceCount*(1-gNode_Det_Goal)),gFeatures+gValidFaceCount);
	ada.thresh = gFeatures[int(gValidFaceCount*(1-gNode_Det_Goal))];

	falseneg = gFaceCount;
	for(i=0;i<gFaceCount;i++) falseneg -= ada.ApplyImagePatch(gTrainSet[i]);
	falsepos = 0;
	for(i=gFaceCount;i<gTotalCount;i++) falsepos += ada.ApplyImagePatch(gTrainSet[i]);
	f<<"-----Use the validation set to determine a threshold ------"<<endl;
	f<<ada.thresh<<"\t"<<falseneg<<"\t"<<falsepos<<endl;
}

// Matrix inversion code, revised from Numerical recipe in C, and the graphviz Documentation
int lu_decompose(REAL** a,int n,REAL** lu,int* ps)
{
	int i,j,k;
	int pivotindex=0;
	REAL pivot,biggest,mult,tempf;
	REAL* scales;

	scales = new REAL[n]; ASSERT(scales!=NULL);
	for(i=0;i<n;i++) {
		biggest=0.0;
		for(j=0;j<n;j++)
			if(biggest<(tempf=fabs(lu[i][j]=a[j][i])))
				biggest=tempf;
		if(biggest!=0.0)
			scales[i] = REAL(1.0)/biggest;
		else {
			scales[i]=0.0;
			delete[] scales; scales=NULL;
			return 0; //zero row: singular matrix
		}
		ps[i]=i;
	}

	for(k=0;k<n-1;k++) {
		biggest=0.0;
		for(i=k;i<n;i++)
			if(biggest<(tempf=fabs(lu[ps[i]][k])*scales[ps[i]])) {
				biggest=tempf;
				pivotindex=i;
			}
		if(biggest==0.0)
		{
			delete[] scales; scales=NULL;
			return 0; //zero row: singular matrix
		}
		if(pivotindex!=k) {
			j=ps[k];
			ps[k] = ps[pivotindex];
			ps[pivotindex]=j;
		}

		pivot=lu[ps[k]][k];
		for(i=k+1;i<n;i++) {
			lu[ps[i]][k]=mult=lu[ps[i]][k]/pivot;
			if(mult!=0.0) {
				for(j=k+1;j<n;j++)
					lu[ps[i]][j] -= mult*lu[ps[k]][j];
			}
		}
	}
	if(lu[ps[n-1]][n-1]==0.0)
	{
		delete[] scales; scales=NULL;
		return 0; // signular matrix
	}

	delete[] scales; scales=NULL;
	return 1;
}

void lu_solve(REAL* x,REAL* b,int n,REAL** lu,int* ps)
{
	int i,j;
	REAL dot;

	for(i=0;i<n;i++) {
		dot=0.0;
		for(j=0;j<i;j++)
			dot+=lu[ps[i]][j]*x[j];
		x[i]=b[ps[i]]-dot;
	}

	for(i=n-1;i>=0;i--) {
		dot=0.0;
		for(j=i+1;j<n;j++)
			dot+=lu[ps[i]][j]*x[j];
		x[i]=(x[i]-dot)/lu[ps[i]][i];
	}
}

void MatrixInversion(REAL** a,int n)
{
	REAL** lu;
	int i,j;
	REAL* col;
	int* ps;

	lu = new REAL*[n]; ASSERT(lu!=NULL);
	for(i=0;i<n;i++)
	{
		lu[i] = new REAL[n];
		ASSERT(lu[i]!=NULL);
	}
	ps = new int[n]; ASSERT(ps!=NULL);
	if(lu_decompose(a,n,lu,ps)==0)
		AfxMessageBox("Singular Matrix 3!");
	col = new REAL[n]; ASSERT(col!=NULL);
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++) col[j]=0.0;
		col[i]=1.0;
		lu_solve(a[i],col,n,lu,ps);
	}
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		{
			REAL temp;
			temp=a[i][j];
			a[i][j]=a[j][i];
			a[j][i]=temp;
		}
	for(i=0;i<n;i++)
	{
		delete[] lu[i];
		lu[i]=NULL;
	}
	delete[] lu; lu=NULL;
	delete[] ps; ps=NULL;
	delete[] col; col=NULL;
}
// Matrix inversion code finished.

void WithinClassScatter(AdaBoostClassifier& ada)
{
	int i,j,k;
	REAL* mean1;
	REAL* mean2;
	REAL** cov;
	REAL** table;

	table = new REAL*[ada.count]; ASSERT(table!=NULL);
	for(i=0;i<ada.count;i++) 
	{
		table[i] = new REAL[gTotalCount]; ASSERT(table[i] != NULL);
	}
	for(i=0;i<ada.count;i++)
		for(j=0;j<gTotalCount;j++)
			table[i][j] = REAL(ada.scs[i].Apply(gTrainSet[j]));

	cov = new REAL*[ada.count]; ASSERT(cov!=NULL);
	for(i=0;i<ada.count;i++)
	{
		cov[i] = new REAL[ada.count]; ASSERT(cov[i]!=NULL);
	}
	for(i=0;i<ada.count;i++)
		for(j=0;j<ada.count;j++)
			cov[i][j]=0.0;

	mean1 = new REAL[ada.count]; ASSERT(mean1!=NULL);
	for(i=0;i<ada.count;i++) mean1[i] = 0;
	for(i=0;i<ada.count;i++)
		for(j=0;j<gFaceCount;j++)
			mean1[i] += table[i][j];
	for(i=0;i<ada.count;i++) mean1[i] /= gFaceCount;
	for(i=0;i<gFaceCount;i++)
		for(j=0;j<ada.count;j++)
			for(k=0;k<ada.count;k++)
				cov[j][k] += (table[j][i]-mean1[j])*(table[k][i]-mean1[k]);

	mean2 = new REAL[ada.count]; ASSERT(mean2!=NULL);
	for(i=0;i<ada.count;i++) mean2[i] = 0;
	for(i=0;i<ada.count;i++)
		for(j=gFaceCount;j<gTotalCount;j++)
			mean2[i] += table[i][j];
	for(i=0;i<ada.count;i++) mean2[i] /= (gTotalCount-gFaceCount);

	if(linear_classifier==LC_FDA)
	{
		for(i=gFaceCount;i<gTotalCount;i++)
			for(j=0;j<ada.count;j++)
				for(k=0;k<ada.count;k++)
					cov[j][k] += (table[j][i]-mean2[j])*(table[k][i]-mean2[k]);
	}
	for(i=0;i<ada.count;i++) cov[i][i] += REAL(0.1);

	MatrixInversion(cov,ada.count);

	for(i=0;i<ada.count;i++) ada.alphas[i]=0.0;
	for(i=0;i<ada.count;i++)
		for(j=0;j<ada.count;j++)
			ada.alphas[i] += cov[i][j]*(mean1[j]-mean2[j]);

	delete[] mean1; mean1=NULL;
	delete[] mean2; mean2=NULL;
	for(i=0;i<ada.count;i++)
	{
		delete[] cov[i]; cov[i]=NULL;
	}
	delete[] cov; cov=NULL;
	for(i=0;i<ada.count;i++)
	{
		delete[] table[i]; table[i]=NULL;
	}
	delete[] table; table=NULL;
}	