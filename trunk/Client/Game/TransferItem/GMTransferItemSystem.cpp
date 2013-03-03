#include "StdAfx.h"
#include "GMTransferItemSystem.h"
#include "GIException.h"
#include "GIObjectManager.h"
#include "GITimeSystem.h"
#include "GIUISystem.h"
#include "..\DBC\GMDataBase.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Event\GMEventSystem.h"
#include "..\..\Common\GameStruct_Item.h"
#include "..\Object\Manager\ObjectManager.h"
#include "..\Object/Logic/Character/Obj_PlayerMySelf.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\DataPool\GMDataPool.h"
#include "..\Object/Logic/Item/Obj_Item.h"
//#include "..\Item\GMItem.h"
//#include "..\Item\GMItem_Equip.h"
#include "..\Interface\GMInterface_Script_Tooltip.h"
#include "..\Interface\GMInterface_Script_Talk.h"
#include "..\Input\GMInputSystem.h"
#include "..\Action\GMActionSystem.h"
//#include "..\SeraphLib\SeraphInputCheck_3.h"

const STRING g_TransferItemNameFlagBegin	= "[";
const STRING g_TransferItemNameFlagEnd		= "]";
const STRING g_TransferChannelNameFlagBegin	= "【";
const STRING g_TransferChannelNameFlagEnd	= "】";

ItemElement::ItemElement()
{
	nType = ELEMENT_ITEMINFO;
	useCount = 1;

	pData = NULL;
}

STRING ItemElement::GetDisplayColor()
{
	if(pData)
	{
		STRING strPrefix = "#c";
		STRING strColor = strPrefix + pData->GetDropVisColor();
		if(strColor.empty()) strColor = "#W";
		return strColor;
	}

	return "#W";
}


STRING ItemElement::GetDisplayPretxt()
{
	//if(pData && pData->getClass() == IC_EQUIP)
	//{
	//	STRING strPre = "";
	//	CItem_Equip* pItemEquip = (CItem_Equip*)pData;
	//	INT id = static_cast<CItem*>(pItemEquip)->getTableIndex();
	//	if(id>=EQUIP_SHENQI_BEGIN && id<=EQUIP_SHENQI_END)
	//	{
	//		strPre = COLORMSGFUNC("CTransferSystem_ItemShen_Prefix");
	//	}
	//	else if(pItemEquip->getSetID() != -1)
	//	{
	//		strPre = COLORMSGFUNC("CTransferSystem_ItemSet_Prefix");
	//	}
	//	return strPre;
	//}
	return "";
}

HyperLinkElement::HyperLinkElement()
{
	nType = ELEMENT_HYPERLINK;
	useCount = 1;
}

STRING HyperLinkElement::GetDisplayColor()
{
	STRING strColor;

	//判断频道来决定显示颜色
	switch(m_nChannelType)
	{
	case CHAT_TYPE_TEAM:		//队聊消息
	case CHAT_TYPE_SCENE:		//场景消息
	case CHAT_TYPE_TELL:		//私聊消息
	case CHAT_TYPE_GUILD:		//帮派消息
	case CHAT_TYPE_MENPAI:		//门派消息
	//case CHAT_TYPE_IPREGION:	//同乡消息
		{
			strColor = SCRIPT_SANDBOX::Talk::s_Talk.GetColorByChannelType(m_nChannelType,TRUE);
		}
		break;
	//case CHAT_TYPE_AUTO:
	//	{
	//		DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);
	//		const _DBC_SYSTEM_COLOR* pSysColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TRANSFER_BLUE);
	//		if(pSysColor)
	//		{
	//			//字体颜色
	//			if(strlen(pSysColor->strFontColor) == 6)
	//			{
	//				strColor += "#c";
	//				strColor += pSysColor->strFontColor;
	//			}
	//		}
	//	}
	//	break;
	case CHAT_TYPE_INVALID:
	case CHAT_TYPE_NORMAL:		//普通说话消息
	case CHAT_TYPE_SELF:		//仅客户端使用的消息
	//case CHAT_TYPE_HELP:		//帮助消息
	case CHAT_TYPE_SYSTEM:		//系统消息
	case CHAT_TYPE_CHANNEL:		//自建聊天频道消息
	default:
		{
			//DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);
			//const _DBC_SYSTEM_COLOR* pSysColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TRANSFER_BLUE);
			//if(pSysColor)
			//{
			//	//字体颜色
			//	if(strlen(pSysColor->strFontColor) == 6)
			//	{
			//		strColor += "#c";
			//		strColor += pSysColor->strFontColor;
			//	}
			//}
		}
		break;
	}

	return strColor;
}

