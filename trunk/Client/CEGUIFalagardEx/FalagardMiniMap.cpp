#include ".\falagardminimap.h"
#include "ceguianimate.h"
#include "ceguiimagesetmanager.h"
#include "ceguiimageset.h"
#include "ceguianimatemanager.h"
#include "ceguitexture.h"
#include "elements\ceguitooltip.h"
#include "math.h"
#include "windows.h"

namespace CEGUI
{
	// �Զ���������ʱ��
	const int   MAX_NUMBER = 16;
	const FLOAT MAX_RETURN_TIMER = 5;

	const utf8 FalagardMiniMap::WidgetTypeName[] = "Falagard/Minimap";

	FalagardMinimapProperties::RenderSize		FalagardMiniMap::d_RenderSizeProperty;
	FalagardMinimapProperties::SceneSize		FalagardMiniMap::d_SceneSizeProperty;
	FalagardMinimapProperties::MinimapImageSet	FalagardMiniMap::d_ImagesetProperty;
	FalagardMinimapProperties::SetMinimapName	FalagardMiniMap::d_MapFileNameProperty;
	FalagardMinimapProperties::PlayerImage		FalagardMiniMap::d_PlayerImageProperty;
	FalagardMinimapProperties::ObjImage			FalagardMiniMap::d_ObjImageProperty;
	FalagardMinimapProperties::MySelfImage		FalagardMiniMap::d_MySelfImageProperty;
	FalagardMinimapProperties::TeammateImage	FalagardMiniMap::d_TeammateImageProperty;
	FalagardMinimapProperties::ExpNpcImage		FalagardMiniMap::d_ExpNpcImageProperty;
	FalagardMinimapProperties::AnimyImage		FalagardMiniMap::d_AnimyImageProperty;
	FalagardMinimapProperties::ScenePosImage	FalagardMiniMap::d_ScenePosImageProperty;
	FalagardMinimapProperties::PetImage			FalagardMiniMap::d_PetImageProperty;

	FalagardMinimapProperties::OpenFlagType		FalagardMiniMap::d_OpenFlagTypeProperty;
	FalagardMinimapProperties::CloseFlagType	FalagardMiniMap::d_CloseFlagTypeProperty;
	FalagardMinimapProperties::IsFlagOpen		FalagardMiniMap::d_IsFlagOpenProperty;
	FalagardMinimapProperties::ClearFlagList	FalagardMiniMap::d_ClearFlagListProperty;
	FalagardMinimapProperties::GetCurCoordinate	FalagardMiniMap::d_GetCurCoordinateProperty;
	FalagardMinimapProperties::ActiveImage		FalagardMiniMap::d_ActiveImageProperty;
	FalagardMinimapProperties::FlashImage		FalagardMiniMap::d_FlashImageProperty;

	FalagardMiniMap::FalagardMiniMap(const String& type, const String& name) :
		FalagardStaticImage(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardMiniMap )
		   CEGUI_ADD_STATICPROPERTY( &d_RenderSizeProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_SceneSizeProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_ImagesetProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_MapFileNameProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_PlayerImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_ObjImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_MySelfImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_TeammateImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_ExpNpcImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_ScenePosImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_AnimyImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_OpenFlagTypeProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_CloseFlagTypeProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_IsFlagOpenProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_ClearFlagListProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_GetCurCoordinateProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_ActiveImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_PetImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_FlashImageProperty );
		CEGUI_END_ADD_STATICPROPERTY

		d_Imageset = NULL;


		d_fresh = true;
		m_bDraping = false;
		m_bMouseInWindow = false;
		m_bSuperlook = false;
		d_CenterPos = Point( 256, 256 );
		m_SceneSize = Size( 128,128 );
		d_RenderSize = Size( 50,50 );
		d_ImageSize = Size( 128, 128 );
		d_ReturnTimer = 0;

