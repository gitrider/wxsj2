#pragma once

#include "CEGUIWindow.h"
#include "FalModule.h"
#include "CEGUIProperty.h"
#include <map>
// Start of CEGUI namespace section
namespace CEGUI
{

	namespace IExpPropertyProperties
	{
		// 所有需要动态生成的控件类型
		class ChildItem: public Property
		{
			public:
			ChildItem() : Property(
				"ChildItem",
				"",
				"type:key name:buttonname")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	};
	class FALAGARDBASE_API IExpProperty : public Window
	{
	public:
		IExpProperty(const String& type, const String& name);
		~IExpProperty() {};
		void		setChildType( const String& szKey, const String& strName );
    public:
		static IExpPropertyProperties::ChildItem		d_ChildItemProperty;
	protected:
		typedef std::map< String, String > ChildItemType;
		ChildItemType				m_mapChildType;
    };
	
}
