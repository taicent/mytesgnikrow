/************************************************************************/
/*  Device.c: All implement for Device function							*/
/************************************************************************/

#include <initguid.h>
#include "ProtoTypes.h"
#include "public.h"

#pragma alloc_text(PAGE, EvtDevicePrepareHardware)
#pragma alloc_text(PAGE, EvtDeviceAdd)

NTSTATUS EvtDeviceAdd(
                      IN WDFDRIVER  Driver,
                      IN PWDFDEVICE_INIT  DeviceInit
                      )
{
  NTSTATUS status;
  WDFDEVICE device;									// Device object
  PDEVICE_CONTEXT devCtx = NULL;					// Device Context 
  WDF_OBJECT_ATTRIBUTES attributes;					// WDF Object Attribute
  WDF_PNPPOWER_EVENT_CALLBACKS pnpPowerCallbacks;
  WDF_IO_QUEUE_CONFIG ioQConfig;					// IO Queues Congig

  UNREFERENCED_PARAMETER(Driver);

  KdPrint((__DRIVER_NAME "--> EvtDeviceAdd\n"));

  /* Set the callback functions that will be executed on PNP and Power events */
  WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
  pnpPowerCallbacks.EvtDevicePrepareHardware = EvtDevicePrepareHardware;	// Callback EvtDevicePrepareHardware()
  pnpPowerCallbacks.EvtDeviceD0Entry = EvtDeviceD0Entry;					// Callback EvtDeviceD0Entry()
  pnpPowerCallbacks.EvtDeviceD0Exit = EvtDeviceD0Exit;						// Callback EvtDeviceD0Exit()
  WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);
  
  // For tracing
  KdPrint((__DRIVER_NAME "    - Set the callback functions that will be executed on PNP and Power events\n"));
  KdPrint((__DRIVER_NAME "         Set callback EvtDevicePrepareHardware when Repare Hardware Event\n"));
  KdPrint((__DRIVER_NAME "         Set callback EvtDeviceD0Entry for D0 State Entry Event\n"));
  KdPrint((__DRIVER_NAME "         Set callback EvtDeviceD0Exit for D0 State Exit Event\n"));
  
  /* Set IO type to buffered IO */
  WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoBuffered);	// buffered IO
  KdPrint((__DRIVER_NAME "    - Set Set IO type to buffered IO by WdfDeviceInitSetIoType\n"));
  
  /* Initialize storage for the device context */
  WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT);
  KdPrint((__DRIVER_NAME "    - Initialize storage for the device context by WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE\n"));
  
  /* Create a device instance. */
  status = WdfDeviceCreate(&DeviceInit, &attributes, &device);  
  KdPrint((__DRIVER_NAME "    - Create a device instance by WdfDeviceCreate\n"));
  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "    - WdfDeviceCreate failed with status 0x%08x\n", status));
    return status;
  }
  
  devCtx = GetDeviceContext(device);

  /* Create the default IO queue. this one will be used for IOCTL requests */
  WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQConfig,
                                          WdfIoQueueDispatchSequential);
  ioQConfig.EvtIoDefault = EvtDeviceIoDefault;
  status = WdfIoQueueCreate(device,
                            &ioQConfig,
                            WDF_NO_OBJECT_ATTRIBUTES,
                            &devCtx->IoDefaultQueue);
							
  KdPrint((__DRIVER_NAME "    - Create the default IO queue (type: Sequential).\n"));
  KdPrint((__DRIVER_NAME "      This one will be used for IOCTL requests\n"));  
    
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfIoQueueCreate failed with status 0x%08x\n", status));
    return status;
  }

  status = WdfDeviceCreateDeviceInterface(device, &GUID_DEV_IF_LEDSCTRL, NULL);
  KdPrint((__DRIVER_NAME "    - Create a device interface, assigned with interface GUID_DEV_IF_LEDSCTRL in public.h\n"));
  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfDeviceCreateDeviceInterface failed with status 0x%08x\n", status));
    return status;
  }

  KdPrint((__DRIVER_NAME "<-- EvtDeviceAdd\n"));
  return status;
}

/******************************************************************************/
/* Call-back function that will be called by the pnp/power manager after the  */
/* Plug and Play manager has assigned hardware resources to the device and    */
/* after the device has entered its uninitialized D0 state. The framework     */
/* calls the driver's EvtDevicePrepareHardware callback function before       */
/* calling the driver's EvtDeviceD0Entry callback function.                   */
/******************************************************************************/
NTSTATUS
EvtDevicePrepareHardware(
    IN WDFDEVICE    Device,
    IN WDFCMRESLIST ResourceList,
    IN WDFCMRESLIST ResourceListTranslated
    )
{
  NTSTATUS status = STATUS_SUCCESS;

  KdPrint((__DRIVER_NAME "--> EvtDevicePrepareHardware\n"));

  UNREFERENCED_PARAMETER(Device);
  UNREFERENCED_PARAMETER(ResourceList);
  UNREFERENCED_PARAMETER(ResourceListTranslated);

  KdPrint((__DRIVER_NAME "<-- EvtDevicePrepareHardware\n"));

  return status;
}