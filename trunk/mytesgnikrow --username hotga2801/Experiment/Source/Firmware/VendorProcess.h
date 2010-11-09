#ifndef __BULK_PROCESS__
#define __BULK_PROCESS__

/* Max In/Out Packet Size */
#define MAX_PACKET  		64
#define MAX_BULK_BUFFER	 	8 * 1024

/* MSC In/Out Endpoint Address */
#define BULK_EP_IN       0x82
#define BULK_EP_OUT      0x02

#define VC_LCD_CLEAR       0xD1
#define VC_LCD_SET_TEXT       0xD2			/* Set LCD Co data */

#define VC_DAC_MUSIC1       0xD3
#define VC_DAC_MUSIC2       0xD4
#define VC_DAC_MUSIC_BY_DATA  0xD5

#define VC_LCD_SET_TEXT_NO_DATA 0xD6

#define VC_GET_LIGHT_BAR      0xD7
#define VC_SET_LIGHT_BAR      0xD8

#define VC_DAC_STOP_MUSIC      0xD9
#define VC_DAC_SET_VOLUME      0xDA

//region Add

#define VC_GET_ENCODED_DATA 	0xDB


extern void ReplyStatusLed(void);
extern void SetStatusLed(void);
extern void Bulk2Out(void);

extern void Bulk2In(void);

extern BOOL VendorSetupStage(USB_SETUP_PACKET *p);
extern void VendorDataStage(USB_SETUP_PACKET *p);
extern void VendorStatusInStage(USB_SETUP_PACKET *p);

#endif/* __BULK_PROCESS__*/
