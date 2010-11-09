
#pragma warning(push)
#pragma warning(disable:4115)  // named typedef in parenthesis
#pragma warning(disable:4200)  // nameless struct/union
#pragma warning(disable:4201)  // nameless struct/union
#pragma warning(disable:4214)  // bit field types other than int

#define INITGUID

#include <ntddk.h>
#include <wdf.h>

#pragma warning(pop)

#define __DRIVER_NAME "LEDSCTRL: "

/*Declaration of the device context. we have to declare the type of the
  device context, and an accessor function name that will return to us
  a pointer to the device context.*/
typedef struct _DEVICE_CONTEXT {
  WDFQUEUE          IoDefaultQueue;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);

NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject, 
    IN PUNICODE_STRING  RegistryPath
    );

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
EvtDeviceIoDefault(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request
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


