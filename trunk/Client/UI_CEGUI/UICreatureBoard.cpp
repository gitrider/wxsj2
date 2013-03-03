
#include "StdAfx.h"
#include "UIGlobal.h"
#include "UICreatureBoard.h"
#include "UICreatureBoardSystem.h"
#include "CEGUIWindowManager.h"
#include "ceguiwindow.h"
#include "ceguiudim.h"
#include "elements\ceguiprogressbar.h"
#include "..\CEGUI\CEGUI\include\CEGUIAnimateManager.h"	// 20100513 AddCode
#include "..\CEGUIFalagardEx\falagardanimate.h"			// 20100513 AddCode
#include "uiwindowmng.h"
#include "GIException.h"
#include "UIString.h"
#include "CEGUIPropertyHelper.h"
#include "ceguicolourrect.h"
#include "elements\CEGUIPushButton.h"
#include "GIGameInterface.h"
#include "ceguipropertyhelper.h"
#include "uisystem.h"
#include "elements\CEGUIStaticImage.h"
#include "GIDataBase.h"
#include "GIDBC_Struct.h"
#include "FalagardSelfFitWindow.h"
#include "IFalagardSelfFitWindow.h"
#include "FalButton.h"
#include "AxTrace.h"


CCreatureBoard::CCreatureBoard( int nIndex )
	:
	m_pWindow( 0 ), 
	m_pNameWindow( 0 ),
	m_pCountryTitleWindow( 0 ),
	m_pGuildTitleWindow( 0 ),
	m_pTitleWindow( 0 ),
	m_pSaleWindow( 0 ),
	m_bShow( TRUE ),
	m_bShowHpLevel( FALSE ),
	m_bShowLevelIco( FALSE ),
	m_bShowStallSign( FALSE ),
	m_bLeaderFlag( FALSE ),
	m_bTeamFlag( FALSE ),
	m_bCountryTitle( FALSE ),
	m_bGuildTitle( FALSE ),
	m_bTitle( FALSE ),
	m_nTitleType( DIMNESS_TITLE ),
	m_nBangpaiFlag( NO_POS )
{
	m_nDebugIndex = nIndex;
	m_fLastX = m_fLastY = -100;

	m_pWindow = CEGUI::WindowManager::getSingleton().createWindow("WoWLook/CreatureBoard");
	KLAssert(m_pWindow);

	//���ô�С
	m_pWindow->setProperty("Position", "x:0 y:0");
	m_pWindow->setProperty("Size", "w:1 h:1");

	//�������
	m_pNameWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playername__");
	m_pNameWindow->hide();//laim

	// Ѫ���͵ȼ��ı���
	m_pHPlevelWindow = CEGUI::WindowManager::getSingleton().getWindow(	m_pWindow->getName() + "__auto_playerhplevel__");
	m_pHPlevelWindow->hide();//laim1641

	// Ѫ��
	m_pHPWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playerhp__");

	// �ȼ�
	m_pLevelTextWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playerleveltext__");
	m_pLevelIcoWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playerlevelico__");


	// PK��ǣ�ֻ��������ʾ��		-- ���Կ��Ǹ�����NPC��ʾ��
	m_pPKWindow = CEGUI::WindowManager::getSingleton().getWindow(
			m_pWindow->getName() + "__auto_playerpktag__");

	//��̯��ť
	m_pSaleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_salessign__");
	m_pSaleWindow->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&CCreatureBoard::handleSaleSignClicked, this));


	// ���ҳƺ�
	m_pCountryTitleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playercountrytitle__");
	
	// ���ƺ�
	m_pGuildTitleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playerguildtitle__");

	//�ƺ�
	m_pTitleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playertitle__");

	// ���ұ��
	m_pCountryFlag = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_countryflag__");

	//����Ȩ��Сͼ��
	m_pBangpaiFlag = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_bangpaiflag__");

	//�ӳ���־
	m_pLeadFlag = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_leaderflag__");

	// ��������
	m_pPaoPao =  CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_paopao__");

	m_InfoState = STATE_NORMAL;

	m_missionState = -1;
}

