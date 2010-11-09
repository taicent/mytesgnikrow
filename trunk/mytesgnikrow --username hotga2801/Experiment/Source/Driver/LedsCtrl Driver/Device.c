/************************************************************************/
/*  Device.c: All implement for Device function							*/
/************************************************************************/

#include <initguid.h>
#include "ProtoTypes.h"
#include "Public.h"

#pragma alloc_text(PAGE, EvtDevicePrepareHardware)
#pragma alloc_text(PAGE, ConfigureUsbInterface)
#pragma alloc_text(PAGE, ConfigureUsbPipes)
#pragma alloc_text(PAGE, EvtDeviceAdd)
#pragma alloc_text(PAGE, CreateQueues)

/******************************************************************************/
/* The callback function that will be called whenever a new device is added.  */
/* this is for PNP devices.                                                   */
/******************************************************************************/
/* External function: 														  */
/*		- CreateQueues	(in file Device.c, OK) 					  			  */
/*		- EvtDevicePrepareHardware	(in file Device.c, OK) 		  			  */
/*		- EvtDeviceD0Entry	(in file Power.c) 		  					  */
/*		- EvtDeviceD0Exit	(in file Power.c) 		  					  */
/******************************************************************************/
NTSTATUS EvtDeviceAdd(
                      IN WDFDRIVER  Driver,
                      IN PWDFDEVICE_INIT  DeviceInit
                      )
{
  NTSTATUS status;
  WDFDEVICE device;									// Device object
  PDEVICE_CONTEXT devCtx = NULL;					// Pointer to a device context
  WDF_OBJECT_ATTRIBUTES attributes;					// Object attribute
  WDF_PNPPOWER_EVENT_CALLBACKS pnpPowerCallbacks;	// PNP Power event callback
  WDF_DEVICE_PNP_CAPABILITIES pnpCapabilities;		//

  UNREFERENCED_PARAMETER(Driver);

  /*Set the callback functions that will be executed on PNP and Power events*/
  WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);	// Init pnpPowerCallbacks
  pnpPowerCallbacks.EvtDevicePrepareHardware = EvtDevicePrepareHardware;
  pnpPowerCallbacks.EvtDeviceD0Entry = EvtDeviceD0Entry;
  pnpPowerCallbacks.EvtDeviceD0Exit = EvtDeviceD0Exit;
  WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);	// Setup pnpPowerCallbacks

  WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoBuffered);	// Buffered IO type

  /*Initialize storage for the device context*/
  WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT);

  /*Create a device instance.*/
  status = WdfDeviceCreate(&DeviceInit, &attributes, &device);  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfDeviceCreate failed with status 0x%08x\n", status));
    return status;
  }

  /*Set the PNP capabilities of our device. we don't want an annoying
    popup if the device is pulled out of the USB slot.*/
  WDF_DEVICE_PNP_CAPABILITIES_INIT(&pnpCapabilities);
  pnpCapabilities.Removable = WdfTrue;				// Can hot-unplug
  pnpCapabilities.SurpriseRemovalOK = WdfTrue;		//
  WdfDeviceSetPnpCapabilities(device, &pnpCapabilities);
  
  devCtx = GetDeviceContext(device);

  /*Create a WDF memory object for the memory that is occupied by the
    WdfMemLEDArrayState variable in the device context.
    this way we have the value itself handy for debugging purposes, and
    we have a WDF memory handle that can be used for passing to the low
    level USB functions.
    this alleviates the need to getting the buffer at run time.*/
  status = WdfMemoryCreatePreallocated(WDF_NO_OBJECT_ATTRIBUTES,
                           &devCtx->D0LEDArrayState,
                           sizeof(devCtx->D0LEDArrayState),
                           &devCtx->WdfMemLEDArrayState);	// for Debbug 
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfMemoryCreatePreallocated failed with status 0x%08x\n", status));
    return status;
  }

  // Create Queues for handling IO request
  status = CreateQueues(device, devCtx);
  if(!NT_SUCCESS(status))
    return status;

  // Create Device Interface
  status = WdfDeviceCreateDeviceInterface(device, &GUID_DEV_IF_LEDSCTRL, NULL);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfDeviceCreateDeviceInterface failed with status 0x%08x\n", status));
    return status;
  }

  return status;
  
}

