
#include "FalMinimapProperties.h"
#include "falagardMinimap.h"
#include "CEGUIPropertyHelper.h"
#include <stdio.h>


namespace CEGUI
{
	namespace FalagardMinimapProperties
	{

		String RenderSize::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::sizeToString( static_cast<const FalagardMiniMap*>(receiver)->getMinimapRenderSize() );
		}

		void RenderSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setMinimapRenderSize( PropertyHelper::stringToSize(value) );
		}

		
		String MinimapImageSet::get(const PropertyReceiver* receiver) const
		{
			return  static_cast<const FalagardMiniMap*>(receiver)->getImageset();
		}

		void MinimapImageSet::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setImageset( value );
		}

		String SetMinimapName::get(const PropertyReceiver* receiver) const
		{
			return  static_cast<const FalagardMiniMap*>(receiver)->getImageset();
		}

		void SetMinimapName::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setImagesetByFileName( value );
		}



		// ����npc��ͼ��ָ��
		String AnimyImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getAnimyImage());
		}

		void AnimyImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setAnimyImage(PropertyHelper::stringToImage(value));
		}

		// // ����npc��ͼ��ָ��
		String ExpNpcImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getExpNpcImage());
		}

		void ExpNpcImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setExpNpcImage(PropertyHelper::stringToImage(value));
		}
		// ���ѵ�ͼ��
		String TeammateImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getTeammateImage());
		}

		void TeammateImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setTeammateImage(PropertyHelper::stringToImage(value));
		}
		// ��ҵ�ͼ��
		String PlayerImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getPlayerImage());
		}

		void PlayerImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setPlayerImage(PropertyHelper::stringToImage(value));
		}
		// Obj��ͼ��
		String ObjImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getObjImage());
		}

		void ObjImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setObjImage(PropertyHelper::stringToImage(value));
		}
		// �Լ���ͼ��,����Ƕ���
		String MySelfImage::get(const PropertyReceiver* receiver) const
		{
			return static_cast<const FalagardMiniMap*>(receiver)->getMyselfImage();
		}

		void MySelfImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setMyselfImage( value );
		}

			// �Լ���ͼ��,����Ƕ���
		String OpenFlagType::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void OpenFlagType::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setEnabel( PropertyHelper::stringToInt( value ) );
		}

					// �Լ���ͼ��,����Ƕ���
		String CloseFlagType::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void CloseFlagType::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setDisable( PropertyHelper::stringToInt( value ) );
		}
		// �Ƿ�����һ������
		String IsFlagOpen::get(const PropertyReceiver* receiver) const
		{
			return  static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void IsFlagOpen::set(PropertyReceiver* receiver, const String& value)
		{
			
		}
		// �Ƿ�����һ������
		String ClearFlagList::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void ClearFlagList::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->clearFlagList();
		}






					// �Ƿ�����һ������
		String SceneSize::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void SceneSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->SetSceneSize( PropertyHelper::stringToSize( value ) );
		}


					// �Ƿ�����һ������
		String ScenePosImage::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void ScenePosImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setScenePosImage( PropertyHelper::stringToImage( value ) );
		}


					// �Ƿ�����һ������
		String GetCurCoordinate::get(const PropertyReceiver* receiver) const
		{
			return  PropertyHelper::pointToString( ( static_cast< const FalagardMiniMap*>(receiver) )->GetSceneCoordinate() );
		}

		void GetCurCoordinate::set(PropertyReceiver* receiver, const String& value)
		{
		}

					// �Ƿ�����һ������
		String ActiveImage::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void ActiveImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setActiveImage( value );
		}

					// �Ƿ�����һ������
		String PetImage::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void PetImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setPetmage( PropertyHelper::stringToImage( value ) );
		}

		String FlashImage::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void FlashImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setFlashImage( value );
		}
	};
}
