#pragma once

#include "falmodule.h"
#include "ceguiwindowfactory.h"
#include "falstaticimage.h"
#include "FalMinimapProperties.h"
#include "IFalagardMinimapWindow.h"
#include <vector>
namespace CEGUI
{

	class FALAGARDBASE_API FalagardMiniMap :	public FalagardStaticImage, public IFalagardMinimapWindow
	{

		
	public:
		enum MINIMAP_IMAGE_TYPE
		{
			IMAGE_TYPE_Animy			= 0,	// ����
			IMAGE_TYPE_ExpNpc			= 1,	// ����npc
			IMAGE_TYPE_Team				= 2,	// ����
			//080423
//			IMAGE_TYPE_PKKillPlayer		= ,		// PK���������
			IMAGE_TYPE_FamilyPlayer		= 3,	// ����
			IMAGE_TYPE_GuildPlayer		= 4,	// ���
//			IMAGE_TYPE_Player			= 3,	// ������	
			IMAGE_TYPE_HomelandPlayer	= 5,	// ����������	
			IMAGE_TYPE_NoHomelandPlayer	= 6,	// �Ǳ���������	
//			IMAGE_TYPE_PK				= ,		// PK����

			IMAGE_TYPE_ExpObj			= 7,	// ������
			IMAGE_TYPE_Active			= 8,	// ������
			IMAGE_TYPE_ScenePos 		= 9,	// ������ת��
			IMAGE_TYPE_Flash			= 10,	// �����
			IMAGE_TYPE_Pet				= 11,	// ����

			IMAGE_TYPE_NUM,
		};

	private:

		struct MINIMAP_FLAG_DATA
		{
			Point   pos;
			String32  name;
			Size    size;
			MINIMAP_FLAG_DATA()
			{
				size = Size( 0,0 );
				pos = Point( 0,0 );
				name.clear();
			}
		};

		bool		d_fresh;			// �Ƿ�ˢ����
		int 		d_displayFlag[ IMAGE_TYPE_NUM ];
		String		d_ImagetSetName;
		Imageset*	d_Imageset;
		Image*		d_MapImage;

		int			d_PlayerDirect;			// ����ĽǶ�

		const Image*		d_AnimyImage;		// ��ʾ���˵�ͼ��
		const Image*		d_ExpNpcImage;		// ����npc��ͼ��
		const Image*		d_TeammateImage;	// ���ѵ�ͼ��
		const Image*		d_PlayerImage;		// �����ҵ�ͼ��
		const Image*		d_ExpObjImage;		// ������Ʒ��ͼ��
		const Image*		d_MyselfImage;		// ����Լ���ͼ��
		const Image*		d_PetImage;			// ����Լ���ͼ��
		const Image*		d_ScenePosImage;	// ���������
	
		Rect				d_PlayerRect;
		String		d_MyselfAnimate;			// ����Լ��Ǹ�����
		String		d_ActiveAnimate;			// ������Ǹ�����
		String      d_FlashAnimate;

		Size		d_RenderSize;				// ��ʾ����ĳߴ�
		Point		d_CenterPos;				// ��ʾ���������
		Point		d_OldCenterPos;				// ��������λ��
		Point		d_MyselfPos;				// �����λ��
		Point		d_PlayerCoordinate;			// ��������
		
		Size        d_SceneToImageScale;		// ���ű���
		Rect		d_RenderRect;				// ��ʾ�÷�Χ
		
		
		Size		d_ScaleSceneToImage;		// �����͵�ͼ�ñ���
		Size		d_ImageSize;				// ��ͼ�óߴ�

		float		d_ReturnTimer;				// �Զ��ָ��ļ�ʹ��
		typedef std::vector< MINIMAP_FLAG_DATA >  RENDER_POS;
		// �����λ�ô�������������ϵ�����
		int			m_aPointerTimer[ IMAGE_TYPE_NUM ]; // ����Ĺ���ָ��
		RENDER_POS	m_aAnimyPos;
		RENDER_POS	m_aExpNpcPos;
		RENDER_POS	m_aTeammatePos;
		RENDER_POS	m_aPlayerPos;
		RENDER_POS	m_aExpObjPos;
		RENDER_POS  m_aScenePos;
		RENDER_POS  m_aActivePos;
		RENDER_POS  m_aFlashPos;
		RENDER_POS  m_aPetPos;
		
		
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
		virtual void setMinimapCenter( const Point& pos );
		const Point getMinimapCenter() const;

		void SetSceneSize( const Size& size );
		const Size GetSceneSize() const { return m_SceneSize; };

		void setMinimapRenderSize( const Size& rect );
		const Size getMinimapRenderSize() const;
	
		void setImageset( const String& name );
		void setImagesetByFileName( const String& name );
		const String& getImageset() const;

		virtual void setPlayerDirection( int nDir ); // ����ĽǶȣ�
		const int getPlayerDirection() const ;
		// ��ʼ������������
		void dataRest(); 

		void addAnimyPos( const Point& pos, const String& name );
		void addExpNpcPos( const Point& pos, const String& name );
		void addTeammatePos( const Point& pos, const String& name );
		void addPlayerPos( const Point& pos, const String& name );
		void addExpObjPos( const Point& pos, const String& name );
		void addScenePos( const Point& pos, const String& name );
		void addActiveObjPos( const Point& pos, const String& name );
		void addPetPos( const Point& pos, const String& name );
		


