#include "windows.h"
#include ".\falagardradar.h"
#include "ceguiimagesetmanager.h"
#include "ceguiimageset.h"
#include "ceguiwindowmanager.h"
#include "elements\ceguistaticimage.h"
#include "ceguianimatemanager.h"
#include "ceguianimate.h"
#include "math.h"
#include "elements\ceguitooltip.h"
#include "CEGUIExceptions.h"
#include "CEGUIPropertyHelper.h"
namespace CEGUI
{
	const utf8 FalagardRadar::WidgetTypeName[] = "Falagard/Radar";

	//===================================================================================
	//
	// FalagardSceneMap/ParentWindow
	//
	//===================================================================================
	const utf8 FalagardRadar::ParentWindow::WidgetTypeName[] = "Falagard/Radar/ParentWindow";

	Window* FalagardRadar::ParentWindowFactory::createWindow(const String& name)
	{
		return new FalagardRadar::ParentWindow(d_type, name);
	}

	void FalagardRadar::ParentWindowFactory::destroyWindow(Window* window)
	{
		delete window;
	}

	FalagardRadar::ParentWindow::ParentWindow(const String& type, const String& name) :
	Window(type, name)
		, m_pRoundRegion(0)
	{
		m_pRoundRegion = ImagesetManager::getSingleton().getImageset("WXSJ_RoundRegion");
	}

	FalagardRadar::ParentWindow::~ParentWindow()
	{
	}

	void FalagardRadar::ParentWindow::drawSelf(float z)
	{
		Window::drawSelf(z);
		if(!m_pRoundRegion) return;
		if( m_bIsUseRondRegion )
		{
			Renderer* renderer = System::getSingleton().getRenderer();

			Rect rect_screen = getPixelRect();
			const Texture* tex = m_pRoundRegion->getTexture();
			Rect rect_texture = Rect(0.f, 0.f, 1.0f, 1.0f);
			renderer->enterMultiTextureMode(rect_screen, tex, rect_texture);
		}
	}

	void FalagardRadar::ParentWindow::notifyDrawEnd(void)
	{
		if( m_bIsUseRondRegion )
		{
			Renderer* renderer = System::getSingleton().getRenderer();

			renderer->exitMultiTextureMode();
		}
	}

	FalagardRadarProperties::IconImage			FalagardRadar::d_IconImageProperty;
	FalagardRadarProperties::AnimateImage		FalagardRadar::d_AnimateImageProperty;
	FalagardRadarProperties::AutoReset			FalagardRadar::d_AutoRestProperty;
	FalagardRadarProperties::RoundEnabled		FalagardRadar::d_RoundProperty;