CCreatureBoard::~CCreatureBoard()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(m_pWindow);
}


void CCreatureBoard::reset(void)
{
	m_bShowStallSign = FALSE;
	m_bLeaderFlag = FALSE;
	m_bTeamFlag = FALSE;
	m_strName = "";

	m_pNameWindow->setText("");
	m_pNameWindow->hide();
	
	m_pHPlevelWindow->hide();
	m_pHPWindow->hide();
	m_pLevelTextWindow->hide();
	m_pLevelIcoWindow->hide();

	m_pSaleWindow->setText("");
	m_pSaleWindow->hide();

	m_pCountryTitleWindow->setText("");
	m_pCountryTitleWindow->hide();

	m_pGuildTitleWindow->setText("");
	m_pGuildTitleWindow->hide();

	m_pTitleWindow->setText("");
	m_pTitleWindow->hide();

	m_pCountryFlag->hide();
	m_pLeadFlag->hide();
	m_pBangpaiFlag->hide();
	m_pPKWindow->hide();
	
	m_pPaoPao->setText("");
	m_pPaoPao->hide();

	m_pWindow->hide();
}

//ɾ������
VOID CCreatureBoard::Destroy(VOID)
{
	if( CUICreatureBoardSystem::GetMe() )
		CUICreatureBoardSystem::GetMe()->DestroyCreatureBoard(this);
	return;
	//�����ٵ����κδ���
	//...
}

fVector2 CCreatureBoard::GetPosition(void) const
{
	CEGUI::Point pt = CEGUI::PropertyHelper::stringToPoint(m_pNameWindow->getProperty( "FitPosition" ));

	return fVector2(pt.d_x, pt.d_y);
}

