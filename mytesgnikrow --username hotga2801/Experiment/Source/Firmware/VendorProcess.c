 /*----------------------------------------------------------------------------
 *      U S B  -  Neo
 *----------------------------------------------------------------------------
 *      Name:    USBCFG.H
 *      Purpose: Xu ly I/O tren endpoint bulk(2)
 *      Version: V1.0
 *---------------------------------------------------------------------------*/

#include <LPC23xx.H> 
#include "type.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbdesc.h"
#include "usbuser.h"
#include "SerialLogger.h"
#include "VendorProcess.h"
#include "LCD.h"
#include "Nhac.h"

/* Bulk In/Out Buffer */
BYTE  BulkBuf[MAX_PACKET];
BYTE  BulkLen;
BYTE  BufferMemory[512];
BYTE  MemoryLen = 0;

/* LCD Buffer */
unsigned char LCD_Buffer[17];

/* LED Array */
BYTE LED_status;

/* Song Data */
extern int iSongSize[3];
extern unsigned char song_custom[256];

/* DAC Volume */
extern int volume;

void Bulk2Out(void)
{
	BulkLen = USB_ReadEP(BULK_EP_OUT, BulkBuf);

	memcpy(BufferMemory + MemoryLen, BulkBuf, BulkLen);
	MemoryLen += BulkLen;

}

//Successful transfer
void Bulk2In(void)
{	
	BulkBuf[0] = 1;
	BulkBuf[1] = 2;
	BulkBuf[2] = 3;
	BulkLen = 3;
	USB_WriteEP(BULK_EP_IN, BulkBuf, 3);
}

void LCD_print()
{
	memset(LCD_Buffer, 0, 17);
	memcpy(LCD_Buffer, BufferMemory, 16);
	LCD_Buffer[16]= 0;
	lcd_print(LCD_Buffer);

	memset(LCD_Buffer, 0, 17);
	memcpy(LCD_Buffer, BufferMemory + 16, 16);
	LCD_Buffer[16]= 0;
	set_cursor(0, 1);
	lcd_print(LCD_Buffer);
}

/*	Processing Vendor control 
 *			 + Input: USB_SETUP_PACKET pointer
 *			 + Output: FALSE: fail transfer
 *					 TRUE: success transfer 
 */

BOOL VendorSetupStage(USB_SETUP_PACKET *p)
{

 //  PLog("Vendor Setup Stage");
	switch(p->bRequest)
	{
		case VC_SET_LIGHT_BAR:
			break;

		case VC_GET_LIGHT_BAR:
			//EP0Data.pData[0] = BufferMemory[0];
			//EP0Data.pData[1] = BufferMemory[1];
			/*EP0Data.pData[0] = FIO2PIN;
			EP0Data.Count = 1;*/
			EP0Data.pData = BufferMemory;
			EP0Data.Count = 3;
			/*memcpy(EP0Data.pData, BufferMemory, 32); 
			EP0Data.Count = 32;
			MemoryLen = 0;
			USB_DataInStage();	*/	 
			/*USB_StatusInStage();
			lcd_clear();
			LCD_print();  
			EP0Data.pData[0] = BufferMemory[0];
			//EP0Data.pData[1] = BufferMemory[1];
			//EP0Data.pData[2] = BufferMemory[2]; 
			//EP0Data.pData[3] = BufferMemory[3];
			EP0Data.Count = 1;	*/
			USB_DataInStage();
			MemoryLen = 0;   
			break;

		case VC_LCD_CLEAR:
			lcd_clear();
			USB_StatusInStage();
			break;
		
		case VC_LCD_SET_TEXT:
			lcd_clear();
			break;

		case VC_LCD_SET_TEXT_NO_DATA:
			USB_StatusInStage();
			lcd_clear();
			LCD_print();
			MemoryLen = 0;
			break;
		
		case VC_DAC_MUSIC1:
			NhacOn(0);
			USB_StatusInStage();
			break;
		
		case VC_DAC_MUSIC2:
			NhacOn(1);
			USB_StatusInStage();	    
			break;

		case VC_DAC_MUSIC_BY_DATA:
			USB_StatusInStage();
			NhacCustom(BufferMemory, MemoryLen);
			MemoryLen = 0;
			break;
		
		case VC_DAC_STOP_MUSIC:
			NhacOff();
			USB_StatusInStage();	  
			break;

		case VC_DAC_SET_VOLUME:
			volume = p->wValue.WB.L;	  
			break;

		case VC_GET_ENCODED_DATA:
			//EP0Data.pData[0] = BufferMemory[0];
			//EP0Data.pData[1] = BufferMemory[1];
			EP0Data.pData = BufferMemory;
			EP0Data.Count = 2;
			/*memcpy(EP0Data.pData, BufferMemory, 32); 
			EP0Data.Count = 32;
			MemoryLen = 0;
			USB_DataInStage();	*/	 
			USB_StatusInStage();
			lcd_clear();
			LCD_print();/*  
			EP0Data.pData[0] = BufferMemory[0];
			//EP0Data.pData[1] = BufferMemory[1];
			//EP0Data.pData[2] = BufferMemory[2]; 
			//EP0Data.pData[3] = BufferMemory[3];
			EP0Data.Count = 1;	*/
			USB_DataInStage();
			MemoryLen = 0;  
			break;
	  
		default:
			return (FALSE);
	}

	return (TRUE);
}

void VendorDataStage(USB_SETUP_PACKET *p)
{
	switch(p->bRequest)
	{
		case VC_SET_LIGHT_BAR:
		    USB_StatusInStage();
			LED_status = p->wValue.WB.L;
			//LED_status = EP0Data.pData[0];
		//	PLog("Set LED Datastage");
		    FIO2CLR = 0xFF;
			FIO2SET = LED_status;
			
			break;

		case VC_LCD_SET_TEXT:
			USB_StatusInStage();
			BulkLen = p->wLength;
			LCD_print();
			break;

		case VC_DAC_SET_VOLUME:
		   	USB_StatusInStage();
			PLogN("EP0Data.pData[0]: ");
			PLogNum(EP0Data.pData[0]);
			PLog("");	
			PLogN("volume: ");
			PLogNum(volume);	  
			break;

		default:
			break;
	}
}

