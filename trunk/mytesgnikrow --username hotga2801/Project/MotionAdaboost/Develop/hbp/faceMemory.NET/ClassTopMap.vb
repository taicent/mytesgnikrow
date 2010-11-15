'Human Body Project - topological map
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


Option Strict Off
Option Explicit On 

''' <summary>
''' This class implements a classical topological map algorithm as described by Tuev Kohonen
''' </summary>
Class ClassTopMap


#Region "Class Properties"

    Public Name As String

    Public width As Short
    Public height As Short
    Public inputs_width As Short
    Public inputs_height As Short
    Dim unit(1, 1, 1, 1) As Single
    Dim img_data(1, 1, 1, 1) As Byte
    Dim outputs(1, 1) As Single
    Dim hits(1, 1) As Integer
    Dim max_hits As Integer
    Dim minValue As Single
    Dim h As Single
    Dim average_similarity As Single

    'a number used to classify a point on the map
    Dim classification(1, 1) As Short
    Dim classificationMulti(1, 1, 1) As Integer
    Dim ClassificationColour(1000) As Long
    Dim ClassificationName(20, 20) As Byte 'name of each class
    Dim Portrait(20, 150) As Byte 'filename of a jpg image associated with the class

    Public learningRate As Single 'o<=r<=1
    Public randomness As Single '0<=r<=1

    Public RadiusExcite As Short

    Dim inputs(1, 1) As Single

    Public WinnerX As Short
    Public WinnerY As Short

    Public Threshold As Single

#End Region

    ''' <summary>
    ''' Initialise the map with the desired dimensions.  It is assumed that the input to the classifier will be a two 
    ''' dimensional image, but if you wish to have only one dimensional input just set InputsHeight to 1.
    ''' </summary>
    ''' <param name="InputsWidth">Width of the input image to be classified</param>
    ''' <param name="InputsHeight">Height of the input image to be classified</param>
    ''' <param name="mapWidth">Width of the topological map</param>
    ''' <param name="mapHeight">Height of the topological map</param>
    Public Sub init(ByRef InputsWidth As Short, ByRef InputsHeight As Short, ByRef mapWidth As Short, ByRef mapHeight As Short)
        On Error GoTo init_err

        Dim x As Short
        Dim y As Short

        inputs_width = InputsWidth
        inputs_height = InputsHeight
        ReDim inputs(inputs_width, inputs_height)

        width = mapWidth
        height = mapHeight
        ReDim unit(width, height, inputs_width, inputs_height)
        ReDim img_data(width, height, inputs_width, inputs_height)
        ReDim outputs(width, height)
        ReDim hits(width, height)
        ReDim classification(width, height)
        ReDim classificationMulti(width, height, 10)

        learningRate = 0.5
        RadiusExcite = 1
        randomness = 0.01
        Threshold = 0.0#
        Call initWeights()

        'zero some parameters
        For x = 0 To width - 1
            For y = 0 To height - 1
                hits(x, y) = 0
                outputs(x, y) = 0
                classification(x, y) = 0
            Next
        Next

init_exit:
        Exit Sub
init_err:
        MsgBox("classTopmap/init/" & ErrorToString(Err.Number))
        Resume init_exit
    End Sub


    ''' <summary>
    ''' Stores a filename associated with the given class index.  In this case the class index 
    ''' is the same as the personID - just a unique number corresponding to a particular classification.
    ''' </summary>
    ''' <param name="index">Index number for the classification</param>
    ''' <param name="filename">An image filename associated with the classification</param>
    Public Sub setPortrait(ByRef index As Short, ByRef filename As String)
        Dim i As Short
        Dim l As Short

        filename = Left(filename, 150)
        l = Len(filename)
        For i = 0 To l
            If (i < l) Then
                Portrait(index, i) = Asc(Mid(filename, i + 1, 1))
            Else
                Portrait(index, i) = 13
            End If
        Next

    End Sub

    ''' <summary>
    ''' Returns a filename associated with the given class index.  In this case the class index 
    ''' is the same as the personID - just a unique number corresponding to a particular classification.
    ''' </summary>
    ''' <param name="index">Index number for the classification</param>
    ''' <returns>Filename of the image</returns>
    Public Function getPortrait(ByRef index As Short) As String
        Dim i As Short
        Dim filename As String
        Dim b As Byte

        filename = ""
        i = 0
        b = 0
        While (i < 150) And (b <> 13)
            b = Portrait(index, i)
            If (b <> 13) Then
                filename = filename & Chr(b)
            End If
            i = i + 1
        End While
        getPortrait = filename
    End Function


    ''' <summary>
    ''' Sets a colour to be assigned to the given class index.  At present this function is not used within the facememory program
    ''' </summary>
    ''' <param name="classificationID">Index number or personID for the classification</param>
    ''' <param name="Red">Red component of the desired colour</param>
    ''' <param name="Green">Green component of the desired colour</param>
    ''' <param name="Blue">Blue component of the desired colour</param>
    Public Sub setClassificationColour(ByRef classificationID As Object, ByRef Red As Short, ByRef Green As Short, ByRef Blue As Short)
        ClassificationColour(classificationID) = RGB(Red, Green, Blue)
    End Sub


    ''' <summary>
    ''' Returns a colour for the given class index.  At present this function is not used within the facememory program
    ''' </summary>
    ''' <param name="classificationID">Index number or personID for the classification</param>
    ''' <returns>A long integer value corresponding to the colour</returns>
    Public Function getClassificationColour(ByRef classificationID As Object) As Long
        getClassificationColour = ClassificationColour(classificationID)
    End Function


    ''' <summary>
    ''' Sets the value of an input in the range 0.0 - 1.0
    ''' </summary>
    ''' <param name="x">X coordinate of the input unit</param>
    ''' <param name="y">Y coordinate of the input unit</param>
    ''' <param name="Value">Value to be assigned to the input</param>
    Public Sub setInput(ByRef x As Short, ByRef y As Short, ByRef Value As Single)
        inputs(x, y) = Value
    End Sub

    ''' <summary>
    ''' Returns the value of an output
    ''' </summary>
    ''' <param name="x">X coordinate on the topological map</param>
    ''' <param name="y">Y coordinate on the topological map</param>
    ''' <returns>Value of the output at the given topological map coordinates</returns>
    Public Function getOutput(ByRef x As Short, ByRef y As Short) As Single
        getOutput = outputs(x, y)
    End Function


    ''' <summary>
    ''' Returns a value indicating how similar the topological map unit is to the current inputs. 
    ''' Smaller values indicate greater similarity
    ''' </summary>
    ''' <param name="x">X coordinate on the topological map</param>
    ''' <param name="y">Y coordinate on the topological map</param>
    ''' <returns>Similarity value</returns>
    Private Function similarity(ByRef x As Short, ByRef y As Short) As Single
        On Error GoTo similarity_err

        Dim xx As Short
        Dim yy As Short
        Dim Value As Single
        Dim dv As Single

        Value = 0
        For xx = 0 To inputs_width - 1
            For yy = 0 To inputs_height - 1
                dv = System.Math.Abs(unit(x, y, xx, yy) - inputs(xx, yy))
                Value = Value + dv
            Next
        Next
        similarity = ((1 - randomness) * (Value / (inputs_width * inputs_height))) + (randomness * Rnd())


