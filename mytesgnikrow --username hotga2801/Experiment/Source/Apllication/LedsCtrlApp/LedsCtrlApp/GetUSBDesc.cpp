#include "stdafx.h"

#include "Public.h"
#include "usb100.h"

/**************************************************************************
Routine Description:

    Called to get unicode string of USB descriptor

Arguments:

        PUSB_ENDPOINT_DESCRIPTOR is bDescriptorType or
        PUSB_DEVICE_DESCRIPTOR is bDescriptorType or
        PUSB_INTERFACE_DESCRIPTOR is bDescriptorType or
        PUSB_STRING_DESCRIPTOR is bDescriptorType or
        PUSB_POWER_DESCRIPTOR is bDescriptorType or
        PUSB_CONFIGURATION_DESCRIPTOR is bDescriptorType

Return Value:

    ptr to string

**************************************************************************/
PTCHAR 
usbDescriptorTypeString(UCHAR bDescriptorType)
{

        switch(bDescriptorType) {

        case USB_DEVICE_DESCRIPTOR_TYPE:
                return _T("USB_DEVICE_DESCRIPTOR_TYPE");

        case USB_CONFIGURATION_DESCRIPTOR_TYPE:
                return _T("USB_CONFIGURATION_DESCRIPTOR_TYPE");


        case USB_STRING_DESCRIPTOR_TYPE:
                return _T("USB_STRING_DESCRIPTOR_TYPE");


        case USB_INTERFACE_DESCRIPTOR_TYPE:
                return _T("USB_INTERFACE_DESCRIPTOR_TYPE");


        case USB_ENDPOINT_DESCRIPTOR_TYPE:
                return _T("USB_ENDPOINT_DESCRIPTOR_TYPE");


#ifdef USB_POWER_DESCRIPTOR_TYPE // this is the older definintion which is actually obsolete
    // workaround for temporary bug in 98ddk, older USB100.h file
        case USB_POWER_DESCRIPTOR_TYPE:
                return _T("USB_POWER_DESCRIPTOR_TYPE");
#endif

#ifdef USB_RESERVED_DESCRIPTOR_TYPE  // this is the current version of USB100.h as in NT5DDK

        case USB_RESERVED_DESCRIPTOR_TYPE:
                return _T("USB_RESERVED_DESCRIPTOR_TYPE");

        case USB_CONFIG_POWER_DESCRIPTOR_TYPE:
                return _T("USB_CONFIG_POWER_DESCRIPTOR_TYPE");

        case USB_INTERFACE_POWER_DESCRIPTOR_TYPE:
                return _T("USB_INTERFACE_POWER_DESCRIPTOR_TYPE");
#endif // for current nt5ddk version of USB100.h

        default:
                return _T("??? UNKNOWN!!");
        }
}




/**************************************************************************
Routine Description:

    Called to get unicode string of endpt descriptor type

Arguments:

        PUSB_ENDPOINT_DESCRIPTOR is bmAttributes

Return Value:

    ptr to string

**************************************************************************/
PTCHAR
usbEndPointTypeString(UCHAR bmAttributes)
{
    UINT typ = bmAttributes & USB_ENDPOINT_TYPE_MASK;


    switch (typ) {
    case USB_ENDPOINT_TYPE_INTERRUPT:
        return _T("USB_ENDPOINT_TYPE_INTERRUPT");

    case USB_ENDPOINT_TYPE_BULK:
        return _T("USB_ENDPOINT_TYPE_BULK");

    case USB_ENDPOINT_TYPE_ISOCHRONOUS:
        return _T("USB_ENDPOINT_TYPE_ISOCHRONOUS");

    case USB_ENDPOINT_TYPE_CONTROL:
            return _T("USB_ENDPOINT_TYPE_CONTROL");

    default:
            return _T("??? UNKNOWN!!");
    }
}