/******************************************************************************/
/* Create the different IO queues that will be used by for sending different  */
/* types of requests to our driver.                                           */
/* 	1 Default Queue for IOCTL request                                         */
/* 	1 Sequential Queue to buffer IOCTL request from above queues              */
/* 	1 Sequential Queue for write request						              */
/* 	1 Sequential Queue for read request						                  */
/* 	1 Manual Queus for storing IOCTL_WDF_USB_GET_SWITCHSTATE_CHANGE	          */
/******************************************************************************/
/* External function: 														  */
/*		- EvtDeviceIoControlEntry	(in file DeviceIO.c) 		  			  */
/*		- EvtDeviceIoControlSerial	(in file DeviceIO.c) 		  			  */
/*		- EvtDeviceIoWrite	(in file DeviceIO.c) 				  			  */
/*		- EvtDeviceIoRead	(in file DeviceIO.c) 				  			  */
/******************************************************************************/
NTSTATUS CreateQueues(WDFDEVICE Device, PDEVICE_CONTEXT Context)
{
  NTSTATUS status = STATUS_SUCCESS;

  WDF_IO_QUEUE_CONFIG ioQConfig;

  /*Create the default IO queue. this one will be used for ioctl request entry.
    this queue is parallel, so as to prevent unnecessary serialization for
    IO requests that can be handled in parallel.*/
  WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQConfig,
                                          WdfIoQueueDispatchParallel);
  ioQConfig.EvtIoDeviceControl = EvtDeviceIoControlEntry;
  status = WdfIoQueueCreate(Device,
                            &ioQConfig,
                            WDF_NO_OBJECT_ATTRIBUTES,
                            &Context->IoControlEntryQueue);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfIoQueueCreate failed with status 0x%08x\n", status));
    return status;
  }

  /*Create the IO queue for serialize IO requests. This queue will be filled by
    the IO control entry handler with the requests that have to be serialized
    for execution.*/
  WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig,
                           WdfIoQueueDispatchSequential);
  ioQConfig.EvtIoDeviceControl = EvtDeviceIoControlSerial;
  status = WdfIoQueueCreate(Device,
                            &ioQConfig,
                            WDF_NO_OBJECT_ATTRIBUTES,
                            &Context->IoControlSerialQueue);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfIoQueueCreate failed with status 0x%08x\n", status));
    return status;
  }

  /*Create the IO queue for write requests*/
  WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig,
                           WdfIoQueueDispatchSequential);
  ioQConfig.EvtIoWrite = EvtDeviceIoWrite;
  status = WdfIoQueueCreate(Device,
                            &ioQConfig,
                            WDF_NO_OBJECT_ATTRIBUTES,
                            &Context->IoWriteQueue);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfIoQueueCreate failed with status 0x%08x\n", status));
    return status;
  }

  status  = WdfDeviceConfigureRequestDispatching(Device,
                                                Context->IoWriteQueue,
                                                WdfRequestTypeWrite);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfDeviceConfigureRequestDispatching failed with status 0x%08x\n",
      status));
    return status;
  }

  /*Create the IO queue for read requests*/
  WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig,
                           WdfIoQueueDispatchSequential);
  ioQConfig.EvtIoRead = EvtDeviceIoRead;
  status = WdfIoQueueCreate(Device,
                            &ioQConfig,
                            WDF_NO_OBJECT_ATTRIBUTES,
                            &Context->IoReadQueue);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfIoQueueCreate failed with status 0x%08x\n", status));
    return status;
  }

  status  = WdfDeviceConfigureRequestDispatching(Device,
                                                Context->IoReadQueue,
                                                WdfRequestTypeRead);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfDeviceConfigureRequestDispatching failed with status 0x%08x\n",
      status));
    return status;
  }

  /*Create a manual queue for storing the IOCTL_WDF_USB_GET_SWITCHSTATE_CHANGE
	IO control requests. If a file handle associated with one or more requests
    in the queue is closed, the requests themselves are automatically removed
    from the queue by the framework and cancelled.*/
  WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig,
                           WdfIoQueueDispatchManual);
  status = WdfIoQueueCreate(Device,
                            &ioQConfig,
                            WDF_NO_OBJECT_ATTRIBUTES,
                            &Context->SwitchChangeRequestQueue);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfIoQueueCreate for manual queue failed with status 0x%08x\n", status));
    return status;
  }

  return status;
}

