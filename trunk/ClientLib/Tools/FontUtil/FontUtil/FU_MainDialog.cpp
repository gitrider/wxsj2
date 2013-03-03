#include "StdAfx.h"
#include "FU_MainDialog.h"
#include "FU_CanvasWnd.h"

//--------------------------------------------------------------------------------------------
CMainDialog::CMainDialog()
{
}

//--------------------------------------------------------------------------------------------
CMainDialog::~CMainDialog()
{
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//Center window
	CenterWindow(GetParent());

	//Init area list
	initCodeAreaList();

	// Init font data
	m_theAxFont.insertCodeArea(0x0000);
	m_theAxFont.setTrueType("ËÎÌו", 12);
	m_theAxFont.rebuild();

	//Create canvas
	HWND hCanvasParent = ::GetDlgItem(m_hWnd, IDC_STATIC_PARENT);

	RECT rect;
	::GetClientRect(hCanvasParent, &rect);

	m_canvasWindow = new CCanvasWindow;
	m_canvasWindow->Create(hCanvasParent, rect, _T("Canvas"), WS_CHILD);
	m_canvasWindow->ShowWindow(SW_SHOW);
	m_canvasWindow->setBmpFont(&m_theAxFont);
	m_canvasWindow->setCodeStart(0x0020);

	updateFontInfo();
	return (LRESULT)TRUE;
}

//--------------------------------------------------------------------------------------------
void CMainDialog::updateFontInfo(void)
{
	std::pair< const char*, int > info = m_theAxFont.getTrueType();
	char temp[MAX_PATH] = {0};
	_snprintf(temp, MAX_PATH, "%s:%d", info.first, info.second);
	SetDlgItemText(IDC_STATIC_FONT_INFO, temp);
}

