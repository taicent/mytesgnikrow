#include "stdafx.h"
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <mmsystem.h>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "Global.h"
#include "Learn.h"
#include "Label.h"

CascadeClassifier::CascadeClassifier():m_iCount(0),m_AdaClassifiers(NULL)
{
}

CascadeClassifier::~CascadeClassifier()
{
	Clear();	
}

void CascadeClassifier::Clear()
{
	m_iCount = 0;
	delete[] m_AdaClassifiers; m_AdaClassifiers=NULL;
}

CascadeClassifier& CascadeClassifier::operator=(const CascadeClassifier& source)
{
	Clear();
	m_iCount = source.m_iCount;
	m_AdaClassifiers = new AdaBoostClassifier[gMaxNumNodes]; ASSERT(m_AdaClassifiers!=NULL);
	for(int i=0;i<m_iCount;i++) m_AdaClassifiers[i] = source.m_AdaClassifiers[i];
	return *this;
}

void CascadeClassifier::ReadFromFile(ifstream& f)
{
	Clear();
	f>>m_iCount; f.ignore(256,'\n');
	m_AdaClassifiers = new AdaBoostClassifier[gMaxNumNodes]; ASSERT(m_AdaClassifiers!=NULL);
	for(int i=0;i<m_iCount;i++) m_AdaClassifiers[i].ReadFromFile(f);
}

void CascadeClassifier::WriteToFile(ofstream& f) const
{
	f<<m_iCount<<endl;
	for(int i=0;i<m_iCount;i++) m_AdaClassifiers[i].WriteToFile(f);
}

void CascadeClassifier::LoadDefaultCascade()
{
	ifstream f;

	f.open(gHomePath + gCascade_Filename);
	ReadFromFile(f);
	f.close();
}

void CascadeClassifier::DrawResults(IntImage& image,const vector<CRect>& results) const
{
	int i;
	unsigned int k;
	int x1,x2,y1,y2;

	for(k=0;k<results.size();k++)
	{
		y1 = (results[k].top>=0)?results[k].top:0; 
		y1 = (results[k].top<image.m_iHeight)?results[k].top:(image.m_iHeight-1);
		y2 = (results[k].bottom>=0)?results[k].bottom:0;
		y2 = (results[k].bottom<image.m_iHeight)?results[k].bottom:(image.m_iHeight-1);
		x1 = (results[k].left>=0)?results[k].left:0;
		x1 = (results[k].left<image.m_iWidth)?results[k].left:(image.m_iWidth-1);
		x2 = (results[k].right>=0)?results[k].right:0;
		x2 = (results[k].right<image.m_iWidth)?results[k].right:(image.m_iWidth-1);	
		for(i=y1;i<=y2;i++) 
		{
			image.m_Data[i][x1] = 0;
			image.m_Data[i][x2] = 0;
			gOrgImg->imageData[gOrgImg->widthStep*i + gOrgImg->nChannels*x1] = 255;
			gOrgImg->imageData[gOrgImg->widthStep*i + gOrgImg->nChannels*x1 + 1] = 0;
			gOrgImg->imageData[gOrgImg->widthStep*i + gOrgImg->nChannels*x1 + 2] = 0;

			gOrgImg->imageData[gOrgImg->widthStep*i + gOrgImg->nChannels*x2] = 255;
			gOrgImg->imageData[gOrgImg->widthStep*i + gOrgImg->nChannels*x2 + 1] = 0;
			gOrgImg->imageData[gOrgImg->widthStep*i + gOrgImg->nChannels*x2 + 2] = 0;

		}
		for(i=x1;i<=x2;i++)
		{
			image.m_Data[y1][i] = 0;
			image.m_Data[y2][i] = 0;
			gOrgImg->imageData[gOrgImg->widthStep*y1 + gOrgImg->nChannels*i] = 255;
			gOrgImg->imageData[gOrgImg->widthStep*y1 + gOrgImg->nChannels*i + 1] = 0;
			gOrgImg->imageData[gOrgImg->widthStep*y1 + gOrgImg->nChannels*i + 2] = 0;

			gOrgImg->imageData[gOrgImg->widthStep*y2 + gOrgImg->nChannels*i] = 255;
			gOrgImg->imageData[gOrgImg->widthStep*y2 + gOrgImg->nChannels*i + 1] = 0;
			gOrgImg->imageData[gOrgImg->widthStep*y2 + gOrgImg->nChannels*i + 2] = 0;
		}
	}
	cvNamedWindow("Detected", CV_WINDOW_AUTOSIZE);
	cvShowImage("Detected", gOrgImg);
	
	cvWaitKey(0);
	cvDestroyWindow("Detected");
}

