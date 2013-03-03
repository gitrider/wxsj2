#include "FalagardScrollInfo.h"
#include "FalChatBoard.h"
#include "CEGUIPropertyHelper.h"
#include "ceguiwindowmanager.h"

#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
namespace CEGUI
{
	const String FalagardScrollInfo::EventNamespace("ScrollInfo");

	const utf8 FalagardScrollInfo::WidgetTypeName[] = "Falagard/ScrollInfo";
	
	
	FalagardScrollInfoProperties::CoolDown  FalagardScrollInfo::d_propertyCoolDown;
	FalagardScrollInfoProperties::MoveSpeed FalagardScrollInfo::d_propertyMoveSpeed;
	FalagardScrollInfoProperties::ScrollTime FalagardScrollInfo::d_propertyScrollTime;


	FalagardScrollInfo::FalagardScrollInfo(const String& type, const String& name) :
		IExpProperty(type, name)
	{
		m_pScrollWindow = NULL;
		m_fPosX = 1;
		m_bMoving = false;
		m_fStringLenght = 0;
		m_nCurScrollTime = 0;
		m_fCoolDown = 1.f;
		m_fCurCoolDown = 0.f;

		ResetIt();

		CEGUI_START_ADD_STATICPROPERTY( FalagardScrollInfo )
			CEGUI_ADD_STATICPROPERTY( &d_propertyCoolDown );
			CEGUI_ADD_STATICPROPERTY( &d_propertyMoveSpeed );
			CEGUI_ADD_STATICPROPERTY( &d_propertyScrollTime );
		CEGUI_END_ADD_STATICPROPERTY

	}
	FalagardScrollInfo::~FalagardScrollInfo()
	{
				
	}

	void	FalagardScrollInfo::populateRenderCache()
	{
		const StateImagery* imagery;
		const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
		if( wlf.isStateImageryPresent( "Background" ) )
		{
			imagery = &wlf.getStateImagery( "Background" );
			// peform the rendering operation.
			imagery->render(*this);
		}
	}

	void FalagardScrollInfo::ResetIt(void)
	{
		m_bMoving = false;
		while(!m_strWaitingInfo.empty()) m_strWaitingInfo.pop();
		m_fCurCoolDown = 0.0f;
	}

	void FalagardScrollInfo::initialise(void)
	{
		Window::initialise();
		setMaximumSize( Size( 100,100 ) );
		m_pScrollWindow = WindowManager::getSingleton().createWindow( m_mapChildType[ "Child" ], getName() + "__auto__ScrollInfo" );
		//����m_pScrollWindow��󳤶ȿ�������Ļ��100������ֹ�ִ�������������ʾ
		m_pScrollWindow->setMetricsMode( Relative );
		m_pScrollWindow->setMaximumSize( Size( 100,100 ) );
		m_pScrollWindow->setProperty("Font","YouYuan11.25");
		m_pScrollWindow->setMouseHollow( true );
		addChildWindow(m_pScrollWindow);
	}

	void FalagardScrollInfo::setWaitingInfo( const String32& strWaitingInfo )
	{
		m_strWaitingInfo.push(strWaitingInfo);
		m_nCurScrollTime = 0;
		if(!strWaitingInfo.empty()) 		
			showAll();
	}

	void FalagardScrollInfo::showAll(void)
	{
		
		if(d_parent && d_parent->getName().find("ScrollInfo_") != CEGUI::String::npos )
		{
			Window* granddad = d_parent->getParent();
			if(granddad && granddad->getName().find("ScrollInfo_") != CEGUI::String::npos )
			{
				granddad->show();
				return;
			}
			else
			{
				d_parent->show();
			}
		}

		show();
	}

	void FalagardScrollInfo::hideAll(void)
	{
		if(d_parent && d_parent->getName().find("ScrollInfo_") != CEGUI::String::npos )
		{
			Window* granddad = d_parent->getParent();
			if(granddad && granddad->getName().find("ScrollInfo_") != CEGUI::String::npos )
			{
				granddad->hide();
				return;
			}
			else
			{
				d_parent->hide();
			}
		}
		hide();
	}

