
#include "FalActionButtonProperties.h"
#include "FalActionButton.h"
#include "CEGUIPropertyHelper.h"
#include <stdio.h>


namespace CEGUI
{
	namespace FalagardActionButtonProperties
	{

		//////////////////////////////////////////////////////////////////////////

		String Empty::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardActionButton*>(receiver)->isEmpty());
		}

		void Empty::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setEmpty(PropertyHelper::stringToBool(value));
		}

		//////////////////////////////////////////////////////////////////////////

		String Checked::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardActionButton*>(receiver)->isChecked());
		}

		void Checked::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setChecked(PropertyHelper::stringToBool(value));
		}

		//////////////////////////////////////////////////////////////////////////

		String Default::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardActionButton*>(receiver)->isDefault());
		}

		void Default::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setDefault(PropertyHelper::stringToBool(value));
		}

		//////////////////////////////////////////////////////////////////////////

		String Gloom::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardActionButton*>(receiver)->isGloom());
		}

		void Gloom::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setGloom(PropertyHelper::stringToBool(value));
		}
		//////////////////////////////////////////////////////////////////////////

		void CornerChar::set(PropertyReceiver* receiver, const String& value)
		{
			char szPos[32]={0};
			char szChar[8]={0};

			if(!value.empty())
			{
				sscanf(value.c_str(), " %16s %3s", szPos, szChar);
				if(strcmp(szPos, "TopLeft")==0)
				{
					static_cast<FalagardActionButton*>(receiver)->setCornerChar(0, (utf8*)szChar);
				}
				else if(strcmp(szPos, "TopRight")==0)
				{
					static_cast<FalagardActionButton*>(receiver)->setCornerChar(1, (utf8*)szChar);
				}
				else if(strcmp(szPos, "BotLeft") == 0)
				{
					static_cast<FalagardActionButton*>(receiver)->setCornerChar(2, (utf8*)szChar);
				}
				else if(strcmp(szPos, "BotRight") == 0)
				{
					static_cast<FalagardActionButton*>(receiver)->setCornerChar(3, (utf8*)szChar);
				}
			}
		}

		String CornerChar::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		//////////////////////////////////////////////////////////////////////////

		String DraggingEnabled::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardActionButton*>(receiver)->isDraggingEnabled());
		}

		void DraggingEnabled::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setDraggingEnabled(PropertyHelper::stringToBool(value));
		}

		//////////////////////////////////////////////////////////////////////////

		String DragThreshold::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::floatToString(static_cast<const FalagardActionButton*>(receiver)->getPixelDragThreshold());
		}

		void DragThreshold::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setPixelDragThreshold(PropertyHelper::stringToFloat(value));
		}

		//////////////////////////////////////////////////////////////////////////

		String DragAcceptName::get(const PropertyReceiver* receiver) const
		{
			return (String)(static_cast<const FalagardActionButton*>(receiver)->getDragAcceptName());
		}

		void DragAcceptName::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setDragAcceptName(value);
		}

		//////////////////////////////////////////////////////////////////////////

		String FlashAnimate::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void FlashAnimate::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setFlashAnimate(value);
		}
		String BackImage::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void BackImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setBackImage(PropertyHelper::stringToImage(value));
		}

		String UseDefaultTooltip::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void UseDefaultTooltip::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardActionButton*>(receiver)->setUseDefaultTooltip(PropertyHelper::stringToBool(value));
		}
	}
}
