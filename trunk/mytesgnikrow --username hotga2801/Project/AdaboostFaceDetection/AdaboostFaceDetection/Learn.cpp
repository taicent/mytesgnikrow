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
//#include "OptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void InitTrain()
{
	int i;
	i = gStartingNode;
	if(i!=1 && i<=gMaxNumNodes)
	{
		ofstream f;
		if(!BoostingInputFiles(true))
		{
			AfxMessageBox(_T("Train bootstrap finished"));
			return;
		}
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
			AfxMessageBox(_T("Train finished!"));
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
		char buf[256];
		wcstombs(buf,(LPCTSTR)(gHomePath + gBootstrap_Filenames[index]), 256);
		gCascade->ApplyOriginalSizeForInputBoosting(buf,pointer);
		gFileUsed[index]=1;
		index++;
	}

	for(i=0;i<gTotalCount;i++)
	{
		int k,t;
		memcpy(im.m_Buf,gTrainSet[i].m_Buf,(gSx+1)*(gSy+1)*sizeof(im.m_Buf[0]));
		for(k=0;k<=gSy;k++) gTrainSet[i].m_Data[0][k] = 0;
		for(k=0;k<=gSx;k++) gTrainSet[i].m_Data[k][0] = 0;
		for(k=1;k<=gSx;k++)
			for(t=1;t<=gSy;t++)
				gTrainSet[i].m_Data[k][t] = im.m_Data[k][t]-im.m_Data[k-1][t]-im.m_Data[k][t-1]+im.m_Data[k-1][t-1];
	}

	of.open(gTrainset_Filename,ios_base::out | ios_base::binary);
	of<<gTotalCount<<endl;
	unsigned char* writebuf;
	writebuf = new unsigned char[gSx*gSy]; ASSERT(writebuf!=NULL);
	for(i=0;i<gTotalCount;i++)
	{
		of<<gTrainSet[i].m_iLabel<<endl;
		of<<gSx<<" "<<gSy<<endl;
		for(int k=0;k<gSx;k++)
			for(int t=0;t<gSy;t++)
				writebuf[k*gSy+t] = (unsigned char)((int)gTrainSet[i].m_Data[k+1][t+1]);
		of.write((char*)writebuf,gSx*gSy);
		of<<endl;
	}
	delete[] writebuf; writebuf=NULL;
	of.close();

	for(i=0;i<gTotalCount;i++) gTrainSet[i].CalculateVarianceAndIntegralImageInPlace();
	for(i=gFaceCount;i<gTotalCount;i++) 
	{
		gCascade->ApplyImagePatch(gTrainSet[i]);
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

	savename.Format(_T("%d"),round);

	savename = gBackup_Directory_Name + name + savename + ext;
	CopyFile(filename,savename,FALSE);
}

void WriteSimpleClassifiers(void)
{
	int x1,x2,x3,x4,y1,y2,y3,y4;
	WeakClassifier sc;
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
					sc.m_iType = 0; sc.m_rError = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.m_iParity = 0;
					sc.m_rThreshold = 0.0;
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
					sc.m_iType = 1; sc.m_rError = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.m_iParity = 0;
					sc.m_rThreshold = 0.0;
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
					sc.m_iType = 2; sc.m_rError = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.m_iParity = 0;
					sc.m_rThreshold = 0.0;
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
					sc.m_iType = 3; sc.m_rError = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.m_iParity = 0;
					sc.m_rThreshold = 0.0;
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
					sc.m_iType = 4; sc.m_rError = 0.0;
					sc.x1 = x1; sc.x2 = x2; sc.x3 = x3; sc.x4 = x4;
					sc.y1 = y1; sc.y2 = y2; sc.y3 = y3; sc.y4 = y4;
					sc.m_iParity = 0;
					sc.m_rThreshold = 0.0;
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
