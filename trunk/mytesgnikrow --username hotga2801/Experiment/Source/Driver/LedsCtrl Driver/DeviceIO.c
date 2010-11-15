
#include <initguid.h>
#include "ProtoTypes.h"
#include "Public.h"

#pragma alloc_text(PAGE, EvtDeviceIoRead)
#pragma alloc_text(PAGE, EvtDeviceIoWrite)
#pragma alloc_text(PAGE, EvtDeviceIoControlEntry)
#pragma alloc_text(PAGE, EvtDeviceIoControlSerial)
#pragma alloc_text(PAGE, EvtIoReadComplete)
#pragma alloc_text(PAGE, EvtIoWriteComplete)

#pragma alloc_text(PAGE, IoCtlGetSwitchPack)
#pragma alloc_text(PAGE, IoCtlGetSwitchPackChange)
#pragma alloc_text(PAGE, IoCtlGetUSBConfigDescriptor)

#pragma alloc_text(PAGE, IoCtlSetLightBar)
#pragma alloc_text(PAGE, IoCtlGetLightBar)

#pragma alloc_text(PAGE, llSetLightBar)			// Low Level
#pragma alloc_text(PAGE, llGetLightBar)			// Low Level

// New Control Code for LPC2300.
// Read Public.h and ProtoType.h to understand all below function
#pragma alloc_text(PAGE, IoCtlLCDClear)
#pragma alloc_text(PAGE, IoCtlLCDSetText)
#pragma alloc_text(PAGE, IoCtlLCDSetTextNoData)
#pragma alloc_text(PAGE, llLCDSetText)			// Low Level

#pragma alloc_text(PAGE, IoCtlDACMusic1)
#pragma alloc_text(PAGE, IoCtlDACMusic2)
#pragma alloc_text(PAGE, IoCtlDACMusicByData)
#pragma alloc_text(PAGE, IoCtlDACStopMusic)
#pragma alloc_text(PAGE, IoCtlDACSetVolume)
#pragma alloc_text(PAGE, llDACSetVolume)		// Low Level

#pragma alloc_text(PAGE, llSendControlCode)		// Low Level

// region Add
#pragma alloc_text(PAGE, IoCtlGetEncodedData)
#pragma alloc_text(PAGE, llGetEncodedData)

/******************************************************************************/
/* Callback function for handling USB interrupts.                             */
/* the switch pack state has to be bit order reversed, and bit value inverted */
/* because the physical representation of the data is not the same as the     */
/* logical representation.                                                    */
/* this function is executed at IRQL = DISPATCH                               */
/* If there is a IO control request queued for a change notification, it is   */
/* completed with the interrupt data. multiple requests can be queued, but    */
/* only the first one is completed.                                           */
/******************************************************************************/
VOID
EvtUsbDeviceInterrupt(
    WDFUSBPIPE  Pipe,
    WDFMEMORY  Buffer,
    size_t  NumBytesTransferred,
    WDFCONTEXT  Context
    )
{
  NTSTATUS status;
  BYTE temp;
  size_t size;
  PDEVICE_CONTEXT devCtx = Context;
  WDFREQUEST Request = NULL;
  
  BYTE *packState = WdfMemoryGetBuffer(Buffer, &size);

  UNREFERENCED_PARAMETER(Pipe);
	// region Add
  UNREFERENCED_PARAMETER(NumBytesTransferred);
  // Check for only 1 byte has sent from Device
  ASSERT(size == sizeof(BYTE));
  ASSERT(NumBytesTransferred == size);
  ASSERT(packState != NULL);
  
  temp = *packState;
  
  // Reverse bit of temp
  temp = (temp & 0x01) << 7 | 
         (temp & 0x02) << 5 | 
         (temp & 0x04) << 3 | 
         (temp & 0x08) << 1 | 
         (temp & 0x10) >> 1 | 
         (temp & 0x20) >> 3 | 
         (temp & 0x40) >> 5 | 
         (temp & 0x80) >> 7;

  KdPrint((__DRIVER_NAME "Converted switch pack from 0x%02x to 0x%02x\n",
    (ULONG)*packState, (ULONG)temp));

  // Save to device context
  devCtx->ActSwitchPack = ~temp;

  /* Is there an io control queued? if so then complete the first one*/
  status = WdfIoQueueRetrieveNextRequest(devCtx->SwitchChangeRequestQueue,
                                         &Request);
										 
  if (NT_SUCCESS(status))
  {
    BYTE* outBuffer;
    status = WdfRequestRetrieveOutputBuffer(Request,
                                            sizeof(BYTE),
                                            &outBuffer,
                                            NULL);

    if (NT_SUCCESS(status))
    {
      /*Do not use the value in the device context, since that may already have
        changed because of a second interrupt while this one was handled.*/
      *outBuffer = temp;
      WdfRequestCompleteWithInformation(Request, status, sizeof(BYTE));
    }
    else
      WdfRequestComplete(Request, status);
    
    KdPrint((__DRIVER_NAME "Completed async pending IOCTL.\n"));
  }
}


