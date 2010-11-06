// usbhidiocDlg.cpp : implementation file
/*
Project: usbhidioc.cpp
Version: 2.0
Date: 12/20/00
by Jan Axelson (jan@lvr.com)

Purpose: demonstrates USB communications with a HID-class device using overlapped I/O.

Description: 
	Finds an attached HID-class device that matches specific vendor & product IDs.
	Retrieves the device's capabilities.
	Sends two bytes to the device using Input reports.
	Receives two bytes from the device using Output reports. 
	A list box displays a log of activity.
	A Bytes Received list box displays the most recent received report bytes.
	Combo boxes enable the user to select what bytes to send.
	An autoincrement check box causes the application to increment the bytes sent
with each report.
	Clicking the Once button causes the application to exchange one set of reports.
	Clicking the Continuous button causes the application to exchange reports periodically
(every 5 seconds).
	
This application was created with Visual C++ 6's AppWizard as a dialog-based application.

Companion firmware and other sample code is available from www.lvr.com.

Thanks to the contributors on Usenet and the excellent archive at 
www.deja.com/usenet for helping me with Visual C++ in this project. Other sources I used were 
www.natsemi.com's example HID application and John Hyde's terminal program at 
www.usb-by-example.com.

The main change from the previous version is the use of overlapped I/O.

Send comments, questions, bug reports, etc. to jan@lvr.com.

About overlapped I/O

Reading HID input reports is done with the API function ReadFile. 
Non-overlapped ReadFile is a blocking call. If the device doesn't return the
expected amount of data, the application hangs and will wait forever.

With overlapped I/O, the call to ReadFile returns immediately. The application then calls 
WaitForSingleObject, which returns when either the data has arrived or the specified timeout has elapsed.

This application has been tested on Windows 98 SE and Windows 2000.
*/

#include "stdafx.h"

#include "usbhidioc.h"
#include "usbhidiocDlg.h"

#include <wtypes.h>
#include <initguid.h>

#define MAX_LOADSTRING 256

extern "C" {
#include "hidsdi.h"
#include <setupapi.h>
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//function prototypes
bool FindTheHID();
void DisplayCurrentTime();
void DisplayData(CString cstrDataToDisplay);
void DisplayInputReport();
void DisplayLastError(CString Operation);
void DisplayReceivedData(char ReceivedByte);
void GetDeviceCapabilities();
void PrepareForOverlappedTransfer();
void ReadAndWriteToDevice();
void ReadReport();
void ScrollToBottomOfListBox(USHORT Index);
void WriteReport();

extern void WINAPI HIDIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsbhidiocDlg dialog

CUsbhidiocDlg::CUsbhidiocDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbhidiocDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsbhidiocDlg)
	m_AutoIncrement = FALSE;
	m_ResultsString = _T("");
	m_strBytesReceived = _T("");
	m_strByteToSend0 = _T("");
	m_strByteToSend1 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


//Application global variables 
	DWORD			ActualBytesRead;
	DWORD			BytesRead;
	HIDP_CAPS		Capabilities;
	DWORD			cbBytesRead;
	PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;
	bool			DeviceDetected;
	HANDLE			DeviceHandle;
	DWORD			dwError;
	HANDLE			hEventObject;
	HANDLE			hDevInfo;
	GUID			HidGuid;
	OVERLAPPED		HIDOverlapped;
	char			InputReport[3];
	ULONG			Length;
	LPOVERLAPPED	lpOverLap;
	DWORD			NumberOfBytesRead;
	HANDLE			ReadHandle;
	ULONG			Required;
	CString			ValueToDisplay;

void CUsbhidiocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsbhidiocDlg)
	DDX_Control(pDX, IDC_Continuous, m_Continuous);
	DDX_Control(pDX, IDC_AutoIncrement, m_cbutAutoIncrement);
	DDX_Control(pDX, IDC_cboByteToSend1, m_cboByteToSend1);
	DDX_Control(pDX, IDC_cboByteToSend0, m_cboByteToSend0);
	DDX_Control(pDX, IDC_lstBytesReceived, m_BytesReceived);
	DDX_Control(pDX, IDC_LIST2, m_ResultsList);
	DDX_Control(pDX, IDC_Once, m_Once);
	DDX_Check(pDX, IDC_AutoIncrement, m_AutoIncrement);
	DDX_LBString(pDX, IDC_LIST2, m_ResultsString);
	DDX_LBString(pDX, IDC_lstBytesReceived, m_strBytesReceived);
	DDX_CBString(pDX, IDC_cboByteToSend0, m_strByteToSend0);
	DDX_CBString(pDX, IDC_cboByteToSend1, m_strByteToSend1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUsbhidiocDlg, CDialog)
	//{{AFX_MSG_MAP(CUsbhidiocDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Once, OnOnce)
	ON_EN_CHANGE(IDC_Results, OnChangeResults)
	ON_BN_CLICKED(IDC_Continuous, OnContinuous)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsbhidiocDlg message handlers

