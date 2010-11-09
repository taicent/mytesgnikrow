
#ifndef __PROTOTYPES_H
#define __PROTOTYPES_H

#pragma warning(push)
#pragma warning(disable:4115)  // named typedef in parenthesis
#pragma warning(disable:4200)  // nameless struct/union
#pragma warning(disable:4201)  // nameless struct/union
#pragma warning(disable:4214)  // bit field types other than int

#define INITGUID

#include <ntddk.h>
#include <wdf.h>

#include "usbdi.h"
#include "wdfusb.h"

#pragma warning(pop)

//
// Driver name for debugger
//
#define __DRIVER_NAME "LEDSCTRL: "

//
// Define the vendor commands supported by our device
//
#define VC_LCD_CLEAR	      		0xD1
#define VC_LCD_SET_TEXT	      		0xD2

#define VC_DAC_MUSIC1	      		0xD3
#define VC_DAC_MUSIC2	      		0xD4
#define VC_DAC_MUSIC_BY_DATA  		0xD5

#define VC_LCD_SET_TEXT_NO_DATA 	0xD6

#define VC_GET_LIGHT_BAR      		0xD7
#define VC_SET_LIGHT_BAR      		0xD8

#define VC_DAC_STOP_MUSIC      		0xD9
#define VC_DAC_SET_VOLUME      		0xDA
// region Add
#define VC_GET_ENCODED_DATA			0xDB


/*Declaration of the device context. we have to declare the type of the
  device context, and an accessor function name that will return to us
  a pointer to the device context.*/
typedef struct _DEVICE_CONTEXT {
  
  // Device interface
  WDFUSBDEVICE      UsbDevice;
  WDFUSBINTERFACE   UsbInterface;
  
  // IO Queues
  WDFQUEUE          IoWriteQueue;
  WDFQUEUE          IoReadQueue;
  WDFQUEUE          IoControlEntryQueue;
  WDFQUEUE          IoControlSerialQueue;
  WDFQUEUE          SwitchChangeRequestQueue;
  
  // Switch state
  BYTE              ActSwitchPack;
  
  // Three pipe for Interrupt, IN and OUT
  WDFUSBPIPE        UsbInterruptPipe;	// Interrupt Pipe
  WDFUSBPIPE        UsbBulkInPipe;		// Bulk IN Pine
  WDFUSBPIPE        UsbBulkOutPipe;		// Bulk OUT Pine 
  
  // Leds array memory
  WDFMEMORY         WdfMemLEDArrayState;// 8 leds, 1 bit for 1 led
  
  // Leds array state
  BYTE              D0LEDArrayState;	
  
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);

/*Declaration of the device context. we have to declare the type of the
  device context, and an accessor function name that will return to us
  a pointer to the device context.*/
  
NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject, 
    IN PUNICODE_STRING  RegistryPath);

NTSTATUS
EvtDeviceAdd(
    IN WDFDRIVER        Driver,
    IN PWDFDEVICE_INIT  DeviceInit
    );

NTSTATUS
EvtDevicePrepareHardware(
    IN WDFDEVICE    Device,
    IN WDFCMRESLIST ResourceList,
    IN WDFCMRESLIST ResourceListTranslated
    );

VOID
EvtDeviceIoRead(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  Length
    );

VOID
EvtDeviceIoWrite(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  Length
    );

VOID
EvtDeviceIoControlEntry(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength,
    IN ULONG  IoControlCode
    );

VOID
EvtDeviceIoControlSerial(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength,
    IN ULONG  IoControlCode
    );

VOID
EvtUsbDeviceInterrupt(
    WDFUSBPIPE  Pipe,
    WDFMEMORY  Buffer,
    size_t  NumBytesTransferred,
    WDFCONTEXT  Context
    );

NTSTATUS
EvtDeviceD0Entry(
    IN WDFDEVICE  Device,
    IN WDF_POWER_DEVICE_STATE  PreviousState
    );

NTSTATUS
EvtDeviceD0Exit(
    IN WDFDEVICE  Device,
    IN WDF_POWER_DEVICE_STATE  TargetState
    );

// Don't have below function
NTSTATUS
EvtDeviceD0ExitPreInterruptsDisabled(
    IN WDFDEVICE  Device,
    IN WDF_POWER_DEVICE_STATE  TargetState
    );

VOID
StopAllPipes(
    IN PDEVICE_CONTEXT DeviceContext
    );

NTSTATUS
StartAllPipes(
    IN PDEVICE_CONTEXT DeviceContext
    );
	
NTSTATUS
InitPowerManagement(
    IN WDFDEVICE  Device,
    IN PDEVICE_CONTEXT Context
    );

VOID
EvtIoReadComplete(
    IN WDFREQUEST  Request,
    IN WDFIOTARGET  Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
    IN WDFCONTEXT  Context
    );

VOID
EvtIoWriteComplete(
    IN WDFREQUEST  Request,
    IN WDFIOTARGET  Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
    IN WDFCONTEXT  Context
    );

NTSTATUS
CreateQueues(
    WDFDEVICE Device,
    PDEVICE_CONTEXT Context
    );

NTSTATUS
ConfigureUsbInterface(
    WDFDEVICE Device,
    PDEVICE_CONTEXT DeviceContext
    );

NTSTATUS
ConfigureUsbPipes(
    PDEVICE_CONTEXT DeviceContext
    );

LPCSTR
PowerName(
    WDF_POWER_DEVICE_STATE PowerState
    );

VOID
IoCtlGetSwitchPack(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );

VOID
IoCtlGetSwitchPackChange(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );

VOID
IoCtlGetUSBConfigDescriptor(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );
	
VOID
IoCtlSetLightBar(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );

VOID
IoCtlGetLightBar(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );

NTSTATUS
llSetLightBar(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    );

NTSTATUS
llGetLightBar(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    );

// New IOCTL
// LCD Clear
VOID
IoCtlLCDClear(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );	

// LCD Set Text
VOID
IoCtlLCDSetText(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );	
	
NTSTATUS
llLCDSetText(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    );
	
// DAC Music 1
VOID
IoCtlDACMusic1(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );	
	
// DAC Music 2
VOID
IoCtlDACMusic2(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );	

// Low Level send control no data
NTSTATUS
llSendControlCode(
    IN PDEVICE_CONTEXT Context,
	IN BYTE ControlCode
    );
	
// DAC Music By Data
VOID
IoCtlDACMusicByData(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );

// LCD Set Text No Data
VOID
IoCtlLCDSetTextNoData(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );
	
// Stop Playing Music
VOID
IoCtlDACStopMusic(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );
	
// Set DAC Voulume
VOID
IoCtlDACSetVolume(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );	
	
// Low Level Set DAC Voulume	
NTSTATUS
llDACSetVolume(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    );
	
// region Add
VOID
IoCtlGetEncodedData(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength
    );	
	
NTSTATUS
llGetEncodedData(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    );
	
#endif