/******************************************************************************/
/* Callback function for handling write requests.                             */
/******************************************************************************/
VOID
EvtDeviceIoWrite(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  Length
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  WDFMEMORY requestMem;
  WDFUSBPIPE pipe;
  BYTE *inChar;
  ULONG i;
  size_t length = 0;

  // Get Device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));
  // region Add
  UNREFERENCED_PARAMETER(Length);
  KdPrint((__DRIVER_NAME "Received a write request of %d bytes\n", Length));

  // Get Input Memory to requestMem
  status = WdfRequestRetrieveInputMemory(Request, &requestMem);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfRequestRetrieveInputMemory failed with status 0x%08x\n", status));
    WdfRequestComplete(Request, status);
    return;
  }

  pipe = devCtx->UsbBulkOutPipe;
  
  // For debug
  inChar = WdfMemoryGetBuffer(requestMem, &length);
  for (i = 0; i < length; i++)
	KdPrint((__DRIVER_NAME "    Buffer[%i] = 0x%02x\n", i, (*(inChar + i))));
  // For debug
  
  //
  // Format the request so it can be sent to a USB target.
  //
  status = WdfUsbTargetPipeFormatRequestForWrite(
                                      pipe,
                                      Request,
                                      requestMem,
                                      NULL);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetPipeFormatRequestForWrite failed with status 0x%08x\n", status));
    WdfRequestComplete(Request, status);
    return;
  }
  
  // Set a complete callback rountine
  WdfRequestSetCompletionRoutine(Request,
                                 EvtIoWriteComplete,
                                 pipe);
  //
  // Send the request. If an error occurs, complete the request.
  //
  // Send the request asynchronously
  if(FALSE == WdfRequestSend(Request,
                            WdfUsbTargetPipeGetIoTarget(pipe),
                            WDF_NO_SEND_OPTIONS))
  {
    KdPrint((__DRIVER_NAME "WdfRequestSend failed with status 0x%08x\n", status));
    status = WdfRequestGetStatus(Request);
  }
  else
    return;

  // Complete request
  WdfRequestComplete(Request, status);
}



/******************************************************************************/
/* Callback function for handling read requests                               */
/******************************************************************************/
VOID
EvtDeviceIoRead(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  Length
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  WDFMEMORY requestMem;
  WDFUSBPIPE pipe;

  // Get device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  // region Add
  UNREFERENCED_PARAMETER(Length);
  KdPrint((__DRIVER_NAME "Received a read request of %d bytes\n", Length));

  // Get output WDF memory object
  status = WdfRequestRetrieveOutputMemory(Request, &requestMem);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfRequestRetrieveOutputMemory failed with status 0x%08x\n", status));
    WdfRequestComplete(Request, status);
    return;
  }

  pipe = devCtx->UsbBulkInPipe;
  
  //
  // Format the request so it can be sent to a USB target.
  //
  status = WdfUsbTargetPipeFormatRequestForRead(
                                      pipe,
                                      Request,
                                      requestMem,
                                      NULL);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetPipeFormatRequestForRead failed with status 0x%08x\n", status));
    WdfRequestComplete(Request, status);
    return;
  }
  
  // Set a complete callback rountine
  WdfRequestSetCompletionRoutine(Request,
                                 EvtIoReadComplete,
                                 pipe);

								 
  if(FALSE == WdfRequestSend(Request,
                            WdfUsbTargetPipeGetIoTarget(pipe),
                            WDF_NO_SEND_OPTIONS))
  {
    KdPrint((__DRIVER_NAME "WdfRequestSend failed with status 0x%08x\n", status));
    status = WdfRequestGetStatus(Request);
  }
  else
    return;

  WdfRequestComplete(Request, status);

}


/******************************************************************************/
/* Entry point for all device IO control operations. here the driver will     */
/* determine what has to happen with the request. change requests get put on  */
/* the manual queue. all other requests get passed to the serialized queue.   */
/******************************************************************************/
/* This function use device context item to READ the stage of switch pack LED */
/* and return value to application. So, it doesn't request the switch pack    */
/* from USB Device															  */
/******************************************************************************/
VOID
EvtDeviceIoControlEntry(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength,
    IN ULONG  IoControlCode
    )
{
  switch(IoControlCode)
  {
  case IOCTL_WDF_USB_GET_SWITCHSTATE:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_GET_SWITCHSTATE Event\n"));
    IoCtlGetSwitchPack(Queue, Request, OutputBufferLength, InputBufferLength);
    break;
  case IOCTL_WDF_USB_GET_SWITCHSTATE_CHANGE:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_GET_SWITCHSTATE_CHANGE Event\n"));
    IoCtlGetSwitchPackChange(Queue, Request, OutputBufferLength, InputBufferLength);
    break;
	
  case IOCTL_WDF_USB_GET_CONFIG_DESCRIPTOR:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_GET_CONFIG_DESCRIPTOR Event\n"));
    IoCtlGetUSBConfigDescriptor(Queue, Request, OutputBufferLength, InputBufferLength);
	break;
	
  // Send all another request to serial Queue
  default:
    {
      PDEVICE_CONTEXT devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));
      WdfRequestForwardToIoQueue(Request, devCtx->IoControlSerialQueue);
    }
    break;
  }
}