		void setPlayerImage( const Image* pImage );
		void setObjImage( const Image* pImage );
		void setTeammateImage( const Image* pImage );
		void setExpNpcImage( const Image* pImage );
		void setAnimyImage( const Image* pImage );
		void setScenePosImage( const Image* pImage );
		void setMyselfImage( const String& name );
		void setActiveImage( const String& name );
		void setPetmage( const Image* pImage );
		void setFlashImage( const String& name );
		

		const Image* getPlayerImage() const;
		const Image* getObjImage() const;
		const Image* getTeammateImage() const;
		const Image* getExpNpcImage() const;
		const Image* getAnimyImage() const;
		const Image* getScenePosImage()const;
		const Image* getActiveImage( Point& pos );
		const String getMyselfImage() const;
		const Image* getPetPosImage()const;
		const String getFlashImage() const;


		static FalagardMinimapProperties::RenderSize		d_RenderSizeProperty;
		static FalagardMinimapProperties::SceneSize			d_SceneSizeProperty;
		static FalagardMinimapProperties::MinimapImageSet	d_ImagesetProperty;
		static FalagardMinimapProperties::SetMinimapName	d_MapFileNameProperty;
		static FalagardMinimapProperties::PlayerImage		d_PlayerImageProperty;
		static FalagardMinimapProperties::ObjImage			d_ObjImageProperty;
		static FalagardMinimapProperties::MySelfImage		d_MySelfImageProperty;
		static FalagardMinimapProperties::TeammateImage		d_TeammateImageProperty;
		static FalagardMinimapProperties::ExpNpcImage		d_ExpNpcImageProperty;
		static FalagardMinimapProperties::ScenePosImage		d_ScenePosImageProperty;
		static FalagardMinimapProperties::AnimyImage		d_AnimyImageProperty;
		static FalagardMinimapProperties::OpenFlagType		d_OpenFlagTypeProperty;
		static FalagardMinimapProperties::CloseFlagType		d_CloseFlagTypeProperty;
		static FalagardMinimapProperties::IsFlagOpen		d_IsFlagOpenProperty;
		static FalagardMinimapProperties::ClearFlagList		d_ClearFlagListProperty;
		static FalagardMinimapProperties::GetCurCoordinate	d_GetCurCoordinateProperty;
		static FalagardMinimapProperties::ActiveImage		d_ActiveImageProperty;
		static FalagardMinimapProperties::PetImage			d_PetImageProperty;
		static FalagardMinimapProperties::FlashImage		d_FlashImageProperty;

		void setEnabel( int nIndex );
		void setDisable( int nIndex );

		const String getEnableType() const;

		virtual void  clearFlagList();

		const Point getActivePos( Point& pos );

		

	public:
		FalagardMiniMap(const String& type, const String& name);
		~FalagardMiniMap(void);

		virtual void	updateSelf(float elapsed); 
		

		virtual void    onMouseMove( MouseEventArgs& e );
		virtual void    onMouseButtonDown( MouseEventArgs& e );
		virtual void    onMouseButtonUp( MouseEventArgs& e );
		virtual void    onMouseDoubleClicked( MouseEventArgs& e );
		virtual void    onMouseEnters( MouseEventArgs& e );
		virtual void    onMouseLeaves( MouseEventArgs& e );
		virtual void	onSized(WindowEventArgs& e);

		void			drawImagePos( RENDER_POS& pImageList, int nType );
		void			drawImageFlash(); // ����������㣬�Ƚ����⣬�������

	protected:
		virtual	void	populateRenderCache();
	protected:
		bool			m_bDraping;
		bool			m_bSuperlook;
		bool			m_bMouseInWindow;
		Point			m_ptLastPos;

		Point			m_ptCoordinate; // ����

		// �����óߴ�
		Size			m_SceneSize;
		Size			m_TextureSize;

		String32			m_objTooltip;
		// ���㵱ǰ��꼤���tooltip
		void	SetMinimapToolTip( RENDER_POS& pImageList, Point& mousePos, const Image* image, int nType  );
		void	SetMinimapToolTipAction( Point& mousePos );
		// ���㵱ǰ�������λ������
		const Point GetMinimapCoordinate( Point& pos) const { return m_ptCoordinate ;}
		
		// ��׼����ʾ����
		void	NormalArea();	

		// ��������ת������Ļ����
		Point ImagePosToWindowPos( Point& pos );
		// ��Ļ����ת����ͼ������
		Point ScreenPosToImagePos( Point& pos );
		//  �ӳ�������ת����ͼ������
		Point ScenePosToImagePos( Point& pos );
		//   ��ͼ������ת������������
		Point ImagePosToScenePos( Point& pos );
		//   �õ���ǰ��ʾ������ԭ��
		Point GetCurrentZero();
		// �ж�һ�����������Ƿ�ɼ�
		bool  IsPosVisible( Point& pos );

		// 
		// �õ���ͷ����ʾλ��
		Point GetActiveArrowRenderPos( Point& pos );
		// �õ���ͷ������λ��
		Point GetActiveArrowImagePos( Point& pos );
		// �õ���ǰ��Ҫ��ʾ�ü�ͷͼ��
		const Image* GetActiveArrowImage( Point& pos );
	public:
		const Point GetSceneCoordinate() const { return m_ptCoordinate; };

		virtual void setMinimapData( int nType, float fX, float fY, String32& szName  );

	};
	
	
	class FALAGARDBASE_API FalagardMinimapFactory : public WindowFactory
    {
    public:
        FalagardMinimapFactory(void) : WindowFactory(FalagardMiniMap::WidgetTypeName) { }
        ~FalagardMinimapFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}	