BOOL CUsbhidiocDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//My declares begin here
	int		ByteToSend =0;
	CString	strByteToSend = "";
	CString	strComboBoxText="";
	//End my declares

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
	
	/*
	My code begins here.
	Anything that needs to happen when the application starts goes in this routine.
	*/

	//Populate the combo boxes with values from 00 to FF.
	DeviceDetected=FALSE;

	for (ByteToSend=0; ByteToSend < 256; ByteToSend++)
	{
		//Display the value as a 2-digit Hex value.
		strByteToSend.Format("%.2X",ByteToSend);
		//Add the value to the combo boxes.
		m_cboByteToSend0.AddString(strByteToSend);
		m_cboByteToSend1.AddString(strByteToSend);	
	}

	//Select default values for the combo boxes.
	m_cboByteToSend0.SetCurSel(0);
	m_cboByteToSend1.SetCurSel(128);

	//Check the autoincrement check box.
	m_cbutAutoIncrement.SetCheck(1);

	//Set the caption for the Continous button.
	m_Continuous.SetWindowText("Continuous");
	//end my code

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUsbhidiocDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUsbhidiocDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUsbhidiocDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*
My routines (routines specific to this application) start here.
*/

void CUsbhidiocDlg::OnOnce() 
{
	//Click the Once button to read and write one pair of reports.
	ReadAndWriteToDevice();
}

void CUsbhidiocDlg::OnContinuous() 
{
	//Click the Continuous button to
	//begin or stop requesting and sending periodic reports.
	CString Caption;

	//Find out whether Continuous is currently selected 
	//and take appropriate action.
	m_Continuous.GetWindowText(Caption);
	if (Caption == "Continuous")
	{
		//Enable periodic exchanges of reports.
		//Change the button caption.
		m_Continuous.SetWindowText("Stop Continuous");
		//Start by reading and writing one pair of reports.
		ReadAndWriteToDevice();
		//Enable the timer to cause periodic exchange of reports.
		//The second parameter is the number of milliseconds between report requests.
		SetTimer(ID_CLOCK_TIMER, 5000, NULL);
	}
	else
	{
		//Stop periodic exchanges of reports.
		//Change the button caption.
		m_Continuous.SetWindowText("Continuous");
		//Disable the timer.
		KillTimer(ID_CLOCK_TIMER);
	}
}

void CUsbhidiocDlg::OnClose() 
{
	//Anything that needs to occur on closing the application goes here.
	//Free any resources used by previous API calls and still allocated.

	//Close open handles.
	CloseHandle(DeviceHandle);
	DisplayLastError("CloseHandle(DeviceHandle)");

	CloseHandle(ReadHandle);
	DisplayLastError("CloseHandle(ReadHandle)");

	CDialog::OnClose();
}

