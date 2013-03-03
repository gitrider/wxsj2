#ifndef _FalImageListBoxPropertiesEx_h_
#define _FalImageListBoxPropertiesEx_h_

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
	namespace FalagardImageListBoxExProperties
	{
		/*!
		\brief
		Property to get/set empty status

		\par Usage:
			- Name: Empty
			- Format: "true|false".

		*/
		
		class SetStateImage : public Property
		{
		public:
			SetStateImage() : Property(
				"SetStateImage",
				"Property to get/set ImageNormal .",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class NormalImage : public Property
		{
		public:
			NormalImage() : Property(
				"NormalImage",
				"Property to get/set ImageNormal .",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		
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
		class SelectImage: public Property
		{
		public:
			SelectImage() : Property(
				"SelectImage",
				"Property to get/set Animate name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class HorverImage: public Property
		{
		public:
			HorverImage() : Property(
				"HorverImage",
				"Property to get/set Animate name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif  // end of guard _FalActionButtonProperties_h_
