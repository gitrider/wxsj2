#pragma once
#include "FalModule.h"
#include "elements/CEGUIStaticImage.h"
#include "ceguiwindowfactory.h"
#include "IExpProperty.h"
#include <queue>

namespace CEGUI
{
	class IFalagardScrollInfo
	{
	public:
		virtual	void setWaitingInfo( const String32& strWaitingInfo ) = 0;
		virtual	void ResetIt() = 0;
	};


	namespace FalagardScrollInfoProperties
	{
	
		class CoolDown: public Property
		{
			public:
			CoolDown() : Property(
				"CoolDown",
				"",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class MoveSpeed: public Property
		{
			public:
			MoveSpeed() : Property(
				"MoveSpeed",
				"",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class ScrollTime: public Property
		{
			public:
			ScrollTime() : Property(
				"ScrollTime",
				"",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
	class FALAGARDBASE_API FalagardScrollInfo :	public IExpProperty, public IFalagardScrollInfo
	{
	public:
		static const String EventNamespace;
		static const utf8	WidgetTypeName[];

		FalagardScrollInfo(const String& type, const String& name);
		~FalagardScrollInfo(void);

		
		static FalagardScrollInfoProperties::CoolDown   d_propertyCoolDown;
		static FalagardScrollInfoProperties::MoveSpeed  d_propertyMoveSpeed;
		static FalagardScrollInfoProperties::ScrollTime d_propertyScrollTime;

		void				setMoveSpeed( float fSpeed ) { m_fMoveSpeed = fSpeed; };
		void				setCoolDown( float fCoolDown ) { m_fCoolDown = fCoolDown; };
		void				setScrollTime( int nTime ) { m_nScrollTime = nTime; };

		float				getMoveSpeed()  const { return m_fMoveSpeed; };
		float				getCoolDown()   const { return m_fCoolDown;  };
		int					getScrollTime() const { return m_nScrollTime;};

		virtual void setWaitingInfo( const String32& strWaitingInfo );
		virtual	void updateSelf(float elapsed); 
		virtual bool isUpdateRequire(void) const { return true; };
		virtual void initialise(void);
		virtual	void ResetIt(void);
		virtual bool	testClassName_impl(const String& class_name) const
		{
			if (class_name==(const utf8*)"FalagardScrollInfo")	return true;
			return Window::testClassName_impl(class_name);
		}
	private:

		
		int					m_nCurScrollTime;
		int					m_nScrollTime;

		bool				m_bMoving;  // �Ѿ�����ʾ�Ķ�����
		float				m_fStringLenght; // ��ǰ��Ϣ�Ŀ��
		// move 
		float				m_fPosX;	// ��ʾ��λ��
		float				m_fCurCoolDown; // ��ǰ��ȴʱ��
		// �ⲿ���ò���

		float				m_fMoveSpeed; // �����ٶ�
		float				m_fCoolDown;  // ��ȴʱ��
		CEGUI::Window*		m_pScrollWindow;

		std::queue<String32>			m_strWaitingInfo; // ���ڵȴ�����Ϣ��ֻ������һ��


	protected:
		virtual void	populateRenderCache();

		void	showAll(void);
		void	hideAll(void);
	};

	class FALAGARDBASE_API FalagardScrollInfoFactory : public WindowFactory
    {
    public:
        FalagardScrollInfoFactory(void) : WindowFactory(FalagardScrollInfo::WidgetTypeName) { }
        ~FalagardScrollInfoFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};