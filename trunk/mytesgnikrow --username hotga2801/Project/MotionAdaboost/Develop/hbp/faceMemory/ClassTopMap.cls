VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ClassTopMap"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Rodney robot - topological map
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


Option Explicit

Public Name As String

Public width As Integer
Public height As Integer
Public inputs_width As Integer
Public inputs_height As Integer
Dim unit() As Single
Dim image() As Byte
Dim outputs() As Single
Dim hits() As Long
Dim max_hits As Long
Dim minValue As Single
Dim h As Single
Dim average_similarity As Single

'a number used to classify a point on the map
Dim classification() As Integer
Dim classificationMulti() As Long
Dim ClassificationColour(1000) As Long
Dim ClassificationName(20, 20) As Byte  'name of each class
Dim Portrait(20, 150) As Byte   'filename of a jpg image associated with the class

Public learningRate As Single 'o<=r<=1
Public randomness As Single  '0<=r<=1

Public RadiusExcite As Integer

Dim inputs() As Single

Public WinnerX As Integer
Public WinnerY As Integer

Public Threshold As Single


Public Sub init(InputsWidth As Integer, InputsHeight As Integer, mapWidth As Integer, mapHeight As Integer)
  On Error GoTo init_err
  
  Dim x As Integer
  Dim y As Integer
  
  inputs_width = InputsWidth
  inputs_height = InputsHeight
  ReDim inputs(inputs_width, inputs_height)
    
  width = mapWidth
  height = mapHeight
  ReDim unit(width, height, inputs_width, inputs_height)
  ReDim image(width, height, inputs_width, inputs_height)
  ReDim outputs(width, height)
  ReDim hits(width, height)
  ReDim classification(width, height)
  ReDim classificationMulti(width, height, 10)
  
  learningRate = 0.5
  RadiusExcite = 1
  randomness = 0.01
  Threshold = 0#
  Call initWeights
  
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
  MsgBox "classTopmap/init/" & Error$(Err)
  Resume init_exit
End Sub


Public Sub setPortrait(index As Integer, filename As String)
'stores a filename associated with the given class index
  Dim i As Integer
  Dim l As Integer
  
  filename = Left$(filename, 150)
  l = Len(filename)
  For i = 0 To l
    If (i < l) Then
      Portrait(index, i) = Asc(Mid$(filename, i + 1, 1))
      Else
      Portrait(index, i) = 13
    End If
  Next
  
End Sub

Public Function getPortrait(index As Integer) As String
'returns a filename associated with the given class index
  Dim i As Integer
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
  Wend
  getPortrait = filename
End Function



Public Sub setClassificationColour(classificationID, Red As Integer, Green As Integer, Blue As Integer)
'sets the colour associated with a classification
  ClassificationColour(classificationID) = RGB(Red, Green, Blue)
End Sub


Public Function getClassificationColour(classificationID) As Long
'gets the colour associated with a classification
  getClassificationColour = ClassificationColour(classificationID)
End Function



Public Sub setInput(x As Integer, y As Integer, Value As Single)
  inputs(x, y) = Value
End Sub


Public Function getOutput(x As Integer, y As Integer) As Single
  getOutput = outputs(x, y)
End Function


Private Function similarity(x As Integer, y As Integer) As Single
'returns the similarity between the input array and the map unit
  On Error GoTo similarity_err
  
  Dim xx As Integer
  Dim yy As Integer
  Dim Value As Single
  Dim dv As Single
  
  Value = 0
  For xx = 0 To inputs_width - 1
    For yy = 0 To inputs_height - 1
      dv = Abs(unit(x, y, xx, yy) - inputs(xx, yy))
      Value = Value + dv
    Next
  Next
  similarity = ((1 - randomness) * (Value / (inputs_width * inputs_height))) + (randomness * Rnd)
  
  
similarity_exit:
  Exit Function
similarity_err:
  MsgBox "classTopmap/similarity/" & Error$(Err)
  Resume similarity_exit
End Function


Public Sub Update()
  On Error GoTo update_err
  
  Dim x As Integer
  Dim y As Integer
  Dim least_hits As Integer
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
      If (max_hits > 32000) Then Call rescaleHits
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
  MsgBox "classTopmap/update/" & Error$(Err)
  Resume update_exit
