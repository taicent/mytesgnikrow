VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "The Human Body Project"
   ClientHeight    =   6885
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   9015
   LinkTopic       =   "Form1"
   ScaleHeight     =   6885
   ScaleWidth      =   9015
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picFace2 
      AutoRedraw      =   -1  'True
      Height          =   1860
      Left            =   5760
      ScaleHeight     =   1800
      ScaleWidth      =   2160
      TabIndex        =   6
      Top             =   4320
      Width           =   2220
   End
   Begin VB.PictureBox picFace 
      AutoRedraw      =   -1  'True
      Height          =   1860
      Left            =   3600
      ScaleHeight     =   1800
      ScaleWidth      =   2160
      TabIndex        =   5
      Top             =   4320
      Width           =   2220
   End
   Begin VB.PictureBox picAvatar 
      AutoRedraw      =   -1  'True
      Height          =   3810
      Left            =   4800
      ScaleHeight     =   1000
      ScaleMode       =   0  'User
      ScaleWidth      =   1000
      TabIndex        =   4
      Top             =   480
      Width           =   4125
   End
   Begin VB.PictureBox picOutput 
      AutoRedraw      =   -1  'True
      Height          =   1860
      Left            =   1440
      ScaleHeight     =   1800
      ScaleWidth      =   2160
      TabIndex        =   3
      Top             =   4320
      Width           =   2220
   End
   Begin VB.CommandButton cmdStop 
      Caption         =   "Stop"
      Height          =   375
      Left            =   4800
      TabIndex        =   2
      Top             =   45
      Width           =   2295
   End
   Begin VB.Timer timGrab 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   8040
      Top             =   4680
   End
   Begin VB.PictureBox Eye 
      AutoRedraw      =   -1  'True
      Height          =   3855
      Index           =   0
      Left            =   0
      ScaleHeight     =   253
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   317
      TabIndex        =   1
      Top             =   480
      Width           =   4815
   End
   Begin VB.CommandButton Preview 
      Caption         =   "&Start"
      Height          =   375
      Left            =   2310
      TabIndex        =   0
      Top             =   45
      Width           =   2415
   End
   Begin VB.Label lblName 
      Alignment       =   2  'Center
      Caption         =   "."
      Height          =   255
      Left            =   1440
      TabIndex        =   10
      Top             =   6480
      Width           =   6615
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      Caption         =   "Normalised face"
      Height          =   255
      Left            =   5760
      TabIndex        =   9
      Top             =   6240
      Width           =   2175
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Caption         =   "Face and detected features"
      Height          =   255
      Left            =   3600
      TabIndex        =   8
      Top             =   6240
      Width           =   2175
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "Body Sillouette"
      Height          =   255
      Left            =   1440
      TabIndex        =   7
      Top             =   6240
      Width           =   2175
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuCamera 
         Caption         =   "Camera Device"
      End
   End
   Begin VB.Menu mnuDetection 
      Caption         =   "Detection"
      Begin VB.Menu mnuDetectMotion 
         Caption         =   "Only detect visible motion"
      End
      Begin VB.Menu mnuDetectFaces 
         Caption         =   "Only detect faces"
      End
      Begin VB.Menu mnuDetectBody 
         Caption         =   "Fully body detection"
         Checked         =   -1  'True
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "Options"
      Begin VB.Menu mnuLearnFaces 
         Caption         =   "Enable learning of new faces"
         Checked         =   -1  'True
      End
      Begin VB.Menu mnuShowStages 
         Caption         =   "Show image processing stages"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'The human body project
'Copyright (C) 2004  Bob Mottram
'
'This program is free software; you can redistribute it and/or modify
'it under the terms of the GNU General Public License as published by
'the Free Software Foundation; either version 2 of the License, or
'(at your option) any later version.
'
'This program is distributed in the hope that it will be useful,
'but WITHOUT ANY WARRANTY; without even the implied warranty of
'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'GNU General Public License for more details.
'
'You should have received a copy of the GNU General Public License
'along with this program; if not, write to the Free Software
'Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



'NOTE: Make sure that you register CapStill.dll and FSFWrap.dll before running
'      this program.  You can do this using the following commandline

'         regsvr32 <filename>

'      After the program is run it creates a file called filters.txt, which
'      lists out all the WDM filters.  Make sure that the filter name is
'      correct for the cameras that you're using


Option Explicit

'location of the classifier data
Const classifier_filename As String = "c:\develop\hbp\faces.net"

'dimensions of the stereo image
Dim IMAGE_WIDTH As Long
Dim IMAGE_HEIGHT As Long
Dim reductionFactor_x As Long
Dim reductionFactor_y As Long
Const SMALL_IMAGE_SIZE = 80

'the busy flag just prevents VB from tripping over itself using the timer
Dim busy As Boolean

Dim gGraph(2) As IMediaControl
Dim gRegFilters(2) As Object
Dim gCapStill(2) As VBGrabber
Dim initialised(2) As Boolean
Dim cameraIndex As Long

Dim hMemDc(2) As Long

'bitmaps for left and right images
'Dim BITMAP() As Byte
'Dim initBitmap As Boolean

Dim camerasRunning As Boolean


'GDI functions to draw a DIBSection into a DC
Private Declare Function CreateCompatibleDC Lib "gdi32" _
    (ByVal hdc As Long) As Long