	const float MAX_RESET_TIMER = 10;	//080423  5
	const int GROWP_SIZE = 5;
	const float ANGEL_1 = 3.1415926f / 180;
	FalagardRadar::FalagardRadar(const String& type, const String& name):
		FalagardSceneMap( type, name )
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardRadar )
			CEGUI_ADD_STATICPROPERTY( &d_IconImageProperty );
			CEGUI_ADD_STATICPROPERTY( &d_AnimateImageProperty );
			CEGUI_ADD_STATICPROPERTY( &d_AutoRestProperty );
			CEGUI_ADD_STATICPROPERTY( &d_RoundProperty );
		CEGUI_END_ADD_STATICPROPERTY

		m_nDisplayMode = SCENEMAP_DISPLAY_MODE_ABSOLTE; 
		m_bMouseInWindow = false;
		m_bIsAutoReset = true;
		m_bRoundEnabled = false;
		m_pMapLight = NULL;
	}

	FalagardRadar::~FalagardRadar(void)
	{
		for( int i = 0; i < IMAGE_TYPE_NUM; i ++ )
		{
			RENDER_POS* pList = GetListByType( i );

			if ( pList != NULL )
			{
				for( int j = 0;j < int( pList->size() ); j ++ )
				{
					MINIMAP_FLAG_DATA* data = pList->at( j );

					if ( data != NULL )
					{
						delete data;
					}					
				}

				pList->clear();
			}		
		}
	}
	void    FalagardRadar::onMouseEnters( MouseEventArgs& e )
	{
		m_bMouseInWindow = true;
		FalagardSceneMap::onMouseEnters( e );
	}

	void    FalagardRadar::onMouseLeaves( MouseEventArgs& e )
	{
		m_bMouseInWindow = false;
		FalagardSceneMap::onMouseLeaves( e );
	}

	void FalagardRadar::onMouseMove( MouseEventArgs &e )
	{
		FalagardSceneMap::onMouseMove( e );
		if( m_pBackground )
		{
			ChildList::const_reverse_iterator	child, end;

			end = m_pBackground->getDrawList()->rend();
			m_objTooltip.clear();
			for (child = m_pBackground->getDrawList()->rbegin(); child != end; ++child)
			{
				if ((*child)->isVisible())
				{
					// recursively scan children of this child windows...
					if( (*child)->isHit(e.position) && !(*child)->getMouseHollow() )
					{
						if( !m_objTooltip.empty() )
						{
							m_objTooltip += "\n";
						}
						m_objTooltip += (*child)->getText();
					}
				}
			}
			if( getTooltip() )
			{
				getTooltip()->setTargetWindow( this );
				setTooltipText( m_objTooltip ); 
				getTooltip()->positionSelf();
			}
		}
	}
	
	void FalagardRadar::setTypeImage( int nType, const Image* pImage ) //����image
	{
		//		case IMAGE_TYPE_Player:
		//			d_PlayerImage = pImage;
		//			break;

		switch( nType )
		{
			// ����
		case IMAGE_TYPE_Animy:	
			d_AnimyImage = pImage;
			break;	
			// ����
		case IMAGE_TYPE_Team:
			d_TeammateImage = pImage;
			break;
			// ����
		case IMAGE_TYPE_FamilyPlayer:	
			d_FamilyPlayerImage = pImage;
			break;
			// ���
		case IMAGE_TYPE_GuildPlayer:
			d_GuildPlayerImage = pImage;
			break;
			// �������
		case IMAGE_TYPE_HomelandPlayer:		
			d_HomelandPlayerImage = pImage;
			break;
			// �Ǳ������
		case IMAGE_TYPE_NoHomelandPlayer:
			d_NoHomelandPlayerImage = pImage;
			break;
			// ������
		case IMAGE_TYPE_ExpObj:
			d_ExpObjImage = pImage;
			break;
			// �������͵�
		case IMAGE_TYPE_ScenePos:
			d_ScenePosImage = pImage;
			break;
			// �����
		case IMAGE_TYPE_Flash:
			d_FlashImage = pImage;
			break;
			// ��ͨNPC
		case IMAGE_TYPE_ExpNpc:
			d_ExpNpcImage = pImage;
			break;
			// ����NPC( ��� )
		case IMAGE_TYPE_MisCompleteNpc:
			d_MissCompleteNpcImage = pImage;
			break;
			// ����NPC( δ��� )
		case IMAGE_TYPE_MisNoCompleteNpc: 
			d_MissNoCompleteNpcImage = pImage;
			break;
			// ����NPC( �ɽ� )	
		case IMAGE_TYPE_AcceptNpc:	
			d_MissionNpcImage = pImage;
			break;
			// ����
		case IMAGE_TYPE_Pet:
			d_PetImage = pImage;
			break;		
		}
	}
	const Image* FalagardRadar::getTypeImage( int nType )
	{
		//		case IMAGE_TYPE_Player:
		//			return d_PlayerImage;

		switch( nType )
		{
			// ����
		case IMAGE_TYPE_Animy:
			return d_AnimyImage;	
			// ����
		case IMAGE_TYPE_Team:
			return d_TeammateImage;
			// ����
		case IMAGE_TYPE_FamilyPlayer:
			return d_FamilyPlayerImage;
			// ���
		case IMAGE_TYPE_GuildPlayer:
			return d_GuildPlayerImage;
			// ����
		case IMAGE_TYPE_HomelandPlayer:		
			return d_HomelandPlayerImage;
			// �Ǳ���
		case IMAGE_TYPE_NoHomelandPlayer:
			return d_NoHomelandPlayerImage;
			// ������
		case IMAGE_TYPE_ExpObj:
			return d_ExpObjImage;
			// �������͵�
		case IMAGE_TYPE_ScenePos:
			return d_ScenePosImage;
			// �����
		case IMAGE_TYPE_Flash:
			return d_FlashImage;
			// ��ͨNPC
		case IMAGE_TYPE_ExpNpc:
			return d_ExpNpcImage;
			// ����NPC( ��� )
		case IMAGE_TYPE_MisCompleteNpc:
			return d_MissCompleteNpcImage;
			// ����NPC( δ��� )
		case IMAGE_TYPE_MisNoCompleteNpc: 
			return d_MissNoCompleteNpcImage;
			// ����NPC( �ɽ� )	
		case IMAGE_TYPE_AcceptNpc:	
			return d_MissionNpcImage;
			// ����
		case IMAGE_TYPE_Pet:
			return d_PetImage;	
	
		}
		return NULL;
	}
	void FalagardRadar::setTypeName( int nType, const String& str ) // ���ö�������
	{
		switch( nType )
		{
		case IMAGE_TYPE_MYSELF:
			d_PlayerAnimateName = str;
			break;
		case IMAGE_TYPE_Flash:
			d_FlashAnimateName = str;
			break;
		case IMAGE_TYPE_Active:
			d_ActiveAnimateName = str;
			break;
		default:
			break;
		}
	}
	String FalagardRadar::getTypeName( int nType )
	{
		switch( nType )
		{
		case IMAGE_TYPE_MYSELF:
			return d_PlayerAnimateName;
		case IMAGE_TYPE_Flash:
			return d_FlashAnimateName;
		case IMAGE_TYPE_Active:
			return d_ActiveAnimateName;
		default:
			break;
		}
		return "";
	}
	// ���ݵ�ǰ����λ����ֵ�ɼ���Χ
	void FalagardRadar::UpdateViewRect() 
	{
		SetViewByCenter( d_MyseflPos );
	} 
	
	// �������������ذ�
	void FalagardRadar::CreateSceneMap( const String& strFileName,const String& strPostFix)
	{
		createBackground( strFileName,strPostFix,true);
		((ParentWindow*)m_pBackground)->setRoundEnabled( m_bRoundEnabled );

		for( int i = 0; i < IMAGE_TYPE_NUM; i ++ )
		{
			m_aPointerTimer[ i ] = 0;
			RENDER_POS* pList = GetListByType( i );

			if ( pList != NULL )
			{
				for( int j = 0;j < int( pList->size() ); j ++ )
				{
					MINIMAP_FLAG_DATA* data = pList->at( j );
					if( data && data->m_pWindow )
						WindowManager::getSingleton().destroyWindow( data->m_pWindow );
				}
				pList->clear();
			}		
		}

		char szWindowName[ 128 ];
		sprintf( szWindowName, "%s_MySelf",getName().c_str() );
		if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) )
		{
			m_pMySelfWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
			m_pBackground->addChildWindow( m_pMySelfWindow );
		}
		else
		{
			m_pMySelfWindow = WindowManager::getSingleton().getWindow( szWindowName );
		}
		
		m_pMySelfWindow->setHookMode( Hook_Center );
		m_pMySelfWindow->setZoomMode( Relative );
		m_pMySelfWindow->setMaximumSize( Size( 100, 100 ) );
		m_pMySelfWindow->setMouseHollow( true );
		m_pBackground->setMouseHollow( true );
		m_pBackground->setMouseLButtonHollow( true );
		m_pBackground->setMouseRButtonHollow( true );
		m_pMySelfWindow->setAlwaysOnTop( true );

		//������ͼ̽�յ�
		_snprintf( szWindowName, 128, "%s_Auto_MapLight", getName().c_str() );  
		if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //������ڲ����ڣ��ʹ���һ��
			WindowManager::getSingleton().createWindow( m_mapChildType[ "Image" ], szWindowName );
		m_pMapLight = (StaticImage*)WindowManager::getSingleton().getWindow( szWindowName );
		m_pBackground->addChildWindow( m_pMapLight );
		m_pMapLight->show();
		m_pMapLight->setAlwaysOnTop(true);
		m_pMapLight->setMouseHollow(true);
		m_pMapLight->setMouseMoveHollow(true);
		m_pMapLight->setMouseLButtonHollow(true);
		m_pMapLight->setMouseRButtonHollow(true);
		m_pMapLight->setProperty("Image", "set:MapLight_RenderTexture image:View");
		m_pMapLight->setSize(CEGUI::Absolute, Size(128, 128));
		m_pMapLight->setHookMode( Hook_Center );
		m_pMapLight->setZoomMode( Relative );
		UpdateAllWindow();

	}
	int FalagardRadar::GetType( int nType )
	{
		//		case IMAGE_TYPE_Player:

		switch( nType )
		{
		case IMAGE_TYPE_Animy:	
		case IMAGE_TYPE_Team:

		case IMAGE_TYPE_FamilyPlayer:
		case IMAGE_TYPE_GuildPlayer:
		case IMAGE_TYPE_HomelandPlayer:		
		case IMAGE_TYPE_NoHomelandPlayer:

		case IMAGE_TYPE_ExpObj:
		case IMAGE_TYPE_ScenePos:

		case IMAGE_TYPE_ExpNpc:
		case IMAGE_TYPE_MisCompleteNpc:
		case IMAGE_TYPE_MisNoCompleteNpc:
		case IMAGE_TYPE_AcceptNpc:
		case IMAGE_TYPE_Pet:
			return POS_TYPE_IMAGE;

		case IMAGE_TYPE_Flash:
		case IMAGE_TYPE_Active:
			return POS_TYPE_ANIMATE;
		}

		return POS_TYPE_INVALIDE;
	}
	void FalagardRadar::setMinimapData( int nType, float fX, float fY, String32& szName  )
	{
		//RENDER_POS* pList = GetListByType( nType );

		MINIMAP_FLAG_DATA *data = NULL;
		data = getMiniMapFlagData( nType, m_aPointerTimer[ nType ] );
		if( data )
		{
			data->pos= ScenePosToWindowPos( Point( fX, fY ) );
			data->name = szName;
			data->m_pWindow->setText( szName );
			data->m_pWindow->setHookPosition( data->pos );
			data->m_pWindow->setSize( Absolute,data->size );
			data->m_pWindow->show();
			//data->m_pWindow->moveToFront();
			m_aPointerTimer[ nType ] ++;
		}
	}
	MINIMAP_FLAG_DATA*  FalagardRadar::getMiniMapFlagData( int nType, int nIndex )
	{
		RENDER_POS* pList = GetListByType( nType );
		if( pList )
		{
			char szWindowName[ 128 ];
			sprintf( szWindowName, "%s_%s_%d",getName().c_str(),GetNameByType( nType ).c_str(), nIndex );
			if( nIndex >= int( pList->size() ) )
			{
				MINIMAP_FLAG_DATA* pNewItem = new MINIMAP_FLAG_DATA;
				if( pNewItem )
				{
					if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) )
						pNewItem->m_pWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
					else
						pNewItem->m_pWindow = WindowManager::getSingleton().getWindow( szWindowName );
					if( pNewItem->m_pWindow )
					{
						pNewItem->m_pWindow->setMaximumSize( Size( 100, 100 ) );
						pNewItem->m_pWindow->setHookMode( Hook_Center );
						pNewItem->m_pWindow->setZoomMode( Relative );
						m_pBackground->addChildWindow( pNewItem->m_pWindow );
						pNewItem->m_pWindow->show();
						
						if( getTypeImage( nType ) )
						{
							( ( StaticImage*)( pNewItem->m_pWindow ))->setImage( getTypeImage( nType ) );
							pNewItem->size = ( getTypeImage( nType )->getSourceTextureArea() ).getSize();
						}
						pList->push_back( pNewItem );
					}
				}
			}
			if( nIndex < int( pList->size() ) )
				return pList->at( nIndex );
			else
				throw InvalidRequestException( "radar item full" );
		}
		return NULL;
	}
	void FalagardRadar::Clear()
	{
		for( int i = 0; i < IMAGE_TYPE_NUM; i ++ )
		{
			m_aPointerTimer[ i ] = 0;
			RENDER_POS* pList = GetListByType( i );

			if ( pList != NULL )
			{
				for( int j = 0;j < int( pList->size() ); j ++ )
				{
					MINIMAP_FLAG_DATA* data = pList->at( j );
					if( data )
						data->m_pWindow->hide();
				}
			}			
		}
	}
	// ������ҵķ���
	void FalagardRadar::setMyselfDir( const int nDirect )
	{
		FalagardSceneMap::setMyselfDir( nDirect );
		updateMyselfPos();
		updateMyselfDir();
		//m_pMySelfWindow->moveToFront();
		

	}

	// ������ҵ�λ��
	void FalagardRadar::setMyselfPos( const Point& pos )
	{
		//if( m_bDraping || d_ReturnTimer != 0 ) // ��������ѷŻ������ڿ�������򣬾͸��µ����ﵱǰλ��
		if( m_PlayerPos != pos )
			d_ReturnTimer = 0;
		m_PlayerPos = pos;
		d_MyseflPos = ScenePosToWindowPos( pos );
		d_fresh = true;
		updateMyselfPos(); 
		updateMyselfDir();
	}
	// �õ������ͷ��λ��
	Point FalagardRadar::GetActiveArrowImagePos( Rect& rcView, Point& posStart, Point& posEnd/*, float fWidth*/)
	{
		//if( !m_bRoundEnabled )
		{
			Point pos;
			float fx,fy,fc,fangle;
			fx = ( posEnd.d_x - posStart.d_x );
			fy = ( posEnd.d_y - posStart.d_y );
			if( fx == 0 ) // �������y����
			{
				pos.d_x = posStart.d_x;
				if( fy > 0 )
					pos.d_y = rcView.d_bottom;
				else
					pos.d_y = rcView.d_top;
				return pos;
			}
			if( fy == 0 )
			{
				pos.d_y = posStart.d_y;
				if( fx > 0 )
					pos.d_x = rcView.d_left;
				else
					pos.d_x = rcView.d_right;
				return pos;
			}
			fangle = float( abs( fy ) ) / float( abs( fx ) );
			if( fangle < 1 ) // ˵���ཻ�����ұ���
			{
				if( fx < 0 )
					pos.d_x = rcView.d_left;
				else
					pos.d_x = rcView.d_right;
				fc = rcView.d_right - posStart.d_x;
				fc = abs( fc * fy / fx );
				if( fy < 0 )
					pos.d_y = posStart.d_y - fc;
				else
					pos.d_y = posStart.d_y + fc;
			}
			else if( fangle > 1 )
			{
				if( fy < 0 )
					pos.d_y = rcView.d_top;
				else
					pos.d_y = rcView.d_bottom;
				fc = rcView.d_bottom - posStart.d_y;
				fc = abs( fc * fx / fy );
				if( fx > 0 )
					pos.d_x = posStart.d_x + fc;
				else
					pos.d_x = posStart.d_x - fc;
			}
			else // == 1
			{
				if( fx < 0 )
					pos.d_x = rcView.d_left;
				else 
					pos.d_x = rcView.d_right;
				if( fy < 0 )
					pos.d_y = rcView.d_top;
				else
					pos.d_y = rcView.d_bottom;
			}
			return pos;
		}

		//float fRadius = rcView.getWidth() / 2 - fWidth;
		//Point pos;
		//float fx,fy,fangle;
		//fx = ( posEnd.d_x - posStart.d_x );
		//fy = ( posEnd.d_y - posStart.d_y );
		//if( fx == 0 ) // �������y����m
		//{
		//	pos.d_x = posStart.d_x;
		//	if( fy > 0 )
		//		pos.d_y = rcView.d_bottom;
		//	else
		//		pos.d_y = rcView.d_top;
		//	return pos;
		//}
		//if( fy == 0 )
		//{
		//	pos.d_y = posStart.d_y;
		//	if( fx > 0 )
		//		pos.d_x = rcView.d_left;
		//	else
		//		pos.d_x = rcView.d_right;
		//	return pos;
		//}

		//fangle = atan( float( fabs(fy ) ) / float( fabs(fx ) ) );

		//if( fx < 0 )
		//	pos.d_x = posStart.d_x - cos( fangle ) * fRadius;
		//else
		//	pos.d_x = posStart.d_x + cos( fangle ) * fRadius;
		//if( fy < 0 )
		//	pos.d_y = posStart.d_y - sin( fangle ) * fRadius;
		//else
		//	pos.d_y = posStart.d_y + sin( fangle ) * fRadius;
		//return pos;
	}
	// �õ������ͷ��ͼ��
	const Image* FalagardRadar::GetActiveArrowImage( Rect& rcView,Point& posStart, Point& posEnd )
	{
		Point pos;
		float fx,fy,fangle;
		fx = ( posEnd.d_x - posStart.d_x );
		fy = ( posEnd.d_y - posStart.d_y );
		
		if( fx == 0 ) // �������y����
		{
			if( fy > 0 )
				fangle = 90;
			else
				fangle = 270;
		}
		else if( fy == 0 )
		{
			if( fx > 0 )
				fangle = 0;
			else
				fangle = 180;
		}
		else
		{
			fangle = atan( abs( fy / fx ) ) / ANGEL_1;
		}
		if( fx > 0 ) // 1,4����
		{
			if( fy > 0 )// 4
				fangle = 360 - fangle;
		}
		else
		{
			if( fy < 0 )//2
				fangle = 180 - fangle;
			else
				fangle = 180 + fangle;
		}
		Animate* pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_ActiveAnimateName );
		int nCurFrame = int( ( fangle * pAnimate->getFrameNumber() ) / 360 );
		return pAnimate->getFrame( nCurFrame );
	}
		
		
	// ���¼����
	void FalagardRadar::updateActive()
	{
		// �ø��ݵ�ǰ�ӿ�������
		static int time_now = 0;
		time_now = (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);
		Point ptCenter = GetCurrentCenter();
		Rect  rcViewRect = GetCurrentViewRect();
		
		Animate* pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_ActiveAnimateName );
		Animate* pFlashAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_FlashAnimateName );
		if( pAnimate == NULL )
			return;
		if( pFlashAnimate == NULL ) 
			return;
		Image* pImage = (Image*)( pAnimate->getFrame( 0 ) ); // Ĭ�ϳߴ�
		
		Rect rcDirViewRect = rcViewRect;

		if( pImage )
		{
			float width = ( pImage->getSourceTextureArea().getWidth() / 2 ) / m_pBackground->getAbsoluteWidth();
			float height = ( pImage->getSourceTextureArea().getHeight() / 2 ) / m_pBackground->getAbsoluteHeight();
			rcDirViewRect.d_left += width;
			rcDirViewRect.d_right -= width;
			rcDirViewRect.d_top += height;
			rcDirViewRect.d_bottom -= height;
		}

		// ����ĵ�
		for( int i = 0; i < m_aPointerTimer[ IMAGE_TYPE_Active ]; i ++ )
		{
			MINIMAP_FLAG_DATA* data = GetListByType( IMAGE_TYPE_Active )->at( i );
			if( data )
			{
				// ���λ�ò����ӿ��ڣ��ͻ���ͷ������Ͳ���
				if( !rcViewRect.isPointInRect( data->pos ) )
				{
					data->m_pWindow->setHookPosition( GetActiveArrowImagePos( rcDirViewRect, ptCenter, data->pos ) );
					Image* pImage = (Image*)( GetActiveArrowImage( rcViewRect, ptCenter, data->pos ) );
					if( pImage )
						( ( StaticImage*)( data->m_pWindow ))->setImage( pImage );
					Size size = pImage->getSourceTextureArea().getSize();
					data->m_pWindow->setSize( Absolute, size );
					data->m_pWindow->show();
					data->m_pWindow->setMouseHollow( false );
				}
				else
				{
					Image* pImage = (Image*)( pFlashAnimate->getFrame( time_now, -1 ) );
					if( pImage )
					{
						if( data )
						{
							data->m_pWindow->setHookPosition( data->pos );
							Size size = pImage->getSourceTextureArea().getSize();
							( ( StaticImage*)data->m_pWindow)->setImage( pImage );
							data->m_pWindow->setSize( Absolute, size );
							data->m_pWindow->setMouseHollow( true );
						}
					}
				}
			}
		}