similarity_exit:
        Exit Function
similarity_err:
        MsgBox("classTopmap/similarity/" & ErrorToString(Err.Number))
        Resume similarity_exit
    End Function


    ''' <summary>
    ''' Main update routine for the topological map.  This should be called after 
    ''' all current inputs have been assigned using the setInput method
    ''' </summary>
    Public Sub Update()
        On Error GoTo update_err

        Dim x As Short
        Dim y As Short
        Dim least_hits As Short
        Dim Value As Single
        Dim min As Single

        WinnerX = -1
        WinnerY = -1
        min = 9999
        average_similarity = 0
        For x = 0 To width - 1
            For y = 0 To height - 1
                Value = similarity(x, y)
                average_similarity = average_similarity + Value

                If (Value < Threshold) Then
                    If (Value < min) Then
                        min = Value
                        WinnerX = x
                        WinnerY = y
                    End If
                    outputs(x, y) = (1 - Value) * (1 - Value)
                Else
                    outputs(x, y) = 0
                End If
            Next
        Next

        'calculate the average similarity across the map
        average_similarity = average_similarity / (width * height)

        'update hit score
        If (WinnerX > -1) Then
            hits(WinnerX, WinnerY) = hits(WinnerX, WinnerY) + 1
            If (hits(WinnerX, WinnerY) > max_hits) Then
                max_hits = hits(WinnerX, WinnerY)
                If (max_hits > 32000) Then Call rescaleHits()
            End If
        Else

            'find the least used unit
            least_hits = max_hits + 1
            For x = 0 To width - 1
                For y = 0 To height - 1
                    If ((hits(x, y) < least_hits) And (classification(x, y) = 0)) Then
                        least_hits = hits(x, y)
                        WinnerX = x
                        WinnerY = y
                    End If
                Next
            Next

        End If