	void FalagardScrollInfo::updateSelf( float elapsed )
	{
		if( m_bMoving )//�ж����ڹ���
		{
			if( !isVisible() )
				showAll();
			if( m_fPosX + m_fStringLenght < 0 ) //���������
			{
				m_nCurScrollTime --;
				if( m_nCurScrollTime <= 0 )
				{
					m_bMoving = false;
					if( m_strWaitingInfo.empty() )	//���һ����û���˲�hide....��Ȼֱ��hide��Ͳ���updateSelf��....���º�ߵĳ�����....
					{
						hideAll();
					}
					((FalagardChatBoard*)m_pScrollWindow)->clearInfoItem();
					m_pScrollWindow->setText( "" );
					m_fCurCoolDown = m_fCoolDown; // ��ʼ��ȴ
				}
				else //�ٹ�һ��
				{
					m_fPosX = getAbsoluteWidth();
					return;
				}
			}
			else
			{
				m_fPosX -= ( elapsed * m_fMoveSpeed );
				m_pScrollWindow->setPosition( Absolute, Point( m_fPosX, 0 ) );
				requestRedraw();
				return;
			}
		}
		else
		{
			if( m_strWaitingInfo.empty() ) // ���Ϊ�գ���˵��ɶҲ����
			{
				if( isVisible() )
					hideAll();
				if(m_fCurCoolDown >0.f)
				{
					m_fCurCoolDown -= elapsed;
				}
				return;
			}
			else
			{
				
				if( m_fCurCoolDown > 0.f ) // ��ȴû�� û��ȴ�Ͳ���
				{
					m_fCurCoolDown -= elapsed;
				}
				else 
				{
					const FontBase* font = m_pScrollWindow->getFont();
					if( font )
					{
						
						m_fStringLenght = font->getTextExtent( m_strWaitingInfo.front(), 1 );
						m_fStringLenght += 20;
						m_pScrollWindow->setHeight( Relative, 1 );
						m_pScrollWindow->setWidth( Absolute, m_fStringLenght );
						// �����
						((FalagardChatBoard*)m_pScrollWindow)->clearInfoItem();
						((FalagardChatBoard*)m_pScrollWindow)->setText("");
						// ����µ���Ϣ
						((FalagardChatBoard*)m_pScrollWindow)->setChatBoardString( m_strWaitingInfo.front() );
						// ��ʼ����
						m_bMoving = true;
						m_fPosX = getAbsoluteWidth();
						m_nCurScrollTime = m_nScrollTime;
						m_strWaitingInfo.pop();
						m_pScrollWindow->setPosition( Absolute, Point( m_fPosX, 0 ) );
						m_pScrollWindow->requestRedraw();
						requestRedraw();
					}
				}
			}
		}
		
	}
		 //////////////////////////////////////////////////////////////////////////
    Window* FalagardScrollInfoFactory::createWindow(const String& name)
    {
        return new FalagardScrollInfo(d_type, name);
    }

    void FalagardScrollInfoFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }




	namespace FalagardScrollInfoProperties
	{

		//////////////////////////////////////////////////////////////////////////////////////////////////
		String MoveSpeed::get( const PropertyReceiver* receiver) const
		{			
			return PropertyHelper::floatToString( static_cast< const FalagardScrollInfo*>(receiver)->getMoveSpeed() );
		}

		void MoveSpeed::set( PropertyReceiver* receiver, const String& value) 
		{
			static_cast< FalagardScrollInfo*>(receiver)->setMoveSpeed( PropertyHelper::stringToFloat( value ) );
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		String CoolDown::get( const PropertyReceiver* receiver) const
		{
			return PropertyHelper::floatToString( static_cast< const FalagardScrollInfo*>(receiver)->getCoolDown() );
		}

		void CoolDown::set( PropertyReceiver* receiver, const String& value) 
		{
			static_cast< FalagardScrollInfo*>(receiver)->setCoolDown( PropertyHelper::stringToFloat( value ) );
		}

				//////////////////////////////////////////////////////////////////////////////////////////////////
		String ScrollTime::get( const PropertyReceiver* receiver) const
		{ 
			return PropertyHelper::intToString( static_cast< const FalagardScrollInfo*>(receiver)->getScrollTime() );
		}

		void ScrollTime::set( PropertyReceiver* receiver, const String& value) 
		{
			static_cast< FalagardScrollInfo*>(receiver)->setScrollTime( PropertyHelper::stringToInt( value ) );
		}
	}
};