
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

	//设置大小
	m_pWindow->setProperty("Position", "x:0 y:0");
	m_pWindow->setProperty("Size", "w:1 h:1");

	//玩家名称
	m_pNameWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playername__");
	m_pNameWindow->hide();//laim

	// 血条和等级的背景
	m_pHPlevelWindow = CEGUI::WindowManager::getSingleton().getWindow(	m_pWindow->getName() + "__auto_playerhplevel__");
	m_pHPlevelWindow->hide();//laim1641

	// 血条
	m_pHPWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playerhp__");

	// 等级
	m_pLevelTextWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playerleveltext__");
	m_pLevelIcoWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playerlevelico__");


	// PK标记（只有主角显示）		-- 可以考虑给任务NPC显示用
	m_pPKWindow = CEGUI::WindowManager::getSingleton().getWindow(
			m_pWindow->getName() + "__auto_playerpktag__");

	//摆摊按钮
	m_pSaleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_salessign__");
	m_pSaleWindow->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&CCreatureBoard::handleSaleSignClicked, this));


	// 国家称号
	m_pCountryTitleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playercountrytitle__");
	
	// 帮会称号
	m_pGuildTitleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playerguildtitle__");

	//称号
	m_pTitleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playertitle__");

	// 国家标记
	m_pCountryFlag = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_countryflag__");

	//帮派权限小图标
	m_pBangpaiFlag = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_bangpaiflag__");

	//队长标志
	m_pLeadFlag = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_leaderflag__");

	// 聊天泡泡
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

//删除自身
VOID CCreatureBoard::Destroy(VOID)
{
	if( CUICreatureBoardSystem::GetMe() )
		CUICreatureBoardSystem::GetMe()->DestroyCreatureBoard(this);
	return;
	//不能再调用任何代码
	//...
}

fVector2 CCreatureBoard::GetPosition(void) const
{
	CEGUI::Point pt = CEGUI::PropertyHelper::stringToPoint(m_pNameWindow->getProperty( "FitPosition" ));

	return fVector2(pt.d_x, pt.d_y);
}

