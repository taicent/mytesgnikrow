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
#include "FFS.h"
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

	dlg.m_size = starting_node;
	dlg.m_fs = train_method;
	dlg.m_lc = linear_classifier;
	dlg.m_ratio = asym_ratio;
	if(dlg.DoModal()!=IDOK)
	{
		AfxMessageBox("Training cancelled.");
		return;
	}
	else
	{
		train_method = dlg.m_fs;
		linear_classifier = dlg.m_lc;
		asym_ratio = REAL(dlg.m_ratio);
	}

	i = starting_node;
	if(i!=1 && i<=max_nodes)
	{
		ofstream f;
		if(!BoostingInputFiles(true))
		{
			AfxMessageBox("All bootstrapping file used! Training finished! (finally...)");
			return;
		}
		f.open(FileUsage_log_filename);
		for(int j=0;j<max_files;j++) f<<fileused[j]<<" ";
		f.close();
	}
	AfxGetApp()->BeginWaitCursor();
	WriteRangeFile();
	while(i<=max_nodes)
	{
		bool result;

		result = cascade->OneRound(i);
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

	im.SetSize(CSize(sx+1,sy+1));
	cascade->LoadDefaultCascade();

	pointer=facecount;
	for(i=facecount;i<totalcount;i++) 
	{
		if(discard) break;
		if(cascade->ApplyImagePatch(trainset[i])!=0)
		{
			if(pointer!=i) SwapIntImage(trainset[i],trainset[pointer]);
			pointer++;
			if(pointer==totalcount) break;
		}
	}
	if(pointer==totalcount) return true;

	index = 0;
	while(pointer<totalcount)
	{
		if(index==bootstrap_size) 
		{
			if(bootstrap_level==max_bootstrap_level-1)
				return false;
			else
			{
				bootstrap_level++;
				for(i=0;i<max_files;i++) fileused[i] = 0;
				index=0;
				pointer=facecount;
			}
		}
		if(fileused[index]==1)
		{
			index++;
			continue;
		}
		cascade->ApplyOriginalSizeForInputBoosting(bootstrap_filenames[index],pointer);
		fileused[index]=1;
		index++;
	}

	for(i=0;i<totalcount;i++)
	{
		int k,t;
		memcpy(im.buf,trainset[i].buf,(sx+1)*(sy+1)*sizeof(im.buf[0]));
		for(k=0;k<=sy;k++) trainset[i].data[0][k] = 0;
		for(k=0;k<=sx;k++) trainset[i].data[k][0] = 0;
		for(k=1;k<=sx;k++)
			for(t=1;t<=sy;t++)
				trainset[i].data[k][t] = im.data[k][t]-im.data[k-1][t]-im.data[k][t-1]+im.data[k-1][t-1];
	}

	of.open(trainset_filename,ios_base::out | ios_base::binary);
	of<<totalcount<<endl;
	unsigned char* writebuf;
	writebuf = new unsigned char[sx*sy]; ASSERT(writebuf!=NULL);
	for(i=0;i<totalcount;i++)
	{
		of<<trainset[i].label<<endl;
		of<<sx<<" "<<sy<<endl;
		for(int k=0;k<sx;k++)
			for(int t=0;t<sy;t++)
				writebuf[k*sy+t] = (unsigned char)((int)trainset[i].data[k+1][t+1]);
		of.write((char*)writebuf,sx*sy);
		of<<endl;
	}
	delete[] writebuf; writebuf=NULL;
	of.close();

	for(i=0;i<totalcount;i++) trainset[i].CalculateVarianceAndIntegralImageInPlace();
	for(i=facecount;i<totalcount;i++) 
	{
		if(cascade->ApplyImagePatch(trainset[i])==0)
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

	savename = Backup_directory_name + name + savename + ext;
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

	BackupIntermediateFile(trainset_filename,round);

	str.Format("Training node: %d",round);
	pwnd->SetWindowText(str);
	ada.TrainLDS(nof[round-1],true,goal_method);

	if(train_method==TRAIN_ADA)
		BackupIntermediateFile(ada_log_filename,round);
	else
		BackupIntermediateFile(FFS_log_filename,round);

	BackupIntermediateFile(cascade_filename,round);

	str.Format("Training node %d finished. Bootstrapping non-face data for next node.",round);
	pwnd->SetWindowText(str);
	result = BoostingInputFiles(false);

	f.open(FileUsage_log_filename);
	for(i=0;i<max_files;i++) f<<fileused[i]<<" ";
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

	for(x1=0;x1<sx;x1+=1)
		for(x3=x1+2;x3<=sx;x3+=2)
			for(y1=0;y1<sy;y1+=1)
				for(y3=y1+1;y3<=sy;y3+=1)
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

	for(x1=0;x1<sx;x1+=1)
		for(x3=x1+1;x3<=sx;x3+=1)
			for(y1=0;y1<sy;y1+=1)
				for(y3=y1+2;y3<=sy;y3+=2)
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

	for(x1=0;x1<sx;x1++)
		for(x4=x1+3;x4<=sx;x4+=3)
			for(y1=0;y1<sy;y1+=1)
				for(y3=y1+1;y3<=sy;y3+=1)
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

	for(x1=0;x1<sx;x1++)
		for(x3=x1+1;x3<=sx;x3+=1)
			for(y1=0;y1<sy;y1++)
				for(y4=y1+3;y4<=sy;y4+=3)
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

	for(x1=0;x1<sx;x1+=1)
		for(x3=x1+2;x3<=sx;x3+=2)
			for(y1=0;y1<sy;y1+=1)
				for(y3=y1+2;y3<=sy;y3+=2)
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
	for(i=0;i<totalfeatures/10;i++)
	{
		classifiers[i*10].WriteToFile(f);
	}
	f.close();
}