/******************************************************************************/
/* Call-back function that will be called by the pnp/power manager after the  */
/* Plug and Play manager has assigned hardware resources to the device and    */
/* after the device has entered its uninitialized D0 state. The framework     */
/* calls the driver's EvtDevicePrepareHardware callback function before       */
/* calling the driver's EvtDeviceD0Entry callback function.                   */
/******************************************************************************/
/* External function: 														  */
/*		- ConfigureUsbInterface	(in file Device.c, OK)	 		  			  */
/*		- ConfigureUsbPipes	(in file Device.c, OK)		 		  			  */
/*		- InitPowerManagement	(in file Power.c) 				  			  */
/******************************************************************************/
NTSTATUS
EvtDevicePrepareHardware(
    IN WDFDEVICE    Device,
    IN WDFCMRESLIST ResourceList,
    IN WDFCMRESLIST ResourceListTranslated
    )
{
  NTSTATUS status;
  PDEVICE_CONTEXT devCtx = NULL; 
  WDF_USB_CONTINUOUS_READER_CONFIG interruptConfig;

  UNREFERENCED_PARAMETER(ResourceList);
  UNREFERENCED_PARAMETER(ResourceListTranslated);

  // Get device context
  devCtx = GetDeviceContext(Device);

  // Configure USB Interface
  status = ConfigureUsbInterface(Device, devCtx);
  if(!NT_SUCCESS(status))
    return status;
	
  // Configure USB Pipe
  status = ConfigureUsbPipes(devCtx);
  if(!NT_SUCCESS(status))
    return status;

  // Init Power Management 
  status = InitPowerManagement(Device, devCtx);
  if(!NT_SUCCESS(status))
    return status;

  /*Set up the interrupt endpoint with a continuous read operation. that
    way we are guaranteed that no interrupt data is lost.*/
  WDF_USB_CONTINUOUS_READER_CONFIG_INIT(&interruptConfig,
                                        EvtUsbDeviceInterrupt,	// Interrupt Event
                                        devCtx,
                                        sizeof(BYTE));
  status = WdfUsbTargetPipeConfigContinuousReader(
                                        devCtx->UsbInterruptPipe,
                                        &interruptConfig);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetPipeConfigContinuousReader failed with status 0x%08x\n", status));
    return status;
  }

  return status;
}

/******************************************************************************/
/* Configure the USB interface of our device                                  */
/******************************************************************************/
NTSTATUS ConfigureUsbInterface(WDFDEVICE Device, PDEVICE_CONTEXT DeviceContext)
{
  NTSTATUS status = STATUS_SUCCESS;
  WDF_USB_DEVICE_SELECT_CONFIG_PARAMS usbConfig;

  // Create USB Target Device Object for Device Context
  status = WdfUsbTargetDeviceCreate(Device,
                                    WDF_NO_OBJECT_ATTRIBUTES,
                                    &DeviceContext->UsbDevice);

  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetDeviceCreate failed with status 0x%08x\n", status));
    return status;
  }

  /*Initialize the parameters struct so that the device can initialize
    and use a single specified interface.
    this only works if the device has just 1 interface.*/
  /*Chon duy nhat 1 interface cua USB Device */
  WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_SINGLE_INTERFACE(&usbConfig);

  /*Chon duy nhat 1 interface cua USB Device */
  status = WdfUsbTargetDeviceSelectConfig(DeviceContext->UsbDevice,
                                          WDF_NO_OBJECT_ATTRIBUTES,
                                          &usbConfig);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetDeviceSelectConfig failed with status 0x%08x\n", status));
    return status;
  }

  /*Put the USB interface in our device context so that we can use it in
    future calls to our driver.*/
  DeviceContext->UsbInterface =
    usbConfig.Types.SingleInterface.ConfiguredUsbInterface;

  return status;
}

