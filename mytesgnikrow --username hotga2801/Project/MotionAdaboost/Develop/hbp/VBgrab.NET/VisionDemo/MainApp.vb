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


Imports DirectX.Capture


''' <summary>
''' Tha main form
''' </summary>
Public Class MainApp
    Inherits System.Windows.Forms.Form

#Region "Local variables"
    'the location of the classifier data
    Dim classifier_filename As String = "c:\develop\hbp\faces.net"

    'SAPI speech, used for announcing names of people
    Dim Voice As New SpeechLib.SpVoice

    Dim busy As Boolean = False
    Dim initialised As Boolean = False
    Dim image_initialised As Boolean = False
    Dim bmp_small As Bitmap  'small bitmap
    Dim bmp As Bitmap        'full sized bitmap
    Dim bb_tx, bb_ty, bb_bx, bb_by As Integer  'top and bottom coords of the bounding box

    'show the camera image or not
    Const DISPLAY_CAMERA_IMAGE = False

    'define the dimensions of the bitmap
    Const IMAGE_WIDTH = 320
    Const IMAGE_HEIGHT = 320

    'size of the face image returned
    Const FACE_IMAGE_SIZE As Integer = 60

    Const SMALL_IMAGE_SIZE = 80
    Dim reductionFactor_x As Integer = IMAGE_WIDTH / SMALL_IMAGE_SIZE
    Dim reductionFactor_y As Integer = IMAGE_HEIGHT / SMALL_IMAGE_SIZE

    'presently unused
    Dim motionImage(IMAGE_WIDTH * IMAGE_HEIGHT * 3) As Byte

    'set to true if teh image is being updated
    Dim updating_image As Boolean = False

    'detection mode constants
    Const DETECTIONMODE_MOTION = 0
    Const DETECTIONMODE_FACES = 1
    Const DETECTIONMODE_BODY = 2
    Dim DETECTION_MODE As Integer = DETECTIONMODE_BODY

    'show the detected face image or not
    Dim showFaceImage As Boolean = False

    'show a jpg image of the recognised person
    Dim showPortrait As Boolean = False

    'show a processed image
    Dim showProcessedImage As Boolean = False
    Dim processedImageID As Integer = -1

    'current and previous person IDs
    Dim currentPersonID As Integer
    Dim prevPersonID As Integer = -1

    'graphics object used for drawing shapes
    Dim gr As Graphics

    'a blank canvas
    Dim blank_img As Image

    'previos person name
    Dim prev_personNameStr As String = ""

    'timeout period for clearing the portrait image
    Const PORTRAIT_TIMEOUT = 5

#End Region

#Region "drawing the Mr Plong avatar"

    ''' <summary>
    ''' Draw the mouth area of Mr Plong
    ''' </summary>
    ''' <param name="x">X coordinate of the centre of the mouth</param>
    ''' <param name="y">Y coordinate of the centre of the mouth</param>
    ''' <param name="mouth_width">a parameter indicating the width of the mouth in the range 0-100</param>
    ''' <param name="mouth_open">a parameter indicating how open the mouth is in the range 0-100 (0=closed, 100=wide open)</param>
    ''' <param name="smile">a parameter indicating smiling in the range 0-100 (0=neutral, 100=biggest smile)</param>
    Public Sub drawMouth(ByVal x As Integer, ByVal y As Integer, ByVal mouth_width As Integer, ByVal mouth_open As Integer, ByVal smile As Integer)
        Dim PenColour As System.Drawing.Color
        Dim PenWidth As Integer
        Dim x1, x2, x3, x4 As Integer
        Dim y1, y2, y3, y4 As Integer
        Dim w As Integer
        Dim h As Integer

        PenColour = Color.DarkRed
        w = mouth_width * (100 - mouth_open) / 100
        h = w / 4
        PenWidth = mouth_width * 0.1
        x1 = CInt(x - (w / 2))
        y1 = y - (smile * h / 100)

        x2 = CInt(x - (w / 3))
        y2 = y

        x3 = CInt(x + (w / 3))
        y3 = y

        x4 = CInt(x + (w / 2))
        y4 = y - (smile * h / 100)

        gr.DrawLine(New Pen(PenColour, PenWidth), x1, y1, x2, y2)
        gr.DrawLine(New Pen(PenColour, PenWidth), x2, y2, x3, y3)
        gr.DrawLine(New Pen(PenColour, PenWidth), x3, y3, x4, y4)

        w = mouth_width
        h = w * mouth_open / 200
        gr.FillEllipse(New SolidBrush(PenColour), CInt(x - (w / 2)), CInt(y - (h / 2)), w, h)

    End Sub


    ''' <summary>
    ''' Draws an eyebrow of Mr Plong
    ''' </summary>
    ''' <param name="x">X coordinate of the centre of the eyebrow</param>
    ''' <param name="y">Y coordinate of the centre of the eyebrow</param>
    ''' <param name="eyebrow_width">width of the eyebrow in pixels</param>
    ''' <param name="curvature_symetrical">curvature of the eyebrow in the range 0-100</param>
    ''' <param name="curvature_asymetrical"></param>
    ''' <param name="leftSide">true=left eyebrow, false=right eyebrow</param>
    Private Sub drawEyebrow(ByVal x As Integer, ByVal y As Integer, ByVal eyebrow_width As Integer, ByVal curvature_symetrical As Integer, ByVal curvature_asymetrical As Integer, ByVal leftSide As Boolean)
        Dim PenColour As System.Drawing.Color
        Dim PenWidth As Integer
        Dim y1, y2, y3, yy As Integer

        PenColour = Color.Black()
        PenWidth = eyebrow_width * 0.1
        y1 = y
        y2 = y - (eyebrow_width * curvature_symetrical / 100)
        y3 = y - (eyebrow_width * curvature_asymetrical / 100)
        If (leftSide) Then
            yy = y1
            y1 = y3
            y3 = yy
        End If
        If (y1 > y) Then y1 = y
        If (y2 > y) Then y2 = y
        If (y3 > y) Then y3 = y
        gr.DrawLine(New Pen(PenColour, PenWidth), CInt(x - (eyebrow_width / 2)), y1, x, y2)
        gr.DrawLine(New Pen(PenColour, PenWidth), x, y2, CInt(x + (eyebrow_width / 2)), y3)

    End Sub


    ''' <summary>
    ''' Draws an eye of Mr Plong
    ''' </summary>
    ''' <param name="x">X coordinate of the centre of the eye</param>
    ''' <param name="y">Y coordinate of the centre of the eye</param>
    ''' <param name="eye_width">width of the eye in pixels</param>
    ''' <param name="gazeLeft">how far is he looking to the left in the range -100 to 100</param>
    ''' <param name="gazeDown">how much is he looking down in the range 0-100</param>
    ''' <param name="eyelid">how far down is the eyelid in the range 0-100</param>
    Private Sub drawEye(ByVal x As Integer, ByVal y As Integer, ByVal eye_width As Integer, ByVal gazeLeft As Integer, ByVal gazeDown As Integer, ByVal eyelid As Integer)
        'draws an eye at the given coordinates
        Dim PenColour As System.Drawing.Color
        Dim tx As Integer
        Dim ty As Integer
        Dim w As Integer
        Dim h As Integer
        Dim iris_width As Integer
        Dim pupil_width As Integer
        Dim offset_x As Integer
        Dim offset_y As Integer
        Dim skin_colour As Integer = &HC0E0FF
        Dim rect As System.Drawing.Rectangle

        PenColour = Color.White
        w = eye_width
        h = w * 0.7
        tx = x
        ty = y
        gr.FillEllipse(New SolidBrush(PenColour), CInt(tx - (w / 2)), CInt(ty - (h / 2)), w, h)

        iris_width = eye_width * 0.47
        pupil_width = eye_width * 0.24
        offset_x = gazeLeft * eye_width / 100
        offset_y = gazeDown * h / 100
        PenColour = Color.LightBlue
        gr.FillEllipse(New SolidBrush(PenColour), CInt(tx - (iris_width / 2) + offset_x), CInt(ty - (iris_width / 2) + offset_y), iris_width, iris_width)
        PenColour = Color.Black
        gr.FillEllipse(New SolidBrush(PenColour), CInt(tx - (pupil_width / 2) + offset_x), CInt(ty - (pupil_width / 2) + offset_y), pupil_width, pupil_width)

        PenColour = ColorTranslator.FromWin32(skin_colour)
        rect.X = tx - (eye_width / 2)
        rect.Y = ty - (h / 2)
        rect.Width = eye_width
        rect.Height = h * eyelid / 100
        gr.FillRectangle(New SolidBrush(PenColour), rect)

    End Sub


    ''' <summary>
    ''' Draws the Mr Plong avatar
    ''' </summary>
    ''' <param name="pic">picture box control in which to draw the avatar</param>
    Private Sub showAvatar(ByVal pic As PictureBox)
        'shows a stick man avatar
        Dim leftShoulderElevation As Integer
        Dim rightShoulderElevation As Integer
        Dim centre_x As Integer
        Dim upperArmLength As Integer
        Static leftelbow_x As Integer
        Static leftelbow_y As Integer
        Static leftwrist_x As Integer
        Static leftwrist_y As Integer
        Static rightelbow_x As Integer
        Static rightelbow_y As Integer
        Static rightwrist_x As Integer
        Static rightwrist_y As Integer
        Dim neck_y As Integer
        Dim bodyHeight As Integer
        Dim leftElbowElevation As Integer
        Dim rightElbowElevation As Integer
        Static leftShoulderAngle As Single
        Static rightShoulderAngle As Single
        Static leftWristAngle As Single
        Static rightWristAngle As Single
        Dim lowerArmLength As Single
        Dim shoulderWidth As Single
        Static top_colour As Integer
        Static bottom_colour As Integer
        Dim skin_colour As Integer
        Dim face_tx As Integer
        Dim face_ty As Integer
        Dim face_bx As Integer
        Dim face_by As Integer
        Dim bodyWidth As Integer
        Dim scaleBody As Single
        Dim origin_x As Integer
        Dim origin_y As Integer
        Dim face_w As Integer
        Dim face_h As Integer
        Dim gazeDirection As Integer
        Static gazeShift As Integer
        Dim headTilt As Integer
        Static tiltshift As Integer
        Dim mouthWidth As Integer
        Dim MouthOpen As Integer
        Dim eyebrowsVertical As Integer
        Dim drawScale As Single
        Dim bodyColour_red As Integer
        Dim bodyColour_green As Integer
        Dim bodyColour_blue As Integer
        Dim backgroundColour_red As Integer
        Dim backgroundColour_green As Integer
        Dim backgroundColour_blue As Integer
        Dim personID As Integer
        Dim personName(20) As Byte
        Dim i As Integer
        Dim c As Integer
        Dim personNameStr As String
        Dim leftHandSize As Integer
        Dim rightHandSize As Integer
        Dim handMultiplier As Single
        Dim PenWidth As Integer
        Dim headPenWidth As Integer
        Dim PenColour As System.Drawing.Color
        Dim tx As Integer
        Dim bx As Integer
        Dim by As Integer
        Dim ty As Integer
        Dim cy As Integer
        Dim w As Integer
        Dim h As Integer
        Dim scale_x As Single = 0.3
        Dim scale_y As Single = 0.3
        Dim x As Integer
        Dim y As Integer
        Dim yy As Integer
        Dim eb1, eb2 As Integer
        Dim invalidFace As Boolean
        Static mouth_open As Integer
        Static gazeDir As Integer = 0
        Static eyebrow_vert As Integer = 30
        Static prev_gazeShift As Integer = 0
        Static prev_tiltShift As Integer = 0
        Static prev_centre_x As Integer = 50
        Static prev_neck_y As Integer = 50
        Static prev_scalebody As Single = 1

        Try
            Call RChbp_getBodyPosture(0, leftShoulderElevation, rightShoulderElevation, leftElbowElevation, rightElbowElevation, leftHandSize, rightHandSize, face_tx, face_ty, face_bx, face_by, bodyWidth, gazeDirection, headTilt, mouthWidth, MouthOpen, eyebrowsVertical, bodyColour_red, bodyColour_green, bodyColour_blue, backgroundColour_red, backgroundColour_green, backgroundColour_blue, personID, invalidFace)

            personID = RChbp_getPersonID()

            If (personID > 0) And (Not invalidFace) Then
                personNameStr = ""
                For i = 1 To 18
                    c = RChbp_getPersonName(personID, i)
                    If (c = 32) Or ((c > 64) And (c < 91)) Or ((c > 96) And (c < 123)) Then
                        personNameStr = personNameStr & Chr(c)
                    End If
                Next
                lblName.Text = personNameStr
            Else
                lblName.Text = ""
            End If


            gr = Graphics.FromImage(pic.Image)
            scale_x = pic.Width / 100 * 0.3
            scale_y = pic.Height / 100 * 0.3

            If (Not invalidFace) Then
                top_colour = RGB(bodyColour_red, bodyColour_green, bodyColour_blue) '&HC0&
            End If
            bottom_colour = &HC00000
            skin_colour = &HC0E0FF
            drawScale = 2

            mouthWidth = 20

            face_w = face_bx - face_tx
            face_h = face_by - face_ty

            If (DETECTION_MODE = DETECTIONMODE_BODY) Then
                If (face_w > 2) Then
                    origin_x = ((face_tx + (face_w / 2)) * 100) / (IMAGE_WIDTH / reductionFactor_x)
                    origin_y = (face_by * 100) / (IMAGE_HEIGHT / reductionFactor_y)
                End If
            End If

            If (DETECTION_MODE = DETECTIONMODE_FACES) Then
                origin_x = 50
                origin_y = 50
                bodyWidth = IMAGE_WIDTH / 5
            End If

            If (bodyWidth > 0) And (face_w > 2) Then
                'clear the picturebox image
                If (DETECTION_MODE = DETECTIONMODE_FACES) Then
                    pic.BackColor = ColorTranslator.FromWin32(RGB(255, 255, 255))
                Else
                    pic.BackColor = ColorTranslator.FromWin32(RGB(backgroundColour_red, backgroundColour_green, backgroundColour_blue))
                End If
                gr.Clear(pic.BackColor)

                scaleBody = 0.5 + ((bodyWidth / CSng(IMAGE_WIDTH / reductionFactor_x)) * 2)
                scaleBody = scaleBody * 0.3 / scale_x

                scaleBody = (prev_scalebody * 0.6) + (scaleBody * 0.4)

                centre_x = CInt((prev_centre_x * 0.6) + (((pic.Width * origin_x * (0.3 / scale_x)) / 100.0#) * 0.4))
                shoulderWidth = CInt(((pic.Width / 2) / 5) * scaleBody)
                upperArmLength = CInt(((pic.Width / 2) / 2) * scaleBody)
                lowerArmLength = CInt((upperArmLength * 2 / 3) * scaleBody)
                neck_y = CInt((prev_neck_y * 0.6) + (((pic.Height * origin_y * (0.3 / scale_y)) / 80.0#) * 0.4))
                bodyHeight = CInt((pic.Height * scaleBody) / 3)

                prev_centre_x = centre_x
                prev_neck_y = neck_y
                prev_scalebody = scaleBody

                If (DETECTION_MODE = DETECTIONMODE_BODY) Then
                    leftShoulderAngle = (2 * 3.1415927) - ((3.1415927 * 1 / 8) + ((100 - leftShoulderElevation) * 3.1415927 / 260))
                    leftelbow_x = centre_x + (upperArmLength * Math.Sin(leftShoulderAngle))
                    leftelbow_y = neck_y + (upperArmLength * Math.Cos(leftShoulderAngle))

                    leftWristAngle = leftShoulderAngle - (leftElbowElevation * 3.1415927 / 100) + (3.1415927 / 8)
                    leftwrist_x = leftelbow_x + (lowerArmLength * Math.Sin(leftWristAngle))
                    leftwrist_y = leftelbow_y + (lowerArmLength * Math.Cos(leftWristAngle))

                    rightShoulderAngle = ((3.1415927 * 1 / 8) + ((100 - rightShoulderElevation) * 3.1415927 / 260))
                    rightelbow_x = centre_x + (upperArmLength * Math.Sin(rightShoulderAngle))
                    rightelbow_y = neck_y + (upperArmLength * Math.Cos(rightShoulderAngle))

                    rightWristAngle = rightShoulderAngle + (rightElbowElevation * 3.1415927 / 100) - (3.1415927 / 8)
                    rightwrist_x = rightelbow_x + (lowerArmLength * Math.Sin(rightWristAngle))
                    rightwrist_y = rightelbow_y + (lowerArmLength * Math.Cos(rightWristAngle))
                End If

                'legs
                PenWidth = (40 * scaleBody * drawScale * scale_x)
                PenColour = ColorTranslator.FromWin32(bottom_colour)
                gr.DrawLine(New Pen(PenColour, PenWidth), centre_x, neck_y + bodyHeight, centre_x, pic.Height)
                'body
                PenWidth = (40 * scaleBody * drawScale * scale_x)
                PenColour = ColorTranslator.FromWin32(top_colour)
                gr.DrawLine(New Pen(PenColour, PenWidth), centre_x, CInt(neck_y - ((bodyHeight / 20) * scale_y)), centre_x, neck_y + bodyHeight)
                'head
                headPenWidth = (30 * scaleBody * drawScale * scale_x)
                PenColour = ColorTranslator.FromWin32(skin_colour)
                w = headPenWidth * 1.5
                h = headPenWidth * 1.5
                tx = CInt(centre_x - (w / 2))
                by = CInt(neck_y - (bodyHeight / 2))
                ty = CInt(neck_y - (bodyHeight / 1.2))
                cy = ty + ((by - ty) / 2)
                gr.FillEllipse(New SolidBrush(PenColour), tx, ty, w, h)

                'eyes
                PenWidth = (4 * scaleBody * drawScale * scale_x)
                w = PenWidth * 1.5
                h = PenWidth * 1.5
                PenColour = Color.Black
                If (Not invalidFace) Then
                    gazeShift = CInt((prev_gazeShift * 0.6) + ((gazeDirection * scaleBody / 4 * scale_x * 0.8) * 0.4))
                    tiltshift = CInt((prev_tiltShift * 0.6) + ((headTilt * scaleBody * scale_y * 0.1) * 0.4))
                    prev_gazeShift = gazeShift
                    prev_tiltShift = tiltshift
                    eyebrow_vert = eyebrowsVertical
                    gazeDir = gazeDirection
                    mouth_open = MouthOpen
                End If
                tx = CInt(centre_x - (bodyHeight / 7) + gazeShift)
                ty = CInt(neck_y - (bodyHeight / 2.0) + tiltshift)
                x = CInt(tx - (w / 2))
                y = CInt(ty - (h / 2))
                If (eyebrow_vert < 10) Then eyebrow_vert = 10
                drawEye(x, y, w * 1.5, gazeDir / 3, 0, (100 - eyebrow_vert) / 1.5)
                yy = CInt(y - (scaleBody * drawScale * scale_y * (1 + (eyebrow_vert * 0.08))))
                If (yy > y - (h / 20)) Then yy = y - (h / 20)
                eb1 = (eyebrow_vert * 0.3)
                eb2 = ((100 - eyebrow_vert) * 0.4)
                drawEyebrow(x, yy, w * 2 * ((gazeDir / 2) + 5) / 80, eb1, eb2, True)

                tx = CInt(centre_x + (bodyHeight / 7) + gazeShift)
                ty = CInt(neck_y - (bodyHeight / 2.0) - tiltshift)
                x = CInt(tx - (w / 2))
                y = CInt(ty - (h / 2))
                drawEye(x, y, w * 1.5, gazeDir / 3, 0, (100 - eyebrow_vert) / 1.5)
                yy = CInt(y - (scaleBody * drawScale * scale_y * (1 + (eyebrow_vert * 0.08))))
                If (yy > y - (h / 20)) Then yy = y - (h / 20)
                eb1 = (eyebrow_vert * 0.3)
                eb2 = ((100 - eyebrow_vert) * 0.4)
                drawEyebrow(x, yy, w * 2 * (55 - (gazeDir / 2)) / 80, eb1, eb2, False)

                'nose
                PenWidth = (5 * scaleBody * drawScale)
                PenColour = Color.Pink
                tx = CInt(centre_x + (gazeShift * 1.2))
                ty = CInt(neck_y - (bodyHeight / 2.6))
                w = PenWidth / 2
                h = PenWidth / 2
                gr.FillEllipse(New SolidBrush(PenColour), CInt(tx - (w / 2)), CInt(ty - (h / 2)), w, h)
                PenColour = ColorTranslator.FromWin32(skin_colour)
                tx = CInt(centre_x + (gazeShift * 1.2))
                ty = CInt(neck_y - (bodyHeight / 2.3))
                gr.FillEllipse(New SolidBrush(PenColour), CInt(tx - (w / 2)), CInt(ty - (h / 2)), w, h)

                'mouth
                tx = CInt(centre_x + (gazeShift * 0.7) - (((bodyHeight / 10) * mouthWidth) / 100))
                ty = CInt(neck_y - (bodyHeight / 4.5))
                drawMouth(tx, ty, CInt((bodyHeight / 1) * mouthWidth / 100), mouth_open, 30)

                'arms
                PenWidth = (14 * scaleBody * drawScale * scale_x)
                PenColour = ColorTranslator.FromWin32(top_colour)
                gr.DrawLine(New Pen(PenColour, PenWidth), leftelbow_x, leftelbow_y, CInt(centre_x - shoulderWidth), neck_y)
                gr.DrawLine(New Pen(PenColour, PenWidth), leftwrist_x, leftwrist_y, leftelbow_x, leftelbow_y)
                gr.DrawLine(New Pen(PenColour, PenWidth), CInt(centre_x + shoulderWidth), neck_y, rightelbow_x, rightelbow_y)
                gr.DrawLine(New Pen(PenColour, PenWidth), rightelbow_x, rightelbow_y, rightwrist_x, rightwrist_y)

                'hands
                handMultiplier = (100 + ((leftHandSize - (((face_w / 2) * 100) / IMAGE_WIDTH)) * 1)) / 100
                If (handMultiplier < 1) Then handMultiplier = 1
                PenWidth = (10 * scaleBody * drawScale * handMultiplier * scale_x)
                PenColour = ColorTranslator.FromWin32(skin_colour)
                w = headPenWidth / 1.6
                h = headPenWidth / 1.6
                gr.FillEllipse(New SolidBrush(PenColour), CInt(leftwrist_x - (w / 2)), CInt(leftwrist_y - (h / 2)), w, h)

                handMultiplier = (100 + ((rightHandSize - (((face_w / 2) * 100) / IMAGE_WIDTH)) * 1)) / 100
                If (handMultiplier < 1) Then handMultiplier = 1
                PenWidth = (10 * scaleBody * drawScale * handMultiplier * scale_x)
                w = headPenWidth / 1.6
                h = headPenWidth / 1.6
                gr.FillEllipse(New SolidBrush(PenColour), CInt(rightwrist_x - (w / 2)), CInt(rightwrist_y - (h / 2)), w, h)

                pic.Refresh()
            End If

        Catch


        End Try

    End Sub
#End Region

#Region "Main update function"

    ''' <summary>
    ''' Processes the given image
    ''' </summary>
    ''' <param name="input_img">The image to be processed</param>
    Public Sub updateVision(ByRef input_img As Image)
        Dim p, x, y, tx, ty, bx, by, w, h As Integer
        Dim col As System.Drawing.Color

        updating_image = True
        If (Not image_initialised) Then
            image_initialised = True
            Call RChbp_enableLearning(True)
            Call RChbp_setDetectionMode(DETECTION_MODE)
        End If

        bmp = Nothing
        bmp = input_img 'picCapture.Image

        'define a subsampled small image
        bmp_small = Nothing
        bmp_small = New Bitmap(bmp.GetThumbnailImage(IMAGE_WIDTH, IMAGE_HEIGHT, AddressOf ThumbNailAbort, Nothing))

        'transfer data from the small image into the DLL
        'This is very cumbersome, but at present I can't find any easy way to pass an array of bytes
        For y = 0 To IMAGE_HEIGHT - 1
            For x = 0 To IMAGE_WIDTH - 1
                col = bmp_small.GetPixel(x, y)
                RChbp_updateImage(x, y, CInt(IMAGE_WIDTH), CInt(IMAGE_HEIGHT), col.R, col.G, col.B)
            Next
        Next
        updating_image = False

        picOutput.Image = Nothing
        picOutput.Image = bmp_small

        'update routine within the DLL
        RChbp_update2(0, CInt(IMAGE_WIDTH), CInt(IMAGE_HEIGHT))

        'identify the person
        identifyPerson()

    End Sub
#End Region

#Region "Functions for displaying stuff on the screen"

    ''' <summary>
    ''' Show an image of the face region detected using the openCV system
    ''' </summary>
    ''' <param name="pic">picture box control in which to draw the face</param>
    Public Sub showFace(ByVal pic As PictureBox)
        Dim PenColour As System.Drawing.Color
        Dim rect As System.Drawing.Rectangle
        Dim r, g, b As Byte
        Dim x, y As Integer

        For y = 0 To FACE_IMAGE_SIZE - 1
            For x = 0 To FACE_IMAGE_SIZE - 1
                RChbp_getFaceImagePixel(x, y, r, g, b)
                PenColour = ColorTranslator.FromWin32(RGB(r, g, b))
                rect.X = pic.Width / FACE_IMAGE_SIZE * x / 2
                rect.Y = pic.Height / FACE_IMAGE_SIZE * y / 2
                rect.Width = pic.Width / FACE_IMAGE_SIZE
                rect.Height = pic.Height / FACE_IMAGE_SIZE
                gr.FillRectangle(New SolidBrush(PenColour), rect)
            Next
        Next
        pic.Refresh()
    End Sub


    ''' <summary>
    ''' Show an image within the given picture box control
    ''' </summary>
    ''' <param name="pic">picture box control in which to draw the face</param>
    ''' <param name="imageIndex">An index number corresponding to the type of image to be displayed</param>
    Public Sub showImage(ByVal pic As PictureBox, ByVal imageIndex As Integer)
        Dim PenColour As System.Drawing.Color
        Dim rect As System.Drawing.Rectangle
        Dim r, g, b As Byte
        Dim x, y As Integer

        For y = 0 To SMALL_IMAGE_SIZE - 1
            For x = 0 To SMALL_IMAGE_SIZE - 1
                RChbp_getImagePixel(imageIndex, x, y, r, g, b)
                PenColour = ColorTranslator.FromWin32(RGB(r, g, b))
                rect.X = pic.Width / SMALL_IMAGE_SIZE * x / 2
                rect.Y = pic.Height / SMALL_IMAGE_SIZE * y / 2
                rect.Width = pic.Width / SMALL_IMAGE_SIZE
                rect.Height = pic.Height / SMALL_IMAGE_SIZE
                gr.FillRectangle(New SolidBrush(PenColour), rect)
            Next
        Next
        pic.Refresh()
    End Sub


    ''' <summary>
    ''' Select a display mode from the menu
    ''' </summary>
    ''' <param name="index">An index number corresponding to the type of image to be displayed</param>
    Public Sub selectDisplaymode(ByVal index As Integer)
        MenuItem2.Checked = False
        MenuItem3.Checked = False
        MenuItem4.Checked = False
        MenuItem5.Checked = False
        MenuItem6.Checked = False
        MenuItem7.Checked = False
        MenuItem8.Checked = False
        MenuItem11.Checked = False

        gr = Graphics.FromImage(picAvatar.Image)

        Select Case index
            Case 0
                MenuItem2.Checked = True
                DETECTION_MODE = DETECTIONMODE_BODY
                showFaceImage = False
                showProcessedImage = False
                showPortrait = False
                'Me.picAvatar.Image = blank_img
                CamSettings.DisplayMode = 0

            Case 1
                MenuItem3.Checked = True
                DETECTION_MODE = DETECTIONMODE_FACES
                showFaceImage = False
                showProcessedImage = False
                showPortrait = False
                'Me.picAvatar.Image = blank_img
                CamSettings.DisplayMode = 1

            Case 2
                MenuItem4.Checked = True
                DETECTION_MODE = DETECTIONMODE_FACES
                showFaceImage = True
                showProcessedImage = False
                showPortrait = False
                'Me.picAvatar.Image = blank_img
                picAvatar.BackColor = ColorTranslator.FromWin32(RGB(255, 255, 255))                
                gr.Clear(picAvatar.BackColor)
                CamSettings.DisplayMode = 2

            Case 3
                MenuItem5.Checked = True
                showFaceImage = False
                showProcessedImage = True
                showPortrait = False
                processedImageID = 0
                'Me.picAvatar.Image = blank_img
                picAvatar.BackColor = ColorTranslator.FromWin32(RGB(255, 255, 255))
                gr.Clear(picAvatar.BackColor)
                CamSettings.DisplayMode = 3

            Case 4
                MenuItem6.Checked = True
                showFaceImage = False
                showProcessedImage = True
                showPortrait = False
                processedImageID = 2
                CamSettings.DisplayMode = processedImageID
                'Me.picAvatar.Image = blank_img
                picAvatar.BackColor = ColorTranslator.FromWin32(RGB(255, 255, 255))
                gr.Clear(picAvatar.BackColor)
                CamSettings.DisplayMode = 4

            Case 5
                MenuItem7.Checked = True
                showFaceImage = False
                showProcessedImage = True
                showPortrait = False
                processedImageID = 5
                'Me.picAvatar.Image = blank_img
                picAvatar.BackColor = ColorTranslator.FromWin32(RGB(255, 255, 255))
                gr.Clear(picAvatar.BackColor)
                CamSettings.DisplayMode = 5

            Case 6
                MenuItem8.Checked = True
                showFaceImage = False
                showProcessedImage = True
                showPortrait = False
                processedImageID = 6
                'Me.picAvatar.Image = blank_img
                picAvatar.BackColor = ColorTranslator.FromWin32(RGB(255, 255, 255))
                gr.Clear(picAvatar.BackColor)
                CamSettings.DisplayMode = 6

            Case 7
                MenuItem11.Checked = True
                DETECTION_MODE = DETECTIONMODE_FACES
                showFaceImage = False
                showProcessedImage = False
                showPortrait = True
                processedImageID = 7
                CamSettings.DisplayMode = 7

        End Select

        CamSettings.Save()
    End Sub


    ''' <summary>
    ''' Show a portrait (JPEG image) of the currently observed person
    ''' </summary>
    Public Sub show_Portrait()
        'show an image of the recognised person
        Dim i As Integer
        Dim b As Byte
        Dim filename As String
        Static cnt As Integer

        If ((currentPersonID > 0) And (RChbp_faceDetected())) Then
            cnt = 0
            i = 0
            b = 1
            filename = ""
            While ((i < 150) And (b <> 13) And (b <> 0))
                b = RChbp_getPortrait(currentPersonID, i)
                If (b <> 13) And (b <> 0) Then
                    filename = filename & Chr(b)
                End If
                i = i + 1
            End While

            If (filename <> "") Then
                Dim pimg As Image = Image.FromFile(filename)
                Me.picAvatar.Image = pimg
                Me.picAvatar.Refresh()

                If (CamSettings.speakNames) Then
                    speakName()
                End If
            End If

        Else

            If (cnt < PORTRAIT_TIMEOUT) Then
                cnt = cnt + 1
            Else
                'clear the image if the person has not been observed for a period of time
                Me.picAvatar.Image = blank_img
                Me.picAvatar.Refresh()
                prev_personNameStr = ""
            End If
        End If
    End Sub


    ''' <summary>
    ''' Show a bounding box.  This function is presently not used
    ''' </summary>
    Public Sub showBoundingBox()
        Dim x, y, w, h, tx, ty As Integer
        Dim col As System.Drawing.Color

        'show the bounding box
        w = bb_bx - bb_tx
        h = bb_by - bb_ty
        tx = (bb_tx * picCapture.Image.Width) / IMAGE_WIDTH
        ty = (bb_ty * picCapture.Image.Height) / IMAGE_HEIGHT
        w = (w * picCapture.Image.Width) / IMAGE_WIDTH
        h = (h * picCapture.Image.Height) / IMAGE_HEIGHT

        Try
            gr = Graphics.FromImage(picCapture.Image)
            'Dim gr As Graphics = Graphics.FromImage(picCapture.Image)
            gr.DrawRectangle(New Pen(Color.Green, 2), tx, ty, w, h)
        Catch ex As Exception

        End Try
    End Sub

    ''' <summary>
    ''' Image refresh routine
    ''' </summary>
    Public Sub RefreshImage(ByVal Frame As System.Windows.Forms.PictureBox)
        Try

            If (DISPLAY_CAMERA_IMAGE) Then
                Me.picCapture.Image = Nothing
                Me.picCapture.Image = Frame.Image
            End If

            CaptureInformation.CounterFrames += 1
            Call updateVision(Frame.Image)

            If (DISPLAY_CAMERA_IMAGE) Then
                Me.picCapture.Refresh()
            End If

            If (Not showPortrait) Then
                If (Not showFaceImage) Then
                    If (Not showProcessedImage) Then
                        showAvatar(picAvatar)
                    Else
                        showImage(picAvatar, processedImageID)
                    End If
                Else
                    showFace(picAvatar)
                End If

            Else
                'show a portrait
                show_Portrait()
            End If

            initialised = True
        Catch ex As Exception

        End Try

    End Sub

#End Region

#Region "Identifying the person name or ID and logging to a text file"

    ''' <summary>
    ''' Log the observed person to a text file
    ''' </summary>
    ''' <param name="personID">An ID number greater than zero corresponding to the person</param>
    Public Sub logFace(ByVal personID As Integer)
        Dim oFile As System.IO.File
        Dim oWrite As System.IO.StreamWriter
        Dim allowWrite As Boolean = True
        Dim textLine As String
        Dim personName As String

        'get the name of the person (20 characters long)
        'this is done in a very crude way, just because I can't find a good way to
        'pass byte arrays from a DLL to vb.net
        personName = getPersonName()

        Try
            'open the file and append to it
            oWrite = oFile.AppendText(System.Windows.Forms.Application.StartupPath & "\facelog.txt")
        Catch
            allowWrite = False
        End Try

        If (allowWrite) Then
            'write the details
            textLine = Format$(Now, "dd/mm/yyyy") & ", " & Format$(Now, "dddd") & ", " & Format$(Now, "hh:mm:ss") & ", " & personID & ", " & personName & ","
            oWrite.WriteLine(textLine)

            'close the file
            oWrite.Close()
        End If

    End Sub


    ''' <summary>
    ''' Get the ID number for the currently observed persona and log the event to a text file if necessary
    ''' </summary>
    Public Sub identifyPerson()
        Dim filename As String
        Static init_ID As Boolean

        'get the ID number of the person
        currentPersonID = RChbp_getPersonID()

        If (currentPersonID > 0) Then
            'log the identity if necessary
            If (CamSettings.createLog) Then
                If ((currentPersonID <> prevPersonID) Or (Not init_ID)) Then
                    init_ID = True
                    logFace(currentPersonID)
                End If
            End If

            prevPersonID = currentPersonID
        End If
    End Sub


