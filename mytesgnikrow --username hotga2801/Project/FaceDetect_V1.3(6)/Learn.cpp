#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
#include <math.h>
#include <mmsystem.h>
using namespace std;
#include "resource.h"
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "Global.h"
//#include "FFS.h"
#include "learn.h"
#include "OptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void InitTrain()
{
	int i;
	OptionDialog dlg;

	dlg.m_size = gStartingNode;
	dlg.m_fs = gTrain_Method;
	dlg.m_lc = linear_classifier;
	dlg.m_ratio = asym_ratio;
	if(dlg.DoModal()!=IDOK)
	{
		AfxMessageBox("Training canceled.");
		return;
	}
	else
	{
		gTrain_Method = TRAIN_ADA;
		linear_classifier = LC_ORIGINAL;
		asym_ratio = REAL(1);
	}

	i = gStartingNode;
	if(i!=1 && i<=gMaxNumNodes)
	{
		ofstream f;
		if(!BoostingInputFiles(true))
		{
			AfxMessageBox("All bootstrapping file used! Training finished! (finally...)");
			return;
		}
		f.open(FileUsage_log_filename);
		for(int j=0;j<gMaxNumFiles;j++) f<<gFileUsed[j]<<" ";
		f.close();
	}
	AfxGetApp()->BeginWaitCursor();
	WriteRangeFile();
	while(i<=gMaxNumNodes)
	{
		bool result;

		result = gCascade->OneRound(i);
		if(result==false)
		{
			AfxGetApp()->EndWaitCursor();
			AfxMessageBox("All bootstrapping file used! Training finished! (finally...)");
			return;
		}
		i++;
	}
	AfxGetApp()->EndWaitCursor();
}

const bool BoostingInputFiles(const bool discard)
{
	int i,pointer,index;
	IntImage im;
	ofstream of;

	im.SetSize(CSize(gSx+1,gSy+1));
	gCascade->LoadDefaultCascade();

	pointer=gFaceCount;
	for(i=gFaceCount;i<gTotalCount;i++) 
	{
		if(discard) break;
		if(gCascade->ApplyImagePatch(gTrainSet[i])!=0)
		{
			if(pointer!=i) SwapIntImage(gTrainSet[i],gTrainSet[pointer]);
			pointer++;
			if(pointer==gTotalCount) break;
		}
	}
	if(pointer==gTotalCount) return true;

	index = 0;
	while(pointer<gTotalCount)
	{
		if(index==gBootstrapSize) 
		{
			if(bootstrap_level==max_bootstrap_level-1)
				return false;
			else
			{
				bootstrap_level++;
				for(i=0;i<gMaxNumFiles;i++) gFileUsed[i] = 0;
				index=0;
				pointer=gFaceCount;
			}
		}
		if(gFileUsed[index]==1)
		{
			index++;
			continue;
		}
		gCascade->ApplyOriginalSizeForInputBoosting(gBootstrap_Filenames[index],pointer);
		gFileUsed[index]=1;
		index++;
	}

	for(i=0;i<gTotalCount;i++)
	{
		int k,t;
		memcpy(im.buf,gTrainSet[i].buf,(gSx+1)*(gSy+1)*sizeof(im.buf[0]));
		for(k=0;k<=gSy;k++) gTrainSet[i].data[0][k] = 0;
		for(k=0;k<=gSx;k++) gTrainSet[i].data[k][0] = 0;
		for(k=1;k<=gSx;k++)
			for(t=1;t<=gSy;t++)
				gTrainSet[i].data[k][t] = im.data[k][t]-im.data[k-1][t]-im.data[k][t-1]+im.data[k-1][t-1];
	}

	of.open(gTrainset_Filename,ios_base::out | ios_base::binary);
	of<<gTotalCount<<endl;
	unsigned char* writebuf;
	writebuf = new unsigned char[gSx*gSy]; ASSERT(writebuf!=NULL);
	for(i=0;i<gTotalCount;i++)
	{
		of<<gTrainSet[i].label<<endl;
		of<<gSx<<" "<<gSy<<endl;
		for(int k=0;k<gSx;k++)
			for(int t=0;t<gSy;t++)
				writebuf[k*gSy+t] = (unsigned char)((int)gTrainSet[i].data[k+1][t+1]);
		of.write((char*)writebuf,gSx*gSy);
		of<<endl;
	}
	delete[] writebuf; writebuf=NULL;
	of.close();

	for(i=0;i<gTotalCount;i++) gTrainSet[i].CalculateVarianceAndIntegralImageInPlace();
	for(i=gFaceCount;i<gTotalCount;i++) 
	{
		if(gCascade->ApplyImagePatch(gTrainSet[i])==0)
			; //AfxMessageBox("Something is wrong?");
	}

	return true;
}

