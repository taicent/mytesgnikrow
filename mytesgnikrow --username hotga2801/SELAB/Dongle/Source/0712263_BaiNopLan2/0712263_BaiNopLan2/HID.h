
#ifndef __HID_H__
#define __HID_H__


void  HID_Init();
void  HID_UnInit();
int   HID_FindDevices();
BOOL  HID_GetName(int num, char *buf, int sz);
BOOL  HID_Open(int num);
void  HID_Close();
BOOL  HID_Read (BYTE *buf, DWORD sz, DWORD *cnt);
BOOL  HID_Write(BYTE *buf, DWORD sz, DWORD *cnt);


#endif /* __HID_H__ */
