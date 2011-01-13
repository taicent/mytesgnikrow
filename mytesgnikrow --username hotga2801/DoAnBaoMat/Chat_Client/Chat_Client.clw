; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChat_ClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Chat_Client.h"

ClassCount=5
Class1=CChat_ClientApp
Class2=CChat_ClientDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_DIALOG1
Resource2=IDR_MAINFRAME
Resource3=IDD_CHAT_CLIENT_DIALOG
Class4=LoginDialog
Resource4=IDD_ABOUTBOX
Class5=PrivatedChatDlg
Resource5=IDD_PRIVATED_CDLG

[CLS:CChat_ClientApp]
Type=0
HeaderFile=Chat_Client.h
ImplementationFile=Chat_Client.cpp
Filter=N
LastObject=CChat_ClientApp

[CLS:CChat_ClientDlg]
Type=0
HeaderFile=Chat_ClientDlg.h
ImplementationFile=Chat_ClientDlg.cpp
Filter=D
LastObject=IDC_EDIT3
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=Chat_ClientDlg.h
ImplementationFile=Chat_ClientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHAT_CLIENT_DIALOG]
Type=1
Class=CChat_ClientDlg
ControlCount=11
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,button,1342177287
Control4=IDC_BUTTON1,button,1342242816
Control5=IDC_EDIT2,edit,1353779396
Control6=IDC_EDIT3,edit,1350631552
Control7=IDC_EDIT4,edit,1350631552
Control8=IDC_BUTTON2,button,1342242816
Control9=IDC_BUTTON3,button,1342242816
Control10=IDC_STATIC,button,1342177287
Control11=IDC_LIST2,listbox,1352728835

[DLG:IDD_DIALOG1]
Type=1
Class=LoginDialog
ControlCount=12
Control1=IDLOGIN,button,1342242817
Control2=IDEXIT,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_EDIT3,edit,1350631584
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[CLS:LoginDialog]
Type=0
HeaderFile=LoginDialog.h
ImplementationFile=LoginDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT1
VirtualFilter=dWC

[DLG:IDD_PRIVATED_CDLG]
Type=1
Class=PrivatedChatDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,button,1342177287
Control4=IDC_EDIT2,edit,1353779396
Control5=IDC_EDIT3,edit,1352728708
Control6=IDC_BUTTON1,button,1342242816

[CLS:PrivatedChatDlg]
Type=0
HeaderFile=PrivatedChatDlg.h
ImplementationFile=PrivatedChatDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT3
VirtualFilter=dWC