#Region " Código generado por el Diseñador de Windows Forms "

    Private Sub initCamera()
        'Call to AddCam to select an available camera
        Dim AddCamera As New AddCam
        AddCamera.ShowDialog(Me)

        CaptureInformation.CaptureInfo.PreviewWindow = Me.picEye

        'Define RefreshImage as event handler of FrameCaptureComplete
        AddHandler CaptureInformation.CaptureInfo.FrameCaptureComplete, AddressOf RefreshImage

        CaptureInformation.Counter = 1
        CaptureInformation.CounterFrames = 1


        Me.Show()

        'Initialization of ConfWindow
        CaptureInformation.ConfWindow = New CW
        CaptureInformation.ConfWindow.Refresh()
        CaptureInformation.ConfWindow.Show()
    End Sub


    Public Sub New()
        MyBase.New()

        InitializeComponent()

        'initialise the camera
        initCamera()

        If (CamSettings.createLog) Then
            MenuItem10.Checked = True
        Else
            MenuItem10.Checked = False
        End If

        If (CamSettings.speakNames) Then
            MenuItem12.Checked = True
        Else
            MenuItem12.Checked = False
        End If

        blank_img = Me.picAvatar.Image.Clone

        'select the appropriate display mode
        selectDisplaymode(CamSettings.DisplayMode)

    End Sub

    'Form reemplaza a Dispose para limpiar la lista de componentes.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Requerido por el Diseñador de Windows Forms
    Private components As System.ComponentModel.IContainer

    'NOTA: el Diseñador de Windows Forms requiere el siguiente procedimiento
    'Puede modificarse utilizando el Diseñador de Windows Forms. 
    'No lo modifique con el editor de código.
    Friend WithEvents timGrab As System.Windows.Forms.Timer
    Friend WithEvents picOutput As System.Windows.Forms.PictureBox
    Friend WithEvents picEye As System.Windows.Forms.PictureBox
    Friend WithEvents picCapture As System.Windows.Forms.PictureBox
    Friend WithEvents mnuMotionDetection As System.Windows.Forms.MenuItem
    Friend WithEvents mnuMain As System.Windows.Forms.MainMenu
    Friend WithEvents mnuExit As System.Windows.Forms.MenuItem
    Friend WithEvents lblName As System.Windows.Forms.Label
    Friend WithEvents picAvatar As System.Windows.Forms.PictureBox
    Friend WithEvents MenuItem1 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem2 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem3 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem4 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem5 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem6 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem7 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem8 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem9 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem10 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem11 As System.Windows.Forms.MenuItem
    Friend WithEvents MenuItem12 As System.Windows.Forms.MenuItem
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(MainApp))
        Me.timGrab = New System.Windows.Forms.Timer(Me.components)
        Me.picOutput = New System.Windows.Forms.PictureBox
        Me.picEye = New System.Windows.Forms.PictureBox
        Me.picCapture = New System.Windows.Forms.PictureBox
        Me.mnuMain = New System.Windows.Forms.MainMenu
        Me.mnuMotionDetection = New System.Windows.Forms.MenuItem
        Me.mnuExit = New System.Windows.Forms.MenuItem
        Me.MenuItem1 = New System.Windows.Forms.MenuItem
        Me.MenuItem2 = New System.Windows.Forms.MenuItem
        Me.MenuItem3 = New System.Windows.Forms.MenuItem
        Me.MenuItem4 = New System.Windows.Forms.MenuItem
        Me.MenuItem5 = New System.Windows.Forms.MenuItem
        Me.MenuItem6 = New System.Windows.Forms.MenuItem
        Me.MenuItem7 = New System.Windows.Forms.MenuItem
        Me.MenuItem8 = New System.Windows.Forms.MenuItem
        Me.MenuItem11 = New System.Windows.Forms.MenuItem
        Me.MenuItem9 = New System.Windows.Forms.MenuItem
        Me.MenuItem10 = New System.Windows.Forms.MenuItem
        Me.lblName = New System.Windows.Forms.Label
        Me.picAvatar = New System.Windows.Forms.PictureBox
        Me.MenuItem12 = New System.Windows.Forms.MenuItem
        Me.SuspendLayout()
        '
        'timGrab
        '
        Me.timGrab.Enabled = True
        '
        'picOutput
        '
        Me.picOutput.Location = New System.Drawing.Point(88, 256)
        Me.picOutput.Name = "picOutput"
        Me.picOutput.Size = New System.Drawing.Size(144, 116)
        Me.picOutput.TabIndex = 5
        Me.picOutput.TabStop = False
        Me.picOutput.Visible = False
        '
        'picEye
        '
        Me.picEye.Location = New System.Drawing.Point(328, 0)
        Me.picEye.Name = "picEye"
        Me.picEye.Size = New System.Drawing.Size(320, 236)
        Me.picEye.TabIndex = 6
        Me.picEye.TabStop = False
        Me.picEye.Visible = False
        '
        'picCapture
        '
        Me.picCapture.Location = New System.Drawing.Point(0, 0)
        Me.picCapture.Name = "picCapture"
        Me.picCapture.Size = New System.Drawing.Size(320, 240)
        Me.picCapture.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.picCapture.TabIndex = 7
        Me.picCapture.TabStop = False
        '
        'mnuMain
        '
        Me.mnuMain.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuMotionDetection, Me.MenuItem1, Me.MenuItem9})
        '
        'mnuMotionDetection
        '
        Me.mnuMotionDetection.Index = 0
        Me.mnuMotionDetection.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuExit})
        Me.mnuMotionDetection.Text = "File"
        '
        'mnuExit
        '
        Me.mnuExit.Index = 0
        Me.mnuExit.Text = "Exit"
        '
        'MenuItem1
        '
        Me.MenuItem1.Index = 1
        Me.MenuItem1.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.MenuItem2, Me.MenuItem3, Me.MenuItem4, Me.MenuItem5, Me.MenuItem6, Me.MenuItem7, Me.MenuItem8, Me.MenuItem11})
        Me.MenuItem1.Text = "Display"
        '
        'MenuItem2
        '
        Me.MenuItem2.Index = 0
        Me.MenuItem2.Text = "Whole body"
        '
        'MenuItem3
        '
        Me.MenuItem3.Index = 1
        Me.MenuItem3.Text = "Facial features only"
        '
        'MenuItem4
        '
        Me.MenuItem4.Index = 2
        Me.MenuItem4.Text = "Detected face"
        '
        'MenuItem5
        '
        Me.MenuItem5.Index = 3
        Me.MenuItem5.Text = "Input Image"
        '
        'MenuItem6
        '
        Me.MenuItem6.Index = 4
        Me.MenuItem6.Text = "Segmented Image"
        '
        'MenuItem7
        '
        Me.MenuItem7.Index = 5
        Me.MenuItem7.Text = "Body Segments"
        '
        'MenuItem8
        '
        Me.MenuItem8.Index = 6
        Me.MenuItem8.Text = "Skin colours"
        '
        'MenuItem11
        '
        Me.MenuItem11.Index = 7
        Me.MenuItem11.Text = "Portrait"
        '
        'MenuItem9
        '
        Me.MenuItem9.Index = 2
        Me.MenuItem9.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.MenuItem10, Me.MenuItem12})
        Me.MenuItem9.Text = "Tools"
        '
        'MenuItem10
        '
        Me.MenuItem10.Index = 0
        Me.MenuItem10.Text = "Create log"
        '
        'lblName
        '
        Me.lblName.Location = New System.Drawing.Point(12, 248)
        Me.lblName.Name = "lblName"
        Me.lblName.Size = New System.Drawing.Size(68, 12)
        Me.lblName.TabIndex = 8
        '
        'picAvatar
        '
        Me.picAvatar.Image = CType(resources.GetObject("picAvatar.Image"), System.Drawing.Image)
        Me.picAvatar.Location = New System.Drawing.Point(0, 0)
        Me.picAvatar.Name = "picAvatar"
        Me.picAvatar.Size = New System.Drawing.Size(204, 176)
        Me.picAvatar.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.picAvatar.TabIndex = 9
        Me.picAvatar.TabStop = False
        '
        'MenuItem12
        '
        Me.MenuItem12.Index = 1
        Me.MenuItem12.Text = "Speak names"
        '
        'MainApp
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(204, 177)
        Me.Controls.Add(Me.picAvatar)
        Me.Controls.Add(Me.lblName)
        Me.Controls.Add(Me.picCapture)
        Me.Controls.Add(Me.picEye)
        Me.Controls.Add(Me.picOutput)
        Me.MaximizeBox = False
        Me.Menu = Me.mnuMain
        Me.MinimizeBox = False
        Me.Name = "MainApp"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Human Body Project"
        Me.ResumeLayout(False)

    End Sub