void BackupIntermediateFile(const CString filename, const int round)
{
	CString savename;
	CString name,ext;
	int pos;

	bool backup = false;

	if(backup==false) return;

	pos = filename.ReverseFind('\\');
	if(pos>0)
		savename = filename.Right(filename.GetLength()-pos-1);
	else
		savename = filename;

	pos = savename.ReverseFind('.');
	name = savename.Left(pos);
	ext = savename.Right(savename.GetLength()-pos);

	savename.Format("%d",round);

	savename = gBackup_Directory_Name + name + savename + ext;
	CopyFile(filename,savename,FALSE);
}

const bool CascadeClassifier::OneRound(const int round)
{
	ofstream f;
	bool result;
	int i;
	AdaBoostClassifier ada;
	CWnd* pwnd;
	CString str;

	pwnd = AfxGetMainWnd();

	BackupIntermediateFile(gTrainset_Filename,round);

	str.Format("Training node: %d",round);
	pwnd->SetWindowText(str);
	ada.TrainLDS(gNof[round-1],true,gGoal_Method);

	//if(gTrain_Method==TRAIN_ADA)
		BackupIntermediateFile(gAda_Log_Filename,round);
	//else
	//	BackupIntermediateFile(FFS_log_filename,round);

	BackupIntermediateFile(gCascade_Filename,round);

	str.Format("Training node %d finished. Bootstrapping non-face data for next node.",round);
	pwnd->SetWindowText(str);
	result = BoostingInputFiles(false);

	f.open(FileUsage_log_filename);
	for(i=0;i<gMaxNumFiles;i++) f<<gFileUsed[i]<<" ";
	f.close();

	BackupIntermediateFile(FileUsage_log_filename,round);

	return result;
}

void WriteSimpleClassifiers(void)
{
	int x1,x2,x3,x4,y1,y2,y3,y4;
	SimpleClassifier sc;
	int index;
	ofstream f;

	int pickup=9;

	f.open("classifiers.txt");
	index = 0;

	for(x1=0;x1<gSx;x1+=1)
		for(x3=x1+2;x3<=gSx;x3+=2)
			for(y1=0;y1<gSy;y1+=1)
				for(y3=y1+1;y3<=gSy;y3+=1)
				{
					x2 = (x1+x3)/2;
					y2 = y4 = x4 = -1;
					sc.type = 0; sc.error = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.parity = 0;
					sc.thresh = 0.0;
					if(index%10==pickup) sc.WriteToFile(f);
					index++;
				}

	for(x1=0;x1<gSx;x1+=1)
		for(x3=x1+1;x3<=gSx;x3+=1)
			for(y1=0;y1<gSy;y1+=1)
				for(y3=y1+2;y3<=gSy;y3+=2)
				{
					y2 = (y1+y3)/2;
					x2 = x4 = y4 = -1;
					sc.type = 1; sc.error = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.parity = 0;
					sc.thresh = 0.0;
					if(index%10==pickup) sc.WriteToFile(f);
					index++;
				}

	for(x1=0;x1<gSx;x1++)
		for(x4=x1+3;x4<=gSx;x4+=3)
			for(y1=0;y1<gSy;y1+=1)
				for(y3=y1+1;y3<=gSy;y3+=1)
				{
					x2 = x1 + (x4-x1)/3;
					x3 = x2 + (x4-x1)/3;
					y2 = y4 = -1;
					sc.type = 2; sc.error = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.parity = 0;
					sc.thresh = 0.0;
					if(index%10==pickup) sc.WriteToFile(f);
					index++;
				}

	for(x1=0;x1<gSx;x1++)
		for(x3=x1+1;x3<=gSx;x3+=1)
			for(y1=0;y1<gSy;y1++)
				for(y4=y1+3;y4<=gSy;y4+=3)
				{
					y2 = y1 + (y4-y1)/3;
					y3 = y2 + (y4-y1)/3;
					x2 = x4 = -1;
					sc.type = 3; sc.error = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.parity = 0;
					sc.thresh = 0.0;
					if(index%10==pickup) sc.WriteToFile(f);
					index++;
				}

	for(x1=0;x1<gSx;x1+=1)
		for(x3=x1+2;x3<=gSx;x3+=2)
			for(y1=0;y1<gSy;y1+=1)
				for(y3=y1+2;y3<=gSy;y3+=2)
				{
					x2 = (x1+x3)/2;
					y2 = (y1+y3)/2;
					x4 = y4 = -1;
					sc.type = 4; sc.error = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.parity = 0;
					sc.thresh = 0.0;
					if(index%10==pickup) sc.WriteToFile(f);
					index++;
				}

	f.close();
}

void SubSampleClassifiers()
{
	ofstream f;
	int i;

	f.open("classifiers.txt");
	for(i=0;i<gTotalFeatures/10;i++)
	{
		gClassifiers[i*10].WriteToFile(f);
	}
	f.close();
}
