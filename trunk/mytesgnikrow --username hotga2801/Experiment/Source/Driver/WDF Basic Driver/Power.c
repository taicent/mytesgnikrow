
#include "ProtoTypes.h"

#pragma alloc_text(PAGE, EvtDeviceD0Entry)
#pragma alloc_text(PAGE, EvtDeviceD0Exit)

/****************************************************************************/
/* This function is called when the device is either started or woken up.   */
/****************************************************************************/
NTSTATUS
EvtDeviceD0Entry(
    IN WDFDEVICE  Device,
    IN WDF_POWER_DEVICE_STATE  PreviousState
    )
{
  NTSTATUS status = STATUS_SUCCESS;

  UNREFERENCED_PARAMETER(Device);
  UNREFERENCED_PARAMETER(PreviousState);

  KdPrint((__DRIVER_NAME "--> EvtDeviceD0Entry\n"));

  KdPrint((__DRIVER_NAME "<-- EvtDeviceD0Entry\n"));

  return status;
}

/******************************************************************************/
/* This function is called when the device is powered down.                   */
/* the current IO is left pending, because otherwise the continuous interrupt */
/* read IO will also be cancelled, and it would have to be reconfigured.      */
/******************************************************************************/
NTSTATUS
EvtDeviceD0Exit(
    IN WDFDEVICE  Device,
    IN WDF_POWER_DEVICE_STATE  TargetState
    )
{
  NTSTATUS status = STATUS_SUCCESS;

  UNREFERENCED_PARAMETER(Device);
  UNREFERENCED_PARAMETER(TargetState);

  KdPrint((__DRIVER_NAME "--> EvtDeviceD0Exit\n"));

  KdPrint((__DRIVER_NAME "<-- EvtDeviceD0Exit\n"));

  return status;
}