Private Declare Function SelectObject Lib "gdi32" _
    (ByVal hdc As Long, ByVal hbitmap As Long) As Long

Private Declare Function BitBlt Lib "gdi32" _
    (ByVal hdc As Long, ByVal x As Long, ByVal y As Long, _
    ByVal width As Long, ByVal height As Long, _
    ByVal hdcSrc As Long, ByVal xSrc As Long, ByVal ySrc As Long, _
    ByVal mode As Long) _
    As Long

Private Declare Sub DeleteDC Lib "gdi32" _
    (ByVal hdc As Long)

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" _
    (Dest As Any, src As Any, ByVal count As Long)
    

'bitmaps for left and right images
Dim left_bmp() As Byte
Dim right_bmp() As Byte
Dim bmp() As Byte
Dim bmp2() As Byte

Const FACE_IMAGE_SIZE = 60
Dim face() As Byte
Dim normalisedFace() As Byte

Dim bma As IBitmapAccess
Dim initStereo As Boolean
Dim NoOfSegments As Long

'detection mode constants
Const DETECTIONMODE_MOTION = 0
Const DETECTIONMODE_FACES = 1
Const DETECTIONMODE_BODY = 2
Dim DETECTION_MODE As Integer

Private Sub GrabNow(cameraIndex As Integer)
  On Error GoTo GrabNow_err
  
  If (initialised(cameraIndex)) Then
    Set bma = gCapStill(cameraIndex).CapToMem
    Call ShowBitmap(bma, cameraIndex)
  End If
  
GrabNow_exit:
  Exit Sub
GrabNow_err:
  Resume GrabNow_exit
End Sub


Private Sub CameraPreview(DriverName As String, cameraIndex As Integer)
  On Error GoTo CameraPreview_err
  
  Dim i As Integer
  Dim index As Integer
  Dim xbar As CrossbarInfo
  Dim pinOut As IPinInfo
  Dim idx As Long
  Dim filter As IRegFilterInfo
  Dim fGrab As IFilterInfo
  Dim fSrc As IFilterInfo
  Dim pin As String
  Dim found As Boolean
  Dim pSC As StreamConfig
  Dim pinIn As IPinInfo
  Dim ppropOut As PinPropInfo
  Dim strFilters As String
  Dim pinErr As Boolean
  
  pinErr = False
  initialised(cameraIndex) = False
  
  'make a new graph
  Set gGraph(cameraIndex) = Nothing
  Set gCapStill(cameraIndex) = Nothing
  Set gGraph(cameraIndex) = New FilgraphManager
  Set gRegFilters(cameraIndex) = gGraph(cameraIndex).RegFilterCollection
    
  'add the grabber including vb wrapper and default props
  found = False
  i = 0
  While (i < gRegFilters(cameraIndex).count) And (Not found)
    Call gRegFilters(cameraIndex).Item(i, filter)
    If (filter.Name = "SampleGrabber") Then
      filter.filter fGrab
      'wrap this filter in the capstill vb wrapper
      'also sets rgb-24 media type and other properties
      Set gCapStill(cameraIndex) = New VBGrabber
      gCapStill(cameraIndex).FilterInfo = fGrab
      found = True
    End If
    i = i + 1
  Wend
    
  
  Open App.Path & "\filters.txt" For Output As #1
  'strFilters = ""
  i = 0
  While (i < gRegFilters(cameraIndex).count)
    Call gRegFilters(cameraIndex).Item(i, filter)
    Print #1, filter.Name
    'strFilters = strFilters & filter.Name & Chr(13)
    i = i + 1
  Wend
  Close #1
  'MsgBox strFilters
  
  
  'add the selected source filter
  'WDM drivers for the cameras can be identified by the word "QuickCam" in their title
  index = 0
  found = False
  i = 0
  While (i < gRegFilters(cameraIndex).count) And (Not found)
    Call gRegFilters(cameraIndex).Item(i, filter)
    
    If (InStr(LCase(filter.Name), LCase(DriverName)) > 0) Then
      If (index = cameraIndex) Then
        filter.filter fSrc
        found = True
      End If
      index = index + 1
    End If
    i = i + 1
  Wend
  
    
  'find first output on src
  found = False
  i = 0
  While (i < fSrc.Pins.count) And (Not found)
    Call fSrc.Pins.Item(i, pinOut)
    If (pinOut.Direction = 1) Then
      found = True
    End If
    i = i + 1
  Wend
    
  'restore specified file before dlg
  Set pSC = New StreamConfig
  pSC.pin = pinOut
  If (pSC.SupportsConfig) Then
    If (Dir$("mtsave.mt") <> "") Then
      'pSC.Restore ("mtsave.mt")
    End If
  End If
    
  'show format of output pin before rendering
  Set ppropOut = New PinPropInfo
  ppropOut.pin = pinOut
  
  'find first input on grabber and connect
  found = False
  i = 0
  While (i < fGrab.Pins.count) And (Not found)
    Call fGrab.Pins.Item(i, pinIn)
    If (pinIn.Direction = 0) Then
      pinErr = True
      pinOut.Connect pinIn
      pinErr = False
      found = True
    End If
    i = i + 1
  Wend
    
  ' find grabber output pin and render
  found = False
  i = 0
  While (i < fGrab.Pins.count) And (Not found)
    Call fGrab.Pins.Item(i, pinOut)
    If (pinOut.Direction = 1) Then
      pinOut.Render
      found = True
    End If
    i = i + 1
  Wend
       
  ' run graph and we are successfully in preview mode
  Call gGraph(cameraIndex).Run
  
  'putting a delay here just gives the camera some time to
  'initialise before trying to grab frames from it
  Dim currTime As Long
  Dim startTime As Long
  currTime = Timer
  startTime = currTime
  While (currTime < startTime + 5)
    DoEvents
    currTime = Timer
  Wend
  
  'camera has been initialised
  initialised(cameraIndex) = True
  timGrab.Enabled = True
  