bool CUsbhidiocDlg::FindTheHID()
{
	//Use a series of API calls to find a HID with a matching Vendor and Product ID.

	HIDD_ATTRIBUTES						Attributes;
	SP_DEVICE_INTERFACE_DATA			devInfoData;
	bool								LastDevice = FALSE;
	int									MemberIndex = 0;
	bool								MyDeviceDetected = FALSE; 
	LONG								Result;

	//These are the vendor and product IDs to look for.
	//Uses Lakeview Research's Vendor ID.
	const unsigned int VendorID = 0xC251;
	const unsigned int ProductID = 0x1301;

	Length = 0;
	detailData = NULL;
	DeviceHandle=NULL;

	/*
	API function: HidD_GetHidGuid
	Get the GUID for all system HIDs.
	Returns: the GUID in HidGuid.
	*/

	HidD_GetHidGuid(&HidGuid);	
	
	/*
	API function: SetupDiGetClassDevs
	Returns: a handle to a device information set for all installed devices.
	Requires: the GUID returned by GetHidGuid.
	*/
	
	hDevInfo=SetupDiGetClassDevs 
		(&HidGuid, 
		NULL, 
		NULL, 
		DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
		
	devInfoData.cbSize = sizeof(devInfoData);

	//Step through the available devices looking for the one we want. 
	//Quit on detecting the desired device or checking all available devices without success.
	MemberIndex = 0;
	LastDevice = FALSE;

	do
	{
		MyDeviceDetected=FALSE;
		
		/*
		API function: SetupDiEnumDeviceInterfaces
		On return, MyDeviceInterfaceData contains the handle to a
		SP_DEVICE_INTERFACE_DATA structure for a detected device.
		Requires:
		The DeviceInfoSet returned in SetupDiGetClassDevs.
		The HidGuid returned in GetHidGuid.
		An index to specify a device.
		*/

		Result=SetupDiEnumDeviceInterfaces 
			(hDevInfo, 
			0, 
			&HidGuid, 
			MemberIndex, 
			&devInfoData);

		if (Result != 0)
		{
			//A device has been detected, so get more information about it.

			/*
			API function: SetupDiGetDeviceInterfaceDetail
			Returns: an SP_DEVICE_INTERFACE_DETAIL_DATA structure
			containing information about a device.
			To retrieve the information, call this function twice.
			The first time returns the size of the structure in Length.
			The second time returns a pointer to the data in DeviceInfoSet.
			Requires:
			A DeviceInfoSet returned by SetupDiGetClassDevs
			The SP_DEVICE_INTERFACE_DATA structure returned by SetupDiEnumDeviceInterfaces.
			
			The final parameter is an optional pointer to an SP_DEV_INFO_DATA structure.
			This application doesn't retrieve or use the structure.			
			If retrieving the structure, set 
			MyDeviceInfoData.cbSize = length of MyDeviceInfoData.
			and pass the structure's address.
			*/
			
			//Get the Length value.
			//The call will return with a "buffer too small" error which can be ignored.
			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				NULL, 
				0, 
				&Length, 
				NULL);

			//Allocate memory for the hDevInfo structure, using the returned Length.
			detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);
			
			//Set cbSize in the detailData structure.
			detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//Call the function again, this time passing it the returned buffer size.
			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				detailData, 
				Length, 
				&Required, 
				NULL);

			//Open a handle to the device.

			/*
			API function: CreateFile
			Returns: a handle that enables reading and writing to the device.
			Requires:
			The DevicePath in the detailData structure
			returned by SetupDiGetDeviceInterfaceDetail.
			*/

			DeviceHandle=CreateFile 
				(detailData->DevicePath, 
				GENERIC_READ|GENERIC_WRITE, 
				FILE_SHARE_READ|FILE_SHARE_WRITE, 
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, 
				0, 
				NULL);

			DisplayLastError("CreateFile: ");

			/*
			API function: HidD_GetAttributes
			Requests information from the device.
			Requires: the handle returned by CreateFile.
			Returns: a HIDD_ATTRIBUTES structure containing
			the Vendor ID, Product ID, and Product Version Number.
			Use this information to decide if the detected device is
			the one we're looking for.
			*/

			//Set the Size to the number of bytes in the structure.
			Attributes.Size = sizeof(Attributes);

			Result = HidD_GetAttributes 
				(DeviceHandle, 
				&Attributes);
			
			DisplayLastError("HidD_GetAttributes: ");
			
			//Is it the desired device?
			MyDeviceDetected = FALSE;
			

			if (Attributes.VendorID == VendorID)
			{
				if (Attributes.ProductID == ProductID)
				{
					//Both the Product and Vendor IDs match.
					MyDeviceDetected = TRUE;
					DisplayData("Device detected");
					//Get the device's capablities.
					GetDeviceCapabilities();
					PrepareForOverlappedTransfer();
				} //if (Attributes.ProductID == ProductID)

				else
					//The Product ID doesn't match.
					CloseHandle(DeviceHandle);
			} //if (Attributes.VendorID == VendorID)

			else
				//The Vendor ID doesn't match.
				CloseHandle(DeviceHandle);

		//Free the memory used by the detailData structure (no longer needed).
		free(detailData);
		}  //if (Result != 0)

		else
			//SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
			LastDevice=TRUE;

		//If we haven't found the device yet, and haven't tried every available device,
		//try the next one.
		MemberIndex = MemberIndex + 1;

	} //do
	while ((LastDevice == FALSE) && (MyDeviceDetected == FALSE));

	if (MyDeviceDetected == FALSE)
		DisplayData("Device not detected");
	else
		DisplayData("Device detected");

	//Free the memory reserved for hDevInfo by SetupDiClassDevs.
	SetupDiDestroyDeviceInfoList(hDevInfo);
	DisplayLastError("SetupDiDestroyDeviceInfoList");

	return MyDeviceDetected;
}

