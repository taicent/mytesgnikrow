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
//#include "FFS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AdaBoostClassifier::AdaBoostClassifier():m_iCount(0),m_rThreshold(0.0),m_rAlphas(NULL),m_WeakClassifiers(NULL)
{
}

AdaBoostClassifier::~AdaBoostClassifier()
{
	Clear();
}

void AdaBoostClassifier::Clear()
{
	delete[] m_WeakClassifiers; m_WeakClassifiers=NULL;
	delete[] m_rAlphas; m_rAlphas=NULL;
	m_iCount = 0;
	m_rThreshold = 0.0;
}

AdaBoostClassifier& AdaBoostClassifier::operator=(const AdaBoostClassifier& source)
{
	InitToGivenSize(source.m_iCount);
	m_iCount = source.m_iCount;
	m_rThreshold = source.m_rThreshold;
	memcpy(m_rAlphas,source.m_rAlphas,sizeof(*m_rAlphas)*m_iCount);
	for(int i=0;i<m_iCount;i++) m_WeakClassifiers[i] = source.m_WeakClassifiers[i];
	return *this;
}

void AdaBoostClassifier::InitToGivenSize(const int size)
{
	Clear();
	m_iCount = 0;
	m_rThreshold = 0.0;
	m_rAlphas = new REAL[size]; ASSERT(m_rAlphas!=NULL);
	m_WeakClassifiers = new WeakClassifier[size]; ASSERT(m_WeakClassifiers!=NULL);
}

void AdaBoostClassifier::WriteToFile(ofstream& f) const
{
	f<<m_iCount<<endl;
	f<<m_rThreshold<<endl;
	for(int i=0;i<m_iCount;i++) f<<m_rAlphas[i]<<" ";
	f<<endl;
	for(int i=0;i<m_iCount;i++) m_WeakClassifiers[i].WriteToFile(f);
	f<<endl;
}

void AdaBoostClassifier::ReadFromFile(ifstream& f)
{
	Clear();
	f>>m_iCount; ASSERT(m_iCount>0);
	f.ignore(256,'\n');
	m_WeakClassifiers = new WeakClassifier[m_iCount]; ASSERT(m_WeakClassifiers!=NULL);
	m_rAlphas = new REAL[m_iCount]; ASSERT(m_rAlphas!=NULL);
	
	f>>m_rThreshold; f.ignore(256,'\n');
	for(int i=0;i<m_iCount;i++) f>>m_rAlphas[i]; f.ignore(256,'\n');
	for(int i=0;i<m_iCount;i++) m_WeakClassifiers[i].ReadFromFile(f); f.ignore(256,'\n');
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
		for(j=0;j<gTotalCount;j++) curresult[j] += (m_rAlphas[i] * m_WeakClassifiers[i].Apply(gTrainSet[j]));

		err = 0;
		for(j=0;j<gTotalCount;j++)
			if( (curresult[j]<m_rThreshold && gTrainSet[j].m_iLabel==1) || (curresult[j]>=m_rThreshold && gTrainSet[j].m_iLabel==0) )
				err++;
		f<<i<<"\t"<<err<<endl;
		f<<i<<"\t"<<m_rAlphas[i]<<"\t"<<m_WeakClassifiers[i].m_rThreshold<<" "<<m_WeakClassifiers[i].m_iParity<<" "<<m_WeakClassifiers[i].m_iType<<" "<<m_WeakClassifiers[i].x1<<" "<<m_WeakClassifiers[i].x2<<" "<<m_WeakClassifiers[i].x3<<" "<<m_WeakClassifiers[i].x4<<" "<<m_WeakClassifiers[i].y1<<" "<<m_WeakClassifiers[i].y2<<" "<<m_WeakClassifiers[i].y3<<" "<<m_WeakClassifiers[i].y4<<endl;
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
			m_rThreshold = curresult[gFaceCount+int((gTotalCount-gFaceCount)*(1-gNode_fp_Goal))]; 
			break;
	}
	fn = 0; for(i=0;i<gFaceCount;i++) if(curresult[i]<m_rThreshold) fn++;
	fp = 0; for(i=gFaceCount;i<gTotalCount;i++) if(curresult[i]>=m_rThreshold) fp++;
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

	/*if(gTrain_Method==TRAIN_ADA)*/
        TrainAdaBoost(rounds,false,method);
	//else if(gTrain_Method==TRAIN_FFS)
	//	ForwardFeatureSelection(*this,rounds,false,method);

	if(linear_classifier==LC_LAC || linear_classifier==LC_FDA)
	{
		WithinClassScatter(*this);
		curresult = new REAL[gTotalCount]; ASSERT(curresult!=NULL);
		for(i=0;i<gTotalCount;i++) curresult[i]=0.0;
		for(i=0;i<gTotalCount;i++)
			for(j=0;j<m_iCount;j++)
				curresult[i] += m_rAlphas[j]*m_WeakClassifiers[j].Apply(gTrainSet[i]);

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
				m_rThreshold = curresult[gFaceCount+int((gTotalCount-gFaceCount)*(1-gNode_fp_Goal))]; 
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

void SingleFeatureClassifier(const int* const labels,WeakClassifier& sc,const int* const indexes)
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
		sc.m_iParity = 1;
		sc.m_rError = min1;
		sc.m_rThreshold = sc.GetOneFeature(gTrainSet[indexes[pos1]]);
	}
	else
	{
		sc.m_iParity = 0;
		sc.m_rError = min2;
		sc.m_rThreshold = sc.GetOneFeature(gTrainSet[indexes[pos2]]);
	}
}

void AdaBoostClassifier::AddOneSimpleClassifier(bool* used)
{
	int i;
	REAL alpha,beta;
	WeakClassifier minsc;
	REAL minerror;
	int minindex;

	minerror = REAL(1.01); minindex  = -1;
	for(i=0;i<gTotalCount;i++) gLabels[i] = gTrainSet[i].m_iLabel;
	NormalizeWeight();
	TRACE("%f %f %f %f\n",*min_element(gWeights,gWeights+gFaceCount)*10000,*max_element(gWeights,gWeights+gFaceCount)*10000,
		     *min_element(gWeights+gFaceCount,gWeights+gTotalCount)*10000,*max_element(gWeights+gFaceCount,gWeights+gTotalCount)*10000);
	for(i=0;i<gTotalFeatures;i++)
	{
		if(used[i]) continue;
		SingleFeatureClassifier(gLabels,gClassifiers[i],gTable[i]);
		if(gClassifiers[i].m_rError<minerror)
		{
			minerror = gClassifiers[i].m_rError;
			minsc = gClassifiers[i];
			minindex = i;
		}
	}

	used[minindex] = true;
	beta = minsc.m_rError / (REAL(1.0)-minsc.m_rError);
	for(i=0;i<gTotalCount;i++)
		if(minsc.Apply(gTrainSet[i]) == gTrainSet[i].m_iLabel)
			gWeights[i] *= beta;
	if(beta<REAL(1e-8)) beta = REAL(1e-8);
	alpha = -log(beta);

	m_WeakClassifiers[m_iCount] = minsc;
	m_rAlphas[m_iCount] = alpha;
	m_rThreshold += (REAL(0.5)*alpha);
	m_iCount++;
}