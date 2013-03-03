;--------------------------------
;ʹ���½���
	!include "MUI.nsh"

;--------------------------------
;������������
	!include "Tlbb_Common.nsh"

;--------------------------------
;��������ļ�����
	Name "${PRODUCT_NAME}Alpha ${PRODUCT_VERSION}"
	OutFile "__.exe"
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
	WriteUninstaller "D:\Prj\Bin_RTM\Client\Uninstall.exe"
	
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