/******************************************************************************/
/* Callback function for handling serialized device IO control operations.    */
/******************************************************************************/
VOID
EvtDeviceIoControlSerial(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength,
    IN ULONG  IoControlCode
    )
{
  switch(IoControlCode)
  {
  // Set LED Array Bitmap
  case IOCTL_WDF_USB_SET_LIGHTBAR:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_SET_LIGHTBAR Event\n"));
    IoCtlSetLightBar(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // Get LED Array Bitmap
  case IOCTL_WDF_USB_GET_LIGHTBAR:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_GET_LIGHTBAR Event\n"));
    IoCtlGetLightBar(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // LCD Clear
  case IOCTL_WDF_USB_LCD_CLEAR:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_LCD_CLEAR Event\n"));
    IoCtlLCDClear(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // LCD Set Text
  case IOCTL_WDF_USB_LCD_SET_TEXT:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_LCD_SET_TEXT Event\n"));
    IoCtlLCDSetText(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // DAC Music 1
  case IOCTL_WDF_USB_DAC_MUSIC1:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_DAC_MUSIC1 Event\n"));
    IoCtlDACMusic1(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // DAC Music 2
  case IOCTL_WDF_USB_DAC_MUSIC2:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_DAC_MUSIC2 Event\n"));
    IoCtlDACMusic2(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // DAC Music By Data
  case IOCTL_WDF_USB_DAC_MUSIC_BY_DATA:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_DAC_MUSIC_BY_DATA Event\n"));
    IoCtlDACMusicByData(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // LCD Set Text No Data
  case IOCTL_WDF_USB_LCD_SET_TEXT_NO_DATA:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_LCD_SET_TEXT_NO_DATA Event\n"));
    IoCtlLCDSetTextNoData(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // DAC Stop Music
  case IOCTL_WDF_USB_DAC_STOP_MUSIC:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_DAC_STOP_MUSIC Event\n"));
    IoCtlDACStopMusic(Queue, Request, OutputBufferLength, InputBufferLength);
    break;

  // DAC Set Volume
  case IOCTL_WDF_USB_DAC_SET_VOLUME:
    KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_DAC_SET_VOLUME Event\n"));
    IoCtlDACSetVolume(Queue, Request, OutputBufferLength, InputBufferLength);
    break;
	
	// region Add
   case IOCTL_WDF_USB_GET_ENCODED_DATA:
	KdPrint((__DRIVER_NAME "IOCTL_WDF_USB_GET_ENCODED_DATA Event\n"));
	IoCtlGetEncodedData(Queue, Request, OutputBufferLength, InputBufferLength);
	break;
	
  default:
    WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);		// Complete request
    break;
  }
}

/******************************************************************************/
/* Callback function for signalling the completion of a read request.         */
/******************************************************************************/
VOID
EvtIoReadComplete(
    IN WDFREQUEST  Request,
    IN WDFIOTARGET  Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
    IN WDFCONTEXT  Context)
{
  PWDF_USB_REQUEST_COMPLETION_PARAMS usbCompletionParams;

  UNREFERENCED_PARAMETER(Context);
  UNREFERENCED_PARAMETER(Target);

  usbCompletionParams = Params->Parameters.Usb.Completion;

  if(NT_SUCCESS(Params->IoStatus.Status))
  {
    KdPrint((__DRIVER_NAME "Completed the read request with %d bytes\n",
          usbCompletionParams->Parameters.PipeRead.Length));
  }
  else
  {
    KdPrint((__DRIVER_NAME "Failed the read request with status 0x%08x\n",
          Params->IoStatus.Status));
  }
  WdfRequestCompleteWithInformation(Request,
                                    Params->IoStatus.Status,
                                    usbCompletionParams->Parameters.PipeRead.Length);
}

/******************************************************************************/
/* Callback function for signalling the completion of a write request.        */
/******************************************************************************/
VOID
EvtIoWriteComplete(
    IN WDFREQUEST  Request,
    IN WDFIOTARGET  Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
    IN WDFCONTEXT  Context)
{
  PWDF_USB_REQUEST_COMPLETION_PARAMS usbCompletionParams;

  UNREFERENCED_PARAMETER(Context);
  UNREFERENCED_PARAMETER(Target);

  usbCompletionParams = Params->Parameters.Usb.Completion;

  if(NT_SUCCESS(Params->IoStatus.Status))
  {
    KdPrint((__DRIVER_NAME "Completed the write request with %d bytes\n",
          usbCompletionParams->Parameters.PipeWrite.Length));
  }
  else
  {
    KdPrint((__DRIVER_NAME "Failed the read request with status 0x%08x\n",
          Params->IoStatus.Status));
  }
  WdfRequestCompleteWithInformation(Request,
                                    Params->IoStatus.Status,
                                    usbCompletionParams->Parameters.PipeWrite.Length);
}


/******************************************************************************/
/* Return the current value of the switch pack state.                         */
/******************************************************************************/
VOID
IoCtlGetSwitchPack(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  BYTE *outChar = NULL;
  size_t length = 0;
  PDEVICE_CONTEXT devCtx = NULL;

  UNREFERENCED_PARAMETER(InputBufferLength);

  if(OutputBufferLength < sizeof(BYTE))
  {
    KdPrint((__DRIVER_NAME
        "IOCTL_WDF_USB_GET_SWITCHSTATE OutputBufferLength < sizeof(BYTE)\n"));
    WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
    return;
  }

  // Lay con tro bo dem vao outChar
  status = WdfRequestRetrieveOutputBuffer(Request,
                                          sizeof(BYTE),
                                          &outChar,
                                          &length);

  if(NT_SUCCESS(status))
  {
    ASSERT(length >= sizeof(BYTE));
    devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));
    *outChar = devCtx->ActSwitchPack;	// set current value in device context to output buffer
  }

  // Complete request
  WdfRequestCompleteWithInformation(Request, status, sizeof(BYTE));
}


/******************************************************************************/
/* Get a the value of the switch pack, but only when a new value is received. */
/* this means that the IO control request is queued in the manual queue until */
/* it can be completed by the USB interrupt callbcack function.               */
/******************************************************************************/
VOID
IoCtlGetSwitchPackChange(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;

  UNREFERENCED_PARAMETER(InputBufferLength);
  UNREFERENCED_PARAMETER(OutputBufferLength);
  UNREFERENCED_PARAMETER(Queue);

  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  /*If the request is succesfull the request ownership is also transferred
    back to the framework.*/
  status = WdfRequestForwardToIoQueue(Request,
                                      devCtx->SwitchChangeRequestQueue);
  /*if the request cannot be forwarded it has to be completed with
    the appropriate status code because the driver still owns the request.*/
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
        "WdfRequestForwardToIoQueue failed with code 0x%08x.\n", status));
    WdfRequestComplete(Request, status);
  }
}

/******************************************************************************/
/* Get a the value of the USB Device and send all information to application  */
/******************************************************************************/
VOID
IoCtlGetUSBConfigDescriptor(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  PUSB_CONFIGURATION_DESCRIPTOR   configurationDescriptor = NULL;
  USHORT requiredSize = 0;
  size_t bytesReturned = 0;

  // region Add
  UNREFERENCED_PARAMETER(OutputBufferLength);
  UNREFERENCED_PARAMETER(InputBufferLength);
  KdPrint((__DRIVER_NAME
      "IOCTL_WDF_USB_GET_CONFIG_DESCRIPTOR OutputBufferLength %i bytes\n", OutputBufferLength));
	  
  // Get a pointer to device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  //
  // First get the size of the config descriptor
  //
  status = WdfUsbTargetDeviceRetrieveConfigDescriptor(
							devCtx->UsbDevice,
							NULL,
							&requiredSize);

  KdPrint((__DRIVER_NAME
      "RequiredSize is %i bytes\n", requiredSize));
	  
	if (status != STATUS_BUFFER_TOO_SMALL) {
		KdPrint((__DRIVER_NAME
			"WdfUsbTargetDeviceRetrieveConfigDescriptor failed 0x%x\n", status));
		return;
	}
							
  //
  // Get the buffer - make sure the buffer is big enough
  //
  status = WdfRequestRetrieveOutputBuffer(Request,
                                          (size_t)requiredSize,
                                          &configurationDescriptor,
                                          NULL);
										 
										 
	if(!NT_SUCCESS(status)){
		KdPrint((__DRIVER_NAME
			"WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
		return;
	}

	status = WdfUsbTargetDeviceRetrieveConfigDescriptor(
								devCtx->UsbDevice,
								configurationDescriptor,
								&requiredSize);
								
	if (!NT_SUCCESS(status)) {
		KdPrint((__DRIVER_NAME
		"WdfUsbTargetDeviceRetrieveConfigDescriptor failed 0x%x\n", status));
		return;
	}

	bytesReturned = (size_t)requiredSize;

    // Complete request
    WdfRequestCompleteWithInformation(Request, status, bytesReturned);
}


/******************************************************************************/
/* Set the state of the LEDs in the LED array.                                */
/******************************************************************************/
VOID
IoCtlSetLightBar(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  WDFMEMORY memHandle = NULL;

  UNREFERENCED_PARAMETER(OutputBufferLength);

  if(InputBufferLength < sizeof(BYTE))
  {
    KdPrint((__DRIVER_NAME
        "IOCTL_WDF_USB_SET_LIGHTBAR InputBufferLength < sizeof(BYTE)\n"));
    WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
    return;
  }

  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));
  
  status = WdfRequestRetrieveInputMemory(Request, &memHandle);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the lightbar memory handle\n"));
    WdfRequestComplete(Request, status);
    return;
  }

  // low level set light bar
  status = llSetLightBar(devCtx, memHandle);

  // Complete request
  WdfRequestCompleteWithInformation(Request, status, sizeof(BYTE));
}


/******************************************************************************/
/* Get the state of the LEDs in the LED array.                                */
/******************************************************************************/
VOID
IoCtlGetLightBar(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  WDFMEMORY memHandle = NULL;
  size_t length = 0;
    
  KdPrint((__DRIVER_NAME "entering IoCtlGetLightBar\n"));
  UNREFERENCED_PARAMETER(InputBufferLength);

  if(OutputBufferLength < sizeof(BYTE))
  {
    KdPrint((__DRIVER_NAME
        "IOCTL_WDF_USB_SET_LIGHTBAR OutputBufferLength < sizeof(BYTE)\n"));
    WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
    return;
  }

  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  status = WdfRequestRetrieveOutputMemory(Request, &memHandle);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the lightbar memory handle\n"));
    WdfRequestComplete(Request, status);
    return;
  }

  // low level get light bar
  status = llGetLightBar(devCtx, memHandle);
	    // Get buffer length
  WdfMemoryGetBuffer(memHandle, &length);
  KdPrint((__DRIVER_NAME "Encoded data Length: %i\n", length));
  
  WdfRequestCompleteWithInformation(Request, status, length);
}