CameraPreview_exit:
  Exit Sub
CameraPreview_err:
  If (pinErr) Then
    Resume CameraPreview_exit
  End If
  
  MsgBox "frmQuickCamStereo/CameraPreview/" & Err & "/" & Error$(Err)
  Resume CameraPreview_exit
End Sub




Private Sub chkShow_Click()

End Sub

Private Sub cmdStop_Click()
  Dim i As Integer
  
  For i = 0 To 1
    If (initialised(i)) Then
      initialised(i) = False
      Call gGraph(i).Stop
      Set gGraph(i) = Nothing
      Set gRegFilters(i) = Nothing
      Set gCapStill(i) = Nothing
    End If
  Next
  
  camerasRunning = False
End Sub


Private Sub Form_Load()
  Dim i As Long

  DETECTION_MODE = DETECTIONMODE_BODY
  IMAGE_WIDTH = 320 'must be a multiple of SMALL_IMAGE_SIZE
  IMAGE_HEIGHT = 320 'must be a multiple of SMALL_IMAGE_SIZE
  reductionFactor_x = IMAGE_WIDTH / SMALL_IMAGE_SIZE
  reductionFactor_y = IMAGE_HEIGHT / SMALL_IMAGE_SIZE
  busy = False
  'initBitmap = False
  initStereo = False
  ReDim left_bmp(IMAGE_WIDTH * IMAGE_HEIGHT * 3)
  ReDim prev_left_bmp(IMAGE_WIDTH * IMAGE_HEIGHT * 3)
  ReDim bmp(IMAGE_WIDTH * IMAGE_HEIGHT * 3)
  ReDim bmp2((IMAGE_WIDTH / reductionFactor_x) * (IMAGE_HEIGHT / reductionFactor_y) * 3)
  ReDim face(FACE_IMAGE_SIZE * FACE_IMAGE_SIZE * 3)
  ReDim normalisedFace((FACE_IMAGE_SIZE / 2) * (FACE_IMAGE_SIZE / 2) * 3)
  
  For i = 0 To (IMAGE_WIDTH * IMAGE_HEIGHT * 3) - 1
    prev_left_bmp(i) = 0
  Next
  Call RChbp_load(classifier_filename)
  Call loadSettings
  Call RChbp_enableLearning(True)
  
End Sub


Private Sub Form_Unload(Cancel As Integer)
  Call RChbp_save(classifier_filename)
  Call saveSettings
End Sub

Private Sub mnuCamera_Click()
  If (camerasRunning = False) Then
    frmFilter.Show 1
    Else
    MsgBox "You cannot change the camera device while it is running"
  End If
End Sub


Private Sub mnuDetectBody_Click()
  Call RChbp_setDetectionMode(DETECTIONMODE_BODY)
  mnuDetectMotion.Checked = False
  mnuDetectFaces.Checked = False
  mnuDetectBody.Checked = True
  DETECTION_MODE = DETECTIONMODE_BODY
End Sub

Private Sub mnuDetectFaces_Click()
  Call RChbp_setDetectionMode(DETECTIONMODE_FACES)
  mnuDetectMotion.Checked = False
  mnuDetectFaces.Checked = True
  mnuDetectBody.Checked = False
  DETECTION_MODE = DETECTIONMODE_FACES
  mnuShowStages.Checked = True
  picAvatar.Cls
End Sub

Private Sub mnuDetectMotion_Click()
  Call RChbp_setDetectionMode(DETECTIONMODE_MOTION)
  mnuDetectMotion.Checked = True
  mnuDetectFaces.Checked = False
  mnuDetectBody.Checked = False
  DETECTION_MODE = DETECTIONMODE_MOTION
  picAvatar.Cls
End Sub

Private Sub mnuLearnFaces_Click()
  Call RChbp_enableLearning(mnuLearnFaces.Checked)
  mnuLearnFaces.Checked = Not mnuLearnFaces.Checked
End Sub

Private Sub mnuShowStages_Click()
  mnuShowStages.Checked = Not mnuShowStages.Checked
End Sub

Private Sub Preview_Click()
'preview using the appropriate WDM filter name for the cameras
'if in doubt about the filter name have a look through filters.txt

  If (Filter_Name = "") Then
    frmFilter.Show 1
  End If

  'start rolling the camera
  Call CameraPreview(Filter_Name, 0)
  
  Call init
  
  camerasRunning = True
End Sub



