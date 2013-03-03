#ifndef _FalImageListBoxProperties_h_
#define _FalImageListBoxProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	// ��ͼ���list������
	// ImageNormal	ͼ��
	// ImageHorver	����
	// ImageChecked ѡ�е�
	// ImageDisable �ر�
	// Animate ����ͼ��
	// AutoScale �Ƿ�����ͼ�굽��ǰ���ʵĸ߶�

/*!
\brief
    Properties for the Falagard ListHeaderSegment
*/
	namespace FalagardImageListBoxProperties
	{
		/*!
		\brief
		Property to get/set empty status

		\par Usage:
			- Name: Empty
			- Format: "true|false".

		*/
		class ImageNormal : public Property
		{
		public:
			ImageNormal() : Property(
				"ImageNormal",
				"Property to get/set ImageNormal .",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class ImageHorver : public Property
		{
		public:
			ImageHorver() : Property(
				"ImageHorver",
				"Property to get/set ImageNormal .",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		
		class ImageChecked : public Property
		{
		public:
			ImageChecked() : Property(
				"ImageChecked",
				"Property to get/set ImageNormal .",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class ImageDisable: public Property
		{
		public:
			ImageDisable() : Property(
				"ImageDisable",
				"Property to get/set ImageNormal .",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		/*!
		\brief
		Property to get/set empty status

		\par Usage:
			- Name: Empty
			- Format: "true|false".

		*/
		class AutoScale : public Property
		{
		public:
			AutoScale() : Property(
				"AutoScale",
				"Property to get/set checked status.",
				"Trye")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
		Property to get/set Animate name status

		\par Usage:
			- Name: Empty
			- Format: "aniamte name".

		*/
		class AddItem : public Property
		{
		public:
			AddItem() : Property(
				"AddItem",
				"Property to get/set Animate name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif  // end of guard _FalActionButtonProperties_h_