		m_aAnimyPos.resize( MAX_NUMBER );
		m_aExpNpcPos.resize( MAX_NUMBER );
		m_aTeammatePos.resize( MAX_NUMBER );
		m_aPlayerPos.resize( MAX_NUMBER );
		m_aExpObjPos.resize( MAX_NUMBER );
		m_aActivePos.resize( MAX_NUMBER );
		m_aScenePos.resize( MAX_NUMBER );
		m_aPetPos.resize( MAX_NUMBER );
		d_MyselfImage = NULL;
	}

	FalagardMiniMap::~FalagardMiniMap(void)
	{
	}

	void FalagardMiniMap::dataRest()
	{
		for( int i = 0; i < IMAGE_TYPE_NUM; i ++ )
			m_aPointerTimer[ i ] = 0;
	}
	
	const int FalagardMiniMap::getPlayerDirection() const
	{
		return d_PlayerDirect;
	}

	void FalagardMiniMap::setPlayerDirection( int nDir )
	{ 
		d_PlayerDirect = ( nDir + 180 ) % 360;
		if( d_PlayerDirect < 0 )
			d_PlayerDirect += 360;
		d_fresh = true;
		
	}


	void FalagardMiniMap::addAnimyPos( const Point& pos, const String& name )
	{
		MINIMAP_FLAG_DATA data;
		data.pos.d_x = pos.d_x / d_SceneToImageScale.d_width;
		data.pos.d_y = pos.d_y / d_SceneToImageScale.d_height;
		data.name = name;
		m_aAnimyPos.push_back( data );
	}

	void FalagardMiniMap::addExpNpcPos( const Point& pos, const String& name )
	{
		MINIMAP_FLAG_DATA data;
		data.pos.d_x = pos.d_x / d_SceneToImageScale.d_width;
		data.pos.d_y = pos.d_y / d_SceneToImageScale.d_height;
		data.name = name;
		m_aExpNpcPos.push_back( data );
	}

	void FalagardMiniMap::addTeammatePos( const Point& pos, const String& name )
	{
		MINIMAP_FLAG_DATA data;
		data.pos.d_x = pos.d_x / d_SceneToImageScale.d_width;
		data.pos.d_y = pos.d_y / d_SceneToImageScale.d_height;
		data.name = name;
		m_aTeammatePos.push_back( data );
		
	}

	void FalagardMiniMap::addPlayerPos( const Point& pos, const String& name )
	{
		MINIMAP_FLAG_DATA data;
		data.pos.d_x = pos.d_x / d_SceneToImageScale.d_width;
		data.pos.d_y = pos.d_y / d_SceneToImageScale.d_height;
		data.name = name;
		m_aPlayerPos.push_back( data );
	}

	void FalagardMiniMap::addExpObjPos( const Point& pos, const String& name )
	{
		MINIMAP_FLAG_DATA data;
		data.pos.d_x = pos.d_x / d_SceneToImageScale.d_width;
		data.pos.d_y = pos.d_y / d_SceneToImageScale.d_height;
		data.name = name;
		m_aExpObjPos.push_back( data );
	}
	void FalagardMiniMap::addScenePos( const Point& pos, const String& name )
	{
		MINIMAP_FLAG_DATA data;
		data.pos.d_x = pos.d_x / d_SceneToImageScale.d_width;
		data.pos.d_y = pos.d_y / d_SceneToImageScale.d_height;
		data.name = name;
		m_aScenePos.push_back( data );
	}
	void FalagardMiniMap::addActiveObjPos( const Point& pos, const String& name )
	{
		MINIMAP_FLAG_DATA data;
		data.pos.d_x = pos.d_x / d_SceneToImageScale.d_width;
		data.pos.d_y = pos.d_y / d_SceneToImageScale.d_height;
		data.name = name;
		m_aActivePos.push_back( data );
	}
	void FalagardMiniMap::addPetPos( const Point& pos, const String& name )
	{
		MINIMAP_FLAG_DATA data;
		data.pos.d_x = pos.d_x / d_SceneToImageScale.d_width;
		data.pos.d_y = pos.d_y / d_SceneToImageScale.d_height;
		data.name = name;
		m_aPetPos.push_back( data );
	}
	void FalagardMiniMap::populateRenderCache()
	{

	/*	char szInfo[ 128 ];
		sprintf( szInfo,"AnimyPos Number=%d\nExpNpcPos Number=%d\nTeammatePos Number=%d\nExpObj Number=%d\nScenePos Number=%d\nActivePos Number=%d",
			int( m_aAnimyPos.size() ), int( m_aExpNpcPos.size() ), int( m_aTeammatePos.size() ), int( m_aPlayerPos.size() ),int( m_aExpObjPos.size() ),int( m_aScenePos.size() ), int( m_aActivePos.size() ) );
		OutputDebugString( szInfo );*/
		//NormalArea();
		//if( d_MapImage )
		//	d_MapImage->setSourceTextureArea( d_RenderRect );
		//d_image.draw( d_renderCache ); // ������
		//// ������
		//drawImagePos( m_aAnimyPos,		IMAGE_TYPE_Animy );
		//// ������npc
		//drawImagePos( m_aExpNpcPos,	IMAGE_TYPE_ExpNpc );
		//// ������
		//drawImagePos( m_aTeammatePos,	IMAGE_TYPE_Team );
		//// ��������
		//drawImagePos( m_aPlayerPos,	IMAGE_TYPE_Player );
		//// ��������Ʒ
		//drawImagePos( m_aExpObjPos,	IMAGE_TYPE_ExpObj );
		//// ��������ת��
		//drawImagePos( m_aScenePos,	IMAGE_TYPE_ScenePos );
		//// ������ͷ
		//drawImagePos( m_aActivePos,	IMAGE_TYPE_Active );

		//drawImageFlash();
		//// ������
		//drawImagePos( m_aPetPos,	IMAGE_TYPE_Pet );
		//// ���Լ�
		//if( d_MyselfImage && IsPosVisible( d_MyselfPos ) )
		//{
		//	d_renderCache.cacheImage( *d_MyselfImage, d_PlayerRect, 0, ColourRect( colour( 1,1,1) ) );
		//}
	}
	void FalagardMiniMap::setEnabel( int nIndex )
	{
		d_displayFlag[ nIndex ] = true;
	}
	void FalagardMiniMap::setDisable( int nIndex )
	{
		d_displayFlag[ nIndex ] = false;
	}
	const String FalagardMiniMap::getEnableType() const
	{
		char str[ 128 ];
//		sprintf( str, "Animy:%d Expnpc:%d Team:%d Player:%d ExpObj:%d", d_displayFlag[ IMAGE_TYPE_Animy ],
		sprintf( str, "Animy:%d Expnpc:%d Team:%d ExpObj:%d", 
			d_displayFlag[ IMAGE_TYPE_Animy ],
			d_displayFlag[ IMAGE_TYPE_ExpNpc ],
			d_displayFlag[ IMAGE_TYPE_Team ],
//			d_displayFlag[ IMAGE_TYPE_Player ],	//080423
			d_displayFlag[ IMAGE_TYPE_ExpObj ]);
		return String( str );
	}
	void  FalagardMiniMap::clearFlagList()
	{
		dataRest();
	}
	void FalagardMiniMap::updateSelf(float elapsed)
	{
		// ����ɼ��Ż����
		if( !m_bMouseInWindow )
			m_ptCoordinate = ImagePosToScenePos( d_MyselfPos );
		if( d_ReturnTimer > 0 && m_bSuperlook ) // ��������ѷ��ˣ��Ϳ�ʼ��ʱ
		{
			d_ReturnTimer -= elapsed;
			if( d_ReturnTimer < 0 )
			{
				d_ReturnTimer = 0;
				m_bSuperlook = false;
				d_CenterPos = d_MyselfPos;
				d_fresh = true;
			}
		}
		if( d_Imageset && d_fresh )
		{
			
			Rect rect;
			Point pos;
			if( IsPosVisible( d_MyselfPos ) )
			{
				Animate* pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_MyselfAnimate );
				if( pAnimate )
				{
					int nCurFrame = ( d_PlayerDirect * pAnimate->getFrameNumber() ) / 360;
					d_MyselfImage = (Image*)( pAnimate->getFrame( nCurFrame ) );
					// �õ����ĵ�
					NormalArea();
					pos = ImagePosToWindowPos( d_MyselfPos );
					// ���㷶Χ
					d_PlayerRect.d_left   = pos.d_x - d_MyselfImage->getSourceTextureArea().getWidth() / 2;
					d_PlayerRect.d_right  = pos.d_x + d_MyselfImage->getSourceTextureArea().getWidth() / 2;
					d_PlayerRect.d_top    = pos.d_y - d_MyselfImage->getSourceTextureArea().getHeight() / 2;
					d_PlayerRect.d_bottom = pos.d_y + d_MyselfImage->getSourceTextureArea().getHeight() / 2;
				}
			}

			d_fresh = false;
			requestRedraw();
		}
	}
	const String& FalagardMiniMap::getImageset() const
	{
		return d_ImagetSetName;
	}

	void FalagardMiniMap::NormalArea()
	{
		// ���ĵ㲻�ܳ�����ʾ��Χ
		if( d_CenterPos.d_x < d_RenderSize.d_width / 2 )
			d_CenterPos.d_x = d_RenderSize.d_width / 2;
		if( d_CenterPos.d_x > d_ImageSize.d_width - d_RenderSize.d_width / 2 )
			d_CenterPos.d_x = d_ImageSize.d_width - d_RenderSize.d_width / 2 ;
		if( d_CenterPos.d_y < d_RenderSize.d_height / 2 )
			d_CenterPos.d_y = d_RenderSize.d_height / 2;
		if( d_CenterPos.d_y > d_ImageSize.d_height - d_RenderSize.d_height / 2 )
			d_CenterPos.d_y = d_ImageSize.d_height - d_RenderSize.d_height / 2;

		d_RenderRect = Rect( 
			d_CenterPos.d_x - d_RenderSize.d_width / 2,
			d_CenterPos.d_y	- d_RenderSize.d_height / 2,
			d_CenterPos.d_x + d_RenderSize.d_width / 2,
			d_CenterPos.d_y	+ d_RenderSize.d_height / 2
			);
	}
	void FalagardMiniMap::setImageset( const String& name )
	{
		if( d_ImagetSetName == name ) // ���ֲ���ʲô������
			return;
		d_ImagetSetName = name;
		d_Imageset = (Imageset*)ImagesetManager::getSingleton().getImageset( name );
		NormalArea();
		Point	offset(0,0);
		if( !d_Imageset->isImageDefined( "view_Minimap_mode" ) )
		{
			 d_Imageset->defineImage( "view_Minimap_mode", d_RenderRect,offset );
		}
		d_MapImage = (Image*)(&d_Imageset->getImage( "view_Minimap_mode" ));
		if( d_MapImage )
		{
			d_image.setImage( d_MapImage );
		}
		d_fresh = true;
	}
	// ��ͼƬ�ļ�������imageset
	void FalagardMiniMap::setImagesetByFileName( const String& name )
	{
		// ����Ѿ����Ÿ�������
		Imageset* pSet;
		if( ImagesetManager::getSingleton().isImagesetPresent( name  ) )
		{
			pSet = ImagesetManager::getSingleton().getImageset( name );
		}
		// ����һ���µ�imageset
		else
		{
			pSet = ImagesetManager::getSingleton().createImagesetFromImageFile( name, name, "Scene" );
		}
		if( pSet == NULL )
			return;
		if( pSet->getTexture() )
		{
			d_ImageSize.d_width = pSet->getTexture()->getWidth();
			d_ImageSize.d_height = pSet->getTexture()->getHeight();
		}
		// ���㳡�������ųߴ�
		d_SceneToImageScale.d_width = m_SceneSize.d_width / d_ImageSize.d_width;
		d_SceneToImageScale.d_height =  m_SceneSize.d_height / d_ImageSize.d_height;
		setImageset( name );
		
	}
	// �����������λ�á�
	void FalagardMiniMap::setMinimapCenter( const Point& pos )
	{
		// ���λ�ò���
		int nX = int( pos.d_x / d_SceneToImageScale.d_width );
		int nY = int( pos.d_y / d_SceneToImageScale.d_height );
		if( d_MyselfPos.d_x == float( nX ) && d_MyselfPos.d_y == float( nY ) )
			return;
		d_MyselfPos.d_x = float( nX ); 
		d_MyselfPos.d_y = float( nY );
		if( m_bDraping ) // ��������ѷţ��Ͳ��ı����ĵ�λ��
		{
			d_OldCenterPos = d_MyselfPos;
		}
		else // ������ٿ���ĵط�,�ͱ����
		{
			d_CenterPos = d_MyselfPos;
		}
		d_fresh = true;
		requestRedraw();
	}
	const Point FalagardMiniMap::getMinimapCenter() const
	{
		return d_CenterPos;

	}
	void FalagardMiniMap::setMinimapRenderSize( const Size& size )
	{
		if( d_RenderSize == size )
			return;
		d_RenderSize = size;
		d_fresh = true;
		requestRedraw();
	}
	const Size FalagardMiniMap::getMinimapRenderSize() const 
	{
		return d_RenderSize;
	}

	void FalagardMiniMap::setActiveImage( const String& name )
	{
		d_ActiveAnimate = name;
	}


	void FalagardMiniMap::setScenePosImage( const Image* pImage )
	{
		d_ScenePosImage = pImage;
	}

	
	void FalagardMiniMap::setPlayerImage( const Image* pImage )
	{
		d_PlayerImage = pImage;
	}

	void FalagardMiniMap::setObjImage( const Image* pImage )
	{
		d_ExpObjImage = pImage;
	}

	void FalagardMiniMap::setMyselfImage( const String& name )
	{
		d_MyselfAnimate = name;
	}

	void FalagardMiniMap::setTeammateImage( const Image* pImage )
	{
		d_TeammateImage = pImage;
	}

	void FalagardMiniMap::setExpNpcImage( const Image* pImage )
	{
		d_ExpNpcImage = pImage;
	}

	void FalagardMiniMap::setAnimyImage( const Image* pImage )
	{
		d_AnimyImage = pImage;
	}
	void FalagardMiniMap::setPetmage( const Image* pImage )
	{
		d_PetImage = pImage;
	}
	void FalagardMiniMap::setFlashImage( const String& name )
	{
		d_FlashAnimate = name;
	}

	const String FalagardMiniMap::getFlashImage() const
	{
		return d_FlashAnimate;
	}
	const Image* FalagardMiniMap::getPlayerImage() const
	{
		return d_PlayerImage;
	}

	const Image* FalagardMiniMap::getObjImage() const 
	{
		return d_ExpObjImage;
	}

	const String FalagardMiniMap::getMyselfImage() const
	{
		return d_MyselfAnimate;
	}
	const Image* FalagardMiniMap::getScenePosImage() const 
	{
		return d_ScenePosImage;
	}
	const Image* FalagardMiniMap::getTeammateImage() const 
	{
		return d_TeammateImage;
	}

	const Image* FalagardMiniMap::getExpNpcImage() const 
	{
		return d_ExpNpcImage;
	}
	
	const Image* FalagardMiniMap::getPetPosImage()const
	{
		return d_PetImage;
	}

	const Image* FalagardMiniMap::getAnimyImage() const 
	{
		return d_AnimyImage;
	}

	bool  FalagardMiniMap::IsPosVisible( Point& pos )
	{
		Point posZero = GetCurrentZero();
		if( pos.d_x < posZero.d_x || pos.d_x > posZero.d_x + d_RenderSize.d_width || pos.d_y < posZero.d_y || pos.d_y > posZero.d_y + d_RenderSize.d_height ) // ������ʾ��Χ
			return false;
		return true;
	}
	void FalagardMiniMap::drawImageFlash()
	{
		MINIMAP_FLAG_DATA *it;
		ColourRect color(colour( 1.0f, 1.0f, 1.0f ));
		static int time_now = 0;
		time_now += (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);
		for( int i = 0; i < m_aPointerTimer[ IMAGE_TYPE_Flash ]; i ++ )
		{
			it = &m_aFlashPos[ i ];
			if( !IsPosVisible( it->pos ) ) // ������ɼ��Ͳ�����
			{
				continue;
			}
			// �õ���������
			Point curPos = ImagePosToWindowPos( it->pos );
			Animate* pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_FlashAnimate );
			if( pAnimate )
			{
				Image* pImage = (Image*)( pAnimate->getFrame( time_now, -1 ) );
				// ���㷶Χ
				Size size = Size( pImage->getSourceTextureArea().getWidth(),pImage->getSourceTextureArea().getHeight() );  
				Rect rect;
				rect.d_left   = curPos.d_x - size.d_width / 2;
				rect.d_right  = curPos.d_x + size.d_width / 2;
				rect.d_top    = curPos.d_y - size.d_height / 2;
				rect.d_bottom = curPos.d_y + size.d_height / 2;
				d_renderCache.cacheImage( *pImage, rect, 0, color);
			}
		}
	}
	void FalagardMiniMap::drawImagePos( RENDER_POS& pImageList, int nType )
	{
		MINIMAP_FLAG_DATA *it;
		ColourRect color(colour( 1.0f, 1.0f, 1.0f ));
		// �õ���ʾԭ���λ��
		Point curPos;
		for( int i = 0; i < m_aPointerTimer[ nType ]; i ++ )
		{
			it = &pImageList[ i ];
			if( !IsPosVisible( it->pos ) ) // ������ɼ��Ͳ�����
			{
				if( nType != IMAGE_TYPE_Active ) // ������Ƿ����ͷ���Ͳ�����
					continue;
			}
			else
			{
				if( nType == IMAGE_TYPE_Active )
					continue;
			}
			// �õ���������
			curPos = ImagePosToWindowPos( it->pos );
			Rect rect;
			Size size;
			const Image* pCurImage;
			switch( nType )
			{
			case IMAGE_TYPE_Animy:
				pCurImage = getAnimyImage(); 
				break;
			case IMAGE_TYPE_ExpNpc:
				pCurImage = getExpNpcImage();
				break;
			case IMAGE_TYPE_Team:
				pCurImage = getTeammateImage();
				break;
			//080423
//			case IMAGE_TYPE_Player:
//				pCurImage = getPlayerImage();
//				break;
			case IMAGE_TYPE_ExpObj:
				pCurImage = getObjImage();
				break;
			case IMAGE_TYPE_ScenePos:
				pCurImage = getScenePosImage();
				break;
			case IMAGE_TYPE_Active:
				{
					pCurImage = GetActiveArrowImage( it->pos );
					curPos = GetActiveArrowRenderPos( it->pos );
				}
				break;
			case IMAGE_TYPE_Pet:
				pCurImage = getPetPosImage();
				break;
			default:
				pCurImage = NULL;
				break;
			}
			if( pCurImage )
			{
				size = Size( pCurImage->getSourceTextureArea().getWidth(),pCurImage->getSourceTextureArea().getHeight() );  
				rect.d_left   = curPos.d_x - size.d_width / 2;
				rect.d_right  = curPos.d_x + size.d_width / 2;
				rect.d_top    = curPos.d_y - size.d_height / 2;
				rect.d_bottom = curPos.d_y + size.d_height / 2;
				d_renderCache.cacheImage( *pCurImage, rect, 0, color);
			}
		}
	}
	// ���㵱ǰ��꼤���tooltip
	void	FalagardMiniMap::SetMinimapToolTip( RENDER_POS& pImageList, Point& mousePos, const Image* image, int nType )
	{
		MINIMAP_FLAG_DATA*   it;
		Point curPos = ScreenPosToImagePos( mousePos );
		Rect  rect;
		for( int i = 0; i < m_aPointerTimer[ nType ]; i ++  )
		{
			it = &pImageList.at( i );
			if( IsPosVisible( it->pos ) ) // ����ɼ����ж�
			{
				rect.d_left		= it->pos.d_x - image->getWidth() / 2;
				rect.d_right	= rect.d_left + image->getWidth();
				rect.d_top		= it->pos.d_y - image->getHeight() / 2;
				rect.d_bottom	= rect.d_top + image->getWidth();
				if( rect.isPointInRect( curPos ) )
				{
					if( m_objTooltip.size() != 0 )
					{
						m_objTooltip += "\n";
					}
					m_objTooltip += it->name;
				}

			}
		}
	}
	// �õ���ͷ����ʾλ��
	Point FalagardMiniMap::GetActiveArrowRenderPos( Point& pos )
	{
		// �õ���������
		Point curPos = GetActiveArrowImagePos( pos );
		curPos = ImagePosToWindowPos( curPos );
		const Image* pImage = GetActiveArrowImage( pos );
		Rect  rect = getAbsoluteRect();
		if( curPos.d_x < ( rect.d_left + pImage->getSourceTextureArea().getWidth() / 2 ) )
			curPos.d_x = rect.d_left + pImage->getSourceTextureArea().getWidth() / 2;

		if( curPos.d_y < ( rect.d_top + pImage->getSourceTextureArea().getHeight() / 2 ) )
			curPos.d_y = rect.d_top + pImage->getSourceTextureArea().getHeight() / 2;
		if( curPos.d_x > ( rect.d_right - pImage->getSourceTextureArea().getWidth() / 2 ) )
			curPos.d_x = rect.d_right - pImage->getSourceTextureArea().getWidth() / 2 ;
		if( curPos.d_y > ( rect.d_bottom - pImage->getSourceTextureArea().getHeight() / 2 ) )
			curPos.d_y = rect.d_bottom - pImage->getSourceTextureArea().getHeight() / 2;
		return curPos;

	}
	// �õ���ͷ������λ��
	Point FalagardMiniMap::GetActiveArrowImagePos( Point& pos )
	{
		Point tmp;
		float fX = abs( pos.d_x - d_CenterPos.d_x );
		float fY = abs( pos.d_y - d_CenterPos.d_y );
		if( fX == 0 ) // �����ͬһy��
		{
			tmp.d_x = pos.d_x;
			if( pos.d_y > d_CenterPos.d_y )
				tmp.d_y = d_CenterPos.d_y + d_RenderSize.d_height / 2;
			else
				tmp.d_y = d_CenterPos.d_y - d_RenderSize.d_height / 2;
			return tmp;
		}

		if( fY == 0 )
		{
			tmp.d_y = pos.d_x;
			if( pos.d_x > d_CenterPos.d_x )
				tmp.d_x = d_CenterPos.d_y + d_RenderSize.d_height / 2;
			else
				tmp.d_x = d_CenterPos.d_y - d_RenderSize.d_height / 2;
			return tmp;
		}
		float fTan = fX / fY;
		if( fTan < 1 )
		{
			if( pos.d_x < d_CenterPos.d_x ) //  
			{
				if( pos.d_y > d_CenterPos.d_y )
				{
					tmp.d_x = d_CenterPos.d_x - d_RenderSize.d_height * fTan / 2;
					tmp.d_y = d_CenterPos.d_y + d_RenderSize.d_height / 2;
				}
				else 
				{
					tmp.d_x = d_CenterPos.d_x - d_RenderSize.d_height * fTan / 2 ;
					tmp.d_y = d_CenterPos.d_y - d_RenderSize.d_height / 2;
				}
			}
			else
			{
				if( pos.d_y > d_CenterPos.d_y )
				{
					tmp.d_x = d_CenterPos.d_x + d_RenderSize.d_height * fTan / 2;
					tmp.d_y = d_CenterPos.d_y + d_RenderSize.d_height / 2;
				}
				else
				{
					tmp.d_x = d_CenterPos.d_x + d_RenderSize.d_height * fTan / 2;
					tmp.d_y = d_CenterPos.d_y - d_RenderSize.d_height / 2;
				}
			}
		}// end if (fTan < 1 )
		else
		{
			if( pos.d_x < d_CenterPos.d_x ) //  
			{
				if( pos.d_y > d_CenterPos.d_y )
				{
					tmp.d_x = d_CenterPos.d_x - d_RenderSize.d_width / 2;
					tmp.d_y = d_CenterPos.d_y + d_RenderSize.d_width / ( fTan * 2 );
				}
				else 
				{
					tmp.d_x = d_CenterPos.d_x - d_RenderSize.d_width / 2;
					tmp.d_y = d_CenterPos.d_y - d_RenderSize.d_width / ( fTan * 2 );
				}
			}
			else
			{
				if( pos.d_y > d_CenterPos.d_y )
				{
					tmp.d_x = d_CenterPos.d_x + d_RenderSize.d_width / 2;
					tmp.d_y = d_CenterPos.d_y + d_RenderSize.d_width / ( fTan * 2 );
				}
				else
				{
					tmp.d_x = d_CenterPos.d_x + d_RenderSize.d_width / 2;
					tmp.d_y = d_CenterPos.d_y - d_RenderSize.d_width / ( fTan * 2 );
				}
			}
		} // end else( if( fTan < 1 ) )
		return tmp;
	}
	const Image* FalagardMiniMap::GetActiveArrowImage( Point& pos )
	{
		
		// ���ȼ���Ƕ�
		Point tmp;
		float fAngel = 0;
		float fX = abs( pos.d_x - d_CenterPos.d_x );
		float fY = abs( pos.d_y - d_CenterPos.d_y );
		if( fX == 0 ) // �����ͬһy��
		{
			if( pos.d_y > d_CenterPos.d_y )
				fAngel = 180;
			else
				fAngel = 0;
		}
		if( fY == 0 )
		{
			if( pos.d_x > d_CenterPos.d_x )
				fAngel = 90;
			else
				fAngel = 270;
		}
		fAngel = float( atan2( fY, fX ) * 180.f / 3.1415926f );

		if( pos.d_x < d_CenterPos.d_x ) //  
		{
			if( pos.d_y > d_CenterPos.d_y )
				fAngel = 270 - fAngel;
			else
				fAngel = 270 + fAngel;
		}
		else
		{
			if( pos.d_y > d_CenterPos.d_y )
				fAngel = 90 + fAngel;
			else
				fAngel = 90 - fAngel;
		}

		Animate* pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_ActiveAnimate );
		if( pAnimate )
		{
			int nCurFrame = int( ( fAngel * pAnimate->getFrameNumber() ) / 360 );
			return pAnimate->getFrame( nCurFrame );
		}
		return NULL;
	}
	// ���÷����tooltip
	void	FalagardMiniMap::SetMinimapToolTipAction( Point& mousePos )
	{
		MINIMAP_FLAG_DATA*   it;
		Point curPos = ScreenPosToImagePos( mousePos );
		Point curActivePos;
		Rect  rect;
		for( int i = 0; i < m_aPointerTimer[ IMAGE_TYPE_Active ]; i ++ )// = m_aActivePos.begin(); it != m_aActivePos.end(); it ++ )
		{
			it = &m_aActivePos.at( i );
			if( !IsPosVisible( it->pos ) ) // ������ɼ����ж�
			{
				const Image* pImage = GetActiveArrowImage( it->pos );
				curActivePos = GetActiveArrowImagePos( it->pos );

				rect.d_left		= curActivePos.d_x - pImage->getSourceTextureArea().getWidth() / 2;
				rect.d_right	= rect.d_left + pImage->getSourceTextureArea().getWidth();
				rect.d_top		= curActivePos.d_y - pImage->getSourceTextureArea().getHeight() / 2;
				rect.d_bottom	= rect.d_top + pImage->getSourceTextureArea().getHeight();
				if( rect.isPointInRect( curPos ) )
				{
					if( m_objTooltip.size() != 0 )
					{
						m_objTooltip += "\r\n";
					}
					m_objTooltip += it->name;
				}

			}
		}
	}
	
	void    FalagardMiniMap::onMouseMove( MouseEventArgs& e )
	{
		if( m_bDraping ) // ��������ѷ�
		{
			d_CenterPos.d_x -= e.position.d_x - m_ptLastPos.d_x; 
			d_CenterPos.d_y -= e.position.d_y - m_ptLastPos.d_y; 
			m_ptLastPos = e.position; 
			NormalArea();
			d_fresh = true;
			requestRedraw(); 
			m_ptCoordinate = ImagePosToScenePos( d_OldCenterPos );
		}
		else
		{
			
			// ����tooltip
			m_objTooltip.clear();// = "";
			assert( m_objTooltip.empty() );
			//OutputDebugStringA( "��ʼ����tooltip\n" );
			//OutputDebugStringA( "Animate \n" );
			SetMinimapToolTip( m_aAnimyPos, e.position, getAnimyImage(),IMAGE_TYPE_Animy ); 
			//OutputDebugStringA( "Animate End\n" );
			


			//OutputDebugStringA( "ExpNpcPos\n" );
			SetMinimapToolTip( m_aExpNpcPos, e.position, getExpNpcImage(), IMAGE_TYPE_ExpNpc ); 
			//OutputDebugStringA( "Teammate\n" );
			SetMinimapToolTip( m_aTeammatePos, e.position, getTeammateImage(), IMAGE_TYPE_Team );
			//OutputDebugStringA( "PlayerPos\n" );
//			SetMinimapToolTip( m_aPlayerPos, e.position, getPlayerImage(), IMAGE_TYPE_Player );	//080423
			//OutputDebugStringA( "expObj\n" );
			SetMinimapToolTip( m_aExpObjPos, e.position, getObjImage(), IMAGE_TYPE_ExpObj );
			//OutputDebugStringA( "ScenePos\n" );
			SetMinimapToolTip( m_aScenePos, e.position, getScenePosImage(), IMAGE_TYPE_ScenePos );
			SetMinimapToolTip( m_aPetPos, e.position, getScenePosImage(), IMAGE_TYPE_Pet );
			//OutputDebugStringA( "Action\n" );
			SetMinimapToolTipAction( e.position );
			if( getTooltip() )
			{
				getTooltip()->setTargetWindow( this );
				setTooltipText( m_objTooltip ); 
				getTooltip()->positionSelf();

			}
			m_ptCoordinate = ImagePosToScenePos( ScreenPosToImagePos( e.position ) );
		}
		Window::onMouseMove( e );
	}
	void    FalagardMiniMap::onMouseButtonDown( MouseEventArgs& e )
	{
		if( e.button == RightButton )  
		{
			m_bDraping = true;
			m_ptLastPos = e.position; 
			d_OldCenterPos = d_MyselfPos;
		}
	}
	void    FalagardMiniMap::onMouseButtonUp( MouseEventArgs& e )
	{
		if( e.button == RightButton )
		{
			if( m_bDraping )
				m_bSuperlook = true;
			m_bDraping = false;
			d_ReturnTimer = MAX_RETURN_TIMER;
		}
	}
	void    FalagardMiniMap::onMouseDoubleClicked( MouseEventArgs& e )
	{
		if( e.button == RightButton ) 
		{
			m_bDraping = false; 
			m_bSuperlook = false; 
			d_CenterPos = d_MyselfPos;
			d_fresh = true;
		}
	}
	void    FalagardMiniMap::onMouseEnters( MouseEventArgs& e )
	{
		m_bMouseInWindow = true;
	}
	void    FalagardMiniMap::onMouseLeaves( MouseEventArgs& e )
	{
		Window::onMouseLeaves( e );
		m_bMouseInWindow = false;
		m_bDraping = false;
		d_fresh = true;
		requestRedraw(); 
	}
	void    FalagardMiniMap::onSized( WindowEventArgs& e )
	{
		FalagardStaticImage::onSized( e );
		d_fresh = true;
	}
	// ���ó����ߴ�
	void FalagardMiniMap::SetSceneSize( const Size& size )
	{
		m_SceneSize = size;
	}


	
	// ��������ת������������
	Point FalagardMiniMap::ImagePosToWindowPos( Point& pos )
	{
		Point tmpPos;
		tmpPos.d_x = pos.d_x - GetCurrentZero().d_x;
		tmpPos.d_y = pos.d_y - GetCurrentZero().d_y;
		tmpPos.d_x = getAbsoluteRect().d_left+ ( getAbsoluteRect().getWidth() * ( tmpPos.d_x ) ) / d_RenderSize.d_width;
		tmpPos.d_y = getAbsoluteRect().d_top + ( getAbsoluteRect().getHeight() * ( tmpPos.d_y ) ) / d_RenderSize.d_height;
		return tmpPos;
	}
	// ��Ļ����ת����ͼ������
	Point FalagardMiniMap::ScreenPosToImagePos( Point& pos )
	{
		Point tmpPos;
		// �Եõ���������Ͻǵ���Ļƫ����
		tmpPos.d_x = pos.d_x - getPixelRect().d_left;
		tmpPos.d_y = pos.d_y - getPixelRect().d_top;
		// ����ƫ���������������
		tmpPos.d_x = GetCurrentZero().d_x + d_RenderSize.d_width * tmpPos.d_x / getPixelRect().getWidth();
		tmpPos.d_y = GetCurrentZero().d_y + d_RenderSize.d_height * tmpPos.d_y / getPixelRect().getHeight();
		return tmpPos;
		
	}
	
	//  �ӳ�������ת����ͼ������
	Point FalagardMiniMap::ScenePosToImagePos( Point& pos )
	{
		return Point( pos.d_x / d_SceneToImageScale.d_width, pos.d_y / d_SceneToImageScale.d_height );
	}
	//   ��ͼ������ת������������
	Point FalagardMiniMap::ImagePosToScenePos( Point& pos )
	{
		return Point( pos.d_x * d_SceneToImageScale.d_width, pos.d_y * d_SceneToImageScale.d_height );
	}
	//   �õ���ǰ��ʾ������ԭ��
	Point FalagardMiniMap::GetCurrentZero()
	{
		return Point( d_CenterPos.d_x - d_RenderSize.d_width / 2, d_CenterPos.d_y - d_RenderSize.d_height / 2 );
	}

	void FalagardMiniMap::setMinimapData( int nType, float fX, float fY, String32& szName  )
	{
		
		RENDER_POS* pList = NULL;
		MINIMAP_FLAG_DATA *data = NULL;
		switch( nType )
		{
		case IMAGE_TYPE_Animy:
			pList = &m_aAnimyPos;
			break;
		case IMAGE_TYPE_ExpNpc:	
			pList = &m_aExpNpcPos;
			break;
		case IMAGE_TYPE_Team:
			pList = &m_aTeammatePos;
			break;
//		case IMAGE_TYPE_Player:	
//			pList = &m_aPlayerPos;
//			break;
		case IMAGE_TYPE_ExpObj:	
			pList = &m_aExpObjPos;
			break;
		case IMAGE_TYPE_Active:
			pList = &m_aActivePos;
			break;
		case IMAGE_TYPE_ScenePos: 
			pList = &m_aScenePos;
			break;
		case IMAGE_TYPE_Flash:
			pList = &m_aFlashPos;
			break;
		case IMAGE_TYPE_Pet:
			pList = &m_aPetPos;
			break;
		default:
			break;
		}
		if( pList == NULL )
			return;
		if( m_aPointerTimer[ nType ] >= (int)(pList->size()) ) // �������巶Χ
			pList->resize( pList->size() + MAX_NUMBER ); // ���ǵ�
		data = &( pList->at( m_aPointerTimer[ nType ] ) );
		if( data )
		{
			data->pos.d_x = fX / d_SceneToImageScale.d_width;
			data->pos.d_y = fY / d_SceneToImageScale.d_height;
			data->name = szName;
			m_aPointerTimer[ nType ] ++;
		}
	}
	
	 //////////////////////////////////////////////////////////////////////////
    Window* FalagardMinimapFactory::createWindow(const String& name)
    {
        return new FalagardMiniMap(d_type, name);
    }

    void FalagardMinimapFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }
}