const int CascadeClassifier::ApplyImagePatch(const IntImage& im) const
{
	for(int i=0;i<m_iCount;i++) if(m_AdaClassifiers[i].ApplyImagePatch(im)==0) return 0;
	return 1;
}

void CascadeClassifier::ApplyOriginalSize(IntImage& original,const CString filename, bool isExp) 
{
	IntImage procface;
	IntImage image,square;
	REAL sq,ex,value;
	int result;
	CRect rect;
	REAL ratio;
	vector<CRect> results;

	procface.Copy(original);
	ratio = 1.0;
	results.clear();
	REAL paddedsize = REAL(1)/REAL((gSx+1)*(gSy+1));
	while((procface.m_iHeight>gSx+1) && (procface.m_iWidth>gSy+1))
	{
		procface.CalcSquareAndIntegral(square,image);
		for(int i=0,size_x=image.m_iHeight-gSx;i<size_x;i+=1)
			for(int j=0,size_y=image.m_iWidth-gSy;j<size_y;j+=1)
			{
				ex = image.m_Data[i+gSx][j+gSy]+image.m_Data[i][j]-image.m_Data[i+gSx][j]-image.m_Data[i][j+gSy];
				if(ex<gMean_Min || ex>gMean_Max) continue;
				sq = square.m_Data[i+gSx][j+gSy]+square.m_Data[i][j]-square.m_Data[i+gSx][j]-square.m_Data[i][j+gSy];
				if(sq<gSq_Min) continue;
				ex *= paddedsize;
				ex = ex * ex;
				sq *= paddedsize;
				sq = sq - ex;
				ASSERT(sq>=0);
				if(sq>0) sq = sqrt(sq); else sq = 1.0;
				if(sq<gVar_Min) continue;
				result = 1;
				for(int k=0;k<m_iCount;k++)
				{
					value = 0.0;
					for(int t=0,size=m_AdaClassifiers[k].m_iCount;t<size;t++)
					{
						REAL f1 = 0;
						REAL** p = image.m_Data + i;
						WeakClassifier& s = m_AdaClassifiers[k].m_WeakClassifiers[t];

						switch(s.m_iType)
						{
							case 0:
								f1 =   p[s.x1][j+s.y3] - p[s.x1][j+s.y1] + p[s.x3][j+s.y3] - p[s.x3][j+s.y1]
									+ REAL(2)*(p[s.x2][j+s.y1] - p[s.x2][j+s.y3]);
								break;
							case 1:
								f1 =   p[s.x3][j+s.y1] + p[s.x3][j+s.y3] - p[s.x1][j+s.y1] - p[s.x1][j+s.y3]
									+ REAL(2)*(p[s.x1][j+s.y2] - p[s.x3][j+s.y2]);
								break;
							case 2:
								f1 =   p[s.x1][j+s.y1] - p[s.x1][j+s.y3] + p[s.x4][j+s.y3] - p[s.x4][j+s.y1]
									+ REAL(3)*(p[s.x2][j+s.y3] - p[s.x2][j+s.y1] + p[s.x3][j+s.y1]  - p[s.x3][j+s.y3]);
								break;
							case 3:
								f1 =   p[s.x1][j+s.y1] - p[s.x1][j+s.y4] + p[s.x3][j+s.y4] - p[s.x3][j+s.y1]
									+ REAL(3)*(p[s.x3][j+s.y2] - p[s.x3][j+s.y3] + p[s.x1][j+s.y3] - p[s.x1][j+s.y2]);
								break;
							case 4:
								f1 =   p[s.x1][j+s.y1] + p[s.x1][j+s.y3] + p[s.x3][j+s.y1] + p[s.x3][j+s.y3]
									- REAL(2)*(p[s.x2][j+s.y1] + p[s.x2][j+s.y3] + p[s.x1][j+s.y2] + p[s.x3][j+s.y2])
									+ REAL(4)*p[s.x2][j+s.y2];
								break;
						}
						if(s.m_iParity!=0)
						{
							if(f1<sq*s.m_rThreshold)
								value += m_AdaClassifiers[k].m_rAlphas[t];
							else ;
						}
						else
							if(f1>=sq*s.m_rThreshold)
								value += m_AdaClassifiers[k].m_rAlphas[t];
							else ;
					}
					if(value<m_AdaClassifiers[k].m_rThreshold) 
					{
						result = 0;
						break;
					}
				}
				if(result!=0) 
				{
					const REAL r = 1.0/ratio;
					rect.left = (LONG)(j*r);rect.top = (LONG)(i*r);
					rect.right = (LONG)((j+gSy)*r);rect.bottom = (LONG)((i+gSx)*r);
					results.push_back(rect);
				}
			}
		ratio = ratio * REAL(0.8);
		procface.Resize(image,REAL(0.8));
		SwapIntImage(procface,image);
	}

	gTotal_fp += results.size();

	PostProcess(results,2);
	PostProcess(results,0);
	if(isExp)
	{
		CheckResults(results);
	}
	else
	{
		DrawResults(original,results);
	}
}

