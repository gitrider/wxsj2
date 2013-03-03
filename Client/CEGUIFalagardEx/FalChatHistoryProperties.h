#ifndef _FalChatHistoryProperties_h_
#define _FalChatHistoryProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	namespace FalagardChatHistoryProperties
	{
		/*!
		\brief
		Property to get/set chatboard number

		\par Usage:
		- Name: ChatBoardNumber
		- Format: "[int]".

		*/
		class ChatBoardNumber : public Property
		{
		public:
			ChatBoardNumber() : Property(
				"ChatBoardNumber",
				"Property to get/set ChatBoard Number.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
		Property to get/set chatboar kill timer

		\par Usage:
		- Name: BoardKillTimer
		- Format: "[int]".

		*/
		class BoardKillTimer : public Property
		{
		public:
			BoardKillTimer() : Property(
				"BoardKillTimer",
				"Property to get/set BoardKillTimer Kill Timer.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
		Property to get/set ChatMouseHollow

		\par Usage:
		- Name: ChatMouseHollow
		- Format: "[String]".

		*/
		class ChatMouseHollow : public Property
		{
		public:
			ChatMouseHollow() : Property(
				"ChatMouseHollow",
				"Property to get/set ChatMouseHollow.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
}


#endif