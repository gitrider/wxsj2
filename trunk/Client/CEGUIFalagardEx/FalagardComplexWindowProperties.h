#ifndef _FalagardComplexWindowProperties_h_
#define _FalagardComplexWindowProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*!
	\brief
	Properties for the Falagard ListHeaderSegment
	*/
	namespace FalagardComplexWindowProperties
	{
		/*
		\brief
		Property to get/set Minimap name status

		\par Usage:
		- Name: Empty
		- Format: "aniamte name".

		*/
		// complexwindow中action button的size
		class ActionButtonSize: public Property
		{
		public:
			ActionButtonSize() : Property(
				"ActionButtonSize",
				"Property to get/set ComplexWindow name status.",
				"w:32 h:32",false)
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		// complexwindow中action button的distance
		class ActionButtonDistance: public Property
		{
		public:
			ActionButtonDistance() : Property(
				"ActionButtonDistance",
				"Property to get/set ComplexWindow name status.",
				"w:5 h:5",false)
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		// 控件的名字
		class QuestButtonImage: public Property
		{
		public:
			QuestButtonImage() : Property(
				"QuestButtonImage",
				"",
				"type:number name:buttonname",false)
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		// CoolDowntime for option button in complex window
		class OptionCooldownTime : public Property
		{
		public:
			OptionCooldownTime() : Property(
				"OptionCooldownTime",
				"",
				"",false)
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

	};
};

#endif  // end of guard _FalagardComplexWindowProperties_h_
