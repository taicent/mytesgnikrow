'Human Body Project
'Copyright (C) 2001-2004  Bob Mottram
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

'Use NDoc (http://ndoc.sourceforge.net) and VBCommenter to produce program documentation


Option Strict Off
Option Explicit On 

Imports DirectX.Capture
Imports System
Imports System.IO

''' <summary>
''' This module contains DLL declarations and a few other titbits
''' </summary>
Module mainModule

#Region "Human Body Project DLL function declarations"

    ''' <summary>
    ''' This is the main update routine
    ''' </summary>
    ''' <param name="cameraIndex">An index number corresponding to the camera.  Typically this is just set to zero.  A camera index was included within this function to cope with a situation where stereo cameras might be used, and you may want to select which of the two cameras that you wish to use</param>
    ''' <param name="bmp">A bitmap containing the image grabbed from the camera.  Usually this is an image in the default webcam resolution of 320x240 pixels</param>
    ''' <param name="width">Width of the camera image in pixels</param>
    ''' <param name="height">Height of the camera image in pixels</param>
    ''' <param name="NoOfSegments ">A value returned to indicate the number of segments which were detected.  This is really only for debugging purposes</param>
    Declare Sub RChbp_update Lib "c:\develop\hbp\release\hbp.DLL" (ByVal cameraIndex As Integer, ByVal bmp() As Byte, ByVal width As Integer, ByVal height As Integer, ByRef NoOfSegments As Integer)

    'set the type of detection
    '  0 - motion only
    '  1 - faces only
    '  2 - full body detection
    Declare Sub RChbp_setDetectionMode Lib "c:\develop\hbp\release\hbp.DLL" (ByVal mode As Integer)

    'returns an image.  imageIndex indicates the type of image to be returned
    'this is used for debugging purposes to check the various stages of image processing
    Declare Sub RChbp_getImage Lib "c:\develop\hbp\release\hbp.DLL" (ByRef bmp() As Byte, ByVal imageIndex As Integer)

    'return one pixel from the given image
    Declare Sub RChbp_getImagePixel Lib "c:\develop\hbp\release\hbp.DLL" (ByVal imageIndex As Integer, ByVal x As Integer, ByVal y As Integer, ByRef r As Byte, ByRef g As Byte, ByRef b As Byte)

    'returns details of a segment (bounding box and colour).  This only works if the detection mode is 2 (full body)
    Declare Sub RChbp_getSegment Lib "c:\develop\hbp\release\hbp.DLL" (ByVal cameraIndex As Integer, ByVal index As Integer, ByRef tx As Integer, ByRef ty As Integer, ByRef bx As Integer, ByRef by As Integer, ByRef r As Integer, ByRef g As Integer, ByRef b As Integer, ByRef segmentID As Integer, ByRef inMotion As Integer, ByRef disparity As Integer)

    'returns details of body and head posture
    Declare Sub RChbp_getBodyPosture Lib "c:\develop\hbp\release\hbp.DLL" (ByVal cameraIndex As Integer, ByRef leftShoulderElevation As Integer, ByRef rightShoulderElevation As Integer, ByRef leftElbowElevation As Integer, ByRef rightElbowElevation As Integer, ByRef leftHandSize As Integer, ByRef rightHandSize As Integer, ByRef face_tx As Integer, ByRef face_ty As Integer, ByRef face_bx As Integer, ByRef face_by As Integer, ByRef bodyWidth As Integer, ByRef gazeDirection As Integer, ByRef heagTilt As Integer, ByRef mouthWidth As Integer, ByRef MouthOpen As Integer, ByRef eyebrowsVertical As Integer, ByRef bodyColour_red As Integer, ByRef bodyColour_green As Integer, ByRef bodyColour_blue As Integer, ByRef backgroundColour_red As Integer, ByRef backgroundColour_green As Integer, ByRef backgroundColour_blue As Integer, ByRef personID As Integer, ByRef invalidFace As Boolean)

    'returns the detected face image
    Declare Sub RChbp_getFaceImage Lib "c:\develop\hbp\release\hbp.DLL" (ByRef bmp() As Byte)

    'return an individual pixel.  This is an unsatisfactory way of retrieving the face image,
    'but the only way I can presently find
    Declare Sub RChbp_getFaceImagePixel Lib "c:\develop\hbp\release\hbp.DLL" (ByVal x As Integer, ByVal y As Integer, ByRef r As Byte, ByRef g As Byte, ByRef b As Byte)

    'returns the normalised face image
    Declare Function RChbp_getNormalisedFaceImage Lib "c:\develop\hbp\release\hbp.DLL" (ByRef bmp() As Byte) As Integer

    'return the ID number and name of the currently detected person
    Declare Sub RChbp_getPersonIdentity Lib "c:\develop\hbp\release\hbp.DLL" (ByRef personID As Integer, ByRef personName() As Byte)

    'return the ID number of the currently detected person
    Declare Function RChbp_getPersonID Lib "c:\develop\hbp\release\hbp.DLL" () As Integer

    'return the name of the given person one character at a time
    'This is very unsatisfactory and is due to not being able to pass an array of bytes in vb.net
    Declare Function RChbp_getPersonName Lib "c:\develop\hbp\release\hbp.DLL" (ByVal personID As Integer, ByVal charIndex As Integer) As Byte

    'gets the filename of a jpg image of the person
    'This is very unsatisfactory and is due to not being able to pass an array of bytes in vb.net
    Declare Function RChbp_getPortrait Lib "c:\develop\hbp\release\hbp.DLL" (ByVal personID As Integer, ByVal charIndex As Integer) As Byte

    'enable or dissable learning of new faces
    Declare Sub RChbp_enableLearning Lib "c:\develop\hbp\release\hbp.DLL" (ByVal state As Boolean)

    'returns the bounding box and average optical flow direction for observed motion (useful if you're using detection mode 0)
    Declare Sub RChbp_getMotionBox Lib "c:\develop\hbp\release\hbp.DLL" (ByRef tx As Integer, ByRef ty As Integer, ByRef bx As Integer, ByRef by As Integer, ByRef flow_x As Single, ByRef flow_y As Single)

    'update the image one pixel at a time.  This is a rather unsatisfactory way of getting around the problem of not being able to pass a fixed array of bytes
    'to a DLL in vb.net
    Declare Sub RChbp_updateImage Lib "c:\develop\hbp\release\hbp.DLL" (ByVal x As Integer, ByVal y As Integer, ByVal width As Integer, ByVal height As Integer, ByVal r As Byte, ByVal g As Byte, ByVal b As Byte)

    'update routine called after all pixels have been sent to the DLL using RChbp_updateImage
    'Again this is rather unsatisfactory - there must be a better way of doing it
    Declare Sub RChbp_update2 Lib "c:\develop\hbp\release\hbp.DLL" (ByVal cameraIndex As Integer, ByVal width As Integer, ByVal height As Integer)

    'has a face been detected or not?
    Declare Function RChbp_faceDetected Lib "c:\develop\hbp\release\hbp.DLL" () As Boolean

    'force the classifier data to be saved
    Declare Sub RChbp_save Lib "c:\develop\hbp\release\hbp.DLL" (ByVal filename As String)

    'force the classifier data to be loaded
    Declare Sub RChbp_load Lib "c:\develop\hbp\release\hbp.DLL" (ByVal filename As String)

#End Region

    'this stores the current settings for the program (camera type, etc)
    Public CamSettings As New cameraSettings

    'bitmaps for left and right images
    Public left_bmp() As Byte
    Public right_bmp() As Byte
    Public camerasRunning As Boolean

    Public initStereo As Boolean
    Public averageIntensity As Long
    Public gvBusy As Boolean
    Public startupTicks As Integer


    Public Structure Active
        Dim Camera As Filter
        Dim CaptureInfo As Capture
        Dim ConfWindow As CW
        Dim Counter As Integer
        Dim CounterFrames As Integer
        Dim PathVideo As String
    End Structure

    'Global variables definition

    Public CaptureInformation As Active
    Public Dispositivos As New Filters

    Public Sub PrepareCam(ByVal PathVideo As String)
        Dim s() As String

        's = PathVideo.Split(".")
        ConfParamCam()
        'CaptureInformation.CaptureInfo.Filename = s(0) + CStr(CaptureInformation.Counter) + ".avi"
        CaptureInformation.Counter += 1
        CaptureInformation.CaptureInfo.RenderPreview()

    End Sub

    Public Sub ConfParamCam()

        Dim s() As String
        Dim size As Size
        Dim Rate As Double

        CaptureInformation.CaptureInfo.Stop()

        ' Change the compressor
        'CaptureInformation.CaptureInfo.VideoCompressor = Dispositivos.VideoCompressors(CaptureInformation.ConfWindow.cmbCompress.Items.IndexOf(CaptureInformation.ConfWindow.cmbCompress.Text))

        ' Change the image size
        s = CaptureInformation.ConfWindow.cmbTam.Text.Split("x")
        size = New Size(Val(s(0)), Val(s(1)))
        CaptureInformation.CaptureInfo.FrameSize = size

        ' Change the number of frames per second
        s = CaptureInformation.ConfWindow.cmbFPS.Text.Split(" ")
        Rate = Val(s(0))
        CaptureInformation.CaptureInfo.FrameRate = Rate

    End Sub


End Module