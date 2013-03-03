;--------------------------------
;使用新界面
	!include "MUI.nsh"
	
;--------------------------------
;基本变量定义
	!include "CJSH_Common.nsh"
	
;--------------------------------
;定义输出文件属性

	Name "${PRODUCT_NAME}Alpha ${PRODUCT_VERSION}"
	OutFile "u:\天龙八部\天龙八部-${PRODUCT_VERSION}\TLBB_Alpha_${PRODUCT_VERSION}.exe"
	!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP win.bmp

	VIAddVersionKey  /LANG=2052 ProductName TLBB(Alpha)
	VIAddVersionKey  /LANG=2052 Comments SetupFile
	VIAddVersionKey  /LANG=2052 CompanyName "FireFox SOHU.COM"
	VIAddVersionKey  /LANG=2052 FileVersion 0.0.0.2
	VIAddVersionKey  /LANG=2052 ProductVersion 0.0.0.2
	VIAddVersionKey  /LANG=2052 FileDescription TLBB(Alpha)
	VIAddVersionKey  /LANG=2052 LegalCopyright (C)SOHU.COM
  
	VIProductVersion 0.0.0.2

	BrandingText "${PRODUCT_PUBLISHER}"
  
	InstallDir "D:\TLBB"
	
;--------------------------------
;接口
	!define MUI_HEADERIMAGE
	!define MUI_ABORTWARNING

;--------------------------------
;都有哪些页
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_INSTFILES

;--------------------------------
;定义卸载页
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;中文界面
	!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
Section "TLBB"
 	SetOverwrite on
	
	;拷贝文件
	SetOutPath $INSTDIR
	File /r D:\Prj\Bin_RTM\Client\*.*

	;写注册表
	WriteRegStr HKLM "SOFTWARE\SOHU\FireFox\TLBB" "InstallPath" "$INSTDIR"
	
	;创建快捷方式
	SetShellVarContext all
	CreateShortCut "$DESKTOP\天龙八部.lnk" "$INSTDIR\Launch.exe"
	CreateShortCut "$DESKTOP\天龙八部体验指南.lnk" "$INSTDIR\天龙八部体验指南.htm"

SectionEnd	
  
;--------------------------------
; 初始化
Function .onInit

FunctionEnd
