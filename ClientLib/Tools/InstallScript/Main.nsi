;-----------------------------------
;全局宏定义
;!define TLBB_SETUP_FOR_ONLINESETUP

;--------------------------------
;使用新界面
	!include "MUI.nsh"
	
;--------------------------------
;基本变量定义
	!include "Tlbb_Common.nsh"
	
;--------------------------------
;用户变量
	
;--------------------------------
;定义输出文件属性

	Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
!ifdef TLBB_SETUP_FOR_ONLINESETUP  
	OutFile "TLBB_for_Online.exe"
!else
	OutFile "v:\天龙八部\天龙八部-${PRODUCT_VERSION}\TLBB_${PRODUCT_VERSION}.exe"
!endif
;	OutFile "e:\TLBB_Alpha_${PRODUCT_VERSION}.exe"
	!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP win.bmp
	!insertmacro MUI_DEFAULT MUI_ICON Install.ico

	VIAddVersionKey  /LANG=2052 ProductName TLBB
	VIAddVersionKey  /LANG=2052 Comments SetupFile
	VIAddVersionKey  /LANG=2052 CompanyName "FireFox SOHU.COM"
	VIAddVersionKey  /LANG=2052 FileVersion 0.0.0.5
	VIAddVersionKey  /LANG=2052 ProductVersion 0.0.0.5
	VIAddVersionKey  /LANG=2052 FileDescription TLBB
	VIAddVersionKey  /LANG=2052 LegalCopyright (C)SOHU.COM
  
	VIProductVersion 0.0.0.5

	BrandingText "${PRODUCT_PUBLISHER}"
  
	InstallDir "D:\TLBB"
	
;--------------------------------
;接口
	!define MUI_HEADERIMAGE
	!define MUI_ABORTWARNING

;--------------------------------
;都有哪些页
!ifndef TLBB_SETUP_FOR_ONLINESETUP  
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_DIRECTORY
!endif
	!insertmacro MUI_PAGE_INSTFILES
	
;--------------------------------
;中文界面
	!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
Section "TLBB"
 	SetOverwrite on
	
	;拷贝文件
	SetOutPath $INSTDIR
	File /r ${PRODUCT_BUILD_DIR}\*.*

	;创建快捷方式
	SetShellVarContext all
	CreateShortCut "$DESKTOP\天龙八部.lnk" "$INSTDIR\Launch.exe"

SectionEnd	

;--------------------------------
; 初始化
Function .onInit

FunctionEnd

