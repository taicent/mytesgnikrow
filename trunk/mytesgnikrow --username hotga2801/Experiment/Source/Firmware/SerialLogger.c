/******************************************************************************/
/* SerialLogger.c: Low Level Serial Routines and Logger for Degug             */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/* 																			  */
/* Description:																  */
/* 		Use this module to print the logger string to HypperTerminal		  */
/******************************************************************************/

#include <LPC23xx.H>                     /* LPC23xx definitions               */
#include "SerialLogger.h"

int USE_SERIAL_LOGGER = 1;

#define UART0                            /* Use UART 0 for printf             */

/* If UART 0 is used for printf                                               */
#ifdef UART0
  #define UxFDR  U0FDR
  #define UxLCR  U0LCR
  #define UxDLL  U0DLL
  #define UxDLM  U0DLM
  #define UxLSR  U0LSR
  #define UxTHR  U0THR
  #define UxRBR  U0RBR
/* If UART 1 is used for printf                                               */
#elif defined(UART1)
  #define UxFDR  U1FDR
  #define UxLCR  U1LCR
  #define UxDLL  U1DLL
  #define UxDLM  U1DLM
  #define UxLSR  U1LSR
  #define UxTHR  U1THR
  #define UxRBR  U1RBR
#endif


void init_serial (void)  {               /* Initialize Serial Interface       */
  #ifdef UART0
    PINSEL0 |= 0x00000050;               /* Enable TxD0 and RxD0              */
  #elif defined (UART1)
    PINSEL0 |= 0x40000000;               /* Enable TxD1                       */
    PINSEL1 |= 0x00000001;               /* Enable RxD1                       */
  #endif
  UxFDR    = 0;                          /* Fractional divider not used       */
  UxLCR    = 0x83;                       /* 8 bits, no Parity, 1 Stop bit     */
  UxDLL    = 78;                         /* 9600 Baud Rate @ 12.0 MHZ PCLK    */
  UxDLM    = 0;                          /* High divisor latch = 0            */
  UxLCR    = 0x03;                       /* DLAB = 0                          */
}


/* Implementation of putchar (also used by printf function to output data)    */
int sendchar (int ch)  {                 /* Write character to Serial Port    */

  while (!(UxLSR & 0x20));

  return (UxTHR = ch);
}


int getkey (void)  {                     /* Read character from Serial Port   */

  while (!(UxLSR & 0x01));

  return (UxRBR);
}

// Send new line
void newline(void)
{
	sendchar(13);
	sendchar(10);
}

// Send a string to Hyper Terminal
void sendstring(char *str)
{
	int i;
	if (str == NULL)
		return;

	for (i = 0; i < strlen(str); i++)
		sendchar(str[i]);
}

void getstring(char *str, int size)
{
	int i;
	char ch;

	i = 0;
	while (1)
	{
		// Nhan 1 ki tu
		ch = getkey();
			
		// Neu la Enter, ket thuc viec nhap chuoi
		if (ch == 13)
			break;

		// Neu la Backspace va chuoi rong
		if (ch == 8 && i == 0)
			continue;

		// Dung nhap khi qua kich thuoc	
		if (i >= size - 1 && ch != 8)
			continue;

		if (ch == 8)
		{
			str[--i] = '\0';
			sendchar(8);
			sendchar(' ');
		}
		else
			str[i++] = ch;

		sendchar(ch);
	}
}

// Reverse a string
void reverse(char s[])
{
	int c, i, j;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

// Change a number to string
void itoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0)
		n = -n;

	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0)
		s[i++] = '-';

	s[i] = '\0';
	reverse(s);
}

// Receive a number from Hyper Terminal
int getnumber(void)
{
	int result;
	int bSign;
	char numKey;
	char ch;

	result = 0;
	bSign = 1;
	numKey = 0;

	while (1)
	{
		// Nhan ki tu tu Hyper
		ch = getkey();

		// Chi nhan so, -, backspace, Enter
		if (!( (ch >= '0' && ch <= '9') ||
			    ch == '-' || ch == 13 || ch == 8) )
			continue;

		// Neu vuot qua so luong ki tu
		if (numKey > 8 && ch != 8)
			continue;

		// Dung khi nhan Enter
		if (ch == 13)
			break;

		// Doi dau thanh am
		if (result == 0 && ch == '-' && bSign == 1)
			bSign = -1;			
		
		// Nhan backspace, lui mot so
		if (ch == 8 && (result != 0 || bSign == -1))
		{
			result /= 10;

			// Send a space to delete current character
			sendchar(8);
			sendchar(' ');
		}

		if (ch >= '0' && ch <= '9')
		{
			result *= 10;
			result += (ch - '0');
		}

		// Xuat ra man hinh Hyper Terminal
		if (ch != 8)
		{
			sendchar(ch);
			numKey++;
		}
		else if (numKey > 0)
		{
			sendchar(ch);
			numKey--;
		}
	}

	return result * bSign;
}

void PLog(char *str)
{
	if (!USE_SERIAL_LOGGER)
		return;

	sendstring(str);
	newline();
}

void PLogN(char *str)
{
	if (!USE_SERIAL_LOGGER)
		return;

	sendstring(str);
}

void PLogNum(int num)
{
	char str[MAX_LEN];

	if (!USE_SERIAL_LOGGER)
		return;

	itoa(num, str);
	sendstring(str);
}
