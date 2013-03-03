#include "StdAfx.h"
#include "GCTopList.h"
#include "Procedure/GameProcedure.h"
#include "Event/GMEventSystem.h"

STRING GetMenPai( MenPaiID_t type );
STRING GetCountry( COUNTRY_ATTRIBUTE type );

uint GCTopListHandler::Execute( GCTopList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	WORLD_TOP_LIST* pList = pPacket->GetTopList();
	TOP_LIST_TYPE type = (TOP_LIST_TYPE)pList->m_Type;
		
	switch ( type )
	{
	// 等级排行榜
	case LEVEL_TOP_LIST:
		{
			// 先把排行榜进行排序，原先排序工作放在服务器，为减少服务器压力现在放到客户端
			_LEVEL_TOP_DATA TopData[1];
			for ( int i=0; i<pList->m_DataCount; i++ )
			{
				for ( int j=i+1; j<pList->m_DataCount; j++ )
				{
					if ( pList->m_LevelDataEx[i].m_UserData.m_Level < pList->m_LevelDataEx[j].m_UserData.m_Level )
					{
						memcpy( TopData,pList->m_LevelDataEx+i,sizeof(_LEVEL_TOP_DATA));
						memcpy( pList->m_LevelDataEx+i, pList->m_LevelDataEx+j,sizeof(_LEVEL_TOP_DATA));
						memcpy( pList->m_LevelDataEx+j, TopData, sizeof(_LEVEL_TOP_DATA) );
					}
					else if ( pList->m_LevelDataEx[i].m_UserData.m_Level == pList->m_LevelDataEx[j].m_UserData.m_Level )
					{
						if ( pList->m_LevelDataEx[i].m_Exp < pList->m_LevelDataEx[j].m_Exp )
						{
							memcpy( TopData,pList->m_LevelDataEx+i,sizeof(_LEVEL_TOP_DATA));
							memcpy( pList->m_LevelDataEx+i, pList->m_LevelDataEx+j,sizeof(_LEVEL_TOP_DATA));
							memcpy( pList->m_LevelDataEx+j, TopData, sizeof(_LEVEL_TOP_DATA) );
						}
						else if (  pList->m_LevelDataEx[i].m_Exp == pList->m_LevelDataEx[j].m_Exp  )
						{
							if ( pList->m_LevelDataEx[i].m_OnlineTime > pList->m_LevelDataEx[j].m_OnlineTime )
							{
								memcpy( TopData,pList->m_LevelDataEx+i,sizeof(_LEVEL_TOP_DATA));
								memcpy( pList->m_LevelDataEx+i, pList->m_LevelDataEx+j,sizeof(_LEVEL_TOP_DATA));
								memcpy( pList->m_LevelDataEx+j, TopData, sizeof(_LEVEL_TOP_DATA) );
							}
						}
					}
				}
			}

			for ( int i=0; i< pList->m_DataCount; i++ )
			{	
				std::vector< STRING > vParam;
				CHAR szTemp[10];	

				vParam.push_back("1");

				// 名称
				vParam.push_back(pList->m_LevelDataEx[i].m_UserData.m_Name);

				// 类型+名次+等级
				_snprintf(szTemp, 10, "%d+%d+%d", type, i, pList->m_LevelDataEx[i].m_UserData.m_Level);
				vParam.push_back(szTemp);

				// 国家
				STRING SCountry = GetCountry( (COUNTRY_ATTRIBUTE)pList->m_LevelDataEx[i].m_UserData.m_Country );
				vParam.push_back( SCountry );

				// 门派
				STRING MenPai = GetMenPai(pList->m_LevelDataEx[i].m_UserData.m_MenPai);
				vParam.push_back( MenPai );

				// 帮会
				vParam.push_back(pList->m_LevelDataEx[i].m_GuildName);

				// 以前排名、用于计算排名浮动
				CHAR PaiMing[5];
				_snprintf(PaiMing,5,"%d",pList->m_LevelDataEx[i].m_OldRank);
				vParam.push_back(PaiMing);

				CEventSystem::GetMe()->PushEvent( GE_UPDATE_TOP_LIST, vParam );	

			}
		}
		break;
	//金钱排行榜
	case MONEY_TOP_LIST:
		{
			//排序
			_MONEY_TOP_DATA TopData[1];
			for ( INT i=0; i<pList->m_DataCount; ++i )
			{
				for ( INT j=i+1; j<pList->m_DataCount; ++j )
				{
					if ( pList->m_MoneyDataEx[i].m_Money < pList->m_MoneyDataEx[j].m_Money )
					{
						memcpy( TopData, pList->m_MoneyDataEx+i, sizeof(_MONEY_TOP_DATA) );
						memcpy( pList->m_MoneyDataEx+i, pList->m_MoneyDataEx+j, sizeof(_MONEY_TOP_DATA) );
						memcpy( pList->m_MoneyDataEx+j, TopData,sizeof(_MONEY_TOP_DATA) );
					}
					if ( pList->m_MoneyDataEx[i].m_Money == pList->m_MoneyDataEx[j].m_Money )
					{
						if (pList->m_MoneyDataEx[i].m_OnLineTime > pList->m_MoneyDataEx[j].m_OnLineTime)
						{
							memcpy( TopData, pList->m_MoneyDataEx+i, sizeof(_MONEY_TOP_DATA) );
							memcpy( pList->m_MoneyDataEx+i, pList->m_MoneyDataEx+j, sizeof(_MONEY_TOP_DATA) );
							memcpy( pList->m_MoneyDataEx+j, TopData,sizeof(_MONEY_TOP_DATA) );
						}
					}
				}
			}

			for ( int i=0; i< pList->m_DataCount; i++ )
			{
				std::vector< STRING > vParam;
				CHAR szTemp[20];	

				vParam.push_back("1");

				// 名称
				vParam.push_back(pList->m_MoneyDataEx[i].m_UserData.m_Name);
				
				// 类型+名次+金钱
				_snprintf(szTemp, 20, "%d+%d+%d", type, i, pList->m_MoneyDataEx[i].m_Money);
				vParam.push_back(szTemp);

				// 国家
				STRING SCountry = GetCountry( (COUNTRY_ATTRIBUTE)pList->m_MoneyDataEx[i].m_UserData.m_Country );
				vParam.push_back( SCountry );

				// 门派
				STRING MenPai = GetMenPai(pList->m_MoneyDataEx[i].m_UserData.m_MenPai);
				vParam.push_back( MenPai );

				// 帮会
				vParam.push_back( pList->m_MoneyDataEx[i].m_GuildName );

				//以前的排名
				CHAR PaiMing[5];
				_snprintf(PaiMing,5,"%d", pList->m_MoneyDataEx[i].m_OldRank);
				vParam.push_back(PaiMing);

				CEventSystem::GetMe()->PushEvent( GE_UPDATE_TOP_LIST, vParam );	
			}
		}
		break;
	//声望排行榜
	case REPUTE_TOP_LIST:
		{

		}
		break;
	//宠物排行榜
	case PET_TOP_LIST:
		{

		}
		break;
	//副本排行榜
	case ECTYPE_TOP_LIST:
		{

		}
		break;
	//侠义排行榜
	case ERRANTRY_TOP_LIST:
		{

		}
		break;
	//罪恶排行榜
	case CRIME_TOP_LIST:
		{

		}
		break;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


//获得门派
STRING GetMenPai(MenPaiID_t type)
{
	switch( type )											 		
	{
	case MATTRIBUTE_SHAOLIN_GWS:
	case MATTRIBUTE_SHAOLIN_TLH:
		{
			return( "少林派" );
		}
		break;
	case MATTRIBUTE_GAIBANG_GGB:
	case MATTRIBUTE_GAIBANG_ZGB:
		{
			return( "丐帮" );
		}
		break;
	case MATTRIBUTE_WUDANG_JWD:
	case MATTRIBUTE_WUDANG_QWD:
		{
			return( "武当派" );
		}
		break;
	case MATTRIBUTE_TANGMEN_DT:
	case MATTRIBUTE_TANGMEN_XJT:
		{
			return( "唐门" );
		}
		break;
	case MATTRIBUTE_XIAOYAO_XXY:
	case MATTRIBUTE_XIAOYAO_ZXY:
		{
			return( "逍遥派" );
		}
		break;
	case MATTRIBUTE_YAOWANGGU_KYW:
	case MATTRIBUTE_YAOWANGGU_YYW:
		{
			return( "药王谷" );
		}
		break;
	case MATTRIBUTE_TAOHUAGU_DTH:
	case MATTRIBUTE_TAOHUAGU_YTH:
		{
			return( "桃花岛" );
		}
		break;
	case MATTRIBUTE_EMEI_CEM:
	case MATTRIBUTE_EMEI_FEM:
		{
			return( "峨嵋派" );
		}
		break;
	case MATTRIBUTE_WUMENPAI: 
		{
			return( "无门无派" );
		}
		break;
	default:
		return("门派错误");
		break;
	}
}

//获得国家
STRING GetCountry( COUNTRY_ATTRIBUTE type )
{
	switch( type )
	{
	case COUATTRIBUTE_XINYUE:
		return("新月汗国");
		break;
	case COUATTRIBUTE_JINZHANG:
		return("金帐汗国");
		break;
	case COUATTRIBUTE_XIYU:
		return("西域汗国");
		break;
	case COUATTRIBUTE_DAMO:
		return("大漠汗国");
		break;
	default:
		return("国家错误");
		break;
	}
}