/*
		// pk����
		for( int i = 0; i < m_aPointerTimer[ IMAGE_TYPE_PK ]; i ++ )	// Ӧ��ͬһʱ��ֻ��һ��
		{
			MINIMAP_FLAG_DATA* data = GetListByType( IMAGE_TYPE_PK )->at( i );
			if( data )
			{
				// λ�ò����ӿ��ھͻ���ͷ
				if( !rcViewRect.isPointInRect( data->pos ) )
				{
					data->m_pWindow->setHookPosition( GetActiveArrowImagePos( rcDirViewRect, ptCenter, data->pos ) );
					Image* pImage = (Image*)( GetActiveArrowImage( rcViewRect, ptCenter, data->pos ) );
					if( pImage )
						( ( StaticImage*)( data->m_pWindow ))->setImage( pImage );

					Size size = pImage->getSourceTextureArea().getSize();
					data->m_pWindow->setSize( Absolute, size );
					data->m_pWindow->show();
					data->m_pWindow->setMouseHollow( false );
				}
				// �ڴ�����
				else
				{
					Image* pImage = (Image*)( pFlashAnimate->getFrame( time_now, -1 ) );
					if( pImage )
					{
						if( data )
						{
							data->m_pWindow->setHookPosition( data->pos );
							Size size = pImage->getSourceTextureArea().getSize();
							( ( StaticImage*)data->m_pWindow)->setImage( pImage );
							data->m_pWindow->setSize( Absolute, size );
							data->m_pWindow->setMouseHollow( true );
						}
					}
				}

			}	// if( data )
		}	// for
*/

	}

	void FalagardRadar::updateMyselfPos()
	{
		m_pMySelfWindow->setHookPosition( d_MyseflPos );
		m_pMapLight->setHookPosition( d_MyseflPos );
		//if( m_bOnShow )
		//	UpdateViewRect();
	}
	void FalagardRadar::updateMyselfDir()
	{
		Animate* pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_PlayerAnimateName );
		if( pAnimate )
		{
			int nCurFrame = ( ( d_PlayerDirect ) * pAnimate->getFrameNumber() ) / 360;
			Image* pImage = (Image*)( pAnimate->getFrame( nCurFrame ) );
			Size size;
			
			if( pImage )
			{
				size.d_width = pImage->getSourceTextureArea().getWidth();
				size.d_height = pImage->getSourceTextureArea().getHeight();
				( ( StaticImage*)( m_pMySelfWindow ))->setImage( pImage );
				m_pMySelfWindow->setSize( Absolute, size );
				m_pMapLight->setSize(Absolute, Size(128, 128));
			}
		}

	}
	void FalagardRadar::updateFlash()
	{
	}
	void FalagardRadar::updateSelf(float elapsed)
	{
		if( !m_bMouseInWindow )
			d_MouseScenePos = m_PlayerPos;

		if( m_bDraping )
		{
			d_ReturnTimer = MAX_RESET_TIMER;
			d_fresh = true;
		}
		else if( d_ReturnTimer > 0 )
		{
			if( m_bIsAutoReset )
			{
				d_ReturnTimer -= elapsed;
				if( d_ReturnTimer < 0 )
				{
					d_ReturnTimer = 0;
					UpdateViewRect();
					d_fresh = true;
				}
			}
		}
		else
		{
			if( m_bIsAutoReset )
				UpdateViewRect();
		}

		if( m_aPointerTimer[ IMAGE_TYPE_Active ] )
			d_fresh = true;

		if( d_fresh && isVisible() )
		{
			updateFlash();
			updateActive();	
			d_fresh = false;
			requestRedraw();
		}
	}
	FalagardRadar::RENDER_POS* FalagardRadar::GetListByType( int nType )
	{
		//		case IMAGE_TYPE_Player:	
		//			pList = &m_aPlayerPos;
		//			break;

		RENDER_POS* pList = NULL;

		switch( nType )
		{
		case IMAGE_TYPE_Animy:
			pList = &m_aAnimyPos;
			break;	
		case IMAGE_TYPE_Team:
			pList = &m_aTeammatePos;
			break;
		case IMAGE_TYPE_FamilyPlayer:
			pList = &m_aFamilyPlayerPos;
			break;
		case IMAGE_TYPE_GuildPlayer:
			pList = &m_aGuildPlayerPos;
			break;
		case IMAGE_TYPE_HomelandPlayer:		
			pList = &m_aHomelandPlayerPos;
			break;
		case IMAGE_TYPE_NoHomelandPlayer:
			pList = &m_aNoHomelandPlayerPos;
			break;
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
		case IMAGE_TYPE_ExpNpc:	
			pList = &m_aExpNpcPos;
			break;
		case IMAGE_TYPE_MisCompleteNpc:
			pList = &m_aMissCompleteNpcPos;
			break;
		case IMAGE_TYPE_MisNoCompleteNpc:
			pList = &m_aMissNoCompleteNpcPos;
			break;
		case IMAGE_TYPE_AcceptNpc:
			pList = &m_aMissionNpcPos;
			break;
		case IMAGE_TYPE_Pet:
			pList = &m_aPetPos;
			break;
		default:
			break;
		}
		return pList;
	}
	int FalagardRadar::GetTypeByName( const String& val )
	{
		int nType = 0;
		
		//		if( val == (utf8*)"PlayerImage" )
		//		{
		//			nType = FalagardRadar::IMAGE_TYPE_Player;
		//		}

		if( val == (utf8*)"AnimyImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_Animy;
		}	
		else if( val == ( utf8*)"TeammateImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_Team;
		}		
		else if( val == (utf8*)"FamilyPlayerImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_FamilyPlayer;
		}
		else if( val == (utf8*)"GuildPlayerImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_GuildPlayer;
		}
		else if( val == (utf8*)"HomelandPlayerImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_HomelandPlayer;
		}
		else if( val == (utf8*)"NoHomelandPlayerImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_NoHomelandPlayer;
		}
		else if( val == ( utf8*)"ObjImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_ExpObj;
		}
		else if( val == ( utf8*)"ActiveImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_Active;
		}
		else if( val == ( utf8*)"ScenePosImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_ScenePos;
		}
		else if( val == ( utf8*)"FlashImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_Flash;
		}
		else if( val == ( utf8*)"ExpNpcImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_ExpNpc;
		}
		else if( val == ( utf8*)"MissCompleteNpcImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_MisCompleteNpc;   // ����NPC( ��� )
		}
		else if( val == ( utf8*)"MissNoCompleteNpcImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_MisNoCompleteNpc; // ����NPC( δ��� )						 
		}
		else if( val == ( utf8*)"MissionNpcImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_AcceptNpc;	    // ����NPC( �ɽ� )
		}	
		else if( val == ( utf8*)"MySelfImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_MYSELF;
		}
		else if( val == ( utf8*)"PetImage" )
		{
			nType = FalagardRadar::IMAGE_TYPE_Pet;
		}		

		return nType;
	}

	String	FalagardRadar::GetNameByType( int nType )
	{
		//		case IMAGE_TYPE_Player:
		//			return (utf8*)"ObjImage";

		switch( nType )
		{
		case IMAGE_TYPE_Animy:
			return (utf8*)"AnimyImage";		
		case IMAGE_TYPE_Team:
			return ( utf8*)"TeammateImage";
		case IMAGE_TYPE_FamilyPlayer:
			return (utf8*)"FamilyPlayerImage";
		case IMAGE_TYPE_GuildPlayer:
			return (utf8*)"GuildPlayerImage";
		case IMAGE_TYPE_HomelandPlayer:		
			return (utf8*)"HomelandPlayerImage";
		case IMAGE_TYPE_NoHomelandPlayer:
			return (utf8*)"NoHomelandPlayerImage";
		case IMAGE_TYPE_ExpObj:
			return ( utf8*)"ObjImage";
		case IMAGE_TYPE_Active:
			return ( utf8*)"ActiveImage";
		case IMAGE_TYPE_ScenePos:
			return ( utf8*)"ScenePosImage";
		case IMAGE_TYPE_Flash:
			return (utf8*)"FlashImage";	
		case IMAGE_TYPE_ExpNpc:
			return  (utf8*)"ExpNpcImage";
		case IMAGE_TYPE_MisCompleteNpc:
			return (utf8*)"MissCompleteNpcImage";
		case IMAGE_TYPE_MisNoCompleteNpc:
			return (utf8*)"MissNoCompleteNpcImage";
		case IMAGE_TYPE_AcceptNpc:
			return ( utf8*)"MissionNpcImage";
		case IMAGE_TYPE_MYSELF:
			return ( utf8*)"MySelfImage";
		case IMAGE_TYPE_Pet:
			return (utf8*)"PetImage";	
		}

		return "";
	}

	namespace FalagardRadarProperties
	{
		String IconImage::get( const PropertyReceiver* receiver) const
		{
			return "";
		}

		void IconImage::set( PropertyReceiver* receiver, const String& value) 
		{
			char szType[ 128 ];
			char szSet[ 128 ];
			char szImage[ 128 ];
			sprintf( szSet, "" );
			sprintf( szType, "" );
			sprintf( szImage, "" );
			sscanf( value.c_str(), "type:%127s set:%127s image:%127s", szType, szSet, szImage );
			const Image* pImage = &ImagesetManager::getSingleton().getImageset( szSet )->getImage( szImage );
			int nType = ( static_cast<FalagardRadar*>(receiver) )->GetTypeByName( (utf8*)szType );
			static_cast< FalagardRadar*>(receiver)->setTypeImage( nType, pImage );
		}

		String AnimateImage::get( const PropertyReceiver* receiver) const
		{
			return "";
		}

		void AnimateImage::set( PropertyReceiver* receiver, const String& value) 
		{
			char szType[ 128 ];
			char szAnimate[ 128 ];
			sprintf( szType, "" );
			sprintf( szAnimate, "" );
			sscanf( value.c_str(), "type:%127s animate:%127s", szType, szAnimate);
			int nType = static_cast<FalagardRadar*>(receiver)->GetTypeByName( (utf8*)szType );
			static_cast< FalagardRadar*>(receiver)->setTypeName( nType, szAnimate );
		}

		String AutoReset::get( const PropertyReceiver* receiver) const
		{
			return "";
		}

		void AutoReset::set( PropertyReceiver* receiver, const String& value) 
		{
			static_cast< FalagardRadar*>(receiver)->setAutoReset( PropertyHelper::stringToBool( value ) );
		}

		String RoundEnabled::get( const PropertyReceiver* receiver) const
		{
			return "";
		}

		void RoundEnabled::set( PropertyReceiver* receiver, const String& value) 
		{
			static_cast< FalagardRadar*>(receiver)->setRoundEnabled( PropertyHelper::stringToBool( value ) );
		}
	}

	
	//*************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardRadarFactory::createWindow(const String& name)
    {
        return new FalagardRadar(d_type, name);
    }

    void FalagardRadarFactory::destroyWindow(Window* window)
    {
		delete window;
    }
}