//��������Ļ�ϵ�����(����)
VOID CCreatureBoard::SetPosition(FLOAT nX, FLOAT nY)
{
	float fAlpha = 1.0f;
	if(m_nDistance > CUICreatureBoardSystem::GetMe()->GetMaxDispearDistance())
	{
		m_pWindow->hide();
		return;
	}

	if(m_nDistance > CUICreatureBoardSystem::GetMe()->GetMaxDistance())
	{
		fAlpha -= (m_nDistance - CUICreatureBoardSystem::GetMe()->GetMaxDistance() )/
					( CUICreatureBoardSystem::GetMe()->GetMaxDispearDistance() - CUICreatureBoardSystem::GetMe()->GetMaxDistance());
	}

	CEGUI::Point fPos(nX, nY);
	float fOffset = 0;
	float fY = fPos.d_y;
	CEGUI::Size size;	

	// �ж��Ƿ�Ҫ��ʾ
	if( m_bShowHpLevel )
	{
		if( m_pHPlevelWindow && m_pHPWindow )
		{
			// ����λ��
			fOffset -= m_pHPlevelWindow->getAbsoluteHeight();
			fPos.d_y += fOffset;
			fPos.d_x -= m_pHPlevelWindow->getAbsoluteWidth() / 2.0f;
			m_pHPlevelWindow->setPosition(CEGUI::Absolute, fPos);

			// Ѫ��
			m_pHPWindow->setPosition(CEGUI::Absolute, fPos);

			// ���ý���
			((CEGUI::ProgressBar*)m_pHPWindow)->setProgress( m_fProgross );

			//laim1648 m_pHPWindow->show();
		}
	}
	
	// �ָ�xƫ��
	fPos.d_x = nX;
	fPos.d_y = nY;

	// �������ֵ�λ��
	fPos.d_y += fOffset;

	( ( CEGUI::IFalagardSelfFitWindow* )( CEGUI::FalagardSelfFitWindow* )m_pNameWindow )->setCenter(fPos);
	fOffset -= m_pNameWindow->getAbsoluteHeight();
	
	// ����
	if( m_pNameWindow->isVisible() && !( m_pNameWindow->getText().empty() ) )
	{
		m_pWindow->show();
		debugInfo();
	}
	else
	{
		// ��̯״̬ʱ������������ֲ���ʾʱ����Ϣ�廹Ҫ��ʾ	20100527 BLL
		if ( m_bShowStallSign == FALSE ) 
		{
			m_pWindow->hide();
		}
		else
			m_pWindow->show();
	}

	// 20100504 AddCodeBegin
	// ������ʾ��������ֺ���
	if( m_bShowHpLevel )
	{
		CEGUI::Point fLevelPos = m_pNameWindow->getAbsolutePosition();
		CEGUI::Size  nameSize  = m_pNameWindow->getAbsoluteSize();

		fLevelPos.d_x = fLevelPos.d_x + nameSize.d_width;

		( ( CEGUI::IFalagardSelfFitWindow* )( CEGUI::FalagardSelfFitWindow* )m_pLevelTextWindow )->setCenter(fLevelPos);

		m_pLevelTextWindow->setPosition( CEGUI::Absolute, fLevelPos );
		m_pLevelIcoWindow->setPosition(  CEGUI::Absolute, fLevelPos );

		if( !m_bShowLevelIco )
		{
			m_pLevelTextWindow->show();
		}
		else
		{
			size = m_pNameWindow->getAbsoluteSize();
			size.d_width = size.d_height;
			m_pLevelIcoWindow->setSize( CEGUI::Absolute, size );
			m_pLevelIcoWindow->show();
		}
	}
	// 20100504 AddCodeEnd

	
	// ���ҳƺ�
	if( m_bCountryTitle && !( m_pCountryTitleWindow->getText().empty() ) )
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pCountryTitleWindow)->setCenter( fPos );
		fOffset -= m_pCountryTitleWindow->getAbsoluteHeight();

		m_pCountryTitleWindow->show();
	}

	// ���ƺ�
	if( m_bGuildTitle && !( m_pGuildTitleWindow->getText().empty() ) )
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pGuildTitleWindow)->setCenter( fPos );	
		fOffset -= m_pGuildTitleWindow->getAbsoluteHeight();

		m_pGuildTitleWindow->show();
	}

	// �ƺ�
	if( m_bTitle && !(m_pTitleWindow->getText().empty()) )
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pTitleWindow)->setCenter(fPos);	
		fOffset -= m_pTitleWindow->getAbsoluteHeight();

		m_pTitleWindow->show();
	}

	// ��̯
	if( m_pSaleWindow->isVisible() )
	{
		fPos.d_y = fY + fOffset;
		m_pSaleWindow->setProperty( "HookPosition", CEGUI::PropertyHelper::pointToString( fPos ) );
		((CEGUI::PushButton*)m_pSaleWindow)->resizeWithText();
		fOffset -= m_pSaleWindow->getAbsoluteHeight();
	}

	// ͷ������
	if( m_pPaoPao->isVisible() )
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pPaoPao)->setCenter(fPos);
		fOffset  -= m_pPaoPao->getAbsoluteHeight();
	}

	// PK��־ 
	// 20100513 ModifyCodeBegin
	if( m_pPKWindow->isVisible() ) 
	{
		CEGUI::Size pkSize;

		fOffset -= m_pPKWindow->getAbsoluteHeight();
		fPos.d_y = fY + fOffset;
		fPos.d_x = nX - m_pPKWindow->getAbsoluteWidth() / 2;

		//pkSize.d_width  = ((CEGUI::StaticImage*)m_pPKWindow)->getImage()->getWidth();
		//pkSize.d_height = ((CEGUI::StaticImage*)m_pPKWindow)->getImage()->getHeight();

		pkSize = CEGUI::AnimateManager::getSingleton().getAnimate( (CEGUI::utf8*)"questsign_accept" )->getSize();

		m_pPKWindow->setSize( CEGUI::Absolute, pkSize );
		m_pPKWindow->setPosition( CEGUI::Absolute, fPos );
	}
	// 20100513 ModifyCodeEnd


	// ���ұ�־
	size = m_pNameWindow->getAbsoluteSize();
	fPos = m_pNameWindow->getAbsolutePosition();

	fPos.d_x -= size.d_height;
	m_pCountryFlag->setPosition(CEGUI::Absolute, fPos);

	size.d_width = size.d_height;
	m_pCountryFlag->setSize(CEGUI::Absolute, size);


	// �ӳ���־
	size = m_pNameWindow->getAbsoluteSize();
	fPos = m_pNameWindow->getAbsolutePosition();

	// 20100504 ModifyCodeBegin �����Ϊ�����ֺ�����ʾ���޸Ķӳ����λ��
	CEGUI::Size  levelTextSize = m_pLevelTextWindow->getAbsoluteSize();

	fPos.d_x += size.d_width;
	fPos.d_x += levelTextSize.d_width;
	// 20100504 ModifyCodeEnd

	m_pLeadFlag->setPosition(CEGUI::Absolute, fPos);
	size.d_width = size.d_height;
	m_pLeadFlag->setSize(CEGUI::Absolute, size);


	// ����Ȩ��ͼ����λ��
	size = m_pTitleWindow->getAbsoluteSize();
	fPos = m_pTitleWindow->getAbsolutePosition();

	fPos.d_x -= size.d_height;
	m_pBangpaiFlag->setPosition( CEGUI::Absolute, fPos );

	size.d_width = size.d_height;
	m_pBangpaiFlag->setSize( CEGUI::Absolute, size );


	// ����͸����
	m_pCountryFlag->setAlpha( fAlpha );
	m_pBangpaiFlag->setAlpha( fAlpha );
	m_pLeadFlag->setAlpha( fAlpha );
	m_pPKWindow->setAlpha( fAlpha );
	m_pSaleWindow->setAlpha( fAlpha );
	m_pNameWindow->setAlpha( fAlpha );
	m_pTitleWindow->setAlpha(fAlpha);
	m_pCountryTitleWindow->setAlpha(fAlpha);
	m_pGuildTitleWindow->setAlpha(fAlpha);


	switch( m_InfoState ) 
	{
	case STATE_NORMAL:
		break;
	case STATE_DIE:
		m_pNameWindow->setProperty( "TextColor", CEGUI::PropertyHelper::colourToString( CEGUI::colour( 0.5,0.5,0.5, fAlpha ) ) );
		break;
	default:
		break;
	}
}

