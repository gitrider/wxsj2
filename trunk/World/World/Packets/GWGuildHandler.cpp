/*
�����¼�����ӿ�
*/
#include "stdafx.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "GuildManager.h"
#include "Log.h"
#include "GWGuild.h"
#include "WGGuildReturn.h"
#include "WGGuildError.h"
#include "WGGuild.h"
#include "City.h"
#include "MailCenter.h"
#include "TimeManager.h"
#include "User.h"
#include "WorldTable.h"
#include "WGChat.h"
#include "ChatCenter.h"

uint	GWGuildHandler::Execute(GWGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GUID_t guid = pPacket->GetGUID();

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X not find!", 
			guid );
		return PACKET_EXE_CONTINUE;
	}

	GuildID_t guildID = pUser->GetGuildID();
	_GUILD_CGW_PACKET* pGuildPacket = pPacket->GetGuildPacket();
	GUILD_ERROR_TYPE GuildErr = GUILD_ERROR;

	switch( pGuildPacket->m_uPacketType )
	{
	case GUILD_PACKET_GW_ASKLIST:
		{ // ���ذ����Ϣ�б�
			GUILD_CGW_ASKLIST* pAskList = (GUILD_CGW_ASKLIST*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ASKLIST));

			WGGuild Msg;
			Msg.SetPlayerID( pUser->GetPlayerID() );
			Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_LIST;
			GUILD_WGC_LIST* pGuildList = (GUILD_WGC_LIST*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_LIST));
			Assert( pGuildList );

			if( g_pGuildManager->GetGuildList( pGuildList, pAskList->m_uStart ) )
			{
				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X get guild list!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}

			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Get Guild info failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_CREATE:
		{
			if( guildID != INVALID_ID )
			{ // �˴���һ������  ��������ᣬ���ǰ���û������������ĸ���һᴦ��Ԥ�����ڵ�״̬
              // MapServer�ϸ���� Guild_IDΪ-1��������GLServer�ϸ�����Ѿ�Guild_ID�Ѿ�����������İ���ID
              // ������Ϊ������а�������봴��������ͻ��������Σ����ﴦ���������
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "����δ����֤�ļ����������¼�����ڸ����뱻������ٳ��Դ�����ᡣ" );
                g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);

				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to create an extra guild!", 
					guid );
				GuildErr = GUILD_ERROR_GUILD_ALREADY_EXIST;

				break;
			}

			GUILD_CGW_CREATE* pGuildCreate = (GUILD_CGW_CREATE*)pGuildPacket->GetPacket(GUILD_PACKET_GW_CREATE);
			Assert( pGuildCreate );

			GuildErr = g_pGuildManager->CreateGuild(pUser, pGuildCreate->m_szGuildName);
			if( GuildErr == GUILD_ERROR_NOTHING )
			{ // �����ɹ�

				//ȫ�������а���е������б����һ�ߣ��Ǻǣ�����
				for( INT i=0; i<MAX_GUILD_SIZE; i++ )
				{
					Guild* pDeleteGuild = g_pGuildManager->GetGuild( i );
					if(pDeleteGuild->GetGuildID() != INVALID_ID && pDeleteGuild->GetGuildID() != pUser->GetGuildID() )
					{
						for(INT j = 0; j<USER_ARRAY_SIZE;j++)
						{
							GUILDUSER* pDeleteUser = pDeleteGuild->GetGuildUserByIndex(j);
							if(pDeleteUser->m_UserGUID != INVALID_ID)
							{
								if(pDeleteUser->m_UserGUID == guid
									&&pDeleteUser->m_Position == GUILD_POSITION_TRAINEE)
								{//ɾ������
									pDeleteGuild->OnUserLeaveByIndex(j);
									break;
								}
							}
						}
					}
				}

				//��ϲ�������AA��ᴴ���ɹ�����ʹ��ALT+O��ͨ���������Խ���ġ���ᡱ�鿴�͹�����İ�ᡣ
				CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
				sprintf(szMailContent, "��ϲ�������%s��ᴴ���ɹ�����ʹ��ALT+O��ͨ���������Խ���ġ���ᡱ�鿴�͹�����İ�ᡣ", pGuildCreate->m_szGuildName);
				g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);

				WGGuildReturn Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
				pGuild->SetGuildDesc(pGuildCreate->m_szGuildDesc);
				_GUILD_RETURN GuildReturn;
				GuildReturn.m_GuildID = pUser->GetGuildID();
				GuildReturn.m_ReturnType = GUILD_RETURN_CREATE;
				GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
				strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);

				Msg.SetGuildReturn( &GuildReturn );
				pServerPlayer->SendPacket( &Msg );

                // ����Ƶ��
                CHAR szContex[256] = {0} ;
                sprintf( szContex, "��%s�Ĵ����£�һ�����˵İ��%s�����ˣ���", pUser->GetName(),pGuild->GetGuildName() ) ;

                WGChat MsgR ;
                MsgR.SetPlayerID( pUser->GetPlayerID() ) ;
                MsgR.SetChatType( CHAT_TYPE_WORLD ) ;
                MsgR.SetContexSize( (BYTE)strlen(szContex) ) ;
                MsgR.SetContex( szContex ) ;
                MsgR.SetSourNameSize( (BYTE)(strlen(pUser->GetName())) ) ;
                MsgR.SetSourName( pUser->GetName() ) ;
                MsgR.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;
                g_pServerManager->BroadCastServer( &MsgR ) ;


				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X created a guild success!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}
			else
			{
				WGGuildReturn Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				_GUILD_RETURN GuildReturn;
				GuildReturn.m_GuildID = pUser->GetGuildID();
				GuildReturn.m_ReturnType = GUILD_RETURN_CREATE_FAILED;

				Msg.SetGuildReturn( &GuildReturn );
				pServerPlayer->SendPacket( &Msg );

			}

			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X created a guild failed!", 
				guid );
		}
		break;
    
    case GUILD_PACKET_GW_INVITE:
        {
            GUILD_CGW_INVITE* pGuildInvite = (GUILD_CGW_INVITE*)pGuildPacket->GetPacket(GUILD_PACKET_GW_INVITE);

            USER* pTargetUser = g_pOnlineUser->FindUser( pGuildInvite->m_szTargetName );

            if (!pTargetUser)
            {
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "����������%sĿǰ�����ߣ�����������ʱ�ٽ������롣", pGuildInvite->m_szTargetName);
                g_pMailCenter->SendNormalMail(pTargetUser->GetName(), szMailContent);
            }

            Assert(pTargetUser);
            Guild *pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);
            if (pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_RECRUIT))
            {
                //Todo:�������˲��ڰ��
                if (pTargetUser->GetGuildID() != INVALID_GUILD_ID)
                {
                    //֪ͨ���뷽  
                    CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                    sprintf(szMailContent, "�����������Ѿ������İ�ᡣ");
                    g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
                    return PACKET_EXE_CONTINUE;
                }

                if (pTargetUser->GetLevel() < 10)
                {
                    //֪ͨ���뷽  
                    CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                    sprintf(szMailContent, "���������ҵȼ����͡�");
                    g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
                    return PACKET_EXE_CONTINUE;
                }

                if (pTargetUser->BeInGuildInvite(g_pTimeManager->CurrentTime()))
                {
                    //֪ͨ���뷽  ����������æ����Ҫ�ȴ� -- Amos
                    CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                    sprintf(szMailContent, "����������%s��æ��һ����֮�������ԡ�", pTargetUser->GetName());
                    g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);

                    Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X send guild_invote to player(%s)��but (%s) is busy!", 
                        guid , pTargetUser->GetName(), pTargetUser->GetName());
                    return PACKET_EXE_CONTINUE;
                }

                //������֤��Ϣ
                USER_GUILD_DATA key;
                key.m_bInvit = TRUE;
                key.m_GuildId= pUser->GetGuildID();
                key.m_iTime  = g_pTimeManager->CurrentTime();
                pTargetUser->SetGuildInviteKey(key);

                //ת��������Ϣ
                ID_t targetServerID = g_Config.SceneID2ServerID(pTargetUser->GetSceneID());
                ServerPlayer *pTargetSPlayer = g_pServerManager->GetServerPlayer(targetServerID);

                WGGuild Msg;
                Msg.SetPlayerID( pTargetUser->GetPlayerID() );
                Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_CONFIRM_GUILD_INVITE;
                GUILD_WGC_INVITE* pInviteInfo       = (GUILD_WGC_INVITE*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_CONFIRM_GUILD_INVITE));
                pInviteInfo->m_GuildId              = pUser->GetGuildID();
                pInviteInfo->m_GuildNameSize        = (UCHAR)strlen(pGuild->GetGuildName());
                strncpy((CHAR*)pInviteInfo->m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
                pInviteInfo->m_InvitorNameSize      = (UCHAR)strlen(pUser->GetName());
                strncpy((CHAR*)pInviteInfo->m_InvitorName, pUser->GetName(), MAX_CHARACTER_NAME);

                pTargetSPlayer->SendPacket( &Msg );
                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X send guild_invote to player(%s)!", 
                    guid , pTargetUser->GetName());

                return PACKET_EXE_CONTINUE;
            }
            return PACKET_EXE_ERROR;
        }
        break;

    case GUILD_PACKET_GW_INVITE_CONFIRM:
        {
            GUILD_CGW_INVITE_CONFIRM* pGuildInvit = (GUILD_CGW_INVITE_CONFIRM*)pGuildPacket->GetPacket(GUILD_PACKET_GW_INVITE_CONFIRM);

            if (pGuildInvit->m_AgreeInvite)//�������ͬ��
            {
                //������֤��Ϣ
                if (!pUser->CompareGuildInviteKey(pGuildInvit->m_GuildId))
                {
                    Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X  guild_invite_confirm error. Guilid != KeyValue!", 
                        guid );
                    return PACKET_EXE_ERROR; ;
                }
                pUser->CleanUpGuildInviteKey();

                Guild* pGuild = g_pGuildManager->GetGuild( pGuildInvit->m_GuildId );
                if( pGuild == NULL )
                {
                    Assert(FALSE);
                    GuildErr = GUILD_ERROR_NOT_EXIST;
                }

                //�������
                if( INVALID_ID != pUser->GetGuildID() )
                { 
                    Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X invite to an extra guild!", 
                        guid );
                    GuildErr = GUILD_ERROR_GUILD_ALREADY_EXIST;

                    // ���ղ߻�SXJָʾ������ֻ��������
                    CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                    sprintf(szMailContent, "%s�Ѿ�����������ᣡ��", pUser->GetName());
                    g_pMailCenter->SendNormalMail(pGuild->GetGuildChiefName(), szMailContent);
                    break;
                }


                if (pGuild)
                {
                    switch( pGuild->GetGuildStatus() )
                    {
                    case GUILD_STATUS_NORMAL:
                        {
                            GuildErr = pGuild->OnUserInvite(pUser);
                            if( GuildErr == GUILD_ERROR_NOTHING )
                            { // �ɹ�
                                pUser->SetGuildID( pGuild->GetGuildID() );

                                // ֪ͨ�����˺ͺͱ������� �ɹ�������
                                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                                sprintf(szMailContent, "��ӭ%s���뱾����ͬ�۹���������%s��", pUser->GetName(), pGuild->GetGuildName());
                                g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);

                                memset(&szMailContent[0], 0, MAX_MAIL_CONTEX);
                                sprintf(szMailContent, "%sͬ��������룬���뱾�", pUser->GetName());
                                g_pMailCenter->SendNormalMail(&pGuildInvit->m_InvitorName[0], szMailContent);

                                // ����Ƶ��
                                CHAR szContex[256] = {0} ;
                                sprintf( szContex, "��ӭ%s���뱾����ͬ�۹���������%s��", pUser->GetName(),pGuild->GetGuildName() ) ;

                                WGChat MsgR ;
                                MsgR.SetPlayerID( pUser->GetPlayerID() ) ;
                                MsgR.SetChatType( CHAT_TYPE_GUILD ) ;
                                MsgR.SetContexSize( (BYTE)strlen(szContex) ) ;
                                MsgR.SetContex( szContex ) ;
                                MsgR.SetSourNameSize( (BYTE)(strlen(pUser->GetName())) ) ;
                                MsgR.SetSourName( pUser->GetName() ) ;
                                MsgR.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;
                                MsgR.SetGuildID(pGuild->GetGuildID());
                                g_pServerManager->BroadCastServer( &MsgR ) ;

                                // ��᷵����Ϣ
                                WGGuildReturn Msg;
                                Msg.SetPlayerID( pUser->GetPlayerID() );
                                _GUILD_RETURN GuildReturn;
                                GuildReturn.m_ReturnType        = GUILD_RETURN_INVIT;
                                GuildReturn.m_GUID              = pUser->GetGUID();
                                GuildReturn.m_GuildID           = pGuild->GetGuildID();
                                GuildReturn.m_GuildNameSize     = (UCHAR)strlen(pGuild->GetGuildName());
                                if (GuildReturn.m_GuildNameSize > 0)
                                {
							        strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
                                }
                                Msg.SetGuildReturn( &GuildReturn );
                                pServerPlayer->SendPacket( &Msg );

                                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X joined a guild!", 
                                    guid );
                                return PACKET_EXE_CONTINUE;
                            }
                        }
                        break;
                    default:
                        Assert(FALSE);
                    }
                }
                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X join a guild failed!", 
                    guid );
            }
            else
            {
                //֪ͨ���뷽  �������˾ܾ� 
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "���%s�ܾ���������롣", pUser->GetName());
                g_pMailCenter->SendNormalMail(&pGuildInvit->m_InvitorName[0], szMailContent);

                pUser->CleanUpGuildInviteKey();
                return PACKET_EXE_CONTINUE;
            }
        }
        break;

	case GUILD_PACKET_GW_JOIN:
		{
			GUILD_CGW_JOIN* pGuildJoin = (GUILD_CGW_JOIN*)pGuildPacket->GetPacket(GUILD_PACKET_GW_JOIN);

            if (INVALID_GUID == pGuildJoin->m_TargetGuid)//�������
            {
                if( guildID != INVALID_ID )
                { // ���а�ᣬ����Ӧ���Ѿ��� Server ��ȷ�������Բ����ش�����
                    Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to join an extra guild!", 
                        guid );
                    GuildErr = GUILD_ERROR_GUILD_ALREADY_EXIST;
                    break;
                }
            }

			Guild* pGuild = g_pGuildManager->GetGuild( pGuildJoin->m_GuildGUID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
			}
			else
			{
				switch( pGuild->GetGuildStatus() )
				{
				case GUILD_STATUS_NORMAL:
					{
						GuildErr = pGuild->OnUserEnter(pUser);
						if( GuildErr == GUILD_ERROR_NOTHING )
						{ // �ɹ�
							pUser->SetGuildID( pGuild->GetGuildID() );

							WGGuildReturn Msg;
							Msg.SetPlayerID( pUser->GetPlayerID() );
							_GUILD_RETURN GuildReturn;
							GuildReturn.m_ReturnType = GUILD_RETURN_JOIN;
							GuildReturn.m_GuildID = pUser->GetGuildID();
							GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
							strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
							Msg.SetGuildReturn( &GuildReturn );

							pServerPlayer->SendPacket( &Msg );
							Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X joined a guild!", 
								guid );
							return PACKET_EXE_CONTINUE;
						}
					}
					break;
				default:
					Assert(FALSE);
				}
			}
			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X join a guild failed!", 
				guid );
		}
		break;

    case GUILD_PACKET_GW_MODIFY_TITLE_NAME:
        {
            //�޸���Ұ��ƺ�����
            GUILD_CGW_MODIFY_TITLE_NAME* pTitleName = (GUILD_CGW_MODIFY_TITLE_NAME*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_MODIFY_TITLE_NAME));
            Assert( pTitleName );

            WGGuild Msg;
            Msg.SetPlayerID( pUser->GetPlayerID() );
            Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_MODIFY_TITLE_NAME;
            GUILD_WGC_MODIFY_TITLE_NAME* pName  = (GUILD_WGC_MODIFY_TITLE_NAME*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_MODIFY_TITLE_NAME));
            pName->m_Guid                       = guid;
            pName->m_uTitleNameSize             = pTitleName->m_NameSize;
            memcpy(pName->m_szTitleName, pTitleName->m_szTitleName, MAX_CHARACTER_TITLE);

            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            if (pGuild)
            {
                pGuild->SetGuildUserTitleName(guid, pTitleName->m_szTitleName);
            }

            //�����Լ�
            pServerPlayer->SendPacket( &Msg );
            Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X modify Guid Title NAme!", guid );

            //��������
            g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);
            USER* pGuildMaster = g_pOnlineUser->FindUser(pGuild->GetChieftainGUID());
            if (pGuildMaster)
            {
                ID_t GuildMasterServerID = g_Config.SceneID2ServerID(pGuildMaster->GetSceneID());
                ServerPlayer *pSPlayer = g_pServerManager->GetServerPlayer(GuildMasterServerID);

                if (pSPlayer)
                {
                    Msg.SetPlayerID(pGuildMaster->GetPlayerID());
                    pServerPlayer->SendPacket( &Msg );
                    Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X modify Guid Title NAme!", 
                        pGuildMaster->GetGUID() );
                }
            }
            return PACKET_EXE_CONTINUE;
        }
        break;

    case GUILD_PACKET_GW_FAMILY_DISMISS:
        {
            GUILD_CGW_FAMILY_DISMISS *pDismiss = (GUILD_CGW_FAMILY_DISMISS*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_FAMILY_DISMISS));  

            Guild* pGuild = g_pGuildManager->GetGuild(pDismiss->m_GuildGUID);
            if(pGuild)
            {
                if (pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_CHIEFTAIN))
                {//����
                    if (FAMILYUSER::RET_SUCCESS == pGuild->OnFamilyDismiss(pDismiss->m_nFamilyID))
                    {
                        WGGuildReturn Msg;
                        Msg.SetPlayerID( pUser->GetPlayerID() );
                        _GUILD_RETURN GuildReturn;
                        GuildReturn.m_ReturnType        = GUILD_RETURN_FAMILY_DISMISS;
                        GuildReturn.m_cFamilyID         = pDismiss->m_nFamilyID;
                        Msg.SetGuildReturn( &GuildReturn );

                        pServerPlayer->SendPacket( &Msg );
                        Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Dismiss a GuildFamily!", guid );
                        return PACKET_EXE_CONTINUE;
                    }
                    else
                        GuildErr = GUILD_ERROR_FAMILY_DIMISS;
                }
            }
        }
        break;

	case GUILD_PACKET_GW_ASKINFO:
		{
			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ASKINFO));
			Guild* pGuild = g_pGuildManager->GetGuild(pAskInfo->m_GuildGUID);
			// Assert(pGuild);
			if(!pGuild)
			{
				/*Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid guild!", 
					pUser->GetName() );*/   
				return PACKET_EXE_CONTINUE;
            }
            const GUILDUSER*	pGuildUser = pGuild->GetGuildUser(guid);
			Assert(pGuildUser);
			if(!pGuildUser)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid User!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}

			//���������������ɲ�ͬ�İ�
			if(pAskInfo->m_Type == GUILD_CGW_ASKINFO::GUILD_SELF_INFO)
			{//���˰�����Ϣ,�����ʼ��ʱ����
				WGGuild Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );

				if(!pGuild->CanDoIt(Guild::OPT_ASK_SELF_INFO, pGuildUser->m_Position, (GUILD_AUTHORITY)pGuildUser->m_uAccess))
				{
					Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Can Not Get Self Guild Info!", 
						pUser->GetName() );
					return PACKET_EXE_CONTINUE;
				}

				Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_SELF_GUILD_INFO;
				GUILD_WGC_SELF_GUILD_INFO* pSelfGuildInfo = (GUILD_WGC_SELF_GUILD_INFO*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_SELF_GUILD_INFO));

				//��ǰֻ��Ҫ������
				memcpy(pSelfGuildInfo->m_GuildName, pGuild->GetGuildName(),MAX_GUILD_NAME_SIZE );
				pSelfGuildInfo->m_GuildName[MAX_GUILD_NAME_SIZE - 1] = 0;
				pSelfGuildInfo->m_GuildNameSize = (BYTE)strlen(pSelfGuildInfo->m_GuildName);

				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s GUILD_PACKET_WG_SELF_GUILD_INFO!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}
			else if(pAskInfo->m_Type == GUILD_CGW_ASKINFO::GUILD_MEMBER_INFO)
			{//��Ա�б�
				WGGuild Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );

				if(!pGuild->CanDoIt(Guild::OPT_ASK_MEMBER_LIST, pGuildUser->m_Position, (GUILD_AUTHORITY)pGuildUser->m_uAccess))
				{
					Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Can Not Get Member Info!", 
						pUser->GetName() );
					return PACKET_EXE_CONTINUE;
				}
				Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_MEMBER_LIST;
				GUILD_WGC_MEMBER_LIST* pMemberList = (GUILD_WGC_MEMBER_LIST*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_MEMBER_LIST));

				memcpy(pMemberList->m_GuildDesc, pGuild->GetGuildDesc(),MAX_GUILD_DESC_SIZE );
				memcpy(pMemberList->m_GuildName, pGuild->GetGuildName(),MAX_GUILD_NAME_SIZE );
	
				pMemberList->m_uMemberCount =	0;
				pMemberList->m_uValidMemberCount = 0;
				pMemberList->m_uMemberMax	=	pGuild->GetGuildUserCount();
				pMemberList->m_bAccess		=	pGuildUser->m_uAccess;
				pMemberList->m_bPosition	=	pGuildUser->m_Position;

				INT CurNum = 0;
				for(INT i = 0; i<USER_ARRAY_SIZE; i++)
				{
					if(pGuild->GetGuildUserByIndex(i)->m_UserGUID != INVALID_ID)
					{
						const GUILDUSER* pUser = pGuild->GetGuildUserByIndex(i);
						pMemberList->m_GuildMemberData[CurNum].m_Guid			=   pUser->m_UserGUID;
						memcpy(pMemberList->m_GuildMemberData[CurNum].m_szName, pUser->m_szUserName, MAX_CHARACTER_NAME);
						pMemberList->m_GuildMemberData[CurNum].m_bLevel			=	pUser->m_uLevel;
						pMemberList->m_GuildMemberData[CurNum].m_bMenPaiID		=	pUser->m_bMenPaiID;
						pMemberList->m_GuildMemberData[CurNum].m_iCurContribute	=	pUser->m_iCurContribute;
						pMemberList->m_GuildMemberData[CurNum].m_iMaxContribute	=	pUser->m_iMaxContribute;
						pMemberList->m_GuildMemberData[CurNum].m_iJoinTime		=	pUser->m_iJoinTime;
						pMemberList->m_GuildMemberData[CurNum].m_iLogOutTime	=	pUser->m_uLastLoginTime;
						pMemberList->m_GuildMemberData[CurNum].m_bIsOnline		=	pUser->m_bOnlineFlag;
                        pMemberList->m_GuildMemberData[CurNum].m_iFamilyID      =   pUser->m_iFamilyID;
                        pMemberList->m_GuildMemberData[CurNum].m_iSex           =   pUser->m_cSex;
                        memcpy(pMemberList->m_GuildMemberData[CurNum].m_szTitleName, pUser->m_szTitleName, MAX_CHARACTER_TITLE);

						//�������
						if(pUser->m_bOnlineFlag && pUser->m_Position>GUILD_POSITION_TRAINEE)
						{
							pMemberList->m_uValidMemberCount++;							
						}
						pMemberList->m_GuildMemberData[CurNum].m_bPosition		=	(BYTE)pUser->m_Position;
						pMemberList->m_uMemberCount++;
						CurNum++;
					}
				}
                
                pMemberList->m_uFamilyCount = pGuild->GetFamilyCount();
                for (INT i=0; i<pMemberList->m_uFamilyCount ; ++i)
                {
                    pMemberList->m_GuildFamilyData[i].m_iFamilyID = i;
                    memcpy(pMemberList->m_GuildFamilyData[i].m_szFamilyName, pGuild->GetFamilyName(i), 
                           MAX_GUILD_FAMILY_NAME_SIZE);
                    memcpy(pMemberList->m_GuildFamilyData[i].m_szFamilyDesc, pGuild->GetFamilyDesc(i), 
                           MAX_GUILD_FAMILY_DESC_SIZE);
                    memcpy(pMemberList->m_GuildFamilyData[i].m_szShaikhName, pGuild->GetShaikhName(i), 
                        MAX_CHARACTER_NAME);

                    pMemberList->m_GuildFamilyData[i].m_iFamilyUserNum = pGuild->GetFamilyUser(i, 
                                                      &pMemberList->m_GuildFamilyData[i].m_aFamilyUser[0]);
                }

				Assert(CurNum == pMemberList->m_uMemberCount);
				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s GUILD_PACKET_WG_MEMBER_LIST!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}
			else if(pAskInfo->m_Type == GUILD_CGW_ASKINFO::GUILD_INFO)
			{//�����Ϣ
                WGGuild Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );

				if(!pGuild->CanDoIt(Guild::OPT_ASK_DETAIL_INFO, pGuildUser->m_Position, (GUILD_AUTHORITY)pGuildUser->m_uAccess))
				{
					Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Can Not Get Guild Info!", 
						pUser->GetName() );
					return PACKET_EXE_CONTINUE;
				}

                Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_GUILD_INFO;
                GUILD_WGC_GUILD_INFO* pGuildInfo = (GUILD_WGC_GUILD_INFO*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_GUILD_INFO));

				memcpy(pGuildInfo->m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
				memcpy(pGuildInfo->m_GuildCreator, pGuild->GetCreatorName(), MAX_CHARACTER_NAME);
				memcpy(pGuildInfo->m_GuildChairMan, pGuild->GetGuildChiefName(), MAX_CHARACTER_NAME);
				if(pGuild->GetCity())
				{
					strncpy(pGuildInfo->m_CityName, pGuild->GetCity()->GetCityName(), MAX_CITY_NAME_SIZE);
					pGuildInfo->m_nPortSceneID	=	pGuild->GetCity()->GetPortSceneID();
				}
				else
				{
					memset(pGuildInfo->m_CityName, 0, MAX_CITY_NAME_SIZE);
					pGuildInfo->m_nPortSceneID = -1;
				}
				pGuildInfo->m_nLevel		=	pGuild->GetGuildLevel();
				pGuildInfo->m_MemNum		=	pGuild->GetGuildUserCount();
				pGuildInfo->m_Longevity		=	pGuild->GetLongevity();
				pGuildInfo->m_Contribute	=	pGuild->GetTotalGuildPoint();	//���׶�
				pGuildInfo->m_Honor			=	pGuild->GetHonor();				//����
				pGuildInfo->m_FoundedMoney	=	pGuild->GetMoney();				//�����ʽ�

				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s GUILD_PACKET_WG_GUILD_INFO!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}
			else if(pAskInfo->m_Type == GUILD_CGW_ASKINFO::GUILD_APPOINT_POS)
			{
				WGGuild Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );

				if(!pGuild->CanDoIt(Guild::OPT_ASK_APPOINT_POS_INFO, pGuildUser->m_Position, (GUILD_AUTHORITY)pGuildUser->m_uAccess))
				{
					Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Can Not Get Appoint Info!", 
						pUser->GetName() );
					return PACKET_EXE_CONTINUE;
				}

				Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_APPOINT_INFO;
				GUILD_WGC_APPOINT_INFO* pAppointInfo = (GUILD_WGC_APPOINT_INFO*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_APPOINT_INFO));

				INT iPosNum = 0;
				for(INT i = GUILD_POSITION_MEMBER; i<=GUILD_POSITION_ASS_CHIEFTAIN; i++ )
				{//ְλ��ȫ������
					pAppointInfo->m_PosList[iPosNum].m_PosID = i;
					if(pGuild->GetPosName(i))
					{
						memcpy(pAppointInfo->m_PosList[iPosNum].m_PosName, pGuild->GetPosName(i), MAX_GUILD_POS_NAME_SIZE);
						iPosNum++;
					}
				}
				pAppointInfo->iPosNum = iPosNum;
				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s GUILD_APPOINT_POS!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
    case GUILD_PACKET_GW_CREATE_FAMILY:
        {
            GUILD_CGW_CREATE_FAMILY* pFamilyInfo = (GUILD_CGW_CREATE_FAMILY*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_CREATE_FAMILY));
            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);

            CHAR cFamilyID;
            pGuild->OnCreateFamily(pUser->GetGUID(), pFamilyInfo->m_ShaikhIndex, pFamilyInfo->m_ShaikhGuid,
                        pFamilyInfo->m_szFamilyName, pFamilyInfo->m_cNameSize, pFamilyInfo->m_szFamilyDesc,
                        pFamilyInfo->m_cDescSize, cFamilyID);

            WGGuildReturn Msg;
            Msg.SetPlayerID( pUser->GetPlayerID() );
            _GUILD_RETURN GuildReturn;
            GuildReturn.m_ReturnType        = GUILD_RETURN_CREATE_FAMILY;
            GuildReturn.m_cFamilyID         = cFamilyID;
            GuildReturn.m_FamilyNameSize    = (UCHAR)strlen(pFamilyInfo->m_szFamilyName);
            strncpy((CHAR*)&GuildReturn.m_FamilyName, pFamilyInfo->m_szFamilyName, MAX_GUILD_FAMILY_NAME_SIZE);
            GuildReturn.m_FamilyDescSize    = (UCHAR)strlen(pFamilyInfo->m_szFamilyDesc);
            strncpy((CHAR*)&GuildReturn.m_FamilyDesc, pFamilyInfo->m_szFamilyDesc, MAX_GUILD_FAMILY_DESC_SIZE);
            Msg.SetGuildReturn( &GuildReturn );

            pServerPlayer->SendPacket( &Msg );
            Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Create a GuildFamily!", guid );
            return PACKET_EXE_CONTINUE;
        }
        break;

    case GUILD_PACKET_GW_LEVELUP:
        {
            GUILD_CGW_LEVELUP* pLevel = (GUILD_CGW_LEVELUP*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_LEVELUP));
            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);

            BOOL bScuess = FALSE;
            if (pLevel->m_ChiefGUID  == pUser->GetGUID() && pLevel->m_GuildGUID ==  pUser->GetGuildID() &&
                pLevel->m_GuildLevel == pGuild->GetGuildLevel())
            {
                //�Ƚϰﹱ
                   INT nPoint = pGuild->GetGuildPoint(pUser->GetGUID());
                if (nPoint <= pLevel->m_GP)
                {
                    nPoint -= pLevel->m_GP;
                    pGuild->SetGuildPoint(pUser->GetGUID(), nPoint);
                    pGuild->SetGuildLevel(pGuild->GetGuildLevel()+1);
                    bScuess = TRUE;                
                }

                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X LevelUp!", guid );
            }
            else
            {
                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X GuildLevelUp Error!", guid );
            }

            WGGuildReturn Msg;
            Msg.SetPlayerID( pUser->GetPlayerID() );
            _GUILD_RETURN GuildReturn;
            GuildReturn.m_ReturnType        = GUILD_RETURN_LEVEL_UP;
            GuildReturn.m_GuildLevel        = pGuild->GetGuildLevel();
            GuildReturn.m_bScuess           = bScuess;
            Msg.SetGuildReturn( &GuildReturn );

            pServerPlayer->SendPacket( &Msg );
            
            return PACKET_EXE_CONTINUE;         
        }
        break;

    case GUILD_PACKET_GW_SUB_GP:
        {
            GUILD_CGW_GP *pGP = (GUILD_CGW_GP*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ADD_GP));

            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);

            INT nGp = pGuild->GetTotalGuildPoint();
            nGp -= pGP->m_GP;
            if (nGp < 0)
                nGp = 0;
            
            pGuild->SetTotalGuildPoint(nGp);
        }
        break;

    case GUILD_PACKET_GW_LEVEL_INFO:
        {
            GUILD_CGW_LEVELINFO *pInfo = (GUILD_CGW_LEVELINFO*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_LEVEL_INFO));

            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);

            INT nMaxUser    = 0, nMaxFamily = 0, nMaxGoldFUser = 0;
            BYTE cLevel     = pGuild->GetGuildLevel();
            Assert(cLevel >= 0 && cLevel < MAX_GUILD_LEVEL);

            nMaxUser        = g_GuildLevelInfoTbl.m_TableInfo[cLevel].m_MaxGuildUser;
            nMaxFamily      = g_GuildLevelInfoTbl.m_TableInfo[cLevel].m_MaxFamily;
            nMaxGoldFUser   = g_GuildLevelInfoTbl.m_TableInfo[cLevel].m_MaxGoldFamilyUser;

            pGuild->SetGuildMaxByLevel(nMaxUser, nMaxFamily, nMaxGoldFUser);
        }
        break;

    case GUILD_PACKET_GW_ADD_GP:
        {
            GUILD_CGW_GP *pGP = (GUILD_CGW_GP*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ADD_GP));

            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);

            //���°��ڵ�GP
            INT nGp = pGuild->GetGuildPoint(pUser->GetGUID());
            pGuild->SetGuildPoint(pUser->GetGUID(), nGp+pGP->m_GP);
            //ͬʱ���Ӱ���
            nGp = pGuild->GetTotalGuildPoint();
            pGuild->SetTotalGuildPoint(nGp + pGP->m_GP);
        }
        break;

    case GUILD_PACKET_GW_ADD_FAMILY_USER:
        {
            GUILD_CGW_ADD_FAMILY_USER* pAddInfo = (GUILD_CGW_ADD_FAMILY_USER*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ADD_FAMILY_USER));
            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);

            INT ret = pGuild->OnAddFamilyUser(pUser->GetGUID(), pAddInfo->m_cFamilyIndex,
                                              pAddInfo->m_iGuildIndex, pAddInfo->m_TargetGuid);

            if (FAMILYUSER::RET_SUCCESS == ret)
            {
                USER *pTargetUser = g_pOnlineUser->FindUser(pAddInfo->m_TargetGuid);
                Assert(pTargetUser);

                //֪ͨADD���� ������
                WGGuildReturn Msg;
                Msg.SetPlayerID( pUser->GetPlayerID() );
                _GUILD_RETURN GuildReturn;
                GuildReturn.m_ReturnType     = GUILD_RETURN_ADD_FAMILY_USER;
                GuildReturn.m_cFamilyID      = pAddInfo->m_cFamilyIndex;
                GuildReturn.m_SourNameSize   = (UCHAR)strlen(pTargetUser->GetName());
                strncpy((CHAR*)&GuildReturn.m_SourName, pTargetUser->GetName(), MAX_CHARACTER_NAME);
                Msg.SetGuildReturn( &GuildReturn );
                pServerPlayer->SendPacket( &Msg );

                //֪ͨ��������
                ID_t targetServerID = g_Config.SceneID2ServerID(pTargetUser->GetSceneID());
                ServerPlayer *pTargetSPlayer = g_pServerManager->GetServerPlayer(targetServerID);

                WGGuildReturn MsgOther;
                MsgOther.SetPlayerID( pUser->GetPlayerID() );
                _GUILD_RETURN GuildReturnOther;
                GuildReturnOther.m_ReturnType     = GUILD_RETURN_BEADD_FAMILY;
                GuildReturnOther.m_cFamilyID      = pAddInfo->m_cFamilyIndex;
                MsgOther.SetGuildReturn( &GuildReturnOther );
                pTargetSPlayer->SendPacket( &MsgOther );

                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "���Ѿ�������%s���塣", pGuild->GetFamilyName(pAddInfo->m_cFamilyIndex));
                g_pMailCenter->SendNormalMail(pTargetUser->GetName(), szMailContent);

                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Add a GuildFamilyUser!", guid );
            }
            else if (FAMILYUSER::RET_POS_FULL == ret)
            {
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "%s���������������޷���ɲ���", pGuild->GetFamilyName(pAddInfo->m_cFamilyIndex));
                g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
            }
            return PACKET_EXE_CONTINUE;
        }
        break;

    case GUILD_PACKET_GW_MODIFY_DESC:
        {
            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);

            if (pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_CHIEFTAIN))
            {
                GUILD_CGW_MODIFY_DESC *pDesc = (GUILD_CGW_MODIFY_DESC*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_MODIFY_DESC));
                if (pDesc->m_DescSize > 0)
                {
                    pGuild->SetGuildDesc(pDesc->m_szGuildDesc);
                }

                WGGuildReturn Msg;
                Msg.SetPlayerID( pUser->GetPlayerID() );
                _GUILD_RETURN GuildReturn;
                GuildReturn.m_ReturnType     = GUILD_RETURN_MODIFY_DESC;
                GuildReturn.m_SourNameSize   = (UCHAR)strlen(pDesc->m_szGuildDesc);
                strncpy((CHAR*)&GuildReturn.m_SourName, pDesc->m_szGuildDesc, GuildReturn.m_SourNameSize);

                Msg.SetGuildReturn( &GuildReturn );
                pServerPlayer->SendPacket( &Msg );
            }

        }
        break;

    case GUILD_PACKET_GW_REMOVE_FAMILY_USER:
        {
            GUILD_CGW_REMOVE_FAMILY_USER* pRemoveInfo = (GUILD_CGW_REMOVE_FAMILY_USER*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_REMOVE_FAMILY_USER));
            Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
            Assert(pGuild);

            INT ret = pGuild->OnRemoveFamilyUser(pUser->GetGUID(), pRemoveInfo->m_cFamilyIndex,
                                                 pRemoveInfo->m_iGuildIndex, pRemoveInfo->m_TargetGuid);

            if (FAMILYUSER::RET_SUCCESS == ret)
            {
                USER *pTargetUser = g_pOnlineUser->FindUser(pRemoveInfo->m_TargetGuid);
                Assert(pTargetUser);

                //֪ͨDEL���� ������
                WGGuildReturn Msg;
                Msg.SetPlayerID( pUser->GetPlayerID() );
                _GUILD_RETURN GuildReturn;
                GuildReturn.m_ReturnType = GUILD_RETURN_REMOVE_FAMILY_USER;
                GuildReturn.m_cFamilyID  = pRemoveInfo->m_cFamilyIndex;
                GuildReturn.m_SourNameSize   = (UCHAR)strlen(pTargetUser->GetName());
                strncpy((CHAR*)&GuildReturn.m_SourName, pTargetUser->GetName(), MAX_CHARACTER_NAME);

                Msg.SetGuildReturn( &GuildReturn );
                pServerPlayer->SendPacket( &Msg );

                //Todo:֪ͨ��������
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "���Ѿ�����������%s���塣", pGuild->GetFamilyName(pRemoveInfo->m_cFamilyIndex));
                g_pMailCenter->SendNormalMail(pTargetUser->GetName(), szMailContent);

                Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Del a GuildFamilyUser!", guid );
                return PACKET_EXE_CONTINUE;
            }
        }
        break;

	case GUILD_PACKET_GW_APPOINT:
		{
			GUILD_CGW_APPOINT* pAppointInfo = (GUILD_CGW_APPOINT*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_APPOINT));
			Guild* pGuild = g_pGuildManager->GetGuild(pAppointInfo->m_GuildGUID);
			Assert(pGuild);
			if(!pGuild)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid guild!", 
					pUser->GetName() );
				break;
			}

			const GUILDUSER*	pGuildUser = pGuild->GetGuildUser(guid);
			Assert(pGuildUser);
			if(!pGuildUser)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid User!", 
					pUser->GetName() );
				break;
			}
	
			const GUILDUSER*	pCandGuildUser = pGuild->GetGuildUser(pAppointInfo->m_CandidateGUID);
			Assert(pCandGuildUser);
			if(!pCandGuildUser)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid User!", 
					pUser->GetName() );
				break;
			}

			if( pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_ASSIGN) == FALSE )
			{
				GuildErr = GUILD_ERROR_UNAUTHORIZED;
			}

			//��������Լ�
			if(pGuildUser == pCandGuildUser)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s appoint himself!", 
					pUser->GetName() );
				break;
			}

			GUILD_POSITION OldPos = pCandGuildUser->m_Position;
			
			if(OldPos == pAppointInfo->m_NewPosition)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s No Change!", 
					pUser->GetName() );
				break;
			}

			GUILD_ADMIN_t::ORESULT bSuccess = pGuild->OnUserPositionChange(pAppointInfo->m_CandidateGUID, (GUILD_POSITION)pAppointInfo->m_NewPosition);
			if(bSuccess == GUILD_ADMIN_t::RET_SUCCESS)
			{
				WGGuildReturn Msg;
				_GUILD_RETURN GuildReturn;
				if(OldPos>pAppointInfo->m_NewPosition)
				{//��ְ
					GuildReturn.m_ReturnType = GUILD_RETURN_DEMOTE;
					GuildReturn.m_GuildID	=	pAppointInfo->m_GuildGUID;
					GuildReturn.m_GUID		=	pAppointInfo->m_CandidateGUID;
					GuildReturn.m_PosID		=	pAppointInfo->m_NewPosition;
					
					GuildReturn.m_SourNameSize = (UCHAR)strlen(pUser->GetName());
					strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), MAX_CHARACTER_NAME);
					
					GuildReturn.m_DestNameSize = (UCHAR)strlen(pCandGuildUser->m_szUserName);
					strncpy((CHAR*)GuildReturn.m_DestName, pCandGuildUser->m_szUserName, GuildReturn.m_DestNameSize);
					
					GuildReturn.m_PositionNameSize = MAX_GUILD_POS_NAME_SIZE;
					strncpy((CHAR*)GuildReturn.m_PositionName, pGuild->GetPosName(pAppointInfo->m_NewPosition), GuildReturn.m_PositionNameSize);
					
					GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
					strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
					
					Msg.SetGuildReturn( &GuildReturn );

					//֪ͨ�Լ�
					Msg.SetPlayerID( pUser->GetPlayerID() );
					pServerPlayer->SendPacket( &Msg );

					//֪ͨ����������
					USER* pNewUser = g_pOnlineUser->FindUser( pAppointInfo->m_CandidateGUID );
					if(pNewUser)
					{
						ServerPlayer* pTmpServerPlayer = NULL;
						pTmpServerPlayer = g_pServerManager->GetServerPlayer(pNewUser->GetServerID());
						if( pTmpServerPlayer != NULL )
						{
							Msg.SetPlayerID( pNewUser->GetPlayerID() );
							pTmpServerPlayer->SendPacket( &Msg );
						}
					}

				}
				else
				{//��ְ
					GuildReturn.m_ReturnType = GUILD_RETURN_PROMOTE;
					GuildReturn.m_GuildID	=	pAppointInfo->m_GuildGUID;
					GuildReturn.m_GUID		=	pAppointInfo->m_CandidateGUID;
					GuildReturn.m_PosID		=	pAppointInfo->m_NewPosition;
					
					GuildReturn.m_SourNameSize = (UCHAR)strlen(pUser->GetName());
					strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), MAX_CHARACTER_NAME);
					
					GuildReturn.m_DestNameSize = (UCHAR)strlen(pCandGuildUser->m_szUserName);
					strncpy((CHAR*)GuildReturn.m_DestName, pCandGuildUser->m_szUserName, GuildReturn.m_DestNameSize);
					
					GuildReturn.m_PositionNameSize = MAX_GUILD_POS_NAME_SIZE;
					strncpy((CHAR*)GuildReturn.m_PositionName, pGuild->GetPosName(pAppointInfo->m_NewPosition), GuildReturn.m_PositionNameSize);
					
					GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
					strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
					
					Msg.SetGuildReturn( &GuildReturn );

					//֪ͨ�Լ�
					Msg.SetPlayerID( pUser->GetPlayerID() );
					pServerPlayer->SendPacket( &Msg );

					//֪ͨ����������
					USER* pNewUser = g_pOnlineUser->FindUser( pAppointInfo->m_CandidateGUID );
					if(pNewUser)
					{
						ServerPlayer* pTmpServerPlayer = NULL;
						pTmpServerPlayer = g_pServerManager->GetServerPlayer(pNewUser->GetServerID());
						if( pTmpServerPlayer != NULL )
						{
							Msg.SetPlayerID( pNewUser->GetPlayerID() );
							pTmpServerPlayer->SendPacket( &Msg );
						}
					}
				}
				
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X recruited a guild!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}
			else if(bSuccess == GUILD_ADMIN_t::RET_NOT_EXIST)
			{
//				GuildErr = GUILD_ERROR_POS_FULL;
			}
			else if(bSuccess == GUILD_ADMIN_t::RET_POS_FULL)
			{
				GuildErr = GUILD_ERROR_POS_FULL;
			}
			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X join a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_ADJUSTAUTHORITY:
		{
		}
		break;
	case GUILD_PACKET_GW_RECRUIT:
		{
			if( guildID == INVALID_ID )
			{ // ���а�ᣬ����Ӧ���Ѿ��� Server ��ȷ�������Բ����ش�����
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to recruit a user!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
			}
			else
			{
				switch( pGuild->GetGuildStatus() )
				{
				case GUILD_STATUS_NORMAL:
					{
						if( pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_RECRUIT) == FALSE ||
                            pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_ASSCHIEFTAIN) == FALSE)
						{
							GuildErr = GUILD_ERROR_UNAUTHORIZED;
						}
						else
						{
							GUILD_CGW_RECRUIT* pGuildRecruit;
							pGuildRecruit = (GUILD_CGW_RECRUIT*)pGuildPacket->GetPacket(GUILD_PACKET_GW_RECRUIT);
							Assert( pGuildRecruit );

							const GUILDUSER* pGuildUser = pGuild->GetGuildUser( pGuildRecruit->m_ProposerGUID );
							if( pGuildUser == NULL )
							{
								GuildErr = GUILD_ERROR;

                                WGGuildReturn Msg;
                                _GUILD_RETURN GuildReturn;
                                GuildReturn.m_ReturnType = GUILD_RETURN_ALREADY_IN;
                                Msg.SetPlayerID(pUser->GetPlayerID());
                                Msg.SetGuildReturn( &GuildReturn );
                
                                pServerPlayer->SendPacket(&Msg);
							}
							else if( pGuildUser->m_Position != GUILD_POSITION_TRAINEE )
							{ // ��һ����ʽ���ڽ��д˲�����Ҳ����һ����ǡ�ý��������˲���
								GuildErr = GUILD_ERROR_ALREADY_MEMBER;
							}
							else
							{
								//ȫ�������а���е������б����һ�ߣ��Ǻǣ�����
								for( INT i=0; i<MAX_GUILD_SIZE; i++ )
								{
									Guild* pDeleteGuild = g_pGuildManager->GetGuild( i );
									if(pDeleteGuild->GetGuildID() != INVALID_ID && pDeleteGuild->GetGuildID() != pGuild->GetGuildID())
									{
										for(INT j = 0; j<USER_ARRAY_SIZE;j++)
										{
											GUILDUSER* pDeleteUser = pDeleteGuild->GetGuildUserByIndex(j);
											if(pDeleteUser->m_UserGUID != INVALID_ID)
											{
												if(pDeleteUser->m_UserGUID == pGuildRecruit->m_ProposerGUID
													&&pDeleteUser->m_Position == GUILD_POSITION_TRAINEE)
												{//ɾ������
													pDeleteGuild->OnUserLeaveByIndex(j);
													break;
												}
											}
										}
									}
								}

								GuildErr = pGuild->OnRecruitUser( pGuildUser->m_UserGUID );
                                
                                if(GuildErr == GUILD_ERROR_MEMBER_FULL)
                                {
                                    CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                                    sprintf(szMailContent, "�����İ��%s�����Ѿ��������ޣ��޷����롣", pGuild->GetGuildName());
                                    g_pMailCenter->SendNormalMail(pGuildUser->m_szUserName, szMailContent);

                                    CHAR szMail[MAX_MAIL_CONTEX] = {0};
                                    sprintf(szMail, "��������Ѿ��������ޣ�%s�޷����롣", pGuild->GetGuildName());
                                    g_pMailCenter->SendNormalMail(pUser->GetName(), szMail);
                                }

								if(GuildErr == GUILD_ERROR_NOTHING)
								{
									USER* pRecruitUser = g_pOnlineUser->FindUser( pGuildUser->m_szUserName );
									if( !pRecruitUser )
									{
										//��һ���ִ���ʼ�
										//���´δ��������ʱ����Serverִ�д��ʼ���Ӧ�Ľű�������ֻ�Ǹ��Ĵ��˵Ĺ���ID
										g_pMailCenter->SendScriptMail( pGuildUser->m_szUserName,
											MAIL_UPDATE_ATTR, MAIL_ATTR_GUILD, guildID);

										//��ӭ�����BB�����ͬ�۹�����Я���߽�����
										CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
										sprintf(szMailContent, "��ӭ�����%s�����ͬ�۹�����Я���߽�����", pGuild->GetGuildName());
										g_pMailCenter->SendNormalMail(pGuildUser->m_szUserName, szMailContent);
									}

									WGGuildReturn Msg;
									_GUILD_RETURN GuildReturn;
									GuildReturn.m_ReturnType = GUILD_RETURN_RECRUIT;
									GuildReturn.m_GUID = pGuildUser->m_UserGUID;
									GuildReturn.m_GUIDChanged = guid;
									GuildReturn.m_GuildID	=	guildID;
									
									GuildReturn.m_SourNameSize = (UCHAR)strlen(pUser->GetName());
									strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), MAX_CHARACTER_NAME);
									
									GuildReturn.m_DestNameSize = (UCHAR)strlen(pGuildUser->m_szUserName);
									strncpy((CHAR*)GuildReturn.m_DestName, pGuildUser->m_szUserName, MAX_CHARACTER_NAME);
									
									GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
									strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
								
									Msg.SetGuildReturn( &GuildReturn );
									pGuild->BroadCast_Return( &Msg );
									Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X recruited a guild!", 
										guid );
									return PACKET_EXE_CONTINUE;
								}
							}
						}
					}
					break;
				default:
					Assert(FALSE);
				}
			}

			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Recruit a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_EXPEL:
		{
			if( guildID == INVALID_ID )
			{ // ���а�ᣬ����Ӧ���Ѿ��� Server ��ȷ�������Բ����ش�����
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to expel a user!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
			}
			else
			{
				switch( pGuild->GetGuildStatus() )
				{
				case GUILD_STATUS_NORMAL:
					{
						if( pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_EXPEL) == FALSE )
						{
							GuildErr = GUILD_ERROR_UNAUTHORIZED;
						}
						else
						{
							GUILD_CGW_EXPEL* pGuildExpel;
							pGuildExpel = (GUILD_CGW_EXPEL*)pGuildPacket->GetPacket(GUILD_PACKET_GW_EXPEL);
							Assert( pGuildExpel );

							const GUILDUSER* pGuildUser = pGuild->GetGuildUser( pGuildExpel->m_GuildUserGUID );
							if( pGuildUser == NULL )
							{
								Assert(FALSE);
								GuildErr = GUILD_ERROR;
							}
							else if( pGuildUser->m_Position
								>= pGuild->GetGuildUser(pUser->GetGUID())->m_Position )
							{ //ֻ�ܶԱ��Լ�����͵��˽��в������Ժ���������жϿ���Ҫ����ϸ��
								//ֻ�������ͨ����
								GuildErr = GUILD_ERROR_UNAUTHORIZED;
							}
							else
							{
								CHAR szTempName[MAX_CHARACTER_NAME];
								strncpy((CHAR*)szTempName, pGuildUser->m_szUserName, sizeof(szTempName) - 1);
								BOOL bIsPropose = (pGuildUser->m_Position == GUILD_POSITION_TRAINEE)?TRUE:FALSE;
								GuildErr = pGuild->OnUserLeave( pGuildUser->m_UserGUID );

								if(GuildErr == GUILD_ERROR_NOTHING)
								{
									WGGuildReturn Msg;

									_GUILD_RETURN GuildReturn;

									if(bIsPropose)
									{
										GuildReturn.m_ReturnType = GUILD_RETURN_REJECT;
										GuildReturn.m_GUID = pGuildExpel->m_GuildUserGUID;
										
										GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
										strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
										
										GuildReturn.m_DestNameSize = (UCHAR)strlen(szTempName);
										strncpy((CHAR*)GuildReturn.m_DestName, szTempName, GuildReturn.m_DestNameSize);
										
										Msg.SetGuildReturn( &GuildReturn );
										Msg.SetPlayerID( pUser->GetPlayerID() );
										pServerPlayer->SendPacket( &Msg );
									}
									else
									{
										GuildReturn.m_ReturnType = GUILD_RETURN_EXPEL;
										GuildReturn.m_GUID = pGuildExpel->m_GuildUserGUID;
										
										GuildReturn.m_SourNameSize = (UCHAR)strlen(pUser->GetName());
										strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), GuildReturn.m_SourNameSize);
										
										GuildReturn.m_DestNameSize = (UCHAR)strlen(szTempName);
										strncpy((CHAR*)GuildReturn.m_DestName, szTempName, GuildReturn.m_DestNameSize);
										
										Msg.SetGuildReturn( &GuildReturn );
										pGuild->BroadCast_Return( &Msg );

                                        // ���Ƶ���㲥
                                        CHAR szChat[MAX_MAIL_CONTEX] = {0};
                                        sprintf(szChat, "%s�뱾��־��ͬ�����ϣ����뿪��%s��", szTempName, pGuild->GetGuildName());

                                        WGChat MsgR ;
                                        MsgR.SetPlayerID( pUser->GetPlayerID() ) ;
                                        MsgR.SetChatType( CHAT_TYPE_GUILD ) ;
                                        MsgR.SetContexSize( (BYTE)strlen(szChat) ) ;
                                        MsgR.SetContex( szChat ) ;
                                        MsgR.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;
                                        MsgR.SetGuildID(pGuild->GetGuildID());
                                        g_pServerManager->BroadCastServer( &MsgR ) ;
									}

									USER* pDestUser = g_pOnlineUser->FindUser( pGuildExpel->m_GuildUserGUID );
									if( pDestUser != NULL )
									{
										pDestUser->CleanUpGuildData();
										pServerPlayer = g_pServerManager->GetServerPlayer(pDestUser->GetServerID());

										if( pServerPlayer != NULL )
										{
											Msg.SetPlayerID( pDestUser->GetPlayerID() );
											pServerPlayer->SendPacket( &Msg );
										}
									}
									else
									{
										//��һ���ִ���ʼ�
										//���´δ��������ʱ����Serverִ�д��ʼ���Ӧ�Ľű�������ֻ�Ǹ��Ĵ��˵Ĺ���ID
										g_pMailCenter->SendScriptMail( szTempName,
											MAIL_UPDATE_ATTR, MAIL_ATTR_GUILD, INVALID_ID);

										if(bIsPropose)
										{
											//BB���ܾ���������롣
											CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
											sprintf(szMailContent, "%s���ܾ���������롣", pGuild->GetGuildName());
											g_pMailCenter->SendNormalMail(szTempName, szMailContent);
										}
										else
										{
											//����BB־��ͬ�����ϣ����Ѿ��뿪��BB�ˡ�
											CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
											sprintf(szMailContent, "����%s־��ͬ�����ϣ����Ѿ��뿪��%s�ˡ�", pGuild->GetGuildName(), pGuild->GetGuildName());
											g_pMailCenter->SendNormalMail(szTempName, szMailContent);

										}
									}

									Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X expelled a guild!", 
										guid );
									return PACKET_EXE_CONTINUE;
								}
							}
						}
					}
					break;
				default:
					Assert(FALSE);
				}
			}
			
			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Recruit a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_WITHDRAW:
		{
		}
		break;
	case GUILD_PACKET_GW_DEPOSIT:
		{
		}
		break;
	case GUILD_PACKET_GW_LEAVE:
		{
			if( guildID == INVALID_ID )
			{ // ���а�ᣬ����Ӧ���Ѿ��� Server ��ȷ�������Բ����ش�����
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to leave from nil guild!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
				break;
			}
			else if( pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_LEAVE) == FALSE )
			{
				GuildErr = GUILD_ERROR_UNAUTHORIZED;
				break;
			}

			GuildErr = pGuild->OnUserLeave(pUser->GetGUID());

			if(GuildErr == GUILD_ERROR_NOTHING)
			{
				pUser->CleanUpGuildData();
				WGGuildReturn Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				_GUILD_RETURN GuildReturn;
				GuildReturn.m_ReturnType = GUILD_RETURN_LEAVE;
				GuildReturn.m_GUID = pUser->GetGUID();
				
				GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
				strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
				
				GuildReturn.m_DestNameSize = (UCHAR)strlen(pUser->GetName());
				strncpy((CHAR*)GuildReturn.m_DestName, pUser->GetName(), MAX_CHARACTER_NAME);
				
				Msg.SetGuildReturn( &GuildReturn );
				pServerPlayer->SendPacket( &Msg );
				pGuild->BroadCast_Return( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X left a guild!", 
					guid );

                //AAA�뱾��־��ͬ�����ϣ����뿪��BB
                CHAR szChat[MAX_MAIL_CONTEX] = {0};
                sprintf(szChat, "%s�뱾��־��ͬ�����ϣ����뿪��%s��", pUser->GetName() , pGuild->GetGuildName());

                WGChat MsgR ;
                MsgR.SetPlayerID( pUser->GetPlayerID() ) ;
                MsgR.SetChatType( CHAT_TYPE_GUILD ) ;
                MsgR.SetContexSize( (BYTE)strlen(szChat) ) ;
                MsgR.SetContex( szChat ) ;
                MsgR.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;
                MsgR.SetGuildID(pGuild->GetGuildID());
                g_pServerManager->BroadCastServer( &MsgR ) ;

				return PACKET_EXE_CONTINUE;
			}

			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Leave a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_DISMISS:
		{
			if( guildID == INVALID_ID )
			{ // ���а�ᣬ����Ӧ���Ѿ��� Server ��ȷ�������Բ����ش�����
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to leave from nil guild!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
				break;
			}
			else if( pUser->GetGUID() != pGuild->GetChieftainGUID() )
			{
				GuildErr = GUILD_ERROR_UNAUTHORIZED;
				break;
			}

			GuildErr = g_pGuildManager->DestroyGuild( pUser->GetGUID(), guildID );
			if( GuildErr == GUILD_ERROR_NOTHING )
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X left a guild!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	case GUILD_PACKET_GW_DEMISE:
		{
			if( guildID == INVALID_ID )
			{ // ���а�ᣬ����Ӧ���Ѿ��� Server ��ȷ�������Բ����ش�����
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to leave from nil guild!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
				break;
			}
			else if( pUser->GetGUID() != pGuild->GetChieftainGUID() )
			{
				GuildErr = GUILD_ERROR_UNAUTHORIZED;
				break;
			}

			GuildErr = pGuild->OnDemise();
			if(GuildErr == GUILD_ERROR_NOTHING)
			{
				USER* pNewChief = g_pOnlineUser->FindUser( pGuild->GetChieftainGUID( ) );
				if(pNewChief == NULL)
				{
					//���Ȩ�޸ı��ˣ���ϲ���Ϊ��������
					CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
					sprintf(szMailContent, "���Ȩ�޸ı��ˣ���ϲ���Ϊ��������");
					g_pMailCenter->SendNormalMail(pGuild->GetGuildChiefName(), szMailContent);
				}
    
				WGGuildReturn Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				_GUILD_RETURN GuildReturn;
				GuildReturn.m_ReturnType	=	GUILD_RETURN_DEMISE;
				GuildReturn.m_GuildID		=	guildID;
				GuildReturn.m_GUID			=	pUser->GetGUID();
				GuildReturn.m_GUIDChanged	=	pGuild->GetChieftainGUID();

				GuildReturn.m_SourNameSize	=	(UCHAR)strlen(pUser->GetName());
				strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), GuildReturn.m_SourNameSize);

				GuildReturn.m_DestNameSize	=	(UCHAR)strlen(pGuild->GetGuildChiefName());
				strncpy((CHAR*)GuildReturn.m_DestName, pGuild->GetGuildChiefName(), GuildReturn.m_DestNameSize);

				GuildReturn.m_GuildNameSize	=	(UCHAR)strlen(pGuild->GetGuildName());
				strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);

				GuildReturn.m_ChangedPositionNameSize	=	(UCHAR)strlen(pGuild->GetPosName(GUILD_POSITION_CHIEFTAIN));
				strncpy((CHAR*)GuildReturn.m_ChangedPositionName, pGuild->GetPosName(GUILD_POSITION_CHIEFTAIN), GuildReturn.m_ChangedPositionNameSize);

				GuildReturn.m_PositionNameSize	=	(UCHAR)strlen(pGuild->GetPosName(GUILD_POSITION_ASS_CHIEFTAIN));
				strncpy((CHAR*)GuildReturn.m_PositionName, pGuild->GetPosName(GUILD_POSITION_ASS_CHIEFTAIN), GuildReturn.m_PositionNameSize);


				Msg.SetGuildReturn( &GuildReturn );
				pServerPlayer->SendPacket( &Msg );
				pGuild->BroadCast_Return( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X left a guild!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	case GUILD_PACKET_GW_CHANGEDESC:
		{
			if( guildID != INVALID_ID )
			{ // ���а�ᣬ����Ӧ���Ѿ��� Server ��ȷ�������Բ����ش�����
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to create an extra guild!", 
					guid );
				GuildErr = GUILD_ERROR_GUILD_ALREADY_EXIST;
				break;
			}

			GUILD_CGW_CHANG_DESC* pGuildChangDesc = (GUILD_CGW_CHANG_DESC*)pGuildPacket->GetPacket(GUILD_PACKET_GW_CHANGEDESC);
			Assert( pGuildChangDesc );

			WGGuildReturn Msg;
			Msg.SetPlayerID( pUser->GetPlayerID() );
			Guild* pGuild = g_pGuildManager->GetGuild(guildID);
			pGuild->SetGuildDesc(pGuildChangDesc->m_GuildDesc);
			_GUILD_RETURN GuildReturn;
			GuildReturn.m_GuildID = pUser->GetGuildID();
			GuildReturn.m_ReturnType = GUILD_RETURN_CHANGEDESC;
			Msg.SetGuildReturn( &GuildReturn );
			pServerPlayer->SendPacket( &Msg );
			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X created a guild success!", 
				guid );
			return PACKET_EXE_CONTINUE;

		}
		break;

	default:
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	WGGuildError Msg;
	Msg.SetPlayerID( pUser->GetPlayerID() );
	Msg.SetErrorCode( GuildErr );
	pServerPlayer->SendPacket( &Msg );
	Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X do a guild action failed!", 
		guid );
	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
