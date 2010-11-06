// eMotionUSBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eMotionUSB.h"
#include "eMotionUSBDlg.h"

#define MAX_LOADSTRING 256

extern "C"
{
// This file is in the Windows DDK available from Microsoft.
#include "hidsdi.h"

#include "setupapi.h"
#include "dbt.h"
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//Application global variables 
//DWORD								ActualBytesRead;
//DWORD								BytesRead;
//DWORD								cbBytesRead;
//DWORD								dwError;
//LPOVERLAPPED						lpOverLap;
HIDP_CAPS							Capabilities;
PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;
HANDLE								DeviceHandle;
HANDLE								ReadHandle;
HANDLE								WriteHandle;
HANDLE								hEventObject;
HANDLE								hDevInfo;
GUID								HidGuid;
OVERLAPPED							HIDOverlapped;
UCHAR								InputReport[256];
UCHAR								OutputReport[256];
ULONG								Length;
bool								isYourDeviceDetected = FALSE; 
CString								YourDevicePathName;
ULONG								Required;
CString								ValueToDisplay;

bool								isFirst = TRUE; 
WCHAR								wbuf[MAX_LOADSTRING];


//These are the vendor and product IDs to look for.
//Uses Lakeview Research's Vendor ID.

//int VendorID = 0x15ca; // IBM
//int ProductID = 0x00c3;
//int VendorID = 0x192f; //mitsumi
//int ProductID = 0x0116;
//int VendorID = 0x15d9; //sony
//int ProductID = 0x0a33;
int VendorID = 0x0810; //joy
int ProductID = 0x0001;



// CeMotionUSBDlg dialog

bool CeMotionUSBDlg::HID_FindYourDevice(bool message)
{
	//Use a series of API calls to find a HID with a specified Vendor IF and Product ID.

	HIDD_ATTRIBUTES						Attributes;
	DWORD								DeviceUsage;
	SP_DEVICE_INTERFACE_DATA			devInfoData;
	bool								LastDevice = FALSE;
	int									MemberIndex = 0;
	LONG								Result;	
	CString								UsageDescription;
	
	Length = 0;
	detailData = NULL;
	DeviceHandle = NULL;

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
	
	hDevInfo = SetupDiGetClassDevs(&HidGuid, 
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
		/*
		API function: SetupDiEnumDeviceInterfaces
		On return, MyDeviceInterfaceData contains the handle to a
		SP_DEVICE_INTERFACE_DATA structure for a detected device.
		Requires:
		The DeviceInfoSet returned in SetupDiGetClassDevs.
		The HidGuid returned in GetHidGuid.
		An index to specify a device.
		*/

		Result = SetupDiEnumDeviceInterfaces(hDevInfo, 
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

			Result = SetupDiGetDeviceInterfaceDetail(hDevInfo, 
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

			Result = SetupDiGetDeviceInterfaceDetail(hDevInfo, 
													 &devInfoData, 
													 detailData, 
													 Length, 
													 &Required, 
													 NULL);

			// Open a handle to the device.
			// To enable retrieving information about a system mouse or keyboard,
			// don't request Read or Write access for this handle.

			/*
			API function: CreateFile
			Returns: a handle that enables reading and writing to the device.
			Requires:
			The DevicePath in the detailData structure
			returned by SetupDiGetDeviceInterfaceDetail.
			*/

			DeviceHandle = CreateFile(detailData->DevicePath, 
									  0, 
									  FILE_SHARE_READ|FILE_SHARE_WRITE, 
									  (LPSECURITY_ATTRIBUTES)NULL,
									  OPEN_EXISTING, 
									  0, 
									  NULL);

			if (message == TRUE)
				DisplayLastError("CreateFile --> ");

			HidD_GetProductString(DeviceHandle, wbuf, sizeof(wbuf));


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

			Result = HidD_GetAttributes(DeviceHandle,&Attributes);
			
			if (message == TRUE)
				DisplayLastError("HidD_GetAttributes --> ");
			
			//Is it the desired device?
			isYourDeviceDetected = FALSE;

			if (Attributes.VendorID == VendorID)
			{
				if (Attributes.ProductID == ProductID)
				{
					//Both the Vendor ID and Product ID match.

					if (message == FALSE) //If HID_FindDevice() is called by OnChangeDevice()
						return TRUE;

					//DisplayData("eko3");
					isYourDeviceDetected = TRUE;
					YourDevicePathName = detailData->DevicePath;

					//Register to receive device notifications.

					HID_RegisterForDeviceNotifications();

					//Get the device's capablities.

					HID_GetDeviceCapabilities();

					// Find out if the device is a system mouse or keyboard.
					
					DeviceUsage = (Capabilities.UsagePage * 256) + Capabilities.Usage;

					if (DeviceUsage == 0x102)
					{
						UsageDescription = "mouse";
					}
				
					if (DeviceUsage == 0x106)
					{
						UsageDescription = "keyboard";
					}

					if ((DeviceUsage == 0x102) | (DeviceUsage == 0x106)) 
					{
						DisplayData("");
						DisplayData("*************************");
						DisplayData("The device is a system " + UsageDescription + ".");
						DisplayData("Windows 2000 and Windows XP don't allow applications");
						DisplayData("to directly request Input reports from or "); 
						DisplayData("write Output reports to these devices.");
						DisplayData("*************************");
						DisplayData("");
					}

					// Get a handle for writing Output reports.

					WriteHandle = CreateFile(detailData->DevicePath, 
										     GENERIC_WRITE, 
										     FILE_SHARE_READ|FILE_SHARE_WRITE, 
										     (LPSECURITY_ATTRIBUTES)NULL,
										     OPEN_EXISTING, 
										     0, 
										     NULL);

					DisplayLastError("CreateFile --> ");

					// Prepare to read reports using Overlapped I/O.

					HID_PrepareForOverlappedTransfer();

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

	} while ((LastDevice == FALSE) && (isYourDeviceDetected == FALSE));
	//do

	
	//Free the memory reserved for hDevInfo by SetupDiClassDevs.

	SetupDiDestroyDeviceInfoList(hDevInfo);
	if (message == TRUE)
		DisplayLastError("SetupDiDestroyDeviceInfoList --> ");

	return isYourDeviceDetected;
}

void CeMotionUSBDlg::HID_RegisterForDeviceNotifications()
{

	// Request to receive messages when a device is attached or removed.
	// Also see WM_DEVICECHANGE in BEGIN_MESSAGE_MAP(CUsbhidiocDlg, CDialog).

	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;
	HDEVNOTIFY DeviceNotificationHandle;

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	DevBroadcastDeviceInterface.dbcc_classguid = HidGuid;

	DeviceNotificationHandle =
		RegisterDeviceNotification(m_hWnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);
}

void CeMotionUSBDlg::HID_GetDeviceCapabilities()
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

	HidD_GetPreparsedData(DeviceHandle,&PreparsedData);
	DisplayLastError("HidD_GetPreparsedData --> ");

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
	
	HidP_GetCaps(PreparsedData,&Capabilities);
	DisplayLastError("HidP_GetCaps --> ");

	//Display the capabilities

	ValueToDisplay.Format("%s%X", " + Usage Page: ", Capabilities.UsagePage);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Input Report Byte Length: ", Capabilities.InputReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Output Report Byte Length: ", Capabilities.OutputReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Feature Report Byte Length: ", Capabilities.FeatureReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of Link Collection Nodes: ", Capabilities.NumberLinkCollectionNodes);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of Input Button Caps: ", Capabilities.NumberInputButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of InputValue Caps: ", Capabilities.NumberInputValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of InputData Indices: ", Capabilities.NumberInputDataIndices);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of Output Button Caps: ", Capabilities.NumberOutputButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of Output Value Caps: ", Capabilities.NumberOutputValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of Output Data Indices: ", Capabilities.NumberOutputDataIndices);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of Feature Button Caps: ", Capabilities.NumberFeatureButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of Feature Value Caps: ", Capabilities.NumberFeatureValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", " + Number of Feature Data Indices: ", Capabilities.NumberFeatureDataIndices);
	DisplayData(ValueToDisplay);

	//No need for PreparsedData any more, so free the memory it's using.

	HidD_FreePreparsedData(PreparsedData);
	DisplayLastError("HidD_FreePreparsedData --> ");
}

void CeMotionUSBDlg::HID_PrepareForOverlappedTransfer()
{
	//Get a handle to the device for the overlapped ReadFiles.

	ReadHandle = CreateFile(detailData->DevicePath, 
						    GENERIC_READ, 
						    FILE_SHARE_READ|FILE_SHARE_WRITE,
						    (LPSECURITY_ATTRIBUTES)NULL, 
						    OPEN_EXISTING, 
						    FILE_FLAG_OVERLAPPED, 
						    NULL);

	DisplayLastError("CreateFile (ReadHandle) --> ");

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
		hEventObject = CreateEvent(NULL, TRUE, TRUE, "");
		DisplayLastError("CreateEvent --> ");

		//Set the members of the overlapped structure.

		HIDOverlapped.hEvent = hEventObject;
		HIDOverlapped.Offset = 0;
		HIDOverlapped.OffsetHigh = 0;
	}
}

BOOL CeMotionUSBDlg::HID_DeviceNameMatch(LPARAM lParam)
{

	// Compare the device path name of a device recently attached or removed 
	// with the device path name of the device we want to communicate with.

	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

	DisplayData("Your device path: " + YourDevicePathName);

	if (lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) 
	{

		PDEV_BROADCAST_DEVICEINTERFACE lpdbi = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;

		CString DeviceNameString;

		//The dbch_devicetype parameter indicates that the event applies to a device interface.
		//So the structure in LParam is actually a DEV_BROADCAST_INTERFACE structure, 
		//which begins with a DEV_BROADCAST_HDR.

		//The dbcc_name parameter of DevBroadcastDeviceInterface contains the device name. 

		//Compare the name of the newly attached device with the name of the device 
		//the application is accessing (myDevicePathName).

		DeviceNameString = lpdbi->dbcc_name;

		DisplayData("This device path: " + DeviceNameString);

		if ((DeviceNameString.CompareNoCase(YourDevicePathName)) == 0)
		{
			//The name matches.
			return true;
		}
		else
		{
			//It's a different device.
			return false;
		}
	}
	else
	{
		return false;
	}	
}

void CeMotionUSBDlg::HID_ReadInputReport()
{

	// Retrieve an Input report from the device.

	ULONG	Result;
	UCHAR	ByteNumber;
	ULONG	NumberOfBytesRead = 0;
	CString ResultToDisplay = "Data received: ";
	CString AByteToString = "";
	
	
	//The first byte is the report number.
	InputReport[0]=0;

	/*API call:ReadFile
	'Returns: the report in InputReport.
	'Requires: a device handle returned by CreateFile
	'(for overlapped I/O, CreateFile must be called with FILE_FLAG_OVERLAPPED),
	'the Input report length in bytes returned by HidP_GetCaps,
	'and an overlapped structure whose hEvent member is set to an event object.
	*/

	if (ReadHandle != INVALID_HANDLE_VALUE)
	{
		Result = ReadFile(ReadHandle, 
						  InputReport, 
						  Capabilities.InputReportByteLength, 
						  &NumberOfBytesRead,
						  (LPOVERLAPPED) &HIDOverlapped);
	}
 
	//DisplayLastError("ReadFile --> ");
	

	/*API call:WaitForSingleObject
	'Used with overlapped ReadFile.
	'Returns when ReadFile has received the requested amount of data or on timeout.
	'Requires an event object created with CreateEvent
	'and a timeout value in milliseconds.
	*/

	Result = WaitForSingleObject(hEventObject, 30);//time??

	//DisplayLastError("WaitForSingleObject --> ");
 
	switch (Result)
	{
		case WAIT_OBJECT_0:
		{
			//DisplayData("Received Input report");						
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
			//Close the device handles and set MyDeviceDetected = False 
			//so the next access attempt will search for the device.
			HID_CloseHandles();
			//DisplayData("Can't read from device");
			//isYourDeviceDetected = FALSE;
			break;
		}
		default:
		{
			ValueToDisplay.Format("%s", "Undefined error");
			DisplayData(ValueToDisplay);
			//Close the device handles and set MyDeviceDetected = False 
			//so the next access attempt will search for the device.

			HID_CloseHandles();
			//DisplayData("Can't read from device");
			//isYourDeviceDetected = FALSE;
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

	if (NumberOfBytesRead > 0)
	{
		for (ByteNumber = 0; ByteNumber < Capabilities.InputReportByteLength; ByteNumber++)
		{
			AByteToString.Format("%02X%s", InputReport[ByteNumber], " ");
			AByteToString = AByteToString.Right(3);
			ResultToDisplay += AByteToString;
		}

		DisplayData(ResultToDisplay);
	}
	
}

void CeMotionUSBDlg::HID_WriteOutputReport()
{
	//Send a report to the device.

	ULONG	NumberOfBytesWritten = 0;
	INT		Index = 0;
	ULONG	Result;
	CString	strBytesWritten = "";

	//The first byte is the report number.
	OutputReport[0]=0;

	//Can set the other report values here, or get them from the combo boxes.
	//OutputReport[1]=33;
	//OutputReport[2]=6;

	//Get the bytes to send from the combo boxes.
	OutputReport[1] = m_cboByte1.GetCurSel();
	OutputReport[2] = m_cboByte2.GetCurSel();
	OutputReport[3] = m_cboByte3.GetCurSel();
	OutputReport[4] = m_cboByte4.GetCurSel();
	OutputReport[5] = m_cboByte5.GetCurSel();
	OutputReport[6] = m_cboByte6.GetCurSel();
	OutputReport[7] = m_cboByte7.GetCurSel();
	OutputReport[8] = m_cboByte8.GetCurSel();
	OutputReport[9] = m_cboByte9.GetCurSel();
	OutputReport[10] = m_cboByte10.GetCurSel();

	/*
	API Function: WriteFile
	Sends a report to the device.
	Returns: success or failure.
	Requires:
	A device handle returned by CreateFile.
	A buffer that holds the report.
	The Output Report length returned by HidP_GetCaps,
	A variable to hold the number of bytes written.
	*/

	if (WriteHandle != INVALID_HANDLE_VALUE)
	{
		Result = WriteFile(WriteHandle, 
						   OutputReport, 
						   Capabilities.OutputReportByteLength, 
						   &NumberOfBytesWritten, 
						   NULL);
	}

	//Display the result of the API call and the report bytes.
	//DisplayLastError("WriteFile: ");

	if (!Result)
	{
		//The WriteFile failed, so close the handles, display a message,
		//and set MyDeviceDetected to FALSE so the next attempt will look for the device.

		HID_CloseHandles();
		DisplayData("Can't write to device");
		//MyDeviceDetected = FALSE;
	}
	else
	{
		DisplayData("An Output report was written to the device.");
		strBytesWritten.Format("%s%d", "Bytes Written: ", NumberOfBytesWritten); 
		DisplayData(strBytesWritten);
	}

}

void CeMotionUSBDlg::HID_CloseHandles()
{
	//Close open handles.

	if (DeviceHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(DeviceHandle);
	}

	if (ReadHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(ReadHandle);
	}

	if (WriteHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(WriteHandle);
	}
}


void CeMotionUSBDlg::ScrollToBottomOfListBox(USHORT index)
{
	/* 
	Scroll to the bottom of the list box. 
	To do so, add a line and set it as the current selection,
	possibly scrolling the window.
	Then deselect the line, 
	leaving the list box scrolled to the bottom with nothing selected.
	*/

	m_lstLog.SetCurSel(index);
	m_lstLog.SetCurSel(-1);
}

void CeMotionUSBDlg::DisplayData(CString cstrDataToDisplay)
{
	//Display data in the log List Box

	USHORT	index;
	index = m_lstLog.InsertString(-1, (LPCTSTR)cstrDataToDisplay);
	ScrollToBottomOfListBox(index);
}

void CeMotionUSBDlg::DisplayLastError(CString Operation)
{
	//Display a message and the last error in the log List Box.

	LPVOID lpMsgBuf;
	USHORT Index = 0;
	CString	strLastError = "";
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				  FORMAT_MESSAGE_FROM_SYSTEM | 
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,
				  GetLastError(),
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  (LPTSTR) &lpMsgBuf,
				  0,
				  NULL);

	//Display the last error.

	strLastError = Operation + (LPCTSTR)lpMsgBuf; 

	//Trim CR/LF from the error message.

	strLastError.TrimRight(); 

	Index = m_lstLog.InsertString(-1, strLastError);
	ScrollToBottomOfListBox(Index);
	LocalFree(lpMsgBuf); 
}

CeMotionUSBDlg::CeMotionUSBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CeMotionUSBDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDeviceName = "<None>";
	m_strLog = "";
	m_ReceiveData = TRUE;
	m_SendData = FALSE;
}

void CeMotionUSBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_txtDeviceName, m_txtDeviceName);
	DDX_Text(pDX, IDC_txtDeviceName, m_strDeviceName);
	DDX_Control(pDX, IDC_lstLog, m_lstLog);
	DDX_LBString(pDX, IDC_lstLog, m_strLog);
	DDX_Check(pDX, IDC_chkReceive, m_ReceiveData);
	DDX_Check(pDX, IDC_chkSend, m_SendData);
	DDX_Control(pDX, IDC_btnStart, m_btnStart);

	DDX_Control(pDX, IDC_COMBO1, m_cboByte1);
	DDX_Control(pDX, IDC_COMBO2, m_cboByte2);
	DDX_Control(pDX, IDC_COMBO3, m_cboByte3);
	DDX_Control(pDX, IDC_COMBO4, m_cboByte4);
	DDX_Control(pDX, IDC_COMBO5, m_cboByte5);
	DDX_Control(pDX, IDC_COMBO6, m_cboByte6);
	DDX_Control(pDX, IDC_COMBO7, m_cboByte7);
	DDX_Control(pDX, IDC_COMBO8, m_cboByte8);
	DDX_Control(pDX, IDC_COMBO9, m_cboByte9);
	DDX_Control(pDX, IDC_COMBO10, m_cboByte10);
}