Private Sub SaveBitmap(cameraIndex As Integer, filename As String)
  If (initialised(cameraIndex)) Then
    gCapStill(cameraIndex).filename = filename
    gCapStill(cameraIndex).CaptureStill
  End If
End Sub



Private Sub ShowBitmap(bma As IBitmapAccess, cameraIndex As Integer)
  'set correct size of image and then
  'BitBlt to the picture control's HDC
    
  Dim hbm As Long
  Dim hOldBM As Long
  Dim initEye(2) As Boolean
  Static flip(2) As Boolean
  
  If (Not initEye(cameraIndex)) Then
    initEye(cameraIndex) = True
    Eye(cameraIndex).width = bma.width * Screen.TwipsPerPixelX
    Eye(cameraIndex).height = bma.height * Screen.TwipsPerPixelY
    hMemDc(cameraIndex) = CreateCompatibleDC(Eye(cameraIndex).hdc)
  End If
  
  hbm = bma.DIBSection
    
  hOldBM = SelectObject(hMemDc(cameraIndex), hbm)
  BitBlt Eye(cameraIndex).hdc, 0, 0, bma.width, bma.height, hMemDc(cameraIndex), 0, 0, &HCC0020
  SelectObject hMemDc(cameraIndex), hOldBM
  Eye(cameraIndex).Refresh

End Sub


Private Sub getSegmentedImage(pic As PictureBox)
  On Error GoTo getSegmentedImage_err
  
  Dim x As Long
  Dim y As Long
  Dim c As Long
  Dim img As ClassImage
  Dim p As Byte
  Dim n As Long
  Dim w As Long
  Dim h As Long
  
  If ((reductionFactor_x > 0) And (reductionFactor_y > 0)) Then
  
    w = IMAGE_WIDTH
    w = w / reductionFactor_x
    h = IMAGE_HEIGHT
    h = h / reductionFactor_y
    pic.ScaleWidth = w
    pic.ScaleHeight = h
  
    Set img = New ClassImage
    Call img.init(CInt(w), CInt(h))
    Call RChbp_getImage(bmp2, 2)  '5
  
    n = 0
    For y = 0 To h - 1
      For x = 0 To w - 1
        For c = 0 To 2
          p = bmp2(n)
          Call img.setPoint(CInt(x), CInt(y), CInt(2 - c), p)
          n = n + 1
        Next
      Next
    Next

    Call img.Show(pic)
    Set img = Nothing
  End If
  
getSegmentedImage_exit:
  Exit Sub
getSegmentedImage_err:
  If (Err = 6) Then
    Set img = Nothing
    Resume getSegmentedImage_exit
  End If

  MsgBox "frmMain/getSegmentedImage/" & Err & "/" & Error$(Err)
  Resume getSegmentedImage_exit
End Sub



Private Sub getFaceImage(pic As PictureBox)
  On Error GoTo getFaceImage_err
  
  Dim x As Long
  Dim y As Long
  Dim c As Long
  Dim img As ClassImage
  Dim p As Byte
  Dim n As Long
  Dim w As Long
  Dim h As Long
  
  pic.ScaleWidth = FACE_IMAGE_SIZE
  pic.ScaleHeight = FACE_IMAGE_SIZE
  
  Set img = New ClassImage
  Call img.init(FACE_IMAGE_SIZE, FACE_IMAGE_SIZE)
  Call RChbp_getFaceImage(face)
  
  n = 0
  For y = 0 To FACE_IMAGE_SIZE - 1
    For x = 0 To FACE_IMAGE_SIZE - 1
      For c = 0 To 2
        p = face(n)
        Call img.setPoint(CInt(x), CInt(y), CInt(2 - c), p)
        n = n + 1
      Next
    Next
  Next
  
  Call img.Show(pic)
  Set img = Nothing
  
getFaceImage_exit:
  Exit Sub
getFaceImage_err:
  If (Err = 6) Then
    Set img = Nothing
    Resume getFaceImage_exit
  End If

  MsgBox "frmMain/getFaceImage/" & Err & "/" & Error$(Err)
  Resume getFaceImage_exit
End Sub



Private Sub getNormalisedFaceImage(pic As PictureBox)
  On Error GoTo getFaceImage_err
  
  Dim x As Long
  Dim y As Long
  Dim c As Long
  Dim img As ClassImage
  Dim p As Byte
  Dim n As Long
  Dim w As Long
  Dim h As Long
    
  If (RChbp_getNormalisedFaceImage(normalisedFace) = 1) Then
    pic.ScaleWidth = FACE_IMAGE_SIZE / 2
    pic.ScaleHeight = FACE_IMAGE_SIZE / 2
    
    Set img = New ClassImage
    Call img.init(FACE_IMAGE_SIZE / 2, FACE_IMAGE_SIZE / 2)
  
    n = 0
    For y = 0 To (FACE_IMAGE_SIZE / 2) - 1
      For x = 0 To (FACE_IMAGE_SIZE / 2) - 1
        For c = 0 To 2
          p = normalisedFace(n)
          Call img.setPoint(CInt(x), CInt(y), CInt(2 - c), p)
          n = n + 1
        Next
      Next
    Next
  
    Call img.Show(pic)
    Set img = Nothing
  End If
  
getFaceImage_exit:
  Exit Sub