// region Add

VOID
IoCtlGetEncodedData(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  WDFMEMORY memHandle = NULL;
  size_t length = 0;
  
  KdPrint((__DRIVER_NAME "entering IoCtlGetEncodedData\n"));
  UNREFERENCED_PARAMETER(InputBufferLength);

  if(OutputBufferLength < sizeof(BYTE))
  {
    KdPrint((__DRIVER_NAME
        "IOCTL_WDF_USB_SET_LIGHTBAR OutputBufferLength < sizeof(BYTE)\n"));
    WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
    return;
  }

  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  status = WdfRequestRetrieveOutputMemory(Request, &memHandle);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the encoded data memory handle\n"));
    WdfRequestComplete(Request, status);
    return;
  }

  // low level get light bar
  status = llGetEncodedData(devCtx, memHandle);
    // Get buffer length
  WdfMemoryGetBuffer(memHandle, &length);
  KdPrint((__DRIVER_NAME "Encoded data Length: %i\n", length));
  
  WdfRequestCompleteWithInformation(Request, status, length);
}

NTSTATUS
llGetEncodedData(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  WDF_USB_CONTROL_SETUP_PACKET controlPacket;
  WDF_MEMORY_DESCRIPTOR memDescriptor;
  BYTE *inChar = NULL;
  size_t length = 0;

  KdPrint((__DRIVER_NAME "Entering llGetEncodedData\n"));

  inChar = WdfMemoryGetBuffer(State, &length);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the encoded data memory pointer\n"));
    return status;
  }
  
  // Check for buffer
  ASSERT(length >= sizeof(BYTE));
  ASSERT(NULL != inChar);

  /* Initialize the descriptor that will be passed to the USB driver*/
  WDF_MEMORY_DESCRIPTOR_INIT_HANDLE(&memDescriptor, State, NULL);

  WDF_USB_CONTROL_SETUP_PACKET_INIT_VENDOR(
                            &controlPacket,	
                            BmRequestDeviceToHost,
                            BmRequestToDevice,			// Recipient: Device
                            VC_GET_ENCODED_DATA,
                            0,
                            0);

  status = WdfUsbTargetDeviceSendControlTransferSynchronously(
                            Context->UsbDevice,
                            WDF_NO_HANDLE,				// Request (Def: NULL)
                            NULL,
                            &controlPacket,
                            &memDescriptor,
                            NULL);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetDeviceSendControlTransferSynchronously failed with status 0x%08x\n",
      status));
    return status;
  }
  
  return status;
}