BEGIN_MESSAGE_MAP(CeMotionUSBDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	ON_BN_CLICKED(IDC_chkReceive, On_chkReceiveData)
	ON_BN_CLICKED(IDC_chkSend, On_chkSendData)
	ON_BN_CLICKED(IDC_btnStart, On_btnStart)

	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CeMotionUSBDlg message handlers

BOOL CeMotionUSBDlg::OnInitDialog()
{
	int		ByteToSend = 0;
	CString	strByteToSend = "";

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HID_FindYourDevice(TRUE);
	if (isYourDeviceDetected == FALSE)
	{
		DisplayData("Your device has not been attached!");
	}
	else
	{
		DisplayData("Your device has been attached!");
		m_txtDeviceName.SetWindowText((CString)wbuf);
		GetDlgItem(IDC_btnStart)->EnableWindow(TRUE);
	}

	//Populate the combo boxes with values from 00 to FF.

	for (ByteToSend = 0; ByteToSend < 256; ByteToSend++)
	{
		//Display the value as a 2-digit Hex value.
		strByteToSend.Format("%.2X", ByteToSend);

		//Add the value to the combo boxes.
		m_cboByte1.AddString(strByteToSend);
		m_cboByte2.AddString(strByteToSend);	
		m_cboByte3.AddString(strByteToSend);
		m_cboByte4.AddString(strByteToSend);
		m_cboByte5.AddString(strByteToSend);
		m_cboByte6.AddString(strByteToSend);
		m_cboByte7.AddString(strByteToSend);
		m_cboByte8.AddString(strByteToSend);
		m_cboByte9.AddString(strByteToSend);
		m_cboByte10.AddString(strByteToSend);
	}

	//Select default values for the combo boxes.

	m_cboByte1.SetCurSel(0);
	m_cboByte2.SetCurSel(0);
	m_cboByte3.SetCurSel(0);
	m_cboByte4.SetCurSel(0);
	m_cboByte5.SetCurSel(0);
	m_cboByte6.SetCurSel(128);
	m_cboByte7.SetCurSel(128);
	m_cboByte8.SetCurSel(128);
	m_cboByte9.SetCurSel(128);
	m_cboByte10.SetCurSel(128);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CeMotionUSBDlg::OnPaint()
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
HCURSOR CeMotionUSBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CeMotionUSBDlg::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	
	switch(wParam) 
	{
		// Find out if a device has been attached or removed.
		// If yes, see if the name matches the device path name of the device we want to access.

		case DBT_DEVICEARRIVAL:
			//is your device attached really?
			if (HID_DeviceNameMatch(lParam))
			{
					DisplayData("Your device has been attached!");
					m_txtDeviceName.SetWindowText((CString)wbuf);
					isYourDeviceDetected = TRUE;
					if (m_ReceiveData == TRUE || m_SendData == TRUE)
						GetDlgItem(IDC_btnStart)->EnableWindow(TRUE);
			}
			else
			{
					//it means other device is attached but it's not your device!
					DisplayData("Other device has been attached!");
			}
			
			return TRUE; 

		case DBT_DEVICEREMOVECOMPLETE:
			//is your device removed really?
			if (HID_DeviceNameMatch(lParam))
			{
					DisplayData("Your device has been removed!");
					m_txtDeviceName.SetWindowText("<None>");
					isYourDeviceDetected = FALSE;
					GetDlgItem(IDC_btnStart)->EnableWindow(FALSE);
			}
			else
			{		
					//it means other device is removed but it's not your device!
					DisplayData("Other device has been removed!");
			}

			return TRUE; 

		case 0x07:
			if (isFirst == TRUE)
			{
				if (isYourDeviceDetected == FALSE)
				{
					Sleep(1000);// Very importance
					HID_FindYourDevice(TRUE);
					if (isYourDeviceDetected == TRUE)
					{
						DisplayData("Your device has been attached!");
						m_txtDeviceName.SetWindowText((CString)wbuf);
						if (m_ReceiveData == TRUE || m_SendData == TRUE)
							GetDlgItem(IDC_btnStart)->EnableWindow(TRUE);
					}
					else
					{
						DisplayData("Your device has not been attached!");
					}					
				}
				else
				{
					isFirst = FALSE;
				}
			}
			
			return TRUE;

		default:
			return TRUE; 
	} 
}


void CeMotionUSBDlg::On_chkReceiveData()
{
	m_ReceiveData ^= 1;
	if (m_ReceiveData == FALSE && m_SendData == FALSE)
	{
		GetDlgItem(IDC_btnStart)->EnableWindow(FALSE);
	}
	else
	{
		if (isYourDeviceDetected == TRUE)
			GetDlgItem(IDC_btnStart)->EnableWindow(TRUE);
	}
}

void CeMotionUSBDlg::On_chkSendData()
{	
	m_SendData ^= 1;
	if (m_ReceiveData == FALSE && m_SendData == FALSE)
	{
		GetDlgItem(IDC_btnStart)->EnableWindow(FALSE);
	}
	else
	{
		if (isYourDeviceDetected == TRUE)
			GetDlgItem(IDC_btnStart)->EnableWindow(TRUE);
	}
}

void CeMotionUSBDlg::On_btnStart()
{
	CString Caption;

	//Find out whether Start button is currently selected 
	//and take appropriate action.

	m_btnStart.GetWindowText(Caption);

	if (Caption == "Start")
	{
		//Enable periodic exchanges of reports.
		//Change the button caption.

		m_btnStart.SetWindowText("Stop");

		//Disable all check box
		GetDlgItem(IDC_chkReceive)->EnableWindow(FALSE);
		GetDlgItem(IDC_chkSend)->EnableWindow(FALSE);

		DisplayData("***HID Test Report***");
		//Start by reading and writing one pair of reports.

		//ReadAndWriteToDevice();

		//Enable the timer to cause periodic exchange of reports.
		//The second parameter is the number of milliseconds between report requests.

		SetTimer(ID_CLOCK_TIMER, 100, NULL);
	}
	else
	{
		//Stop periodic exchanges of reports.
		//Change the button caption.

		m_btnStart.SetWindowText("Start");

		//Disable the timer.

		KillTimer(ID_CLOCK_TIMER);

		//Enable all check box
		GetDlgItem(IDC_chkReceive)->EnableWindow(TRUE);
		GetDlgItem(IDC_chkSend)->EnableWindow(TRUE);
	}
}

void CeMotionUSBDlg::OnTimer(UINT nIDEvent) 
{
	//The timer event.
	//Consider all check box to decide: Read or Write or both reports.

	if (m_ReceiveData == TRUE && m_SendData == TRUE)
	{
		//Write a report to the device.
		HID_WriteOutputReport();

		//Read a report from the device.
		HID_ReadInputReport();
	}
	else
	{
		if (m_ReceiveData == TRUE)
		{
			//Read a report from the device.
			HID_ReadInputReport();
		}
		else
		{
			//Write a report to the device.
			HID_WriteOutputReport();
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}


