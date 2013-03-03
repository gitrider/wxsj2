#pragma once


// CCanvasWnd
class CImageset;
class CCanvasWnd : public CStatic
{
	DECLARE_DYNAMIC(CCanvasWnd)

public:
	CCanvasWnd();
	virtual ~CCanvasWnd();

	void setImageset(CImageset*	pImageset);
	
	enum FRESH_TYPE { FT_RGB, FT_ALPHA };
	void freshType(FRESH_TYPE type);

	void setFocus(int index);
private:
	CImageset*	m_pImageset;
	FRESH_TYPE	m_freshType;
	int			m_nFocusIndex;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


