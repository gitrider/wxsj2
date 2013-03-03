#pragma once


class CMainWnd;
// CLegendWnd
class CLegendWnd : public CStatic
{
	DECLARE_DYNAMIC(CLegendWnd)

public:
	//The state of a legend
	enum LegendState
	{
		LS_DIE,				//gray
		LS_LIVE_0,			//live state0  (green)
		LS_LIVE_1,			//live state1  (blue)
		LS_FAULT,			//fault (red)
	};

	//Select state of a legend
	enum SelectState
	{
		SS_NORMAL,
		SS_HORVER,
		SS_SELECT,
	};

	// Allocate a legend to control, return the id.
	int		AllocaLegend(void);

	// set the legend state
	void	SetLegendState(int id, LegendState state);

	// need redraw
	void	InvalidateLegend(void);

public:
	CLegendWnd(CMainWnd* pMainWnd);
	virtual ~CLegendWnd();
	/*!
	\brief
		Return pointer to singleton HelperSystem object

	\return
		Pointer to singleton HelperSystem object
	*/
	static CLegendWnd* GetMe(void) { return s_pMe; }

protected:
	//Legend struct
	struct Legend
	{
		INT			 id;
		LegendState	 state;
		SelectState	 sel_state;
	};
	typedef std::map< INT, Legend > LegendMap;

	CMainWnd*		m_pMainWnd;			//!< Point to the main wnd;
	LegendMap		m_legendMap;		//!< All Legend
	CBitmap			m_bmpLegend;		//!< Bitmap resource to draw legend
	int				m_idHover;			//!< id of mouse hover legend;
	int				m_idSelected;		//!< id of select legend;

private:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    /*!
    \brief
        Implementation method to generate a unique id to use for a legend.
    */
    int generateUniqueId(void);

	static CLegendWnd* s_pMe;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


