; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFaceDetectView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "facedetect.h"
LastPage=0

ClassCount=5
Class1=CFaceDetectApp
Class2=CAboutDlg
Class3=CFaceDetectDoc
Class4=CFaceDetectView
Class5=CMainFrame

ResourceCount=4
Resource1=IDR_MAINFRAME (English (U.S.))
Resource2=IDD_METHOD (English (U.S.))
Resource3=IDD_SELECT (English (U.S.))
Resource4=IDD_ABOUTBOX (English (U.S.))

[CLS:CFaceDetectApp]
Type=0
BaseClass=CWinApp
HeaderFile=FaceDetect.h
ImplementationFile=FaceDetect.cpp
LastObject=CFaceDetectApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=FaceDetect.cpp
ImplementationFile=FaceDetect.cpp
LastObject=CAboutDlg

[CLS:CFaceDetectDoc]
Type=0
BaseClass=CDocument
HeaderFile=FaceDetectDoc.h
ImplementationFile=FaceDetectDoc.cpp
LastObject=CFaceDetectDoc
Filter=N
VirtualFilter=DC

[CLS:CFaceDetectView]
Type=0
BaseClass=CView
HeaderFile=FaceDetectView.h
ImplementationFile=FaceDetectView.cpp
LastObject=ID_VIEW_TRAINIMAGE
Filter=C
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=ID_VIEW_TRAINIMAGE

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CFaceDetectDoc
Command1=ID_FILE_OPEN
Command2=ID_FILE_MRU_FILE1
Command3=ID_APP_EXIT
Command4=ID_VIEW_TOOLBAR
Command5=ID_VIEW_STATUS_BAR
Command6=ID_APP_ABOUT
Command7=ID_TEST_ALL
CommandCount=7

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_EDIT_CUT
CommandCount=2

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CFaceDetectDoc
Command1=ID_FILE_OPEN
Command2=ID_VIEW_TRAINIMAGE
CommandCount=2

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_METHOD (English (U.S.))]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_CHECK1,button,1342242819
Control3=IDC_CHECK2,button,1342242819
Control4=IDC_CHECK3,button,1342242819
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_SELECT (English (U.S.))]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_CHECK1,button,1342242819