void CUsbhidiocDlg::GetDeviceCapabilities()
{
	//Get the Capabilities structure for the device.
	PHIDP_PREPARSED_DATA	PreparsedData;

	/*
	API function: HidD_GetPreparsedData
	Returns: a pointer to a buffer containing the information about the device's capabilities.
	Requires: A handle returned by CreateFile.
	There's no need to access the buffer directly,
	but HidP_GetCaps and other API functions require a pointer to the buffer.
	*/

	HidD_GetPreparsedData 
		(DeviceHandle, 
		&PreparsedData);
	DisplayLastError("HidD_GetPreparsedData: ");

	/*
	API function: HidP_GetCaps
	Learn the device's capabilities.
	For standard devices such as joysticks, you can find out the specific
	capabilities of the device.
	For a custom device, the software will probably know what the device is capable of,
	and the call only verifies the information.
	Requires: the pointer to the buffer returned by HidD_GetPreparsedData.
	Returns: a Capabilities structure containing the information.
	*/
	
	HidP_GetCaps 
		(PreparsedData, 
		&Capabilities);
	DisplayLastError("HidP_GetCaps: ");

	//Display the capabilities

	ValueToDisplay.Format("%s%X", "Usage Page: ", Capabilities.UsagePage);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Input Report Byte Length: ", Capabilities.InputReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Output Report Byte Length: ", Capabilities.OutputReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Feature Report Byte Length: ", Capabilities.FeatureReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Link Collection Nodes: ", Capabilities.NumberLinkCollectionNodes);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Input Button Caps: ", Capabilities.NumberInputButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of InputValue Caps: ", Capabilities.NumberInputValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of InputData Indices: ", Capabilities.NumberInputDataIndices);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Output Button Caps: ", Capabilities.NumberOutputButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Output Value Caps: ", Capabilities.NumberOutputValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Output Data Indices: ", Capabilities.NumberOutputDataIndices);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Feature Button Caps: ", Capabilities.NumberFeatureButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Feature Value Caps: ", Capabilities.NumberFeatureValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Feature Data Indices: ", Capabilities.NumberFeatureDataIndices);
	DisplayData(ValueToDisplay);

	//No need for PreparsedData any more, so free the memory it's using.
	HidD_FreePreparsedData(PreparsedData);
	DisplayLastError("HidD_FreePreparsedData: ") ;
}

void CUsbhidiocDlg::PrepareForOverlappedTransfer()
{
	//Get another handle to the device for the overlapped ReadFiles.
	ReadHandle=CreateFile 
		(detailData->DevicePath, 
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL, 
		OPEN_EXISTING, 
		FILE_FLAG_OVERLAPPED, 
		NULL);
	DisplayLastError("CreateFile (ReadHandle): ");

	//Get an event object for the overlapped structure.

	/*API function: CreateEvent
	Requires:
	  Security attributes or Null
	  Manual reset (true). Use ResetEvent to set the event object's state to non-signaled.
	  Initial state (true = signaled) 
	  Event object name (optional)
	Returns: a handle to the event object
	*/

	if (hEventObject == 0)
	{
		hEventObject = CreateEvent 
			(NULL, 
			TRUE, 
			TRUE, 
			"");
	DisplayLastError("CreateEvent: ") ;

	//Set the members of the overlapped structure.
	HIDOverlapped.hEvent = hEventObject;
	HIDOverlapped.Offset = 0;
	HIDOverlapped.OffsetHigh = 0;
	}
}