ChannelElement::ChannelElement()
{
	nType = ELEMENT_CHANNEL;
	useCount = 1;
}

PetElement::PetElement()
{
	nType = ELEMENT_PET;
	useCount = 1;

	m_Pet.CleanUp();
}

STRING PetElement::GetDisplayPretxt()
{
	if(1 == m_Pet.m_nEraCount)
	{
		STRING strPre = COLORMSGFUNC("CTransferSystem_Pet2_Prefix");
		return strPre;
	}

	return "";
}

STRING PetElement::GetDisplayColor()
{
	DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);
	//const _DBC_SYSTEM_COLOR* pSysColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TRANSFER_ORANGE);

	STRING strColor;
	//if(pSysColor)
	//{
	//	//字体颜色
	//	if(strlen(pSysColor->strFontColor) == 6)
	//	{
	//		strColor += "#c";
	//		strColor += pSysColor->strFontColor;
	//	}
	//}

	return strColor;
}

BossElement::BossElement()
{
	nType = ELEMENT_BOSS;
	useCount = 1;

	m_nBossID = -1;
}


CTransferItemSystem* CTransferItemSystem::s_pMe = NULL;
KL_IMPLEMENT_DYNAMIC(CTransferItemSystem, GETCLASS(tTransferItemSystem));
CTransferItemSystem::CTransferItemSystem() :
	m_nIdFactory(0),
	m_pCurElement(0)
{
	s_pMe = this;
}

CTransferItemSystem::~CTransferItemSystem()
{
	s_pMe = 0;
}

void CTransferItemSystem::Initial(VOID*)
{

}
void CTransferItemSystem::ReSet(VOID)
{
	m_nIdFactory = 0;
	m_pCurElement = NULL;
	m_mapElements.clear();
}
void CTransferItemSystem::Release(VOID)
{
	std::vector<INT> vID;
	ElementMap::iterator it = m_mapElements.begin();
	for(; it != m_mapElements.end(); ++it)
	{
		vID.push_back(it->first);

		Element * pE = it->second;
		pE->useCount = 1;
	}

	for(INT i = 0; i < (INT)vID.size(); ++i)
	{
		DeleteElement(vID[i]);
	}

	m_mapElements.clear();
}

CTransferItemSystem::Element* CTransferItemSystem::ConvertFromItem(tObject_Item* pItem)
{
	KLAssert(pItem);
	if(!pItem) return 0;
	//if(pItem->getNeedExtraInfo()) return 0;

	//创建新元素
	ItemElement *newElement = new ItemElement;
	if(!newElement) return 0;

	newElement->pData = CObject_Item::NewItem( pItem->GetIdTable() );
	newElement->pData->Clone(pItem);

	newElement->nID = m_nIdFactory++;
	newElement->nObjID = pItem->GetID();

	//更新相应的内容信息
	newElement->strOrgDisplay = pItem->GetName();
	
	if(GenItemBinStr(pItem ,newElement->strContents))
	{
		//生成显示名称
		newElement->strDisplay = g_TransferItemNameFlagBegin;
		newElement->strDisplay += GetDisplayPrefix(*newElement);
		newElement->strDisplay += pItem->GetName();
		newElement->strDisplay += g_TransferItemNameFlagEnd;
		newElement->strDisplay = GetDisplayColour(*newElement)+newElement->strDisplay;
		//加入map
		m_mapElements[newElement->nID] = newElement;
		//创建Tooltip使用的ActionItem
		newElement->pData->SetGUID(0,0,0);
		//newElement->pData->setContainer(CT_TRANSFER_ITEM);
		newElement->pData->SetTypeOwner( tObject_Item::IO_ITEMTRANSFER );
		newElement->pData->SetPosIndex(newElement->nID);

		CActionSystem::GetMe()->TransferSystem_Update(newElement->pData);
		//返回
		return (m_mapElements.find(newElement->nID) )->second;
	}
	else
	{
		m_nIdFactory--;
		delete newElement;
		return NULL;
	}
}

