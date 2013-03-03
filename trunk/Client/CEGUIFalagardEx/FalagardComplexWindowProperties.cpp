
#include "FalagardComplexWindowProperties.h"
#include "falagardComplexWindow.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"
#include <stdio.h>


namespace CEGUI
{
	namespace FalagardComplexWindowProperties
	{

		String ActionButtonSize::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::sizeToString( static_cast<const FalagardComplexWindow*>(receiver)->getActionButtonSize() );
		}

		void ActionButtonSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardComplexWindow*>(receiver)->setActionButtonSize( PropertyHelper::stringToSize(value) );
		}

		String ActionButtonDistance::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::sizeToString( static_cast<const FalagardComplexWindow*>(receiver)->getActionButtonDistance() );
		}

		void ActionButtonDistance::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardComplexWindow*>(receiver)->setActionButtonDistance( PropertyHelper::stringToSize(value) );
		}


		String QuestButtonImage::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void QuestButtonImage::set(PropertyReceiver* receiver, const String& value)
		{
			int nIndex = -1;
			char szSet[128];
			char szImage[128];
			sprintf( szSet, "" );
			sprintf( szImage, "" );
			sscanf( value.c_str(), "index:%d set:%127s image:%127s", &nIndex, szSet, szImage);
			const Image* pImage= &ImagesetManager::getSingleton().getImageset( (utf8*)szSet )->getImage( (utf8*)szImage );
			static_cast<FalagardComplexWindow*>(receiver)->setActionButtonImage( nIndex, pImage );
		}

		/////////////////////////////////////////////////////////////////////////////	
		String OptionCooldownTime::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::floatToString( static_cast<const FalagardComplexWindow*>(receiver)->getOptionbuttonCooldownTime() );
		}

		void OptionCooldownTime::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardComplexWindow*>(receiver)->setOptionbuttonCooldownTime(PropertyHelper::stringToFloat(value));
		}
	};
}
