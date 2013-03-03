#ifndef _FalComplexWindow_h_
#define _FalComplexWindow_h_

#include "CEGUIWindow.h"
#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "IFalagardComplexWindow.h"
#include "falstaticImage.h"
#include "FalagardComplexWindowProperties.h"
#include "IExpProperty.h"
// Start of CEGUI namespace section
namespace CEGUI
{
	class FalagardButton;
	class FalagardRewardItem;
	class FalagardActionButton;
	class FalagardMoney;
	class FalagardChatBoard;

	//================================================================================
	//
	// FalagardComplexWindow
	//
	//================================================================================
	class FALAGARDBASE_API FalagardComplexWindow : public IExpProperty, public IFalagardComplexWindow
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.

		/*************************************************************************
		Event name constants
		*************************************************************************/
		static const String EventNamespace;				//!< Namespace for global events
		static const String EventOptionClicked;
		static const String EventLinkClicked;
		static const String EventRewardItemClicked;
		static const String EventActionButtonClicked;

		static const String EventInfoElementClick;
		static const String EventInfoElementDelete;

		static FalagardComplexWindowProperties::ActionButtonSize		d_ActionButtonSizeProperty;
		static FalagardComplexWindowProperties::ActionButtonDistance	d_ActionButtonDistanceProperty;
		static FalagardComplexWindowProperties::QuestButtonImage		d_QuestButtonImageProperty;
		static FalagardComplexWindowProperties::OptionCooldownTime 		d_OptionCooldownTimeProperty;

		/*!
		\brief
		Constructor
		*/
		FalagardComplexWindow(const String& type, const String& name);

		/*!
		\brief
		Destructor
		*/
		~FalagardComplexWindow();

		/*!
		\brief
		Initialise the Window based object ready for use.

		\note
		This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

		\return
		Nothing
		*/
		virtual void	initialise(void);

		//设置window内的action button的size
		void		setActionButtonSize( const Size& rect );
		//取得window内的action button的size
		const Size	getActionButtonSize() const;

		//设置window内的action button的distance
		void		setActionButtonDistance( const Size& rect );
		//取得window内的action button的distance
		const Size	getActionButtonDistance() const;

		void		setActionButtonImage( int nType, const Image* strImage );

		float		getOptionbuttonCooldownTime(void) const { return d_optionButtonCooldownTime; }
		void		setOptionbuttonCooldownTime(float time) {d_optionButtonCooldownTime=time; }

		/// get vert scrollbar
		Scrollbar*	getVerteScrollbar(void) { return d_vertScrollbar; }

	public:
		//Add ChildElement (Text)
		virtual void			AddChildElement_Text(String32 strText, FontBase* font, TextFormatting typeset=WordWrapLeftAligned);
		//Add ChildElement (Option)
		virtual void			AddChildElement_Option(String32 strText,String32 strExtra1,String32 strExtra2, String32 strText3, FontBase* font=NULL);

		//Add ChildElement (Link)
		virtual void			AddChildElement_HyperLink( String32 strText, int nLinkID, bool bReArrange, FontBase* font=NULL);

		//Add ChildElement (Item)
		virtual void			AddChildElement_Item(String strIconImage, String32 strText,  bool bRadio, bool bReArrange, FontBase* font=NULL);

		//Add ChildElement (Action)
		virtual Window*			AddChildElement_Action(String strIconImage, int ActionID, bool bRadio, bool bReArrange, FontBase* font=NULL);

		//Add ChildElement (Money)
		virtual void			AddChildElement_Money(int money, FontBase* font=NULL);

		//Add ChildElement (ChatBoard)
		virtual void			AddChildElement_ChatBoard(String32 strText, FontBase* font, TextFormatting typeset=WordWrapLeftAligned);

		//Clear
		virtual void			ClearAllElement(void);
		//Recaculate
		void					performWindowLayout(void);

		virtual void			PageEnd();

		virtual void			subscribInfoItemClickEvent(Event::Subscriber subscriber);
		virtual void			subscribInfoItemDeleteEvent(Event::Subscriber subscriber);
	public:
		class ParentWindow : public Window
		{
		public:
			static const utf8   WidgetTypeName[];       //!< type name for this widget.
			ParentWindow(const String& type, const String& name);
			~ParentWindow();

			void	SetComplexWindow(FalagardComplexWindow* pComplex)
			{
				d_ComplexWindow = pComplex;
			}

			void requestAllRedraw(void);

		protected:
			virtual void	populateRenderCache();
			virtual void	onMouseWheel(MouseEventArgs& e);

			FalagardComplexWindow* d_ComplexWindow;
		};

		class ParentWindowFactory : public WindowFactory
		{
		public:
			ParentWindowFactory(void) : WindowFactory(ParentWindow::WidgetTypeName) { }
			~ParentWindowFactory(void){}
			Window* createWindow(const String& name);
			void destroyWindow(Window* window);
		};

	protected:
		//----------------------------------------------------------------
		//Child Window Type
		struct ChildElement
		{
			String			d_Name;
			ChildType		d_Type;
			Rect			d_rectClient;
			const FontBase*	d_Font;
			String32         d_exInfo1;
			String32         d_exInfo2;
			TextFormatting	d_Typeset;

