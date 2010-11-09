/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    USBDESC.C
 *      Purpose: USB Descriptors
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------
 *		String Index:
 *			- Index 0x04: Manufacturer
 *			- Index 0x30: Product
 *			- Index 0x6A: Serial Number
 *			- Index 0x86: Interface 0, Alternate Setting 0
 *
 *		USB Descriptors:
 *			- DeviceDescriptor
 *					- ConfigurationDescriptor
 *							- Interface 0 (Bulk Only),
 *							  Alternate Setting 0
 *									- EndPoint 1 - In (8 Byte Max) - Interrupt EndPoint
 *									- EndPoint 2 - In (64 Byte Max)
 *									- EndPoint 2 - Out(64 Byte Max)
 *---------------------------------------------------------------------------*/

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbdesc.h"


/* USB Standard Device Descriptor */
const BYTE USB_DeviceDescriptor[] = {
  USB_DEVICE_DESC_SIZE,              /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0110), /* 1.10 */          /* bcdUSB */
  0x00,                              /* bDeviceClass */
  0x00,                              /* bDeviceSubClass */
  0x00,                              /* bDeviceProtocol */
  USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
  WBVAL(0xC251),                     /* idVendor */				// TODO: New Vendor ID here
  WBVAL(0x1703),                     /* idProduct */			// TODO: New Product ID here
  WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
  0x04,                              /* iManufacturer */
  0x30,                              /* iProduct */
  0x6A,                              /* iSerialNumber */
  0x01                               /* bNumConfigurations */
};

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const BYTE USB_ConfigDescriptor[] = {
/* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
  WBVAL(                             /* wTotalLength */
    1*USB_CONFIGUARTION_DESC_SIZE +
    1*USB_INTERFACE_DESC_SIZE     +
    3*USB_ENDPOINT_DESC_SIZE		 /* Three EndPoint*/
  ),
  0x01,                              /* bNumInterfaces */
  0x01,                              /* bConfigurationValue */	   // Select Configuration 1
  0x00,                              /* iConfiguration */
  USB_CONFIG_BUS_POWERED /*|*/       /* bmAttributes */
/*USB_CONFIG_REMOTE_WAKEUP*/,
  USB_CONFIG_POWER_MA(100),          /* bMaxPower */

  USB_INTERFACE_DESC_SIZE,           /* bLength */
/* Interface 0, Alternate Setting 0, MSC Class */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x00,                              /* bInterfaceNumber */			// Interface 0
  0x00,                              /* bAlternateSetting */		// AlternateSetting 0
  0x03,                              /* bNumEndpoints */
  
  //USB_DEVICE_CLASS_STORAGE,          /* bInterfaceClass */		// TODO: Mass Storage Class? Define new class here. Use 0xFF?
  //MSC_SUBCLASS_SCSI,                 /* bInterfaceSubClass */		// TODO: Use Mass Storage Class (MSC_SUBCLASS_SCSI)? Set 0x00
  //MSC_PROTOCOL_BULK_ONLY,            /* bInterfaceProtocol */		// TODO: Use Mass Storage Class (MSC_PROTOCOL_BULK_ONLY)? Set 0x00

  // Use my interface
  USB_DEVICE_CLASS_VENDOR_SPECIFIC,	 /* bInterfaceClass */
  0x00,								 /* bInterfaceSubClass */
  0x00,								 /* bInterfaceProtocol */

  0x86,                              /* iInterface */
/* Interrupt Out Endpoint 1*/
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_IN(1),                /* bEndpointAddress */		// Descript: EndPoint 1 - IN (Device to Host)
  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */			// Descript: Interrupt Packet
  WBVAL(0x0008),                     /* wMaxPacketSize */		// Descript: Max Packet size: 8 Byte
  0x08,                              /* bInterval */			// Descript: Interval: 8 ms
/* Bulk In Endpoint 2*/
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_IN(2),                /* bEndpointAddress */		// Descript: EndPoint 2 - In (Device to Host)
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */			// Descript: Bulk Packet
  WBVAL(0x0040),                     /* wMaxPacketSize */		// Descript: Max Packet size: 64 Byte
  0,                                 /* bInterval */
/* Bulk Out Endpoint 2*/
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_OUT(2),               /* bEndpointAddress */		// Descript: EndPoint 2 - Out (Host to Device)
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */			// Descript: Bulk Packet
  WBVAL(0x0040),                     /* wMaxPacketSize */		// Descript: Max Packet size: 64 Byte
  0,                                 /* bInterval */
/* Terminator */
  0                                  /* bLength */
};

/* USB String Descriptor (optional) */
const BYTE USB_StringDescriptor[] = {
/* Index 0x00: LANGID Codes */
  0x04,                              /* bLength */				// Descript: 04 Bytes (bLength + bDescriptorType + wLANGID)
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0409), /* US English */    /* wLANGID */
/* Index 0x04: Manufacturer */
  0x2C,                              /* bLength */				// Descript: 44 Bytes (bLength + bDescriptorType + 21 UnicodeChar)
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'S',0,
  'E',0,
  'L',0,
  'a',0,
  'b',0,
  ' ',0,
  'E',0,
  'm',0,
  'b',0,
  'e',0,
  'd',0,
  'd',0,
  'e',0,
  'd',0,
  ' ',0,
  'S',0,
  'y',0,
  's',0,
  't',0,
  'e',0,
  'm',0,
/* Index 0x30: Product */
  0x3A,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */		// Descript: 58 Bytes (bLength + bDescriptorType + 28 UnicodeChar)
  'K',0,
  'e',0,
  'i',0,
  'l',0,
  ' ',0,
  'M',0,
  'C',0,
  'B',0,
  '2',0,
  '3',0,
  '0',0,
  '0',0,
  ' ',0,
  'L',0,
  'e',0,
  'd',0,
  's',0,
  ' ',0,  
  'C',0,
  'o',0,
  'n',0,
  't',0,
  'r',0,
  'o',0,
  'l',0,
  'l',0,
  'e',0,
  'r',0,
/* Index 0x6A: Serial Number */
  0x1C,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */		// Descript: 28 Bytes (bLength + bDescriptorType + 13 UnicodeChar)
  'L',0,
  'E',0,
  'D',0,
  'C',0,
  'T',0,
  'R',0,
  'L',0,
  '3',0,
  '7',0,
  '3',0,
  '4',0,
  '6',0,
  '8',0,  
/* Index 0x86: Interface 0, Alternate Setting 0 */
  0x12,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */		// Descript: 18 Bytes (bLength + bDescriptorType + 8 UnicodeChar)
  'L',0,
  'e',0,
  'd',0,
  's',0,
  'C',0,
  't',0,
  'r',0,
  'l',0,
};
