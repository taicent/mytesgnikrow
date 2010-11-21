// FaceDetectView.cpp : implementation of the CFaceDetectView class
//

#include "stdafx.h"
#include <vector>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "global.h"
#include "learn.h"
#include "FaceDetect.h"

#include "FaceDetectDoc.h"
#include "FaceDetectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectView

IMPLEMENT_DYNCREATE(CFaceDetectView, CView)

BEGIN_MESSAGE_MAP(CFaceDetectView, CView)
	//{{AFX_MSG_MAP(CFaceDetectView)
	ON_COMMAND(ID_VIEW_TRAINIMAGE, OnViewTrainimage)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectView construction/destruction

CFaceDetectView::CFaceDetectView()
{
	// TODO: add construction code here
	index = -1;
}

CFaceDetectView::~CFaceDetectView()
{
}

BOOL CFaceDetectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectView drawing

void CFaceDetectView::OnDraw(CDC* pDC)
{
	CFaceDetectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int i,j;
	IntImage& im = pDoc->image;
	if(im.height>0)
	{
		for(i=0;i<im.height;i++)
			for(j=0;j<im.width;j++)
				pDC->SetPixel(j,i,RGB(im.data[i][j],im.data[i][j],im.data[i][j]));
	}

}

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectView printing

BOOL CFaceDetectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFaceDetectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFaceDetectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectView diagnostics

#ifdef _DEBUG
void CFaceDetectView::AssertValid() const
{
	CView::AssertValid();
}

void CFaceDetectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFaceDetectDoc* CFaceDetectView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFaceDetectDoc)));
	return (CFaceDetectDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectView message handlers

void CFaceDetectView::OnViewTrainimage() 
{
	CString s;
	int i,j;
	CFaceDetectDoc* pDoc;

	index++;
	if(index<0 || index>totalcount) index=0;
	s.Format("index=%d",index);
	pDoc = GetDocument();
	pDoc->SetTitle(s);
	pDoc->image=trainset[index];
	for(i=1;i<sx+1;i++)
		for(j=1;j<sy+1;j++)
		{
			pDoc->image.data[i][j]=trainset[index].data[i][j]+trainset[index].data[i-1][j-1]-trainset[index].data[i][j-1]-trainset[index].data[i-1][j];
		}
	pDoc->UpdateAllViews(NULL);
}

void CFaceDetectView::OnLButtonUp(UINT /*nFlags*/, CPoint point)
{
	CFaceDetectDoc* pDoc;

	pDoc = GetDocument();
	if(point.x<pDoc->image.width && point.y<pDoc->image.height)
	{
		CString s;
		s.Format("x=%d,y=%d,value=%d",point.x,point.y,int(pDoc->image.data[point.y][point.x]));
		pDoc->SetTitle(s);
	}
}
