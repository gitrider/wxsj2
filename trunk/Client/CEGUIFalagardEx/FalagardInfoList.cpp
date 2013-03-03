#include ".\falagardinfolist.h"
#include "ceguipropertyhelper.h"
namespace CEGUI
{

	/*----------------------------------------------------------------------*/
	const utf8 FalagardInfoList::WidgetTypeName[] = "Falagard/InfoList";

	FalagardInfoListProperties::FadeSpeed	FalagardInfoList::d_FadeSpeedProperty;
	FalagardInfoListProperties::Life		FalagardInfoList::d_LifeProperty;

	// 得意自动删除得列表框
	FalagardInfoItem::FalagardInfoItem(const String32& text, uint item_id , void* item_data , bool disabled , bool auto_delete ):
	ListboxTextItem( text, item_id, item_data, disabled, auto_delete )
	{
		m_fCurAlpha = 1;
	}
	bool FalagardInfoItem::update( float elapsed )
	{
		m_nTimer -= int( elapsed * 1000 ); // 生存周期
		if( m_nTimer < 0 )
		{
			m_fCurAlpha -= float( m_nFadeSpeed * elapsed ) / 255.f;
			if( m_fCurAlpha < 0 )
				return false;
		}
		return true;
	}
	void FalagardInfoItem::draw(RenderCache& cache,const Rect& targetRect, float zBase,  float alpha, const Rect* clipper) const
	{

		if (d_selected && d_selectBrush != 0)
		{
			cache.cacheImage(*d_selectBrush, targetRect, zBase, getModulateAlphaColourRect(d_selectCols, alpha), clipper);
		}

		const FontBase* font = getFont();

		if (font)
		{
			Rect finalPos(targetRect);
			//			finalPos.d_top -= (FontBase::LineSpace) * 0.5f;
			cache.cacheText(getOwnerWindow(), d_itemText, font, Centred, finalPos, zBase, getModulateAlphaColourRect(d_textCols, m_fCurAlpha), clipper);
		}
	}

	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
	FalagardInfoList::FalagardInfoList(const String& type, const String& name) :
	FalagardListbox(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardInfoList )
			CEGUI_ADD_STATICPROPERTY( &d_FadeSpeedProperty  );
		CEGUI_ADD_STATICPROPERTY( &d_LifeProperty  );
		CEGUI_END_ADD_STATICPROPERTY

			m_nMaxInfoNumber = 5;
	}

	FalagardInfoList::~FalagardInfoList(void)
	{
	}
	void FalagardInfoList::NormalizeItemNumber()
	{
		if( (int)getItemCount() < m_nMaxInfoNumber )
			return;
		for (size_t i = 0; i < getItemCount() - m_nMaxInfoNumber; i ++ ) 
			removeItem( d_listItems[ 0 ] );

	}
	void FalagardInfoList::updateSelf( float elapsed )
	{
		FalagardListbox::updateSelf( elapsed );

		for (size_t i = 0; i < getItemCount(); ++i ) 
		{
			// if item is supposed to be deleted by us
			FalagardInfoItem* pItem = (FalagardInfoItem*)d_listItems[i]; 
			if ( pItem->update( elapsed ) == false )
			{
				// clean up this item.
				removeItem( d_listItems[ i ] );
			}

		}
		int x=(int)getItemCount();
		if( x == 0)
			setVisible(false);
		else
			setVisible(true);
		requestRedraw();
	}

	void FalagardInfoList::RemoveAllInfo(void)
	{
		resetList_impl();
		//for (size_t i = 0; i < getItemCount(); ++i ) 
		//{
		//	// if item is supposed to be deleted by us
		//	FalagardInfoItem* pItem = (FalagardInfoItem*)d_listItems[i]; 
		//		// clean up this item.
		//		removeItem( d_listItems[ i ] );

		//}
	}
	void FalagardInfoList::AddInfo(const String32& strInfo)
	{
		FalagardInfoItem* pItem = new FalagardInfoItem(strInfo);
		if( pItem)
		{
			pItem->setFadeSpeed( getFadeSpeed());
			pItem->setTimer( getLife() );
			addItem( pItem );
			NormalizeItemNumber();
			setVisible(true);
		}
	}

	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
	// FalagardWorldMapFactory
	Window* FalagardInfoListFactory::createWindow(const String& name)
	{
		return new FalagardInfoList(d_type, name);
	}

	void FalagardInfoListFactory::destroyWindow(Window* window)
	{
		delete window;
	}

	/*----------------------------------------------------------------------*/
	namespace FalagardInfoListProperties
	{
		//*----------------------------------------------------------------------
		String Life::get(const PropertyReceiver* receiver) const
		{
			return  PropertyHelper::intToString( static_cast< const FalagardInfoList*>(receiver)->getLife() );
		}

		void Life::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardInfoList*>(receiver)->setLife( PropertyHelper::stringToInt( value ) );
		};

		//*----------------------------------------------------------------------
		String FadeSpeed::get(const PropertyReceiver* receiver) const
		{
			return  PropertyHelper::intToString( static_cast< const FalagardInfoList*>(receiver)->getFadeSpeed() );
		}

		void FadeSpeed::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardInfoList*>(receiver)->setFadeSpeed( PropertyHelper::stringToInt( value ) );
		};

		//*----------------------------------------------------------------------
		String MaxInfo::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void MaxInfo::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardInfoList*>(receiver)->setMaxInfo( PropertyHelper::stringToInt( value ) );
		};

	};
};