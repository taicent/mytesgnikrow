#ifndef __WDF_USB_DD_PUBLIC__
#define __WDF_USB_DD_PUBLIC__

/* WDF USB device GUID for LPC2300 LedsCtrl Driver */
// {BF433C13-0D7D-4ef8-B5A5-D5CCC7F3C416}
DEFINE_GUID(GUID_DEV_IF_LEDSCTRL, 
0xbf433c13, 0xd7d, 0x4ef8, 0xb5, 0xa5, 0xd5, 0xcc, 0xc7, 0xf3, 0xc4, 0x16);

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

