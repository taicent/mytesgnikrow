
#include "ProtoTypes.h"

#pragma alloc_text(INIT, DriverEntry)

/******************************************************************************/
/* Entry point of the device driver.                                          */
/******************************************************************************/
NTSTATUS DriverEntry(
                      IN PDRIVER_OBJECT  DriverObject, 
                      IN PUNICODE_STRING  RegistryPath
                      )
{
  WDF_DRIVER_CONFIG config;
  NTSTATUS status;

  KdPrint((__DRIVER_NAME "--> DriverEntry\n"));

  WDF_DRIVER_CONFIG_INIT(&config, EvtDeviceAdd);

  status = WdfDriverCreate(
                      DriverObject,
                      RegistryPath,
                      WDF_NO_OBJECT_ATTRIBUTES,
                      &config,
                      WDF_NO_HANDLE);
					  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME "WdfDriverCreate failed with status 0x%08x\n", status));
  }

  KdPrint((__DRIVER_NAME "<-- DriverEntry\n"));

  return status;
}

