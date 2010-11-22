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
#include "FFS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AdaBoostClassifier::AdaBoostClassifier():count(0),thresh(0.0),alphas(NULL),scs(NULL)
{
}

AdaBoostClassifier::~AdaBoostClassifier()
{
	Clear();
}

void AdaBoostClassifier::Clear()
{
	delete[] scs; scs=NULL;
	delete[] alphas; alphas=NULL;
	count = 0;
	thresh = 0.0;
}

AdaBoostClassifier& AdaBoostClassifier::operator=(const AdaBoostClassifier& source)
{
	InitToGivenSize(source.count);
	count = source.count;
	thresh = source.thresh;
	memcpy(alphas,source.alphas,sizeof(*alphas)*count);
	for(int i=0;i<count;i++) scs[i] = source.scs[i];
	return *this;
}

void AdaBoostClassifier::InitToGivenSize(const int size)
{
	Clear();
	count = 0;
	thresh = 0.0;
	alphas = new REAL[size]; ASSERT(alphas!=NULL);
	scs = new SimpleClassifier[size]; ASSERT(scs!=NULL);
}

void AdaBoostClassifier::WriteToFile(ofstream& f) const
{
	f<<count<<endl;
	f<<thresh<<endl;
	for(int i=0;i<count;i++) f<<alphas[i]<<" ";
	f<<endl;
	for(int i=0;i<count;i++) scs[i].WriteToFile(f);
	f<<endl;
}

void AdaBoostClassifier::ReadFromFile(ifstream& f)
{
	Clear();
	f>>count; ASSERT(count>0);
	f.ignore(256,'\n');
	scs = new SimpleClassifier[count]; ASSERT(scs!=NULL);
	alphas = new REAL[count]; ASSERT(alphas!=NULL);
	
	f>>thresh; f.ignore(256,'\n');
	for(int i=0;i<count;i++) f>>alphas[i]; f.ignore(256,'\n');
	for(int i=0;i<count;i++) scs[i].ReadFromFile(f); f.ignore(256,'\n');
}

void AdaBoostClassifier::TrainAdaBoost(int rounds,const bool update,const int method)
{
	ofstream f;
	int i,j;
	REAL* curresult;
	int fn,fp;
	bool* used;

	InitToGivenSize(rounds);
	for(i=0;i<gTotalFeatures;i++) FillTheTable(gTable[i],gClassifiers[i]);
 	InitializeWeights();
	used = new bool[gTotalFeatures]; ASSERT(used!=NULL);
	for(i=0;i<gTotalFeatures;i++) used[i]=false;

	f.open(gAda_Log_Filename);
	curresult = new REAL[gTotalCount]; 	ASSERT(curresult!=NULL);
	f<<gFaceCount<<"\t"<<gTotalCount-gFaceCount<<endl;
	for(i=0;i<gTotalCount;i++) curresult[i] = 0.0;
	for(i=0;i<rounds;i++)
	{
		int err;

		if(asym_ratio>1.0) SkewWeight(asym_ratio);
		AddOneSimpleClassifier(used);
		for(j=0;j<gTotalCount;j++) curresult[j] += (alphas[i] * scs[i].Apply(gTrainSet[j]));

		err = 0;
		for(j=0;j<gTotalCount;j++)
			if( (curresult[j]<thresh && gTrainSet[j].label==1) || (curresult[j]>=thresh && gTrainSet[j].label==0) )
				err++;
		f<<i<<"\t"<<err<<endl;
		f<<i<<"\t"<<alphas[i]<<"\t"<<scs[i].thresh<<" "<<scs[i].parity<<" "<<scs[i].type<<" "<<scs[i].x1<<" "<<scs[i].x2<<" "<<scs[i].x3<<" "<<scs[i].x4<<" "<<scs[i].y1<<" "<<scs[i].y2<<" "<<scs[i].y3<<" "<<scs[i].y4<<endl;
	}
	nth_element(curresult+gFaceCount,curresult+gFaceCount+int((gTotalCount-gFaceCount)*(1-gNode_fp_Goal)),curresult+gTotalCount);
	switch(method)
	{
		case GOAL_METHOD_MIN_ERROR: 
			break;
		case GOAL_METHOD_VALID_DR: 
			ValidateTheThreshold(*this,f); 
			break;
		case GOAL_METHOD_FIX_FP: 
			thresh = curresult[gFaceCount+int((gTotalCount-gFaceCount)*(1-gNode_fp_Goal))]; 
			break;
	}
	fn = 0; for(i=0;i<gFaceCount;i++) if(curresult[i]<thresh) fn++;
	fp = 0; for(i=gFaceCount;i<gTotalCount;i++) if(curresult[i]>=thresh) fp++;
	f<<fn<<"\t"<<fp<<endl;
	f<<"Validation Error:"<<endl;
	fn = 0; for(i=0;i<gValidFaceCount;i++) if(ApplyImagePatch(gValidationSet[i])==0) fn++;
	fp = 0; for(i=gValidFaceCount;i<gValidationCount;i++) if(ApplyImagePatch(gValidationSet[i])==1) fp++;
	f<<fn<<"\t"<<fp<<endl;
	f.close();
	
	if(update) AppendAdaBoostClassifier(*this);
	delete[] used; used=NULL;
	delete[] curresult; curresult = NULL;
}

