Attribute VB_Name = "basWinAPI"
'Rodney robot - Windows API functions
'Copyright (C) 2003  Bob Mottram

'This program is free software; you can redistribute it and/or modify
'it under the terms of the GNU General Public License as published by
'the Free Software Foundation; either version 2 of the License, or
'(at your option) any later version.

'This program is distributed in the hope that it will be useful,
'but WITHOUT ANY WARRANTY; without even the implied warranty of
'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'GNU General Public License for more details.

'You should have received a copy of the GNU General Public License
'along with this program; if not, write to the Free Software
'Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Option Explicit



Public Type RGBthingy
  Value As Long
End Type

Public Type RGBpoint
  Red As Byte
  Green As Byte
  Blue As Byte
End Type

Public RGBsource As RGBthingy
Public RGBdest As RGBpoint



Public Const ws_child As Long = &H40000000
Public Const ws_visible As Long = &H10000000
Public Const SWP_NOSIZE As Long = &H1&
Public Const SWP_NOMOVE As Long = &H2&
Public Const SWP_NOZORDER As Long = &H4&
Public Const SWP_NOSENDCHANGING As Long = &H400&   ' /* Don't send WM_WINDOWPOSCHANGING */
Public Const HWND_BOTTOM As Long = 1&

Public Const SM_CYCAPTION As Long = 4
Public Const SM_CXBORDER As Long = 5
Public Const SM_CYBORDER   As Long = 6

Public Const SM_CYMENU   As Long = 15

Public Const SM_CXEDGE     As Long = 45
Public Const SM_CYEDGE    As Long = 46

Declare Function ShellAbout Lib "shell32" Alias "ShellAboutA" _
                            (ByVal hwnd As Long, _
                            ByVal szApp As String, _
                            ByVal szOtherStuff As String, _
                            ByVal hIcon As Long) As Long
Declare Function SetWindowTextAsLong Lib "user32" Alias "SetWindowTextA" (ByVal hwnd As Long, ByVal LPCSTR As Long) As Long ' C BOOL
Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long
Declare Function SetParent Lib "user32" (ByVal hWndChild As Long, ByVal hWndNewParent As Long) As Long
Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Declare Function DestroyWindow Lib "user32" (ByVal hwnd As Long) As Long 'C BOOL
Declare Function GetDiskFreeSpace Lib "kernel32" Alias "GetDiskFreeSpaceA" _
                            (ByVal lpRootPathName As String, _
                            lpSectorsPerCluster As Long, _
                            lpBytesPerSector As Long, _
                            lpNumberOfFreeClusters As Long, _
                            lpTtoalNumberOfClusters As Long) As Long 'C BOOL

'== Global Memory Functions ==================================================
Declare Function GlobalAlloc Lib "kernel32" (ByVal wFlags As Long, ByVal dwBytes As Long) As Long
Declare Function GlobalFree Lib "kernel32" (ByVal hMem As Long) As Long
Declare Function GlobalLock Lib "kernel32" (ByVal hMem As Long) As Long
Declare Function GlobalUnlock Lib "kernel32" (ByVal hMem As Long) As Long

Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)

Declare Function lstrcpy Lib "kernel32" Alias "lstrcpyA" (ByVal lpStringDest As Long, ByVal lpStringSrc As Long) As Long
Declare Sub CopyPTRtoANY Lib "kernel32.dll" Alias "RtlMoveMemory" (ByRef Dest As Any, ByVal PtrSrc As Long, ByVal length As Long)
Declare Sub CopyPTRtoLONG Lib "kernel32.dll" Alias "RtlMoveMemory" (ByRef LONGDest As Long, ByVal PtrSrc As Long, ByVal length As Long)

Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Declare Function CreateBitmap Lib "gdi32" (ByVal nWidth As Long, ByVal nHeight As Long, ByVal nPlanes As Long, ByVal nBitCount As Long, lpBits As Any) As Long
Public Declare Function SetBkColor Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
Public Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long
Public Declare Function CreateCompatibleBitmap Lib "gdi32" (ByVal hdc As Long, ByVal nWidth As Long, ByVal nHeight As Long) As Long
Public Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long
Public Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) As Long
Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long

Public Const GMEM_MOVEABLE = &H2&
Public Const GMEM_SHARE = &H2000&
Public Const GMEM_ZEROINIT = &H40&


'VFW stuff
Global Const WM_USER = 1024
Global Const WM_CAP_EDIT_COPY = WM_USER + 30
Global Const wm_cap_driver_connect = WM_USER + 10
Global Const wm_cap_set_preview = WM_USER + 50
Global Const wm_cap_set_overlay = WM_USER + 51
Global Const WM_CAP_SET_PREVIEWRATE = WM_USER + 52
Global Const WM_CAP_SEQUENCE = WM_USER + 62
Global Const WM_CAP_SINGLE_FRAME_OPEN = WM_USER + 70
Global Const WM_CAP_SINGLE_FRAME_CLOSE = WM_USER + 71
Global Const WM_CAP_SINGLE_FRAME = WM_USER + 72

Public Const WM_CAP_DLG_VIDEOFORMAT As Long = WM_USER + 41

Global Const DRV_USER = &H4000
Global Const DVM_DIALOG = DRV_USER + 100
Global Const WM_CAP_DRIVER_DISCONNECT As Long = WM_USER + 11
Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Declare Function capCreateCaptureWindow Lib "avicap32.dll" Alias "capCreateCaptureWindowA" (ByVal a As String, ByVal b As Long, ByVal c As Integer, ByVal d As Integer, ByVal e As Integer, ByVal f As Integer, ByVal g As Long, ByVal h As Integer) As Long
Declare Function SendMessageAsLong Lib "user32" Alias "SendMessageA" _
                                            (ByVal hwnd As Long, _
                                            ByVal wMsg As Long, _
                                            ByVal wParam As Long, _
                                            ByVal lParam As Long) As Long
Global Const WM_CAP_GRAB_FRAME As Long = WM_USER + 60


Global Const FLOODFILLBORDER = 0
Global Const FLOODFILLSURFACE = 1
Global Const PS_SOLID = 0
Global Const PS_DASH = 1
Global Const PS_DOT = 2
Global Const PS_DASHDOT = 3
Global Const PS_DASHDOTDOT = 4
Global Const PS_NULL = 5
Global Const PS_INSIDEFRAME = 6
'Declare Function SelectObject Lib "gdi32.dll" (ByVal hdc As Long, ByVal hObject As Long) As Long
Declare Function CreateSolidBrush Lib "gdi32.dll" (ByVal crColor As Long) As Long
Declare Function CreateHatchBrush Lib "gdi32.dll" (ByVal nIndex As Long, ByVal crColor As Long) As Long
Declare Function CreatePen Lib "gdi32.dll" (ByVal fnPenStyle As Long, ByVal nWidth As Long, ByVal crColor As Long) As Long
Declare Function ExtFloodFill Lib "gdi32.dll" (ByVal hdc As Long, ByVal nXStart As Long, ByVal nYStart As Long, ByVal crColor As Long, ByVal fuFillType As Long) As Long








Public Declare Function VarPtrArray Lib "msvbvm60.dll" Alias "VarPtr" (Ptr() As Any) As Long

Public Type SAFEARRAYBOUND
    cElements As Long
    lLbound As Long
End Type

Public Type SAFEARRAY2D
    cDims As Integer
    fFeatures As Integer
    cbElements As Long
    cLocks As Long
    pvData As Long
    Bounds(0 To 1) As SAFEARRAYBOUND
End Type


'== Global Memory Functions ==================================================


Public Filter_Name As String




