#pragma once
#include "Resource.h"
#include "AxBmpFont.h"

//pre-define class
class CCanvasWindow;

/** Main Function Dialog
*/
class CMainDialog : public CDialogImpl< CMainDialog >
{
	/*************************************************************************
			Public Methods
	*************************************************************************/
public:

	/*************************************************************************
			Inherit Methods
	*************************************************************************/
public:
	enum { IDD = IDD_DIALOG_MAIN };

	BEGIN_MSG_MAP(CMainDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(ID_FILE_OPENFILE, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_SAVEFILE, OnFileSave)
		COMMAND_ID_HANDLER(ID_FONT_REBUILD, OnFontRebuild)
		COMMAND_ID_HANDLER(ID_FONT_SIZE_12, OnFontSize12)
		COMMAND_ID_HANDLER(ID_FONT_SIZE_14, OnFontSize14)
		COMMAND_ID_HANDLER(ID_FONT_SIZE_16, OnFontSize16)
		COMMAND_ID_HANDLER(IDC_BUTTON_FONT, OnButtonFont)
	END_MSG_MAP()

	/** Init Dialog
	*/
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/** On Close
	*/
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** On Command ID_FONT_REBUILD
	*/
	LRESULT OnFontRebuild(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** On Command ID_FILE_OPEN
	*/
	LRESULT OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** On Command ID_FILE_SAVE
	*/
	LRESULT OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** On Command IDC_BUTTON_FONT
	*/
	LRESULT OnButtonFont(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** On Command ID_FONT_SIZE_12
	*/
	LRESULT OnFontSize12(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** On Command ID_FONT_SIZE_14
	*/
	LRESULT OnFontSize14(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** On Command ID_FONT_SIZE_16
	*/
	LRESULT OnFontSize16(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/** On WM_SIZE
	*/
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/** On WM_NOTIFY
	*/
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/*************************************************************************
			Implementation Methods
	*************************************************************************/
private:
	/**Init list-view to display code area list
	*/
	void initCodeAreaList(void);
	/**Fill tooltips for code area list
	*/
	void fillCodeAreaTooltips(LPNMLVGETINFOTIP lpGetInfoTip);
	/** Update font info
	*/
	void updateFontInfo(void);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
private:
	CAxBitmapFont						m_theAxFont;			//!< AXIA|Bitmap font
	CAxBitmapFont::CODEAREA_DEFINE_BUF	m_allCodeAreaDefine;	//!< All sustaining code area
	CCanvasWindow*						m_canvasWindow;			//!< Canvas window 

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
public:
	CMainDialog();
	~CMainDialog();
};
