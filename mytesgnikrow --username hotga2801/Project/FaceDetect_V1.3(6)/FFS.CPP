#include "stdafx.h"
#include <math.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <float.h>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "Global.h"
//#include "FFS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void TrainWeakClassifiers()
{
	int i,j;
	REAL* errors; REAL* threshes;
	int* parity; int* indexes; 
	ofstream of;

	errors = new REAL[gTotalFeatures]; ASSERT(errors!=NULL);
	threshes = new REAL[gTotalFeatures]; ASSERT(threshes!=NULL);
	indexes = new int[gTotalFeatures]; ASSERT(indexes!=NULL);
	parity = new int[gTotalFeatures]; ASSERT(parity!=NULL);

	InitializeWeights();
	for(i=0;i<gTotalFeatures;i++)
	{
		FillTheTable(gTable[i],gClassifiers[i]);
		for(j=0;j<gTotalCount;j++) gLabels[j] = gTrainSet[j].m_iLabel;
		SingleFeatureClassifier(gLabels,gClassifiers[i],gTable[i]);
		errors[i] = gClassifiers[i].m_rError;
		parity[i] = gClassifiers[i].m_iParity;
		threshes[i] = gClassifiers[i].m_rThreshold;
	}
	for(i=0;i<gTotalFeatures;i++) indexes[i] = i;
	QuickSort(errors,indexes,threshes,parity,0,gTotalFeatures-1);

	of.open(FFS_WeakClassifiers_filename);
	for(i=0;i<gTotalFeatures;i++)
		of<<indexes[i]<<" "<<threshes[i]<<" "<<errors[i]<<" "<<parity[i]<<endl;
	of.close();

	delete[] errors; errors = NULL;
	delete[] threshes; threshes = NULL;
	delete[] indexes; indexes = NULL;
	delete[] parity; parity = NULL;
}

void LoadWeakClassifiers(int* const indexes)
{
	ifstream f;
	int i,j;

	f.open(FFS_WeakClassifiers_filename);
	for(i=0;i<gTotalFeatures;i++)
	{
		f>>indexes[i];
		f>>gClassifiers[indexes[i]].m_rThreshold;
		f>>gClassifiers[indexes[i]].m_rError;
		f>>gClassifiers[indexes[i]].m_iParity;
	}
	f.close();

	for(i=0;i<gTotalFeatures;i++)
		for(j=0;j<gTotalCount;j++)
			gTable[i][j]=gClassifiers[i].Apply(gTrainSet[j]);
}

void BuildHistogram(const int* const curresult, const int size,int* const hist_pos, int* const hist_neg, const int facecount)
{
	int i;

	memset(hist_pos,0,sizeof(*hist_pos)*size);
	memset(hist_neg,0,sizeof(*hist_neg)*size);
	for(i=0;i<facecount;i++) hist_pos[curresult[i]]++;
	for(i=facecount;i<gTotalCount;i++) hist_neg[curresult[i]]++;
}

