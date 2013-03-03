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
			Window*		pCityWindow;		// ���еô���
			Window*		pCityNameWindow;	// �������ֵô���
			Window*		pFriendWindow;		// ���ѵô���
			Window*		pMyselfWindow;
			Point		cityPos;			// ���е�λ��
			Point		namePos;			// �������ֵ�λ��
			String		szType;				// ���е�����
			String		szCityWindowName;
			String		szCityNameWindowName;
			String		szFriendWindowName;
			String		szMyselfWindowName;
			int			nFriendNumber;		// �м�������
			int 		bIsCurrentScene;	// �ǲ��ǵ�ǰ����
			Image*		pNameNormalImage;	// ��������ͼ��
			Image*		pNameHoverImage;	// ���ָ���ͼ��
			String32		pTooltip;			// ��ǰ��tooltip
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

		/********** ���(_WX@7)�����һЩ���ú��� *******************************/
		virtual void setCityTooltipText( int nCityID, const String32& szCityTip);
		virtual void setCityNameTooltipText( int nCityID, const String32& szCityNameTip );
		/************************************************************************/

		
		typedef std::map< String, S_CITY_DEFINE >  CITY_DEFINE;
		typedef std::map< int, CITY_DATA* >  CITY_DATA_MAP;

		// 5�ֶ��ѵ�ͼƬ
		Image*			m_FriendImage[ 5 ];
		Image*			m_MyselfImage;


		CITY_DEFINE		m_CityDefineList;
		CITY_DATA_MAP	m_aCityList;	// ���еó��е��б�

		int				m_nCurrentScene; // ��ǰѡ��ĳ���
		int				m_nLastScene;

		void			UpdateMouseMove( const Point& pos );
		virtual void			SetCityHover( int nSceneID, bool bHover );

		String			m_strBackgroundName; // ����ͼƬ�ĸ�
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
		/* ˵��
			���ӱ���ж�ȡ���ĳ��е����λ����Ϣ��ת��Ϊ��Ļ���������
			
		/* ����wnd	
			����ͼ�꣬������������ͼ�� ���ڵ�ָ��

		/* ����point
			�ӱ���ж�ȡ������λ����Ϣ

		/* ����image
			�����õ��������ָ��

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