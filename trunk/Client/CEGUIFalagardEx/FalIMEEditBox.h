#ifndef _FalIMEEditBox_h_
#define _FalIMEEditBox_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalIMEEditBoxProperties.h"
#include "FalEditbox.h"
#include "IFalIMEEditBox.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	class FALAGARDBASE_API FalagardIMEEditBox : public FalagardEditbox, public IFalagardIMEEditBox
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.

		static const String EventNamespace;
		static const String EventItemElementDelete;
		static const String EventItemelementJoinFailure;
		static const String EventItemElementFull;

		static const utf32 infoBegin;
		static const utf32 infoEnd;

		/*!
		\brief
		Constructor
		*/
		FalagardIMEEditBox(const String& type, const String& name);

		/*!
		\brief
		Destructor
		*/
		~FalagardIMEEditBox();

		/*************************************************************************
		Accessors
		*************************************************************************/

		/*!
		\brief
		Reset the caret flash time.

		\return
		None.
		*/
		virtual void	resetCaretBlink(void);

		/*!
		\brief
		return true if the Editbox has input focus.

		\return
		true if the Editbox has keyboard input focus, false if the Editbox does not have keyboard input focus.
		*/
		virtual bool	hasInputFocus(void) const;

		/*!
		Add a item element info to edit box
		*/
		virtual bool	injectItemInfo(int nID, const String32& strDisplayName, int nLength);
		virtual void	subscribeItemElementDeleteEvent(Event::Subscriber subscriber);
		virtual void	setItemElementString(const String32& szInfoString);
		virtual String32	getItemElementString();

		/*!
		Set blink txt. use this to simulate blink old and refresh as new.
		*/
		virtual void	setBlinkText (const String32& text);

		/*!
		\brief
		set clear offset property.
		\return
		None.
		*/
		virtual void	setClearOffset(const bool set);
		virtual void	setMaxItemElement(const int num){d_maxItemElement = num;}

		/*!
		\brief
		get clear offset property.
		\return
		true if clear offset is set.
		false if clear offset is not set.
		*/
		virtual bool	getClearOffset(void) const;
		virtual int		getMaxItemElement(void) const {return d_maxItemElement;};
		virtual Rect	getCaratTextExtent(void) const;

		/*************************************************************************
		Overload EditBox function.
		*************************************************************************/
		virtual void    addCharater( const utf32 &codepoint, bool isFromSoftKey = false );
		virtual void	handleBackspace(void);
		virtual void	handleDelete(void);

		/** Is this editbox disable Ctrl+C and Ctrl+V
		*/
		virtual bool	getCopyPasteDisable(void) const { return d_bCopyPasteDisable; }
		void			setCopyPasteDisable(bool disable) { d_bCopyPasteDisable=disable; }
		void	setSoftKeyOnly( bool bSoftKeyOnly ) { d_softKeyOnly = bSoftKeyOnly; };				//是否只接受软件盘的输入
	protected:
		/*************************************************************************
		Implementation of abstract methods in Window
		*************************************************************************/
		virtual	void			onMouseButtonDown(MouseEventArgs& e);
		virtual	void			onTextAcceptedEvent(WindowEventArgs& e);
		virtual void			populateRenderCache();
		virtual void			updateSelf(float elapsed);
		virtual void			onActivated(ActivationEventArgs& e);
		virtual void			onDeactivated(ActivationEventArgs& e);
		virtual void			onParentHidden(void);

		virtual size_t			getTextIndexFromPosition(const Point& pt) const;
		// rendering internal vars
		float	d_lastTextOffset;			//!< x rendering offset used last time we drew the widget.


		double	d_blinkTime;				//!< Caret blink time in milliseconds
		double	d_lastBlink;				//!< Last timestamp of caret blink
		bool	d_caretOn;					//!< Flag to indicate whether caret is currently visible

		String32	d_blinkText;

		int		d_lastTextLen;				//!< Store the text length.
		bool	d_clearOffset;				//!< Flag to indicate whether the offset should clear.
		int		d_maxItemElement;			//!< Maximum number of itemelements for this Editbox.
		bool	d_addDirect;				//!< Flag to indicate add character ignore the length check.
		bool	d_bCopyPasteDisable;		//!< Disable Copy Paste

		bool	d_softKeyOnly;				//是否只接受软件盘的输入
		/*!
		Struct to store logic item data
		*/
		struct ItemElement
		{
			int nID;			//!< ID impl logic manager
			String32 strColor;	//!< String Color of this item.
			int nBeginIndex;	//!< Index begin at window text
			int nEndIndex;		//!< Index end at window text
			int	nLength;		//!< Element contents length, used for check length valid.

			ItemElement():nID(-1),nBeginIndex(-1),nEndIndex(-1),nLength(0){}
		};

		typedef std::list< ItemElement > ItemElementList;
		ItemElementList d_itemList;

		//check str or pElement with d_maxLength
		virtual bool			checkLength(const String32& str, const ItemElement* pElement = 0);
		//find list delete iterator
		void findSelectionItemElements(ItemElementList::iterator& itBegin, ItemElementList::iterator& itEnd, ItemElementList& itemList);
		void findCaratIndexItemElement(ItemElementList::iterator& itCarat, ItemElementList& itemList,int newPos = -1);
		//del list elements
		void delSelectionItemElements(ItemElementList& itemList, bool bFireEvent = true);
		bool delCaratIndexItemElement(ItemElementList& itemList, int newPos = -1, bool bFireEvent = true);


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
			if (class_name==(const utf8*)"FalagardIMEEditBox")	return true;
			return FalagardEditbox::testClassName_impl(class_name);
		}

	private:
		/*************************************************************************
		Static properties for the ChatEditBox widget
		*************************************************************************/
		static FalagardIMEEditBoxProperties::DefaultEditBox	d_defaultEditBoxProperty;
		static FalagardIMEEditBoxProperties::ClearOffset	d_clearOffsetProperty;
		static FalagardIMEEditBoxProperties::ElementNum		d_elementNumProperty;
		static FalagardIMEEditBoxProperties::DisableCopyPaste	d_disableCopyPasteProperty;
		static FalagardIMEEditBoxProperties::SoftKeyOnly	d_softKeyOnlyProperty;

		/*************************************************************************
		Implementation methods
		*************************************************************************/

		/*!
		\brief
		Adds properties specific to the IMEEditBox base class.

		\return
		Nothing.
		*/
		void addIMEEditBoxProperties(void);

		void changeBlinkText(void);
	};

	/*!
	\brief
	WindowFactory for FalagardIMEEditBox type Window objects.
	*/
	class FALAGARDBASE_API FalagardIMEEditBoxFactory : public WindowFactory
	{
	public:
		FalagardIMEEditBoxFactory(void) : WindowFactory(FalagardIMEEditBox::WidgetTypeName) { }
		~FalagardIMEEditBoxFactory(void){}
		Window* createWindow(const String& name);
		void destroyWindow(Window* window);
	};
}

#endif