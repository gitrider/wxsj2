
namespace CEGUI
{
	class IFalagardComplexWindow
	{
	public:
		enum ChildType
		{
			CT_Text,
			CT_Option,
			CT_Item,
			CT_Action,
			CT_Money,
			CT_HyperLink,
			CT_ChatBoard,
		};

		//Add ChildElement (Text)
		virtual void			AddChildElement_Text(String32 strText, FontBase* font, TextFormatting typeset=WordWrapLeftAligned) = 0;

		//Add ChildElement (Option)
		virtual void			AddChildElement_Option(String32 strText,String32 strText2,String32 strText3, String32 strText4, FontBase* font=NULL) = 0;

		//Add ChildElement (Link)
		virtual void			AddChildElement_HyperLink( String32 strText, int nLinkID, bool bReArrange, FontBase* font=NULL) = 0;

		//Add ChildElement (Item)
		virtual void			AddChildElement_Item(String strIconImage, String32 strText, bool bRadio,bool bReArrange, FontBase* font=NULL) = 0;

		//Add ChildElement (Action)
		virtual Window*			AddChildElement_Action(String strIconImage, int ActionID, bool bRadio, bool bReArrange, FontBase* font=NULL) = 0;

		//Add ChildElement (Money)
		virtual void			AddChildElement_Money(int money, FontBase* font=NULL) = 0;

		//Add ChildElement (ChatBoard)
		virtual void			AddChildElement_ChatBoard(String32 strText, FontBase* font, TextFormatting typeset=WordWrapLeftAligned) = 0;

		//Clear
		virtual void			ClearAllElement(void) = 0;

		virtual void			PageEnd() = 0;

		virtual void			subscribInfoItemClickEvent(Event::Subscriber subscriber) = 0;
		virtual void			subscribInfoItemDeleteEvent(Event::Subscriber subscriber) = 0;
	};
}