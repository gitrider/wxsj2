#include "FalChatHistoryProperties.h"
#include "FalChatHistory.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	// Start of ChatHistoryProperties namespace section
	namespace FalagardChatHistoryProperties
	{
		//////////////////////////////////////////////////////////////////////////

		String ChatBoardNumber::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardChatHistory*>(receiver)->getChatBoardNumber());
		}

		void ChatBoardNumber::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatHistory*>(receiver)->setChatBoardNumber(PropertyHelper::stringToInt(value));
		}

		String BoardKillTimer::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardChatHistory*>(receiver)->getBoardKillTimer());
		}

		void BoardKillTimer::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatHistory*>(receiver)->setBoardKillTimer(PropertyHelper::stringToInt(value));
		}

		String ChatMouseHollow::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardChatHistory*>(receiver)->getChatMouseHollow());
		}

		void ChatMouseHollow::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatHistory*>(receiver)->setChatMouseHollow(PropertyHelper::stringToBool(value));
		}

	}
}