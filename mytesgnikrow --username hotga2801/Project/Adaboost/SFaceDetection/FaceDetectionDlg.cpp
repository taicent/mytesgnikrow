/*
---- Author : Binh Nguyen - Bioz
---- Last Update : 21 Oct 2008	 
---- License : Free
---- Official site : www.sectic.com
---- Description : detect face by using haar model supplied by OpenCV lib. 
*/
// FaceDetectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FaceDetection.h"
#include "FaceDetectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CFaceDetectionDlg dialog

CFaceDetectionDlg::CFaceDetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFaceDetectionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFaceDetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFaceDetectionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CFaceDetectionDlg::OnBnClickedOk)	
	ON_BN_CLICKED(IDCANCEL, &CFaceDetectionDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CASCADE_OPEN, &CFaceDetectionDlg::OnBnClickedButtonCascadeOpen)
	ON_LBN_SELCHANGE(IDC_LIST_IMAGE, &CFaceDetectionDlg::OnLbnSelchangeListImage)
	ON_LBN_SELCHANGE(IDC_LIST_CASCADE, &CFaceDetectionDlg::OnLbnSelchangeListCascade)	
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_OPEN, &CFaceDetectionDlg::OnBnClickedButtonImageOpen)
END_MESSAGE_MAP()


// CFaceDetectionDlg message handlers

BOOL CFaceDetectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	
	//-------- Set default cascade ------------

	char strPathName[_MAX_PATH];
	::GetModuleFileName(NULL, strPathName, _MAX_PATH);

	// The following code will allow you to get the path.

	CString temp_path(strPathName);
	int fpos = temp_path.ReverseFind('\\');

	if (fpos != -1)
	{
		cascade_root = temp_path.Left(fpos + 1);
		image_path = cascade_root + "face_default.jpg";
		cascade_path = cascade_root + "haarcascade_frontalface_default.xml";
	}

	GetDlgItem(IDC_EDIT_SCALE)->SetWindowText("1");

	//----------------------------------------------------
			
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFaceDetectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFaceDetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFaceDetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFaceDetectionDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OpenCVFaceDetection *detector;
	int status;
	double time; // time to run algorithm

	CvSeq* faces = NULL;  // Storage for found face coordinates.
	
	status = detector->init_cascade(cascade_path);

	if(status < 0){
		fprintf(stderr, "Initializing cascade failed.\n");
		exit(1);
	}
	
	//--------- Get Value of Scale from Textbox ------------
	CString strTextBoxValue;
	CWnd* txtScale=GetDlgItem(IDC_EDIT_SCALE);
	txtScale->GetWindowText(strTextBoxValue);

	if(strTextBoxValue.IsEmpty())
	{
		strTextBoxValue.Format(_T("%f"),SCALE);
	}

	double scale;
	try{
		scale = atof(strTextBoxValue);
	}catch(CException* e)
	{		
		AfxMessageBox("Please input a number for scale!",MB_OK,0);
		GetDlgItem(IDC_EDIT_SCALE)->SetWindowText("1");
	}
	
	//------------------------------------------------------

	IplImage *output;																			// This is the image pointer (output)
		
	output= cvCreateImage(cvSize(image->width,image->height), image->depth, image->nChannels);  // output image memory allocation
	
	cvCopyImage(image, output);																	// create a copy of origin image

	faces = detector->detect_face(output, atof(strTextBoxValue), time);

	CString temp;

	temp.Format(_T("Num of Face: %d"), faces->total);

	GetDlgItem(IDC_STATIC_STATISTIC_LABEL)->SetWindowText(temp);
	
	temp.Format(_T("Time to run: %f ms"), time);

	GetDlgItem(IDC_STATIC_STATISTIC_LABEL2)->SetWindowText(temp);

	cvNamedWindow( "Original Image");       // Create window for image show by OpenCV
	cvShowImage("Original Image", output);  // Show image in the window
	cvReleaseImage(&output);
	
}

void CFaceDetectionDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	cvDestroyAllWindows();
	OnCancel();
}