getFaceImage_err:
  If (Err = 6) Then
    Set img = Nothing
    Resume getFaceImage_exit
  End If

  MsgBox "frmMain/getNormalisedFaceImage/" & Err & "/" & Error$(Err)
  Resume getFaceImage_exit
End Sub





Private Sub timGrab_Timer()
'Eye eye
  Dim i As Integer
  Static pic_index As Long
  
  If (initialised(0)) And (camerasRunning) And (Not busy) Then
  
    busy = True
  
    'grab images from the cameras
    'note that two frames are averaged in order to try to 'defuzz' the image
    For i = 0 To 1
      Call GrabNow(0)
      Call picToBitmap(0, i, False)
    Next
  
    'do the calculations
    Call RChbp_update(cameraIndex, left_bmp, IMAGE_WIDTH, IMAGE_HEIGHT, NoOfSegments)

    If (DETECTION_MODE > DETECTIONMODE_MOTION) Then
      If (mnuShowStages.Checked) Then
        Call getSegmentedImage(picOutput)
        Call getFaceImage(picFace)
        Call getNormalisedFaceImage(picFace2)
      End If
      
      Else
      
      Call showMotionBox(Eye(0))
    End If
    
    'picOutput.Cls
    'Call showSegments(picOutput)
    
    If (DETECTION_MODE >= DETECTIONMODE_BODY) Then
      Call showAvatar(picAvatar)
    End If
    
    'SavePicture Eye(0).image, "c:\temp\pics\test" & pic_index & ".bmp"
    'SavePicture picAvatar.image, "c:\temp\pics\avatar" & pic_index & ".bmp"
    'SavePicture picOutput.image, "c:\temp\pics\segments" & pic_index & ".bmp"
    pic_index = pic_index + 1

    busy = False
    DoEvents
    
  End If
  
End Sub




Private Sub init()
  
End Sub



Private Sub picToBitmap_old(index As Integer, bma As IBitmapAccess)
  On Error GoTo picToBitmap_err
  
  Dim cDib As New cDIBsection
  Dim tSA As SAFEARRAY2D
  Dim bDib() As Byte
  Dim x As Integer
  Dim y As Integer
  Dim c As Integer
  Dim n As Long
  Dim xx As Integer
  Dim yy As Integer
  Dim hght As Integer
  Dim w As Long
  Dim h As Long
  Dim pixels As Long
  
  cDib.Create bma.width, bma.height
  cDib.LoadPictureBlt Eye(index).hdc, 0, 0, bma.width, bma.height

  If (Not initStereo) Then
    w = IMAGE_WIDTH
    h = IMAGE_HEIGHT
    pixels = w * h * 3
    ReDim left_bmp(pixels)
    ReDim right_bmp(pixels)
    initStereo = True
  End If

  If (cDib.width > 0) Then

    With tSA
      .cbElements = 1
      .cDims = 2
      .Bounds(0).lLbound = 0
      .Bounds(0).cElements = cDib.height
      .Bounds(1).lLbound = 0
      .Bounds(1).cElements = cDib.BytesPerScanLine
      .pvData = cDib.DIBSectionBitsPtr
    End With
    CopyMemory ByVal VarPtrArray(bDib), VarPtr(tSA), 4
  
    hght = cDib.height - 1
    'If (hght > 400) Then hght = 400
    n = 0
    For y = IMAGE_HEIGHT - 1 To 0 Step -1
      yy = y / IMAGE_HEIGHT * hght
      For x = 0 To IMAGE_WIDTH - 1
        xx = x / IMAGE_WIDTH * cDib.width
        For c = 0 To 2
          If (index = 0) Then
            left_bmp(n) = bDib((xx * 3) + c, yy)
            Else
            right_bmp(n) = bDib((xx * 3) + c, yy)
          End If
          n = n + 1
        Next
      Next
    Next
  
    'tidy up
    CopyMemory ByVal VarPtrArray(bDib), 0&, 4
  End If
  
  Set cDib = Nothing
  
picToBitmap_exit:
  Exit Sub
picToBitmap_err:
  If (Err = 9) Then 'subscript out of range
    CopyMemory ByVal VarPtrArray(bDib), 0&, 4
    Resume picToBitmap_exit
  End If

  MsgBox "frmMain/picToBitmap/" & Err & "/" & Error$(Err)
  Resume picToBitmap_exit
End Sub