//设置在屏幕上的坐标(像素)
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

	// 判断是否要显示
	if( m_bShowHpLevel )
	{
		if( m_pHPlevelWindow && m_pHPWindow )
		{
			// 计算位置
			fOffset -= m_pHPlevelWindow->getAbsoluteHeight();
			fPos.d_y += fOffset;
			fPos.d_x -= m_pHPlevelWindow->getAbsoluteWidth() / 2.0f;
			m_pHPlevelWindow->setPosition(CEGUI::Absolute, fPos);

			// 血条
			m_pHPWindow->setPosition(CEGUI::Absolute, fPos);

			// 设置进度
			((CEGUI::ProgressBar*)m_pHPWindow)->setProgress( m_fProgross );

			//laim1648 m_pHPWindow->show();
		}
	}
	
	// 恢复x偏移
	fPos.d_x = nX;
	fPos.d_y = nY;

	// 计算名字的位置
	fPos.d_y += fOffset;

	( ( CEGUI::IFalagardSelfFitWindow* )( CEGUI::FalagardSelfFitWindow* )m_pNameWindow )->setCenter(fPos);
	fOffset -= m_pNameWindow->getAbsoluteHeight();
	
	// 名字
	if( m_pNameWindow->isVisible() && !( m_pNameWindow->getText().empty() ) )
	{
		m_pWindow->show();
		debugInfo();
	}
	else
	{
		// 摆摊状态时，其他玩家名字不显示时，信息板还要显示	20100527 BLL
		if ( m_bShowStallSign == FALSE ) 
		{
			m_pWindow->hide();
		}
		else
			m_pWindow->show();
	}

	// 20100504 AddCodeBegin
	// 级别显示在玩家名字后面
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

	
	// 国家称号
	if( m_bCountryTitle && !( m_pCountryTitleWindow->getText().empty() ) )
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pCountryTitleWindow)->setCenter( fPos );
		fOffset -= m_pCountryTitleWindow->getAbsoluteHeight();

		m_pCountryTitleWindow->show();
	}

	// 帮会称号
	if( m_bGuildTitle && !( m_pGuildTitleWindow->getText().empty() ) )
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pGuildTitleWindow)->setCenter( fPos );	
		fOffset -= m_pGuildTitleWindow->getAbsoluteHeight();

		m_pGuildTitleWindow->show();
	}

	// 称号
	if( m_bTitle && !(m_pTitleWindow->getText().empty()) )
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pTitleWindow)->setCenter(fPos);	
		fOffset -= m_pTitleWindow->getAbsoluteHeight();

		m_pTitleWindow->show();
	}

	// 摆摊
	if( m_pSaleWindow->isVisible() )
	{
		fPos.d_y = fY + fOffset;
		m_pSaleWindow->setProperty( "HookPosition", CEGUI::PropertyHelper::pointToString( fPos ) );
		((CEGUI::PushButton*)m_pSaleWindow)->resizeWithText();
		fOffset -= m_pSaleWindow->getAbsoluteHeight();
	}

	// 头顶泡泡
	if( m_pPaoPao->isVisible() )
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pPaoPao)->setCenter(fPos);
		fOffset  -= m_pPaoPao->getAbsoluteHeight();
	}

	// PK标志 
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


	// 国家标志
	size = m_pNameWindow->getAbsoluteSize();
	fPos = m_pNameWindow->getAbsolutePosition();

	fPos.d_x -= size.d_height;
	m_pCountryFlag->setPosition(CEGUI::Absolute, fPos);

	size.d_width = size.d_height;
	m_pCountryFlag->setSize(CEGUI::Absolute, size);


	// 队长标志
	size = m_pNameWindow->getAbsoluteSize();
	fPos = m_pNameWindow->getAbsolutePosition();

	// 20100504 ModifyCodeBegin 级别改为在名字后面显示后，修改队长标记位置
	CEGUI::Size  levelTextSize = m_pLevelTextWindow->getAbsoluteSize();

	fPos.d_x += size.d_width;
	fPos.d_x += levelTextSize.d_width;
	// 20100504 ModifyCodeEnd

	m_pLeadFlag->setPosition(CEGUI::Absolute, fPos);
	size.d_width = size.d_height;
	m_pLeadFlag->setSize(CEGUI::Absolute, size);


	// 帮派权限图标标的位置
	size = m_pTitleWindow->getAbsoluteSize();
	fPos = m_pTitleWindow->getAbsolutePosition();

	fPos.d_x -= size.d_height;
	m_pBangpaiFlag->setPosition( CEGUI::Absolute, fPos );

	size.d_width = size.d_height;
	m_pBangpaiFlag->setSize( CEGUI::Absolute, size );


	// 设置透明度
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

//显示/隐藏
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

		// 摆摊
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
	// 国家
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
	// 帮会
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
	// 玩法
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

//设置名字
VOID CCreatureBoard::SetElement_Name(LPCTSTR szName)
{
	m_strName = szName;
	_UpdateNameElement();
	return;
}

// 设置生命值
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

