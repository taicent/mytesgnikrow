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

Public Class AddCam
    Inherits System.Windows.Forms.Form

    Dim initialised As Boolean = False

#Region " Código generado por el Diseñador de Windows Forms "

    Public Sub New()
        MyBase.New()

        'initialise it baby
        InitializeComponent()

        'cogito ergo sum
        Dim j As Short
        Dim f As Filter

        'what cameras are available?
        cboCamaras.Items.Clear()
        For j = 0 To Dispositivos.VideoInputDevices.Count - 1
            f = Dispositivos.VideoInputDevices(j)
            cboCamaras.Items.Add(f.Name)
        Next
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
    Friend WithEvents cboCamaras As System.Windows.Forms.ComboBox
    Public WithEvents cmdCancel As System.Windows.Forms.Button
    Public WithEvents cmdOK As System.Windows.Forms.Button
    Friend WithEvents timAuto As System.Timers.Timer

    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.cboCamaras = New System.Windows.Forms.ComboBox
        Me.cmdCancel = New System.Windows.Forms.Button
        Me.cmdOK = New System.Windows.Forms.Button
        Me.timAuto = New System.Timers.Timer
        CType(Me.timAuto, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'cboCamaras
        '
        Me.cboCamaras.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.cboCamaras.Location = New System.Drawing.Point(40, 16)
        Me.cboCamaras.Name = "cboCamaras"
        Me.cboCamaras.Size = New System.Drawing.Size(184, 21)
        Me.cboCamaras.TabIndex = 4
        Me.cboCamaras.Text = "Click to chose a camera"
        '
        'cmdCancel
        '
        Me.cmdCancel.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.cmdCancel.BackColor = System.Drawing.SystemColors.Control
        Me.cmdCancel.Cursor = System.Windows.Forms.Cursors.Default
        Me.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.cmdCancel.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdCancel.Location = New System.Drawing.Point(168, 56)
        Me.cmdCancel.Name = "cmdCancel"
        Me.cmdCancel.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.cmdCancel.Size = New System.Drawing.Size(81, 25)
        Me.cmdCancel.TabIndex = 5
        Me.cmdCancel.Text = "Cancel"
        '
        'cmdOK
        '
        Me.cmdOK.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.cmdOK.BackColor = System.Drawing.SystemColors.Control
        Me.cmdOK.Cursor = System.Windows.Forms.Cursors.Default
        Me.cmdOK.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdOK.Location = New System.Drawing.Point(72, 56)
        Me.cmdOK.Name = "cmdOK"
        Me.cmdOK.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.cmdOK.Size = New System.Drawing.Size(81, 25)
        Me.cmdOK.TabIndex = 3
        Me.cmdOK.Text = "OK"
        '
        'timAuto
        '
        Me.timAuto.Enabled = True
        Me.timAuto.Interval = 1000
        Me.timAuto.SynchronizingObject = Me
        '
        'AddCam
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(264, 93)
        Me.Controls.Add(Me.cboCamaras)
        Me.Controls.Add(Me.cmdCancel)
        Me.Controls.Add(Me.cmdOK)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "AddCam"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "AddCam"
        CType(Me.timAuto, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub Bo_Selecta()
        Dim IdVentana As String

        If (cboCamaras.SelectedItem = Nothing) Then
            MsgBox("Select an available camera.", MsgBoxStyle.Exclamation, "Error")
            Exit Sub
        End If

        CaptureInformation.Camera = Dispositivos.VideoInputDevices(cboCamaras.SelectedIndex)
        CaptureInformation.CaptureInfo = New Capture(CaptureInformation.Camera, Nothing)

        CamSettings.cameraName = cboCamaras.Text
        CamSettings.Save()
        Me.Dispose()
    End Sub


    Private Sub cmdOK_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOK.Click
        CamSettings.frameRate = ""
        CamSettings.resolution = ""
        Bo_Selecta()
    End Sub

    Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
        Me.Dispose()
        End
    End Sub


    Private Sub AddCam_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        CamSettings.Load()
        If (CamSettings.cameraName <> "") Then
            cboCamaras.Text = CamSettings.cameraName
            If (CamSettings.firstTime) Then
                Bo_Selecta()
            End If
        End If
    End Sub

    Protected Overrides Sub Finalize()
        MyBase.Finalize()
    End Sub

End Class
