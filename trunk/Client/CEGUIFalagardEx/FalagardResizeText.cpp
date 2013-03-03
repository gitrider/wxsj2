#include ".\falagardresizetext.h"
#include "ceguiwindowmanager.h"
#include "CEGUIFontBase.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
namespace CEGUI
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	const utf8  FalagardResizeText::WidgetTypeName[] = "Falagard/ResizeText";


	FalagardResizeTextProperties::AimWindow     FalagardResizeText::d_AimWindowPropety;
	FalagardResizeTextProperties::AimPosition   FalagardResizeText::d_AimPositionPropety;


	FalagardResizeText::FalagardResizeText( const String& type, const String& name ):
		FalagardStaticText(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardResizeText )
		   CEGUI_ADD_STATICPROPERTY( &d_AimWindowPropety  );
		   CEGUI_ADD_STATICPROPERTY( &d_AimPositionPropety  );
		CEGUI_END_ADD_STATICPROPERTY
	}

	FalagardResizeText::~FalagardResizeText(void)
	{
	}

	void FalagardResizeText::SetAimWindow( const String& strWindowName )
	{
		m_pAimWindow = WindowManager::getSingleton().getWindow( strWindowName );
	}
	void FalagardResizeText::SetAimPosition( const String& strPositionName )
	{
		char szAim[ 128 ];
		char szSelf[ 128 ];
		char szLock[ 128 ];
		sscanf( strPositionName.c_str(),  "Aim:%s Self:%s Lock:%s", szAim, szSelf, szLock  ); 
		if( strcmp( szAim, "LeftTop" ) == 0 )
		{
			m_nAimPositionType = LeftTop;
		}
		else if( strcmp( szAim,"RightTop" )== 0 )
		{
			m_nAimPositionType = RightTop;
		}
		else if( strcmp( szAim, "RightBottom" ) == 0 )
		{
			m_nAimPositionType = RightBottom;
		}
		else if( strcmp( szAim,"LeftBottom" ) == 0)
		{
			m_nAimPositionType = LeftBottom;
		}
		else
			m_nAimPositionType = InvalidPosition;


		if( strcmp( szSelf, "LeftTop" ) == 0 )
		{
			m_nSelfPositionType = LeftTop;
		}
		else if( strcmp( szSelf, "RightTop" ) == 0 )
		{
			m_nSelfPositionType = RightTop;
		}
		else if( strcmp( szSelf, "RightBottom" ) == 0 )
		{
			m_nSelfPositionType = RightBottom;
		}
		else if( strcmp( szSelf, "LeftBottom" ) == 0)
		{
			m_nSelfPositionType = LeftBottom;
		}
		else 
			m_nSelfPositionType = InvalidPosition;


		if( strcmp( szLock, "Width" ) )
			m_nLockType = LockWidth;
		else if( strcmp( szLock, "Height" ) )
			m_nLockType = LockHeight;
	}
	void FalagardResizeText::positionSelf()
	{
		Point pos;
		if( m_pAimWindow ) // ����Ұ󶨵�Ŀ�괰��,�͸���Ŀ�괰�ڵ�λ�ü���
		{
			Size sz = getAbsoluteSize();
			Size szAim = m_pAimWindow->getAbsoluteSize();
			pos = m_pAimWindow->getAbsolutePosition();
			switch( m_nAimPositionType )
			{
			case LeftTop:
				{
					
					switch( m_nSelfPositionType )
					{
					case LeftTop: // ������ܰ�
						break;
					case RightTop: // ���Լ�ƽ��,��Ŀ�괰�ڵ����, �ϱ߶���
						pos.d_x -= sz.d_width;
						break;
					case RightBottom: // ��Ŀ�괰������,�·���Ŀ�괰���ϱ߶���
						pos.d_x -= sz.d_width;
						pos.d_y -= sz.d_height;
						break;
					case LeftBottom: // ��Ŀ�괰�����Ϸ�,��߶���
						pos.d_y -= sz.d_height;
						break;
					default:
						break;
					}
				}
				break;
			case RightTop:
				{
					switch( m_nSelfPositionType )
					{
					case LeftTop: // ��Ŀ�괰�����,�ϱ߶���
						pos.d_x += sz.d_width;
						break;
					case RightTop: // ������ܰ�
						break;
					case RightBottom:// ��Ŀ�����Ϸ�, �ұ߶���
						pos.d_y -= sz.d_height;
						pos.d_x += szAim.d_width - sz.d_width;
						break;
					case LeftBottom: // Ŀ�����Ϸ�,��ߺ�Ŀ���ұ߶���
						pos.d_x += szAim.d_width;
						pos.d_y -= sz.d_height;
						break;
					default:
						break;
					}
				}
				break;
			case RightBottom:
				{
					Size sz = m_pAimWindow->getAbsoluteSize();
					switch( m_nSelfPositionType )
					{
					case LeftTop: //  Ŀ�괰�ڵ����·�,���ϽǺ�Ŀ�����½��غ�
						pos.d_x += sz.d_width;
						pos.d_y += sz.d_height;
						break;
					case RightTop:// Ŀ�����·�,�ұ߶���
						pos.d_x += szAim.d_width - sz.d_width;
						pos.d_y += szAim.d_height;
						break;
					case RightBottom: // ���ܰ�
						break;
					case LeftBottom: // ���ҷ�,���½̶���
						pos.d_x += szAim.d_width;
						pos.d_y += szAim.d_height - sz.d_height;
						break;
					default:
						break;
					}
				}
				break;
			case LeftBottom:
				{
					switch( m_nSelfPositionType )
					{
					case LeftTop: // ���·�,��߶���
						pos.d_y += szAim.d_height; 
						break;
					case RightTop: // Ŀ�괰�����¶���
						pos.d_x -= sz.d_width;
						pos.d_y += szAim.d_height;
						break;
					case RightBottom: // ����,�±߶���
						pos.d_x -= sz.d_width;
						pos.d_y += szAim.d_height - sz.d_height;
						break;
					case LeftBottom: // ���ܰ�
						break;
					default:
						break;
					}
				}
				break;
			}
			
		} // end m_pAimWindow != NULL;
		else // û��Ŀ�괰�ھ�ֱ�Ӱ󵽸�������
		{
			Size sz = getAbsoluteSize();
			Size szParent = getParentSize();
			pos = getParent()->getAbsolutePosition();
			switch( m_nSelfPositionType )
			{
				case LeftTop: // ���Ͻ�
					break;
				case RightTop: // 
					pos.d_x += szParent.d_width - sz.d_width;
					break;
				case RightBottom: // ����,�±߶���
					pos.d_x += szParent.d_width - sz.d_width;
					pos.d_y += szParent.d_height - sz.d_height; 
					break;
				case LeftBottom: // ���ܰ�
					pos.d_y += szParent.d_height - sz.d_height; 
					break;
				default:
					break;
			}
		}
		setPosition( Absolute, pos );
	}
	void FalagardResizeText::onTextChanged( WindowEventArgs& e )
	{
		//setSize( getTextSize() );	
		//positionSelf();
	}
	// ����ߴ�ı���,�����¸��Լ����³ߴ�
	void FalagardResizeText::onSized( WindowEventArgs& e )
	{
		//setSize( getTextSize() );	
		//positionSelf();
	}
	void	FalagardResizeText::ResizeWindow()
	{
		setHeight( Absolute, getTextHeight() );
		d_frame.setSize(getAbsoluteSize());
		requestRedraw();
		//positionSelf();
	}
	float FalagardResizeText::getTextHeight() const
	{
		const FontBase* fnt = getFont();
		Size sz;
        if (fnt)
        {
            Rect area = getAbsoluteRect();
			float height = fnt->getFormattedSize(d_text, area, CEGUI::WordWrapCentred).d_height;
			return height;
		}
		return 0;

	}
	Size FalagardResizeText::getTextSize() const // �õ��ַ��ĳߴ�
	{
		const FontBase* fnt = getFont();
		Size sz;
		//const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);

  //      Rect textArea(wlf.getNamedArea("TextArea").getArea().getPixelRect(*this));
  //      Rect wndArea(getAbsoluteRect());
        if (fnt)
        {
            Rect area = getAbsoluteRect();
			 // get required size of the tool tip according to the text extents.
            // TODO: Add a proprty to allow specification of text formatting.
			float height = fnt->getFormattedSize(d_text, area, CEGUI::WordWrapCentred).d_height;
			//float width = fnt->getFormattedTextExtent(d_text, area, CEGUI::WordWrapCentred);

			sz =  Size(area.getWidth(), height);
        }
        else
        {
            sz = Size(0,0);
        }

		

        // get WidgetLookFeel for the assigned look.


      /*  sz.d_width  += wndArea.getWidth() - textArea.getWidth();
        sz.d_height += wndArea.getHeight() - textArea.getHeight();*/
	
		return sz;
	}
	void FalagardResizeText::populateRenderCache()
	{

		StaticText::populateRenderCache();
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	Window* FalagardResizeTextWindowFactory::createWindow(const String& name)
    {
        return new FalagardResizeText(d_type, name);
    }

    void FalagardResizeTextWindowFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	namespace FalagardResizeTextProperties 
	{
		////////////////////////////////////////////////////////////////////////		
		// ��׼�Ĵ�������,������ͬ����
		String AimWindow::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString(static_cast<const FalagardSuperToolTip*>(receiver)->isEmpty());
		}

		void AimWindow::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardResizeText*>(receiver)->SetAimWindow( value );
		}
		////////////////////////////////////////////////////////////////////////		
		// ��׼�Ĵ��ڵĽ�,������ͬ����
		String AimPosition::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString(static_cast<const FalagardSuperToolTip*>(receiver)->isEmpty());
		}

		void AimPosition::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardResizeText*>(receiver)->SetAimPosition( value );
		}
	}

};