/******************************************************************************/
/* Clear LCD for LPC2300						                              */
/******************************************************************************/
VOID
IoCtlLCDClear(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
    
  KdPrint((__DRIVER_NAME "Entering IoCtlLCDClear\n"));
  
  // UnReferenced input and output buffer
  UNREFERENCED_PARAMETER(InputBufferLength);
  UNREFERENCED_PARAMETER(OutputBufferLength);

  // Get device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  // Send a VC_LCD_CLEAR setup packet to Device
  status = llSendControlCode(devCtx, VC_LCD_CLEAR);

  //KdPrint((__DRIVER_NAME "WdfRequestCompleteWithInformation for IoCtlLCDClear\n"));
  //WdfRequestCompleteWithInformation(Request, status, 0);
  
  KdPrint((__DRIVER_NAME "WdfRequestComplete for IoCtlLCDClear\n"));
  WdfRequestComplete(Request, status);	// Complete with no information return  
}


/******************************************************************************/
/* Set LCD Text for LPC2300						                              */
/******************************************************************************/
VOID
IoCtlLCDSetText(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  WDFMEMORY memHandle = NULL;
  size_t length;

  UNREFERENCED_PARAMETER(OutputBufferLength);

  // LCD of LPC2300 must be 32 Character
  if(InputBufferLength != 32 * sizeof(BYTE))
  {
    KdPrint((__DRIVER_NAME
        "IOCTL_WDF_USB_SET_LIGHTBAR InputBufferLength != 32 * sizeof(BYTE)\n"));
    WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
    return;
  }

  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));
  
  status = WdfRequestRetrieveInputMemory(Request, &memHandle);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the LCD Text memory handle\n"));
    WdfRequestComplete(Request, status);
    return;
  }

  // low level set light bar
  status = llLCDSetText(devCtx, memHandle);
  
  // Get buffer length
  WdfMemoryGetBuffer(memHandle, &length);
  KdPrint((__DRIVER_NAME "LCD Text Length: %i\n", length));
  
  // Complete request
  WdfRequestCompleteWithInformation(Request, status, length);
}

