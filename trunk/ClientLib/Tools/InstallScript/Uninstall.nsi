;--------------------------------
;ʹ���½���
	!include "MUI.nsh"

;--------------------------------
;������������
	!include "Tlbb_Common.nsh"

;--------------------------------
;��������ļ�����
	Name "��${PRODUCT_NAME}��"
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
;�ӿ�
	!define MUI_HEADERIMAGE
	!define MUI_ABORTWARNING

;--------------------------------
;����ж��ҳ
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;���Ľ���
	!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
Section "TLBB"
 	SetOverwrite on

	;����ж�س���
	WriteUninstaller "${PRODUCT_BUILD_DIR}\Uninstall.exe"
	
SectionEnd	
  
  
;--------------------------------
; ж�ض�
Section "Uninstall"
  	; Remove registry keys
  	
  	DeleteRegKey HKLM "SOFTWARE\SOHU\FireFox\TLBB"

  	; Remove files and uninstaller
  	Delete $INSTDIR\*.*
  	Delete $INSTDIR\uninstall.exe

  	; Remove shortcuts
  	SetShellVarContext all
		Delete "$DESKTOP\�����˲�.lnk"

  	; Remove directories used
  	RMDir /r $INSTDIR
SectionEnd  
  
;--------------------------------
; ��ʼ��
Function .onInit

FunctionEnd
