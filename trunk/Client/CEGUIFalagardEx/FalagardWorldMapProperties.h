#pragma once
#include "CEGUIProperty.h"
namespace CEGUI
{
	namespace FalagardWorldMapProperties
	{
		class BackgroundName: public Property
		{
		public:
			BackgroundName() : Property(
				"BackgroundName",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class BackgroundSize: public Property
		{
		public:
			BackgroundSize() : Property(
				"BackgroundSize",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// ��ͨͼ��
		class CityImageNormal: public Property
		{
		public:
			CityImageNormal() : Property(
				"CityImageNormal",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// ����ͼ��
		class CityImageHover: public Property
		{
		public:
			CityImageHover() : Property(
				"CityImageHover",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// һ�����ѵ�ͼ��
		class MySelfImage: public Property
		{
		public:
			MySelfImage() : Property(
				"MySelfImage",
				"",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// һ�����ѵ�ͼ��
		class FriendImage1: public Property
		{
		public:
			FriendImage1() : Property(
				"FriendImage1",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// �������ѵ�ͼ��
		class FriendImage2: public Property
		{
		public:
			FriendImage2() : Property(
				"FriendImage2",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// �������ѵ�ͼ��
		class FriendImage3: public Property
		{
		public:
			FriendImage3() : Property(
				"FriendImage3",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// �ĸ����ѵ�ͼ��
		class FriendImage4: public Property
		{
		public:
			FriendImage4() : Property(
				"FriendImage4",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*----------------------------------------------------------------------------------
		// ������ѵ�ͼ��
		class FriendImage5: public Property
		{
		public:
			FriendImage5() : Property(
				"FriendImage5",
				"Property to get/set Minimap name status.",
				"type: set: image:")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	
	}
}