// 设置等级
VOID CCreatureBoard::SetElement_Level( INT nLevel, INT nType )
{
	// 图片， 不处理直接返回
	if( 1 == nType )
	{
		m_bShowLevelIco = TRUE;
		//m_pLevelIcoWindow;
		return ;
	}


	// 设置文字
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

// 设置国家标记
VOID CCreatureBoard::SetElement_CountryFlag( INT nCountry )
{
	// 国家标记
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


//设置称号
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
				case EXCELLENCE_POS:	// 精英
					{
						((CEGUI::StaticImage*)m_pBangpaiFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
						m_pBangpaiFlag->show();
					}
					break;
				case OFFICEHOLDER_POS:	// 官员
					{
						((CEGUI::StaticImage*)m_pBangpaiFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
						m_pBangpaiFlag->show();
					}
					break;
				case CHAIRMAN_POS:		// 帮主
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

//设置队长标记
VOID CCreatureBoard::SetElement_LeaderFlag( BOOL bShow, INT nType )
{
/*
	// 队长
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

	// 20100513 ModifyCodeBegin 区分显示队员、队长标志
	if( 0 == nType )	  // 队员
		((CEGUI::StaticImage*)m_pLeadFlag)->setImage( (CEGUI::utf8*)"UIIcons",(CEGUI::utf8*)"Confraternity_Class2" );
	else if( 1 == nType ) // 队长
		((CEGUI::StaticImage*)m_pLeadFlag)->setImage( (CEGUI::utf8*)"UIIcons",(CEGUI::utf8*)"Icon_Captain_1" );
	// 20100513 ModifyCodeEnd


	if( bShow )
	{
		// 摆摊状态时，队伍标记不显示	20100527 BLL
		if ( m_bShowStallSign == FALSE ) 
		{
			m_pLeadFlag->show();
		}		
	}
	else
		m_pLeadFlag->hide();

}


// 设置 pk标记 | 任务提示
VOID CCreatureBoard::SetElement_PKFlag( INT nType, INT nState, BOOL bShow )
{
	// 20100513 AddCodeBegin 
	// 如果进入函数的目的是为了隐藏窗口，则隐藏后返回
	if ( m_pPKWindow->isVisible() == TRUE && bShow == FALSE )
	{
		m_pPKWindow->hide();
		return;
	}

	// 如果当前对象是任务NPC，且任务状态没有变化的话，直接返回，避免SetAnimate重新初始化播放起始时间，导致动画始终播放第一张图片
	if ( nType == 1 && ( m_missionState == nState ) ) 
	{
		return;
	}
	else
		m_missionState = nState;

	// 20100513 AddCodeEnd

	// 20100513 ModifyCodeBegin 更改任务NPC头顶静态图片为动画
	// pk
	if( 0 == nType ) // 程序暂时没找到设置PK状态的地方，先注释掉，待以后接到服务器通知时加上，PK标记还没有动画图片
	{
		//((CEGUI::StaticImage*)m_pPKWindow)->setImage( (CEGUI::utf8*)"blood",(CEGUI::utf8*)"blood_xueyu" );
	}
	// 任务提示
	else 
	{
		// 根据不同的状态显示
		switch( nState )
		{
		// 有可接任务
		case 0:
			{
				//((CEGUI::StaticImage*)m_pPKWindow)->setImage( (CEGUI::utf8*)"A_npctop_1",(CEGUI::utf8*)"A_npctop_1_2" );
				((CEGUI::FalagardAnimate*)m_pPKWindow)->SetAnimate( (CEGUI::utf8*)"questsign_accept" );
				break;
			}
		// 已接任务， 未完成
		case 1:
			{
				//((CEGUI::StaticImage*)m_pPKWindow)->setImage( (CEGUI::utf8*)"A_npctop_1",(CEGUI::utf8*)"A_npctop_1_3" );
				((CEGUI::FalagardAnimate*)m_pPKWindow)->SetAnimate( (CEGUI::utf8*)"questsign_un" );
				break;
			}
		// 已接任务， 已完成
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


//设置摆摊上的文字
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
//设置是否显示摆摊信息
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
	//设置逻辑对象ID
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
	// 队长标记
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


// 显示摆摊状态的信息板	20100527 BLL
VOID CCreatureBoard::ShowIsInStall( BOOL bIsMyself )
{
	m_bShowHpLevel = FALSE;			// 是否显示血条和等级

	m_bCountryTitle = FALSE;		// 国家称号
	m_bGuildTitle = FALSE;			// 帮会称号
	m_bTitle = FALSE;				// 玩法称号

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