void AdaBoostClassifier::TrainLDS(int rounds,const bool update,const int method)
{
	REAL* curresult;
	int i,j;
	ofstream f;
	int fn,fp;

	if(gTrain_Method==TRAIN_ADA)
        TrainAdaBoost(rounds,false,method);
	else if(gTrain_Method==TRAIN_FFS)
		ForwardFeatureSelection(*this,rounds,false,method);

	if(linear_classifier==LC_LAC || linear_classifier==LC_FDA)
	{
		WithinClassScatter(*this);
		curresult = new REAL[gTotalCount]; ASSERT(curresult!=NULL);
		for(i=0;i<gTotalCount;i++) curresult[i]=0.0;
		for(i=0;i<gTotalCount;i++)
			for(j=0;j<count;j++)
				curresult[i] += alphas[j]*scs[j].Apply(gTrainSet[i]);

		nth_element(curresult+gFaceCount,curresult+gFaceCount+int((gTotalCount-gFaceCount)*(1-gNode_fp_Goal)),curresult+gTotalCount);
		if(gTrain_Method==TRAIN_ADA)
			f.open(gAda_Log_Filename, ios_base::out|ios_base::app);
		else if(gTrain_Method==TRAIN_FFS)
			f.open(FFS_log_filename, ios_base::out|ios_base::app);
		switch(method)
		{
			case GOAL_METHOD_MIN_ERROR: 
				break;
			case GOAL_METHOD_VALID_DR: 
				ValidateTheThreshold(*this,f); 
				break;
			case GOAL_METHOD_FIX_FP: 
				thresh = curresult[gFaceCount+int((gTotalCount-gFaceCount)*(1-gNode_fp_Goal))]; 
				break;
		}
		fn = 0; for(i=0;i<gFaceCount;i++) if(ApplyImagePatch(gTrainSet[i])==0) fn++;
		fp = 0; for(i=gFaceCount;i<gTotalCount;i++) if(ApplyImagePatch(gTrainSet[i])==1) fp++;
		f<<fn<<"\t"<<fp<<endl;
		f<<"LDS	 Error:"<<endl;
		fn = 0; for(i=0;i<gValidFaceCount;i++) if(ApplyImagePatch(gValidationSet[i])==0) fn++;
		fp = 0; for(i=gValidFaceCount;i<gValidationCount;i++) if(ApplyImagePatch(gValidationSet[i])==1) fp++;
		f<<fn<<"\t"<<fp<<endl;	
		f.close();
		delete[] curresult; curresult=NULL;
	}
	if(update) AppendAdaBoostClassifier(*this);
}

void SingleFeatureClassifier(const int* const labels,SimpleClassifier& sc,const int* const indexes)
{
	int i;
	REAL min1; REAL min2;
	REAL e1,e2;
	int pos1,pos2;

	// compute errors1, suppose parity is 1, that is f(x)<thresh ==> h(x) = 1
	// compute errors2, suppose parity is 0, that is f(x)>thresh ==> h(x) = 1
	e1 = 0.0; for(i=0;i<gTotalCount;i++) if(labels[i]!=0) e1+=gWeights[i];
	e2 = REAL(1.0)-e1;
	min1 = e1; pos1 = 0;
	min2 = e2; pos2 = 0;
	for(i=0;i<gTotalCount;i++)
	{
		if(labels[indexes[i]]!=0) e1-=gWeights[indexes[i]]; else e1+=gWeights[indexes[i]];
		if(e1<min1) { min1=e1; pos1=i; }
		e2 = 1 - e1;
		if(e2<min2) { min2=e2; pos2=i; }
	}

	pos1++; if(pos1==gTotalCount) pos1--;
	pos2++; if(pos2==gTotalCount) pos2--;
	if(min1<min2)
	{
		sc.parity = 1;
		sc.error = min1;
		sc.thresh = sc.GetOneFeature(gTrainSet[indexes[pos1]]);
	}
	else
	{
		sc.parity = 0;
		sc.error = min2;
		sc.thresh = sc.GetOneFeature(gTrainSet[indexes[pos2]]);
	}
}

void AdaBoostClassifier::AddOneSimpleClassifier(bool* used)
{
	int i;
	REAL alpha,beta;
	SimpleClassifier minsc;
	REAL minerror;
	int minindex;

	minerror = REAL(1.01); minindex  = -1;
	for(i=0;i<gTotalCount;i++) gLabels[i] = gTrainSet[i].label;
	NormalizeWeight();
	TRACE("%f %f %f %f\n",*min_element(gWeights,gWeights+gFaceCount)*10000,*max_element(gWeights,gWeights+gFaceCount)*10000,
		     *min_element(gWeights+gFaceCount,gWeights+gTotalCount)*10000,*max_element(gWeights+gFaceCount,gWeights+gTotalCount)*10000);
	for(i=0;i<gTotalFeatures;i++)
	{
		if(used[i]) continue;
		SingleFeatureClassifier(gLabels,gClassifiers[i],gTable[i]);
		if(gClassifiers[i].error<minerror)
		{
			minerror = gClassifiers[i].error;
			minsc = gClassifiers[i];
			minindex = i;
		}
	}

	used[minindex] = true;
	beta = minsc.error / (REAL(1.0)-minsc.error);
	for(i=0;i<gTotalCount;i++)
		if(minsc.Apply(gTrainSet[i]) == gTrainSet[i].label)
			gWeights[i] *= beta;
	if(beta<REAL(1e-8)) beta = REAL(1e-8);
	alpha = -log(beta);

	scs[count] = minsc;
	alphas[count] = alpha;
	thresh += (REAL(0.5)*alpha);
	count++;
}