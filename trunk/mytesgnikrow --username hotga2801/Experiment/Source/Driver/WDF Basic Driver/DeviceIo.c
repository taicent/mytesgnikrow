
#include "ProtoTypes.h"

#pragma alloc_text(PAGE, EvtDeviceIoDefault)

/******************************************************************************/
/* Default IO request handler                                                 */
/******************************************************************************/
VOID
EvtDeviceIoDefault(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;
  WDFMEMORY requestMem;
  BYTE *bufAddr;
  size_t bufSize;
  size_t i;
  
  KdPrint((__DRIVER_NAME "--> EvtDeviceIoDefault\n"));

  devCtx = GetDeviceContext(WdfIoQueueGetDevice(Queue));

  status = WdfRequestRetrieveInputMemory(Request, &requestMem);
  
  // Get pointer to Input Buffer
  bufAddr = WdfMemoryGetBuffer(requestMem, &bufSize);
  
  KdPrint((__DRIVER_NAME " WdfMemoryGetBuffer return address 0x%08x\n", bufAddr));
  KdPrint((__DRIVER_NAME " Buffer size: 0x%08x (%i bytes)\n", bufSize, bufSize));  
  KdPrint((__DRIVER_NAME " Buffer content:\n"));
  
  for (i = 0; i < bufSize; i++)
  {
	if (i % 32 == 0)
		KdPrint(("\n" __DRIVER_NAME ": "));	
	
	KdPrint(("0x%08x (%i)", bufAddr[i], bufAddr[i]));	
  }
  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "      WdfRequestRetrieveInputMemory failed with status 0x%08x\n", status));
    WdfRequestComplete(Request, status);
    return;
  }

  WdfRequestComplete(Request, status);

  KdPrint((__DRIVER_NAME "<-- EvtDeviceIoDefault\n"));

}
