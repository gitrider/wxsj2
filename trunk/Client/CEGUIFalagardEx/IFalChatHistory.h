
namespace CEGUI
{

	class IFalagardChatHistory
	{
	public:
		/*!
		\brief
		Insert a chat string to chat board list.
		*/
		virtual Window*			insertChatString(int msgid,const String32& strChat, int nTalkerLength = 0, int nOp = -1) = 0;

		/*!
		\brief
		Remove all chat boards.
		*/
		virtual void			removeAllChatStrings() = 0;
		/*!
		\brief
		Scroll to ParentWindows's end
		*/
		virtual void			scrollToEnd() = 0;
		/*!
		\brief
		Extend a Clear Region in history window.
		*/
		virtual void			extendClearRegion() = 0;

		virtual void			subscribInfoItemClickEvent(Event::Subscriber subscriber) = 0;
		virtual void			subscribInfoItemDeleteEvent(Event::Subscriber subscriber) = 0;
		virtual void			subscribInfoItemMoveInEvent(Event::Subscriber subscriber) = 0;
		virtual void			subscribInfoItemMoveOutEvent(Event::Subscriber subscriber) = 0;
	};

}