CTransferItemSystem::Element* CTransferItemSystem::ConvertFromPet(INT nIdx)
{
	//SDATA_PET* My_Pet = CDataPool::GetMe()->Pet_GetPet(nIdx);

	//if(My_Pet && (0 < My_Pet->m_nIsPresent) && (!My_Pet->m_GUID.IsNull()) && (My_Pet->m_nDataID >= 0))
	//{
	//	PetElement* newElement = new PetElement;
	//	if(!newElement) return 0;

	//	CDataPool::Pet_Copy(My_Pet, &(newElement->m_Pet));

	//	newElement->nID = m_nIdFactory++;
	//	newElement->nObjID = -1;
	//	
	//	newElement->strOrgDisplay = My_Pet->GetDBCPetName();

	//	if(My_Pet->GetTransferString(newElement->strContents) > 0)
	//	{
	//		//生成显示名称
	//		newElement->strDisplay = g_TransferItemNameFlagBegin;
	//		newElement->strDisplay += GetDisplayPrefix(*newElement);
	//		newElement->strDisplay += My_Pet->GetDBCPetName();
	//		newElement->strDisplay += g_TransferItemNameFlagEnd;
	//		newElement->strDisplay = GetDisplayColour(*newElement) + newElement->strDisplay;
	//		//加入map
	//		m_mapElements[newElement->nID] = newElement;
	//		//返回
	//		return m_mapElements.find(newElement->nID)->second;
	//	}
	//	else
	//	{
	//		m_nIdFactory--;
	//		delete newElement;
	//		return NULL;
	//	}
	//}

	return NULL;
}

CTransferItemSystem::Element* CTransferItemSystem::ConvertFromString(const STRING& strIn, const STRING& strContents)
{
	if(strContents.size() >= 0)
	{
		STRING strOp = strContents.substr(0, 1);
		if(strOp == "i")
		{//转换为物品
			return ConvertItemFromString(strIn, strContents);
		}
		else if(strOp == "p")
		{//转换为珍兽
			return ConvertPetFromString(strIn, strContents);
		}
		else if(strOp == "b")
		{//转换为BOSS
			return ConvertBossFromString(strIn, strContents);
		}
	}

	return NULL;
}

CTransferItemSystem::Element* CTransferItemSystem::ConvertPetFromString(const STRING& strIn, const STRING& strContents)
{
	////创建新元素
	//PetElement *newElement = new PetElement;
	//if(!newElement) return 0;

	//newElement->nID = m_nIdFactory++;

	//newElement->m_Pet.SetTransferString(strContents.c_str());
	//if(newElement->m_Pet.m_nIsPresent != INVALID_ID && newElement->m_Pet.GetDBCPetName())
	//{
	//	//生成显示名称
	//	newElement->strDisplay = g_TransferItemNameFlagBegin;
	//	newElement->strDisplay += GetDisplayPrefix(*newElement);
	//	if(!strIn.empty())
	//	{
	//		newElement->strDisplay += strIn;
	//	}
	//	else
	//	{
	//		newElement->strDisplay += newElement->m_Pet.GetDBCPetName();
	//	}
	//	newElement->strDisplay += g_TransferItemNameFlagEnd;
	//	newElement->strOrgDisplay = newElement->m_Pet.GetDBCPetName();

	//	newElement->strDisplay = GetDisplayColour(*newElement)+newElement->strDisplay;
	//	//加入map
	//	m_mapElements[newElement->nID] = newElement;
	//	//返回
	//	return m_mapElements.find(newElement->nID)->second;
	//}
	//else
	//{
	//	m_nIdFactory--;
	//	delete newElement;
	//	return NULL;
	//}
	return NULL;
}

