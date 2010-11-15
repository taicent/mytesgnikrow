Attribute VB_Name = "basMaths"
Option Explicit

Public Function Acos(x As Single) As Single
  If (Abs(x) >= 1) Then
    x = 0.9999 * (x / Abs(x))
  End If
  Acos = Atn(-x / Sqr(-x * x + 1)) + 2 * Atn(1)
End Function

Public Function function_sigmoid(x As Single) As Single
  function_sigmoid = (1# / (1# + Exp(-x)))
End Function

