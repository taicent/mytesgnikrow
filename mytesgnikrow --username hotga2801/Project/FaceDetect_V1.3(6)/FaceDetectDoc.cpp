// FaceDetectDoc.cpp : implementation of the CFaceDetectDoc class
//

#include "stdafx.h"
#include "FaceDetect.h"
#include <vector>
#include <fstream>
#include <math.h>
#include <mmsystem.h>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "Global.h"
#include "learn.h"
#include "FaceDetectDoc.h"
#include ".\facedetectdoc.h"
#include "Label.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFaceDetectDoc

IMPLEMENT_DYNCREATE(CFaceDetectDoc, CDocument)

BEGIN_MESSAGE_MAP(CFaceDetectDoc, CDocument)
	//{{AFX_MSG_MAP(CFaceDetectDoc)
	ON_COMMAND(ID_TEST_ALL, OnTestAll)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TRAIN, OnTrain)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectDoc construction/destruction

CFaceDetectDoc::CFaceDetectDoc()
{
	// TODO: add one-time construction code here

}

CFaceDetectDoc::~CFaceDetectDoc()
{
}

BOOL CFaceDetectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFaceDetectDoc serialization

void CFaceDetectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectDoc diagnostics

#ifdef _DEBUG
void CFaceDetectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFaceDetectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectDoc commands


BOOL CFaceDetectDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	BeginWaitCursor();
	if(gCascade->m_iCount>0)
	{
		image.Load(lpszPathName);
//		__int64 t1,t2;
//		QueryPerformanceCounter((LARGE_INTEGER*)&t1);
		gCascade->ApplyOriginalSize(image,lpszPathName);
//		QueryPerformanceCounter((LARGE_INTEGER*)&t2);
		//CString s;
		//s.Format("%d",gNumDetectedLabel);
		//AfxMessageBox(s);
		//UpdateAllViews(NULL);
	}
	EndWaitCursor();

	return TRUE;
}

void CFaceDetectDoc::OnTestAll() 
{
	ifstream inFile, inLabel;
	
	int i,count;
	char buf[256];
	DWORD t1,t2;
	// region add
	LoadOptions();
	gCascade = new CascadeClassifier;
	ASSERT(gCascade != NULL);
	gCascade->LoadDefaultCascade();
	ReadRangeFile();

	BeginWaitCursor();
	gNumRightLabel = 0;
	gNumWrongDetectedFace = 0;
	inFile.open(/*gTestSet_Filename*/"testsetD_filename.txt");
	inLabel.open(/*gTestSet_Label*/"testsetD_label.txt");
	inLabel >> gTotalLabel;
	inFile >> count; 
	inFile.ignore(256,'\n');

	gTotal_fp = 0;

	t1=timeGetTime();
	for(i=0;i<count;i++)
	{
		int nFaces;
		inFile.getline(buf,255,' ');
		inFile >> nFaces;
		inFile.ignore(256,'\n');
		gFaceLabels.resize(nFaces);
		for(int j = 0;j < nFaces;j++)
		{
			inLabel.ignore(256,'\n');
			inLabel >> gFaceLabels.at(j).m_LeftEye.x;
			inLabel >> gFaceLabels.at(j).m_LeftEye.y;
			inLabel >> gFaceLabels.at(j).m_RightEye.x;
			inLabel >> gFaceLabels.at(j).m_RightEye.y;
			inLabel >> gFaceLabels.at(j).m_CenterMouth.x;
			inLabel >> gFaceLabels.at(j).m_CenterMouth.y;

		}
		OnOpenDocument(buf);
	}

	inLabel.close();
	inFile.close();
	t2=timeGetTime();
	EndWaitCursor();

	CString s;
	s.Format("Total #false positives = %d",gTotal_fp);
	AfxMessageBox(s);
	s.Format("Total Label: %d.\n Num Faces detected: %d",gTotalLabel, gNumRightLabel);
	AfxMessageBox(s);
	s.Format("Total Label: %d.\n Num wrong Faces detected: %d",gTotalLabel, gNumWrongDetectedFace);
	AfxMessageBox(s);
	//s.Format("Total Label: %d milliseconds.\n",t2-t1);
	//AfxMessageBox(s);
	/*ifstream fi;
	fstream fo, fof;
	char buf[300][256];
	char comp[256] = " ";
	float point[300][6][2];
	int count = 0;
	int nfile = 0;
	fo.open("testsetB_label.txt", ios_base::out);
	fof.open("testsetB_filename.txt", ios_base::out);
	fi.open("testsetB.txt");
	while(!fi.eof())
	{
		fi.getline(buf[count], 256, ' ');
		if(strcmp(comp, buf[count]))
		{
			strcpy_s(comp, buf[count]);
			nfile ++;
		}
		int len = strlen(buf[count]);
		for(int i = 0;i < 6;i++)
			for(int j = 0;j < 2;j++)
			{
				fi >> point[count][i][j];
			}
			fi.ignore();
		count ++;
	}
	fo << count << endl;
	fof << nfile;
	nfile = 0;
	for(int i = 0; i < count;i++)
	{
		nfile ++;
		if( i == count - 1)
		{
			fof << " " << nfile + 1;
			fof << endl;
		}
		if(strcmp(comp, buf[i]))
		{
			fof << " " << nfile;
			fof << endl;
			strcpy_s(comp, buf[i]);
			fof << "testB\\";
			fof.write(comp, strlen(comp));
			nfile = 0;
		}
		//fo << "testD\\";
		//fo.write(buf[i], strlen(buf[i]));
		//fo << " ";
		for(int j = 0;j < 2;j++)
		{
			fo << point[i][0][j]<< " ";
		}
		for(int j = 0;j < 2;j++)
		{
			fo << point[i][1][j]<< " ";
		}
		for(int j = 0;j < 2;j++)
		{
			fo << point[i][4][j];
			if(j == 0)
			{
				fo << " ";

			}
			else
			{
				fo << endl;
			}
		}
	}
	fo.close();
	int stop = 0;*/
}

CString CFaceDetectDoc::GetPathName()
{
	CString strPath;
	strPath = AfxGetApp()->m_pszHelpFilePath;

	for(int i = 0;i < strPath.GetLength();i++)
	{
		if(strPath.Mid(i, 22) == _T("MarioGame_FinalProject"))
		{
			strPath = strPath.Mid(0, i + 23);
			break;
		}
	}
	return strPath;
}

void CFaceDetectDoc::OnTrain()
{
	InitTrain();
}