CTransferItemSystem::Element* CTransferItemSystem::ConvertItemFromString(const STRING& strIn, const STRING& strContents)
{
	//创建新元素
	ItemElement *newElement = new ItemElement;
	if(!newElement) return 0;

	newElement->nID = m_nIdFactory++;
	newElement->strContents = strContents;

	//更新相应的内容信息
	//这句错误，注释掉
	//newElement->strOrgDisplay = strIn;

	GenBinStrItem(newElement->pData, strContents);
	if(newElement->pData)
	{
		//生成显示名称
		newElement->strDisplay = g_TransferItemNameFlagBegin;
		newElement->strDisplay += GetDisplayPrefix(*newElement);
		if(!strIn.empty())
		{
			newElement->strDisplay += strIn;
			newElement->strOrgDisplay = strIn;
		}
		else
		{
			newElement->strDisplay += newElement->pData->GetName();
			newElement->strOrgDisplay = newElement->pData->GetName();
		}
		newElement->strDisplay += g_TransferItemNameFlagEnd;

		newElement->strDisplay = GetDisplayColour(*newElement)+newElement->strDisplay;
		//加入map
		m_mapElements[newElement->nID] = newElement;

		//创建Tooltip使用的ActionItem
		newElement->pData->SetGUID(0,0,0);
		//newElement->pData->setContainer(CT_TRANSFER_ITEM);
		newElement->pData->SetTypeOwner( tObject_Item::IO_ITEMTRANSFER );
		newElement->pData->SetPosIndex(newElement->nID);

		CActionSystem::GetMe()->TransferSystem_Update(newElement->pData);

		//返回
		return ( m_mapElements.find(newElement->nID) )->second;
	}
	else
	{
		m_nIdFactory--;
		return NULL;
	}
	return NULL;
}

CTransferItemSystem::Element* CTransferItemSystem::ConvertBossFromString(const STRING& strIn, const STRING& strContents)
{
	////创建新元素
	//BossElement *newElement = new BossElement;
	//if(!newElement) return 0;

	//newElement->nID = m_nIdFactory++;

	////获取BossID....
	//INT temp = 0;
	//sscanf(strContents.c_str(),"%db%d", &temp, &(newElement->m_nBossID) );

	////生成显示名称....
	//DBC_DEFINEHANDLE( s_RefBoss, DBC_WORLDREF_BOSS );
	//const _DBC_WORLDREF_BOSS* pBoss = (_DBC_WORLDREF_BOSS*)( s_RefBoss->Search_First_Column_Equ( 4, newElement->m_nBossID ) );
	//if( NULL == pBoss )
	//{
	//	m_nIdFactory--;
	//	delete newElement;
	//	return NULL;
	//}
	//newElement->strOrgDisplay = pBoss->TransText;
	//newElement->strDisplay = pBoss->TransText;

	////加入map
	//m_mapElements[newElement->nID] = newElement;

	////返回
	//return m_mapElements.find(newElement->nID)->second;
	return NULL;
}

CTransferItemSystem::Element* CTransferItemSystem::AddHyperLink(const STRING& strName, const STRING& strContex,INT nCh/* = CHAT_TYPE_INVALID*/)
{
	//0. 查找相同的超链信息
	ElementMap::iterator it = m_mapElements.begin();
	for(; it != m_mapElements.end(); ++it)
	{
		Element *pE = it->second;
		if(!pE->IsType(ELEMENT_HYPERLINK)) continue;

		if( pE->strContents == strContex && 
			pE->strOrgDisplay == strName &&
			((HyperLinkElement*)pE)->m_nChannelType == nCh
		) break;
	}

	HyperLinkElement* newElement = NULL;
	//1. 没找到的话，就添加新的
	if(it == m_mapElements.end())
	{
		newElement = new HyperLinkElement;
		//创建新元素
		newElement->nID = m_nIdFactory++;
		newElement->nObjID = -1;
		newElement->m_nChannelType = nCh;
		//生成显示名称
		newElement->strDisplay = GetDisplayColour(*newElement);
		newElement->strDisplay += g_TransferItemNameFlagBegin;
		newElement->strDisplay += GetDisplayPrefix(*newElement);
		newElement->strDisplay += strName;
		newElement->strDisplay += g_TransferItemNameFlagEnd;

		//更新相应的内容信息
		newElement->strContents = strContex;
		newElement->strOrgDisplay = strName;

		//加入map
		m_mapElements[newElement->nID] = newElement;
	}
	else
	{
		newElement = static_cast<HyperLinkElement*>(it->second);
		AddElementUseCount(it->first);		
	}

	//2. 返回
	return m_mapElements.find(newElement->nID)->second;
}

