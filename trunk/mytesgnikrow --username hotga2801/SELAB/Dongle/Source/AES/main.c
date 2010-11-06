#include <LPC23XX.H>
#include <string.h>
#include "aes.h"

void InitUART1(void);
int GetKey(void);
int SendKey(int ch);
void SendString(char* str);
void SendEnter(void);

void SendStringFromNum(int num);
int GetNumFromString(void);

// Khoi tao chuc nang cho UART1
void InitUART1()
{
	// Khoi tao cho COM 1
	PINSEL0|=0x40000000;
	PINSEL1|=0x00000001;

	//Khoi tao cac thanh ghi cho UART1
	U1LCR = 0x83;
	U1DLL = 78;
	U1DLM = 0;
	U1LCR = 0x03;
}

// Nhan mot ky tu 
int GetKey(void)
{
	while(!(U1LSR &0x01));
	return U1RBR;
}

// Truyen mot ky tu
int SendKey(int ch)
{
	while(!(U1LSR & 0x20));
	return (U1THR=ch);
}

// Truyen 1 chuoi str
void SendString(char* str)
{
	int i;
	int length = strlen(str);
	for(i = 0; i<length; i++)
	{
		SendKey(str[i]);
	}
}

// Truyen ky tu xuong dong
void SendEnter()
{
	SendKey('\r');
	SendKey('\n');
}

// Truyen 1 chuoi tu mot so
void SendStringFromNum(int num)
{
	int div = 1;
	int temp;
	char sign = 0;
	//Kiem tra dau
	if(num < 0)
	{
		num *= (-1);
		sign = 1;
	}
	
	while((num/div)>=10)
	{
		div*=10;
	}

	
	if(sign == 1)
	{
		SendKey('-');
	}

	while(div > 0)
	{
		temp = (num/div) + 48;
		SendKey(temp);
		num = num % div;
		div /= 10;			
	}
}

// Lay 1 so tu chuoi nhan duoc
int GetNumFromString(void)
{
	int res = 0;
	char sign = 0;
	int ch;

	ch = GetKey(); 
	SendKey(ch);
	if(ch == '-')
	{
		sign = 1;	
	}
	while(1)
	{
		if(ch == 13)
		{
			break;
		}
		else
		{
			if(ch >= 48 && ch <= 57)
			{
				ch -= 48;
				res = res*10 + ch;
			}
		}
		ch = GetKey();
		SendKey(ch);
	}
	if(sign == 1)
	{
		res *= (-1);
	}

	return res;	
}

int main()
{
	int i;
	int Nr = 0;
	int Nk = 0;
	unsigned char Key[32]= {0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f};
	unsigned char in[16]= {0x00  ,0x11  ,0x22  ,0x33  ,0x44  ,0x55  ,0x66  ,0x77  ,0x88  ,0x99  ,0xaa  ,0xbb  ,0xcc  ,0xdd  ,0xee  ,0xff};
	unsigned char out[16];
	InitUART1();

	while(Nr!=128 && Nr!=192 && Nr!=256)
	{
		SendString("Enter the length of Key(128, 192 or 256 only): ");
		Nr = GetKey();
	}

	Nk = Nr / 32;
	Nr = Nk + 6;
	
	KeyExpansion(Key, Nk, Nr);
	Cipher(in, out, Nr);

	SendString("Text encrypted: ");
	for(i = 0; i < Nb*4; i++)
	{
		SendStringFromNum(out[i]);		
	} 
	SendEnter();
	while(1);
}

