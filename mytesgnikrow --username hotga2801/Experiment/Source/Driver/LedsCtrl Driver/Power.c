
#include "ProtoTypes.h"

#pragma alloc_text(PAGE, InitPowerManagement)
#pragma alloc_text(PAGE, EvtDeviceD0Entry)
#pragma alloc_text(PAGE, EvtDeviceD0Exit)
#pragma alloc_text(PAGE, PowerName)

/****************************************************************************/
/* This function is called when the device is either started or woken up.   */
/****************************************************************************/
/* External function: llSetLightBar	(in file ?)								*/
/****************************************************************************/
NTSTATUS
EvtDeviceD0Entry(
    IN WDFDEVICE  Device,
    IN WDF_POWER_DEVICE_STATE  PreviousState
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;

  KdPrint((__DRIVER_NAME "Device D0 Entry. Coming from %s\n",
    PowerName(PreviousState)));

  devCtx = GetDeviceContext(Device);	// Get device context
  
  // Khoi dong Pipes
  status = WdfIoTargetStart(WdfUsbTargetPipeGetIoTarget(devCtx->UsbInterruptPipe));
  
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfIoTargetStart failed with status 0x%08x\n", status));
    return status;
  }

  /*Restore the state of the LED array if the device is waking up from a
    D3 power state.*/
  if(PreviousState == PowerDeviceD3)
  {
    status = llSetLightBar(devCtx, devCtx->WdfMemLEDArrayState);
  }

  return status;
}

/******************************************************************************/
/* This function is called when the device is powered down.                   */
/* the current IO is left pending, because otherwise the continuous interrupt */
/* read IO will also be cancelled, and it would have to be reconfigured.      */
/******************************************************************************/
/* External function: 														  */
/* 		- PowerName (in file: Power.c, OK)									  */
/* 		- llGetLightBar (in file: ?)										  */
/******************************************************************************/
NTSTATUS
EvtDeviceD0Exit(
    IN WDFDEVICE  Device,
    IN WDF_POWER_DEVICE_STATE  TargetState
    )
{
  NTSTATUS status = STATUS_SUCCESS;
  PDEVICE_CONTEXT devCtx = NULL;

  devCtx = GetDeviceContext(Device);

  KdPrint((__DRIVER_NAME "Device D0 Exit. Going to %s\n",
    PowerName(TargetState)));

  /*Save the state of the LED array if the device is waking up from a
    D3 power state.*/
  if(TargetState == PowerDeviceD3)
  {
    status = llGetLightBar(devCtx, devCtx->WdfMemLEDArrayState);
    if(!NT_SUCCESS(status))
      return status;
  }

  // Dung viec gui gui cac lenh dang pending qua ong interrupt
  WdfIoTargetStop(WdfUsbTargetPipeGetIoTarget(devCtx->UsbInterruptPipe),
                  WdfIoTargetLeaveSentIoPending);

  return status;
}

/******************************************************************************/
/* Initialize the power management functionality of our USB device.           */
/* we want it to support system wake-up with the on-board switch, and         */
/* idle- time sleep that puts the device into a sleeping state if it is       */
/* idle for a specified duration.                                             */
/******************************************************************************/
/* External function: None													  */
/******************************************************************************/
NTSTATUS
InitPowerManagement(
    IN WDFDEVICE  Device,
    IN PDEVICE_CONTEXT Context)
{
  NTSTATUS status = STATUS_SUCCESS;
  WDF_USB_DEVICE_INFORMATION usbInfo;

  KdPrint((__DRIVER_NAME "Device init power management\n"));

  // Lay thong tin cua thiet bi USB
  WDF_USB_DEVICE_INFORMATION_INIT(&usbInfo);	// init object
  status = WdfUsbTargetDeviceRetrieveInformation(
                                Context->UsbDevice,
                                &usbInfo);
  if(!NT_SUCCESS(status))
  {
    KdPrint((__DRIVER_NAME
      "WdfUsbTargetDeviceRetrieveInformation failed with status 0x%08x\n",
      status));
    return status;
  }
  
  // Xem cac thong tin debug, thiet bi co ho tro self powered, wakeup hay high speed khong?
  KdPrint((__DRIVER_NAME  "Device self powered: %d\n",
    usbInfo.Traits & WDF_USB_DEVICE_TRAIT_SELF_POWERED ? 1 : 0));
  KdPrint((__DRIVER_NAME  "Device remote wake capable: %d\n",
    usbInfo.Traits & WDF_USB_DEVICE_TRAIT_REMOTE_WAKE_CAPABLE ? 1 : 0));
  KdPrint((__DRIVER_NAME  "Device high speed: %d\n",
    usbInfo.Traits & WDF_USB_DEVICE_TRAIT_AT_HIGH_SPEED ? 1 : 0));

  if(usbInfo.Traits & WDF_USB_DEVICE_TRAIT_REMOTE_WAKE_CAPABLE)
  {
    WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS idleSettings;
    WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS wakeSettings;

    WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS_INIT(&idleSettings,
                                               IdleUsbSelectiveSuspend);
    idleSettings.IdleTimeout = 10000;
    status = WdfDeviceAssignS0IdleSettings(Device, &idleSettings);
    if(!NT_SUCCESS(status))
    {
      KdPrint((__DRIVER_NAME
        "WdfDeviceAssignS0IdleSettings failed with status 0x%08x\n",
        status));
      return status;
    }
    
    WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS_INIT(&wakeSettings);
    wakeSettings.DxState = PowerDeviceD2;
    status = WdfDeviceAssignSxWakeSettings(Device, &wakeSettings);
    if(!NT_SUCCESS(status))
    {
      KdPrint((__DRIVER_NAME
        "WdfDeviceAssignSxWakeSettings failed with status 0x%08x\n",
        status));
      return status;
    }
  }

  return status;
}

/******************************************************************************/
/* For debugging purposes it is nice to be able to print out power state      */
/*  names so that we can see where we are coming from, and where we are       */
/*  going to.                                                                 */
/******************************************************************************/
LPCSTR PowerName(WDF_POWER_DEVICE_STATE PowerState)
{
  char * name = "";

  switch(PowerState)
  {
  case PowerDeviceUnspecified: name = "PowerDeviceUnspecified"; break;
  case PowerDeviceD0: name = "PowerDeviceD0"; break;
  case PowerDeviceD1: name = "PowerDeviceD1"; break;
  case PowerDeviceD2: name = "PowerDeviceD2"; break;
  case PowerDeviceD3: name = "PowerDeviceD3"; break;
  case PowerDeviceMaximum: name = "PowerDeviceMaximum"; break;
  default:
    name = "PowerDeviceUnknown"; break;
  }
  return name;
}