CTransferItemSystem::Element* CTransferItemSystem::AddChannel(INT nType, const STRING& strContex)
{
	//0. 查找相同的频道信息
	ElementMap::iterator it = m_mapElements.begin();
	for(; it != m_mapElements.end(); ++it)
	{
		Element *pE = it->second;
		if(!pE->IsType(ELEMENT_CHANNEL)) continue;

		if(((ChannelElement*)pE)->m_nChannelType == nType)
		{
			if(pE->strContents == strContex) break;
		}
	}

	ChannelElement* newElement = NULL;
	//1. 没找到的话，就添加新的
	if(it == m_mapElements.end())
	{
		newElement = new ChannelElement;
		//创建新元素
		newElement->nID = m_nIdFactory++;
		newElement->nObjID = -1;

		//显示名称
		const tDataBase* pTalkConfig = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_CONFIG);
		if(pTalkConfig)
		{
			const _DBC_TALK_CONFIG* pLine = (const _DBC_TALK_CONFIG*)pTalkConfig->Search_LineNum_EQU(nType);
			if(pLine)
			{
				newElement->strDisplay = g_TransferChannelNameFlagBegin;
				newElement->strDisplay += pLine->szChannelHeader;
				newElement->strDisplay += g_TransferChannelNameFlagEnd;
			}
		}

		//更新相应的内容信息
		newElement->m_nChannelType = nType;
		newElement->strContents = strContex;	//对应的频道名称	主要是为了私聊的时候准备的。

		//加入map
		m_mapElements[newElement->nID] = newElement;
	}
	else
	{
		newElement = static_cast<ChannelElement*>(it->second);
		AddElementUseCount(it->first);		
	}

	//2. 返回
	return m_mapElements.find(newElement->nID)->second;
}

void CTransferItemSystem::DeleteElement(int nID)
{
	//ElementMap::iterator itFind = m_mapElements.find(nID);
	////if(itFind != m_mapElements.end())
	////{
	////	//char dbgmsg[128] = {0};

	//Element* pE = itFind->second;

	////	//_snprintf(dbgmsg, 127, "OrgDisplay:%s useCount: %d -> ", pE->strOrgDisplay.c_str(),pE->useCount);
	////	//::OutputDebugString(dbgmsg);

	////	pE->useCount--;

	////	//_snprintf(dbgmsg, 127, "%d\n", pE->useCount);
	////	//::OutputDebugString(dbgmsg);

	//	if(0 == pE->useCount)
	//	{
	//		//物品类型的Element需要删除内部数据
	//		if(pE->nType == ELEMENT_ITEMINFO)
	//		{
	//			ItemElement *pItem = (ItemElement*)pE;
	//			
	//			if(pItem->pData)
	//			{
	//				//通知ActionSystem删除无用数据
	//				CActionSystem::GetMe()->TransferSystem_Update(pItem->pData, TRUE);
	//				if(pItem->pData)
	//				{
	//					CObject_Item::DestroyItem( pItem->pData->GetID() );
	//				}
	//				pItem->pData=0;
	//			}
	//		}

	//		//需要清除当前选择的Element
	//		if(m_pCurElement == pE) m_pCurElement = NULL;
	//		
	//		delete pE; pE = 0;
	//		m_mapElements.erase(itFind);
	//	}
	//}
}

