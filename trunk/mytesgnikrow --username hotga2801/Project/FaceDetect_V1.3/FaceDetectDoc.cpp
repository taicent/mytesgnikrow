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
	if(cascade->count>0)
	{
		image.Load(lpszPathName);
//		__int64 t1,t2;
//		QueryPerformanceCounter((LARGE_INTEGER*)&t1);
		cascade->ApplyOriginalSize(image,lpszPathName);
//		QueryPerformanceCounter((LARGE_INTEGER*)&t2);
//		CString s;
//		s.Format("%I64d",t2-t1);
//		AfxMessageBox(s);
		UpdateAllViews(NULL);
	}
	EndWaitCursor();

	return TRUE;
}

void CFaceDetectDoc::OnTestAll() 
{
	ifstream f;
	int i,count;
	char buf[256];
	DWORD t1,t2;

	BeginWaitCursor();
	f.open(TestSet_filename);
	f>>count; f.ignore(256,'\n');

	total_fp = 0;

	t1=timeGetTime();
	for(i=0;i<count;i++)
	{
		f.getline(buf,255,'\n');
		OnOpenDocument(buf);
	}
	f.close();
	t2=timeGetTime();
	EndWaitCursor();

	CString s;
	s.Format("Total #false positives = %d",total_fp);
	//AfxMessageBox(s);
	s.Format("Total time: %d milliseconds.\n",t2-t1);
	AfxMessageBox(s);
}

void CFaceDetectDoc::OnTrain()
{
	InitTrain();
}