/******************************************************************************/
/* Configure the different IO pipes that are available on our device          */
/******************************************************************************/
NTSTATUS ConfigureUsbPipes(PDEVICE_CONTEXT DeviceContext)
{
  NTSTATUS status = STATUS_SUCCESS;
  BYTE index = 0;
  WDF_USB_PIPE_INFORMATION pipeConfig;
  WDFUSBPIPE pipe = NULL;
  BYTE  numEndpoints;

  // For debug
  numEndpoints = WdfUsbInterfaceGetNumEndpoints(
                                              DeviceContext->UsbInterface,
                                              0
                                              );

  KdPrint((__DRIVER_NAME " Found %i EndPoints\n", numEndpoints)); 
  // For debug

  DeviceContext->UsbInterruptPipe = NULL;
  DeviceContext->UsbBulkInPipe = NULL;
  DeviceContext->UsbBulkOutPipe = NULL;
  WDF_USB_PIPE_INFORMATION_INIT(&pipeConfig);	// Init pipe config
  
  //
  // Scan all pipe from USB Interface
  //
  do
  {
    pipe = WdfUsbInterfaceGetConfiguredPipe(DeviceContext->UsbInterface,
                                          index,
                                          &pipeConfig);
											
    if (NULL == pipe)
      break;

    // For debug
    KdPrint((__DRIVER_NAME " Pipe[%i].PacketSize = %i\n", index, pipeConfig.MaximumPacketSize));    
    // For debug

    /*None of our data transfers will have a guarantee that the requested
      data size is a multiple of the packet size.*/
    WdfUsbTargetPipeSetNoMaximumPacketSizeCheck(pipe);

    if(WdfUsbPipeTypeInterrupt == pipeConfig.PipeType)	// Interrupt pipe
    { 
	  KdPrint((__DRIVER_NAME " Found Interrupt Pipe at index %i\n", index));
      DeviceContext->UsbInterruptPipe = pipe;
    }
    else if(WdfUsbPipeTypeBulk == pipeConfig.PipeType)	// Bulk pipe
    {
      if(TRUE == WdfUsbTargetPipeIsInEndpoint(pipe))	// In EndPoint of Bulk
      {
	    KdPrint((__DRIVER_NAME " Found In Endpoint Pipe at index %i\n", index));
        DeviceContext->UsbBulkInPipe = pipe;
      }
      else if(TRUE == WdfUsbTargetPipeIsOutEndpoint(pipe))
      {
        KdPrint((__DRIVER_NAME " Found Out Endpoint Pipe at index %i\n", index));
        DeviceContext->UsbBulkOutPipe = pipe;			// Out EndPoint of Bulk
      }
    }
	
	index++;	// Encreate index of Pipe
	
  } while (NULL != pipe);

  // Check for all 3 pipe: interrupt, Bulk In, Bulk Out for our device
  if((NULL == DeviceContext->UsbInterruptPipe) ||
	  (NULL == DeviceContext->UsbBulkInPipe) ||
	  (NULL == DeviceContext->UsbBulkOutPipe))
  {
    KdPrint((__DRIVER_NAME
      "Not all expected USB pipes were found.\n"));
    return STATUS_INVALID_PARAMETER;
  }
  
  return status;
}

