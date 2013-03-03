

#pragma once

#include "falmodule.h"
#include "ceguiwindowfactory.h"
#include "falstaticimage.h"
#include "FalMinimapProperties.h"
#include "IFalagardSceneMapWindow.h"
#include "IExpProperty.h"

namespace CEGUI
{

	namespace FalagardSceneMapProperties
	{
		class PlayerImage: public Property
		{
		public:
			PlayerImage() : Property(
				"PlayerImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class TeammateImage: public Property
		{
		public:
			TeammateImage() : Property(
				"TeammateImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
		class MySelfImage: public Property
		{
		public:
			MySelfImage() : Property(
				"MySelfImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class ImageNameRuler: public Property
		{
		public:
			ImageNameRuler() : Property(
				"ImageNameRuler",
				"Property to get/set Image Name Ruler.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};



	}
	/*
	设计思路
	把地图分成256X256的小块，地图和图片有一个固定的比例，比如，每256象素的图片对应地图上的64单位，这样可以用来定义所有的地图块的命名规则

	*/
	enum 
	{
		SCENEMAP_DISPLAY_MODE_FULLSCREEN = 0,	// 全屏
		SCENEMAP_DISPLAY_MODE_ABSOLTE,		// 真是象素比例
	};
	class FALAGARDBASE_API FalagardSceneMap :	public IExpProperty, public IFalagardSceneMapWindow
	{
	public:

		static const utf8   WidgetTypeName[];       

		static FalagardSceneMapProperties::TeammateImage d_TeammateImageProperty;	//队友用得图标
		static FalagardSceneMapProperties::PlayerImage		d_PlayerImageProperty;		// 玩家用得图标
		static FalagardSceneMapProperties::MySelfImage		d_MyselfImageProperty;		// 自己所需要得图片动画
		static FalagardSceneMapProperties::ImageNameRuler		d_ImageNameRulerProperty;		// 名字类型

		void   Close();

		virtual void onMouseMove( MouseEventArgs &e );
		virtual void onMouseButtonDown( MouseEventArgs &e );
		virtual void onMouseButtonUp( MouseEventArgs &e );
		virtual void onMouseLeaves( MouseEventArgs& e );
		virtual	void updateSelf(float elapsed); 
		virtual void onMouseEnters(MouseEventArgs& e);

		// 得到鼠标所点得位置
		virtual const Point GetMouseScenePos() const { return d_MouseScenePos; };
		//
		void setPlayerImage( const Image* pImage );

		void setTeammateImage( const Image* pImage );

		void setMyselfImage( const String& name );

		void seImageRulerNormal( const bool rul ) { m_bImageRuler_Normal = rul; }
		bool getImageRulerNormal(void) const { return m_bImageRuler_Normal; }

		//设置自己的方向
		virtual void setMyselfDir( const int nDirect );
		// 设置自己的位置
		virtual void setMyselfPos( const Point& pos );
		// 增加一个队友的位置
		virtual void addTeammatePos( const Point& pos, const String32& name );
		// 清除队友的数据
		virtual void Clear();
		// 设置场景的尺寸
		virtual void SetSceneSize( Size size ) { d_SceneSize = size ; };
		//  设置场景的文件
		virtual void SetSceneFileName( const String& str,const String& strPostFix);

		virtual void SetZoomMode( int nMode );// { m_nDisplayMode = nMode; };
		virtual void moveMapByDirection(int direction, int step);

		virtual void setMinimapData( int nType, float fX, float fY, String32& szName  ){};
	protected:
		virtual void	onParentSized(WindowEventArgs& e);
		const Image*	getImageByName( const String& str,const String& strPostFix,Rect rect ); // 根据名字得到image
		void			createBackground( const String& strFileName,const String& strPostFix,bool bIsRadar=false);
		void			SetViewByCenter( const Point& pos ); // 设置当前窗口可见范围为以这个位置为中心
		Rect			GetCurrentViewRect(); // 得到当前的可见范围
		Point			GetCurrentCenter();
		virtual	void	setSceneMapCenter();
	protected:

		struct SCENEMAP_FLAG_DATA
		{
			Point   pos;
			String32  name;
		};
		typedef std::list< SCENEMAP_FLAG_DATA >  RENDER_POS;
		RENDER_POS	m_aPlayerPos;
		RENDER_POS	m_aTeammatePos;

		int			d_PlayerDirect;			// 人物的角度
		Point		d_MyseflPos;
		bool		d_fresh;

		Image*		d_PlayerImage;
		Image*		d_TeammateImage;
		String		d_MyselfAnimate;
		
		Window*		m_pFriendWindow[ 5 ]; // 5个队友得窗口
		Window*		m_pMySelfWindow; // 自己的窗口
		Window*		m_pLTWindow; // 左上角得地图窗口
		Window*		m_pRBWindow;// 右下角得地图窗口
		Window*		m_pBackground;

		int		m_nXCount;
		int		m_nYCount;
		Point	d_MouseScenePos;
		Point	m_PlayerPos;			// 脱放时记录当前人物位置
		Point   m_WindowZero; // 窗口的坐标原点
		// 场景尺寸
		Size	d_SceneSize;
		// 创建场景地图
		virtual void CreateSceneMap( const String& strFileName,const String& strPostFix);
		void ResetPlayerWindow( int nIndex, SCENEMAP_FLAG_DATA& data );
		void ResetTeammateWindow( int nIndex, SCENEMAP_FLAG_DATA& data );
		Point ScenePosToWindowPos( const Point& pos ); // 场景坐标转到窗口坐标
		Point ScreenPosToScenePos( const Point& pos ); // 屏幕坐标转换到场景坐标f
		Point ScreenPosToScenePosRelative( const Point& pos ); // 屏幕坐标转换到场景坐标
		Point ScreenPosToWindowPosRelative( const Point& pos ); // 屏幕坐标转换到场景坐标
		Point ScreenPosToPicPos( const Point& pos ); // 屏幕坐标转换到图片坐标
		void  ResetMyself();
		void  UpdateAllWindow();
		void  UpdateBackgroundItem();

		float m_fZoomValueX;		// 缩放比例，可变的
		float m_fZoomValueY;		// 缩放比例，可变的
		bool  m_bDraping;
		bool m_bImageRuler_Normal;		//图片以'_'隔开
		Point m_ptDrapOffset;
		int   m_nDisplayMode; // 显示模式，分为固定比例和全屏比例
	public:
		FalagardSceneMap( const String& type, const String& name );
		~FalagardSceneMap();

		static const String EventNamespace;
		static const String MouseRButtonDownCtrl;

	};

	class FALAGARDBASE_API FalagardScenemapFactory : public WindowFactory
    {
    public:
        FalagardScenemapFactory(void) : WindowFactory(FalagardSceneMap::WidgetTypeName) { }
        ~FalagardScenemapFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};