void FFS_Histogram(AdaBoostClassifier& ada,const int round,int* const curresult,ofstream& f,const int method)
{
	int i,j,k;
	int* hist_pos; int* hist_neg;
	int* tempresult;
	int error,minerror,minindex;
	int thresh,minthresh;
	bool* used;

	tempresult = new int[gTotalCount]; ASSERT(tempresult!=NULL);
	hist_pos = new int[round+1]; ASSERT(hist_pos!=NULL);
	hist_neg = new int[round+1]; ASSERT(hist_neg!=NULL);
	used = new bool[gTotalFeatures]; ASSERT(used!=NULL);
	for(i=0;i<gTotalFeatures;i++) used[i]=false;
	f<<"Begin Histogram FFS"<<endl;
	ada.InitToGivenSize(round);
	for(i=0;i<gTotalCount;i++) curresult[i] = 0;
	for(i=0;i<round;i++)
	{
		minerror = INT_MAX; minindex = -1; minthresh = INT_MAX;
		for(j=0;j<gTotalFeatures;j++)
		{
			if(used[j]) continue;
			int temp;
			for(k=0;k<gTotalCount;k++) tempresult[k] = curresult[k] + gTable[j][k];
			BuildHistogram(tempresult,i+2,hist_pos,hist_neg,gFaceCount);
			error = temp = gTotalCount-gFaceCount;
			for(k=0;k<=i+1;k++)
			{
				temp = temp - hist_neg[k] + hist_pos[k];
				if(temp<error)	{ error = temp; thresh = k+1; }
			}
			if(error<minerror)
			{
				minerror = error;
				minindex = j;
				minthresh = thresh;
			}
		}
		used[minindex]=true;
		ada.m_rAlphas[ada.m_iCount] = 1;
		ada.m_WeakClassifiers[ada.m_iCount] = gClassifiers[minindex];
		ada.m_iCount++;
		ada.m_rThreshold = REAL(minthresh);
		for(j=0;j<gTotalCount;j++) curresult[j] += gTable[minindex][j];
		f<<i+1<<"\t"<<minindex<<"\t"<<minerror<<"\t"<<gClassifiers[minindex].m_rThreshold<<endl;
	}
	BuildHistogram(curresult,round+1,hist_pos,hist_neg,gFaceCount);
	for(i=1;i<=round;i++) { hist_neg[i] += hist_neg[i-1]; hist_pos[i] += hist_pos[i-1]; }
	i=0; while(hist_neg[i]<(gTotalCount-gFaceCount)*(1-gNode_fp_Goal)) i++;
	switch(method)
	{
		case GOAL_METHOD_MIN_ERROR: 
			break;
		case GOAL_METHOD_VALID_DR: 
			ValidateTheThreshold(ada,f); 
			break;
		case GOAL_METHOD_FIX_FP: 
			ada.m_rThreshold = i + REAL(0.5); 
			break;
	}
	f<<ada.m_rThreshold<<"\t"<<hist_pos[i-1]<<"\t"<<gTotalCount-gFaceCount-hist_neg[i-1]<<endl;

	int fn,fp;
	f<<"Training Error:"<<endl;
	fn = 0; for(i=0;i<gFaceCount;i++) if(ada.ApplyImagePatch(gTrainSet[i])==0) fn++;
	fp = 0; for(i=gFaceCount;i<gTotalCount;i++) if(ada.ApplyImagePatch(gTrainSet[i])==1) fp++;
	f<<fn<<"\t"<<fp<<endl;
	f<<"Validation Error:"<<endl;
	fn = 0; for(i=0;i<gValidFaceCount;i++) if(ada.ApplyImagePatch(gValidationSet[i])==0) fn++;
	fp = 0; for(i=gValidFaceCount;i<gValidationCount;i++) if(ada.ApplyImagePatch(gValidationSet[i])==1) fp++;
	f<<fn<<"\t"<<fp<<endl;
	f<<"End of Histogram FFS"<<endl;
	delete[] hist_pos; hist_pos=NULL;
	delete[] hist_neg; hist_neg=NULL;
	delete[] tempresult; tempresult=NULL;
}

void ForwardFeatureSelection(AdaBoostClassifier& ada,const int round,const bool update,const int method)
{
	int* curresult;
	int* indexes;
	ofstream f;

	ada.Clear();

	curresult = new int[gTotalCount]; ASSERT(curresult!=NULL);
	indexes = new int[gTotalFeatures]; ASSERT(indexes != NULL);
	f.open(FFS_log_filename);
	TrainWeakClassifiers();
	LoadWeakClassifiers(indexes);
	FFS_Histogram(ada,round,curresult,f,method);
	if(update) AppendAdaBoostClassifier(ada);

	delete[] curresult; curresult=NULL; 
	delete[] indexes; indexes=NULL;
	f.close();
}