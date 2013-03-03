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
			//			IMAGE_TYPE_PKKillPlayer		= ,		// PK���������
			//			IMAGE_TYPE_Player			= 3,	// ������	
			//			IMAGE_TYPE_PK				= ,		// PK����

			IMAGE_TYPE_Animy			= 0,	// ����
			IMAGE_TYPE_Team				= 1,	// ����

			IMAGE_TYPE_FamilyPlayer		= 2,	// ����
			IMAGE_TYPE_GuildPlayer		= 3,	// ���
			IMAGE_TYPE_HomelandPlayer	= 4,	// ����������	
			IMAGE_TYPE_NoHomelandPlayer	= 5,	// �Ǳ���������		

			IMAGE_TYPE_ExpObj			= 6,	// ������
			IMAGE_TYPE_Active			= 7,	// ������
			IMAGE_TYPE_ScenePos 		= 8,	// �������͵�
			IMAGE_TYPE_Flash			= 9,	// �����

			IMAGE_TYPE_ExpNpc			 = 10,	// ��ͨnpc
			IMAGE_TYPE_MisCompleteNpc    = 11,	// ����NPC( ��� )
			IMAGE_TYPE_MisNoCompleteNpc  = 12,  // ����NPC( δ��� )
			IMAGE_TYPE_AcceptNpc		 = 13,	// ����NPC( �ɽ� )
			
			IMAGE_TYPE_NUM,		

			IMAGE_TYPE_MYSELF,	// ����Լ�
			IMAGE_TYPE_Pet,		// ����

		};
		
		enum MINIMAP_POS_TYPE
		{
			POS_TYPE_INVALIDE = -1,
			POS_TYPE_IMAGE = 0,
			POS_TYPE_ANIMATE,
		};

		//		const Image*		d_PlayerImage;				// �����ҵ�ͼ��

		const Image*		d_AnimyImage;				// ���˵�ͼ��
		const Image*		d_TeammateImage;			// ���ѵ�ͼ��

		const Image*		d_FamilyPlayerImage;		// ������ҵ�ͼ��
		const Image*		d_GuildPlayerImage;			// �����ҵ�ͼ��

		const Image*		d_HomelandPlayerImage;		// ���������ҵ�ͼ��
		const Image*		d_NoHomelandPlayerImage;	// �Ǳ��������ҵ�ͼ��	

		const Image*		d_ExpObjImage;				// �������ͼ��
		const Image*		d_ScenePosImage;			// �������͵�
		const Image*		d_FlashImage;				// �����

	
		const Image*		d_ExpNpcImage;				// ��ͨnpc��ͼ��
		const Image*		d_MissCompleteNpcImage;		// ����NPC(���)
		const Image*		d_MissNoCompleteNpcImage;	// ����NPC(δ���)
		const Image*		d_MissionNpcImage;			// ����NPC(�ɽ�)

		const Image*		d_MyselfImage;				// ����Լ���ͼ��
		const Image*		d_PetImage;					// �����ͼ��

		String				d_PlayerAnimateName;		// �Լ��ķ��򶯻�����
		String				d_ActiveAnimateName;		// �����Ķ�������
		String				d_FlashAnimateName;			// �����Ķ�������

		typedef std::vector< MINIMAP_FLAG_DATA* >  RENDER_POS;

// ���е���б�
		int			m_aPointerTimer[ IMAGE_TYPE_NUM ];	// ����Ĺ���ָ��

		//		RENDER_POS			m_aPlayerPos;		// �������	
		// PK����
		// PK���������

		RENDER_POS			m_aAnimyPos;				// ����
		RENDER_POS			m_aTeammatePos;				// ����

		RENDER_POS			m_aFamilyPlayerPos;			// ����
		RENDER_POS			m_aGuildPlayerPos;			// ���
		RENDER_POS			m_aHomelandPlayerPos;		// �����������
		RENDER_POS			m_aNoHomelandPlayerPos;		// �Ǳ����������

		RENDER_POS			m_aExpObjPos;				// ������
		RENDER_POS  		m_aActivePos;				// ������
		RENDER_POS  		m_aScenePos;				// �������͵�
		RENDER_POS  		m_aFlashPos;				// �����

		RENDER_POS  		m_aPetPos;					// ����

		RENDER_POS			m_aExpNpcPos;				// ����npc
		RENDER_POS			m_aMissCompleteNpcPos;		// ����NPC(���)
		RENDER_POS			m_aMissNoCompleteNpcPos;	// ����NPC(δ���)
		RENDER_POS			m_aMissionNpcPos;			// ����NPC(�ɽ�)
		
		float				d_ReturnTimer;				// �Զ��ָ��ļ�ʱ��
		String32			m_objTooltip;

		bool				m_bMouseInWindow;

		bool				m_bIsAutoReset;
		
	public:
		static const utf8   WidgetTypeName[];       
	public:
		FalagardRadar(const String& type, const String& name);
		~FalagardRadar(void);

	public:
		void setTypeImage( int nType, const Image* pImage ); //����image
		const Image* getTypeImage( int nType );

		void setTypeName( int nType, const String& str ); // ���ö�������
		String getTypeName( int nType );

		virtual	void updateSelf(float elapsed); 
		
		void		UpdateViewRect(); // ���ݵ�ǰ����λ����ֵ�ɼ���Χ

		int			GetTypeByName( const String& str );
		String		GetNameByType( int nType );

		void		setAutoReset( bool bAutoReset ){ m_bIsAutoReset = bAutoReset; };

		RENDER_POS* GetListByType( int nType );

		Window*		m_pMapLight;			//��ͼ̽��
	public:
		virtual void setMinimapData( int nType, float fX, float fY, String32& szName  );
		virtual void Clear();
		// �����Լ���λ��
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