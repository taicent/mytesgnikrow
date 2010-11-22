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
#include "FFS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CascadeClassifier::CascadeClassifier():count(0),ac(NULL)
{
}

CascadeClassifier::~CascadeClassifier()
{
	Clear();	
}

void CascadeClassifier::Clear()
{
	count = 0;
	delete[] ac; ac=NULL;
}

CascadeClassifier& CascadeClassifier::operator=(const CascadeClassifier& source)
{
	Clear();
	count = source.count;
	ac = new AdaBoostClassifier[gMaxNumNodes]; ASSERT(ac!=NULL);
	for(int i=0;i<count;i++) ac[i] = source.ac[i];
	return *this;
}

void CascadeClassifier::ReadFromFile(ifstream& f)
{
	Clear();
	f>>count; f.ignore(256,'\n');
	ac = new AdaBoostClassifier[gMaxNumNodes]; ASSERT(ac!=NULL);
	for(int i=0;i<count;i++) ac[i].ReadFromFile(f);
}

void CascadeClassifier::WriteToFile(ofstream& f) const
{
	f<<count<<endl;
	for(int i=0;i<count;i++) ac[i].WriteToFile(f);
}

void CascadeClassifier::LoadDefaultCascade()
{
	ifstream f;

	f.open(gCascade_Filename);
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
		y1 = (results[k].top<image.height)?results[k].top:(image.height-1);
		y2 = (results[k].bottom>=0)?results[k].bottom:0;
		y2 = (results[k].bottom<image.height)?results[k].bottom:(image.height-1);
		x1 = (results[k].left>=0)?results[k].left:0;
		x1 = (results[k].left<image.width)?results[k].left:(image.width-1);
		x2 = (results[k].right>=0)?results[k].right:0;
		x2 = (results[k].right<image.width)?results[k].right:(image.width-1);	
		for(i=y1;i<=y2;i++) 
		{
			image.data[i][x1] = 0;
			image.data[i][x2] = 0;
		}
		for(i=x1;i<=x2;i++)
		{
			image.data[y1][i] = 0;
			image.data[y2][i] = 0;
		}
	}
}

const int CascadeClassifier::ApplyImagePatch(const IntImage& im) const
{
	for(int i=0;i<count;i++) if(ac[i].ApplyImagePatch(im)==0) return 0;
	return 1;
}

void CascadeClassifier::ApplyOriginalSize(IntImage& original,const CString filename) 
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
	while((procface.height>gSx+1) && (procface.width>gSy+1))
	{
		procface.CalcSquareAndIntegral(square,image);
		for(int i=0,size_x=image.height-gSx;i<size_x;i+=1)
			for(int j=0,size_y=image.width-gSy;j<size_y;j+=1)
			{
				ex = image.data[i+gSx][j+gSy]+image.data[i][j]-image.data[i+gSx][j]-image.data[i][j+gSy];
				if(ex<gMean_Min || ex>gMean_Max) continue;
				sq = square.data[i+gSx][j+gSy]+square.data[i][j]-square.data[i+gSx][j]-square.data[i][j+gSy];
				if(sq<gSq_Min) continue;
				ex *= paddedsize;
				ex = ex * ex;
				sq *= paddedsize;
				sq = sq - ex;
				ASSERT(sq>=0);
				if(sq>0) sq = sqrt(sq); else sq = 1.0;
				if(sq<gVar_Min) continue;
				result = 1;
				for(int k=0;k<count;k++)
				{
					value = 0.0;
					for(int t=0,size=ac[k].count;t<size;t++)
					{
						REAL f1 = 0;
						REAL** p = image.data + i;
						SimpleClassifier& s = ac[k].scs[t];

						switch(s.type)
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
							default:
#ifndef DEBUG 
								__assume(0);
#else
								;
#endif
						}
						if(s.parity!=0)
							if(f1<sq*s.thresh)
								value += ac[k].alphas[t];
							else ;
						else
							if(f1>=sq*s.thresh)
								value += ac[k].alphas[t];
							else ;
					}
					if(value<ac[k].thresh) 
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
	DrawResults(original,results);
//	original.Save(filename+"_result.JPG");
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

void CascadeClassifier::ApplyOriginalSizeForInputBoosting(const CString filename,int& pointer) const
{
	IntImage procface;
	IntImage image,square;
	REAL sq,ex,value;
	int result;
	CRect rect;
	REAL ratio;

	procface.Load(filename);
	if(procface.height <=0 || procface.width<=0) return;
	ratio = 1.0;
	REAL paddedsize = REAL(1)/REAL((gSx+1)*(gSy+1));
	while((procface.height>gSx+1) && (procface.width>gSy+1))
	{
		procface.CalcSquareAndIntegral(square,image);
		for(int i=0,size_x=image.height-gSx;i<size_x;i+=bootstrap_increment[bootstrap_level])
			for(int j=0,size_y=image.width-gSy;j<size_y;j+=bootstrap_increment[bootstrap_level])
			{
				ex = image.data[i+gSx][j+gSy]+image.data[i][j]-image.data[i+gSx][j]-image.data[i][j+gSy];
				if(ex<gMean_Min || ex>gMean_Max) continue;
				sq = square.data[i+gSx][j+gSy]+square.data[i][j]-square.data[i+gSx][j]-square.data[i][j+gSy];
				if(sq<gSq_Min) continue;
				ex *= paddedsize;
				ex = ex * ex;
				sq *= paddedsize;
				sq = sq - ex;
				ASSERT(sq>=0);
				if(sq>0) sq = sqrt(sq); else sq = 1.0;
				if(sq<gVar_Min) continue;
				result = 1;
				for(int k=0;k<count;k++)
				{
					value = 0.0;
					for(int t=0,size=ac[k].count;t<size;t++)
						value += (ac[k].alphas[t]*ac[k].scs[t].Apply(ac[k].scs[t].GetOneFeatureTranslation(image.data+i,j)/sq));
					if(value<ac[k].thresh) 
					{
						result = 0;
						break;
					}
				}
				if(result==1)
				{
					for(int k=1;k<=gSx;k++)
						for(int t=1;t<=gSy;t++)
							gTrainSet[pointer].data[k][t]=image.data[i+k][j+t]-image.data[i+k][j]-image.data[i][j+t]+image.data[i][j];
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

	cas.ac[cas.count] = ada;
	cas.count++;

	of.open(gCascade_Filename);
	cas.WriteToFile(of);
	of.close();
}