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


Imports System.io

''' <summary>
''' This class stores, loads and saves useful parameters to a file called settings.txt
''' </summary>
Public Class cameraSettings

#Region "properties"
    Public cameraName As String = ""
    Public frameRate As String = ""
    Public resolution As String = ""
    Public firstTime As Boolean = True
    Public createLog As Boolean = False
    Public DisplayMode As Integer
    Public speakNames As Boolean = False
#End Region

#Region "Methods"

    ''' <summary>
    ''' Load settings from settings.txt file
    ''' </summary>
    Public Sub Load()
        Dim oFile As System.IO.File
        Dim oRead As System.IO.StreamReader
        Dim str As String
        Dim filefound As Boolean = True

        cameraName = ""

        Try
            oRead = oFile.OpenText(System.Windows.Forms.Application.StartupPath & "\settings.txt")
        Catch
            filefound = False
        End Try

        If (filefound) Then
            str = oRead.ReadLine()
            If (Not (str Is Nothing)) Then
                cameraName = str
            End If
            str = oRead.ReadLine()
            If (Not (str Is Nothing)) Then
                frameRate = str
            End If
            str = oRead.ReadLine()
            If (Not (str Is Nothing)) Then
                resolution = str
            End If
            str = oRead.ReadLine()
            If (Not (str Is Nothing)) Then
                If (Val(str) = 1) Then
                    createLog = True
                Else
                    createLog = False
                End If
            End If
            str = oRead.ReadLine()
            If (Not (str Is Nothing)) Then
                DisplayMode = Val(str)
            End If
            str = oRead.ReadLine()
            If (Not (str Is Nothing)) Then
                If (Val(str) = 1) Then
                    speakNames = True
                Else
                    speakNames = False
                End If
            End If
            oRead.Close()
        End If

    End Sub


    ''' <summary>
    ''' Save settings to settings.txt file
    ''' </summary>
    Public Sub Save()
        Dim oFile As System.IO.File
        Dim oWrite As System.IO.StreamWriter
        Dim allowWrite As Boolean = True

        Try
            oWrite = oFile.CreateText(System.Windows.Forms.Application.StartupPath & "\settings.txt")
        Catch
            allowWrite = False
        End Try

        If (allowWrite) Then
            oWrite.WriteLine(cameraName)
            oWrite.WriteLine(frameRate)
            oWrite.WriteLine(resolution)
            If (createLog) Then
                oWrite.WriteLine("1")
            Else
                oWrite.WriteLine("0")
            End If
            oWrite.WriteLine(CStr(DisplayMode))
            If (speakNames) Then
                oWrite.WriteLine("1")
            Else
                oWrite.WriteLine("0")
            End If
            oWrite.Close()
        End If

    End Sub
#End Region

End Class