void CascadeClassifier::CheckResults(const vector<CRect>& results)
{
	int count = 0;
	for(int i = 0;i < gFaceLabels.size();i++)
	{
		for(int j = 0;j < results.size();j++)
		{
			if(gFaceLabels[i].IsInsideRect(results[j]))
			{
				gNumRightLabel++;
				count++;
				break;
			}
		}
	}
	gNumWrongDetectedFace += ((int)results.size() - count);
}

inline int SizeOfRect(const CRect& rect)
{
	return rect.Height()*rect.Width();
}

void PostProcess(vector<CRect>& result,const int combine_min)
{
	vector<CRect> res1;
	vector<CRect> resmax;
	vector<int> res2;
	bool yet;
	CRect rectInter;
	
	for(unsigned int i=0,size_i=result.size();i<size_i;i++)
	{
		yet = false;
		CRect& result_i = result[i];
		for(unsigned int j=0,size_r=res1.size();j<size_r;j++)
		{
			CRect& resmax_j = resmax[j];
			if(rectInter.IntersectRect(result_i,resmax_j))
			{
				if(
					SizeOfRect(rectInter)>0.6*SizeOfRect(result_i) 
				 && SizeOfRect(rectInter)>0.6*SizeOfRect(resmax_j)
				  )
				{
					CRect& res1_j = res1[j];
					resmax_j.UnionRect(resmax_j,result_i);
					res1_j.bottom += result_i.bottom;
					res1_j.top += result_i.top;
					res1_j.left += result_i.left;
					res1_j.right += result_i.right;
					res2[j]++;
					yet = true;
					break;
				}
			}
		}
		if(yet==false)
		{
			res1.push_back(result_i);
			resmax.push_back(result_i);
			res2.push_back(1);
		}
	}

	for(unsigned int i=0,size=res1.size();i<size;i++)
	{
		const int count = res2[i];
		CRect& res1_i = res1[i];
		res1_i.top /= count;
		res1_i.bottom /= count;
		res1_i.left /= count;
		res1_i.right /= count;
	}

	result.clear();
	for(unsigned int i=0,size=res1.size();i<size;i++) 
		if(res2[i]>combine_min)
			result.push_back(res1[i]);
}

