;--------------------------------
;使用新界面
	!include "MUI.nsh"

;--------------------------------
;基本变量定义
	!include "Tlbb_Common.nsh"

;--------------------------------
;定义输出文件属性
	Name "《${PRODUCT_NAME}》"
	OutFile "__.exe"
	!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP win.bmp
	!insertmacro MUI_DEFAULT MUI_ICON Install.ico
	!insertmacro MUI_DEFAULT MUI_UNICON Uninstall.ico

	VIAddVersionKey  /LANG=2052 ProductName TLBB(Beta)
	VIAddVersionKey  /LANG=2052 Comments SetupFile
	VIAddVersionKey  /LANG=2052 CompanyName "FireFox SOHU.COM"
	VIAddVersionKey  /LANG=2052 FileVersion 0.0.0.3
	VIAddVersionKey  /LANG=2052 ProductVersion 0.0.0.3
	VIAddVersionKey  /LANG=2052 FileDescription TLBB(Beta)
	VIAddVersionKey  /LANG=2052 LegalCopyright (C)SOHU.COM
  
	VIProductVersion 0.0.0.3

	BrandingText "${PRODUCT_PUBLISHER}"
  
	InstallDir "D:\TLBB"
	
;--------------------------------
;接口
	!define MUI_HEADERIMAGE
	!define MUI_ABORTWARNING

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

	;创建卸载程序
	WriteUninstaller "${PRODUCT_BUILD_DIR}\Uninstall.exe"
	
SectionEnd	
  
  
;--------------------------------
; 卸载段
Section "Uninstall"
  	; Remove registry keys
  	
  	DeleteRegKey HKLM "SOFTWARE\SOHU\FireFox\TLBB"

  	; Remove files and uninstaller
  	Delete $INSTDIR\*.*
  	Delete $INSTDIR\uninstall.exe

  	; Remove shortcuts
  	SetShellVarContext all
		Delete "$DESKTOP\天龙八部.lnk"

  	; Remove directories used
  	RMDir /r $INSTDIR
SectionEnd  
  
;--------------------------------
; 初始化
Function .onInit

FunctionEnd
