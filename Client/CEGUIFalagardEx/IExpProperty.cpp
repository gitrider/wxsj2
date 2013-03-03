
#include "IExpProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	namespace IExpPropertyProperties
	{
		String ChildItem::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void ChildItem::set(PropertyReceiver* receiver, const String& value)
		{
			char szKey[128];
			char szName[128];
			sscanf( value.c_str(), "key:%127s name:%127s", szKey, szName);
			static_cast<IExpProperty*>(receiver)->setChildType( szKey, (utf8*)szName );
		}


	};


	IExpPropertyProperties::ChildItem		IExpProperty::d_ChildItemProperty;

	IExpProperty::IExpProperty(const String& type, const String& name) :
        Window(type, name)
    {
		CEGUI_START_ADD_STATICPROPERTY( IExpProperty )
		   CEGUI_ADD_STATICPROPERTY( &d_ChildItemProperty  );
		   
		CEGUI_END_ADD_STATICPROPERTY

    }
	void	IExpProperty::setChildType( const String& szKey, const String& strName )
	{
		m_mapChildType[szKey] = strName;
	}
	
};