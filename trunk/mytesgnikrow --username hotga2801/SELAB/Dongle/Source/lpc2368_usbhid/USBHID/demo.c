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


BYTE InReport[USB_MAX_PACKET0];                              /* HID Input Report    */
                                            /*   Bit0   : Buttons  */
                                            /*   Bit1..7: Reserved */

BYTE OutReport[USB_MAX_PACKET0];       /* HID Out Report */
	                              /* Byte 0  Bit0..7: LEDs  */
	                              /* Byte 1..16  LCD line 1 */
	                              /* Byte 17..32 LCD line 2 */


/*
 *  Get HID Input Report -> InReport
 */

void GetInReport (void) {

  /*if ((FIO2PIN & PBINT) == 0) {             // Check if PBINT is pressed 
    InReport = 0x01;
  } else {
    InReport = 0x00;
  }*/
  InReport[0] = '1';
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
  InReport[15] = '0';
}


/*
 *  Set HID Output Report <- OutReport
 */

void SetOutReport (void) {

  FIO2CLR = LEDMSK;
  FIO2SET = OutReport[0];
  lcd_clear();                      /* clear LCD Display    */
  lcd_print (&OutReport[1]);        /* print first line     */
  set_cursor (0, 1);
  lcd_print (&OutReport[17]);       /* print second line    */
	
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

  USB_Init();                               /* USB Initialization */
  USB_Connect(TRUE);                        /* USB Connect */

  while (1);                                /* Loop forever */
}
