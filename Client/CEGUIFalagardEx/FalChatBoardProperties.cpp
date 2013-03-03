
#include "FalChatBoardProperties.h"
#include "FalChatBoard.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	// Start of StaticTextProperties namespace section
	namespace FalagardChatBoardProperties
	{
		String KillTimer::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::floatToString(static_cast<const FalagardChatBoard*>(receiver)->getKillTime());
		}

		void KillTimer::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatBoard*>(receiver)->setKillTime(PropertyHelper::stringToFloat(value));
		}
	}
}
