#include "stdafx.h"
#include "Public.h"

PUCHAR
CreatePatternBuffer(
    IN ULONG Length
    )
{
    unsigned int i;
    PUCHAR p, pBuf;

    pBuf = (PUCHAR)malloc(Length);
    if( pBuf == NULL ) {
        return NULL;
    }

    p = pBuf;

    for(i=0; i < Length; i++ ) {
        *p = (UCHAR)i;
        p++;
    }

    return pBuf;
}

PUCHAR
CreateBuffer(
    IN ULONG Length
    )
{
    PUCHAR pBuf;

    pBuf = (PUCHAR)malloc(Length);
    if( pBuf == NULL ) {
        return NULL;
    }

	memset(pBuf, 0, Length);

    return pBuf;
}

// Device path
TCHAR* ListDevice[] = {	_T("\\\\?\\Root#LPCBOARD#0000#{BF433C13-0D7D-4ef8-B5A5-D5CCC7F3C416}"),
						_T("\\\\?\\USB#Vid_C251&Pid_1703#LEDCTRL373468#{BF433C13-0D7D-4ef8-B5A5-D5CCC7F3C416}")	};

