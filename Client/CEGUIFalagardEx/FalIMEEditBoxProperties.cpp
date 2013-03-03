#include "FalIMEEditBoxProperties.h"
#include "FalIMEEditBox.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIIme.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	// Start of FalagardIMEEditBoxProperties namespace section
	namespace FalagardIMEEditBoxProperties
	{
		////////////////////////////////////////////////////////////////////////

		String	DefaultEditBox::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(Ime::GetActiveIMEEditBox() == static_cast<const Window*>(receiver));
		}

		void DefaultEditBox::set(PropertyReceiver* receiver, const String& value)
		{
			Window* old = static_cast<Window*>(Ime::GetActiveIMEEditBox());

			Ime::SetActiveIMEEditBox(static_cast<Window*>(receiver), PropertyHelper::stringToBool(value));

			if(Ime::GetActiveIMEEditBox() == receiver)
			{
				static_cast<FalagardIMEEditBox*>(Ime::GetActiveIMEEditBox())->resetCaretBlink();
			}
			if(old)
			{
				old->requestRedraw();
			}
		}

		String ClearOffset::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardIMEEditBox*>(receiver)->getClearOffset());
		}

		void ClearOffset::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardIMEEditBox*>(receiver)->setClearOffset(PropertyHelper::stringToBool(value));
		}

		String ElementNum::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardIMEEditBox*>(receiver)->getMaxItemElement());
		}

		void ElementNum::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardIMEEditBox*>(receiver)->setMaxItemElement(PropertyHelper::stringToInt(value));
		}

		////////////////////////////////////////////////////////////////////////////
		String	DisableCopyPaste::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardIMEEditBox*>(receiver)->getCopyPasteDisable());
		}

		void DisableCopyPaste::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardIMEEditBox*>(receiver)->setCopyPasteDisable(PropertyHelper::stringToBool(value));
		}

		////////////////////////////////////////////////////////////////////////////
		String	SoftKeyOnly::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void SoftKeyOnly::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardIMEEditBox*>(receiver)->setSoftKeyOnly(PropertyHelper::stringToBool(value));
		}
	}
}