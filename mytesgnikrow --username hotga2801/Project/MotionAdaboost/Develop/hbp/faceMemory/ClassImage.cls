VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ClassImage"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

'Sentience project - image object
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

Public width As Integer
Public height As Integer
Dim image() As Byte
Dim Integral() As Long

Public Texture As Integer

Dim calibrationActive As Boolean
Dim CalibrationMap() As Integer

Dim RGBsource As RGBthingy
Dim RGBdest As RGBpoint


Public Sub Negative()
  Dim x As Integer
  Dim y As Integer
  Dim c As Integer
  
  For x = 0 To width - 1
    For y = 0 To height - 1
      For c = 0 To 2
        image(x, y, c) = 255 - image(x, y, c)
      Next
    Next
  Next
End Sub


Public Sub Rotate()
  Dim x As Integer
  Dim y As Integer
  Dim c As Integer
  Dim temp As Byte
  
  For x = 0 To width - 1
    For y = 0 To height - 1
      For c = 0 To 2
        temp = image(y, x, c)
        image(y, x, c) = image(x, y, c)
        image(x, y, c) = temp
      Next
    Next
  Next
End Sub



Public Sub loadCalibration(filename As String)
'loads the camera calibration map
  Dim chess_x As Integer
  Dim chess_y As Integer
  Dim x As Integer
  Dim y As Integer
  Dim w As Integer
  Dim h As Integer
  Dim xx As Integer
  Dim yy As Integer
  Dim rows As Integer
  Dim columns As Integer
  Dim top_x As Integer
  Dim top_y As Integer
  Dim bottom_x As Integer
  Dim bottom_y As Integer

  calibrationActive = True

  Open filename For Input As #1
  Input #1, top_x
  Input #1, top_y
  Input #1, bottom_x
  Input #1, bottom_y
  Input #1, rows
  Input #1, columns
  For chess_x = 0 To columns
    For chess_y = 0 To rows
      Input #1, x
      Input #1, y
    Next
  Next
  Input #1, w
  Input #1, h
  For x = 0 To w - 1
    For y = 0 To h - 1
      Input #1, xx
      Input #1, yy
      CalibrationMap(x, y, 0) = xx
      CalibrationMap(x, y, 1) = yy
    Next
  Next
  Close #1
End Sub


Public Sub init(ImageWidth As Integer, ImageHeight As Integer)
  width = ImageWidth
  height = ImageHeight
  ReDim image(width, height, 3)
  ReDim Integral(width, height, 2)
  
  calibrationActive = False
  ReDim CalibrationMap(width, height, 2)
End Sub



Public Sub Update(canvas As PictureBox)
'import a picture
  Dim x As Integer
  Dim y As Integer
  Dim screenX As Integer
  Dim screenY As Integer
  Dim w As Integer
  Dim h As Integer
  Dim xx As Integer
  Dim yy As Integer
  Dim r As Double
  Dim g As Double
  Dim b As Double
  Dim RGBval As Long
  Dim pixels As Double
  Dim maxCol As Long
  Dim edgeValue As Single
  Dim screenWidth As Single
  Dim screenHeight As Single
  Dim screenLeft As Single
  Dim screenTop As Single
  Dim dx As Integer
  Dim dy As Integer
  Dim px As Integer
  Dim py As Integer
  
  screenLeft = 0
  screenTop = 0
  screenWidth = canvas.ScaleWidth
  screenHeight = canvas.ScaleHeight
  
  w = CInt(screenWidth / width)
  If (w < 1) Then
    w = 1
  End If
  h = CInt(screenHeight / height)
  If (h < 1) Then
    h = 1
  End If

  pixels = w * h
  maxCol = RGB(255, 255, 255)
  For x = 0 To width - 1
    For y = 0 To height - 1
      
      If (calibrationActive) Then
        dx = CalibrationMap(x, y, 0)
        dy = CalibrationMap(x, y, 1)
        Else
        dx = 0
        dy = 0
      End If
      
      screenX = screenLeft + ((x / width) * screenWidth)
      screenY = screenTop + ((y / height) * screenHeight)
      r = 0
      g = 0
      b = 0
      For xx = screenX To screenX + w - 1
        For yy = screenY To screenY + h - 1
        
          px = xx + (dx * (screenWidth / width))
          py = yy + (dy * (screenHeight / height))
          'If (px < 0) Then px = 0
          'If (py < 0) Then py = 0
          'If (px > width - 1) Then px = width - 1
          'If (py > height - 1) Then py = height - 1
          
          RGBval = canvas.Point(px, py)
          RGBsource.Value = RGBval
          Call CopyMemory(RGBdest, RGBsource, 3)
          r = r + RGBdest.Red
          g = g + RGBdest.Green
          b = b + RGBdest.Blue
          
        Next
      Next
      r = (r / pixels)
      g = (g / pixels)
      b = (b / pixels)
      
                 
      image(x, y, 0) = r
      image(x, y, 1) = g
      image(x, y, 2) = b
    Next
  Next
  
  Call updateIntegralImage

