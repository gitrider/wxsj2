
#include "FalagardComplexWindowProperties.h"
#include "FalagardComplexWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "FalButton.h"
#include "FalagardRewardItem.h"
#include "FalMoney.h"
#include "FalActionButton.h"
#include "FalChatBoard.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	const utf8  ComplexTypeName_Option[]		= "Option";
	const utf8  ComplexTypeName_Image[]			= "Image";
	const utf8  ComplexTypeName_RewardItem[]	= "RewardItem";
	const utf8  ComplexTypeName_ActionButton[]	= "ActionButton";
	const utf8  ComplexTypeName_Button[]		= "Button";
	const utf8  ComplexTypeName_Money[]			= "Money";
	const utf8  ComplexTypeName_ChatBoard[]     = "ChatBoard";
	//===================================================================================
	//
	// FalagardComplexWindow/ParentWindow
	//
	//===================================================================================
	const utf8 FalagardComplexWindow::ParentWindow::WidgetTypeName[] = "Falagard/ComplexWindow/ParentWindow";

	FalagardComplexWindowProperties::ActionButtonSize		FalagardComplexWindow::d_ActionButtonSizeProperty;
	FalagardComplexWindowProperties::ActionButtonDistance	FalagardComplexWindow::d_ActionButtonDistanceProperty;
	FalagardComplexWindowProperties::QuestButtonImage		FalagardComplexWindow::d_QuestButtonImageProperty;
	FalagardComplexWindowProperties::OptionCooldownTime 	FalagardComplexWindow::d_OptionCooldownTimeProperty;


	Window* FalagardComplexWindow::ParentWindowFactory::createWindow(const String& name)
	{
		return new FalagardComplexWindow::ParentWindow(d_type, name);
	}

	void FalagardComplexWindow::ParentWindowFactory::destroyWindow(Window* window)
	{
		delete window;
	}

	FalagardComplexWindow::ParentWindow::ParentWindow(const String& type, const String& name) :
	Window(type, name), 
		d_ComplexWindow(0)
	{

	}

	FalagardComplexWindow::ParentWindow::~ParentWindow()
	{
	}

	/*************************************************************************
	redraw self and all child window
	*************************************************************************/
	void FalagardComplexWindow::ParentWindow::requestAllRedraw(void)
	{
		requestRedraw();
		for(int i=0; i<(int)d_ComplexWindow->d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ComplexWindow->d_ChildVector[i];
			switch(pElement->d_Type)
			{
			case CT_Money:
				((ChildElement_Money*)pElement)->d_Money->requestRedraw();
				break;
			}
		}
	}

	/*************************************************************************
	Handler for mouse wheel changes
	*************************************************************************/
	void FalagardComplexWindow::ParentWindow::onMouseWheel(MouseEventArgs& e)
	{
		// base class processing.
		Window::onMouseWheel(e);
		if(!d_ComplexWindow) return;

		Scrollbar* vertScrollbar = d_ComplexWindow->getVerteScrollbar();

		if ( vertScrollbar && vertScrollbar->isVisible() && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
		{
			vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.wheelChange);
		}

		e.handled = true;
	}

	/*************************************************************************
	Perform the actual rendering for this Window.
	*************************************************************************/
	void FalagardComplexWindow::ParentWindow::populateRenderCache()
	{
		// get destination area for the text.
		Rect absarea(getPixelRect());
		Rect clipper(absarea);


		//	    ColourRect final_cols(colour(255.0f, 255.0f, 255.0f));

		ColourRect final_cols(colour(1.0f, 1.0f, 1.0f));


		for(int i=0; i<(int)d_ComplexWindow->d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ComplexWindow->d_ChildVector[i];
			const FontBase* font = pElement->d_Font;

			switch(pElement->d_Type)
			{
			case CT_Text:
				{
					ChildElement_Text* pTextElement = (ChildElement_Text*)pElement;
					Rect textArea(pElement->d_rectClient);
					textArea.d_top -= d_ComplexWindow->d_vertScrollbar->getScrollPosition();

					//					AxTrace(0, 1, "ComplexWindowDraw:%f, %f", textArea.getWidth(), textArea.getHeight());

					d_renderCache.cacheText(this, 
						pTextElement->d_Text, font, (TextFormatting)pTextElement->d_Typeset, textArea, 0.0f, final_cols);
				}
				break;

			case CT_Option:
				{
				}
				break;

			case CT_Item:
				{
				}
				break;
			}
		}
	}
	//===================================================================================
	//
	// FalagardComplexWindow
	//
	//===================================================================================
	const utf8 FalagardComplexWindow::WidgetTypeName[] = "Falagard/ComplexWindow";

	/*************************************************************************
	Event name constants
	*************************************************************************/
	const String FalagardComplexWindow::EventNamespace("FalagardComplexWindow");
	const String FalagardComplexWindow::EventOptionClicked( (utf8*)"OptionClicked" );
	const String FalagardComplexWindow::EventLinkClicked( (utf8*)"LinkClicked" );
	const String FalagardComplexWindow::EventRewardItemClicked( (utf8*)"RewardItemClicked" );
	const String FalagardComplexWindow::EventActionButtonClicked( (utf8*)"ActionButtonClicked" );

	// Event Strings
	const String FalagardComplexWindow::EventInfoElementClick("InfoElementClick");
	const String FalagardComplexWindow::EventInfoElementDelete("InfoElementDelete");


	FalagardComplexWindow::FalagardComplexWindow(const String& type, const String& name) :
	IExpProperty(type, name), 
		d_ParentWindow(0),
		d_vertScrollbar(0),
		d_enableVertScrollbar(false),
		d_totalHeight(0.0f),
		d_optionButtonCooldownTime(1.0f)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardComplexWindow )
			CEGUI_ADD_STATICPROPERTY( &d_ActionButtonSizeProperty  );
		CEGUI_ADD_STATICPROPERTY( &d_ActionButtonDistanceProperty  );
		CEGUI_ADD_STATICPROPERTY( &d_QuestButtonImageProperty  );
		CEGUI_ADD_STATICPROPERTY( &d_OptionCooldownTimeProperty  );
		CEGUI_END_ADD_STATICPROPERTY

			d_ActionButtonSize = Size( 32,32 );
		d_ActionButtonDistance = Size( 5,5 );
	}

	FalagardComplexWindow::~FalagardComplexWindow()
	{
		_destroyAllElements();
	}

	void FalagardComplexWindow::initialise(void)
	{
		// create the component sub-widgets
		d_ParentWindow = static_cast<ParentWindow*>(WindowManager::getSingleton().getWindow(getName() + "__auto_parent__"));
		d_vertScrollbar = static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(getName() + "__auto_vscrollbar__"));

		d_ParentWindow->SetComplexWindow(this);
		d_vertScrollbar->hide();

		addChildWindow(d_vertScrollbar);

		performWindowLayout();
		performChildWindowLayout();

		// event subscription
		d_vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&FalagardComplexWindow::handleScrollbarChange, this));
	}

	/*************************************************************************
	Add edit box specific events
	*************************************************************************/
	/*	void FalagardComplexWindow::addComplexWindowEvents(void)
	{
	addEvent(EventOptionClicked);
	addEvent(EventLinkClicked);
	addEvent(EventRewardItemClicked);
	}
	*/
	/*************************************************************************
	Handler called when the scroll bar positions change
	*************************************************************************/
	bool FalagardComplexWindow::handleScrollbarChange(const EventArgs& e)
	{
		performWindowLayout();
		requestRedraw();
		d_ParentWindow->requestAllRedraw();
		return true;
	}
	/*************************************************************************
	Handler function for when option button clicked.
	*************************************************************************/
	bool FalagardComplexWindow::handleOptionClicked(const EventArgs& e)
	{
		Window* eventWindow = ((const WindowEventArgs&) e).window;

		//serach child data
		ChildVector::iterator it,end=d_ChildVector.end();
		for(it=d_ChildVector.begin(); it!=end; ++it)
		{
			ChildElement* pChild = *it;
			if(pChild->d_Type != CT_Option) continue;

			ChildElement_Option* pOptionChild = (ChildElement_Option*)pChild;
			if(pOptionChild->d_Button != eventWindow) continue;

			//check cooldown time
			float timeCurrent = System::getSingleton().getCurTimeElapsed();
			if(pOptionChild->d_timeLastClick != 0.f)
			{
				if( timeCurrent - pOptionChild->d_timeLastClick < getOptionbuttonCooldownTime() )
				{
					// still in cooldown...
					return true;
				}
			}

			pOptionChild->d_timeLastClick = timeCurrent;
			WindowEventArgs arg( ((const WindowEventArgs&) e).window);
			fireEvent(EventOptionClicked, arg, EventNamespace);

			break;
		}

		return true;
	}


	/*************************************************************************
	Handler function for when Link button clicked.
	*************************************************************************/
	bool FalagardComplexWindow::handleLinkClicked(const EventArgs& e)
	{
		WindowEventArgs arg( ((const WindowEventArgs&) e).window);
		fireEvent(EventLinkClicked, arg, EventNamespace);

		return true;
	}

	/*************************************************************************
	Handler function for when reward item icon clicked.
	*************************************************************************/
	bool FalagardComplexWindow::handleRewardItemClicked(const EventArgs& e)
	{
		//----------------------------------------------
		//Process radio group
		FalagardRewardItem* pRewardItem =  static_cast<FalagardRewardItem*>(((const WindowEventArgs&) e).window);
		if(!pRewardItem) return true;

		ChildElement_Item* pItemElement = (ChildElement_Item*)pRewardItem->getHelper();
		if(!pItemElement) return true;

		if(!(pItemElement->d_Radio)) return true;

		for(int i=0; i<(int)d_RadioChild.size(); i++)
		{
			if(d_RadioChild[i] == pItemElement) 
				d_RadioChild[i]->d_Button->setChecked(true);
			else
				d_RadioChild[i]->d_Button->setChecked(false);
			d_RadioChild[i]->d_Button->requestRedraw();
		}

		//----------------------------------------------
		//FireEvent
		WindowEventArgs arg( ((const WindowEventArgs&) e).window);
		fireEvent(EventRewardItemClicked, arg, EventNamespace);

		return true;
	}

	/*************************************************************************
	Handler function for when Action Button clicked.
	*************************************************************************/
	bool FalagardComplexWindow::handleActionButtonClicked(const EventArgs& e)
	{
		//----------------------------------------------
		//Process radio group
		FalagardActionButton* pActionButton =  static_cast<FalagardActionButton*>(((const WindowEventArgs&) e).window);
		if(!pActionButton) return true;

		ChildElement_Action* pActionElement = (ChildElement_Action*)pActionButton->getHelper();
		if(!pActionElement) return true;

		if(!(pActionElement->d_Radio)) return true;

		for(int i=0; i<(int)d_RadioChild_Action.size(); i++)
		{
			if(d_RadioChild_Action[i] == pActionElement) 
				d_RadioChild_Action[i]->d_ActionButton->setChecked(true);
			else
				d_RadioChild_Action[i]->d_ActionButton->setChecked(false);
			d_RadioChild_Action[i]->d_ActionButton->requestRedraw();
		}

		//----------------------------------------------
		//FireEvent
		WindowEventArgs arg( ((const WindowEventArgs&) e).window);
		fireEvent(EventActionButtonClicked, arg, EventNamespace);

		return true;
	}

	/*************************************************************************
	handler function for when ChatBoard InfoElement clicked.
	*************************************************************************/
	bool FalagardComplexWindow::handleChatBoardInfoElementClicked(const EventArgs& e)
	{
		ItemElementEventArgs args(this);
		const ItemElementEventArgs e1 = (const ItemElementEventArgs&)e;
		args.msgID = e1.msgID;
		args.itemID = e1.itemID;
		args.button = e1.button;

		fireEvent(EventInfoElementClick, args, EventNamespace);
		return true;
	}

	/*************************************************************************
	handler function for when ChatBoard InfoElement delete.
	*************************************************************************/
	bool FalagardComplexWindow::handleChatBoardInfoElementDelete(const EventArgs& e)
	{
		ItemElementEventArgs args(this);
		const ItemElementEventArgs e1 = (const ItemElementEventArgs&)e;

		args.itemID = e1.itemID;
		args.button = e1.button;

		fireEvent(EventInfoElementDelete, args, EventNamespace);
		return true;
	}
	/*************************************************************************
	Handler called when size is changed
	*************************************************************************/
	void FalagardComplexWindow::onSized(WindowEventArgs& e)
	{
		Window::onSized(e);

		performWindowLayout();
		requestRedraw();
	}

	Rect FalagardComplexWindow::getTextRenderArea(void) const
	{
		return d_ParentWindow->getAbsoluteRect();   //->getPixelRect();
	}
	void FalagardComplexWindow::setActionButtonSize( const Size& rect )
	{
		d_ActionButtonSize.d_width = rect.d_width ;
		d_ActionButtonSize.d_height = rect.d_height;
	}

	const Size FalagardComplexWindow::getActionButtonSize() const 
	{
		return d_ActionButtonSize;
	}
	void FalagardComplexWindow::setActionButtonDistance( const Size& rect )
	{
		d_ActionButtonDistance.d_width = rect.d_width;
		d_ActionButtonDistance.d_height = rect.d_height ;
	}


	void		FalagardComplexWindow::setActionButtonImage( int nType, const Image* strImage)
	{
		if( nType == -1 )
			return;
		m_mapButtonImage[ nType ] = strImage;
	}
	const Size FalagardComplexWindow::getActionButtonDistance() const 
	{
		return d_ActionButtonDistance;
	}

	void FalagardComplexWindow::AddChildElement_Text(String32 strText, FontBase* font, TextFormatting typeset)
	{
		ChildElement_Text* pNewChild = new ChildElement_Text;

		pNewChild->d_Name = (utf8*)"text";
		pNewChild->d_Type = CT_Text;
		pNewChild->d_Text = strText;
		pNewChild->d_Typeset = typeset;
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		d_ChildVector.push_back(pNewChild);

		performWindowLayout();
	}

	void FalagardComplexWindow::AddChildElement_Option(String32 strText,String32 strExtra1,String32 strExtra2, String32 strText3, FontBase* font)
	{
		ChildElement_Option* pNewChild = new ChildElement_Option;

		pNewChild->d_Type = CT_Option;
		pNewChild->d_Text = strText;
		pNewChild->d_timeLastClick = 0.f;
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		char szTempName[32];
		int nIndex = 0;
		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			if(d_ChildVector[i]->d_Name.compare(0, 10, "__option__")==0)
			{
				nIndex++;
			}
		}

		_snprintf(szTempName, 32, "__option__%02d#%d,%d", nIndex,atoi(strExtra1.c_str()),atoi(strExtra2.c_str()));

		pNewChild->d_Name = (utf8*)szTempName;
		pNewChild->d_State = atoi(strText3.c_str());
		nIndex = 8;
		if( pNewChild->d_State > 0 && pNewChild->d_State < 19)
			nIndex = pNewChild->d_State;
		if( !WindowManager::getSingleton().isWindowPresent( getName() + szTempName ) )
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow( m_mapChildType[ ComplexTypeName_Option ], getName() + szTempName));
		else
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().getWindow( getName() + szTempName ));
		pNewChild->d_Button->setText(strText);
		pNewChild->d_Button->show();
		pNewChild->d_Button->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&FalagardComplexWindow::handleOptionClicked, this));
		_snprintf(szTempName, 32, "__optionImage__%02d#%d,%d", nIndex,atoi(strExtra1.c_str()),atoi(strExtra2.c_str()));
		if( !WindowManager::getSingleton().isWindowPresent( getName() + szTempName ) )
			pNewChild->d_Image = static_cast<FalagardStaticImage*>(WindowManager::getSingleton().createWindow(m_mapChildType[ ComplexTypeName_Image ], getName() + szTempName));
		else
			pNewChild->d_Image = static_cast<FalagardStaticImage*>(WindowManager::getSingleton().getWindow( getName() + szTempName ));
		//设为子窗口
		pNewChild->d_Image->setImage( m_mapButtonImage[ nIndex ] );
		d_ParentWindow->addChildWindow(pNewChild->d_Button);
		d_ParentWindow->addChildWindow(pNewChild->d_Image);
		d_ChildVector.push_back(pNewChild);
		performWindowLayout();
	}


	void FalagardComplexWindow::AddChildElement_Item(String strIconImage, String32 strText,  bool bRadio, bool bReArrange, FontBase* font)
	{
		ChildElement_Item* pNewChild = new ChildElement_Item;

		pNewChild->d_Type		= CT_Item;
		pNewChild->d_Text		= strText;
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		char szTempName[32];
		int nIndex = 0;

		if(bRadio)
		{
			for(int i=0; i<(int)d_ChildVector.size(); i++)
			{
				if(d_ChildVector[i]->d_Name.compare(0, 16, "__reward_radio__")==0) nIndex++;
				_snprintf(szTempName, 32, "__reward_radio__%02d", nIndex);
			}
		}
		else
		{
			_snprintf(szTempName, 32, "__reward__%02d", (int)d_ChildVector.size());
		}

		pNewChild->d_Name = (utf8*)szTempName;
		pNewChild->d_ReArrange = bReArrange;
		if( !WindowManager::getSingleton().isWindowPresent( getName() + szTempName ) )
			pNewChild->d_Button = static_cast<FalagardRewardItem*>(WindowManager::getSingleton().createWindow( m_mapChildType[ ComplexTypeName_RewardItem ], getName() + szTempName));
		else
			pNewChild->d_Button = static_cast<FalagardRewardItem*>(WindowManager::getSingleton().getWindow(getName() + szTempName));
		pNewChild->d_Button->setProperty("NormalImage", strIconImage);
		pNewChild->d_Button->setProperty("Tooltip", String(strText.c_str()));
		pNewChild->d_Button->show();
		pNewChild->d_Radio = bRadio;
		pNewChild->d_Button->setHelper((void*)pNewChild);
		pNewChild->d_Button->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&FalagardComplexWindow::handleRewardItemClicked, this));

		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_Button);

		d_ChildVector.push_back(pNewChild);
		if(bRadio) d_RadioChild.push_back(pNewChild);
		performWindowLayout();
	}

	Window* FalagardComplexWindow::AddChildElement_Action(String strIconImage, int ActionID, bool bRadio, bool bReArrange, FontBase* font)
	{
		ChildElement_Action* pNewChild = new ChildElement_Action;

		pNewChild->d_Type		= CT_Action;
		pNewChild->d_Text		= (utf8*)"action";
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		char szTempName[32];
		int nIndex = 0;

		if(bRadio)
		{
			for(int i=0; i<(int)d_ChildVector.size(); i++)
			{

				if(d_ChildVector[i]->d_Name.compare(0, 15, "__reward_radio_")==0) nIndex++;
				_snprintf(szTempName, 32, "__reward_radio_%02d_%02d", (int)ActionID,nIndex);

			}
		}
		else
		{
			//			_snprintf(szTempName, 32, "__reward__%02d", (int)d_ChildVector.size());
			_snprintf(szTempName, 32, "__reward__%02d", (int)ActionID);
		}

		pNewChild->d_Name = (utf8*)szTempName;
		pNewChild->d_ReArrange = bReArrange;
		pNewChild->d_Radio = bRadio;
		if( !WindowManager::getSingleton().isWindowPresent( getName() + szTempName ) )
			pNewChild->d_ActionButton = static_cast<FalagardActionButton*>(WindowManager::getSingleton().createWindow( m_mapChildType[ ComplexTypeName_ActionButton ] , getName() + szTempName));
		else
			pNewChild->d_ActionButton = static_cast<FalagardActionButton*>(WindowManager::getSingleton().getWindow( getName() + szTempName));
		pNewChild->d_ActionButton->setLogicItemData((void*)(__w64 int)ActionID);
		pNewChild->d_ActionButton->show();
		pNewChild->d_ActionButton->setProperty("Empty", "False");
		pNewChild->d_ActionButton->setClippedByParent(true);
		pNewChild->d_ActionButton->setHelper((void*)pNewChild);
		pNewChild->d_ActionButton->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&FalagardComplexWindow::handleActionButtonClicked, this));

		if(!strIconImage.empty())
		{
			pNewChild->d_ActionButton->setProperty("NormalImage", strIconImage);
		}

		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_ActionButton);

		d_ChildVector.push_back(pNewChild);
		if(bRadio) d_RadioChild_Action.push_back(pNewChild);
		performWindowLayout();
		return pNewChild->d_ActionButton;
	}


	void FalagardComplexWindow::AddChildElement_HyperLink( String32 strText,	int nLinkID, bool bReArrange, FontBase* font)
	{
		ChildElement_HyperLink* pNewChild = new ChildElement_HyperLink;

		pNewChild->d_Type		= CT_HyperLink;
		pNewChild->d_Text		= strText;//(utf8*)"hyperlink";
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		char szTempName[32];
		int nIndex = 0;
		/*
		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
		if(d_ChildVector[i]->d_Name.substr(0, 10) == (utf8*)"__option__")
		{
		nIndex++;
		}
		}
		*/
		_snprintf(szTempName, 32, "%d", nLinkID);

		pNewChild->d_Name = (utf8*)szTempName;
		pNewChild->d_ReArrange = bReArrange;
		if( !WindowManager::getSingleton().isWindowPresent( getName() + szTempName ) )
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow(m_mapChildType[ ComplexTypeName_Button], getName() + szTempName));
		else
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().getWindow(getName() + szTempName));
		pNewChild->d_Button->setText(strText);
		pNewChild->d_Button->show();
		pNewChild->d_Button->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&FalagardComplexWindow::handleLinkClicked, this));
		//		pNewChild->d_State = atoi(strExtra2.c_str());


		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_Button);

		d_ChildVector.push_back(pNewChild);
		//		if(bRadio) d_RadioChild.push_back(pNewChild);
		performWindowLayout();
		return ;
	}


	void FalagardComplexWindow::AddChildElement_Money(int money, FontBase* font)
	{
		ChildElement_Money* pNewChild = new ChildElement_Money;

		pNewChild->d_Type = CT_Money;
		pNewChild->d_Name = (utf8*)"money";
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();
		if( !WindowManager::getSingleton().isWindowPresent( pNewChild->d_Name ) )
			pNewChild->d_Money = static_cast<FalagardMoney*>(WindowManager::getSingleton().createWindow(m_mapChildType[ ComplexTypeName_Money ]));
		else
			pNewChild->d_Money = static_cast<FalagardMoney*>(WindowManager::getSingleton().getWindow(pNewChild->d_Name));

		pNewChild->d_Money->setMoneyNumber(money);
		pNewChild->d_Money->show();

		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_Money);

		d_ChildVector.push_back(pNewChild);
		performWindowLayout();
	}

	void FalagardComplexWindow::AddChildElement_ChatBoard(String32 strText, FontBase* font, TextFormatting typeset)
	{
		ChildElement_ChatBoard* pNewChild = new ChildElement_ChatBoard;

		pNewChild->d_Type = CT_ChatBoard;
		pNewChild->d_Name = (utf8*)"chatboard";
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();
		if( !WindowManager::getSingleton().isWindowPresent( pNewChild->d_Name ) )
			pNewChild->d_Board = static_cast<FalagardChatBoard*>(WindowManager::getSingleton().createWindow(m_mapChildType[ ComplexTypeName_ChatBoard ]) );
		else
			pNewChild->d_Board = static_cast<FalagardChatBoard*>(WindowManager::getSingleton().getWindow(pNewChild->d_Name) );
		pNewChild->d_Board->setChatBoardString(strText);
		pNewChild->d_Board->setFont(pNewChild->d_Font);
		//TextFormatting 目前还不需要
		pNewChild->d_Board->show();

		pNewChild->d_Board->subscribeEvent(FalagardChatBoard::EventInfoElementLClick, Event::Subscriber(&FalagardComplexWindow::handleChatBoardInfoElementClicked, this));
		pNewChild->d_Board->subscribeEvent(FalagardChatBoard::EventInfoElementRClick, Event::Subscriber(&FalagardComplexWindow::handleChatBoardInfoElementClicked, this));
		pNewChild->d_Board->subscribeEvent(FalagardChatBoard::EventInfoElementDelete, Event::Subscriber(&FalagardComplexWindow::handleChatBoardInfoElementDelete, this));

		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_Board);

		d_ChildVector.push_back(pNewChild);
		performWindowLayout();
	}

	void FalagardComplexWindow::performWindowLayout(void)
	{
		// get WidgetLookFeel for the assigned look.
		const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
		// get Icon area for Item
		//        const Rect iconItemArea(wlf.getNamedArea("ItemIconSize").getArea().getPixelRect(*this));

		Rect absarea(getTextRenderArea());

		float fHeightNow = 0.0f;
		float fItemPosition = 0.0f;
		float fTextPosition = 0.0f;
		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ChildVector[i];
			const FontBase* font = pElement->d_Font;

			switch(pElement->d_Type)
			{
			case CT_Text:
				{
					fItemPosition = 0.0f;
					ChildElement_Text* pTextElement = (ChildElement_Text*)pElement;
					/*					if(i > 0)
					{
					ChildElement* pElement_Before = d_ChildVector[i-1];

					//					if(pElement_Before->d_Type != CT_Item)
					//						fHeightNow += d_ActionButtonDistance.d_height  * absarea.getHeight();
					if(pElement_Before && pElement_Before->d_Type != CT_Text )
					fHeightNow += d_ActionButtonDistance.d_height  * absarea.getHeight();
					}
					*/
					float textHeight = font->getFormattedSize(pTextElement->d_Text, absarea, (TextFormatting)pTextElement->d_Typeset).d_height;
					float fontHeight = font->getFormattedSize(pTextElement->d_Text, absarea, (TextFormatting)LeftAligned).d_height;

					//					AxTrace(0, 2, "ComplexWindowCalText:%f, %f[%f]", absarea.getWidth(), absarea.getHeight(), textHeight);

					pTextElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
					pTextElement->d_rectClient.setSize(Size(absarea.getWidth(), textHeight));

					//					fTextPosition += absarea.getWidth();
					fHeightNow += textHeight + CEGUI::FontBase::LineSpace;
					//					fHeightNow += fontHeight;
					//					fHeightNow += font->getLineSpacing() * 1.2f;
					//					AxTrace(0, 2, "textHeight = %f",textHeight);
				}
				break;

			case CT_Option:
				{
					fItemPosition = 0.0f;
					fTextPosition = 0.0f;
					ChildElement_Option* pOptionElement = (ChildElement_Option*)pElement;
					if(i > 0)
					{
						ChildElement* pElement_Before = d_ChildVector[i-1];

						if( pElement_Before && pElement_Before->d_Type != CT_Option )
						{
							//AxTrace(0, 2, "fHeightNow = %f",fHeightNow);
							fHeightNow += (font->getLineSpacing()); //* 2.0f;
							//AxTrace(0, 2, "font->getLineSpacing() = %f",font->getLineSpacing());
							//AxTrace(0, 2, "fHeightNow = %f",fHeightNow);
						}
					}

					float textHeight = font->getLineSpacing()*1.7f;

					pOptionElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
					pOptionElement->d_rectClient.setSize(Size(absarea.getWidth(), textHeight));


					pOptionElement->d_Image->setPosition(Absolute, Point(0.0f, fHeightNow-d_vertScrollbar->getScrollPosition()));
					pOptionElement->d_Image->setSize(Absolute, Size( textHeight, textHeight));
					pOptionElement->d_Button->setPosition(Absolute, Point(textHeight, fHeightNow-d_vertScrollbar->getScrollPosition()));
					pOptionElement->d_Button->setSize(Absolute, Size(absarea.getWidth() - textHeight, textHeight));

					fHeightNow += (float)(textHeight * 1.08);
				}
				break;
			case CT_HyperLink:
				{
					/*
					fItemPosition = 0.0f;
					ChildElement_HyperLink*	pLinkElement = (ChildElement_HyperLink*)pElement;
					ChildElement* pElement_Before = d_ChildVector[i-1];

					float textHeight = font->getFormattedSize(pLinkElement->d_Text, absarea, (TextFormatting)WordWrapLeftAligned).d_height;

					float textWidth  = font->getFormattedSize(pLinkElement->d_Text, absarea, (TextFormatting)WordWrapLeftAligned).d_width;

					//					if(pElement_Before->d_Type != CT_Item)
					//						fHeightNow += d_ActionButtonDistance.d_height  * absarea.getHeight();

					//					if( pElement_Before->d_Type != CT_Text )
					//						fHeightNow += textHeight;

					if(pLinkElement->d_ReArrange)
					{
					pLinkElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
					fTextPosition = 0.0f;
					}
					else
					{

					float temp = fTextPosition + textWidth;

					if( temp > absarea.getWidth() )
					fTextPosition = 0.0f;
					else if(fTextPosition != 0.0f)
					{
					fHeightNow -= textHeight;
					}
					}
					/*
					pLinkElement->d_rectClient.setPosition(Point(fTextPosition, fHeightNow));
					//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), iconItemArea.getHeight()));
					//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );
					pLinkElement->d_rectClient.setSize(Size(d_ActionButtonSize.d_width * absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );

					pItemElement->d_Button->setPosition(Absolute, Point(fItemPosition, fHeightNow-d_vertScrollbar->getScrollPosition()));
					//					pItemElement->d_Button->setSize(Absolute, Size(iconItemArea.getWidth(), iconItemArea.getHeight()));
					pItemElement->d_Button->setSize(Relative, Size(d_ActionButtonSize.d_width, d_ActionButtonSize.d_height) );

					//					fHeightNow += iconItemArea.getHeight();
					fHeightNow += ( ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height ) * absarea.getHeight() );
					fItemPosition += ( ( d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width ) * absarea.getWidth() );
					//如果已经是最后一个元素了，下一行就别空那么多了。chris
					if(i == (int)d_ChildVector.size() )
					fHeightNow -= (d_ActionButtonDistance.d_height  * absarea.getHeight());
					*/
				}
				break;
			case CT_Item:
				{
					fTextPosition = 0.0f;
					ChildElement_Item*	pItemElement = (ChildElement_Item*)pElement;
					if(i > 0)
					{
						ChildElement* pElement_Before = d_ChildVector[i-1];

						//					if(pElement_Before->d_Type != CT_Item)
						//						fHeightNow += d_ActionButtonDistance.d_height  * absarea.getHeight();
						if(pElement_Before && pElement_Before->d_Type != CT_Item && pElement_Before->d_Type != CT_Action)
							fHeightNow += d_ActionButtonDistance.d_height;
					}

					if(pItemElement->d_ReArrange)
					{
						pItemElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
						fItemPosition = 0.0f;
					}
					else
					{

						float temp = fItemPosition + ( d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width  );

						if( temp > absarea.getWidth() )
							fItemPosition = 0.0f;
						else if(fItemPosition != 0.0f)
						{
							fHeightNow -= ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height );
						}
					}

					pItemElement->d_rectClient.setPosition(Point(fItemPosition, fHeightNow));
					//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), iconItemArea.getHeight()));
					//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );
					pItemElement->d_rectClient.setSize(Size(d_ActionButtonSize.d_width , d_ActionButtonSize.d_height  ) );

					pItemElement->d_Button->setPosition(Absolute, Point(fItemPosition, fHeightNow-d_vertScrollbar->getScrollPosition()));
					//					pItemElement->d_Button->setSize(Absolute, Size(iconItemArea.getWidth(), iconItemArea.getHeight()));
					pItemElement->d_Button->setSize(Absolute, Size(d_ActionButtonSize.d_width, d_ActionButtonSize.d_height) );

					//					fHeightNow += iconItemArea.getHeight();
					fHeightNow += ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height );
					fItemPosition += (  d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width );
					//如果已经是最后一个元素了，下一行就别空那么多了。chris
					if(i == (int)d_ChildVector.size() )
						fHeightNow -= (d_ActionButtonDistance.d_height );
				}
				break;

			case CT_Action:
				{
					fTextPosition = 0.0f;
					ChildElement_Action*	pItemElement = (ChildElement_Action*)pElement;

					if(i > 0)
					{
						ChildElement* pElement_Before = d_ChildVector[i-1];

						if(pElement_Before->d_Type != CT_Item && pElement_Before->d_Type != CT_Action)
							fHeightNow += (d_ActionButtonDistance.d_height);
					}

					if(pItemElement->d_ReArrange)
					{
						pItemElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
						fItemPosition = 0.0f;
					}
					else
					{

						float temp = fItemPosition + (d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width  );

						if( temp > absarea.getWidth() )
							fItemPosition = 0.0f;
						else if(fItemPosition != 0.0f)
						{
							fHeightNow -= ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height );
						}
					}

					pItemElement->d_rectClient.setPosition(Point(fItemPosition, fHeightNow));
					//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), iconItemArea.getHeight()));
					//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );
					pItemElement->d_rectClient.setSize(Size(d_ActionButtonSize.d_width, d_ActionButtonSize.d_height ) );

					pItemElement->d_ActionButton->setPosition(Absolute, Point(fItemPosition, fHeightNow-d_vertScrollbar->getScrollPosition()));
					//					pItemElement->d_Button->setSize(Absolute, Size(iconItemArea.getWidth(), iconItemArea.getHeight()));
					pItemElement->d_ActionButton->setSize(Absolute, Size(d_ActionButtonSize.d_width, d_ActionButtonSize.d_height) );

					//					fHeightNow += iconItemArea.getHeight();
					fHeightNow += ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height );
					fItemPosition += ( d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width  );
					//如果已经是最后一个元素了，下一行就别空那么多了。chris
					if(i == (int)d_ChildVector.size() - 1 )
						fHeightNow -= ( d_ActionButtonDistance.d_height );
				}
				break;
			case CT_Money:
				{
					fTextPosition = 0.0f;
					fItemPosition = 0.0f;
					ChildElement_Money* pMoneyElement = (ChildElement_Money*)pElement;

					float textHeight = font->getLineSpacing() * 1.4f;

					pMoneyElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
					pMoneyElement->d_rectClient.setSize(Size(absarea.getWidth(), textHeight));

					pMoneyElement->d_Money->setPosition(Absolute, Point(0.0f, fHeightNow-d_vertScrollbar->getScrollPosition()));
					pMoneyElement->d_Money->setSize(Absolute, Size(absarea.getWidth(), textHeight));

					fHeightNow += textHeight;
				}
				break;
			case CT_ChatBoard:
				{
					fTextPosition = 0.0f;
					fItemPosition = 0.0f;
					ChildElement_ChatBoard* pChatBoard = (ChildElement_ChatBoard*)pElement;

					float childHeight = pChatBoard->d_Board->resizeSelf(absarea.getWidth());

					pChatBoard->d_Board->setSize(Absolute, Size(absarea.getWidth(), childHeight));
					pChatBoard->d_Board->setPosition(Absolute, Point(0.0f, fHeightNow-d_vertScrollbar->getScrollPosition()));
					pChatBoard->d_Board->requestRedraw();

					fHeightNow += childHeight;
				}
				break;
			}
		}

		d_totalHeight = fHeightNow;

		configureScrollbars();
	}

	void FalagardComplexWindow::ClearAllElement(void)
	{
		_destroyAllElements();

		performWindowLayout();
		requestRedraw();
		d_ParentWindow->requestRedraw();
	}

	void FalagardComplexWindow::_destroyAllElements(void)
	{
		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ChildVector[i];
			switch(pElement->d_Type)
			{
			case CT_Text:
				break;

			case CT_Option:
				WindowManager::getSingleton().destroyWindow(((ChildElement_Option*)pElement)->d_Button);
				WindowManager::getSingleton().destroyWindow(((ChildElement_Option*)pElement)->d_Image);
				break;

			case CT_Item:
				WindowManager::getSingleton().destroyWindow(((ChildElement_Item*)pElement)->d_Button);
				break;

			case CT_Action:
				WindowManager::getSingleton().destroyWindow(((ChildElement_Action*)pElement)->d_ActionButton);
				break;

			case CT_Money:
				WindowManager::getSingleton().destroyWindow(((ChildElement_Money*)pElement)->d_Money);
				break;

			case CT_HyperLink:
				WindowManager::getSingleton().destroyWindow(((ChildElement_HyperLink*)pElement)->d_Button);
				break;
			case CT_ChatBoard:
				((ChildElement_ChatBoard*)pElement)->d_Board->clearInfoItem();
				WindowManager::getSingleton().destroyWindow(((ChildElement_ChatBoard*)pElement)->d_Board);
				break;
			}

			delete d_ChildVector[i];
			d_ChildVector[i] = NULL;
		}
		d_ChildVector.clear();
		d_RadioChild.clear();
		d_RadioChild_Action.clear();
		requestRedraw();
		d_ParentWindow->requestRedraw();
		//		performWindowLayout();
	}

	void FalagardComplexWindow::configureScrollbars(void)
	{
		Scrollbar* vertScrollbar;

		try
		{
			vertScrollbar = static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(getName() + "__auto_vscrollbar__"));
		}
		catch (UnknownObjectException)
		{
			// no scrollbars?  Can't configure then!
			return;
		}

		Rect initialArea(getTextRenderArea());
		float totalHeight	= d_totalHeight;

		//
		// First show or hide the scroll bars as needed (or requested)
		//
		// show or hide vertical scroll bar as required (or as specified by option)
		if (totalHeight > getTextRenderArea().getHeight())
		{
			vertScrollbar->show();
		}
		else
		{
			vertScrollbar->hide();
		}

		//
		// Set up scroll bar values
		//
		Rect renderArea(getTextRenderArea());

		vertScrollbar->setDocumentSize(totalHeight);
		vertScrollbar->setPageSize(renderArea.getHeight());
		vertScrollbar->setStepSize(ceguimax(1.0f, renderArea.getHeight() / 10.0f));
		vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition());
		requestRedraw();
		d_ParentWindow->requestRedraw();
	}

	void FalagardComplexWindow::PageEnd()
	{
		Rect renderArea(getTextRenderArea());
		d_vertScrollbar->setScrollPosition( d_totalHeight - renderArea.getHeight() );
		configureScrollbars();
	}

	void FalagardComplexWindow::subscribInfoItemClickEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventInfoElementClick, subscriber);
	}

	void FalagardComplexWindow::subscribInfoItemDeleteEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventInfoElementDelete, subscriber);
	}

	void FalagardComplexWindow::prepareBigchar(void)
	{
		Window::prepareBigchar();

		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ChildVector[i];
			if(CT_Text == pElement->d_Type && pElement->d_Font)
			{
				((FontBase*)(pElement->d_Font))->prepareString(((ChildElement_Text*)pElement)->d_Text);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

	Factory Methods

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	Window* FalagardComplexWindowFactory::createWindow(const String& name)
	{
		return new FalagardComplexWindow(d_type, name);
	}

	void FalagardComplexWindowFactory::destroyWindow(Window* window)
	{
		delete window;
	}
};