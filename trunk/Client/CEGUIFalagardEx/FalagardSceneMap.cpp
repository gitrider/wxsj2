#include "falagardscenemap.h"
#include "ceguiPropertyHelper.h"
#include "ceguiimagesetmanager.h"
#include "ceguiimageset.h"
#include "ceguianimatemanager.h"
#include "ceguiwindowmanager.h"
#include "windows.h"
#include <math.h>
namespace CEGUI
{

	FalagardSceneMapProperties::PlayerImage			FalagardSceneMap::d_PlayerImageProperty;		// ����õ�ͼ��
	FalagardSceneMapProperties::TeammateImage		FalagardSceneMap::d_TeammateImageProperty;	//
	FalagardSceneMapProperties::MySelfImage			FalagardSceneMap::d_MyselfImageProperty;		// �Լ�����Ҫ��ͼƬ����
	FalagardSceneMapProperties::ImageNameRuler			FalagardSceneMap::d_ImageNameRulerProperty;		// ͼƬ��������

	const String FalagardSceneMap::EventNamespace("Falagard/SceneMap");
	const String FalagardSceneMap::MouseRButtonDownCtrl("MouseRButtonDownCtrl");
	const utf8 FalagardSceneMap::WidgetTypeName[] = "Falagard/SceneMap";
	const int SCENE_MAP_PER_PIC = 64; // ÿһ���ͼƬ��Ӧ���ĵ�ͼ����
	const int SCENE_MAP_ZOOMSCALE = 4;

	FalagardSceneMap::FalagardSceneMap(const String& type, const String& name) :
		IExpProperty(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardSceneMap )
		   CEGUI_ADD_STATICPROPERTY( &d_PlayerImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_MyselfImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_TeammateImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_ImageNameRulerProperty );
		CEGUI_END_ADD_STATICPROPERTY
		
