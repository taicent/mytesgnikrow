/******************************************************************************/
/* SerialLogger.h: Low Level Serial Routines and Logger for Degug             */
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

#ifndef MAX_LEN
#define MAX_LEN	32
#endif

#ifndef NULL
#define NULL	0
#endif

// String Library
#include "string.h"

// Function prototype
void init_serial (void);		// Init Serial UART0 Port
int sendchar (int ch);			// Send char 'ch'
int getkey (void);				// Get a char

// Send one new line string
void newline(void);

// Send string 'str' to HyperTerminal
void sendstring(char *str);

// Get a string to 'str', specific 'size' from HyperTerminal
void getstring(char *str, int size);

// Get a number from HyperTerminal
int getnumber(void);

// Print a log string to HyperTerminal
void PLog(char *str);
void PLogN(char *str);
void PLogNum(int num);

// String processing
void reverse(char s[]);			// Reverse a string
void itoa(int n, char s[]);		// Chang string 's' to number 'n'