/******************************************************************************/
/* Low level USB function that sets LCD Text.					              */
/******************************************************************************/
NTSTATUS
llLCDSetText(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  WDF_USB_CONTROL_SETUP_PACKET controlPacket;	// Control packet to send to IO Control Pipe
  WDF_MEMORY_DESCRIPTOR memDescriptor;
  BYTE *inChar = NULL;
  size_t length = 0;
  ULONG i;

  KdPrint((__DRIVER_NAME "Entering llLCDSetText\n"));
  
  // Point to Input buffer
  inChar = WdfMemoryGetBuffer(State, &length);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the LCD Text memory pointer\n"));
    return status;
  }

  // Check Input Length
  //ASSERT(length >= 32 * sizeof(BYTE));
  
  // For debug
  KdPrint((__DRIVER_NAME "LCD Text Buffer (Before)\n"));
  for (i = 0; i < length; i++)
	KdPrint((__DRIVER_NAME "   LCD_Buffer[%i] = 0x%02x\n", i, (*(inChar + i))));
  // For debug

  WDF_MEMORY_DESCRIPTOR_INIT_HANDLE(&memDescriptor, State, NULL);

  WDF_USB_CONTROL_SETUP_PACKET_INIT_VENDOR(
                            &controlPacket,				// Packet
                            BmRequestHostToDevice,		// Direction: Host to Device
                            BmRequestToDevice,			// Recipient: Device
                            VC_LCD_SET_TEXT,			// Control Code (Packet.bRequest)
                            0,							// Packet.wValue.Value 
                            0);							// Packet.wIndex.Value 

  status = WdfUsbTargetDeviceSendControlTransferSynchronously(
                            Context->UsbDevice,			// UsbDevice 
                            WDF_NO_HANDLE,				// Request (Def: NULL)
                            NULL,						// RequestOptions 
                            &controlPacket,				// SetupPacket 
                            &memDescriptor,				// MemoryDescriptor 
                            NULL);						// BytesTransferred  
							
  // For debug
  KdPrint((__DRIVER_NAME "LCD Text Buffer (After)\n"));
  for (i = 0; i < length; i++)
	KdPrint((__DRIVER_NAME "   LCD_Buffer[%i] = 0x%02x\n", i, (*(inChar + i))));
  // For debug
  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetDeviceSendControlTransferSynchronously failed with status 0x%08x\n",
      status));
    return status;
  }

  return status;
}

/******************************************************************************/
/* DAC Music1 for LPC2300						                              */
/******************************************************************************/
VOID
IoCtlDACMusic1(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
    
  KdPrint((__DRIVER_NAME "Entering IoCtlDACMusic1\n"));
  
  // UnReferenced input and output buffer
  UNREFERENCED_PARAMETER(InputBufferLength);
  UNREFERENCED_PARAMETER(OutputBufferLength);

  // Get device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  // Send a VC_DAC_MUSIC1 setup packet to Device
  status = llSendControlCode(devCtx, VC_DAC_MUSIC1);

  //WdfRequestCompleteWithInformation(Request, status, 0);
  
  KdPrint((__DRIVER_NAME "WdfRequestComplete for IoCtlDACMusic1\n"));
  WdfRequestComplete(Request, status);	// Complete with no information return 
}

/******************************************************************************/
/* DAC Music2 for LPC2300						                              */
/******************************************************************************/
VOID
IoCtlDACMusic2(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
    
  KdPrint((__DRIVER_NAME "Entering IoCtlDACMusic2\n"));
  
  // UnReferenced input and output buffer
  UNREFERENCED_PARAMETER(InputBufferLength);
  UNREFERENCED_PARAMETER(OutputBufferLength);

  // Get device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  // Send a VC_DAC_MUSIC2 setup packet to Device
  status = llSendControlCode(devCtx, VC_DAC_MUSIC2);

  //WdfRequestCompleteWithInformation(Request, status, 0);
  KdPrint((__DRIVER_NAME "WdfRequestComplete for IoCtlDACMusic2\n"));
  WdfRequestComplete(Request, status);	// Complete with no information return 
}

/******************************************************************************/
/* DAC Music By Data for LPC2300				                              */
/******************************************************************************/
VOID
IoCtlDACMusicByData(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
    
  KdPrint((__DRIVER_NAME "Entering IoCtlDACMusicByData\n"));
  
  // UnReferenced input and output buffer
  UNREFERENCED_PARAMETER(InputBufferLength);
  UNREFERENCED_PARAMETER(OutputBufferLength);

  // Get device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  // Send a VC_DAC_MUSIC1 setup packet to Device
  status = llSendControlCode(devCtx, VC_DAC_MUSIC_BY_DATA);

  //WdfRequestCompleteWithInformation(Request, status, 0);
  KdPrint((__DRIVER_NAME "WdfRequestComplete for IoCtlDACMusicByData\n"));
  WdfRequestComplete(Request, status);	// Complete with no information return 
}

