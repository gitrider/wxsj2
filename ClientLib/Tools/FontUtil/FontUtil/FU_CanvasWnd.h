#pragma once

class CAxBitmapFont;

class CCanvasWindow : public CWindowImpl< CCanvasWindow >
{
	/*************************************************************************
		Public Methods
	*************************************************************************/
public:
	/** set Bitmap font data
	*/
	void setBmpFont(CAxBitmapFont* pBmpFont);
	/** set character start to draw
	*/
	void setCodeStart(wchar_t codeStart);
	/** redraw canvas
	*/
	void redraw(void);

	/*************************************************************************
			Inherit Methods
	*************************************************************************/
public:
	DECLARE_WND_CLASS(NULL)

	virtual void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	}

	BEGIN_MSG_MAP(CHelloView)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	/** On WM_SIZE
	*/
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/** On WM_PAINT
	*/
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
private:
	/** recreate background canvas
	*/
	void _createCanvas(int width, int height);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
private:
	CBitmap			m_bmpCanvas;	//!< Bmp canvas
	SIZE			m_size;			//!< Canvas size
	CAxBitmapFont*	m_pBmpFont;		//!< AXIA|Bitmap font data
	wchar_t			m_codeStart;	//!< Character start
	unsigned short*	m_pCanvasBitbuf;//!< Canvas bit buf
	int				m_nCanvasPitch;	//!< Cavas bit buf pitch(usnigned short)

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
public:
	CCanvasWindow();
	~CCanvasWindow();
};