//��ʾ/����
VOID CCreatureBoard::Show(BOOL bShow)
{
//	if(bShow == m_bShow) return;

	if( bShow && !(m_pNameWindow->getText().empty()) )
	{
		m_pWindow->show();
		m_pWindow->requestRedraw();

		m_pNameWindow->requestRedraw();

		if( m_bShowHpLevel )
		{
			m_pHPlevelWindow->requestRedraw();
			m_pHPWindow->requestRedraw();
			if( !m_bShowLevelIco )
				m_pLevelTextWindow->requestRedraw();
			else
				m_pLevelIcoWindow->requestRedraw();
		}

		if( m_bCountryTitle )
			m_pCountryTitleWindow->requestRedraw();
		if( m_bGuildTitle )
			m_pGuildTitleWindow->requestRedraw();
		if( m_bTitle )
			m_pTitleWindow->requestRedraw();

		m_pBangpaiFlag->requestRedraw();
		m_pCountryFlag->requestRedraw();
		m_pPKWindow->requestRedraw();

		// ��̯
		if( !m_bShowStallSign )
			m_pSaleWindow->hide();
	}
	else 
	{
		m_pWindow->hide();

		m_pCountryTitleWindow->hide();
		m_pGuildTitleWindow->hide();
		m_pTitleWindow->hide();
	}

	m_bShow = bShow;
}