update_exit:
        Exit Sub
update_err:
        MsgBox("classTopmap/update/" & ErrorToString(Err.Number))
        Resume update_exit
    End Sub


    ''' <summary>
    ''' Returns the two dimensional distance between the given topological map coordinate and the winning unit
    ''' </summary>
    ''' <param name="x">X coordinate on the topological map</param>
    ''' <param name="y">Y coordinate on the topological map</param>
    ''' <returns>Distance</returns>
    Private Function dist(ByRef x As Short, ByRef y As Short) As Single
        dist = (((x - WinnerX) * (x - WinnerX)) + ((y - WinnerY) * (y - WinnerY)))
    End Function


    ''' <summary>
    ''' Takes a value and adds a little random noise to it
    ''' </summary>
    ''' <param name="Value">The value to be made noisy</param>
    ''' <returns>A noisy value</returns>
    Private Function randVal(ByRef Value As Single) As Single
        randVal = ((1 - randomness) * Value) + (randomness * Rnd())
    End Function

    ''' <summary>
    ''' Adjusts weights of the unit in accordance with its value
    ''' </summary>
    ''' <param name="x">X coordinate on the topological map</param>
    ''' <param name="y">Y coordinate on the topological map</param>
    Private Sub adjustWeights(ByRef x As Short, ByRef y As Short, ByRef Value As Single)
        On Error GoTo adjustWeights_err

        Dim xx As Short
        Dim yy As Short
        Dim dw As Single
        Dim w As Single

        If (classification(x, y) = 0) Then
            For xx = 0 To inputs_width - 1
                For yy = 0 To inputs_height - 1
                    w = unit(x, y, xx, yy)
                    dw = inputs(xx, yy) - w
                    w = w + (learningRate * dw * System.Math.Exp(-Value * Value))
                    unit(x, y, xx, yy) = w
                Next
            Next
        End If

adjustWeights_exit:
        Exit Sub
