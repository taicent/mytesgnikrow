#ifndef __WDF_USB_DD_PUBLIC__
#define __WDF_USB_DD_PUBLIC__

#pragma once

#include "stdafx.h"

#ifndef MAX_LEN
#define MAX_LEN	128
#endif

PUCHAR CreatePatternBuffer(IN ULONG Length);
PUCHAR CreateBuffer(IN ULONG Length);
extern TCHAR* ListDevice[];


#define	MAX_READ_PACKET_SIZE		64				// (64 Bytes)
#define	MAX_WRITE_PACKET_SIZE		64				// (64 Bytes)
#define	MAX_LED_IOCTRL_PACKET_SIZE	1				// (1 Bytes)
#define NUM_LCD_CHAR				32
#define MAX_DATA_SIZE				8 * 1024		// (8 KBytes USB RAM)


/* Device IO control codes for the WDF USB driver */
#define IOCTL_INDEX             0x800
#define FILE_DEVICE_USB_LPC     0x65500	// My file device type

#define IOCTL_WDF_USB_GET_SWITCHSTATE CTL_CODE(FILE_DEVICE_USB_LPC,     \
                                                     IOCTL_INDEX,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_READ_ACCESS)

#define IOCTL_WDF_USB_SET_LIGHTBAR    CTL_CODE(FILE_DEVICE_USB_LPC,     \
                                                     IOCTL_INDEX + 1,   \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)

// Set LED Patern Bitmap
#define IOCTL_WDF_USB_GET_LIGHTBAR    CTL_CODE(FILE_DEVICE_USB_LPC,     \
                                                     IOCTL_INDEX + 2,   \
                                                     METHOD_BUFFERED,   \
                                                     FILE_READ_ACCESS)
// Get LED Patern Bitmap
#define IOCTL_WDF_USB_GET_SWITCHSTATE_CHANGE CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 3,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_READ_ACCESS)
													 
// Get USB Device Descriptor
#define IOCTL_WDF_USB_GET_CONFIG_DESCRIPTOR	CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 4,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_READ_ACCESS)
													 
// Clear LCD
#define IOCTL_WDF_USB_LCD_CLEAR				CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 5,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)
													 
// Set LCD Text
#define IOCTL_WDF_USB_LCD_SET_TEXT			CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 6,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)
													 
// DAC Music 1
#define IOCTL_WDF_USB_DAC_MUSIC1		CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 7,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)
// DAC Music 2
#define IOCTL_WDF_USB_DAC_MUSIC2		CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 8,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)

// DAC Music By Data
#define IOCTL_WDF_USB_DAC_MUSIC_BY_DATA	CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 9,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)
													 
// LCD Set Text No Data
#define IOCTL_WDF_USB_LCD_SET_TEXT_NO_DATA	CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 10,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)

// Stop Playing Music
#define IOCTL_WDF_USB_DAC_STOP_MUSIC	CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 11,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)

// Set DAC Volume
#define IOCTL_WDF_USB_DAC_SET_VOLUME	CTL_CODE(FILE_DEVICE_USB_LPC,\
                                                     IOCTL_INDEX + 12,       \
                                                     METHOD_BUFFERED,   \
                                                     FILE_WRITE_ACCESS)
// region Add
#define IOCTL_WDF_USB_GET_ENCODED_DATA    CTL_CODE(FILE_DEVICE_USB_LPC,     \
	IOCTL_INDEX + 13,   \
	METHOD_BUFFERED,   \
	FILE_READ_ACCESS)	

#endif //__WDF_USB_DD_PUBLIC__