/**************************************************************************
Routine Description:

    Called to get unicode string of USB_CONFIGURATION_DESCRIPTOR attributes

Arguments:

        PUSB_CONFIGURATION_DESCRIPTOR is bmAttributes

Return Value:

    ptr to string

**************************************************************************/
PTCHAR
usbConfigAttributesString(UCHAR bmAttributes)

{
    UINT typ = bmAttributes & USB_CONFIG_POWERED_MASK;


    switch( typ) {

    case USB_CONFIG_BUS_POWERED:
            return _T("USB_CONFIG_BUS_POWERED");

    case USB_CONFIG_SELF_POWERED:
            return _T("USB_CONFIG_SELF_POWERED");

    case USB_CONFIG_REMOTE_WAKEUP:
            return _T("USB_CONFIG_REMOTE_WAKEUP");


    default:
            return _T("??? UNKNOWN!!");
    }
}



CString
get_USB_CONFIGURATION_DESCRIPTOR(PUSB_CONFIGURATION_DESCRIPTOR cd)
{
	CString str = _T("");
	CString result = _T("");
    
	result += _T("USB_CONFIGURATION_DESCRIPTOR\r\n");
	str.Format(_T("bLength			= 0x%02x, decimal %d\r\n"), cd->bLength, cd->bLength);
	result += str;

    str.Format(_T("bDescriptorType		= 0x%02x (%s)\r\n"), cd->bDescriptorType,
            usbDescriptorTypeString(cd->bDescriptorType)
    );
	result += str;

    str.Format(_T("wTotalLength		= 0x%04x, decimal %d\r\n"), cd->wTotalLength, cd->wTotalLength);
	result += str;

    str.Format(_T("bNumInterfaces		= 0x%02x, decimal %d\r\n"), cd->bNumInterfaces, cd->bNumInterfaces);
	result += str;

    str.Format(_T("bConfigurationValue	= 0x%02x, decimal %d\r\n"), cd->bConfigurationValue, cd->bConfigurationValue);
	result += str;

    str.Format(_T("iConfiguration		= 0x%02x, decimal %d\r\n"), cd->iConfiguration, cd->iConfiguration);
	result += str;

    str.Format(_T("bmAttributes		= 0x%02x (%s)\r\n"), cd->bmAttributes, usbConfigAttributesString(cd->bmAttributes));
	result += str;

    str.Format(_T("bMaxPower		= 0x%02x, decimal %d\r\n"), cd->MaxPower, cd->MaxPower);
	result += str;

	return result;
}


CString
get_USB_INTERFACE_DESCRIPTOR(PUSB_INTERFACE_DESCRIPTOR id, UINT ix)
{
	CString str = _T("");
	CString result = _T("");

    str.Format(_T("\r\nUSB_INTERFACE_DESCRIPTOR #%d\r\n"), ix);
	result += str;

    str.Format(_T("bLength			= 0x%02x\r\n"), id->bLength);
	result += str;

    str.Format(_T("bDescriptorType		= 0x%02x (%s)\r\n"), id->bDescriptorType,
            usbDescriptorTypeString( id->bDescriptorType )
     );
	result += str;

    str.Format(_T("bInterfaceNumber		= 0x%02x\r\n"), id->bInterfaceNumber);
	result += str;

    str.Format(_T("bAlternateSetting		= 0x%02x\r\n"), id->bAlternateSetting);
	result += str;

    str.Format(_T("bNumEndpoints		= 0x%02x\r\n"), id->bNumEndpoints);
	result += str;

    str.Format(_T("bInterfaceClass		= 0x%02x\r\n"), id->bInterfaceClass);
	result += str;

    str.Format(_T("bInterfaceSubClass		= 0x%02x\r\n"), id->bInterfaceSubClass);
	result += str;

    str.Format(_T("bInterfaceProtocol		= 0x%02x\r\n"), id->bInterfaceProtocol);
	result += str;

    str.Format(_T("bInterface		= 0x%02x\r\n"), id->iInterface);
	result += str;

	return result;
}