adjustWeights_err:
        MsgBox("classTopmap/adjustWeights/" & ErrorToString(Err.Number))
        Resume adjustWeights_exit
    End Sub

    ''' <summary>
    ''' Perform learning (weight adjustments) for all units on the topological map
    ''' </summary>
    Public Sub learn()
        On Error GoTo learn_err

        Dim x As Short
        Dim y As Short
        Dim d As Single
        Dim e As Single
        Dim i As Short
        Dim Value As Single
        Dim max As Single

        'store the current input image on the winning unit
        If (WinnerX > -1) Then

            max = RadiusExcite * RadiusExcite
            For x = (WinnerX - RadiusExcite) To (WinnerX + RadiusExcite)
                For y = (WinnerY - RadiusExcite) To (WinnerY + RadiusExcite)
                    If ((x >= 0) And (x < width) And (y >= 0) And (y < height)) Then
                        d = dist(x, y) / (2 * max)
                        If (d < 1) Then
                            Value = randVal(d)
                            Call adjustWeights(x, y, Value)

                            If (Not ((x = WinnerX) And (y = WinnerY))) Then
                                For i = 0 To 9
                                    classificationMulti(x, y, i) = classificationMulti(x, y, i) + ((classificationMulti(WinnerX, WinnerY, i) - classificationMulti(x, y, i)) * (1 - d) * learningRate)
                                Next
                            End If
                        End If
                    End If
                Next
            Next

            'update the threshold based upon the average similarity
            Threshold = (Threshold + (average_similarity * 10)) / 2

            For x = 0 To inputs_width - 1
                For y = 0 To inputs_height - 1
                    img_data(WinnerX, WinnerY, x, y) = inputs(x, y)
                Next
            Next
        End If

learn_exit:
        Exit Sub
learn_err:
        MsgBox("classTopmap/learn/" & ErrorToString(Err.Number))
        Resume learn_exit
    End Sub


    ''' <summary>
    ''' Create some initial random weights in the given range
    ''' </summary>
    ''' <param name="minVal">Minimum weight value</param>
    ''' <param name="maxVal">Maximum weight value</param>
    Public Sub initWeights(Optional ByRef minVal As Object = Nothing, Optional ByRef maxVal As Object = Nothing)
        On Error GoTo initWeights_err

        Dim min As Single
        Dim max As Single
        Dim x As Short
        Dim y As Short
        Dim xx As Short
        Dim yy As Short

        min = 0
        max = 0.01

        If (Not IsNothing(minVal)) Then
            min = minVal
        End If

        If (Not IsNothing(maxVal)) Then
            max = maxVal
        End If

        For x = 0 To width - 1
            For y = 0 To height - 1
                For xx = 0 To inputs_width - 1
                    For yy = 0 To inputs_height - 1
                        unit(x, y, xx, yy) = min + (Rnd() * (max - min))
                    Next
                Next
            Next
        Next

initWeights_exit:
        Exit Sub
