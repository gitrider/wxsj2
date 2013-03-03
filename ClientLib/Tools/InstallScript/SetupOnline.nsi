;--------------------------------
;使用新界面
	!include "MUI.nsh"
	
;--------------------------------
;基本变量定义
	!include "Tlbb_Common.nsh"
	
;--------------------------------
;用户变量
	!define DEFAULT_DOWN_URL "http://update.tl.sohu.com/tlbb/TLBB_for_Online.exe"
	
	Var URL_FOR_ONLINE
;--------------------------------
;定义输出文件属性

	Name "${PRODUCT_NAME}在线安装程序"
	OutFile "TLBB_OnlineSetup.exe"
	!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP win.bmp
;	!insertmacro MUI_DEFAULT MUI_ICON Install.ico

	VIAddVersionKey  /LANG=2052 ProductName TLBB(Beta)
	VIAddVersionKey  /LANG=2052 Comments SetupFile
	VIAddVersionKey  /LANG=2052 CompanyName "FireFox SOHU.COM"
	VIAddVersionKey  /LANG=2052 FileVersion 0.0.0.1
	VIAddVersionKey  /LANG=2052 ProductVersion 0.0.0.1
	VIAddVersionKey  /LANG=2052 FileDescription TLBB(Beta)
	VIAddVersionKey  /LANG=2052 LegalCopyright (C)SOHU.COM
  
	VIProductVersion 0.0.0.1

	BrandingText "${PRODUCT_PUBLISHER}"
  
	InstallDir "D:\TLBB"

;--------------------------------
;接口
	!define MUI_HEADERIMAGE


;--------------------------------
;都有哪些页
 !insertmacro MUI_PAGE_WELCOME
 !insertmacro MUI_PAGE_DIRECTORY
 Page Custom DownloadPageShow DownloadPageLeave

;--------------------------------
;中文界面
	!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
;下载界面
Function DownloadPageShow
	!insertmacro MUI_HEADER_TEXT "下载完整数据" ""

	;解出sogou
	SOGOU::ExtractSogouDll /NOUNLOAD 
	Pop $1
	
	StrCmp $1 "true" 0 extact_error
	
	;进入下载流程
	SOGOU::ShowDownloadDialog /NOUNLOAD "$URL_FOR_ONLINE" "$INSTDIR"

extact_error:
	Return
		
FunctionEnd

##
Function DownloadPageLeave
FunctionEnd

## Just a dummy section
Section 'A section'
SectionEnd


##onInit
Function .onInit
	Call GetParameters
	Pop $2

;	MessageBox MB_OK $2

# search for quoted /URL
	StrCpy $1 '"'
	Push $2
	Push '"/URL='
  	Call StrStr
  	Pop $0
  	StrCpy $0 $0 "" 1 # skip quote
  	StrCmp $0 "" "" next
# search for non quoted /URL
    StrCpy $1 ' '
    Push $2
    Push '/URL='
    Call StrStr
    Pop $0
next:
	StrCmp $0 "" done
# copy the value after /URL=
    StrCpy $0 $0 "" 5
# search for the next parameter
	Push $0
	Push $1
	Call StrStr
  	Pop $1
  	StrCmp $1 "" done
  	StrLen $1 $1
  	StrCpy $0 $0 -$1
done:
	StrCmp $0 "" 0 +3
	StrCpy $URL_FOR_ONLINE ${DEFAULT_DOWN_URL}
	Return

	Strcpy $URL_FOR_ONLINE $0
FunctionEnd


 ; GetParameters
 ; input, none
 ; output, top of stack (replaces, with e.g. whatever)
 ; modifies no other variables.
 
 Function GetParameters
 
   Push $R0
   Push $R1
   Push $R2
   Push $R3
   
   StrCpy $R2 1
   StrLen $R3 $CMDLINE
   
   ;Check for quote or space
   StrCpy $R0 $CMDLINE $R2
   StrCmp $R0 '"' 0 +3
     StrCpy $R1 '"'
     Goto loop
   StrCpy $R1 " "
   
   loop:
     IntOp $R2 $R2 + 1
     StrCpy $R0 $CMDLINE 1 $R2
     StrCmp $R0 $R1 get
     StrCmp $R2 $R3 get
     Goto loop
   
   get:
     IntOp $R2 $R2 + 1
     StrCpy $R0 $CMDLINE 1 $R2
     StrCmp $R0 " " get
     StrCpy $R0 $CMDLINE "" $R2
   
   Pop $R3
   Pop $R2
   Pop $R1
   Exch $R0
 
 FunctionEnd

 ; StrStr
 ; input, top of stack = string to search for
 ;        top of stack-1 = string to search in
 ; output, top of stack (replaces with the portion of the string remaining)
 ; modifies no other variables.
 ;
 ; Usage:
 ;   Push "this is a long ass string"
 ;   Push "ass"
 ;   Call StrStr
 ;   Pop $R0
 ;  ($R0 at this point is "ass string")

 Function StrStr
   Exch $R1 ; st=haystack,old$R1, $R1=needle
   Exch    ; st=old$R1,haystack
   Exch $R2 ; st=old$R1,old$R2, $R2=haystack
   Push $R3
   Push $R4
   Push $R5
   StrLen $R3 $R1
   StrCpy $R4 0
   ; $R1=needle
   ; $R2=haystack
   ; $R3=len(needle)
   ; $R4=cnt
   ; $R5=tmp
   loop:
     StrCpy $R5 $R2 $R3 $R4
     StrCmp $R5 $R1 done
     StrCmp $R5 "" done
     IntOp $R4 $R4 + 1
     Goto loop
 done:
   StrCpy $R1 $R2 "" $R4
   Pop $R5
   Pop $R4
   Pop $R3
   Pop $R2
   Exch $R1
 FunctionEnd
