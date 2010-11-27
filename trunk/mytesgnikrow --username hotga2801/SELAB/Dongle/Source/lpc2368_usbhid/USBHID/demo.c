/*----------------------------------------------------------------------------
 *      Name:    DEMO.C
 *      Purpose: USB HID Demo
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2006 Keil Software.
 *---------------------------------------------------------------------------*/

#include <LPC23xx.H>                        /* LPC23xx definitions */

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"

#include "demo.h"
#include "LCD.h"
#include "aes.h"

#define AUTHENTICATION_CODE 1
#define CHECK_EXISTANCE_CODE 2


BYTE InReport[USB_MAX_PACKET0];                              /* HID Input Report    */
                                            /*   Bit0   : Buttons  */
                                            /*   Bit1..7: Reserved */

BYTE OutReport[USB_MAX_PACKET0];       /* HID Out Report */
	                              /* Byte 0  Bit0..7: LEDs  */
	                              /* Byte 1..16  LCD line 1 */
	                              /* Byte 17..32 LCD line 2 */

unsigned char Key[32] = {0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f};
int Nr = 0;
int Nk = 0;

unsigned char Encryp[16];
unsigned char Decryp[16];


/*
 *  Get HID Input Report -> InReport
 */

void GetInReport (void) {

  /*if ((FIO2PIN & PBINT) == 0) {             // Check if PBINT is pressed 
    InReport = 0x01;
  } else {
    InReport = 0x00;
  }*/
  /*InReport[0] = '1';
  InReport[1] = '2';
  InReport[2] = '3';
  InReport[3] = '4';
  InReport[4] = '5';
  InReport[5] = '6';
  InReport[6] = '7';
  InReport[7] = '8';
  InReport[8] = '9';
  InReport[9] = 'A';
  InReport[10] = 'B';
  InReport[11] = 'C';
  InReport[12] = 'D';
  InReport[13] = 'E';
  InReport[14] = 'F';
  InReport[15] = '0'; */
  int i;
  for(i = 0;i < 16;i++)
  {
  	InReport[i+1] = Encryp[i];
  }
}


/*
 *  Set HID Output Report <- OutReport
 */

void SetOutReport (void) {

/*  FIO2CLR = LEDMSK;
  FIO2SET = OutReport[0];
  lcd_clear();                      
  lcd_print (&OutReport[1]);        
  set_cursor (0, 1);
  lcd_print (&OutReport[17]);   */
  FIO2CLR = LEDMSK;
  if(OutReport[0] == AUTHENTICATION_CODE)
  {
  	FIO2SET = 1;
  	Decryp[0] = 'K'; Decryp[1] = 'e'; Decryp[2] = 'i'; Decryp[3] ='l';
	Decryp[4] = ' '; Decryp[5] = 'M'; Decryp[6] = 'B';Decryp[7] ='C';
	Decryp[8] = '2'; Decryp[9] = '3'; Decryp[10] = '0';Decryp[11] ='0';
	Decryp[12] = ' '; Decryp[13] = 'H'; Decryp[14] = 'I';Decryp[15] = 'D';	
	
	InReport[0] = AUTHENTICATION_CODE;	
  }
  else//if(OutReport[0] == CHECK_EXISTANCE_CODE)
  {
    FIO2SET = 2;
	Decryp[0] = 'C'; Decryp[1] = 'h'; Decryp[2] = 'e'; Decryp[3] ='c';
	Decryp[4] = 'k'; Decryp[5] = ' '; Decryp[6] = 'e';Decryp[7] ='x';
	Decryp[8] = 'i'; Decryp[9] = 's'; Decryp[10] = 't';Decryp[11] ='a';
	Decryp[12] = 'n'; Decryp[13] = 'c'; Decryp[14] = 'e';Decryp[15] = '!';
	InReport[0] = CHECK_EXISTANCE_CODE;
  }
  Cipher(Decryp, Encryp, Nr);
      	
}


/* Main Program */

int main (void) {

  PINSEL10 = 0;                             /* Disable ETM interface */
  FIO2DIR = LEDMSK;                         /* LEDs, port 2, bit 0~7 output only */

  lcd_init();
  lcd_clear();
  lcd_print ("MCB2300 HID Demo");
  set_cursor (0, 1);
  lcd_print ("  www.keil.com  ");

  	Nr = 128;
	Nk = Nr / 32;
	Nr = Nk + 6;
	KeyExpansion(Key, Nk, Nr);

  USB_Init();                               /* USB Initialization */
  USB_Connect(TRUE);                        /* USB Connect */

  while (1);                                /* Loop forever */
}
