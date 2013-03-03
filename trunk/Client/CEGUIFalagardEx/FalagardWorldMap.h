#pragma once
#include "falstaticimage.h"
#include "falagardworldmapproperties.h"
#include "IFalagardWorldmap.h"
namespace CEGUI
{
	/*----------------------------------------------------------------------*/
	class FALAGARDBASE_API FalagardWorldMap :public FalagardStaticImage, public IFalagardWorldMap
	{
		struct S_CITY_DEFINE
		{
			Image* pNormalImage;
			Image* pHoverImage;
		};
		struct CITY_DATA
		{
			Window*		pCityWindow;		// 城市得窗口
			Window*		pCityNameWindow;	// 城市名字得窗口
			Window*		pFriendWindow;		// 队友得窗口
			Window*		pMyselfWindow;
			Point		cityPos;			// 城市得位置
			Point		namePos;			// 城市名字得位置
			String		szType;				// 城市得类型
			String		szCityWindowName;
			String		szCityNameWindowName;
			String		szFriendWindowName;
			String		szMyselfWindowName;
			int			nFriendNumber;		// 有几个队友
			int 		bIsCurrentScene;	// 是不是当前场景
			Image*		pNameNormalImage;	// 名字正常图标
			Image*		pNameHoverImage;	// 名字高亮图标
			String32		pTooltip;			// 当前得tooltip
			CITY_DATA()
			{
				pCityWindow = NULL;
				pCityNameWindow = NULL;	
				pFriendWindow = NULL;		
				szType="";				
				szCityWindowName.clear();//
				szCityNameWindowName.clear();
				szFriendWindowName.clear();
				nFriendNumber = 0;		
				bIsCurrentScene = false;	
				pNameNormalImage = NULL;	
				pNameHoverImage = NULL;	
				pTooltip.clear();
			};

		};	

		CITY_DATA* getCity( int nCityID );
		void createNewCity( int nCityID, CITY_DATA* data );

		float AbsoluteToRelativeX( float fValue );
		float AbsoluteToRelativeY( float fValue );
		float AbsoluteToRelativeX2( float fValue );
		float AbsoluteToRelativeY2( float fValue );

	protected:
		virtual void	onParentSized(WindowEventArgs& e);
	public:
		static const utf8   WidgetTypeName[];       

		void setMySelfImage( const Image* pImage );
		void setCityNormalImage( const String& szType, const Image* pImage );
		void setCityHoverImage( const String& szType, const Image* pImage );

		void setFriendImage( int nFriend, const Image* pImage );


		virtual void setCityPos( int nCityID, const Point& pos );
		virtual void setCityNamePos( int nCityID, const Point& pos );
		virtual void setCityType( int nCityID,const String& szType );
		virtual void setCityCurrentScene( int nCityID, int bCurrentScene );
		virtual void setCityNameNormalImage( int nCityID, const String& szImageset, const String& szImage );
		virtual void setCityNameHoverImage( int nCityID, const String& szImageset, const String& szImage );
		virtual void addPlayerToCity( int nCityID, const String& szPlayerName );
		virtual void resetWorldMap();
		virtual void clearPlayerInfo();
		virtual int	 getCurrentSelectScene() { return m_nCurrentScene; };

		/********** 标记(_WX@7)，添加一些设置函数 *******************************/
		virtual void setCityTooltipText( int nCityID, const String32& szCityTip);
		virtual void setCityNameTooltipText( int nCityID, const String32& szCityNameTip );
		/************************************************************************/

		
		typedef std::map< String, S_CITY_DEFINE >  CITY_DEFINE;
		typedef std::map< int, CITY_DATA* >  CITY_DATA_MAP;

		// 5种队友得图片
		Image*			m_FriendImage[ 5 ];
		Image*			m_MyselfImage;


		CITY_DEFINE		m_CityDefineList;
		CITY_DATA_MAP	m_aCityList;	// 所有得城市得列表

		int				m_nCurrentScene; // 当前选择的场景
		int				m_nLastScene;

		void			UpdateMouseMove( const Point& pos );
		virtual void			SetCityHover( int nSceneID, bool bHover );

		String			m_strBackgroundName; // 背景图片的根
		void			setBackgroundName( const String& strName );

		Size			m_BackGroundSize;
		void			setBackgroundSize( const Size& size );

		virtual void	createBackground();
		void			updataBackground();

		Window*			m_pBackground;

		bool			cityHover(const EventArgs& e);
		bool			cityNormal(const EventArgs& e);
		bool			onDrapMove( const EventArgs& e);
		bool			onDrapStart( const EventArgs& e);
		bool			onDrapEnd( const EventArgs& e);

		Point			m_ptZero;

		bool			m_bDraping;
		Point			m_ptDrapOffset;
		Point			m_ptDrapOldPos;

		/************************************************************************/
		/* 说明
			将从表格中读取到的城市的相关位置信息，转换为屏幕的相对坐标
			
		/* 参数wnd	
			城市图标，或者是其名字图标 窗口的指针

		/* 参数point
			从表格中读取到的其位置信息

		/* 参数image
			其所用到的纹理的指针

		/************************************************************************/
		void			tranpoint( Window* wnd, const Point point, Image* image );
	public:
		FalagardWorldMap(const String& type, const String& name);
		~FalagardWorldMap(void);

		static FalagardWorldMapProperties::CityImageNormal			d_CityImageNormalProperty;
		static FalagardWorldMapProperties::CityImageHover			d_CityImageHoverProperty;
		static FalagardWorldMapProperties::FriendImage1				d_FriendImage1Property;
		static FalagardWorldMapProperties::FriendImage2				d_FriendImage2Property;
		static FalagardWorldMapProperties::FriendImage3				d_FriendImage3Property;
		static FalagardWorldMapProperties::FriendImage4				d_FriendImage4Property;
		static FalagardWorldMapProperties::FriendImage5				d_FriendImage5Property;
		static FalagardWorldMapProperties::BackgroundSize			d_BackgroundSizeProperty;
		static FalagardWorldMapProperties::BackgroundName			d_BackgroundNameProperty;
		static FalagardWorldMapProperties::MySelfImage				d_MySelfImageProperty;
	
		
	};
	/*----------------------------------------------------------------------*/
	class FALAGARDBASE_API FalagardWorldMapFactory : public WindowFactory
    {
    public:
        FalagardWorldMapFactory(void) : WindowFactory(FalagardWorldMap::WidgetTypeName) { }
        ~FalagardWorldMapFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
	/*----------------------------------------------------------------------*/
};