End Sub


Private Function dist(x As Integer, y As Integer) As Single
  dist = (((x - WinnerX) * (x - WinnerX)) + ((y - WinnerY) * (y - WinnerY)))
End Function


Private Function randVal(Value As Single) As Single
'adds randomness to a value
  randVal = ((1 - randomness) * Value) + (randomness * Rnd)
End Function


Private Sub adjustWeights(x As Integer, y As Integer, Value As Single)
'adjusts weights of the unit in accordance with its value
  On Error GoTo adjustWeights_err
  
  Dim xx As Integer
  Dim yy As Integer
  Dim dw As Single
  Dim w As Single
  
  If (classification(x, y) = 0) Then
    For xx = 0 To inputs_width - 1
      For yy = 0 To inputs_height - 1
        w = unit(x, y, xx, yy)
        dw = inputs(xx, yy) - w
        w = w + (learningRate * dw * Exp(-Value * Value))
        unit(x, y, xx, yy) = w
      Next
    Next
  End If
  
adjustWeights_exit:
  Exit Sub
adjustWeights_err:
  MsgBox "classTopmap/adjustWeights/" & Error$(Err)
  Resume adjustWeights_exit
End Sub


Public Sub learn()
  On Error GoTo learn_err
  
  Dim x As Integer
  Dim y As Integer
  Dim d As Single
  Dim e As Single
  Dim i As Integer
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
        image(WinnerX, WinnerY, x, y) = inputs(x, y)
      Next
    Next
  End If
  
learn_exit:
  Exit Sub
learn_err:
  MsgBox "classTopmap/learn/" & Error$(Err)
  Resume learn_exit
End Sub



Public Sub initWeights(Optional minVal As Variant, Optional maxVal As Variant)
'randomly initialises the weights within the given range
  On Error GoTo initWeights_err
  
  Dim min As Single
  Dim max As Single
  Dim x As Integer
  Dim y As Integer
  Dim xx As Integer
  Dim yy As Integer
  
  min = 0
  max = 0.01
  
  If (Not IsMissing(minVal)) Then
    min = minVal
  End If
  
  If (Not IsMissing(maxVal)) Then
    max = maxVal
  End If
  
  For x = 0 To width - 1
    For y = 0 To height - 1
      For xx = 0 To inputs_width - 1
        For yy = 0 To inputs_height - 1
          unit(x, y, xx, yy) = min + (Rnd * (max - min))
        Next
      Next
    Next
  Next
  
initWeights_exit:
  Exit Sub
initWeights_err:
  MsgBox "classTopmap/initWeights/" & Error$(Err)
  Resume initWeights_exit
End Sub


Public Sub Show(canvas As Object)
  On Error GoTo show_err
  
  Dim x As Integer
  Dim y As Integer
  Dim tx As Single
  Dim ty As Single
  Dim bx As Single
  Dim by As Single
  Dim Value As Single
  Dim c As Long
  Dim i As Integer
  
  canvas.FillStyle = 0
  For x = 0 To width - 1
    For y = 0 To height - 1
      Value = outputs(x, y) * 255
      tx = (x / width) * canvas.ScaleWidth
      ty = (y / height) * canvas.ScaleHeight
      bx = ((x + 1) / width) * canvas.ScaleWidth
      by = ((y + 1) / height) * canvas.ScaleHeight
      i = CInt(Value)
      If (Not ((x = WinnerX) And (y = WinnerY))) Then
        c = RGB(0, i, 255 - i)
        Else
        c = RGB(255, 255, 255)
      End If
      canvas.FillColor = c
      canvas.Line (tx, ty)-(bx, by), c, B
    Next
  Next
  
show_exit:
  Exit Sub
show_err:
  MsgBox "classTopmap/show/" & Error$(Err)
  Resume show_exit
End Sub


Public Sub showClassifications(canvas As Object)
'shows the distribution of classifications
  On Error GoTo show_err
  
  Dim x As Integer
  Dim y As Integer
  Dim screenX(2) As Single
  Dim screenY(2) As Single
  Dim c As Long
  
  canvas.FillStyle = 0
  For x = 0 To width - 1
    For y = 0 To height - 1
      screenX(0) = (x / width) * canvas.ScaleWidth
      screenY(0) = (y / height) * canvas.ScaleHeight
      screenX(1) = ((x + 1) / width) * canvas.ScaleWidth
      screenY(1) = ((y + 1) / height) * canvas.ScaleHeight
      c = ClassificationColour(getClassification(x, y))
      canvas.FillColor = c
      canvas.Line (screenX(0), screenY(0))-(screenX(1), screenY(1)), c, B
    Next
  Next
  
