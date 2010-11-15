VERSION 5.00
Begin VB.Form frmFilter 
   Caption         =   "Choose camera"
   ClientHeight    =   945
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   3525
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   945
   ScaleWidth      =   3525
   StartUpPosition =   3  'Windows Default
   Begin VB.ComboBox cmbFilter 
      Height          =   315
      Left            =   105
      Sorted          =   -1  'True
      TabIndex        =   0
      Text            =   "Combo1"
      Top             =   405
      Width           =   3315
   End
   Begin VB.Label Label1 
      Caption         =   "Choose the DirectShow filter for your camera"
      Height          =   300
      Left            =   105
      TabIndex        =   1
      Top             =   135
      Width           =   3300
   End
End
Attribute VB_Name = "frmFilter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim gGraph As IMediaControl
Dim gRegFilters As Object


Private Sub populateCombo()
  
  Dim i As Integer
  Dim filter As IRegFilterInfo
  
  'make a new graph
  Set gGraph = New FilgraphManager
  Set gRegFilters = gGraph.RegFilterCollection
    
  cmbFilter.Clear
  i = 0
  While (i < gRegFilters.count)
    Call gRegFilters.Item(i, filter)
    cmbFilter.AddItem filter.Name
    i = i + 1
  Wend
  
End Sub




Private Sub cmbFilter_Click()
  Filter_Name = cmbFilter.Text
End Sub

Private Sub Form_Load()
  Call populateCombo
End Sub