End Sub


Public Sub Show(canvas As PictureBox)
  Dim x As Integer
  Dim y As Integer
  Dim screenX(2) As Single
  Dim screenY(2) As Single
  Dim r As Byte
  Dim g As Byte
  Dim b As Byte
  Dim c As Long
  Dim i As Integer
  Dim tx As Integer
  Dim ty As Integer
  Dim bx As Integer
  Dim by As Integer
  
  canvas.FillStyle = 0
  For x = 0 To width - 1
    For y = 0 To height - 1
      r = image(x, y, 0)
      g = image(x, y, 1)
      b = image(x, y, 2)
      
      c = RGB(r, g, b)
      canvas.FillColor = c
      tx = (x / width) * canvas.ScaleWidth
      ty = (y / height) * canvas.ScaleHeight
      bx = ((x + 1) / width) * canvas.ScaleWidth
      by = ((y + 1) / height) * canvas.ScaleHeight
      canvas.Line (tx, ty)-(bx, by), c, B
    Next
  Next
    
End Sub


Public Function setPoint(x As Integer, y As Integer, col As Integer, Value As Byte)
  image(x, y, col) = Value
End Function


Public Function getPoint(x As Integer, y As Integer, col As Integer) As Byte
  getPoint = image(x, y, col)
End Function



Private Sub updateIntegralImage()
  Dim x As Integer
  Dim y As Integer
  Dim p As Long
  Dim p2 As Long
  Dim textureVal As Long
  Dim v1 As Integer
  Dim v2 As Integer
  Dim tVal As Integer
  
  textureVal = 0
  For y = 0 To height - 1
    p = 0
    p2 = 0
    For x = 0 To width - 1
      v1 = image(x, y, 0)
      If (x > 0) Then v2 = image(x - 1, y, 0) Else v2 = 0
      tVal = Abs(v1 - v2)
      If (y > 0) Then v2 = image(x, y - 1, 0) Else v2 = 0
      tVal = tVal + Abs(v1 - v2)
      textureVal = textureVal + tVal
      
      p = p + image(x, y, 0)
      p2 = p2 + tVal
      If (y > 0) Then
        Integral(x, y, 0) = p + Integral(x, y - 1, 0)
        Integral(x, y, 1) = p2 + Integral(x, y - 1, 1)
        Else
        Integral(x, y, 0) = p
        Integral(x, y, 1) = p2
      End If
    Next
  Next
  Texture = textureVal / (width * height)
  
End Sub


Private Function getIntegral(tx As Integer, ty As Integer, bx As Integer, by As Integer, index As Integer) As Long
  getIntegral = Integral(bx, by, index) + Integral(tx, ty, index) - (Integral(tx, by, index) + Integral(bx, ty, index))
End Function