show_exit:
  Exit Sub
show_err:
  MsgBox "classTopmap/show/" & Error$(Err)
  Resume show_exit
End Sub


Public Sub showClassificationsMulti(canvas As Object)
'shows the distribution of classifications
  On Error GoTo show_err
  
  Dim x As Integer
  Dim y As Integer
  Dim tx As Single
  Dim ty As Single
  Dim bx As Single
  Dim by As Single
  Dim Value As Integer
  Dim c(3) As Long
  Dim i As Integer
  Dim col As Long
  Dim c1 As Integer
  Dim c2 As Integer
  Dim c3 As Integer
  
  canvas.FillStyle = 0
  For x = 0 To width - 1
    For y = 0 To height - 1
      tx = (x / width) * canvas.ScaleWidth
      ty = (y / height) * canvas.ScaleHeight
      bx = ((x + 1) / width) * canvas.ScaleWidth
      by = ((y + 1) / height) * canvas.ScaleHeight
      
      For i = 0 To 2
        Value = getClassificationMulti(x, y, i)
        c(i) = Value + 150
        If (c(i) > 255) Then c(i) = 255
      Next
      
      c1 = c(0)
      c2 = c(1)
      c3 = c(2)
      col = RGB(c1, c2, c1)
      canvas.FillColor = col
      canvas.Line (tx, ty)-(bx, by), col, B
    Next
  Next
  
show_exit:
  Exit Sub
show_err:
  MsgBox "classTopmap/showClassificationsMulti/" & Error$(Err)
  Resume 0
  Resume show_exit
End Sub


Public Sub randomInputs()
  On Error GoTo randomInputs_err
  
  Dim x As Integer
  Dim y As Integer
  
  For x = 0 To inputs_width - 1
    For y = 0 To inputs_height - 1
      inputs(x, y) = Rnd
    Next
  Next
  
randomInputs_exit:
  Exit Sub
randomInputs_err:
  MsgBox "classTopmap/randomInputs/" & Error$(Err)
  Resume randomInputs_exit
End Sub


Public Sub setImage(img As Object)
'loads in image into the inputs array
  On Error GoTo setImage_err
  
  Dim x As Integer
  Dim y As Integer
  Dim i As Integer
  
  i = 0
  For x = 0 To img.width - 1
    For y = 0 To img.height - 1
      inputs(i) = img.getPoint(x, y) / 255
      i = i + 1
    Next
  Next
  
setImage_exit:
  Exit Sub
setImage_err:
  MsgBox "classTopmap/setImage/" & Error$(Err)
  Resume setImage_exit
End Sub


Public Sub setImageWeights(x As Integer, y As Integer, img As Object)
'loads in image into the inputs array
  On Error GoTo setImage_err
  
  Dim xx As Integer
  Dim yy As Integer
  Dim i As Integer
  Dim Value As Integer
  
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
  MsgBox "classTopmap/setImageWeights/" & Error$(Err)
  Resume setImage_exit
End Sub



Public Sub setClassification(classificationID As Integer)
'sets the classification value of the winning unit
  If (WinnerX > -1) Then
    classification(WinnerX, WinnerY) = classificationID
  End If
End Sub

Public Sub setClassificationName(classID As Integer, className As String)
'sets the name for the given classification ID
  Dim i As Integer
  Dim ch As String
  Dim length As Integer
  
  className = Left$(className, 19)
  length = Len(className)
  For i = 1 To 18
    If (i <= length) Then
      ch = Mid$(className, i, 1)
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


Public Function getClassificationName(classID As Integer) As String
'gets the name for the given classification ID
  Dim i As Integer
  Dim className As String
  
  className = ""
  For i = 1 To 18
    className = className & Chr(ClassificationName(classID, i))
  Next
  getClassificationName = Trim(className)
End Function