			ChildElement(){}
			virtual ~ChildElement() {}
		};

		//Text
		struct ChildElement_Text : public ChildElement
		{
			String32					d_Text;
		};

		//Option
		struct ChildElement_Option : public ChildElement
		{
			FalagardStaticImage*		d_Image;
			FalagardButton*				d_Button;
			String32					d_Text;
			int							d_State;
			float						d_timeLastClick;
		};

		// RewardItem
		struct ChildElement_Item : public ChildElement
		{
			FalagardRewardItem*			d_Button;
			//			FalagardActionButton*		d_ActionButton;
			String32					d_Text;
			bool						d_ReArrange;
			bool						d_Radio;
		};

		// 只有Action
		struct ChildElement_Action : public ChildElement
		{
			//			FalagardRewardItem*			d_Button;
			FalagardActionButton*		d_ActionButton;
			String32					d_Text;
			bool						d_ReArrange;
			bool						d_Radio;
		};

		// 只有HyperLink这种button
		struct ChildElement_HyperLink : public ChildElement
		{
			FalagardButton*				d_Button;
			int							d_State;
			String32					d_Text;
			bool						d_ReArrange;
		};

		// Money
		struct ChildElement_Money : public ChildElement
		{
			FalagardMoney*				d_Money;
		};

		// ChatBoard
		struct ChildElement_ChatBoard : public ChildElement
		{
			FalagardChatBoard*			d_Board;
		};

		Size							d_ActionButtonSize;			// Action Button的size
		Size							d_ActionButtonDistance;		// Action Button的distance

		typedef std::vector< ChildElement* >	ChildVector;
		typedef std::vector< ChildElement_Item* >	RadioItemChild;
		typedef std::vector< ChildElement_Action* >	RadioActionChild;

		typedef std::map< int, const Image* > ButtonImage;
		ButtonImage				m_mapButtonImage;

		float d_optionButtonCooldownTime;		//!< Cooldown time for option button.

	protected:
		/*************************************************************************
		Implementation functions
		*************************************************************************/

		/*************************************************************************
		Overridden events
		*************************************************************************/
		virtual void	onSized(WindowEventArgs& e);
		virtual void	prepareBigchar(void);

		/*!
		\brief
		Return a Rect object describing, in un-clipped pixels, the window relative area
		that the text should be rendered in to.

		\return
		Rect object describing the area of the Window to be used for rendering text.
		*/
		Rect		getTextRenderArea(void) const;

		/*!
		\brief
		display required integrated scroll bars according to current state of the edit box and update their values.
		*/
		void		configureScrollbars(void);
		/*!
		\brief
		Destroy all child elements
		*/
		void		_destroyAllElements(void);


		/*!
		\brief
		Return whether this window was inherited from the given class name at some point in the inheritance heirarchy.

		\param class_name
		The class name that is to be checked.

		\return
		true if this window was inherited from \a class_name. false if not.
		*/
		virtual bool	testClassName_impl(const String& class_name) const
		{
			if (class_name==(const utf8*)"FalagardComplexWindow")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*************************************************************************
		Event subscribers
		*************************************************************************/
		/*!
		\brief
		handler function for when the scroll bar positions change.
		*/
		bool	handleScrollbarChange(const EventArgs& e);
		/*!
		\brief
		handler function for when option button clicked.
		*/
		bool	handleOptionClicked(const EventArgs& e);
		/*!
		\brief
		handler function for when Link button clicked.
		*/
		bool	handleLinkClicked(const EventArgs& e);
		/*!
		\brief
		handler function for when reward item icon clicked.
		*/
		bool	handleRewardItemClicked(const EventArgs& e);

		/*!
		\brief
		handler function for when Action Button clicked.
		*/
		bool	handleActionButtonClicked(const EventArgs& e);

		/*!
		\brief
		handler function for when ChatBoard InfoElement clicked.
		*/
		bool	handleChatBoardInfoElementClicked(const EventArgs& e);

		/*!
		\brief
		handler function for when ChatBoard InfoElement delete.
		*/
		bool	handleChatBoardInfoElementDelete(const EventArgs& e);

		/*************************************************************************
		Implementation Data
		*************************************************************************/
		ParentWindow*	d_ParentWindow;			//!< Parent of all child element
		Scrollbar*		d_vertScrollbar;		//!< Widget used as vertical scrollbar;
		bool			d_enableVertScrollbar;	//!< true if vertical scroll bar is enabled.
		ChildVector		d_ChildVector;			//!< Child element vector 
		RadioItemChild	d_RadioChild;			//!< Item that enable radio selected
		RadioActionChild d_RadioChild_Action;	//!< Action Button that enable radio selected
		float			d_totalHeight;			//!< Total Height 
	};


	/*!
	\brief
	WindowFactory for FalagardComplexWindow type Window objects.
	*/
	class FALAGARDBASE_API FalagardComplexWindowFactory : public WindowFactory
	{
	public:
		FalagardComplexWindowFactory(void) : WindowFactory(FalagardComplexWindow::WidgetTypeName) { }
		~FalagardComplexWindowFactory(void){}
		Window* createWindow(const String& name);
		void destroyWindow(Window* window);
	};
}

#endif