CString
get_USB_ENDPOINT_DESCRIPTOR(PUSB_ENDPOINT_DESCRIPTOR ed, int i)
{
	CString str = _T("");
	CString result = _T("");

	str.Format(_T("\r\nUSB_ENDPOINT_DESCRIPTOR for Pipe #%02d\r\n"), i);
	result += str;

    str.Format(_T("bLength			= 0x%02x\r\n"), ed->bLength);
	result += str;

    str.Format(_T("bDescriptorType		= 0x%02x (%s)\r\n"), ed->bDescriptorType,
                usbDescriptorTypeString( ed->bDescriptorType )
        );
	result += str;

    if ( USB_ENDPOINT_DIRECTION_IN( ed->bEndpointAddress ) ) {
        str.Format(_T("bEndpointAddress		= 0x%02x (INPUT)\r\n"), ed->bEndpointAddress);
		result += str;
    } else {
        str.Format(_T("bEndpointAddress		= 0x%02x (OUTPUT)\r\n"), ed->bEndpointAddress);
		result += str;
    }

    str.Format(_T("bmAttributes		= 0x%02x (%s)\r\n"), ed->bmAttributes,
                usbEndPointTypeString ( ed->bmAttributes )
        );
	result += str;

    str.Format(_T("wMaxPacketSize		= 0x%02x, decimal %d\r\n"), ed->wMaxPacketSize,
                ed->wMaxPacketSize);
	result += str;

    str.Format(_T("bInterval			= 0x%02x, decimal %d\r\n"), ed->bInterval, ed->bInterval);
	result += str;

	return result;
}


CString
GetUsbConfigDescriptor(HANDLE hDev)
/*++
Routine Description:

    Called to do formatted ascii dump to console of USB
    configuration, interface, and endpoint descriptors.

Arguments:

    Handle of Device

Return Value:

    String of human-readable string descriptor

--*/
{
	CString result = _T("");
    int siz, nBytes;
    char buf[256];
	BOOL success;

	// Check device handle
    if(hDev == INVALID_HANDLE_VALUE)
    {
        return _T("Handle of Device is Invalid!");
    }

    siz = sizeof(buf);
	memset(buf, 0, siz);

    success = DeviceIoControl(hDev,
                    IOCTL_WDF_USB_GET_CONFIG_DESCRIPTOR,	// Specific in public.h
                    buf,	// input buffer
                    siz,
                    buf,	// output buffer
                    siz,
                    (PULONG) &nBytes,
                    NULL);

    if(success == FALSE) {
		result.Format(_T("GetUsbConfigDescriptor failed with code %d\r\n"), GetLastError());
		return result;
    } else {

        ULONG i;
        UINT  j, n;
        char *pch;
        PUSB_CONFIGURATION_DESCRIPTOR cd;
        PUSB_INTERFACE_DESCRIPTOR id;
        PUSB_ENDPOINT_DESCRIPTOR ed;

        pch = buf;
        n = 0;
		result = _T("");

		// cd: pointer to USB_CONFIGURATION_DESCRIPTOR 
        cd = (PUSB_CONFIGURATION_DESCRIPTOR)pch;

        result += get_USB_CONFIGURATION_DESCRIPTOR(cd);

        pch += cd->bLength;

        do {
			// Interface descriptor
            id = (PUSB_INTERFACE_DESCRIPTOR)pch;

            result += get_USB_INTERFACE_DESCRIPTOR(id, n++);

			//::MessageBox(NULL, result, _T(""), MB_ICONERROR);
			pch += id->bLength;
			//ed = (PUSB_ENDPOINT_DESCRIPTOR)pch;
			//result += get_USB_ENDPOINT_DESCRIPTOR(ed, 0);
            for (j = 0; j < id->bNumEndpoints; j++) {

				// endpoint descriptor
                ed = (PUSB_ENDPOINT_DESCRIPTOR)pch;

                result += get_USB_ENDPOINT_DESCRIPTOR(ed, j);

                pch += ed->bLength;
            }

            i = (ULONG)(pch - buf);

        } while (i < cd->wTotalLength);
    }

    return result;
}

