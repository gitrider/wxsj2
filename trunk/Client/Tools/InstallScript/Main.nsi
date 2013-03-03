;--------------------------------
;ʹ���½���
	!include "MUI.nsh"
	
;--------------------------------
;������������
	!include "CJSH_Common.nsh"
	
;--------------------------------
;��������ļ�����

	Name "${PRODUCT_NAME}Alpha ${PRODUCT_VERSION}"
	OutFile "u:\�����˲�\�����˲�-${PRODUCT_VERSION}\TLBB_Alpha_${PRODUCT_VERSION}.exe"
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
;������Щҳ
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_INSTFILES

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
	
	;�����ļ�
	SetOutPath $INSTDIR
	File /r D:\Prj\Bin_RTM\Client\*.*

	;дע���
	WriteRegStr HKLM "SOFTWARE\SOHU\FireFox\TLBB" "InstallPath" "$INSTDIR"
	
	;������ݷ�ʽ
	SetShellVarContext all
	CreateShortCut "$DESKTOP\�����˲�.lnk" "$INSTDIR\Launch.exe"
	CreateShortCut "$DESKTOP\�����˲�����ָ��.lnk" "$INSTDIR\�����˲�����ָ��.htm"

SectionEnd	
  
;--------------------------------
; ��ʼ��
Function .onInit

FunctionEnd
