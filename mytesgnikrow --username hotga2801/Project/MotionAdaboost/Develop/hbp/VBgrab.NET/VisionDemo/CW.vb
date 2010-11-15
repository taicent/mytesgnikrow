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

Public Class CW
    Inherits System.Windows.Forms.Form

#Region " Código generado por el Diseñador de Windows Forms "

    Public Sub New()
        MyBase.New()

        'El Diseñador de Windows Forms requiere esta llamada.
        InitializeComponent()

        Dim Tamano As Size = CaptureInformation.CaptureInfo.FrameSize
        Dim Rate As Integer = CaptureInformation.CaptureInfo.FrameRate * 1000
        Dim list As ListBox
        Dim i As Integer
        Dim fAux As Filter
        Dim p As PropertyPage

        'Add available capture sizes
        Me.cmbTam.Items.Clear()
        Me.cmbTam.Items.Add("160x120")
        Me.cmbTam.Items.Add("176x144")
        Me.cmbTam.Items.Add("320x240")
        Me.cmbTam.Items.Add("352x288")
        Me.cmbTam.Items.Add("640x480")
        If (Tamano.Equals(New Size(160, 120))) Then
            Me.cmbTam.Text = "160x120"
        End If
        If (Tamano.Equals(New Size(176, 144))) Then
            Me.cmbTam.Text = "176x144"
        End If
        If (Tamano.Equals(New Size(320, 240))) Then
            Me.cmbTam.Text = "320x240"
        End If
        If (Tamano.Equals(New Size(352, 288))) Then
            Me.cmbTam.Text = "352x288"
        End If
        If (Tamano.Equals(New Size(640, 480))) Then
            Me.cmbTam.Text = "640x480"
        End If

        'Add available capture frames per second
        Me.cmbFPS.Items.Clear()
        Me.cmbFPS.Items.Add("5 fps")
        Me.cmbFPS.Items.Add("10 fps")
        Me.cmbFPS.Items.Add("15 fps")
        Me.cmbFPS.Items.Add("20 fps")
        Me.cmbFPS.Items.Add("25 fps (PAL)")
        Me.cmbFPS.Items.Add("30 fps")
        Me.cmbFPS.Items.Add("60 fps")
        If (Rate = 5000) Then
            Me.cmbFPS.Text = "5 fps"
        End If
        If (Rate = 10000) Then
            Me.cmbFPS.Text = "10 fps"
        End If
        If (Rate = 15000) Then
            Me.cmbFPS.Text = "15 fps"
        End If
        If (Rate = 20000) Then
            Me.cmbFPS.Text = "20 fps"
        End If
        If (Rate = 25000) Then
            Me.cmbFPS.Text = "25 fps (PAL)"
        End If
        If (Rate = 30000) Then
            Me.cmbFPS.Text = "30 fps"
        End If
        If (Rate = 60000) Then
            Me.cmbFPS.Text = "60 fps"
        End If

        'Add the possible compressors to use in capturing
        'Me.cmbCompress.Items.Clear()
        'Me.cmbCompress.Items.Add("Without Compressor")
        'If (CaptureInformation.CaptureInfo.VideoCompressor Is Nothing) Then
        'Me.cmbCompress.Text = "Without Compressor"
        'End If
        'For i = 0 To Dispositivos.VideoCompressors.Count - 1
        'fAux = Dispositivos.VideoCompressors(i)
        'Me.cmbCompress.Items.Add(fAux.Name)
        'If (CaptureInformation.CaptureInfo.VideoCompressor Is fAux) Then
        'Me.cmbCompress.Text = fAux.Name
        'End If
        'Next

        'Driver Configuration

        'For i = 0 To CaptureInformation.CaptureInfo.PropertyPages.Count - 1
        'p = CaptureInformation.CaptureInfo.PropertyPages(i)
        'Me.lboxDriver.Items.Add(p.Name + "...")
        'Next


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
    Friend WithEvents lblFPS As System.Windows.Forms.Label
    Friend WithEvents lblTam As System.Windows.Forms.Label
    Friend WithEvents cmbFPS As System.Windows.Forms.ComboBox
    Friend WithEvents cmbTam As System.Windows.Forms.ComboBox
    Public WithEvents OKButton As System.Windows.Forms.Button
    Public WithEvents CanButton As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.lblFPS = New System.Windows.Forms.Label
        Me.lblTam = New System.Windows.Forms.Label
        Me.cmbFPS = New System.Windows.Forms.ComboBox
        Me.cmbTam = New System.Windows.Forms.ComboBox
        Me.OKButton = New System.Windows.Forms.Button
        Me.CanButton = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'lblFPS
        '
        Me.lblFPS.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.lblFPS.Location = New System.Drawing.Point(32, 16)
        Me.lblFPS.Name = "lblFPS"
        Me.lblFPS.Size = New System.Drawing.Size(124, 16)
        Me.lblFPS.TabIndex = 1
        Me.lblFPS.Text = "- Frames per second"
        '
        'lblTam
        '
        Me.lblTam.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.lblTam.Location = New System.Drawing.Point(32, 72)
        Me.lblTam.Name = "lblTam"
        Me.lblTam.Size = New System.Drawing.Size(124, 16)
        Me.lblTam.TabIndex = 2
        Me.lblTam.Text = "- Image size"
        '
        'cmbFPS
        '
        Me.cmbFPS.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.cmbFPS.Location = New System.Drawing.Point(64, 40)
        Me.cmbFPS.Name = "cmbFPS"
        Me.cmbFPS.Size = New System.Drawing.Size(212, 21)
        Me.cmbFPS.TabIndex = 4
        '
        'cmbTam
        '
        Me.cmbTam.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.cmbTam.Location = New System.Drawing.Point(64, 96)
        Me.cmbTam.Name = "cmbTam"
        Me.cmbTam.Size = New System.Drawing.Size(212, 21)
        Me.cmbTam.TabIndex = 5
        '
        'OKButton
        '
        Me.OKButton.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.OKButton.BackColor = System.Drawing.SystemColors.Control
        Me.OKButton.Cursor = System.Windows.Forms.Cursors.Default
        Me.OKButton.ForeColor = System.Drawing.SystemColors.ControlText
        Me.OKButton.Location = New System.Drawing.Point(104, 136)
        Me.OKButton.Name = "OKButton"
        Me.OKButton.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.OKButton.Size = New System.Drawing.Size(81, 25)
        Me.OKButton.TabIndex = 7
        Me.OKButton.Text = "OK"
        '
        'CanButton
        '
        Me.CanButton.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CanButton.BackColor = System.Drawing.SystemColors.Control
        Me.CanButton.Cursor = System.Windows.Forms.Cursors.Default
        Me.CanButton.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.CanButton.ForeColor = System.Drawing.SystemColors.ControlText
        Me.CanButton.Location = New System.Drawing.Point(200, 136)
        Me.CanButton.Name = "CanButton"
        Me.CanButton.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.CanButton.Size = New System.Drawing.Size(81, 25)
        Me.CanButton.TabIndex = 8
        Me.CanButton.Text = "Cancel"
        '
        'CW
        '
        Me.AcceptButton = Me.OKButton
        Me.AutoScale = False
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.CancelButton = Me.CanButton
        Me.ClientSize = New System.Drawing.Size(292, 168)
        Me.Controls.Add(Me.CanButton)
        Me.Controls.Add(Me.OKButton)
        Me.Controls.Add(Me.cmbTam)
        Me.Controls.Add(Me.cmbFPS)
        Me.Controls.Add(Me.lblTam)
        Me.Controls.Add(Me.lblFPS)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "CW"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Camera configuration"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub lboxDriver_MouseDown(ByVal sender As System.Object, ByVal e As System.Windows.Forms.MouseEventArgs)

        'CaptureInformation.CaptureInfo.PropertyPages(Me.lboxDriver.SelectedIndex).Show(CW.ActiveForm)

    End Sub

    Private Sub DoIt()
        ConfParamCam()
        PrepareCam(CaptureInformation.PathVideo)
        Me.Hide()
        CaptureInformation.CaptureInfo.CaptureFrame()

        CamSettings.frameRate = cmbFPS.Text
        CamSettings.resolution = cmbTam.Text
        CamSettings.Save()
    End Sub

    Private Sub OKButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles OKButton.Click
        CamSettings.firstTime = False
        DoIt()
    End Sub

    Private Sub CanButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles CanButton.Click
        CamSettings.firstTime = False
        Me.Close()
    End Sub

    Private Sub CW_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        If (CamSettings.frameRate <> "") Then
            cmbFPS.Text = CamSettings.frameRate
        End If

        If (CamSettings.resolution <> "") Then
            cmbTam.Text = CamSettings.resolution
        End If

        If (CamSettings.firstTime) And (CamSettings.frameRate <> "") And (CamSettings.resolution <> "") Then
            CamSettings.firstTime = False
            DoIt()
            Me.Close()
        End If
    End Sub
End Class
