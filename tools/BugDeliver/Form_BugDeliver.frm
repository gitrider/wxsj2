VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form Form_BugDeliver 
   Caption         =   "bug分发"
   ClientHeight    =   1020
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7410
   LinkTopic       =   "Form1"
   ScaleHeight     =   1020
   ScaleWidth      =   7410
   StartUpPosition =   2  '屏幕中心
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   3360
      Top             =   360
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
   End
   Begin VB.Label Label_Warning 
      Caption         =   "请注意RTX Server必须先启动"
      BeginProperty Font 
         Name            =   "楷体_GB2312"
         Size            =   26.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   855
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   7215
   End
End
Attribute VB_Name = "Form_BugDeliver"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
    Winsock1.LocalPort = 5555
    Winsock1.RemoteHost = "127.0.0.1"
    Winsock1.RemotePort = 5555
    Winsock1.Bind
End Sub

Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
    On Error Resume Next
    Dim strContent As String
    Winsock1.GetData strContent
    'strContent = "chenjun;666666;genghao;bug是程序员的爱人，你的爱人找你来了。;{45E974F3-B242-486b-8487-56C23D37FF51}"
    Dim strArray() As String
    strArray = Split(strContent, ";")
    
    If UBound(strArray) < 4 Then
        Exit Sub
    End If
    
    Debug.Print strArray(0)
    Debug.Print strArray(1)
    Debug.Print strArray(2)
    Debug.Print strArray(3)
    Debug.Print strArray(4)
    Set RootObj = CreateObject("RTXSAPIRootObj.RTXSAPIRootObj") ' 创建根对象
    'RootObj.SendIM "chenjun", "666666", "genghao", "bug是程序员的爱人，你的爱人找你来了。", "{45E974F3-B242-486b-8487-56C23D37FF51}"
    RootObj.SendIM strArray(0), strArray(1), strArray(2), strArray(3), strArray(4)
    Set RootObj = Nothing
End Sub