void CUsbhidiocDlg::ReadAndWriteToDevice()
{
	//If we haven't done so already, find the device and learn its capabilities.
	//Then send a report and request a report.
	//The test device firmware (usbhidio) adds 1 to each byte received in an OUT report
	//and sends the results back in the next IN report.

	//Clear the List Box (optional).
	//m_ResultsList.ResetContent();

	{
	DisplayData("***HID Test Report***");
	DisplayCurrentTime();

	//If the device hasn't been detected already, look for it.
	if (DeviceDetected==FALSE)
		DeviceDetected=FindTheHID();
	
	if (DeviceDetected==TRUE)
		{
			//Write a report to the device.
			WriteReport();
			//Read a report from the device.
			ReadReport();
		} 
	}
}

void CUsbhidiocDlg::ReadReport()
{
	CString	ByteToDisplay = "";
	CString	MessageToDisplay = "";
	DWORD	Result;
	
	//Read a report from the device.
 	
	/*API call:ReadFile
	'Returns: the report in InputReport.
	'Requires: a device handle returned by CreateFile
	'(for overlapped I/O, CreateFile must be called with FILE_FLAG_OVERLAPPED),
	'the Input report length in bytes returned by HidP_GetCaps,
	'and an overlapped structure whose hEvent member is set to an event object.
	*/

	Result = ReadFile 
		(ReadHandle, 
		InputReport, 
		Capabilities.InputReportByteLength, 
		&NumberOfBytesRead,
		(LPOVERLAPPED) &HIDOverlapped); 
 
	DisplayLastError("ReadFile: ") ;

	/*API call:WaitForSingleObject
	'Used with overlapped ReadFile.
	'Returns when ReadFile has received the requested amount of data or on timeout.
	'Requires an event object created with CreateEvent
	'and a timeout value in milliseconds.
	*/

	Result = WaitForSingleObject 
		(hEventObject, 
		6000);
	DisplayLastError("WaitForSingleObject: ") ;
 
	switch (Result)
	{
	case WAIT_OBJECT_0:
		{
		ValueToDisplay.Format("%s", "ReadFile Completed");
		DisplayData(ValueToDisplay);
		break;
		}
	case WAIT_TIMEOUT:
		{
		ValueToDisplay.Format("%s", "ReadFile timeout");
		DisplayData(ValueToDisplay);
		//Cancel the Read operation.

		/*API call: CancelIo
		Cancels the ReadFile
        Requires the device handle.
        Returns non-zero on success.
		*/
		
		Result = CancelIo(ReadHandle);
		
		//A timeout may mean that the device has been removed. 
		//Close the device handles and set DeviceDetected = False 
		//so the next access attempt will search for the device.
		CloseHandle(ReadHandle);
		CloseHandle(DeviceHandle);
		DisplayData("Can't read from device");
		DeviceDetected = FALSE;
		break;
	default:
		ValueToDisplay.Format("%s", "Undefined error");
		break;
		}
	}

	/*
	API call: ResetEvent
	Sets the event object to non-signaled.
	Requires a handle to the event object.
	Returns non-zero on success.
	*/

	ResetEvent(hEventObject);

	//Display the report data.
	DisplayInputReport();
}

void CUsbhidiocDlg::WriteReport()
{
	//Send a report to the device.

	//The maximum size of an output report. (This can be increased).
	const unsigned short int	MAXREPORTSIZE = 256;

	DWORD	BytesWritten = 0;
	INT		Index =0;
	CHAR	OutputReport[MAXREPORTSIZE];
	ULONG	Result;
	CString	strBytesWritten = "";

	//The first byte is the report number.
	OutputReport[0]=0;

	//Can set the other report values here, or get them from the combo boxes.
	//OutputReport[1]=33;
	//OutputReport[2]=6;
	//OutputReport[3]=15;

	//Get the bytes to send from the combo boxes.
	
	//If Autoincrement is checked, increment the selection.
	if (m_cbutAutoIncrement.GetCheck()>0)
	{
		Index=m_cboByteToSend0.GetCurSel();
		Index=Index+1;
		m_cboByteToSend0.SetCurSel(Index);
	}

	if (m_cbutAutoIncrement.GetCheck()>0)
	{
		Index=m_cboByteToSend1.GetCurSel();
		Index=Index+1;
		m_cboByteToSend1.SetCurSel(Index);
	}

	//Get the values from the combo boxes.
	OutputReport[1]=m_cboByteToSend0.GetCurSel();
	OutputReport[2]=m_cboByteToSend1.GetCurSel();

	/*
	API Function: WriteFile
	Sends a report to the device.
	Returns: success or failure.
	Requires:
	The device handle returned by CreateFile.
	The Output Report length returned by HidP_GetCaps,
	A report to send.
	*/

	Result = WriteFile 
		(DeviceHandle, 
		OutputReport, 
		Capabilities.OutputReportByteLength, 
		&BytesWritten, 
		NULL);

	if (Result == 0)
	{
		//The WriteFile failed, so close the handle, display a message,
		//and set DeviceDetected to FALSE so the next attempt will look for the device.
		CloseHandle(DeviceHandle);
		CloseHandle(ReadHandle);
		DisplayData("Can't write to device");
		DeviceDetected = FALSE;
	}
	
	//Display the result of the API call and the report bytes.
	DisplayLastError("WriteFile: ");
	strBytesWritten.Format("%s%d", "Bytes Written: ", BytesWritten); 
	DisplayData(strBytesWritten);
}