//--------------------------------------------------------------------------------------------
void CMainDialog::initCodeAreaList(void)
{
	HWND hListWnd = ::GetDlgItem(m_hWnd, IDC_LIST_CODEAREA);
	if(!hListWnd) return;

	//set check box
	ListView_SetExtendedListViewStyleEx(hListWnd , 0, LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP);

	// insert column
	LVCOLUMN newColum;
	newColum.mask = LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	newColum.fmt = LVCFMT_CENTER;
	newColum.cx = 18;
	newColum.pszText = _T("");
	newColum.cchTextMax = 0;
	newColum.iSubItem = 0;
	ListView_InsertColumn(hListWnd , 0, &newColum);

	newColum.fmt = LVCFMT_LEFT;
	newColum.cx = 170;
	newColum.pszText = _T("");
	newColum.cchTextMax = 6;
	newColum.iSubItem = 1;
	ListView_InsertColumn(hListWnd, 1, &newColum);

	//init code area define
	m_theAxFont.getSustainingCodeArea(m_allCodeAreaDefine);

	//insert to list
	for(int i=0; i<(int)m_allCodeAreaDefine.size(); i++)
	{
		const CAxBitmapFont::CODEAREA_DEFINE& codeArea = *(m_allCodeAreaDefine[i]);

		LVITEM newItem; ZeroMemory(&newItem, sizeof(LVITEM));

		newItem.mask = LVIF_TEXT;
		newItem.iItem = 0;
		newItem.iSubItem = 0;
		newItem.state = 0;
		newItem.stateMask = 0;
		newItem.pszText = _T("");
		newItem.cchTextMax = 0;
		newItem.iImage = 0;
		newItem.lParam = 0;

		newItem.iItem = i;
		ListView_InsertItem(hListWnd, &newItem);
		ListView_SetItemText(hListWnd, i, 1, (LPSTR)codeArea.szName);
	}

	//check area 0
	ListView_SetCheckState(hListWnd, 0, TRUE);
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnButtonFont(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfHeight = -m_theAxFont.getTrueType().second;
	strcpy(lf.lfFaceName, m_theAxFont.getTrueType().first);

	CFontDialog dlg(&lf);
	dlg.DoModal();

	dlg.GetCurrentFont(&lf);

	m_theAxFont.setTrueType(lf.lfFaceName, -lf.lfHeight);

	updateFontInfo();
	return 0;
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPNMHDR lpnmh = (LPNMHDR)lParam;

	if(lpnmh->idFrom==IDC_LIST_CODEAREA)
	{
		if(lpnmh->code==LVN_GETINFOTIP)
		{
			//fill tooltips struct
			fillCodeAreaTooltips((LPNMLVGETINFOTIP)lParam);
		}
		else if(lpnmh->code==LVN_ITEMCHANGED)
		{
			LPNMLISTVIEW pnmv = (LPNMLISTVIEW)lParam;
			int index = pnmv->iItem;
			if(index<0 || index>=(int)m_allCodeAreaDefine.size()) return 0;
			bool checked = ListView_GetCheckState(pnmv->hdr.hwndFrom, index)==TRUE;

			if(checked) m_theAxFont.insertCodeArea(m_allCodeAreaDefine[index]->wID);
			else m_theAxFont.removeCodeArea(m_allCodeAreaDefine[index]->wID);
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
void CMainDialog::fillCodeAreaTooltips(LPNMLVGETINFOTIP pInfoTip)
{
	assert(pInfoTip);

	HWND hListWnd = ::GetDlgItem(m_hWnd, IDC_LIST_CODEAREA);
	int index = pInfoTip->iItem;

	assert(index>=0 && index<(int)m_allCodeAreaDefine.size());
	const CAxBitmapFont::CODEAREA_DEFINE* codeArea = m_allCodeAreaDefine[index];

	_sntprintf(pInfoTip->pszText, pInfoTip->cchTextMax, 
		_T("CodeArea:%s\nBegin:%04X\nEnd:%04X"), 
		codeArea->szName, codeArea->wCodeStart, codeArea->wCodeEnd);
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnFontRebuild(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_theAxFont.rebuild();
//	m_theAxFont.debug();
	m_canvasWindow->redraw();

	return 0;
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFileDialog fileDlg(TRUE, _T("BitmapFont"), 0, OFN_HIDEREADONLY, 
		_T("Bitmap font file(*.bin)\0*.bin\0All files(*.*)\0*.*\0\0"), m_hWnd);
	if(IDOK != fileDlg.DoModal()) return 0;

	m_theAxFont.openFromFile(fileDlg.m_szFileName);
	m_canvasWindow->redraw();

	HWND hListWnd = ::GetDlgItem(m_hWnd, IDC_LIST_CODEAREA);

	const CAxBitmapFont::CODE_AREA_BUF& areaBuf = m_theAxFont.getCodeArea();
	for(int i=0; i<(int)m_allCodeAreaDefine.size(); i++)
	{
		wchar_t idArea = m_allCodeAreaDefine[i]->wID;
		bool valid = areaBuf.find(idArea) != areaBuf.end();

		ListView_SetCheckState(hListWnd, i, valid?TRUE:FALSE);
	}

	updateFontInfo();

	m_theAxFont.getFontWidth(0x201c);

	return 0;
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFileDialog fileDlg(FALSE, "BitmapFont", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		"Bitmap font file(*.bin)\0*.bin\0All files(*.*)\0*.*\0\0", m_hWnd);
	if(IDOK != fileDlg.DoModal()) return 0;

	m_theAxFont.rebuild();
	m_theAxFont.saveToFile(fileDlg.m_szFileName);

	m_canvasWindow->redraw();
	return 0;
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnFontSize12(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_theAxFont.setFontSize(CAxBitmapFont::FS_12);
	return 0;
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnFontSize14(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_theAxFont.setFontSize(CAxBitmapFont::FS_14);
	return 0;
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnFontSize16(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_theAxFont.setFontSize(CAxBitmapFont::FS_16);
	return 0;
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

//--------------------------------------------------------------------------------------------
LRESULT CMainDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int cx = GET_X_LPARAM(lParam);
	int cy = GET_Y_LPARAM(lParam);
	bHandled = FALSE;

	if(cx==0 || cy==0) return 0;

	return 0;
}