initWeights_err:
        MsgBox("classTopmap/initWeights/" & ErrorToString(Err.Number))
        Resume initWeights_exit
    End Sub


    ''' <summary>
    ''' Show the outputs of the topolgical map in the given picture box control
    ''' </summary>
    ''' <param name="pic">A picture box control</param>
    Public Sub Show(ByVal pic As PictureBox)
        Dim PenColour As System.Drawing.Color
        Dim rect As System.Drawing.Rectangle
        Dim r, g, b As Byte
        Dim x, y As Integer
        Dim Value As Single
        Dim i As Short
        Dim c As Long
        Dim gr As Graphics

        gr = Graphics.FromImage(pic.Image)

        For y = 0 To height - 1
            For x = 0 To width - 1

                Value = outputs(x, y) * 255
                i = CShort(Value)
                If (Not ((x = WinnerX) And (y = WinnerY))) Then
                    c = RGB(0, i, 255 - i)
                Else
                    c = RGB(255, 255, 255)
                End If

                PenColour = ColorTranslator.FromWin32(c)
                rect.X = pic.Width / width * x / 2
                rect.Y = pic.Height / height * y / 2
                rect.Width = pic.Width / width
                rect.Height = pic.Height / height
                gr.FillRectangle(New SolidBrush(PenColour), rect)
            Next
        Next
        pic.Refresh()
    End Sub



    ''' <summary>
    ''' Show the classifications of the topolgical map in the given picture box control. 
    ''' This uses the colours defined using the setClassificationColour method.
    ''' </summary>
    ''' <param name="pic">A picture box control</param>
    Public Sub showClassifications(ByVal pic As PictureBox)
        Dim PenColour As System.Drawing.Color
        Dim rect As System.Drawing.Rectangle
        Dim r, g, b As Byte
        Dim x, y As Integer
        Dim Value As Single
        Dim i As Short
        Dim c As Long
        Dim gr As Graphics

        gr = Graphics.FromImage(pic.Image)

        For y = 0 To height - 1
            For x = 0 To width - 1
                c = ClassificationColour(getClassification(x, y))
                PenColour = ColorTranslator.FromWin32(c)
                rect.X = pic.Width / width * x / 2
                rect.Y = pic.Height / height * y / 2
                rect.Width = pic.Width / width
                rect.Height = pic.Height / height
                gr.FillRectangle(New SolidBrush(PenColour), rect)
            Next
        Next
        pic.Refresh()
    End Sub


    ''' <summary>
    ''' Show the classifications of the topolgical map in the given picture box control. 
    ''' This uses the colours defined using the setClassificationColour method.
    ''' </summary>
    ''' <param name="pic">A picture box control</param>
    Public Sub showClassificationsMulti(ByVal pic As PictureBox)
        Dim PenColour As System.Drawing.Color
        Dim rect As System.Drawing.Rectangle
        Dim r, g, b As Byte
        Dim x, y As Integer
        Dim Value As Single
        Dim i As Short
        Dim v(3) As Integer
        Dim c As Long
        Dim c1, c2, c3 As Integer
        Dim gr As Graphics

        gr = Graphics.FromImage(pic.Image)

        For y = 0 To height - 1
            For x = 0 To width - 1
                For i = 0 To 2
                    Value = getClassificationMulti(x, y, i)
                    v(i) = Value + 150
                    If (v(i) > 255) Then v(i) = 255
                Next

                c1 = v(0)
                c2 = v(1)
                c3 = v(2)
                c = RGB(c1, c2, c1)

                PenColour = ColorTranslator.FromWin32(c)
                rect.X = pic.Width / width * x / 2
                rect.Y = pic.Height / height * y / 2
                rect.Width = pic.Width / width
                rect.Height = pic.Height / height
                gr.FillRectangle(New SolidBrush(PenColour), rect)
            Next
        Next
        pic.Refresh()
    End Sub


    ''' <summary>
    ''' Greates a random input image, used for testing purposes
    ''' </summary>
    Public Sub randomInputs()
        On Error GoTo randomInputs_err

        Dim x As Short
        Dim y As Short

        For x = 0 To inputs_width - 1
            For y = 0 To inputs_height - 1
                inputs(x, y) = Rnd()
            Next
        Next

randomInputs_exit:
        Exit Sub
randomInputs_err:
        MsgBox("classTopmap/randomInputs/" & ErrorToString(Err.Number))
        Resume randomInputs_exit
    End Sub


    ''' <summary>
    ''' Uses an image object to set the input values.  This can be easier than using the setInput method to set each individual input
    ''' </summary>
    Public Sub setImage(ByRef img As ClassImage)
        Dim x As Short
        Dim y As Short
        Dim i As Short

        i = 0
        For x = 0 To img.width - 1
            For y = 0 To img.height - 1
                inputs(x, y) = img.getPoint(x, y, 0) / 255
                i = i + 1
            Next
        Next
    End Sub


    ''' <summary>
    ''' Updates an image object with the weight values for the topological map unit with the given coordinates. 
    ''' This is used for testing purposes to see what the weights are doing during learning
    ''' </summary>
    ''' <param name="x">X coordinate on the topological map</param>
    ''' <param name="y">Y coordinate on the topological map</param>
    ''' <param name="img">The image object to be updated</param>
    Public Sub setImageWeights(ByRef x As Short, ByRef y As Short, ByRef img As Object)
        On Error GoTo setImage_err

        Dim xx As Short
        Dim yy As Short
        Dim i As Short
        Dim Value As Short

        For xx = 0 To img.width - 1
            For yy = 0 To img.height - 1
                Value = unit(x, y, xx, yy)
                If (Value < 0) Then Value = 0
                If (Value > 255) Then Value = 255
                Call img.setPoint(xx, yy, 0, CByte(Value))
                Call img.setPoint(xx, yy, 1, CByte(Value))
                Call img.setPoint(xx, yy, 2, CByte(Value))
            Next
        Next

setImage_exit:
        Exit Sub
setImage_err:
        MsgBox("classTopmap/setImageWeights/" & ErrorToString(Err.Number))
        Resume setImage_exit
    End Sub



    ''' <summary>
    ''' Sets the classification (personID) for the current winning unit on the topological map
    ''' </summary>
    ''' <param name="classificationID">Classification index or personID</param>
    Public Sub setClassification(ByRef classificationID As Short)
        If (WinnerX > -1) Then
            classification(WinnerX, WinnerY) = classificationID
        End If
    End Sub

    ''' <summary>
    ''' Sets a name for the given classification ID
    ''' </summary>
    ''' <param name="classID">Classification index or personID</param>
    ''' <param name="className">Name of the class (person name)</param>
    Public Sub setClassificationName(ByRef classID As Short, ByRef className As String)
        Dim i As Short
        Dim ch As String
        Dim length As Short

        className = Left(className, 19)
        length = Len(className)
        For i = 1 To 18
            If (i <= length) Then
                ch = Mid(className, i, 1)
                If (ch <> "") Then
                    ClassificationName(classID, i) = Asc(ch)
                Else
                    ClassificationName(classID, i) = 13
                End If
            Else
                ClassificationName(classID, i) = Asc(" ")
            End If
        Next
    End Sub


    ''' <summary>
    ''' Returns the name for the given classification ID
    ''' </summary>
    ''' <param name="classID">Classification index or personID</param>
    ''' <returns>Name of the class or person</returns>
    Public Function getClassificationName(ByRef classID As Short) As String
        Dim i As Short
        Dim className As String

        className = ""
        For i = 1 To 18
            className = className & Chr(ClassificationName(classID, i))
        Next
        getClassificationName = Trim(className)
    End Function


    ''' <summary>
    ''' Sets the multi-dimensional classification value of the winning unit
    ''' </summary>
    ''' <param name="classificationIndex">Classification index or personID</param>
    ''' <param name="Value">Value to be assigned</param>
    Public Sub setClassificationMulti(ByRef classificationIndex As Short, ByRef Value As Short)
        If (WinnerX > -1) Then
            classificationMulti(WinnerX, WinnerY, classificationIndex) = Value
        End If
    End Sub


    ''' <summary>
    ''' Returns the classification index (or personID) of the given topological map unit
    ''' </summary>
    ''' <param name="x">X coordinate on the topological map</param>
    ''' <param name="y">Y coordinate on the topological map</param>
    ''' <returns>Classification index or personID</returns>
    Public Function getClassification(ByRef x As Short, ByRef y As Short) As Object
        getClassification = classification(x, y)
    End Function


    ''' <summary>
    ''' Returns the geographically nearest classification (or personID) for the given topological map coordinates
    ''' </summary>
    ''' <param name="x">X coordinate on the topological map</param>
    ''' <param name="y">Y coordinate on the topological map</param>
    ''' <returns>Classification index or personID</returns>
    Public Function getNearestClassification(ByRef x As Short, ByRef y As Short) As Object
        Dim xx As Short
        Dim yy As Short
        Dim classID As Short
        Dim dx As Short
        Dim dy As Short
        Dim dist As Single
        Dim minDist As Single

        classID = classification(x, y)

        'if this unit is unclassified then return the nearest classification on the map
        If (classID = 0) Then
            minDist = 9999
            For xx = 0 To width - 1
                For yy = 0 To height - 1
                    If (classification(xx, yy) > 0) Then
                        dx = xx - x
                        dy = yy - y
                        dist = System.Math.Sqrt((dx * dx) + (dy * dy))
                        If (dist < minDist) Then
                            minDist = dist
                            classID = classification(xx, yy)
                        End If
                    End If
                Next
            Next
        End If

        getNearestClassification = classID
    End Function


    ''' <summary>
    ''' Returns the multi-dimensional classification of the given topological map unit
    ''' </summary>
    ''' <param name="x">X coordinate on the topological map</param>
    ''' <param name="y">Y coordinate on the topological map</param>
    ''' <param name="classificationIndex">Dimension index for the classification</param>
    ''' <returns>Classification index or personID</returns>
    Public Function getClassificationMulti(ByRef x As Short, ByRef y As Short, ByRef classificationIndex As Short) As Short
        getClassificationMulti = classificationMulti(x, y, classificationIndex)
    End Function


    ''' <summary>
    ''' Classify the given image using the given classification index (or personID)
    ''' </summary>
    ''' <param name="img">Image object</param>
    ''' <param name="classificationID">The classification index or personID to be assigned to the winning unit</param>
    Public Sub classifyimg_data(ByRef img As Object, ByRef classificationID As Short)
        Call setImage(img)
        Call Update()
        Call setClassification(classificationID)
        Call learn()
    End Sub


    ''' <summary>
    ''' Save the topological map data
    ''' </summary>
    ''' <param name="filename">Filename for the data</param>
    Public Sub Save(ByRef filename As String)
        On Error GoTo Save_err

        Dim x As Short
        Dim y As Short
        Dim xx As Short
        Dim yy As Short
        Dim i As Short
        Dim j As Short

        FileOpen(1, filename, OpenMode.Output)
        PrintLine(1, width)
        PrintLine(1, height)
        PrintLine(1, inputs_width)
        PrintLine(1, inputs_height)

        For x = 0 To width - 1
            For y = 0 To height - 1
                For xx = 0 To inputs_width - 1
                    For yy = 0 To inputs_height - 1
                        PrintLine(1, unit(x, y, xx, yy))
                        PrintLine(1, img_data(x, y, xx, yy))
                    Next
                Next

                PrintLine(1, hits(x, y))

                PrintLine(1, classification(x, y))
                For i = 0 To 9
                    PrintLine(1, classificationMulti(x, y, i))
                Next
            Next
        Next

        For i = 0 To 19
            For j = 0 To 19
                PrintLine(1, ClassificationName(i, j))
            Next
        Next

        For i = 0 To 19
            For j = 0 To 149
                PrintLine(1, Portrait(i, j))
            Next
        Next

        FileClose(1)