VOID CCreatureBoard::ShowCharAttr( BOOL bShow )
{
	m_bShowHpLevel = bShow;
}

VOID CCreatureBoard::ShowCharTitle( BOOL bShow, INT nType )
{
	switch( nType )
	{
	// ����
	case 1:
		{
			if( bShow )
				m_bCountryTitle = TRUE;
			else
			{
				m_bCountryTitle = FALSE;
				m_pCountryTitleWindow->hide();
			}
		}
		break;
	// ���
	case 2:
		{
			if( bShow )
				m_bGuildTitle = TRUE;
			else
			{
				m_bGuildTitle = FALSE;
				m_pGuildTitleWindow->hide();
			}
		}
		break;
	// �淨
	case 3:
		{
			if( bShow )
				m_bTitle = TRUE;
			else
			{
				m_bTitle = FALSE;
				m_pTitleWindow->hide();
			}
		}
		break;
	};
}


void CCreatureBoard::debugInfo(void)
{
	CEGUI::Point pt = m_pNameWindow->getAbsolutePosition();
	if(pt.d_x != m_fLastX || pt.d_y != m_fLastY)
	{
//		AxTrace(m_nDebugIndex, 0, "%.2f,%.2f", pt.d_x, pt.d_y);
	}

	m_fLastX = pt.d_x;
	m_fLastY = pt.d_y;
}

//��������
VOID CCreatureBoard::SetElement_Name(LPCTSTR szName)
{
	m_strName = szName;
	_UpdateNameElement();
	return;
}

// ��������ֵ
VOID CCreatureBoard::SetElement_HPProgress( INT hp, INT maxHp )
{
	if( 0 == hp || 0 == maxHp )
	{
//		((CEGUI::ProgressBar*)m_pHPWindow)->setProgress((FLOAT)1);
		m_fProgross = 1.0f;
	}
	else
	{
//		((CEGUI::ProgressBar*)m_pHPWindow)->setProgress( (FLOAT)(hp / maxHp) );
		m_fProgross = (FLOAT)hp / (FLOAT)maxHp;
	}

//	<Event name="MouseEnter" function="PartyFrame_HP_Text_MouseEnter(1);" />
//	<Event name="MouseLeave" function="PartyFrame_HP_Text_MouseLeave(1);" />
}
VOID CCreatureBoard::SetElement_HPProgress( FLOAT hpPercent )
{
	m_fProgross = hpPercent;
}

// ���õȼ�
VOID CCreatureBoard::SetElement_Level( INT nLevel, INT nType )
{
	// ͼƬ�� ������ֱ�ӷ���
	if( 1 == nType )
	{
		m_bShowLevelIco = TRUE;
		//m_pLevelIcoWindow;
		return ;
	}


	// ��������
	TCHAR szBuf[64];
	memset( szBuf, 0, sizeof(szBuf) );

	// 20100510 ModifyCode
	_stprintf( szBuf, "(%d)", nLevel );

	CEGUI::String32 strUtfName32;
	CUIStringSystem::GetMe()->ParserString_Runtime( szBuf, strUtfName32 );

	if( m_pLevelTextWindow && !strUtfName32.empty() )
	{
		m_pLevelTextWindow->setText(strUtfName32);
	}
}


VOID CCreatureBoard::SetElement_PaoPaoText( LPCTSTR szPaoPao )
{
	if( CUISystem::GetMe()->IsPaoPaoActive() == false )
		return ;

	CEGUI::String32 strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(szPaoPao, strUtf32);

	if( m_pPaoPao )
	{
		m_pPaoPao->show();
		m_pPaoPao->setText( strUtf32 );
	}
}