void CTransferItemSystem::DoElement(int nID, int Action,int msgID)
{
	ElementMap::iterator itFind = m_mapElements.find(nID);
	if(itFind != m_mapElements.end())
	{
		//根据Element的类型进行相应的处理
		Element *el = itFind->second;
		m_pCurElement = el;

		switch(el->nType) {
		case ELEMENT_HYPERLINK:
			{
				if(Action == ELEMENT_R_CLICK)
				{//显示右键菜单
					if(CEventSystem::GetMe())
					{
						CHAR szmsgID[16] = {0};
						SCRIPT_SANDBOX::Talk::HistoryMsg hmsg;
						if(!SCRIPT_SANDBOX::Talk::s_Talk.GetMsgFromCurTabHistoryQue(hmsg,msgID))
						{
							_snprintf(szmsgID,15,"%d",-1);
						}
						else
						{
							if(hmsg.GetMsgName() == el->strOrgDisplay)
							{
								_snprintf(szmsgID,15,"%d",msgID);
							}
							else
							{
								_snprintf(szmsgID,15,"%d",-1);
							}
						}
						std::vector< STRING > vParam;
						vParam.push_back(el->strOrgDisplay);
						vParam.push_back(el->strContents);
						vParam.push_back(szmsgID);
						CEventSystem::GetMe()->PushEvent(GE_CHAT_CONTEX_MENU, vParam );
						//CEventSystem::GetMe()->PushEvent(GE_CHAT_CONTEX_MENU, el->strOrgDisplay.c_str(),el->strContents.c_str(), szmsgID);
					}
				}
				else if(Action == ELEMENT_L_CLICK)
				{//设为私聊名称
					if(CEventSystem::GetMe())
					{
						CEventSystem::GetMe()->PushEvent(GE_CHAT_CHANGE_PRIVATENAME, el->strOrgDisplay.c_str());
					}
				}
			}
			break;
		case ELEMENT_ITEMINFO:
			{
				if(Action == ELEMENT_L_CLICK)
				{
					if( CInputSystem::GetMe()->IsCtrlDown() )
					{
						//CEventSystem::GetMe()->PushEvent(GE_HISBOARD_LCTRDOWN, el->nID);
						if ( !CGameProcedure::s_pUISystem->InjectItemInfo(el))
						{
							CTransferItemSystem::GetMe()->DeleteElement( el->nID );
						} 
					}
					else
					{
						//显示传输过来的物品的tooltip
						tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(el->nID, NAMETYPE_TRANSFER_ITEM);

						if(CEventSystem::GetMe() && pActionItem)
						{
							SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip2.SetActionItem((CActionItem*)pActionItem);

							ItemElement* pItem = (ItemElement*)el;

							std::vector< STRING > vParam;
							vParam.push_back("1");
							vParam.push_back(pActionItem->GetType_String());

							POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
							

							char szTemp[32];
							_snprintf(szTemp, 32, "%d", ptMouse.x);
							vParam.push_back(szTemp);
							_snprintf(szTemp, 32, "%d", ptMouse.y);
							vParam.push_back(szTemp);
							_snprintf(szTemp, 32, "%d", ptMouse.x+10);
							vParam.push_back(szTemp);
							_snprintf(szTemp, 32, "%d", ptMouse.y+10);
							vParam.push_back(szTemp);

							CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP2, vParam);

							//SI3
							//SI3_ResetCounts();
						}
					}
				}
				else if(Action == ELEMENT_MOVE_OUT)
				{
					////隐藏tooltip
					//if(CEventSystem::GetMe())
					//{
					//	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP2, "0");
					//}

					SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip2.SetActionItem(NULL);
				}
			}
			break;
		case ELEMENT_CHANNEL:
			{
				if(Action == ELEMENT_L_CLICK)
				{
					ChannelElement* pChannel = (ChannelElement*)el;
					//更换当前频道
					SCRIPT_SANDBOX::Talk::s_Talk.ChangeDefaultChannel(pChannel->m_nChannelType, pChannel->strContents);
				}
			}
			break;
		case ELEMENT_PET:
			{
				if(Action == ELEMENT_L_CLICK)
				{
					PetElement* pPet = (PetElement*)el;
					//查看珍兽信息
					CDataPool::GetMe()->Pet_CopyToTarget(&(pPet->m_Pet));
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_TARGETPET_PAGE,INVALID_ID);
				}
			}
			break;
		case ELEMENT_BOSS:
			{
				//if(Action == ELEMENT_L_CLICK)
				//{
				//	// 根据LinkID寻找该BOSS的ID....
				//	BossElement* pBoss = (BossElement*)el;

				//	DBC_DEFINEHANDLE( s_RefBoss, DBC_WORLDREF_BOSS );
				//	const _DBC_WORLDREF_BOSS* pLine = (_DBC_WORLDREF_BOSS*)( ( (tDataBase*)s_RefBoss )->Search_First_Column_Equ( 4, pBoss->m_nBossID ) );

				//	if( NULL == pLine )
				//		return;

				//	CGameProcedure::s_pEventSystem->PushEvent(GE_WR_SHOWBOSSINFO,pLine->nID);
				//}
			}
			break;
		default:
			break;
		}
	}
}