Public Sub setClassificationMulti(classificationIndex As Integer, Value As Integer)
'sets the multi-dimensional classification value of the winning unit
  If (WinnerX > -1) Then
    classificationMulti(WinnerX, WinnerY, classificationIndex) = Value
  End If
End Sub


Public Function getClassification(x As Integer, y As Integer)
'returns the classification of the given unit
  getClassification = classification(x, y)
End Function



Public Function getNearestClassification(x As Integer, y As Integer)
'returns the nearest classification of the given unit
  Dim xx As Integer
  Dim yy As Integer
  Dim classID As Integer
  Dim dx As Integer
  Dim dy As Integer
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
          dist = Sqr((dx * dx) + (dy * dy))
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



Public Function getClassificationMulti(x As Integer, y As Integer, classificationIndex As Integer) As Integer
'returns the classification of the given unit
  getClassificationMulti = classificationMulti(x, y, classificationIndex)
End Function


Public Sub classifyImage(img As Object, classificationID As Integer)
  Call setImage(img)
  Call Update
  Call setClassification(classificationID)
  Call learn
End Sub



Public Sub Save(filename As String)
  On Error GoTo Save_err
  
  Dim x As Integer
  Dim y As Integer
  Dim xx As Integer
  Dim yy As Integer
  Dim i As Integer
  Dim j As Integer

  Open filename For Output As #1
  Print #1, width
  Print #1, height
  Print #1, inputs_width
  Print #1, inputs_height
  
  For x = 0 To width - 1
    For y = 0 To height - 1
      For xx = 0 To inputs_width - 1
        For yy = 0 To inputs_height - 1
          Print #1, unit(x, y, xx, yy)
          Print #1, image(x, y, xx, yy)
        Next
      Next
      
      Print #1, hits(x, y)
      
      Print #1, classification(x, y)
      For i = 0 To 9
        Print #1, classificationMulti(x, y, i)
      Next
    Next
  Next
  
  For i = 0 To 19
    For j = 0 To 19
      Print #1, ClassificationName(i, j)
    Next
  Next
  
  For i = 0 To 19
    For j = 0 To 149
      Print #1, Portrait(i, j)
    Next
  Next
  
  Close #1
  
Save_exit:
  Exit Sub
Save_err:
  MsgBox "classTopMap/Save/" & Err & "/" & Error$(Err)
  Resume Save_exit
End Sub


Public Sub Load(filename As String)
  On Error GoTo Save_err
  
  Dim x As Integer
  Dim y As Integer
  Dim xx As Integer
  Dim yy As Integer
  Dim i As Integer
  Dim j As Integer
  Dim w As Single
  Dim c As Long
  Dim ch As Byte

  Open filename For Input As #1
  Input #1, width
  Input #1, height
  Input #1, inputs_width
  Input #1, inputs_height
  
  max_hits = 0
  
  For x = 0 To width - 1
    For y = 0 To height - 1
      For xx = 0 To inputs_width - 1
        For yy = 0 To inputs_height - 1
          Input #1, w
          unit(x, y, xx, yy) = w
          Input #1, i
          image(x, y, xx, yy) = i
        Next
      Next
      
      Input #1, c
      hits(x, y) = c
      If (c > max_hits) Then max_hits = c
      
      Input #1, c
      classification(x, y) = c
      For i = 0 To 9
        Input #1, c
        classificationMulti(x, y, i) = c
      Next
    Next
  Next
  
  For i = 0 To 19
    For j = 0 To 19
      Input #1, ch
      ClassificationName(i, j) = ch
    Next
  Next
  
  For i = 0 To 19
    For j = 0 To 149
      Input #1, ch
      Portrait(i, j) = ch
    Next
  Next
    
  Close #1
  
Save_exit:
  Exit Sub
Save_err:
  If (Err = 62) Then  'input past the end of file
    Close #1
    Resume Save_exit
  End If

  MsgBox "classTopMap/Load/" & Err & "/" & Error$(Err)
  Resume Save_exit
End Sub


Private Sub rescaleHits()
're-scale the hit scores if they get too big
  Dim x As Integer
  Dim y As Integer
  Dim h As Long

  For x = 0 To width - 1
    For y = 0 To height - 1
      h = hits(x, y)
      h = (h * 1000) / 32000
      hits(x, y) = h
    Next
  Next
End Sub