// ���ù��ұ��
VOID CCreatureBoard::SetElement_CountryFlag( INT nCountry )
{
	// ���ұ��
	switch( nCountry )
	{
	case 0:
		((CEGUI::StaticImage*)m_pCountryFlag)->setImage( (CEGUI::utf8*)"blood",(CEGUI::utf8*)"blood_xinyue" );	// Icon_Gold, UIIcons
		m_pCountryFlag->show();
		break;
	case 1:
		((CEGUI::StaticImage*)m_pCountryFlag)->setImage( (CEGUI::utf8*)"blood",(CEGUI::utf8*)"blood_jinzhang" );
		m_pCountryFlag->show();
		break;
	case 2:
		((CEGUI::StaticImage*)m_pCountryFlag)->setImage( (CEGUI::utf8*)"blood",(CEGUI::utf8*)"blood_damo" );
		m_pCountryFlag->show();
		break;
	case 3:
		((CEGUI::StaticImage*)m_pCountryFlag)->setImage( (CEGUI::utf8*)"blood",(CEGUI::utf8*)"blood_xueyu" );
		m_pCountryFlag->show();
		break;

	default:
		m_pCountryFlag->hide();

	};

}


//���óƺ�
VOID CCreatureBoard::SetElement_Title(LPCTSTR szTitle, INT nType)
{
//	KLAssert(szTitle);

	if( strlen(szTitle) == 0 )
	{
		switch( m_nTitleType )
		{
		case COUNTRY_TITLE:
			{
				if( m_pCountryTitleWindow )
				{
					m_pCountryTitleWindow->setText("");
					m_pCountryTitleWindow->hide();
				}
			}
			break;
		case BANGPAI_TITLE:
			{
				if( m_pGuildTitleWindow )
				{
					m_pGuildTitleWindow->setText("");
					m_pGuildTitleWindow->hide();
				}
			}
			break;
		case SELF_ADD_TITLE:
			{
				if( m_pTitleWindow )
				{
					m_pTitleWindow->setText("");
					m_pTitleWindow->hide();
				}
			}
			break;
		};

		return;
	}


	STRING strColorTitle;
	CEGUI::String32 strUtf32;

	strColorTitle = szTitle;
	m_nTitleType = nType;

	CUIStringSystem::GetMe()->ParserString_Runtime(strColorTitle.c_str(), strUtf32);

	switch( m_nTitleType )
	{
	case COUNTRY_TITLE:
		{
			if( m_pCountryTitleWindow )
			{
				m_pCountryTitleWindow->setText( strUtf32 );
			}
		}
		break;
	case BANGPAI_TITLE:
		{
			if( m_pGuildTitleWindow )
			{
				m_pGuildTitleWindow->setText( strUtf32 );

				switch(m_nBangpaiFlag)
				{
				case NO_POS:
					break;
				case EXCELLENCE_POS:	// ��Ӣ
					{
						((CEGUI::StaticImage*)m_pBangpaiFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
						m_pBangpaiFlag->show();
					}
					break;
				case OFFICEHOLDER_POS:	// ��Ա
					{
						((CEGUI::StaticImage*)m_pBangpaiFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
						m_pBangpaiFlag->show();
					}
					break;
				case CHAIRMAN_POS:		// ����
					{
						((CEGUI::StaticImage*)m_pBangpaiFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
						m_pBangpaiFlag->show();
					}
					break;
				}
			}
		}
		break;
	case SELF_ADD_TITLE:
		{
			if( m_pTitleWindow )
			{
				m_pTitleWindow->setText( strUtf32 );
			}
		}
		break;
	};
}

//���öӳ����
VOID CCreatureBoard::SetElement_LeaderFlag( BOOL bShow, INT nType )
{
/*
	// �ӳ�
	if( 0 == nType )
		m_bLeaderFlag = bShow;
	//
	else if( 1 == nType )
		m_bTeamFlag = bShow;

	_UpdateNameElement();

*/

	if( m_strName.empty() )
	{
		m_pNameWindow->setText( (CEGUI::utf8*)"" );
		m_pWindow->hide();
		return;
	}

	// 20100513 ModifyCodeBegin ������ʾ��Ա���ӳ���־
	if( 0 == nType )	  // ��Ա
		((CEGUI::StaticImage*)m_pLeadFlag)->setImage( (CEGUI::utf8*)"UIIcons",(CEGUI::utf8*)"Confraternity_Class2" );
	else if( 1 == nType ) // �ӳ�
		((CEGUI::StaticImage*)m_pLeadFlag)->setImage( (CEGUI::utf8*)"UIIcons",(CEGUI::utf8*)"Icon_Captain_1" );
	// 20100513 ModifyCodeEnd


	if( bShow )
	{
		// ��̯״̬ʱ�������ǲ���ʾ	20100527 BLL
		if ( m_bShowStallSign == FALSE ) 
		{
			m_pLeadFlag->show();
		}		
	}
	else
		m_pLeadFlag->hide();

}


// ���� pk��� | ������ʾ
VOID CCreatureBoard::SetElement_PKFlag( INT nType, INT nState, BOOL bShow )
{
	// 20100513 AddCodeBegin 
	// ������뺯����Ŀ����Ϊ�����ش��ڣ������غ󷵻�
	if ( m_pPKWindow->isVisible() == TRUE && bShow == FALSE )
	{
		m_pPKWindow->hide();
		return;
	}

	// �����ǰ����������NPC��������״̬û�б仯�Ļ���ֱ�ӷ��أ�����SetAnimate���³�ʼ��������ʼʱ�䣬���¶���ʼ�ղ��ŵ�һ��ͼƬ
	if ( nType == 1 && ( m_missionState == nState ) ) 
	{
		return;
	}
	else
		m_missionState = nState;

	// 20100513 AddCodeEnd

	// 20100513 ModifyCodeBegin ��������NPCͷ����̬ͼƬΪ����
	// pk
	if( 0 == nType ) // ������ʱû�ҵ�����PK״̬�ĵط�����ע�͵������Ժ�ӵ�������֪ͨʱ���ϣ�PK��ǻ�û�ж���ͼƬ
	{
		//((CEGUI::StaticImage*)m_pPKWindow)->setImage( (CEGUI::utf8*)"blood",(CEGUI::utf8*)"blood_xueyu" );
	}
	// ������ʾ
	else 
	{
		// ���ݲ�ͬ��״̬��ʾ
		switch( nState )
		{
		// �пɽ�����
		case 0:
			{
				//((CEGUI::StaticImage*)m_pPKWindow)->setImage( (CEGUI::utf8*)"A_npctop_1",(CEGUI::utf8*)"A_npctop_1_2" );
				((CEGUI::FalagardAnimate*)m_pPKWindow)->SetAnimate( (CEGUI::utf8*)"questsign_accept" );
				break;
			}
		// �ѽ����� δ���
		case 1:
			{
				//((CEGUI::StaticImage*)m_pPKWindow)->setImage( (CEGUI::utf8*)"A_npctop_1",(CEGUI::utf8*)"A_npctop_1_3" );
				((CEGUI::FalagardAnimate*)m_pPKWindow)->SetAnimate( (CEGUI::utf8*)"questsign_un" );
				break;
			}
		// �ѽ����� �����
		case 2:
			{
				//((CEGUI::StaticImage*)m_pPKWindow)->setImage( (CEGUI::utf8*)"A_npctop_1",(CEGUI::utf8*)"A_npctop_1_1" );
				((CEGUI::FalagardAnimate*)m_pPKWindow)->SetAnimate( (CEGUI::utf8*)"questsign_accomplish" );
				break;
			}
		};
	}

	// 20100513 ModifyCodeEnd

	
	if( bShow )
		m_pPKWindow->show();
	else
		m_pPKWindow->hide();
}


//���ð�̯�ϵ�����
VOID CCreatureBoard::SetElement_SaleText(LPCTSTR szSaleText)
{
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(szSaleText, strUtf8);

	if( m_pSaleWindow != NULL )
	{
		//m_pSaleWindow->show();

		m_pSaleWindow->setText( (CEGUI::String32)(CEGUI::utf8*)(strUtf8.c_str()) );
	}
	
}
//�����Ƿ���ʾ��̯��Ϣ
VOID CCreatureBoard::SetElement_SaleSign(BOOL bShow)
{
  	if(bShow)
	{
		m_bShowStallSign = TRUE;
		if(m_pSaleWindow) 
			m_pSaleWindow->show();
	}
	else
	{
		m_bShowStallSign = FALSE;
		if(m_pSaleWindow) 
			m_pSaleWindow->hide();
	}
}
	//�����߼�����ID
VOID CCreatureBoard::SetElement_ObjId(INT nObjId)
{
	m_nObjId = nObjId;
} 

bool CCreatureBoard::handleSaleSignClicked(const CEGUI::EventArgs& e)
{
	g_pGameInterface->Stall_OpenStallSale(m_nObjId);
	return false;
}

void CCreatureBoard::_UpdateNameElement(void)
{
	if(m_strName.empty())
	{
		m_pNameWindow->setText((CEGUI::utf8*)"");
		m_pWindow->hide();
		return;
	}

	CEGUI::String32 strUtfName32;
	CUIStringSystem::GetMe()->ParserString_Runtime(m_strName, strUtfName32);

/*
	// �ӳ����
	if( m_bLeaderFlag )
	{
		m_pLeadFlag->show();
	}
	else
	{
		m_pLeadFlag->hide();
	}
*/

/*
	if( !m_bTeamFlag && !m_pLeadFlag )
	{
		m_pLeadFlag->hide();
	}
	else
	{
		if( m_bLeaderFlag )
		{
//			((CEGUI::StaticImage*)m_pLeadFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
			m_pLeadFlag->show();
		}
		else if( m_bTeamFlag )
		{
//			((CEGUI::StaticImage*)m_pLeadFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
			m_pLeadFlag->show();
		}
	}
*/

	if(m_pNameWindow && !strUtfName32.empty())
	{
		m_pNameWindow->show();
		m_pNameWindow->setText(strUtfName32);
		m_pWindow->hide();
	}
}

VOID CCreatureBoard::SetInfoState( int nState )
{
	m_InfoState = nState;
}


// ��ʾ��̯״̬����Ϣ��	20100527 BLL
VOID CCreatureBoard::ShowIsInStall( BOOL bIsMyself )
{
	m_bShowHpLevel = FALSE;			// �Ƿ���ʾѪ���͵ȼ�

	m_bCountryTitle = FALSE;		// ���ҳƺ�
	m_bGuildTitle = FALSE;			// ���ƺ�
	m_bTitle = FALSE;				// �淨�ƺ�

	m_bShowLevelIco = FALSE;

	if ( bIsMyself == TRUE )
	{
		m_pNameWindow->show();

		m_pHPlevelWindow->hide();
		m_pHPWindow->hide();
		m_pLevelTextWindow->hide();
		m_pLevelIcoWindow->hide();

		m_pCountryTitleWindow->hide();
		m_pGuildTitleWindow->hide();
		m_pTitleWindow->hide();

		m_pLeadFlag->hide();
		m_pBangpaiFlag->hide();
		m_pPKWindow->hide();

		m_pCountryFlag->show();

		SetElement_SaleSign( TRUE );
	}
	else
	{	
		m_pNameWindow->hide();

		m_pHPlevelWindow->hide();
		m_pHPWindow->hide();
		m_pLevelTextWindow->hide();
		m_pLevelIcoWindow->hide();

		m_pCountryTitleWindow->hide();
		m_pGuildTitleWindow->hide();
		m_pTitleWindow->hide();

		m_pCountryFlag->hide();

		m_pLeadFlag->hide();
		m_pBangpaiFlag->hide();
		m_pPKWindow->hide();	
		
		SetElement_SaleSign( TRUE );
	}
}                                           