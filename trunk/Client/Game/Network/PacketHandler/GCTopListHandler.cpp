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
	// �ȼ����а�
	case LEVEL_TOP_LIST:
		{
			// �Ȱ����а��������ԭ�����������ڷ�������Ϊ���ٷ�����ѹ�����ڷŵ��ͻ���
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

				// ����
				vParam.push_back(pList->m_LevelDataEx[i].m_UserData.m_Name);

				// ����+����+�ȼ�
				_snprintf(szTemp, 10, "%d+%d+%d", type, i, pList->m_LevelDataEx[i].m_UserData.m_Level);
				vParam.push_back(szTemp);

				// ����
				STRING SCountry = GetCountry( (COUNTRY_ATTRIBUTE)pList->m_LevelDataEx[i].m_UserData.m_Country );
				vParam.push_back( SCountry );

				// ����
				STRING MenPai = GetMenPai(pList->m_LevelDataEx[i].m_UserData.m_MenPai);
				vParam.push_back( MenPai );

				// ���
				vParam.push_back(pList->m_LevelDataEx[i].m_GuildName);

				// ��ǰ���������ڼ�����������
				CHAR PaiMing[5];
				_snprintf(PaiMing,5,"%d",pList->m_LevelDataEx[i].m_OldRank);
				vParam.push_back(PaiMing);

				CEventSystem::GetMe()->PushEvent( GE_UPDATE_TOP_LIST, vParam );	

			}
		}
		break;
	//��Ǯ���а�
	case MONEY_TOP_LIST:
		{
			//����
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

				// ����
				vParam.push_back(pList->m_MoneyDataEx[i].m_UserData.m_Name);
				
				// ����+����+��Ǯ
				_snprintf(szTemp, 20, "%d+%d+%d", type, i, pList->m_MoneyDataEx[i].m_Money);
				vParam.push_back(szTemp);

				// ����
				STRING SCountry = GetCountry( (COUNTRY_ATTRIBUTE)pList->m_MoneyDataEx[i].m_UserData.m_Country );
				vParam.push_back( SCountry );

				// ����
				STRING MenPai = GetMenPai(pList->m_MoneyDataEx[i].m_UserData.m_MenPai);
				vParam.push_back( MenPai );

				// ���
				vParam.push_back( pList->m_MoneyDataEx[i].m_GuildName );

				//��ǰ������
				CHAR PaiMing[5];
				_snprintf(PaiMing,5,"%d", pList->m_MoneyDataEx[i].m_OldRank);
				vParam.push_back(PaiMing);

				CEventSystem::GetMe()->PushEvent( GE_UPDATE_TOP_LIST, vParam );	
			}
		}
		break;
	//�������а�
	case REPUTE_TOP_LIST:
		{

		}
		break;
	//�������а�
	case PET_TOP_LIST:
		{

		}
		break;
	//�������а�
	case ECTYPE_TOP_LIST:
		{

		}
		break;
	//�������а�
	case ERRANTRY_TOP_LIST:
		{

		}
		break;
	//������а�
	case CRIME_TOP_LIST:
		{

		}
		break;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


//�������
STRING GetMenPai(MenPaiID_t type)
{
	switch( type )											 		
	{
	case MATTRIBUTE_SHAOLIN_GWS:
	case MATTRIBUTE_SHAOLIN_TLH:
		{
			return( "������" );
		}
		break;
	case MATTRIBUTE_GAIBANG_GGB:
	case MATTRIBUTE_GAIBANG_ZGB:
		{
			return( "ؤ��" );
		}
		break;
	case MATTRIBUTE_WUDANG_JWD:
	case MATTRIBUTE_WUDANG_QWD:
		{
			return( "�䵱��" );
		}
		break;
	case MATTRIBUTE_TANGMEN_DT:
	case MATTRIBUTE_TANGMEN_XJT:
		{
			return( "����" );
		}
		break;
	case MATTRIBUTE_XIAOYAO_XXY:
	case MATTRIBUTE_XIAOYAO_ZXY:
		{
			return( "��ң��" );
		}
		break;
	case MATTRIBUTE_YAOWANGGU_KYW:
	case MATTRIBUTE_YAOWANGGU_YYW:
		{
			return( "ҩ����" );
		}
		break;
	case MATTRIBUTE_TAOHUAGU_DTH:
	case MATTRIBUTE_TAOHUAGU_YTH:
		{
			return( "�һ���" );
		}
		break;
	case MATTRIBUTE_EMEI_CEM:
	case MATTRIBUTE_EMEI_FEM:
		{
			return( "������" );
		}
		break;
	case MATTRIBUTE_WUMENPAI: 
		{
			return( "��������" );
		}
		break;
	default:
		return("���ɴ���");
		break;
	}
}

//��ù���
STRING GetCountry( COUNTRY_ATTRIBUTE type )
{
	switch( type )
	{
	case COUATTRIBUTE_XINYUE:
		return("���º���");
		break;
	case COUATTRIBUTE_JINZHANG:
		return("���ʺ���");
		break;
	case COUATTRIBUTE_XIYU:
		return("���򺹹�");
		break;
	case COUATTRIBUTE_DAMO:
		return("��Į����");
		break;
	default:
		return("���Ҵ���");
		break;
	}
}