Option Strict Off
Option Explicit On
Module basMaths
	
	Public Function Acos(ByRef x As Single) As Single
		If (System.Math.Abs(x) >= 1) Then
			x = 0.9999 * (x / System.Math.Abs(x))
		End If
		Acos = System.Math.Atan(-x / System.Math.Sqrt(-x * x + 1)) + 2 * System.Math.Atan(1)
	End Function
	
	Public Function function_sigmoid(ByRef x As Single) As Single
		function_sigmoid = (1# / (1# + System.Math.Exp(-x)))
	End Function
End Module