void CascadeClassifier::ApplyOriginalSizeForInputBoosting(const char* filename,int& pointer) const
{
	IntImage procface;
	IntImage image,square;
	REAL sq,ex,value;
	int result;
	CRect rect;
	REAL ratio;

	procface.Load(filename);
	if(procface.m_iHeight <=0 || procface.m_iWidth<=0) return;
	ratio = 1.0;
	REAL paddedsize = REAL(1)/REAL((gSx+1)*(gSy+1));
	while((procface.m_iHeight>gSx+1) && (procface.m_iWidth>gSy+1))
	{
		procface.CalcSquareAndIntegral(square,image);
		for(int i=0,size_x=image.m_iHeight-gSx;i<size_x;i+=bootstrap_increment[bootstrap_level])
			for(int j=0,size_y=image.m_iWidth-gSy;j<size_y;j+=bootstrap_increment[bootstrap_level])
			{
				ex = image.m_Data[i+gSx][j+gSy]+image.m_Data[i][j]-image.m_Data[i+gSx][j]-image.m_Data[i][j+gSy];
				if(ex<gMean_Min || ex>gMean_Max) continue;
				sq = square.m_Data[i+gSx][j+gSy]+square.m_Data[i][j]-square.m_Data[i+gSx][j]-square.m_Data[i][j+gSy];
				if(sq<gSq_Min) continue;
				ex *= paddedsize;
				ex = ex * ex;
				sq *= paddedsize;
				sq = sq - ex;
				ASSERT(sq>=0);
				if(sq>0) sq = sqrt(sq); else sq = 1.0;
				if(sq<gVar_Min) continue;
				result = 1;
				for(int k=0;k<m_iCount;k++)
				{
					value = 0.0;
					for(int t=0,size=m_AdaClassifiers[k].m_iCount;t<size;t++)
						value += (m_AdaClassifiers[k].m_rAlphas[t]*m_AdaClassifiers[k].m_WeakClassifiers[t].Apply(m_AdaClassifiers[k].m_WeakClassifiers[t].GetOneFeatureTranslation(image.m_Data+i,j)/sq));
					if(value<m_AdaClassifiers[k].m_rThreshold) 
					{
						result = 0;
						break;
					}
				}
				if(result==1)
				{
					for(int k=1;k<=gSx;k++)
						for(int t=1;t<=gSy;t++)
							gTrainSet[pointer].m_Data[k][t]=image.m_Data[i+k][j+t]-image.m_Data[i+k][j]-image.m_Data[i][j+t]+image.m_Data[i][j];
					pointer++;
					if(pointer==gTotalCount) return;
				}
			}
		ratio = ratio * bootstrap_resizeratio[bootstrap_level];
		procface.Resize(image,bootstrap_resizeratio[bootstrap_level]);
		SwapIntImage(procface,image);
	}
}

void AppendAdaBoostClassifier(const AdaBoostClassifier& ada)
{
	CascadeClassifier cas;
	ifstream f;
	ofstream of;

	f.open(gCascade_Filename);
	cas.ReadFromFile(f);
	f.close();

	cas.m_AdaClassifiers[cas.m_iCount] = ada;
	cas.m_iCount++;

	of.open(gCascade_Filename);
	cas.WriteToFile(of);
	of.close();
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

	str.Format(_T("Training node: %d"),round);
	pwnd->SetWindowText(str);
	ada.TrainLDS(gNof[round-1],true);

	BackupIntermediateFile(gAda_Log_Filename,round);

	BackupIntermediateFile(gCascade_Filename,round);

	str.Format(_T("Training node %d finished. Bootstrapping non-face data for next node."),round);
	pwnd->SetWindowText(str);
	result = BoostingInputFiles(false);

	return result;
}