/*
Display-related routines
*/

void CUsbhidiocDlg::DisplayInputReport()
{
	USHORT	ByteNumber;
	CHAR	ReceivedByte;
	
	//Display the received data in the log and the Bytes Received List boxes.
	//Start at the top of the List Box.
	m_BytesReceived.ResetContent();
	
	//Step through the received bytes and display each.
	for (ByteNumber=0; ByteNumber < Capabilities.InputReportByteLength; ByteNumber++)
	{
		//Get a byte.
		ReceivedByte = InputReport[ByteNumber];
		//Display it.
		DisplayReceivedData(ReceivedByte);
	}
}

void CUsbhidiocDlg::DisplayCurrentTime()
{
	//Get the current time and date and display them in the log List Box.
	CTime curTime = CTime::GetCurrentTime();
	CString CurrentTime = curTime.Format( "%H:%M:%S, %B %d, %Y" );
	DisplayData(CurrentTime);
}

void CUsbhidiocDlg::DisplayData(CString cstrDataToDisplay)
{
	//Display data in the log List Box
	USHORT	Index;
	Index=m_ResultsList.InsertString(-1, (LPCTSTR)cstrDataToDisplay);
	ScrollToBottomOfListBox(Index);
}

void CUsbhidiocDlg::DisplayLastError(CString Operation)
{
	//Display a message and the last error in the log List Box. 
	LPVOID lpMsgBuf;
	USHORT Index = 0;
	CString	strLastError = "";
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	//Display the last error.
	strLastError = Operation + (LPCTSTR)lpMsgBuf; 
	//Trim CR/LF from the error message.
	strLastError.TrimRight(); 
	Index = m_ResultsList.InsertString(-1, strLastError);
	ScrollToBottomOfListBox(Index);
	LocalFree(lpMsgBuf); 
}

void CUsbhidiocDlg::DisplayReceivedData(char ReceivedByte)
{
	//Display data received from the device.
	CString	strByteRead;
	//Convert the value to a 2-character Cstring.
	strByteRead.Format("%02X", ReceivedByte);
	strByteRead = strByteRead.Right(2); 
	//Display the value in the Bytes Received List Box.
	m_BytesReceived.InsertString(-1, strByteRead);
	//Display the value in the log List Box (optional).
	//MessageToDisplay.Format("%s%s", "Byte 0: ", strByteRead); 
	//DisplayData(MessageToDisplay);
}

void CUsbhidiocDlg::OnChangeResults() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}

void CUsbhidiocDlg::ScrollToBottomOfListBox(USHORT Index)
{
	/* 
	Scroll to the bottom of the list box. 
	To do so, add a line and set it as the current selection,
	possibly scrolling the window.
	Then deselect the line, 
	leaving the list box scrolled to the bottom with nothing selected.
	*/

	m_ResultsList.SetCurSel( Index );
	m_ResultsList.SetCurSel( -1 );
}

/*
Misc. routines.
*/

void CUsbhidiocDlg::OnTimer(UINT nIDEvent) 
{
	//The timer event.
	//Read and Write one pair of reports.
	ReadAndWriteToDevice();

	CDialog::OnTimer(nIDEvent);
}

void CUsbhidiocDlg::OnOK() 
{
	CDialog::OnOK();
}



