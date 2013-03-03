#pragma once
#include "falagardscenemap.h"
#include "IFalagardMinimapWindow.h"
#include <vector>
namespace CEGUI
{
	namespace FalagardRadarProperties
	{
		class IconImage: public Property
		{
			public:
			IconImage() : Property(
				"IconImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class AnimateImage: public Property
		{
			public:
			AnimateImage() : Property(
				"AnimateImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class AutoReset: public Property
		{
			public:
			AutoReset() : Property(
				"AutoReset",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class RoundEnabled: public Property
		{
		public:
			RoundEnabled() : Property(
				"RoundEnabled",
				"",
				"false")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
	struct MINIMAP_FLAG_DATA
	{
		Point   pos;
		String32  name;
		Size    size;
		Window* m_pWindow;
		MINIMAP_FLAG_DATA()
		{
			pos = Point( 0,0 );
			name.clear();
			size = Size( 0,0 );
		}
	};
	class FalagardRadar : public FalagardSceneMap
	{
		enum MINIMAP_IMAGE_TYPE
		{
			//080423
			//			IMAGE_TYPE_PKKillPlayer		= ,		// PK红名的玩家
			//			IMAGE_TYPE_Player			= 3,	// 别的玩家	
			//			IMAGE_TYPE_PK				= ,		// PK对手

			IMAGE_TYPE_Animy			= 0,	// 敌人
			IMAGE_TYPE_Team				= 1,	// 队友

			IMAGE_TYPE_FamilyPlayer		= 2,	// 家族
			IMAGE_TYPE_GuildPlayer		= 3,	// 帮会
			IMAGE_TYPE_HomelandPlayer	= 4,	// 本国别的玩家	
			IMAGE_TYPE_NoHomelandPlayer	= 5,	// 非本国别的玩家		

			IMAGE_TYPE_ExpObj			= 6,	// 生长点
			IMAGE_TYPE_Active			= 7,	// 激活方向点
			IMAGE_TYPE_ScenePos 		= 8,	// 场景传送点
			IMAGE_TYPE_Flash			= 9,	// 闪光点

			IMAGE_TYPE_ExpNpc			 = 10,	// 普通npc
			IMAGE_TYPE_MisCompleteNpc    = 11,	// 任务NPC( 完成 )
			IMAGE_TYPE_MisNoCompleteNpc  = 12,  // 任务NPC( 未完成 )
			IMAGE_TYPE_AcceptNpc		 = 13,	// 任务NPC( 可接 )
			
			IMAGE_TYPE_NUM,		

			IMAGE_TYPE_MYSELF,	// 玩家自己
			IMAGE_TYPE_Pet,		// 宠物

		};
		
		enum MINIMAP_POS_TYPE
		{
			POS_TYPE_INVALIDE = -1,
			POS_TYPE_IMAGE = 0,
			POS_TYPE_ANIMATE,
		};

		//		const Image*		d_PlayerImage;				// 别的玩家的图标

		const Image*		d_AnimyImage;				// 敌人的图标
		const Image*		d_TeammateImage;			// 队友的图标

		const Image*		d_FamilyPlayerImage;		// 家族玩家的图标
		const Image*		d_GuildPlayerImage;			// 帮会玩家的图标

		const Image*		d_HomelandPlayerImage;		// 本国别的玩家的图标
		const Image*		d_NoHomelandPlayerImage;	// 非本国别的玩家的图标	

		const Image*		d_ExpObjImage;				// 生长点的图标
		const Image*		d_ScenePosImage;			// 场景传送点
		const Image*		d_FlashImage;				// 闪光点

	
		const Image*		d_ExpNpcImage;				// 普通npc的图标
		const Image*		d_MissCompleteNpcImage;		// 任务NPC(完成)
		const Image*		d_MissNoCompleteNpcImage;	// 任务NPC(未完成)
		const Image*		d_MissionNpcImage;			// 任务NPC(可接)

		const Image*		d_MyselfImage;				// 玩家自己的图标
		const Image*		d_PetImage;					// 宠物的图标

		String				d_PlayerAnimateName;		// 自己的方向动画名字
		String				d_ActiveAnimateName;		// 方向点的动画名字
		String				d_FlashAnimateName;			// 闪光点的动画名字

		typedef std::vector< MINIMAP_FLAG_DATA* >  RENDER_POS;

// 所有点的列表
		int			m_aPointerTimer[ IMAGE_TYPE_NUM ];	// 数组的管理指针

		//		RENDER_POS			m_aPlayerPos;		// 人类玩家	
		// PK对手
		// PK红名的玩家

		RENDER_POS			m_aAnimyPos;				// 敌人
		RENDER_POS			m_aTeammatePos;				// 队友

		RENDER_POS			m_aFamilyPlayerPos;			// 家族
		RENDER_POS			m_aGuildPlayerPos;			// 帮会
		RENDER_POS			m_aHomelandPlayerPos;		// 本国人类玩家
		RENDER_POS			m_aNoHomelandPlayerPos;		// 非本国人类玩家

		RENDER_POS			m_aExpObjPos;				// 生长点
		RENDER_POS  		m_aActivePos;				// 激活方向点
		RENDER_POS  		m_aScenePos;				// 场景传送点
		RENDER_POS  		m_aFlashPos;				// 闪光点

		RENDER_POS  		m_aPetPos;					// 宠物

		RENDER_POS			m_aExpNpcPos;				// 特殊npc
		RENDER_POS			m_aMissCompleteNpcPos;		// 任务NPC(完成)
		RENDER_POS			m_aMissNoCompleteNpcPos;	// 任务NPC(未完成)
		RENDER_POS			m_aMissionNpcPos;			// 任务NPC(可接)
		
		float				d_ReturnTimer;				// 自动恢复的计时器
		String32			m_objTooltip;

		bool				m_bMouseInWindow;

		bool				m_bIsAutoReset;
		
	public:
		static const utf8   WidgetTypeName[];       
	public:
		FalagardRadar(const String& type, const String& name);
		~FalagardRadar(void);

	public:
		void setTypeImage( int nType, const Image* pImage ); //设置image
		const Image* getTypeImage( int nType );

		void setTypeName( int nType, const String& str ); // 设置动画名字
		String getTypeName( int nType );

		virtual	void updateSelf(float elapsed); 
		
		void		UpdateViewRect(); // 根据当前人物位置重值可见范围

		int			GetTypeByName( const String& str );
		String		GetNameByType( int nType );

		void		setAutoReset( bool bAutoReset ){ m_bIsAutoReset = bAutoReset; };

		RENDER_POS* GetListByType( int nType );

		Window*		m_pMapLight;			//地图探灯
	public:
		virtual void setMinimapData( int nType, float fX, float fY, String32& szName  );
		virtual void Clear();
		// 设置自己的位置
		virtual void setMyselfPos( const Point& pos );
		virtual void setMyselfDir( const int nDirect );

		virtual void CreateSceneMap( const String& strFileName,const String& strPostFix);


		virtual void	onMouseMove( MouseEventArgs &e );
		virtual void    onMouseEnters( MouseEventArgs& e );
		virtual void    onMouseLeaves( MouseEventArgs& e );

		
	public:
		static FalagardRadarProperties::IconImage			d_IconImageProperty;
		static FalagardRadarProperties::AnimateImage		d_AnimateImageProperty;
		static FalagardRadarProperties::AutoReset			d_AutoRestProperty;
		static FalagardRadarProperties::RoundEnabled		d_RoundProperty;

	private:
		void updateFlash();
		void updateActive();
		void updateMyselfDir();
		void updateMyselfPos();
		MINIMAP_FLAG_DATA*  getMiniMapFlagData( int nType, int nIndex );

		int GetType( int nType );

		Point GetActiveArrowImagePos( Rect& rcView, Point& posStart, Point& posEnd );
		const Image* GetActiveArrowImage( Rect& rcView, Point& posStart, Point& posEnd );

	public:
		bool		m_bRoundEnabled;
		void	setRoundEnabled( bool bEnabled ) { m_bRoundEnabled = bEnabled; }

		class ParentWindow : public Window
		{
		public:
			static const utf8   WidgetTypeName[];       //!< type name for this widget.
			ParentWindow(const String& type, const String& name);
			~ParentWindow();

			void setRoundEnabled( bool bRoundEnabled ) { m_bIsUseRondRegion = bRoundEnabled; };
		protected:
			virtual void	drawSelf(float z);
			virtual void	notifyDrawEnd(void);

		private:
			bool		m_bIsUseRondRegion;
			Imageset* m_pRoundRegion;
		};

		class ParentWindowFactory : public WindowFactory
		{
		public:
			ParentWindowFactory(void) : WindowFactory(ParentWindow::WidgetTypeName) { }
			~ParentWindowFactory(void){}
			Window* createWindow(const String& name);
			void destroyWindow(Window* window);
		};

	};

	class FALAGARDBASE_API FalagardRadarFactory : public WindowFactory
    {
    public:
        FalagardRadarFactory(void) : WindowFactory(FalagardRadar::WidgetTypeName) { }
        ~FalagardRadarFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}