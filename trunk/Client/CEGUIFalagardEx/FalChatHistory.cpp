//#include "windows.h"
#include "FalChatHistory.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "CEGUIPropertyHelper.h"
#include "elements/CEGUIPushButton.h"
#include "elements/CEGUIScrollbar.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "FalChatBoard.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardChatHistory/ParentWindow
	//
	//===================================================================================
	const utf8 FalagardChatHistory::ParentWindow::WidgetTypeName[] = "Falagard/ChatHistory/ParentWindow";
	FalagardChatHistory::ParentWindow::ParentWindow(const String& type, const String& name) :
	Window(type, name)
	{
	}

	FalagardChatHistory::ParentWindow::~ParentWindow()
	{
	}

	Window* FalagardChatHistory::ParentWindowFactory::createWindow(const String& name)
	{
		return new FalagardChatHistory::ParentWindow(d_type, name);
	}

	void FalagardChatHistory::ParentWindowFactory::destroyWindow(Window* window)
	{
		delete window;
	}	
	//===================================================================================
	//
	// FalagardChatHistory
	//
	//===================================================================================
	const utf8  FalagardChatHistory::WidgetTypeName[] = "Falagard/ChatHistory";

	// Event Namespace
	const String FalagardChatHistory::EventNamespace("Falagard/ChatHistory");

	// Event Strings
	const String FalagardChatHistory::EventInfoElementClick("InfoElementClick");
	const String FalagardChatHistory::EventInfoElementDelete("InfoElementDelete");

	const String FalagardChatHistory::EventInfoElementMoveIn("InfoElementMoveIn");
	const String FalagardChatHistory::EventInfoElementMoveOut("InfoElementMoveOut");

	FalagardChatHistoryProperties::ChatBoardNumber		FalagardChatHistory::d_chatBoardNumProperty;
	FalagardChatHistoryProperties::BoardKillTimer		FalagardChatHistory::d_boardKillTimerProperty;
	FalagardChatHistoryProperties::ChatMouseHollow		FalagardChatHistory::d_ChatMouseHollowProperty;

	FalagardChatHistory::FalagardChatHistory(const String& type, const String& name) :
	IExpProperty(type, name), 
		d_parentWindow(0),
		d_vertScrollbar(0),
		d_totalHeight(0),
		d_chatBoardNum(3),
		d_boardKillTimer(0),
		d_first(true),
		d_hollow( true ),
		d_extendHeight(0)
	{
		addChatHistoryProperties();
	}

	FalagardChatHistory::~FalagardChatHistory()
	{
	}

	void FalagardChatHistory::addChatHistoryProperties(void)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardChatHistory )
			CEGUI_ADD_STATICPROPERTY( &d_chatBoardNumProperty );
			CEGUI_ADD_STATICPROPERTY( &d_boardKillTimerProperty );
			CEGUI_ADD_STATICPROPERTY( &d_ChatMouseHollowProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	void FalagardChatHistory::initialise(void)
	{
		// create the component sub-widgets
		d_parentWindow = static_cast<ParentWindow*>(WindowManager::getSingleton().getWindow(getName() + "__auto_parent__"));
		d_vertScrollbar = static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(getName() + "__auto_vscrollbar__"));
		d_toEnd = static_cast<PushButton*>(WindowManager::getSingleton().getWindow(getName() + "__auto_toend_btn__"));
		// alawys show.
		d_vertScrollbar->show();
		d_vertScrollbar->useSpecialThumb(true);
		
		d_parentWindow->setChatHistoryWindow(this);
		d_parentWindow->setMouseLButtonHollow(true);
		d_parentWindow->setMouseRButtonHollow(true);

		addChildWindow(d_vertScrollbar);
		addChildWindow(d_toEnd);

		//Resize child window(VerscrollBar and ParentWindow)
		performChildWindowLayout();
		//Resize all child elements.
		performWindowLayout();

		// event subscription
		d_vertScrollbar->setScrollPosition(0);
		d_vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&FalagardChatHistory::handleScrollbarChange, this));
		d_toEnd->subscribeEvent(PushButton::EventMouseButtonDown, Event::Subscriber(&FalagardChatHistory::handleToEndBtnClick, this));
	}

	void FalagardChatHistory::setChatMouseHollow(const bool bchatMouseHollow)
	{
		d_hollow = bchatMouseHollow;
		if ( !d_hollow )
		{
			d_parentWindow->setMouseLButtonHollow(false);
			d_parentWindow->setMouseRButtonHollow(false);
		}
	}

	Rect FalagardChatHistory::getTextRenderArea(void) const
	{
		return d_parentWindow->getPixelRect();
	}

	void FalagardChatHistory::onSized(WindowEventArgs& e)
	{
		Window::onSized(e);

		performWindowLayout();
		requestRedraw();
	}

	void FalagardChatHistory::subscribInfoItemClickEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventInfoElementClick, subscriber);
	}

	void FalagardChatHistory::subscribInfoItemDeleteEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventInfoElementDelete, subscriber);
	}

	void FalagardChatHistory::subscribInfoItemMoveInEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventInfoElementMoveIn, subscriber);
	}

	void FalagardChatHistory::subscribInfoItemMoveOutEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventInfoElementMoveOut, subscriber);
	}
	/*************************************************************************
	Insert a chat string to chat board list.
	*************************************************************************/
	void FalagardChatHistory::setChatBoardString(FalagardChatBoard* pBoard,const String32& strTxt, int nTalkerLength)
	{
		if(!pBoard) return;
		pBoard->setChatBoardString(strTxt, nTalkerLength);
	}

	Window* FalagardChatHistory::insertChatString(int msgid,const String32& strChat, int nTalkerLength, int nOp/* = -1*/)
	{
		//2 refresh only. Change for respond speed.[7/13/2006]
		if(insert_refresh_only == nOp)
		{
			performWindowLayout();
			return 0;
		}
		float topHeight = 0;
		FalagardChatBoard* pWindow = 0;
		if(m_listChatBoard.size() == d_chatBoardNum && m_listChatBoard.size() > 0)
		{
			/* Change for respond speed.[7/13/2006]
			FalagardChatBoard* pChild = static_cast<FalagardChatBoard*>(d_parentWindow->getChild((uint)0));
			pChild->clearInfoItem();

			d_parentWindow->removeChildWindow((uint)0);
			*/
			pWindow = *(m_listChatBoard.begin());
			if(pWindow->isVisible() == false || pWindow->getText().empty())
				topHeight = 0.0f;
			else
				topHeight = pWindow->resizeSelf(getTextRenderArea().getWidth());
			pWindow->clearInfoItem();
			pWindow->clearKillTime();
			pWindow->setText("");
			pWindow->show();
			if ( !d_hollow )
			{
				pWindow->setBoardHollow( d_hollow );
			}
			m_listChatBoard.erase(m_listChatBoard.begin());
		}
		else
		{
			pWindow = static_cast<FalagardChatBoard*>(WindowManager::getSingleton().createWindow( m_mapChildType[ "ChatBoard" ] ) );
			//pWindow->setText(strChat);
			pWindow->show();
			//pWindow->setMouseLButtonHollow(true);
			pWindow->subscribeEvent(FalagardChatBoard::EventInfoElementLClick, Event::Subscriber(&FalagardChatHistory::handleInfoItemBtnClick, this));
			pWindow->subscribeEvent(FalagardChatBoard::EventInfoElementRClick, Event::Subscriber(&FalagardChatHistory::handleInfoItemBtnClick, this));
			pWindow->subscribeEvent(FalagardChatBoard::EventInfoElementDelete, Event::Subscriber(&FalagardChatHistory::handleInfoItemDelete, this));

			pWindow->subscribeEvent(FalagardChatBoard::EventInfoElementMoveIn, Event::Subscriber(&FalagardChatHistory::handleInfoItemMoveIn, this));
			pWindow->subscribeEvent(FalagardChatBoard::EventInfoElementMoveOut, Event::Subscriber(&FalagardChatHistory::handleInfoItemMoveOut, this));
			pWindow->subscribeEvent(FalagardChatBoard::EventKillBoard, Event::Subscriber(&FalagardChatHistory::handleBoardKillTime, this));
			if ( !d_hollow )
			{
				pWindow->setBoardHollow( d_hollow );
			}
			d_parentWindow->addChildWindow(pWindow);
		}

		setChatBoardString(pWindow, strChat, nTalkerLength);
		pWindow->setmsgID(msgid);
		m_listChatBoard.push_back(pWindow);

		//InsertOperation switch
		switch(nOp)
		{
		case insert_kill_add_only:
			pWindow->setKillTime(float(d_boardKillTimer));
		case insert_add_only:
			return pWindow;
		case insert_kill_normal:
			pWindow->setKillTime(float(d_boardKillTimer));
		case insert_refresh_only:
		case insert_normal:
		default:
			break;
		}


		float posEnd = d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize();
		bool bPosEnd = (posEnd - d_vertScrollbar->getScrollPosition() < (1e-08f)) && posEnd>0?true:false;

		float fWinHeight = pWindow->resizeSelf(getTextRenderArea().getWidth());
		//use d_extendHeight first.
		if(d_extendHeight > 0.0f)
		{
			d_extendHeight = (d_extendHeight - fWinHeight > 0.0f)?(d_extendHeight - fWinHeight):0.0f;
		}

		performWindowLayout();

		if(bPosEnd || (d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize()>0 && d_first))
		{
			//scroll to end
			d_vertScrollbar->setScrollPosition(
				d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize());
			d_first = false;
		}

		if(!bPosEnd /*&& m_listChatBoard.size() == d_chatBoardNum*/ )
		{
			Rect absarea(getTextRenderArea());
			if(d_vertScrollbar->getScrollPosition() - topHeight > (1e-08f))
			{
				//scroll up a new chatboard window's height
				d_vertScrollbar->setScrollPosition(
					d_vertScrollbar->getScrollPosition() - topHeight);
			}
			else
			{
				d_vertScrollbar->setScrollPosition(
					d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize());
			}
		}

		return pWindow;
	}

	/*************************************************************************
	Remove all chat boards.
	*************************************************************************/
	void FalagardChatHistory::removeAllChatStrings()
	{
		/* Change for respond speed.[7/13/2006]
		uint count = d_parentWindow->getChildCount();
		for(uint i = 0; i < count; ++i)
		{
		FalagardChatBoard* pWindow = static_cast<FalagardChatBoard*>(d_parentWindow->getChild((uint)0));
		pWindow->clearInfoItem();
		d_parentWindow->removeChildWindow((uint)0);
		}

		m_listChatBoard.clear();
		*/
		std::list< FalagardChatBoard* >::iterator it;

		for(it=m_listChatBoard.begin(); it!=m_listChatBoard.end(); it++)
		{
			FalagardChatBoard& child = *(*it);
			if(child.getText().empty()) continue;

			child.clearInfoItem();
			child.clearKillTime();
			child.setText("");
			child.hide();
		}

		d_extendHeight = 0.0f;

		performWindowLayout();

		d_first = false;
	}

	/*************************************************************************
	Extend a Clear Region in history window.
	*************************************************************************/
	void FalagardChatHistory::extendClearRegion()
	{
		if(m_listChatBoard.empty())
		{
			d_extendHeight = 0.0f;
			return;
		}

		//scroll to end before extend
		scrollToEnd();

		Rect absarea(getTextRenderArea());
		d_extendHeight = absarea.getHeight();	

		performWindowLayout();
		//set new scroll pos after extend
		d_vertScrollbar->setScrollPosition(
			d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize()+d_extendHeight);
	}

	/*************************************************************************
	Scroll to End.
	*************************************************************************/
	void FalagardChatHistory::scrollToEnd()
	{
		d_extendHeight = 0.0f;
		performWindowLayout();
		//scroll to end
		d_vertScrollbar->setScrollPosition(
			d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize());
	}

	/*************************************************************************
	Recaculate all sub window size.
	*************************************************************************/
	void FalagardChatHistory::performWindowLayout(void)
	{
		Rect absarea(getTextRenderArea());

		std::list< FalagardChatBoard* >::iterator it;
		float fHeightNow = 0.0f;

		for(it=m_listChatBoard.begin(); it!=m_listChatBoard.end(); it++)
		{
			FalagardChatBoard& child = *(*it);
			if(child.getText().empty()) continue;

			float childHeight = child.resizeSelf(absarea.getWidth());

			child.setSize(Absolute, Size(absarea.getWidth(), childHeight));
			child.setPosition(Absolute, Point(0.0f, fHeightNow-d_vertScrollbar->getScrollPosition()));
			child.requestRedraw();

			fHeightNow += childHeight;

			//char dbgmsg[128] = {0};
			//_snprintf(dbgmsg, 127, "FalagardChatHistory performWindowLayout fHeightNow:%f ScrollPos:%f, child YPos:%f\n", 
			//	fHeightNow,d_vertScrollbar->getScrollPosition(),child.getAbsoluteYPosition());
			//::OutputDebugString(dbgmsg);
		}

		d_totalHeight = fHeightNow + d_extendHeight;

		configureScrollbars();
	}

	void FalagardChatHistory::configureScrollbars()
	{
		// no scrollbars?  Can't configure then!
		if(!d_vertScrollbar) return;

		Rect initialArea(getTextRenderArea());

		//
		// Set up scroll bar values
		//
		Rect renderArea(getTextRenderArea());

		d_vertScrollbar->setDocumentSize(d_totalHeight);
		d_vertScrollbar->setPageSize(renderArea.getHeight());
		d_vertScrollbar->setStepSize(ceguimax(1.0f, renderArea.getHeight() / 10.0f));
		d_vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition());

		requestRedraw();
		d_parentWindow->requestRedraw();
	}

	/*************************************************************************
	Handler called when the scroll bar positions change
	*************************************************************************/
	bool FalagardChatHistory::handleScrollbarChange(const EventArgs& e)
	{
		performWindowLayout();

		requestRedraw();
		d_parentWindow->requestRedraw();
		return true;
	}

	/*************************************************************************
	Handler called when when the toEnd button clicked
	*************************************************************************/
	bool FalagardChatHistory::handleToEndBtnClick(const EventArgs& e)
	{
		scrollToEnd();
		return true;
	}

	bool FalagardChatHistory::handleInfoItemBtnClick(const EventArgs& e)
	{
		ItemElementEventArgs args(this);
		const ItemElementEventArgs e1 = (const ItemElementEventArgs&)e;
		args.msgID = e1.msgID;
		args.itemID = e1.itemID;
		args.button = e1.button;

		fireEvent(EventInfoElementClick, args, EventNamespace);
		return true;
	}

	bool FalagardChatHistory::handleInfoItemDelete(const EventArgs& e)
	{
		ItemElementEventArgs args(this);
		const ItemElementEventArgs e1 = (const ItemElementEventArgs&)e;

		args.itemID = e1.itemID;
		args.button = e1.button;

		fireEvent(EventInfoElementDelete, args, EventNamespace);
		return true;
	}

	bool FalagardChatHistory::handleInfoItemMoveIn(const EventArgs& e)
	{
		ItemElementEventArgs args(this);
		const ItemElementEventArgs e1 = (const ItemElementEventArgs&)e;

		args.itemID = e1.itemID;

		fireEvent(EventInfoElementMoveIn, args, EventNamespace);
		return true;
	}

	bool FalagardChatHistory::handleInfoItemMoveOut(const EventArgs& e)
	{
		ItemElementEventArgs args(this);
		const ItemElementEventArgs e1 = (const ItemElementEventArgs&)e;

		args.itemID = e1.itemID;

		fireEvent(EventInfoElementMoveOut, args, EventNamespace);
		return true;
	}

	bool FalagardChatHistory::handleBoardKillTime(const EventArgs& e)
	{
		const WindowEventArgs we = (const WindowEventArgs&)e;
		//0.must be a valid board child of d_parentWindow
		Window* pWin = we.window;
		if(!pWin) return false;
		if(!d_parentWindow->isChild(pWin)) return false;

		//1.remove from m_listChatBoard
		std::list< FalagardChatBoard* >::iterator it;
		for(it=m_listChatBoard.begin();it!=m_listChatBoard.end();it++)
		{
			if(*it == pWin) 
			{
				m_listChatBoard.erase(it);
				break;
			}
		}

		//2 remove pWin from d_parentWindow
		if(pWin->testClassName((CEGUI::utf8*)"FalagardChatBoard"))
		{
			FalagardChatBoard* pBoard = (FalagardChatBoard*)pWin;
			pBoard->clearInfoItem();
			pBoard->clearKillTime();
			d_vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition()-pWin->getHeight(Absolute));
		}
		d_parentWindow->removeChildWindow(pWin);
		performWindowLayout();

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

	Factory Methods

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	Window* FalagardChatHistoryFactory::createWindow(const String& name)
	{
		return new FalagardChatHistory(d_type, name);
	}

	void FalagardChatHistoryFactory::destroyWindow(Window* window)
	{
		delete window;
	}
}