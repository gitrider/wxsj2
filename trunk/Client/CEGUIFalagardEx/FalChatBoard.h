#ifndef _FalChatBoard_h_
#define _FalChatBoard_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalChatBoardProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	//InfoElement type
	enum InfoType
	{
		hyperlinkInfo,		//hyperlink type info
		itemelementInfo,	//itemelement type info
	};

	class FALAGARDBASE_API FalagardChatBoard : public Window
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.

		static const utf32 infoBegin;
		static const utf32 infoEnd;

		static const String EventNamespace;
		static const String EventInfoElementLClick;
		static const String EventInfoElementRClick;
		static const String EventInfoElementDelete;

		static const String EventInfoElementMoveIn;
		static const String EventInfoElementMoveOut;

		static const String EventKillBoard;

		/*!
		\brief
		Constructor
		*/
		FalagardChatBoard(const String& type, const String& name);

		/*!
		\brief
		Destructor
		*/
		~FalagardChatBoard();

		/*!
		\brief
		Recaculate the window size.
		*/
		virtual float			resizeSelf(float maxWidth);

		bool					IsPointOnInfoItem( const Point& pt );

		void					appendInfoItem(int nID, const String32& strDisplay, int nLength, int nTalkerLength );
		void					appendNormalCharacter(const utf32& code);
		void					clearInfoItem();
		void					clearKillTime(){d_killTime=0.0f;}

		void					setBoardHollow(const bool hollow) { d_boardHollow = hollow; }
		void					setChatBoardString(const String32& strTxt, int nTalkerLength=0);
		void					setKillTime(const float elapsed){d_killTime=elapsed;}
		float					getKillTime() const{return d_killTime;}

		void					setmsgID(const int msgid){d_msgID=msgid;}
		int						getmsgID() const{return d_msgID;}
	protected:
		/*************************************************************************
		Implementation of abstract methods in Window
		*************************************************************************/
		virtual void			onMouseButtonUp(MouseEventArgs& e);
		virtual void			onMouseMove(MouseEventArgs& e);
		virtual void			onMouseLeaves(MouseEventArgs& e);
		virtual void			populateRenderCache();
		virtual void			updateSelf(float elapsed);

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
			if (class_name==(const utf8*)"FalagardChatBoard")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*************************************************************************
		Implementation Data
		*************************************************************************/
		struct InfoElement 
		{
			int nID;			//!< ID impl logic manager
			int nBeginIndex;	//!< Index begin at window text
			int nEndIndex;		//!< Index end at window text
			int nLength;		//!< InfoElement contents length

			InfoElement():nID(-1),nBeginIndex(-1),nEndIndex(-1),nLength(0){}

			bool isEqual(const InfoElement& il)
			{
				if(	nID == il.nID && 
					nBeginIndex == il.nBeginIndex && nEndIndex == il.nEndIndex && 
					nLength == il.nLength)
					return true;
				return false;
			}

			bool isEqual(const InfoElement* pil)
			{
				if(!pil) return false;
				if(	nID == pil->nID &&
					nBeginIndex == pil->nBeginIndex && nEndIndex == pil->nEndIndex &&
					nLength == pil->nLength)
					return true;
				return false;
			}
		};

		typedef	std::list<InfoElement>	InfoElementList;
		InfoElementList				d_infoLit;
		InfoElement*				d_pCurrentMoveInElement;

		float						d_textHeight;
		bool						d_needCacultextHeight;
		float						d_killTime;
		int							d_msgID;
		bool						d_boardHollow;

		int			getTextIndexFromPosition(const Point& pt) const;
	private:
		/*************************************************************************
		Static properties for the ChatBoard widget
		*************************************************************************/
		static FalagardChatBoardProperties::KillTimer		d_KillTimerProperty;

		/*************************************************************************
		Implementation methods
		*************************************************************************/

		/*!
		\brief
		Adds properties specific to the ChatBoard base class.

		\return
		Nothing.
		*/
		void addChatBoardProperties(void);
	};

	/*!
	\brief
	WindowFactory for FalagardChatBoard type Window objects.
	*/
	class FALAGARDBASE_API FalagardChatBoardFactory : public WindowFactory
	{
	public:
		FalagardChatBoardFactory(void) : WindowFactory(FalagardChatBoard::WidgetTypeName) { }
		~FalagardChatBoardFactory(void){}
		Window* createWindow(const String& name);
		void destroyWindow(Window* window);
	};

}

#endif