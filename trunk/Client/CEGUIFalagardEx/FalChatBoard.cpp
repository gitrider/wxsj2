
#include "FalChatBoard.h"
//#include "windows.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardChatBoard
	//
	//===================================================================================
	const utf32 FalagardChatBoard::infoBegin	= 0xE4000000;
	const utf32 FalagardChatBoard::infoEnd	= 0xE5000000;

	const utf8  FalagardChatBoard::WidgetTypeName[] = "Falagard/ChatBoard";

	// Event Namespace
	const String FalagardChatBoard::EventNamespace("Falagard/FalagardChatBoard");

	// Event Strings
	const String FalagardChatBoard::EventInfoElementLClick("InfoElementLClick");
	const String FalagardChatBoard::EventInfoElementRClick("InfoElementRClick");
	const String FalagardChatBoard::EventInfoElementDelete("InfoElementDelete");

	const String FalagardChatBoard::EventInfoElementMoveIn("InfoElementMoveIn");
	const String FalagardChatBoard::EventInfoElementMoveOut("InfoElementMoveOut");

	const String FalagardChatBoard::EventKillBoard("KillBoard");

	FalagardChatBoardProperties::KillTimer		FalagardChatBoard::d_KillTimerProperty;

	FalagardChatBoard::FalagardChatBoard(const String& type, const String& name) :
	Window(type, name),
		d_pCurrentMoveInElement(0),
		d_textHeight(0.0f),
		d_needCacultextHeight(true),
		d_boardHollow(true),
		d_killTime(0.0f),
		d_msgID(-1)
	{
		addChatBoardProperties();
	}

	FalagardChatBoard::~FalagardChatBoard()
	{
	}

	void FalagardChatBoard::addChatBoardProperties(void)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardChatBoard )
			CEGUI_ADD_STATICPROPERTY( &d_KillTimerProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	float FalagardChatBoard::resizeSelf(float maxWidth)
	{
		if(!d_needCacultextHeight) return d_textHeight;

		const FontBase* font = getFont();
		// can't do anything without a font :)
		if (font == 0)
			return 0.0f;

		// get destination area for the text.
		Rect absarea(0, 0, maxWidth, getPixelRect().getHeight());

		//d_needCacultextHeight = false;	// char board must calc everytime [5/15/2007]
		d_textHeight = font->getFormattedSize(d_text, absarea, ((TextFormatting)WordWrapLeftAligned)).d_height 
			+ (FontBase::LineSpace);

		return d_textHeight;
	}

	void FalagardChatBoard::populateRenderCache()
	{
		const FontBase* font = getFont();
		// can't render text without a font :)
		if (font == 0)
			return;

		// get destination area for the text.
		Rect absarea(0, 0, getAbsoluteSize().d_width, getAbsoluteSize().d_height);

		ColourRect final_cols(colour(1.0f, 1.0f, 1.0f));
		d_renderCache.cacheText(this, 
			d_text, font, (TextFormatting)WordWrapLeftAligned, absarea, 0, final_cols);
	}

	void FalagardChatBoard::updateSelf(float elapsed)
	{
		Window::updateSelf(elapsed);

		if(d_killTime > 0.5f)
		{
			(d_killTime-elapsed > 0.0f)?d_killTime -= elapsed:0.0f;

			if(d_killTime < 0.5f)
			{
				d_killTime = 0.0f;
				WindowEventArgs args(this);
				fireEvent(EventKillBoard, args, EventNamespace);
			}

			//char dbgmsg[128] = {0};
			//_snprintf(dbgmsg, 127, "FalagardChatBoard KillTime:%f\n", d_killTime);
			//::OutputDebugString(dbgmsg);
		}// end of if(d_killTime > 0.5f)
	}

	void FalagardChatBoard::appendInfoItem(int nID, const String32& strDisplay, int nLength, int nTalkerLength)
	{
		if(strDisplay.empty()) return;

		InfoElement il;

		il.nID = nID;
		il.nBeginIndex = (int)d_text.length()-nTalkerLength;

		d_text += strDisplay;
		il.nEndIndex = (int)d_text.length()-1-nTalkerLength;
		il.nLength = nLength;
		requestRedraw();

		d_infoLit.push_back(il);
	}

	void FalagardChatBoard::appendNormalCharacter(const utf32& code)
	{
		d_text += code;
		requestRedraw();
	}

	bool FalagardChatBoard::IsPointOnInfoItem( const Point& pt )
	{
		int idx = getTextIndexFromPosition( pt );
		if(idx < 0) return false;

		InfoElementList::iterator it = d_infoLit.begin();
		for(; it != d_infoLit.end(); ++it)
		{
			InfoElement il = *it;
			if(il.nBeginIndex <= (int)idx && il.nEndIndex >= (int)idx) break;
		}
		if(it == d_infoLit.end()) return false;

		return true;
	}

	/*************************************************************************
	Handler for mouse button pushed events
	*************************************************************************/
	void FalagardChatBoard::onMouseButtonUp(MouseEventArgs& e)
	{
		Window::onMouseButtonUp(e);

		int idx = getTextIndexFromPosition(e.position);
		if(idx < 0) 
		{
			if( !d_boardHollow )
			{
				e.handled = true;
			}
			return;
		}

		InfoElementList::iterator it = d_infoLit.begin();
		for(; it != d_infoLit.end(); ++it)
		{
			InfoElement il = *it;
			if(il.nBeginIndex <= (int)idx && il.nEndIndex >= (int)idx) break;
		}

		if(it == d_infoLit.end())
		{
			if( !d_boardHollow )
			{
				e.handled = true;
			}
			return;
		}

		InfoElement il = *it;
		if(e.button == LeftButton)
		{
			//left mouse down
			ItemElementEventArgs args(this);
			args.itemID = il.nID;
			args.button = LeftButton;
			fireEvent(EventInfoElementLClick, args, EventNamespace);
		}
		else if(e.button == RightButton)
		{
			//right mouse down
			ItemElementEventArgs args(this);
			//if(it == ++d_infoLit.begin())
			args.msgID = this->getmsgID();

			//else
			//	args.msgID = -1;
			args.itemID = il.nID;
			args.button = RightButton;
			fireEvent(EventInfoElementRClick, args, EventNamespace);
		}

		e.handled = true;
	}

	void FalagardChatBoard::onMouseMove(MouseEventArgs& e)
	{
		Window::onMouseMove(e);

		int idx = getTextIndexFromPosition(e.position);
		if(idx < 0) return;
		if(d_pCurrentMoveInElement)
		{
			//Check is in current element.
			if(d_pCurrentMoveInElement->nBeginIndex <= (int)idx && d_pCurrentMoveInElement->nEndIndex >= (int)idx) return;
		}

		InfoElementList::iterator it = d_infoLit.begin();
		for(; it != d_infoLit.end(); ++it)
		{
			InfoElement il = *it;
			if(il.nBeginIndex <= (int)idx && il.nEndIndex >= (int)idx) break;
		}

		if(it == d_infoLit.end())
		{
			if(d_pCurrentMoveInElement)
			{
				//MouseMove Out
				ItemElementEventArgs args(this);
				args.itemID = d_pCurrentMoveInElement->nID;
				fireEvent(EventInfoElementMoveOut, args, EventNamespace);
				d_pCurrentMoveInElement = 0;
			}
			return;
		}

		//Check is Same??
		if((*it).isEqual(d_pCurrentMoveInElement)) return;

		//MouseMove In
		InfoElement il = *it;
		ItemElementEventArgs args(this);
		args.itemID = il.nID;
		fireEvent(EventInfoElementMoveIn, args, EventNamespace);

		//Save for nexe check.
		d_pCurrentMoveInElement = &(*it);

		//Must Not set true to e.handled, for use for automove in tlbb.
		e.handled = false;
	}

	void FalagardChatBoard::onMouseLeaves(MouseEventArgs& e)
	{
		Window::onMouseLeaves(e);

		if(d_pCurrentMoveInElement)
		{
			//MouseMove Out
			ItemElementEventArgs args(this);
			args.itemID = d_pCurrentMoveInElement->nID;
			fireEvent(EventInfoElementMoveOut, args, EventNamespace);
			d_pCurrentMoveInElement = 0;
		}
	}

	void FalagardChatBoard::clearInfoItem()
	{
		InfoElementList::iterator it = d_infoLit.begin();
		for(; it != d_infoLit.end(); ++it)
		{
			ItemElementEventArgs args(this);
			args.itemID = (*it).nID;
			fireEvent(EventInfoElementDelete, args, EventNamespace);
		}

		d_infoLit.clear();
		d_pCurrentMoveInElement = 0;
		d_needCacultextHeight = true;
	}

	/*************************************************************************
	Insert a chat string to chat board list.
	*************************************************************************/
	void FalagardChatBoard::setChatBoardString(const String32& strTxt, int nTalkerLength)
	{
		enum c_sign
		{
			c_nameBegin,
			c_normal,
		};

		CEGUI::utf32 beginType = (infoBegin>>24)&0xFF;
		CEGUI::utf32 endType = (infoEnd>>24)&0xFF;

		//1.insert string
		String32 strDisplayName;
		int infoId, infoLength;
		c_sign	sign = c_normal;

		for(size_t c = 0; c < strTxt.length(); ++c)
		{
			CEGUI::utf32 code = strTxt[c];
			CEGUI::utf32 type = (strTxt[c]>>24)&0xFF;

			if(type == beginType)
			{
				sign = c_nameBegin;
				strDisplayName.erase();

				infoId = (int)(code & 0x00FFFFFF);
			}
			else if(type == endType)
			{
				infoLength = (int)(code & 0x00FFFFFF);
				appendInfoItem(infoId, strDisplayName, infoLength, nTalkerLength);
				sign = c_normal;
			}
			else
			{
				switch(sign) {
				case c_nameBegin:
					strDisplayName += code;
					break;
				case c_normal:
				default:
					appendNormalCharacter(code);
					break;
				}
			}
		}
	}

	int FalagardChatBoard::getTextIndexFromPosition(const Point& pt) const
	{
		if(d_text.empty() || !getFont()) return -1;
		//
		// calculate final window position to be checked
		//
		Point wndPt = screenToWindow(pt);

		if (getMetricsMode() == Relative)
		{
			wndPt = relativeToAbsolute(wndPt);
		}

		// use board max width as wrapwidth
		float maxWidth = getPixelRect().getWidth();

		return getFont()->getWrapedCharAtPoint(d_text, wndPt, maxWidth);
	}

	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

	Factory Methods

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	Window* FalagardChatBoardFactory::createWindow(const String& name)
	{
		return new FalagardChatBoard(d_type, name);
	}

	void FalagardChatBoardFactory::destroyWindow(Window* window)
	{
		delete window;
	}
}