#End Region


    ''' <summary>
    ''' Gets the name of the currently observed person
    ''' </summary>
    ''' <returns>the name of the person as a string up to 19 characters in length)</returns>
    Public Function getPersonName() As String
        'returns the name of the person
        Dim personNameStr As String
        Dim i As Integer
        Dim c As Byte
        Dim finished As Boolean

        personNameStr = ""
        finished = False
        i = 1
        While (i < 19) And (Not finished)
            c = RChbp_getPersonName(currentPersonID, i)
            If (c = 32) Or ((c > 64) And (c < 91)) Or ((c > 96) And (c < 123)) Then
                personNameStr = personNameStr & Chr(c)
            End If
            If ((c = 13) Or (c = 10)) Then
                finished = True
            End If
            i = i + 1
        End While
        getPersonName = personNameStr
    End Function
#End Region

#Region "Speech (SAPI) functions"

    ''' <summary>
    ''' Use SAPI to speak the name of the currently observed person
    ''' </summary>
    Public Sub speakName()
        Dim personNameStr As String

        personNameStr = getPersonName()
        If (personNameStr <> prev_personNameStr) Then
            'You might want to add your own salutation here
            Voice.Speak(personNameStr)
        End If
        prev_personNameStr = personNameStr
    End Sub

#End Region

#Region "Other stuff"
    ''' <summary>
    ''' Does absolutely nothing
    ''' </summary>
    Private Function ThumbNailAbort() As Boolean
        'Do Nothing Here
    End Function


    ''' <summary>
    ''' Grab an image from the camera periodically, using a timer control
    ''' </summary>
    Public Sub timGrab_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles timGrab.Tick
        If (initialised) And (Not busy) Then
            busy = True

            Try

                'grab an image from the camera
                CaptureInformation.CaptureInfo.CaptureFrame()

                'occasionally save data
                autoSave()

            Catch ex As Exception

            End Try
            busy = False
        End If
    End Sub

    ''' <summary>
    ''' Exit the application
    ''' </summary>
    Private Sub mnuExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles mnuExit.Click
        End
    End Sub

    Private Sub picCapture_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles picCapture.Paint
        'If (DISPLAY_CAMERA_IMAGE) Then
        'Me.picCapture.Refresh()
        'End If
    End Sub



    ''' <summary>
    ''' Save anything which was learned during this session before closing
    ''' </summary>
    Public Sub MainApp_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        'save the classifier data before closing
        RChbp_save(CPP_Path(classifier_filename))
    End Sub





    ''' <summary>
    ''' Load the face recognition classifier
    ''' </summary>
    Public Sub MainApp_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        'load the classifier data
        If (classifier_filename = "") Then
            classifier_filename = System.AppDomain.CurrentDomain.BaseDirectory() & "\faces.net"
        Else
            RChbp_load(classifier_filename)
        End If
    End Sub




    Private Sub MenuItem2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem2.Click
        selectDisplaymode(0)
    End Sub

    Private Sub MenuItem3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem3.Click
        selectDisplaymode(1)
    End Sub

    Private Sub MenuItem4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem4.Click
        selectDisplaymode(2)
    End Sub


    Private Sub MenuItem5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem5.Click
        selectDisplaymode(3)
    End Sub

    Private Sub MenuItem6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem6.Click
        selectDisplaymode(4)
    End Sub


    Private Sub MenuItem7_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem7.Click
        selectDisplaymode(5)
    End Sub


    Private Sub MenuItem8_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem8.Click
        selectDisplaymode(6)
    End Sub


    Private Sub MenuItem11_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem11.Click
        selectDisplaymode(7)
    End Sub

    Private Sub MenuItem10_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem10.Click
        'switch logging on or off
        MenuItem10.Checked = Not MenuItem10.Checked
        If (MenuItem10.Checked) Then
            CamSettings.createLog = True
        Else
            CamSettings.createLog = False
        End If
        'save the new setting
        CamSettings.Save()
    End Sub


    ''' <summary>
    ''' Converts an ordinary path (eg C:\mypath\stuff) to a C++ style path (eg C:\\mypath\\stuff)
    ''' </summary>
    ''' <param name="filename">The path to be converted</param>
    ''' <returns>A C++ style path</returns>
    Public Function CPP_Path(ByVal filename As String) As String
        'convert to a c++ path using \\ instead of \
        Dim cpp_filename As String
        Dim i As Integer
        Dim c As String

        cpp_filename = ""
        For i = 1 To Len(filename)
            c = Mid$(filename, i, 1)
            cpp_filename = cpp_filename & c
            If (c = "\") Then
                cpp_filename = cpp_filename & c
            End If
        Next
        CPP_Path = cpp_filename
    End Function


    ''' <summary>
    ''' Occasionally save the face classifier data.  This ensures that any new faces learned are not forgotten.
    ''' </summary>
    Public Sub autoSave()
        'occasionally save the classifier data
        Static prev_hour As Integer
        Dim curr_hour As Integer

        curr_hour = Val(Format$(Now, "hh"))
        If (curr_hour <> prev_hour) Then
            busy = True
            RChbp_save(CPP_Path(classifier_filename))
            busy = False
            prev_hour = curr_hour
        End If
    End Sub

    Private Sub MenuItem12_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MenuItem12.Click
        CamSettings.speakNames = Not CamSettings.speakNames
        If (CamSettings.speakNames) Then
            MenuItem12.Checked = True
        Else
            MenuItem12.Checked = False
        End If
        CamSettings.Save()
    End Sub

#End Region

End Class