/******************************************************************************/
/* Low level USB function that sets the state of the LED array.               */
/******************************************************************************/
NTSTATUS
llSetLightBar(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  WDF_USB_CONTROL_SETUP_PACKET controlPacket;	// Control packet to send to IO Control Pipe
  WDF_MEMORY_DESCRIPTOR memDescriptor;
  BYTE *inChar = NULL;
  size_t length = 0;

  KdPrint((__DRIVER_NAME "Entering llSetLightBar\n"));
  
  inChar = WdfMemoryGetBuffer(State, &length);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the lightbar memory pointer\n"));
    return status;
  }

  ASSERT(length >= sizeof(BYTE));

  KdPrint((__DRIVER_NAME "LED SwitchPack (Before): 0x%02x\n", *inChar));
  
  WDF_MEMORY_DESCRIPTOR_INIT_HANDLE(&memDescriptor, State, NULL);

  WDF_USB_CONTROL_SETUP_PACKET_INIT_VENDOR(
                            &controlPacket,				// Packet
                            BmRequestHostToDevice,		// Direction: Host to Device
                            BmRequestToDevice,			// Recipient: Device
                            VC_SET_LIGHT_BAR,			// Control Code (Packet.bRequest)
                            *inChar,					// Packet.wValue.Value 
                            0);							// Packet.wIndex.Value 

  status = WdfUsbTargetDeviceSendControlTransferSynchronously(
                            Context->UsbDevice,			// UsbDevice 
                            WDF_NO_HANDLE,				// Request (Def: NULL)
                            NULL,						// RequestOptions 
                            &controlPacket,				// SetupPacket 
                            &memDescriptor,				// MemoryDescriptor 
                            NULL);						// BytesTransferred  
							
  KdPrint((__DRIVER_NAME "LED SwitchPack (After): 0x%02x\n", *inChar));
  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetDeviceSendControlTransferSynchronously failed with status 0x%08x\n",
      status));
    return status;
  }

  return status;
}

/******************************************************************************/
/* Low level USB function that retrieves the state of the LED array.          */
/******************************************************************************/
NTSTATUS
llGetLightBar(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  WDF_USB_CONTROL_SETUP_PACKET controlPacket;
  WDF_MEMORY_DESCRIPTOR memDescriptor;
  BYTE *inChar = NULL;
  size_t length = 0;

  KdPrint((__DRIVER_NAME "Entering llGetLightBar\n"));

  inChar = WdfMemoryGetBuffer(State, &length);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the lightbar memory pointer\n"));
    return status;
  }

  // Check for buffer
  ASSERT(length >= sizeof(BYTE));
  ASSERT(NULL != inChar);

  /* Initialize the descriptor that will be passed to the USB driver*/
  WDF_MEMORY_DESCRIPTOR_INIT_HANDLE(&memDescriptor, State, NULL);

  WDF_USB_CONTROL_SETUP_PACKET_INIT_VENDOR(
                            &controlPacket,	
                            BmRequestDeviceToHost,
                            BmRequestToDevice,			// Recipient: Device
                            VC_GET_LIGHT_BAR,
                            0,
                            0);

  status = WdfUsbTargetDeviceSendControlTransferSynchronously(
                            Context->UsbDevice,
                            WDF_NO_HANDLE,				// Request (Def: NULL)
                            NULL,
                            &controlPacket,
                            &memDescriptor,
                            NULL);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetDeviceSendControlTransferSynchronouslyfailed with status 0x%08x\n",
      status));
    return status;
  }
  
  return status;
}

/******************************************************************************/
/* Low Level send control code no data											  */
/******************************************************************************/
NTSTATUS
llSendControlCode(
    IN PDEVICE_CONTEXT Context,
	IN BYTE ControlCode
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  WDF_USB_CONTROL_SETUP_PACKET controlPacket;	// Control packet to send to IO Control Pipe

  KdPrint((__DRIVER_NAME "Entering llSendControlCode\n"));
  KdPrint((__DRIVER_NAME "Control Code: 0x%02x\n", ControlCode));
  
  // Init control setup packet
  KdPrint((__DRIVER_NAME "Init SETUP_PACKET...\n", ControlCode));
  WDF_USB_CONTROL_SETUP_PACKET_INIT_VENDOR(
                            &controlPacket,				// Packet
                            BmRequestHostToDevice,		// Direction: Host to Device
                            BmRequestToDevice,			// Recipient: Device
                            ControlCode,				// Control Code (Packet.bRequest)
                            0,							// Packet.wValue.Value 
                            0);							// Packet.wIndex.Value 

  KdPrint((__DRIVER_NAME "Sending SETUP_PACKET...\n", ControlCode));
  status = WdfUsbTargetDeviceSendControlTransferSynchronously(
                            Context->UsbDevice,			// UsbDevice 
                            WDF_NO_HANDLE,				// Request (Def: NULL)
                            NULL,						// RequestOptions 
                            &controlPacket,				// SetupPacket 
                            NULL,						// MemoryDescriptor
                            NULL);						// BytesTransferred  
							
  KdPrint((__DRIVER_NAME "Control Code 0x%02x has sent to device with status 0x%08x\n", ControlCode, status));							
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "[In llSendControlCode] --> WdfUsbTargetDeviceSendControlTransferSynchronously failed with status 0x%08x\n",
      status));
    return status;
  }

  return status;
}