Save_exit:
        Exit Sub
Save_err:
        MsgBox("classTopMap/Save/" & Err.Number & "/" & ErrorToString(Err.Number))
        Resume Save_exit
    End Sub


    ''' <summary>
    ''' Load the topological map data
    ''' </summary>
    ''' <param name="filename">Filename for the data</param>
    Public Sub Load(ByRef filename As String)
        On Error GoTo Save_err

        Dim x As Short
        Dim y As Short
        Dim xx As Short
        Dim yy As Short
        Dim i As Short
        Dim j As Short
        Dim w As Single
        Dim c As Integer
        Dim ch As Byte

        FileOpen(1, filename, OpenMode.Input)
        Input(1, width)
        Input(1, height)
        Input(1, inputs_width)
        Input(1, inputs_height)

        max_hits = 0

        For x = 0 To width - 1
            For y = 0 To height - 1
                For xx = 0 To inputs_width - 1
                    For yy = 0 To inputs_height - 1
                        Input(1, w)
                        unit(x, y, xx, yy) = w
                        Input(1, i)
                        img_data(x, y, xx, yy) = i
                    Next
                Next

                Input(1, c)
                hits(x, y) = c
                If (c > max_hits) Then max_hits = c

                Input(1, c)
                classification(x, y) = c
                For i = 0 To 9
                    Input(1, c)
                    classificationMulti(x, y, i) = c
                Next
            Next
        Next

        For i = 0 To 19
            For j = 0 To 19
                Input(1, ch)
                ClassificationName(i, j) = ch
            Next
        Next

        For i = 0 To 19
            For j = 0 To 149
                Input(1, ch)
                Portrait(i, j) = ch
            Next
        Next

        FileClose(1)

Save_exit:
        Exit Sub
Save_err:
        If (Err.Number = 62) Then 'input past the end of file
            FileClose(1)
            Resume Save_exit
        End If

        MsgBox("classTopMap/Load/" & Err.Number & "/" & ErrorToString(Err.Number))
        Resume Save_exit
    End Sub


    ''' <summary>
    ''' Re-scale the hit scores if they get too big.  This function is presently not used
    ''' </summary>
    Private Sub rescaleHits()
        Dim x As Short
        Dim y As Short
        Dim h As Integer

        For x = 0 To width - 1
            For y = 0 To height - 1
                h = hits(x, y)
                h = (h * 1000) / 32000
                hits(x, y) = h
            Next
        Next
    End Sub
End Class