BOOL CTransferItemSystem::IsElementExist(int nID)
{
	return (GetElement(nID))?TRUE:FALSE;
}

CTransferItemSystem::Element* CTransferItemSystem::GetElement(int nID)
{
	ElementMap::iterator itFind = m_mapElements.find(nID);
	if(itFind != m_mapElements.end())
	{
		return itFind->second;
	}
	return NULL;
}

STRING CTransferItemSystem::GetDisplayColour(Element& el)
{
	return el.GetDisplayColor();
}

STRING CTransferItemSystem::GetDisplayPrefix(Element& el)
{
	return el.GetDisplayPretxt();
}

int	CTransferItemSystem::GetElementType(int nID)
{
	if(GetElement(nID))
	{
		return GetElement(nID)->nType;
	}
	return ELEMENT_INVALID;
}
STRING CTransferItemSystem::GetElementDisplayName(int nID)
{
	if(GetElement(nID))
	{
		return GetElement(nID)->strDisplay;
	}
	return "";
}

STRING CTransferItemSystem::GetOrgElementDisplayName(int nID)
{
	if(GetElement(nID))
	{
		return GetElement(nID)->strOrgDisplay;
	}
	return "";
}

STRING CTransferItemSystem::GetElementContents(int nID)
{
	if(GetElement(nID))
	{
		return GetElement(nID)->strContents;
	}
	return "";
}

VOID CTransferItemSystem::AddElementUseCount(int nID)
{
	if(GetElement(nID))
	{
		//char dbgmsg[128] = {0};
		//_snprintf(dbgmsg, 127, "Add Element UseCount ID:%d OrgDisplay:%s useCount: %d -> ", nID, GetElement(nID)->strDisplay.c_str(),GetElement(nID)->useCount);
		//::OutputDebugString(dbgmsg);

		GetElement(nID)->useCount++;

		//_snprintf(dbgmsg, 127, "%d\n", GetElement(nID)->useCount);
		//::OutputDebugString(dbgmsg);
	}
}

// 物品系统重构相关传输修改[12/7/2006]
BOOL CTransferItemSystem::GenItemBinStr( tObject_Item* pItem, STRING& strContents)
{
	if(!pItem) return FALSE;
	//if(pItem->getNeedExtraInfo()) return FALSE;

	INT nRealLength = 0;
	CHAR    strItemTransfer[MAX_ITEM_STRING_LENGTH] = {0};

	//if(Item2String((_ITEM*)(pItem->getExtraInfo()), strItemTransfer, nRealLength, MAX_ITEM_STRING_LENGTH))
	//{
	//	strContents = strItemTransfer;
	//	return TRUE;
	//}
	strContents = pItem->GetExtranInfo();

	//if ( strContents.empty())
	//	return FALSE;

	return TRUE;
}

BOOL CTransferItemSystem::GenBinStrItem(tObject_Item*& pItem, const STRING& strContents)
{
	if(pItem) return FALSE;
	if(strContents.empty()) return FALSE;
    
	const CHAR* pContents = strContents.c_str();

	UINT idTable, nSize;
	CHAR strIn[MAX_ITEM_STRING_LENGTH+1] = {0};
	//描述字串的长度(skip)
	DBStr2Binary(pContents+1, (INT)strlen(pContents)-1, strIn, MAX_ITEM_STRING_LENGTH, nSize);

	memcpy(&idTable, strIn, sizeof(UINT));
	pItem = CObject_Item::NewItem(idTable);

	
	if(pItem)
	{
		pItem->SetExtraInfo( strIn );
		return TRUE;
	}

	pItem = 0;
	return FALSE;
}