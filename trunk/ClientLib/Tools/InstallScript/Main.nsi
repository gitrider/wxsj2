;-----------------------------------
;ȫ�ֺ궨��
;!define TLBB_SETUP_FOR_ONLINESETUP

;--------------------------------
;ʹ���½���
	!include "MUI.nsh"
	
;--------------------------------
;������������
	!include "Tlbb_Common.nsh"
	
;--------------------------------
;�û�����
	
;--------------------------------
;��������ļ�����

	Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
!ifdef TLBB_SETUP_FOR_ONLINESETUP  
	OutFile "TLBB_for_Online.exe"
!else
	OutFile "v:\�����˲�\�����˲�-${PRODUCT_VERSION}\TLBB_${PRODUCT_VERSION}.exe"
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
;�ӿ�
	!define MUI_HEADERIMAGE
	!define MUI_ABORTWARNING

;--------------------------------
;������Щҳ
!ifndef TLBB_SETUP_FOR_ONLINESETUP  
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_DIRECTORY
!endif
	!insertmacro MUI_PAGE_INSTFILES
	
;--------------------------------
;���Ľ���
	!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
Section "TLBB"
 	SetOverwrite on
	
	;�����ļ�
	SetOutPath $INSTDIR
	File /r ${PRODUCT_BUILD_DIR}\*.*

	;������ݷ�ʽ
	SetShellVarContext all
	CreateShortCut "$DESKTOP\�����˲�.lnk" "$INSTDIR\Launch.exe"

SectionEnd	

;--------------------------------
; ��ʼ��
Function .onInit

FunctionEnd