void CFaceDetectionDlg::OnBnClickedButtonCascadeOpen()
{
	 //TODO: Add your control notification handler code here
	BROWSEINFO   bi; 
	ZeroMemory(&bi,   sizeof(bi)); 
	TCHAR   szDisplayName[MAX_PATH]; 
	szDisplayName[0]    =   ' ';  

	bi.hwndOwner        =   NULL; 
	bi.pidlRoot         =   NULL; 
	bi.pszDisplayName   =   szDisplayName; 
	bi.lpszTitle        =   _T("Please select a folder that stored cascade files :"); 
	bi.ulFlags          =   BIF_RETURNONLYFSDIRS;
	bi.lParam           =   NULL; 
	bi.iImage           =   0;  

	LPITEMIDLIST   pidl   =   SHBrowseForFolder(&bi);
	TCHAR   szPathName[MAX_PATH]; 
	if   (NULL   !=   pidl)
	{
		BOOL bRet = SHGetPathFromIDList(pidl,szPathName);
		if(FALSE == bRet)
		{
			return;
		}
		cascade_root = strcat(szPathName,"\\");

		LoadFileListToListBox(IDC_LIST_CASCADE,cascade_root, "*.xml");
	}
}

void CFaceDetectionDlg::LoadFileListToListBox(int id_list, CString root, CString extFile)
{
	CListBox * pList;
	pList = (CListBox *)GetDlgItem(id_list);
	
	pList->ResetContent();
	
	CFileFind finder;
	BOOL finding = finder.FindFile (root + extFile);

	//iterate the results

	int count=0;
	while (finding)
	{
		finding = finder.FindNextFile();

		pList->AddString(finder.GetFileName());
		
		count++;
	}

	if(count <= 0)
	{
		pList->AddString("no files found");
	}
	
	finder.Close ();	
}

CString CFaceDetectionDlg::GetFileNameFromListBox(int id_list)
{
	CListBox * pList;
	pList = (CListBox *)GetDlgItem(id_list);

	CString result="";

	int nIndex = pList->GetCurSel();	

	pList->GetText(nIndex, result);

	return result;
}

void CFaceDetectionDlg::OnLbnSelchangeListImage()
{
	// TODO: Add your control notification handler code here
	CString path = image_path + GetFileNameFromListBox(IDC_LIST_IMAGE);

	image= cvLoadImage(path);             // load the image

	cvNamedWindow( "Original Image");     // Create window for image show by OpenCV

	cvShowImage("Original Image", image); // display it
}

void CFaceDetectionDlg::OnLbnSelchangeListCascade()
{
	// TODO: Add your control notification handler code here
	cascade_path = cascade_root + GetFileNameFromListBox(IDC_LIST_CASCADE);
}
void CFaceDetectionDlg::OnBnClickedButtonImageOpen()
{
	// TODO: Add your control notification handler code here
	BROWSEINFO   bi; 
	ZeroMemory(&bi,   sizeof(bi)); 
	TCHAR   szDisplayName[MAX_PATH]; 
	szDisplayName[0]    =   ' ';  

	bi.hwndOwner        =   NULL; 
	bi.pidlRoot         =   NULL; 
	bi.pszDisplayName   =   szDisplayName; 
	bi.lpszTitle        =   _T("Please select a folder that stored image files :"); 
	bi.ulFlags          =   BIF_RETURNONLYFSDIRS;
	bi.lParam           =   NULL; 
	bi.iImage           =   0;  

	LPITEMIDLIST   pidl   =   SHBrowseForFolder(&bi);
	TCHAR   szPathName[MAX_PATH]; 
	if   (NULL   !=   pidl)
	{
		BOOL bRet = SHGetPathFromIDList(pidl,szPathName);
		if(FALSE == bRet)
		{
			return;
		}
		image_path = strcat(szPathName,"\\");

		LoadFileListToListBox(IDC_LIST_IMAGE,image_path, "*.jpg");
	}
}
