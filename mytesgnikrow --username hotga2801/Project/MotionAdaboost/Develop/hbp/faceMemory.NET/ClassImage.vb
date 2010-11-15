Option Strict Off
Option Explicit On 

Class ClassImage

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

    Public width As Short
    Public height As Short
    Dim img_data(1, 1, 1) As Byte
    Dim Integral(1, 1, 1) As Integer

    Public Texture As Short

    Dim calibrationActive As Boolean
    Dim CalibrationMap(1, 1, 1) As Short

    'Dim RGBsource As RGBthingy
    'Dim RGBdest As RGBpoint


    Public Sub Negative()
        Dim x As Short
        Dim y As Short
        Dim c As Short

        For x = 0 To width - 1
            For y = 0 To height - 1
                For c = 0 To 2
                    img_data(x, y, c) = 255 - img_data(x, y, c)
                Next
            Next
        Next
    End Sub


    Public Sub Rotate()
        Dim x As Short
        Dim y As Short
        Dim c As Short
        Dim temp As Byte

        For x = 0 To width - 1
            For y = 0 To height - 1
                For c = 0 To 2
                    temp = img_data(y, x, c)
                    img_data(y, x, c) = img_data(x, y, c)
                    img_data(x, y, c) = temp
                Next
            Next
        Next
    End Sub



    Public Sub loadCalibration(ByRef filename As String)
        'loads the camera calibration map
        Dim chess_x As Short
        Dim chess_y As Short
        Dim x As Short
        Dim y As Short
        Dim w As Short
        Dim h As Short
        Dim xx As Short
        Dim yy As Short
        Dim rows As Short
        Dim columns As Short
        Dim top_x As Short
        Dim top_y As Short
        Dim bottom_x As Short
        Dim bottom_y As Short

        calibrationActive = True

        FileOpen(1, filename, OpenMode.Input)
        Input(1, top_x)
        Input(1, top_y)
        Input(1, bottom_x)
        Input(1, bottom_y)
        Input(1, rows)
        Input(1, columns)
        For chess_x = 0 To columns
            For chess_y = 0 To rows
                Input(1, x)
                Input(1, y)
            Next
        Next
        Input(1, w)
        Input(1, h)
        For x = 0 To w - 1
            For y = 0 To h - 1
                Input(1, xx)
                Input(1, yy)
                CalibrationMap(x, y, 0) = xx
                CalibrationMap(x, y, 1) = yy
            Next
        Next
        FileClose(1)
    End Sub


    Public Sub init(ByRef ImageWidth As Short, ByRef ImageHeight As Short)
        width = ImageWidth
        height = ImageHeight
        ReDim img_data(width, height, 3)
        ReDim Integral(width, height, 2)

        calibrationActive = False
        ReDim CalibrationMap(width, height, 2)
    End Sub






    Public Sub Show(ByVal pic As PictureBox)
        Dim PenColour As System.Drawing.Color
        Dim rect As System.Drawing.Rectangle
        Dim r, g, b As Byte
        Dim x, y As Integer
        Dim gr As Graphics

        gr = Graphics.FromImage(pic.Image)

        For y = 0 To height - 1
            For x = 0 To width - 1
                r = img_data(x, y, 0)
                g = img_data(x, y, 1)
                b = img_data(x, y, 2)

                PenColour = ColorTranslator.FromWin32(RGB(r, g, b))
                rect.X = pic.Width / width * x '/ 2
                rect.Y = pic.Height / height * y '/ 2
                rect.Width = pic.Width / width * 2
                rect.Height = pic.Height / height * 2
                gr.FillRectangle(New SolidBrush(PenColour), rect)
            Next
        Next
        pic.Refresh()
    End Sub




    Public Function setPoint(ByRef x As Short, ByRef y As Short, ByRef col As Short, ByRef Value As Byte) As Object
        img_data(x, y, col) = Value
    End Function


    Public Function getPoint(ByRef x As Short, ByRef y As Short, ByRef col As Short) As Byte
        getPoint = img_data(x, y, col)
    End Function



    Private Sub updateIntegralimg_data()
        Dim x As Short
        Dim y As Short
        Dim p As Integer
        Dim p2 As Integer
        Dim textureVal As Integer
        Dim v1 As Short
        Dim v2 As Short
        Dim tVal As Short

        textureVal = 0
        For y = 0 To height - 1
            p = 0
            p2 = 0
            For x = 0 To width - 1
                v1 = img_data(x, y, 0)
                If (x > 0) Then v2 = img_data(x - 1, y, 0) Else v2 = 0
                tVal = System.Math.Abs(v1 - v2)
                If (y > 0) Then v2 = img_data(x, y - 1, 0) Else v2 = 0
                tVal = tVal + System.Math.Abs(v1 - v2)
                textureVal = textureVal + tVal

                p = p + img_data(x, y, 0)
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


    Private Function getIntegral(ByRef tx As Short, ByRef ty As Short, ByRef bx As Short, ByRef by As Short, ByRef index As Short) As Integer
        getIntegral = Integral(bx, by, index) + Integral(tx, ty, index) - (Integral(tx, by, index) + Integral(bx, ty, index))
    End Function


    Public Function detectFeature(ByRef x As Short, ByRef y As Short, ByRef wdth As Short, ByRef hght As Short, ByRef featureType As Short) As Integer
        Dim area(4) As Integer
        Dim v As Integer

        Select Case featureType
            Case 0 'A
                area(0) = getIntegral(x, y, x + wdth, y + hght + hght, 0)
                area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght + hght, 0)
                v = System.Math.Abs(area(0) - area(1))
            Case 1 'B
                area(0) = getIntegral(x, y, x + wdth, y + hght, 0)
                area(1) = getIntegral(x, y + hght, x + wdth, y + hght + hght, 0)
                v = System.Math.Abs(area(0) - area(1))
            Case 2 'C
                area(0) = getIntegral(x, y, x + wdth, y + hght, 0)
                area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 0)
                area(2) = getIntegral(x + wdth + wdth, y, x + wdth + wdth + wdth, y + hght, 0)
                v = System.Math.Abs((area(1) * 2) - (area(0) + area(2)))
            Case 3 'D
                area(0) = getIntegral(x, y, x + wdth, y + hght, 0)
                area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 0)
                area(2) = getIntegral(x, y + hght, x + wdth, y + hght + hght, 0)
                area(3) = getIntegral(x + wdth, y + hght, x + wdth + wdth, y + hght + hght, 0)
                v = System.Math.Abs((area(1) + area(2)) - (area(0) + area(3)))
            Case 4 'A
                area(0) = getIntegral(x, y, x + wdth, y + hght + hght, 1)
                area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght + hght, 1)
                v = System.Math.Abs(area(0) - area(1))
            Case 5 'B
                area(0) = getIntegral(x, y, x + wdth, y + hght, 1)
                area(1) = getIntegral(x, y + hght, x + wdth, y + hght + hght, 1)
                v = System.Math.Abs(area(0) - area(1))
            Case 6 'C
                area(0) = getIntegral(x, y, x + wdth, y + hght, 1)
                area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 1)
                area(2) = getIntegral(x + wdth + wdth, y, x + wdth + wdth + wdth, y + hght, 1)
                v = System.Math.Abs((area(1) * 2) - (area(0) + area(2)))
            Case 7 'D
                area(0) = getIntegral(x, y, x + wdth, y + hght, 1)
                area(1) = getIntegral(x + wdth, y, x + wdth + wdth, y + hght, 1)
                area(2) = getIntegral(x, y + hght, x + wdth, y + hght + hght, 1)
                area(3) = getIntegral(x + wdth, y + hght, x + wdth + wdth, y + hght + hght, 1)
                v = System.Math.Abs((area(1) + area(2)) - (area(0) + area(3)))
        End Select

        detectFeature = v
    End Function



    Public Sub sampleFromimg_data(ByRef img As ClassImage, ByRef tx As Short, ByRef ty As Short, ByRef bx As Short, ByRef by As Short)
        Dim x As Short
        Dim y As Short
        Dim xx As Short
        Dim yy As Short
        Dim dx As Short
        Dim dy As Short
        Dim c As Short

        dx = bx - tx
        dy = by - ty

        For x = 0 To width - 1
            xx = tx + ((x * dx) / width)

            For y = 0 To height - 1
                yy = ty + ((y * dy) / height)

                For c = 0 To 2
                    img_data(x, y, c) = img.getPoint(xx, yy, c)
                Next
            Next
        Next

        Call updateIntegralimg_data()
    End Sub



    Public Sub getEdges(ByRef sourceImage As ClassImage)
        Dim x As Short
        Dim y As Short
        Dim p(5) As Short
        Dim edge As Short
        Dim n As Short

        For x = 1 To width - 2
            For y = 1 To height - 2
                p(0) = img_data(x, y, 0)
                p(1) = sourceImage.getPoint(x - 1, y, 0)
                p(2) = sourceImage.getPoint(x + 1, y, 0)
                p(3) = sourceImage.getPoint(x, y - 1, 0)
                p(4) = sourceImage.getPoint(x, y + 1, 0)
                edge = 0
                For n = 1 To 4
                    edge = edge + System.Math.Abs(p(0) - p(n))
                Next
                edge = edge / 4
                img_data(x, y, 0) = edge
                img_data(x, y, 1) = edge
                img_data(x, y, 2) = edge
            Next
        Next
    End Sub
End Class