Public Function detectFeature(x As Integer, y As Integer, wdth As Integer, hght As Integer, featureType As Integer) As Long
  Dim area(4) As Long
  Dim v As Long
  
  Select Case featureType
    Case 0  'A
      area(0) = getIntegral(x, y, x + wdth, y + hght + hght, 0)
      area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght + hght, 0)
      v = Abs(area(0) - area(1))
    Case 1  'B
      area(0) = getIntegral(x, y, x + wdth, y + hght, 0)
      area(1) = getIntegral(x, y + hght, x + wdth, y + hght + hght, 0)
      v = Abs(area(0) - area(1))
    Case 2  'C
      area(0) = getIntegral(x, y, x + wdth, y + hght, 0)
      area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 0)
      area(2) = getIntegral(x + wdth + wdth, y, x + wdth + wdth + wdth, y + hght, 0)
      v = Abs((area(1) * 2) - (area(0) + area(2)))
    Case 3  'D
      area(0) = getIntegral(x, y, x + wdth, y + hght, 0)
      area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 0)
      area(2) = getIntegral(x, y + hght, x + wdth, y + hght + hght, 0)
      area(3) = getIntegral(x + wdth, y + hght, x + wdth + wdth, y + hght + hght, 0)
      v = Abs((area(1) + area(2)) - (area(0) + area(3)))
    Case 4  'A
      area(0) = getIntegral(x, y, x + wdth, y + hght + hght, 1)
      area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght + hght, 1)
      v = Abs(area(0) - area(1))
    Case 5  'B
      area(0) = getIntegral(x, y, x + wdth, y + hght, 1)
      area(1) = getIntegral(x, y + hght, x + wdth, y + hght + hght, 1)
      v = Abs(area(0) - area(1))
    Case 6  'C
      area(0) = getIntegral(x, y, x + wdth, y + hght, 1)
      area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 1)
      area(2) = getIntegral(x + wdth + wdth, y, x + wdth + wdth + wdth, y + hght, 1)
      v = Abs((area(1) * 2) - (area(0) + area(2)))
    Case 7  'D
      area(0) = getIntegral(x, y, x + wdth, y + hght, 1)
      area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 1)
      area(2) = getIntegral(x, y + hght, x + wdth, y + hght + hght, 1)
      area(3) = getIntegral(x + wdth, y + hght, x + wdth + wdth, y + hght + hght, 1)
      v = Abs((area(1) + area(2)) - (area(0) + area(3)))
  End Select
  
  detectFeature = v
End Function



Public Sub sampleFromImage(img As ClassImage, tx As Integer, ty As Integer, bx As Integer, by As Integer)
  Dim x As Integer
  Dim y As Integer
  Dim xx As Integer
  Dim yy As Integer
  Dim dx As Integer
  Dim dy As Integer
  Dim c As Integer
  
  dx = bx - tx
  dy = by - ty
  
  For x = 0 To width - 1
    xx = tx + ((x * dx) / width)
    
    For y = 0 To height - 1
      yy = ty + ((y * dy) / height)
    
      For c = 0 To 2
        image(x, y, c) = img.getPoint(xx, yy, c)
      Next
    Next
  Next
  
  Call updateIntegralImage
End Sub



Public Sub getEdges(sourceImage As ClassImage)
  Dim x As Integer
  Dim y As Integer
  Dim p(5) As Integer
  Dim edge As Integer
  Dim n As Integer
  
  For x = 1 To width - 2
    For y = 1 To height - 2
      p(0) = image(x, y, 0)
      p(1) = sourceImage.getPoint(x - 1, y, 0)
      p(2) = sourceImage.getPoint(x + 1, y, 0)
      p(3) = sourceImage.getPoint(x, y - 1, 0)
      p(4) = sourceImage.getPoint(x, y + 1, 0)
      edge = 0
      For n = 1 To 4
        edge = edge + Abs(p(0) - p(n))
      Next
      edge = edge / 4
      image(x, y, 0) = edge
      image(x, y, 1) = edge
      image(x, y, 2) = edge
    Next
  Next
End Sub
