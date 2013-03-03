#ifndef _FalIMEEditBoxProperties_h_
#define _FalIMEEditBoxProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*!
	\brief
	Properties for the Falagard IMEEditBox
	*/
	namespace FalagardIMEEditBoxProperties
	{

		/*!
		\brief
		Property to access the default editbox property.

		\par Usage:
		- Name: DefaultEditBox
		- Format: "bool".
		*/
		class DefaultEditBox : public Property
		{
		public:
			DefaultEditBox() : Property(
				"DefaultEditBox",
				"Property to get/set the default editbox property.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
		Property to clear the text's offset.
		*/
		class ClearOffset	: public Property
		{
		public:
			ClearOffset()	: Property(
				"ClearOffset",
				"Property to set clear offset flag of editbox's property.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
		Property to set max itemelement number.
		*/
		class ElementNum	: public Property
		{
		public:
			ElementNum()	: Property(
				"ElementNum",
				"Property to set max itemelement number of editbox's property.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		class DisableCopyPaste : public Property
		{
		public:
			DisableCopyPaste() : Property(
				"DisableCopyPaste",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
		class SoftKeyOnly : public Property
		{
		public:
			SoftKeyOnly() : Property(
				"SoftKeyOnly",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif