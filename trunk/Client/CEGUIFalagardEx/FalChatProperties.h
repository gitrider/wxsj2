#pragma once
#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Properties for the Falagard ListHeaderSegment
*/
	namespace FalagardChatProperties
	{
	/*!
	\brief
	Property to get/set empty status

	\par Usage:
		- Name: Empty
		- Format: "INT".

	*/
		// ����¼������
		class MaxLineNumber : public Property
		{
		public:
			MaxLineNumber() : Property(
				"MaxLineNumber",
				"Property to get/set empty status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class AddText : public Property
		{
		public:
			AddText() : Property(
				"AddText",
				"Property to get/set empty status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);

		};

		class TextClear : public Property
		{
		public:
			TextClear() : Property(
				"TextClear",
				"Property to get/set empty status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);

		};
		// ���¹���һ��
		class LineDown: public Property
		{
		public:
			LineDown() : Property(
				"LineDown",
				"Property to get/set empty status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);

		};
		// ���Ϲ���һ��
		class LineUp: public Property
		{
		public:
			LineUp() : Property(
				"LineUp",
				"Property to get/set empty status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);

		};



	}
}