		d_fresh = false;
		d_PlayerDirect = 0;
		d_MyselfAnimate.clear();// = "";
		m_pMySelfWindow = NULL;
		m_pLTWindow		= NULL;
		m_pRBWindow		= NULL;
		m_nXCount		= 0;
		m_nYCount		= 0;
		d_MouseScenePos = Point( 0, 0 );
		d_SceneSize		= Size( 128, 128 );
		d_PlayerImage	= NULL;
		d_TeammateImage = NULL;
		for( int i = 0; i < 5; i ++ )
			m_pFriendWindow[ i ] = NULL;
		m_pBackground = NULL;
		m_WindowZero = Point( 0, 0 );
		m_aPlayerPos.clear();
		m_fZoomValueX = 1; // Ĭ��Ϊһƹȫ����ʾ
		m_fZoomValueY = 1; // Ĭ��Ϊһƹȫ����ʾ
		m_bDraping = false;
		m_bImageRuler_Normal = true;
		m_nDisplayMode = SCENEMAP_DISPLAY_MODE_ABSOLTE;
		setMouseHollow( true );
	}

	FalagardSceneMap::~FalagardSceneMap()
	{
	}
	void FalagardSceneMap::Close()
	{
		Clear();

		uint child_count = getChildCount();
		for (uint i = 0; i < child_count; ++i)
		{
			d_children[i]->hide();

		}
		//WindowManager::getSingleton().destroyWindow( "SceneMapWindow_background" );
		if( m_pMySelfWindow )
		{
			m_pMySelfWindow = NULL;
		}
		for( int i = 0; i < 5; i ++ )
		{
			m_pFriendWindow[ i ] = NULL;
		}

		m_pLTWindow = NULL;
		m_pRBWindow = NULL;
		

	}
	void FalagardSceneMap::addTeammatePos( const Point& pos, const String32& name )
	{
		SCENEMAP_FLAG_DATA  data;
		data.pos = pos;
		data.name = name;
		m_aTeammatePos.push_back( data );
		d_fresh = true;
	}
	void FalagardSceneMap::onMouseButtonDown( MouseEventArgs &e )
	{
		
		// �����ԭ��֮���λ��
		// �õ���ǰ����������������Ͻǵ��Ǹ�λ��
		m_ptDrapOffset = ScreenPosToScenePosRelative( e.position );
		if( e.button == RightButton )
		{
			if( e.sysKeys & Control )
				return;
			m_bDraping = true;
			e.handled = true;
			captureInput();
		}
		Window::onMouseButtonDown( e );
	}
	void FalagardSceneMap::onMouseEnters(MouseEventArgs& e)
	{
		d_MouseScenePos = ScreenPosToScenePos( e.position );
		d_MouseScenePos.d_x /= m_fZoomValueX;
		d_MouseScenePos.d_y /= m_fZoomValueY;
		if( d_MouseScenePos.d_x < 0 || d_MouseScenePos.d_x > d_SceneSize.d_width ||
			d_MouseScenePos.d_y < 0 || d_MouseScenePos.d_y > d_SceneSize.d_height )
		{
			d_MouseScenePos.d_x = 0;
			d_MouseScenePos.d_y = 0;
		}
		Window::onMouseEnters( e );
	}
	void FalagardSceneMap::onMouseButtonUp( MouseEventArgs &e )
	{
		if( e.button == RightButton && ( e.sysKeys & Control ) )
		{
			WindowEventArgs args(this);
			fireEvent(MouseRButtonDownCtrl, args, EventNamespace);
			return;
		}
		m_bDraping = false;
		releaseInput();
		Window::onMouseButtonUp( e );
	}
	void    FalagardSceneMap::onMouseLeaves( MouseEventArgs& e )
	{
		m_bDraping = false;
		d_MouseScenePos = m_PlayerPos;
		Window::onMouseLeaves( e );
	}

	void FalagardSceneMap::onMouseMove( MouseEventArgs &e )
	{
		
		if( m_bDraping ) // �����ж�
		{
			Point tmp = ScreenPosToWindowPosRelative( e.position );
			m_WindowZero.d_x = tmp.d_x - m_ptDrapOffset.d_x;
			m_WindowZero.d_y = tmp.d_y - m_ptDrapOffset.d_y;
			
			UpdateAllWindow();
		}
		else
		{
			d_MouseScenePos = ScreenPosToScenePos( e.position );
			d_MouseScenePos.d_x /= m_fZoomValueX;
			d_MouseScenePos.d_y /= m_fZoomValueY;
			if( d_MouseScenePos.d_x < 0 || d_MouseScenePos.d_x > d_SceneSize.d_width ||
				d_MouseScenePos.d_y < 0 || d_MouseScenePos.d_y > d_SceneSize.d_height )
				return;
		}
		IExpProperty::onMouseMove( e );
	}
	void FalagardSceneMap::updateSelf(float elapsed)
	{
		if( d_fresh && isVisible() )
		{
			int i = 0;
			// �����Լ���ͼƬ
			ResetMyself();
			// ���¶��ѵ�λ�ú���Ϣ

			for( i = 0; i < 5; i ++ )
				m_pFriendWindow[ i ]->hide();
			RENDER_POS::iterator   it;
			String32 strTooltip[ 5 ]; //5���˵�tooltip

			for( i = 0, it = m_aTeammatePos.begin(); it != m_aTeammatePos.end(); it ++, i ++ )
			{
				ResetTeammateWindow( i, ( *it ) );

			}

			for( i = 0, it = m_aPlayerPos.begin(); it != m_aPlayerPos.end(); it ++, i ++ )
			{
				ResetPlayerWindow( i, ( *it ) );

			}

			// ����tooltip 
			
			for( int i = 0; i < 5; i ++ )
			{
				
				if( !m_pFriendWindow[ i ]->isVisible() ) // ����ɼ��ż���
					break;
				strTooltip[ i ] = m_pFriendWindow[ i ]->getTooltipText();
				for( int j = 0; j < 5; j ++ )
				{
					if( j == i )
						continue;
					if( !m_pFriendWindow[ j ]->isVisible() )
						break;
					if( Rect( 0, 0, 0, 0 ) != m_pFriendWindow[ i ]->getRect().getIntersection( m_pFriendWindow[ j ]->getRect() ) )
					{
						if( strTooltip[ i ].size() )
							strTooltip[ i ] += "\r\n";
						strTooltip[ i ] += m_pFriendWindow[ j ]->getTooltipText();
					}

				}
			}
			for( int i = 0; i < 5; i ++ )
			{
				if( !m_pFriendWindow[ i ]->isVisible() ) // ����ɼ��ż���
					break;
				m_pFriendWindow[ i ]->setTooltipText( strTooltip[ i ] );

			}
			d_fresh = false;
			requestRedraw();
		}
	}
	void FalagardSceneMap::setPlayerImage( const Image* pImage )
	{
		
		d_PlayerImage = ( Image* )pImage;
	}

	void FalagardSceneMap::setTeammateImage(const Image* pImage )
	{
		d_TeammateImage = ( Image* )pImage;
	}

	void FalagardSceneMap::setMyselfImage( const String& name )
	{
		d_MyselfAnimate = name;
	}
	void FalagardSceneMap::Clear()
	{
		m_aPlayerPos.clear();
		m_aTeammatePos.clear();
	}
	void    FalagardSceneMap::SetViewByCenter( const Point& pos )
	{
		if( m_pBackground )
		{
			float fWidthWindowPerBackground = getAbsoluteWidth() / m_pBackground->getAbsoluteWidth();
			float fHeightWindowPerBackground = getAbsoluteHeight() / m_pBackground->getAbsoluteHeight();
			m_WindowZero.d_x = -( pos.d_x - fWidthWindowPerBackground / 2 ) / fWidthWindowPerBackground;
			m_WindowZero.d_y = -( pos.d_y - fHeightWindowPerBackground / 2 ) / fHeightWindowPerBackground;
			UpdateAllWindow();
		}
	}
	void FalagardSceneMap::setMyselfDir( const int nDirect )
	{
		d_PlayerDirect = ( nDirect ) % 360; 
		if( d_PlayerDirect < 0 )
			d_PlayerDirect += 360;
		d_fresh = true;
	}
	void FalagardSceneMap::setMyselfPos( const Point& pos )
	{
		m_PlayerPos = pos;
		d_MyseflPos = ScenePosToWindowPos( pos );	
		d_fresh = true;
	}
	void FalagardSceneMap::SetSceneFileName( const String& str ,const String& strPostFix)
	{
		CreateSceneMap( str.c_str() ,strPostFix.c_str());
	}
	void FalagardSceneMap::onParentSized( WindowEventArgs &e )
	{
		IExpProperty::onParentSized( e );
		UpdateAllWindow();
	}
	// �õ���ǰ�ӿ������ڱ����ϵ�λ��
	Point	FalagardSceneMap::GetCurrentCenter()
	{
		Point pos;
		if(m_pBackground)
		{
			float fWidthWindowPerBackground = getAbsoluteWidth() / m_pBackground->getAbsoluteWidth();
			float fHeightWindowPerBackground = getAbsoluteHeight() / m_pBackground->getAbsoluteHeight();
			pos.d_x = -m_WindowZero.d_x * fWidthWindowPerBackground + fWidthWindowPerBackground / 2;
			pos.d_y = -m_WindowZero.d_y * fHeightWindowPerBackground + fHeightWindowPerBackground / 2 ;
		}
		return pos;
	}
	// �õ���ǰ�ӿ��ڱ����ϵĿɼ���Χ
	Rect FalagardSceneMap::GetCurrentViewRect() 
	{
		Rect rect;
		if(m_pBackground)
		{
			float fWidthWindowPerBackground = getAbsoluteWidth() / m_pBackground->getAbsoluteWidth();
			float fHeightWindowPerBackground = getAbsoluteHeight() / m_pBackground->getAbsoluteHeight();
			rect.d_left		= -m_WindowZero.d_x * fWidthWindowPerBackground;
			rect.d_top		= -m_WindowZero.d_y * fHeightWindowPerBackground;
			rect.d_right	= rect.d_left + fWidthWindowPerBackground;
			rect.d_bottom	= rect.d_top + fHeightWindowPerBackground;
		}
		return rect;
	}
	void  FalagardSceneMap::UpdateAllWindow()
	{
		if( m_pBackground == NULL )
			return;
		// �������ű������õ�ͼ
		// �����1:1����������ȫ��
		float fMax = ( d_SceneSize.d_width > d_SceneSize.d_height ) ? d_SceneSize.d_width : d_SceneSize.d_height;
		if( m_nDisplayMode == SCENEMAP_DISPLAY_MODE_FULLSCREEN ) 
		{
			float fScale = getAbsoluteHeight() / getAbsoluteWidth();
			m_fZoomValueX = fScale * d_SceneSize.d_width / fMax ;
			m_fZoomValueY = d_SceneSize.d_height / fMax;
		}
		else 
		{
			m_fZoomValueX = d_SceneSize.d_width * SCENE_MAP_ZOOMSCALE / getAbsoluteWidth(); 
			m_fZoomValueY = d_SceneSize.d_height * SCENE_MAP_ZOOMSCALE / getAbsoluteHeight(); 
		}
		
		Rect winRect;
		winRect = Rect( 0, 0, m_fZoomValueX, m_fZoomValueY );
		if( m_fZoomValueX < 1 ) // ˵����Ȳ���,�͵þ�����ʾ��									
		{
			winRect.d_left = ( 1 - m_fZoomValueX ) / 2;
			winRect.d_right = winRect.d_left + m_fZoomValueX;
			m_WindowZero.d_x = winRect.d_left;
		}
		else
		{
			if( m_WindowZero.d_x < 1 - m_fZoomValueX  )
				m_WindowZero.d_x = 1 - m_fZoomValueX;
			if( m_WindowZero.d_x > 0 )
				m_WindowZero.d_x = 0;
		}
		if( m_fZoomValueY < 1 )
		{
			winRect.d_top = ( 1 - m_fZoomValueY ) / 2;
			winRect.d_bottom = winRect.d_top + m_fZoomValueY;
			m_WindowZero.d_y = winRect.d_top;
		}
		else
		{
			if( m_WindowZero.d_y < 1 - m_fZoomValueY )
				m_WindowZero.d_y = 1 - m_fZoomValueY;
			if( m_WindowZero.d_y > 0 )
				m_WindowZero.d_y = 0;
		}
		m_pBackground->setRect( Relative, winRect );
		m_pBackground->setSize( Relative, winRect.getSize() );
		
		m_pBackground->setPosition( Relative, m_WindowZero );
		m_pBackground->show();
		UpdateBackgroundItem();
		requestRedraw();
		d_fresh = true;
	}
	void FalagardSceneMap::UpdateBackgroundItem()
	{
		int i = 0;
		char szWindowName[ 128 ];
		float fOffsetX, fOffsetY; // ���Ǹ�����ֵ
	
		fOffsetX = float( ( int( d_SceneSize.d_width ) % SCENE_MAP_PER_PIC ) / float( SCENE_MAP_PER_PIC ) );  
		fOffsetY = float( ( int( d_SceneSize.d_height ) % SCENE_MAP_PER_PIC ) / float( SCENE_MAP_PER_PIC ) );
		m_nXCount =  int( d_SceneSize.d_width ) / SCENE_MAP_PER_PIC;
		m_nYCount =  int( d_SceneSize.d_height ) / SCENE_MAP_PER_PIC;
		if( fOffsetX ) // ����ж���Ͷ��һ��
			m_nXCount += 1;
		else 
			fOffsetX = 1;
		if( fOffsetY )
			m_nYCount += 1;
		else 
			fOffsetY = 1;
		Size size;
		float fPicSizeX = 256, fPicSizeY = 256;
		if( m_nDisplayMode == SCENEMAP_DISPLAY_MODE_FULLSCREEN )
		{
			float fMax = ( d_SceneSize.d_width > d_SceneSize.d_height ) ? d_SceneSize.d_width : d_SceneSize.d_height;
			float fScale = getAbsoluteHeight() / getAbsoluteWidth();
			fPicSizeX *= fScale * getAbsoluteWidth() / ( fMax * SCENE_MAP_ZOOMSCALE ); 
			fPicSizeY *= getAbsoluteHeight() / ( fMax * SCENE_MAP_ZOOMSCALE );
			
			//ȡ��
			fPicSizeY  = PixelAligned(fPicSizeY);
			fPicSizeX = PixelAligned(fPicSizeX);
		}
		Point pos( 0, 0);
		for( i = 0; i < m_nXCount; i ++ )
		{
			if( i == m_nXCount - 1 && fOffsetX != 1 ) // �����x���һ��,����ͼ�ж����
				size.d_width = float( fOffsetX * fPicSizeX );
			else
				size.d_width = fPicSizeX;
			
			for( int j = 0; j < m_nYCount; j ++ ) // �������е�ͼƬ��������Ӧ�ô���
			{
				
				if( j == m_nYCount - 1 && fOffsetY != 1 )
					size.d_height = float( fOffsetY * fPicSizeY);
				else
					size.d_height = fPicSizeY;

				sprintf( szWindowName, "%s_map_%d_%d", getName().c_str(),i, j ); 
				Window* pWindow = (StaticImage*)WindowManager::getSingleton().getWindow( szWindowName );
				if( pWindow )
				{
					pWindow->setPosition( Absolute, pos );
					pWindow->setSize( Absolute, size );
				}
				pos.d_y += size.d_height;

			}
			pos.d_x += size.d_width; 
			pos.d_y = 0;
		}
	}
	void FalagardSceneMap::SetZoomMode( int nMode )
	{ 
		m_nDisplayMode = nMode; 
		UpdateAllWindow();
	};
	const Image* FalagardSceneMap::getImageByName( const String& str,const String& strPostFix,Rect rect ) // �������ֵõ�image
	{
		const String imagename = "Scenemap_view"; // ���еĵ�ͼ����������֣�
		Imageset* pSet;
		char szFullFileName[ 128 ];
		sprintf( szFullFileName, "%s.%s", str.c_str(),strPostFix.c_str() );
		Point offset( 0, 0 );
		if( !ImagesetManager::getSingleton().isImagesetPresent( str ) )
		{
			pSet = ImagesetManager::getSingleton().createImagesetFromImageFile( str.c_str(), szFullFileName, "Scene" );
		}
		pSet = ImagesetManager::getSingleton().getImageset( str );
		if( pSet ) 
		{
			if( !pSet->isImageDefined( imagename ) )
				pSet->defineImage( imagename, rect, offset );
			return &pSet->getImage( imagename );
		}
		return NULL;
		
	}
	void	FalagardSceneMap::createBackground( const String& strFileName,const String& strPostFix,bool bIsRadar)
	{
		// ��������Ϊ128 X 128��С�飬��ӦͼƬ��256X256
		int i = 0;
		char szImagesetName[ 128 ];
		char szWindowName[ 128 ];

		//Window* pWindowBack = NULL;
		sprintf( szWindowName, "%s_auto_background", getName().c_str() ); 
		if( WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //������ڴ��ڣ���ɾ��
		{
			
			WindowManager::getSingleton().destroyWindow( szWindowName );
			
		}
		if(bIsRadar)
			WindowManager::getSingleton().createWindow( "Falagard/Radar/ParentWindow", szWindowName );
		else 
			WindowManager::getSingleton().createWindow( "DefaultWindow", szWindowName );

		m_pBackground = WindowManager::getSingleton().getWindow( szWindowName );
		addChildWindow( m_pBackground );

		m_pBackground->setMaximumSize( Size( 100, 100 ) );
		m_pBackground->setRiseOnClickEnabled( false );
		m_pBackground->setMouseMoveHollow( true );
		
		
		

		float fOffsetX, fOffsetY; // ���Ǹ�����ֵ
		float fPicSizeX = 256, fPicSizeY = 256;
		fOffsetX = float( ( int( d_SceneSize.d_width ) % SCENE_MAP_PER_PIC ) / float( SCENE_MAP_PER_PIC ) );  
		fOffsetY = float( ( int( d_SceneSize.d_height ) % SCENE_MAP_PER_PIC ) / float( SCENE_MAP_PER_PIC ) );
		m_nXCount =  int( d_SceneSize.d_width ) / SCENE_MAP_PER_PIC;
		m_nYCount =  int( d_SceneSize.d_height ) / SCENE_MAP_PER_PIC;
		if( fOffsetX ) // ����ж���Ͷ��һ��
			m_nXCount += 1;
		else 
			fOffsetX = 1;
		if( fOffsetY )
			m_nYCount += 1;
		else 
			fOffsetY = 1;
			

		float fWidthPerItem = 1 / d_SceneSize.d_width;
		float fHeightPerItem = 1 / d_SceneSize.d_height;
		int nIndex = 1;
		for( int j = 0; j < m_nYCount; j ++ ) // �������е�ͼƬ��������Ӧ�ô���
		{
			for( i = 0; i < m_nXCount; i ++ )
			{
				if( m_bImageRuler_Normal )
				{
					sprintf( szImagesetName, "%s_%d_%d", strFileName.c_str(),i, j );
				}
				else
				{
					sprintf( szImagesetName, "%s_%02d", strFileName.c_str(), nIndex );
				}
				
				Rect rect( 0, 0, float( fPicSizeX ), float( fPicSizeY ) );
				Point offset( 0, 0 );
				if( i == m_nXCount - 1 && fOffsetX != 1 ) // �����x���һ��,����ͼ�ж����
					rect.d_right = float( fOffsetX * fPicSizeX );
				if( j == m_nYCount - 1 && fOffsetY != 1 )
					rect.d_bottom = float( fOffsetY * fPicSizeY);
				// �������ͼƬ�������ô���
				sprintf( szWindowName, "%s_map_%d_%d", getName().c_str(),i, j ); 
				Window* pWindow = NULL;
				if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //������ڲ����ڣ��ʹ���һ��
				{
					pWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
					m_pBackground->addChildWindow( pWindow );
				}
				pWindow = WindowManager::getSingleton().getWindow( szWindowName );
				if( pWindow ) 
				{
					((StaticImage*)pWindow)->setImage( getImageByName( szImagesetName,strPostFix,rect ) );
					pWindow->setRiseOnClickEnabled( false );
					pWindow->setMouseHollow( true );
				}
				if( i == 0 && j == 0 ) //���ϽǴ���
					m_pLTWindow = pWindow;
				if( i == m_nXCount - 1 && j == m_nYCount - 1 ) // ���´���
					m_pRBWindow = pWindow;

				// �����ͼ���������εģ�
				CEGUI::Rect winRect;
				winRect.d_left= i * SCENE_MAP_PER_PIC / d_SceneSize.d_width;
				winRect.d_top = j * SCENE_MAP_PER_PIC / d_SceneSize.d_height;
				winRect.d_right = winRect.d_left + ( fOffsetX * SCENE_MAP_PER_PIC) / d_SceneSize.d_width;
				winRect.d_bottom = winRect.d_top + ( fOffsetY * SCENE_MAP_PER_PIC) / d_SceneSize.d_height;
				pWindow->setRect( Relative, winRect );
				pWindow->setSize( Relative, winRect.getSize() );
				pWindow->show();
				pWindow->setMouseHollow( true ); 
				pWindow->setMouseMoveHollow( true ); 
				pWindow->setMaximumSize( Size( 100, 100 ) );
				pWindow->setRiseOnClickEnabled( false );
				
				nIndex++;
			}
		}

	}
	void FalagardSceneMap::CreateSceneMap( const String& strFileName,const String& strPostFix)
	{
		char szWindowName[ 128 ];
		int i;
		createBackground( strFileName,strPostFix);
		for( i = 0; i < 5; i ++ )
		{
			sprintf( szWindowName, "Scenemap_Player_%d", i );
			if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //������ڲ����ڣ��ʹ���һ��
				WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
			m_pFriendWindow[ i ] = (StaticImage*)WindowManager::getSingleton().getWindow( szWindowName );
			m_pBackground->addChildWindow( m_pFriendWindow[ i ] );
			m_pFriendWindow[ i ]->hide();
			//m_pFriendWindow[ i ]->setAlwaysOnTop( true );
			((StaticImage*)m_pFriendWindow[ i ])->setImage( d_PlayerImage );
			

		}
		sprintf( szWindowName, "%s_Auto_MyselfWindow", getName().c_str() );  
		if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //������ڲ����ڣ��ʹ���һ��
			WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
		m_pMySelfWindow = (StaticImage*)WindowManager::getSingleton().getWindow( szWindowName );
		m_pBackground->addChildWindow( m_pMySelfWindow );
		m_pMySelfWindow->setAlwaysOnTop( true );
		m_pMySelfWindow->show();
		m_pBackground->setMouseHollow( true );
		m_pBackground->setMouseMoveHollow( true );

		UpdateAllWindow();
		//SetViewByCenter( d_MyseflPos );
			// ����5�����ѵô���
		
	}
	void	FalagardSceneMap::setSceneMapCenter()
	{
		SetViewByCenter( d_MyseflPos );
	}
	void FalagardSceneMap::ResetMyself()
	{
		Animate* pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_MyselfAnimate );
		if( pAnimate && m_pBackground && m_pMySelfWindow )
		{
			int nCurFrame = ( d_PlayerDirect * pAnimate->getFrameNumber() ) / 360;
			Image* pPlayerImage = (Image*)( pAnimate->getFrame( nCurFrame ) );
			
			Point curPos = d_MyseflPos;
			Rect rect;
			float fWidth = ( pPlayerImage->getWidth() / m_pBackground->getAbsoluteWidth() ) / 2;
			float fHeight = ( pPlayerImage->getHeight() / m_pBackground->getAbsoluteHeight() ) / 2;
			rect.d_left   = curPos.d_x -  fWidth;
			rect.d_right  = curPos.d_x + fWidth;
			rect.d_top    = curPos.d_y - fHeight;
			rect.d_bottom = curPos.d_y + fHeight; 
			(( StaticImage* )m_pMySelfWindow)->setImage( pPlayerImage ) ;
			m_pMySelfWindow->setRect( Relative, rect );
			m_pMySelfWindow->setSize( Relative, rect.getSize() );
			
		}
	}
	// ���¼�����Ҵ��ڵ�λ��
	void FalagardSceneMap::ResetPlayerWindow( int nIndex, SCENEMAP_FLAG_DATA& data )
	{
		Point curPos = ScenePosToWindowPos( data.pos );
		Rect rect;
		float fWidth = ( d_PlayerImage->getSourceTextureArea().getWidth() / m_pBackground->getAbsoluteWidth() ) / 2;
		float fHeight = ( d_PlayerImage->getSourceTextureArea().getHeight() / m_pBackground->getAbsoluteHeight() ) / 2;
		rect.d_left   = curPos.d_x - fWidth / 2;
		rect.d_right  = curPos.d_x + fWidth / 2;
		rect.d_top    = curPos.d_y - fHeight / 2;
		rect.d_bottom = curPos.d_y + fHeight / 2;
	
		//m_pFriendWindow[ nIndex ]->show();
		//m_pFriendWindow[ nIndex ]->setRect( Relative, rect );
		//m_pFriendWindow[ nIndex ]->setSize( Relative, rect.getSize() );

		//m_pFriendWindow[ nIndex ]->setTooltipText( data.name );
	}

	// ���¼�����Ҵ��ڵ�λ��
	void FalagardSceneMap::ResetTeammateWindow( int nIndex, SCENEMAP_FLAG_DATA& data )
	{
		Point curPos = ScenePosToWindowPos( data.pos );
		Rect rect;
		float fWidth = ( d_TeammateImage->getSourceTextureArea().getWidth() / m_pBackground->getAbsoluteWidth() );
		float fHeight = ( d_TeammateImage->getSourceTextureArea().getHeight() / m_pBackground->getAbsoluteHeight() );
		rect.d_left   = curPos.d_x - fWidth / 2;
		rect.d_right  = curPos.d_x + fWidth / 2;
		rect.d_top    = curPos.d_y - fHeight / 2;
		rect.d_bottom = curPos.d_y + fHeight / 2;

		m_pFriendWindow[ nIndex ]->show();
		m_pFriendWindow[ nIndex ]->setRect( Relative, rect );
		m_pFriendWindow[ nIndex ]->setSize( Relative, rect.getSize() );

		m_pFriendWindow[ nIndex ]->setTooltipText( data.name );
	}

	void FalagardSceneMap::moveMapByDirection(int direction, int step)
	{
		Point pt(step,step);

		Point tmp = ScenePosToWindowPos( pt );

		switch(direction)
		{
		case 1:
			tmp.d_y = -tmp.d_y;
			tmp.d_x = 0;
			break;
		case 2:
			tmp.d_x = 0;
			break;
		case 3:
			tmp.d_y = 0;
			break;
		case 4:
			tmp.d_x = -tmp.d_x;
			tmp.d_y = 0;
			break;
		default:
			return;
		}

		m_WindowZero.d_x -= tmp.d_x;
		m_WindowZero.d_y -= tmp.d_y;

		UpdateAllWindow();
		
	}

	Point FalagardSceneMap::ScreenPosToScenePos( const Point& pos ) // ��Ļ����ת������������
	{

		Point tmp;
		float fMapScreenWidth = getPixelRect().getWidth();
		float fMapScreenHeight = getPixelRect().getHeight();
		// �Եõ���������Ͻǵ���Ļƫ����,����ֵ
		tmp.d_x = ( pos.d_x - getPixelRect().d_left ) / fMapScreenWidth - m_WindowZero.d_x;
		tmp.d_y = ( pos.d_y - getPixelRect().d_top ) / fMapScreenHeight - m_WindowZero.d_y;

		tmp.d_x *= d_SceneSize.d_width;
		tmp.d_y *= d_SceneSize.d_height;
		return tmp;
	}

	Point FalagardSceneMap::ScreenPosToWindowPosRelative( const Point& pos ) // ��Ļ����ת������������f
	{
		Point tmp;
		float fMapScreenWidth = getPixelRect().getWidth();
		float fMapScreenHeight = getPixelRect().getHeight();
		// �Եõ���������Ͻǵ���Ļƫ����,����ֵ
		tmp.d_x = ( pos.d_x - getPixelRect().d_left ) / fMapScreenWidth;
		tmp.d_y = ( pos.d_y - getPixelRect().d_top ) / fMapScreenHeight;
		return tmp;
	}
	Point FalagardSceneMap::ScreenPosToScenePosRelative( const Point& pos ) // ��Ļ����ת������������
	{
		Point tmp;
		float fMapScreenWidth = getPixelRect().getWidth();
		float fMapScreenHeight = getPixelRect().getHeight();
		// �Եõ���������Ͻǵ���Ļƫ����,����ֵ
		tmp.d_x = ( pos.d_x - getPixelRect().d_left ) / fMapScreenWidth - m_WindowZero.d_x;
		tmp.d_y = ( pos.d_y - getPixelRect().d_top ) / fMapScreenHeight - m_WindowZero.d_y;
		return tmp;
	}
	Point FalagardSceneMap::ScenePosToWindowPos( const Point& pos )
	{
		Point tmp;
		tmp.d_x = pos.d_x / d_SceneSize.d_width;
		tmp.d_y = pos.d_y / d_SceneSize.d_height;
		return tmp;
	}

	namespace FalagardSceneMapProperties
	{
		//////////////////////////////////////////////////////////////////////////
		String PlayerImage::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::imageToString(static_cast<const FalagardSceneMap*>(receiver)->getObjImage());
		}

		void PlayerImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSceneMap*>(receiver)->setPlayerImage(PropertyHelper::stringToImage(value));
		}

		//////////////////////////////////////////////////////////////////////////
		String TeammateImage::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::imageToString(static_cast<const FalagardSceneMap*>(receiver)->getObjImage());
		}

		void TeammateImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSceneMap*>(receiver)->setTeammateImage(PropertyHelper::stringToImage(value));
		}

		//////////////////////////////////////////////////////////////////////////
		String MySelfImage::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void MySelfImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSceneMap*>(receiver)->setMyselfImage( value ) ;
		}
		String   ImageNameRuler::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardSceneMap*>(receiver)->getImageRulerNormal());
		}
		void   ImageNameRuler::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSceneMap*>(receiver)->seImageRulerNormal(PropertyHelper::stringToBool(value));
		}


	};
 //////////////////////////////////////////////////////////////////////////
    Window* FalagardScenemapFactory::createWindow(const String& name)
    {
        return new FalagardSceneMap(d_type, name);
    }

    void FalagardScenemapFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }
};