Private Sub picToBitmap(index As Integer, imgID As Integer, invert As Boolean)
  On Error GoTo picToBitmap_err
  
  Dim cDib As New cDIBsection
  Dim tSA As SAFEARRAY2D
  Dim bDib() As Byte
  Dim x As Integer
  Dim y As Integer
  Dim y2 As Integer
  Dim c As Integer
  Dim n As Long
  Dim xx As Integer
  Dim yy As Integer
  Dim xxx As Integer
  Dim yyy As Integer
  Dim hght As Integer
  Dim wdth As Integer
  Dim w As Long
  Dim h As Long
  Dim pixels As Long
  Dim p1 As Integer
  Dim p2 As Integer
  Dim heightAdjust As Integer
  
  If (index = 0) Then
    heightAdjust = 0 'EYE_TILT_ADJUST
    Else
    heightAdjust = 0
  End If
  
  cDib.Create bma.width, bma.height
  cDib.LoadPictureBlt Eye(index).hdc, 0, 0, bma.width, bma.height

  If (Not initStereo) Then
    w = IMAGE_WIDTH
    h = IMAGE_HEIGHT
    pixels = w * h * 3
    ReDim left_bmp(pixels)
    ReDim right_bmp(pixels)
    initStereo = True
  End If

  If (cDib.width > 0) Then

    With tSA
      .cbElements = 1
      .cDims = 2
      .Bounds(0).lLbound = 0
      .Bounds(0).cElements = cDib.height
      .Bounds(1).lLbound = 0
      .Bounds(1).cElements = cDib.BytesPerScanLine
      .pvData = cDib.DIBSectionBitsPtr
    End With
    CopyMemory ByVal VarPtrArray(bDib), VarPtr(tSA), 4
  
    hght = cDib.height - 1
    wdth = cDib.width - 1
    'If (hght > 400) Then hght = 400
    n = 0
    For y = IMAGE_HEIGHT - 1 + heightAdjust To 0 + heightAdjust Step -1
      If (y >= IMAGE_HEIGHT) Then
        y2 = IMAGE_HEIGHT - 1
        Else
        y2 = y
        If (y < 0) Then
          y2 = 0
        End If
      End If
      yy = y2 / IMAGE_HEIGHT * hght
      If (invert) Then
        yyy = hght - yy
        Else
        yyy = yy
      End If
      For x = 0 To IMAGE_WIDTH - 1
        xx = (x / IMAGE_WIDTH * cDib.width) + imgID
        If (invert) Then
          xxx = wdth - xx
          Else
          xxx = xx
        End If
        For c = 0 To 2
          If (index = 0) Then
            If (imgID = 0) Then
              left_bmp(n) = bDib((xxx * 3) + c, yyy)
              Else
              p1 = bDib((xxx * 3) + c, yyy)
              p2 = left_bmp(n)
              left_bmp(n) = CByte((p1 + p2) / 2)
            End If
            Else
            If (imgID = 0) Then
              right_bmp(n) = bDib((xxx * 3) + c, yyy)
              Else
              p1 = bDib((xxx * 3) + c, yyy)
              p2 = right_bmp(n)
              right_bmp(n) = CByte((p1 + p2) / 2)
            End If
          End If
          n = n + 1
        Next
      Next
    Next
  
    'tidy up
    CopyMemory ByVal VarPtrArray(bDib), 0&, 4
  End If
  
  Set cDib = Nothing
  
picToBitmap_exit:
  Exit Sub
picToBitmap_err:
  If (Err = 9) Then
    Resume picToBitmap_exit
  End If
  
  MsgBox "error " & Err & "/" & Error$(Err)
  Resume picToBitmap_exit
End Sub




Private Sub showSegments(pic As PictureBox)
  Dim i As Long
  Dim tx As Long
  Dim ty As Long
  Dim bx As Long
  Dim by As Long
  Dim r As Long
  Dim g As Long
  Dim b As Long
  Dim cx As Integer
  Dim cy As Integer
  Dim segmentID As Long
  Dim inMotion As Long
  Dim x As Integer
  Dim y As Integer
  Dim disparity As Long
  Dim w As Integer
  Dim h As Integer
  Dim min_height As Integer
  Dim max_height As Integer
  
  
  min_height = IMAGE_HEIGHT / 3
  max_height = IMAGE_HEIGHT * 3 / 4
  
  pic.ForeColor = RGB(0, 255, 0)
  pic.FillStyle = 1
  pic.ScaleWidth = IMAGE_WIDTH
  pic.ScaleHeight = IMAGE_HEIGHT
  For i = 0 To NoOfSegments - 1
    Call RChbp_getSegment(cameraIndex, i, tx, ty, bx, by, r, g, b, segmentID, inMotion, disparity)
    h = by - ty
    w = bx - tx
    If ((w < IMAGE_WIDTH / 2) And (h > min_height) And (h < max_height)) Then
      cx = tx + ((bx - tx) / 2)
      cy = ty + ((by - ty) / 2)
      pic.PSet (cx, cy)
      pic.Print segmentID
      
      pic.Line (tx, ty)-(bx, by), RGB(0, 255, 0), B
      
    End If
  Next
  
End Sub