/******************************************************************************/
/* Set LCD Text No Data (Methods 2)				                              */
/******************************************************************************/
VOID
IoCtlLCDSetTextNoData(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
    
  KdPrint((__DRIVER_NAME "Entering IoCtlLCDSetTextNoData\n"));
  
  // UnReferenced input and output buffer
  UNREFERENCED_PARAMETER(InputBufferLength);
  UNREFERENCED_PARAMETER(OutputBufferLength);

  // Get device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  // Send a VC_DAC_MUSIC1 setup packet to Device
  status = llSendControlCode(devCtx, VC_LCD_SET_TEXT_NO_DATA);

  //WdfRequestCompleteWithInformation(Request, status, 0);
  KdPrint((__DRIVER_NAME "WdfRequestComplete for IoCtlLCDSetTextNoData\n"));
  WdfRequestComplete(Request, status);	// Complete with no information return 
}

/******************************************************************************/
/* DAC Stop Music								                              */
/******************************************************************************/
VOID
IoCtlDACStopMusic(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
    
  KdPrint((__DRIVER_NAME "Entering IoCtlDACStopMusic\n"));
  
  // UnReferenced input and output buffer
  UNREFERENCED_PARAMETER(InputBufferLength);
  UNREFERENCED_PARAMETER(OutputBufferLength);

  // Get device context
  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  // Send a VC_DAC_MUSIC1 setup packet to Device
  status = llSendControlCode(devCtx, VC_DAC_STOP_MUSIC);

  //WdfRequestCompleteWithInformation(Request, status, 0);
  KdPrint((__DRIVER_NAME "WdfRequestComplete for IoCtlDACStopMusic\n"));
  WdfRequestComplete(Request, status);	// Complete with no information return 
}

/******************************************************************************/
/* Set DAC Volume								                              */
/******************************************************************************/
VOID
IoCtlDACSetVolume(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength)
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  WDFMEMORY memHandle = NULL;

  UNREFERENCED_PARAMETER(OutputBufferLength);

  if(InputBufferLength < sizeof(BYTE))
  {
    KdPrint((__DRIVER_NAME
        "IOCTL_WDF_USB_SET_LIGHTBAR InputBufferLength < sizeof(BYTE)\n"));
    WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
    return;
  }

  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));
  
  status = WdfRequestRetrieveInputMemory(Request, &memHandle);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the lightbar memory handle\n"));
    WdfRequestComplete(Request, status);
    return;
  }

  // low level set dac volume
  status = llDACSetVolume(devCtx, memHandle);

  // Complete request
  WdfRequestCompleteWithInformation(Request, status, sizeof(BYTE));
}

/******************************************************************************/
/* Low level USB function that sets DAC Volume. 				              */
/******************************************************************************/
NTSTATUS
llDACSetVolume(
    IN PDEVICE_CONTEXT Context,
    IN WDFMEMORY State
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  WDF_USB_CONTROL_SETUP_PACKET controlPacket;	// Control packet to send to IO Control Pipe
  WDF_MEMORY_DESCRIPTOR memDescriptor;
  BYTE *inChar = NULL;
  size_t length = 0;

  KdPrint((__DRIVER_NAME "Entering llDACSetVolume\n"));
  
  inChar = WdfMemoryGetBuffer(State, &length);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "Could not retrieve the DAC Volume memory pointer\n"));
    return status;
  }

  ASSERT(length >= sizeof(BYTE));

  KdPrint((__DRIVER_NAME "DAC Volume (Before): 0x%02x\n", *inChar));
  
  WDF_MEMORY_DESCRIPTOR_INIT_HANDLE(&memDescriptor, State, NULL);

  WDF_USB_CONTROL_SETUP_PACKET_INIT_VENDOR(
                            &controlPacket,				// Packet
                            BmRequestHostToDevice,		// Direction: Host to Device
                            BmRequestToDevice,			// Recipient: Device
                            VC_DAC_SET_VOLUME,			// Control Code (Packet.bRequest)
                            *inChar,					// Packet.wValue.Value 
                            0);							// Packet.wIndex.Value 

  status = WdfUsbTargetDeviceSendControlTransferSynchronously(
                            Context->UsbDevice,			// UsbDevice 
                            WDF_NO_HANDLE,				// Request (Def: NULL)
                            NULL,						// RequestOptions 
                            &controlPacket,				// SetupPacket 
                            &memDescriptor,				// MemoryDescriptor 
                            NULL);						// BytesTransferred  
							
  KdPrint((__DRIVER_NAME "DAC Volume (After): 0x%02x\n", *inChar));
  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetDeviceSendControlTransferSynchronously failed with status 0x%08x\n",
      status));
    return status;
  }

  return status;
}

