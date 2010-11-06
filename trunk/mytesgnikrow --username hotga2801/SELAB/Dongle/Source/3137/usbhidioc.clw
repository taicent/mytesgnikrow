; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUsbhidiocDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "usbhidioc.h"

ClassCount=3
Class1=CUsbhidiocApp
Class2=CUsbhidiocDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_USBHIDIOC_DIALOG

[CLS:CUsbhidiocApp]
Type=0
HeaderFile=usbhidioc.h
ImplementationFile=usbhidioc.cpp
Filter=N
LastObject=CUsbhidiocApp

[CLS:CUsbhidiocDlg]
Type=0
HeaderFile=usbhidiocDlg.h
ImplementationFile=usbhidiocDlg.cpp
Filter=D
LastObject=CUsbhidiocDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=usbhidiocDlg.h
ImplementationFile=usbhidiocDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_USBHIDIOC_DIALOG]
Type=1
Class=CUsbhidiocDlg
ControlCount=10
Control1=IDC_STATIC,button,1342177287
Control2=IDC_cboByteToSend0,combobox,1344340226
Control3=IDC_cboByteToSend1,combobox,1344340226
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_Once,button,1342242816
Control7=IDC_Continuous,button,1342242816
Control8=IDC_AutoIncrement,button,1342242819
Control9=IDC_LIST2,listbox,1352728835
Control10=IDC_lstBytesReceived,listbox,1352728835