Private Sub showAvatar(pic As PictureBox)
'shows a stick man avatar
  Dim leftShoulderElevation As Long
  Dim rightShoulderElevation As Long
  Dim centre_x As Integer
  Dim upperArmLength As Integer
  Dim leftelbow_x As Integer
  Dim leftelbow_y As Integer
  Dim leftwrist_x As Integer
  Dim leftwrist_y As Integer
  Dim rightelbow_x As Integer
  Dim rightelbow_y As Integer
  Dim rightwrist_x As Integer
  Dim rightwrist_y As Integer
  Dim neck_y As Integer
  Dim bodyHeight As Integer
  Dim leftElbowElevation As Long
  Dim rightElbowElevation As Long
  Dim leftShoulderAngle As Single
  Dim rightShoulderAngle As Single
  Dim leftWristAngle As Single
  Dim rightWristAngle As Single
  Dim lowerArmLength As Single
  Dim shoulderWidth As Single
  Dim top_colour As Long
  Dim bottom_colour As Long
  Dim skin_colour As Long
  Dim face_tx As Long
  Dim face_ty As Long
  Dim face_bx As Long
  Dim face_by As Long
  Dim bodyWidth As Long
  Dim scaleBody As Single
  Dim origin_x As Long
  Dim origin_y As Long
  Dim face_w As Long
  Dim face_h As Long
  Dim gazeDirection As Long
  Dim gazeShift As Integer
  Dim headTilt As Long
  Dim tiltshift As Integer
  Dim mouthWidth As Long
  Dim MouthOpen As Long
  Dim eyebrowsVertical As Long
  Dim drawScale As Single
  Dim bodyColour_red As Long
  Dim bodyColour_green As Long
  Dim bodyColour_blue As Long
  Dim backgroundColour_red As Long
  Dim backgroundColour_green As Long
  Dim backgroundColour_blue As Long
  Dim personID As Long
  Dim personName(20) As Byte
  Dim i As Integer
  Dim c As Integer
  Dim personNameStr As String
  Dim leftHandSize As Long
  Dim rightHandSize As Long
  Dim handMultiplier As Single
  Dim invalidFace As Boolean
  
  Call RChbp_getBodyPosture(0, leftShoulderElevation, rightShoulderElevation, leftElbowElevation, rightElbowElevation, leftHandSize, rightHandSize, face_tx, face_ty, face_bx, face_by, bodyWidth, gazeDirection, headTilt, mouthWidth, MouthOpen, eyebrowsVertical, bodyColour_red, bodyColour_green, bodyColour_blue, backgroundColour_red, backgroundColour_green, backgroundColour_blue, personID, invalidFace)
    
  Call RChbp_getPersonIdentity(personID, personName)
    
  If (personID > 0) Then
    personNameStr = ""
    For i = 1 To 18
      personNameStr = personNameStr & Chr(personName(i))
    Next
    lblName.Caption = personNameStr
    Else
    lblName.Caption = ""
  End If
    
  top_colour = RGB(bodyColour_red, bodyColour_green, bodyColour_blue) '&HC0&
  bottom_colour = &HC00000
  skin_colour = &HC0E0FF
  drawScale = 2
    
  mouthWidth = 20
    
  face_w = face_bx - face_tx
  face_h = face_by - face_ty
  If (face_w > 2) Then
    origin_x = ((face_tx + (face_w / 2)) * 100) / (IMAGE_WIDTH / reductionFactor_x)
    origin_y = (face_by * 100) / (IMAGE_HEIGHT / reductionFactor_y)
  End If
  
  If (bodyWidth > 0) And (face_w > 2) Then
    pic.Cls
    pic.BackColor = RGB(backgroundColour_red, backgroundColour_green, backgroundColour_blue)
  
    scaleBody = 0.5 + ((bodyWidth / CSng(IMAGE_WIDTH / reductionFactor_x)) * 2)
  
    centre_x = pic.ScaleWidth * (origin_x / 100#)
    shoulderWidth = CInt(((pic.ScaleWidth / 2) / 5) * scaleBody)
    upperArmLength = CInt(((pic.ScaleWidth / 2) / 2) * scaleBody)
    lowerArmLength = CInt((upperArmLength * 2 / 3) * scaleBody)
    neck_y = pic.ScaleHeight * (origin_y / 80#)
    bodyHeight = CInt((pic.ScaleHeight / 3) * scaleBody)
  
    leftShoulderAngle = (2 * 3.1415927) - ((3.1415927 * 1 / 8) + ((100 - leftShoulderElevation) * 3.1415927 / 260))
    leftelbow_x = centre_x + (upperArmLength * Sin(leftShoulderAngle))
    leftelbow_y = neck_y + (upperArmLength * Cos(leftShoulderAngle))
  
    leftWristAngle = leftShoulderAngle - (leftElbowElevation * 3.1415927 / 100) + (3.1415927 / 8)
    leftwrist_x = leftelbow_x + (lowerArmLength * Sin(leftWristAngle))
    leftwrist_y = leftelbow_y + (lowerArmLength * Cos(leftWristAngle))
  
    rightShoulderAngle = ((3.1415927 * 1 / 8) + ((100 - rightShoulderElevation) * 3.1415927 / 260))
    rightelbow_x = centre_x + (upperArmLength * Sin(rightShoulderAngle))
    rightelbow_y = neck_y + (upperArmLength * Cos(rightShoulderAngle))
  
    rightWristAngle = rightShoulderAngle + (rightElbowElevation * 3.1415927 / 100) - (3.1415927 / 8)
    rightwrist_x = rightelbow_x + (lowerArmLength * Sin(rightWristAngle))
    rightwrist_y = rightelbow_y + (lowerArmLength * Cos(rightWristAngle))
    
    'If (Not ((face_tx = 0) And (face_ty = 0))) Then
    '  Eye(0).ScaleWidth = IMAGE_WIDTH / reductionFactor_x
    '  Eye(0).ScaleHeight = IMAGE_HEIGHT / reductionFactor_y
    '  Eye(0).DrawWidth = 2
    '  Eye(0).Line (face_tx, face_ty)-(face_bx, face_by), RGB(255, 0, 0), B
    'End If
  
    'legs
    pic.DrawWidth = (30 * scaleBody * drawScale)
    pic.Line (centre_x, neck_y + bodyHeight)-(centre_x, pic.ScaleHeight), bottom_colour
    'body
    pic.DrawWidth = (30 * scaleBody * drawScale)
    pic.Line (centre_x, neck_y)-(centre_x, neck_y + bodyHeight), top_colour
    'head
    pic.DrawWidth = (30 * scaleBody * drawScale)
    pic.Line (centre_x, neck_y - (bodyHeight / 2))-(centre_x, neck_y - (bodyHeight / 2.5)), skin_colour
    'mouth
    'pic.DrawWidth = (2 * scaleBody)
    'pic.Line (centre_x, neck_y - (bodyHeight / 4))-(centre_x, neck_y - (bodyHeight / 3.5)), 0
    'eyes
    pic.DrawWidth = (4 * scaleBody * drawScale)
    gazeShift = gazeDirection * scaleBody / 3
    tiltshift = headTilt * scaleBody
    pic.Line (centre_x - (bodyHeight / 6.5) + gazeShift, neck_y - (bodyHeight / 2) + tiltshift)-(centre_x - (bodyHeight / 6) + gazeShift, neck_y - (bodyHeight / 2) + tiltshift), 0
    pic.Line (centre_x + (bodyHeight / 6.5) + gazeShift, neck_y - (bodyHeight / 2) - tiltshift)-(centre_x + (bodyHeight / 6) + gazeShift, neck_y - (bodyHeight / 2) - tiltshift), 0
    'nose
    'pic.DrawWidth = (1 * scaleBody)
    'pic.Line (centre_x + (gazeShift * 0.9), neck_y - (bodyHeight / 2.4))-(centre_x + (gazeShift * 1.2), neck_y - (bodyHeight / 3)), 0
    pic.DrawWidth = (5 * scaleBody * drawScale)
    pic.Line (centre_x + (gazeShift * 1.2), neck_y - (bodyHeight / 2.6))-(centre_x + (gazeShift * 1.2), neck_y - (bodyHeight / 2.6)), 0
    pic.Line (centre_x + (gazeShift * 1.2), neck_y - (bodyHeight / 2.4))-(centre_x + (gazeShift * 1.2), neck_y - (bodyHeight / 2.4)), skin_colour
    pic.DrawWidth = (2 * scaleBody * (1 + (MouthOpen / 50)) * drawScale)
    pic.Line (centre_x + (gazeShift * 0.7) - (((bodyHeight / 10) * mouthWidth) / 100), neck_y - (bodyHeight / 4.5))-(centre_x + (gazeShift * 0.7) + (((bodyHeight / 10) * mouthWidth) / 100), neck_y - (bodyHeight / 4.5)), 0
    'arms
    pic.DrawWidth = (10 * scaleBody * drawScale)
    pic.Line (leftelbow_x, leftelbow_y)-(centre_x - shoulderWidth, neck_y), top_colour
    pic.Line (leftwrist_x, leftwrist_y)-(leftelbow_x, leftelbow_y), top_colour
    pic.Line (centre_x + shoulderWidth, neck_y)-(rightelbow_x, rightelbow_y), top_colour
    pic.Line (rightelbow_x, rightelbow_y)-(rightwrist_x, rightwrist_y), top_colour
    'hands
    handMultiplier = (100 + ((leftHandSize - (((face_w / 2) * 100) / IMAGE_WIDTH)) * 1)) / 100
    If (handMultiplier < 1) Then handMultiplier = 1
    pic.DrawWidth = (10 * scaleBody * drawScale * handMultiplier)
    pic.Line (leftwrist_x, leftwrist_y)-(leftwrist_x, leftwrist_y + 20), skin_colour
    handMultiplier = (100 + ((rightHandSize - (((face_w / 2) * 100) / IMAGE_WIDTH)) * 1)) / 100
    If (handMultiplier < 1) Then handMultiplier = 1
    pic.DrawWidth = (10 * scaleBody * drawScale * handMultiplier)
    pic.Line (rightwrist_x, rightwrist_y)-(rightwrist_x + 1, rightwrist_y + 20), skin_colour
  End If
  
End Sub



Private Sub loadSettings()
  Dim filename As String

  filename = App.Path & "\settings.txt"
  
  If (Dir$(filename) <> "") Then
    Open filename For Input As #1
    Input #1, Filter_Name
    Close #1
  End If
End Sub


Private Sub saveSettings()
  Dim filename As String

  filename = App.Path & "\settings.txt"
  
  Open filename For Output As #1
  Print #1, Filter_Name
  Close #1
End Sub


Private Sub showMotionBox(pic As PictureBox)
'shows the bounding box for observed motion
  Dim tx As Long
  Dim ty As Long
  Dim bx As Long
  Dim by As Long
  Dim flow_x As Single
  Dim flow_y As Single
  
  Call RChbp_getMotionBox(tx, ty, bx, by, flow_x, flow_y)
  
  pic.ForeColor = RGB(0, 255, 0)
  pic.FillStyle = 1
  pic.ScaleWidth = IMAGE_WIDTH
  pic.ScaleHeight = IMAGE_HEIGHT
  pic.Line (tx, ty)-(bx, by), RGB(0, 255, 0), B
End Sub
