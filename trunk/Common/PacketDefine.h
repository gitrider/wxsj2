#ifndef __PACKET_DEFINE_H__
#define __PACKET_DEFINE_H__


namespace Packets
{
	//��Ϣ����ֵ������ʽ��PACKET_XX_YYYYYY
	//XX��������Ϊ��GC��CG��CL��LC��GL��LG
	//G��Ϸ�������ˡ�C�ͻ��ˡ�L��½�������ˡ�S�������˳��򣨿����ǵ�½��Ҳ��������Ϸ��
	//YYYYYY��ʾ��Ϣ����
	//���磺PACKET_CG_ATTACK ��ʾ�ͻ��˷�����Ϸ�������˹��ڹ�������Ϣ
	enum PACKET_DEFINE
	{
		PACKET_NONE = 0 ,				// 0����
		//1
		PACKET_CG_TEST ,
		PACKET_GC_TEST ,
		PACKET_CG_CONNECT ,				// �ͻ������ӵ���Ϸ�������˺��͵ĵ�һ����֤��Ϣ
		PACKET_CG_HEARTBEAT ,			// ������Ϣ
		PACKET_GC_CONNECT ,				// �������յ�PACKET_CG_CONNECT���͸��ͻ���
										// ��ȷ����Ϣ
		PACKET_CG_ENTERSCENE ,			//�ͻ�����ͼ����ĳ������ʱ������������������Ϣ	
		PACKET_CG_LEAVESCENE ,			//�ͻ�����ͼ�뿪ĳ������ʱ������������������Ϣ	
		PACKET_GC_LEAVESCENE ,			//���������������ͻ���ĳ����뿪����Ϣ
		PACKET_GC_ENTERSCENE ,			//�����������ͻ��˽��볡����ȷ����Ϣ
		PACKET_CG_ENVREQUEST ,			// �ͻ������������������Χ������Ϣ
		//11
		PACKET_GC_NEWITEM,				//���µĶ�����Ʒ�������Լ�����Ұ��Χ
		PACKET_GC_NEWITEMBOX,
		PACKET_GC_NEWMONSTER,			//���µĹ�������Լ�����Ұ��Χ
		PACKET_GC_NEWMONSTER_DEATH,		//����������OBJ
		PACKET_GC_NEWMONSTER_MOVE,		//�����ƶ���OBJ
		PACKET_GC_NEWPLAYER,			//���µ�������ҽ����Լ�����Ұ��Χ
		PACKET_GC_NEWPLAYER_DEATH,		//����������PLAYER
		PACKET_GC_NEWPLAYER_MOVE,		//�����ƶ���PLAYER
		PACKET_GC_PLAYERREALMOVE,		//����
		PACKET_GC_MONSTERREALMOVE,		//����
		//21
		PACKET_CG_CHARASKBASEATTRIB,		//��ȡ��ɫ������
		PACKET_CG_CHARASKEQUIPMENT,			//��ȡ��װ����
		PACKET_GC_CHARBASEATTRIB,			//ˢ�½�ɫ��������
		PACKET_GC_CHAREQUIPMENT,			//ˢ�½�ɫ��װ����
		PACKET_GC_DIE ,
		PACKET_GC_LEVEUP,
		PACKET_CG_REQLEVELUP,				//��������
		PACKET_GC_LEVELUPRESULT,			//�����������
		PACKET_CG_REQMANUALATTR,			//�����������
		PACKET_GC_MANUALATTRRESULT,			//����������
		//31
		PACKET_CG_REQRESETATTR,				//ϴ������
		PACKET_GC_RESETATTRRESULT,			//ϴ����
		PACKET_CG_MYEQUIPMENT,			//����
		PACKET_CG_OTHEREQUIPMENT,		//����
		PACKET_GC_MYEQUIPMENT,			//����
		PACKET_GC_OTHEREQUIPMENT,		//����
		PACKET_CG_ASKDETAILATTRIB ,			//������µ�ǰ��������
		PACKET_CG_ASKDETAILEQUIPMENTLIST,	//������µ�ǰ����װ���б�
		PACKET_CG_ASKDETAILSKILLLIST,		//������µ�ǰ���Ǽ����б�
		PACKET_CG_ASKDETAILSUBEFFECTLIST,	//������µ�ǰ���Ǹ��������б�
		//41
		PACKET_GC_DETAILATTRIB ,			//ˢ�µ�ǰ��������
		PACKET_GC_DETAILEQUIPMENTLIST,		//ˢ�µ�ǰ����װ���б�
		PACKET_GC_DETAILSKILLLIST,			//ˢ�µ�ǰ���Ǽ����б�
		PACKET_GC_DETAILSUBEFFECTLIST,		//ˢ�µ�ǰ���Ǹ��������б�
		PACKET_CG_ASKMYBAGLIST,				//����ˢ�µ�ǰ����
		PACKET_GC_MYBAGLIST,				//ˢ�����Ǳ���
		PACKET_CG_CHARIDLE,					//֪ͨSERVER����IDLE״̬
		PACKET_CG_CHARJUMP,					//֪ͨSERVER��Ծһ��
		PACKET_CG_CHARMOVE,					//֪ͨSERVER�ƶ�
		PACKET_GC_CHARIDLE,					//֪ͨCLIENT����IDLE״̬
		//51
		PACKET_GC_CHARJUMP,					//֪ͨCLIENT��Ծһ��
		PACKET_GC_CHARMOVE,					//֪ͨCLIENT�ƶ�
		PACKET_CG_MOVE ,				//����
		PACKET_GC_MOVE ,				//����
		PACKET_GC_ARRIVE ,				//����
		PACKET_CG_CHARUSESKILL,				//֪ͨSERVERʹ�ü���
		PACKET_GC_CHARSKILL_CREATEBULLET,	//�����ӵ�
		PACKET_GC_CHARSKILL_EFFECT,			//��������Ч��(�˺�)
		PACKET_GC_CHARSKILL_GATHER,			//���ܾ���
		PACKET_GC_CHARSKILL_GATHER_MODIFY,	//���ܾ���ʱ����ӳ��ȵ�
		//61
		PACKET_GC_CHARSKILL_LEAD,			//��������
		PACKET_GC_CHARSKILL_LEAD_MODIFY,	//��������ʱ������̵ȵ�
		PACKET_GC_CHARSKILL_SEND,			//���ܷ���
		PACKET_CG_ATTACK ,				//����
		PACKET_GC_ATTACK ,				//����
		PACKET_GC_OTHERATTACK ,			//����
		PACKET_GC_ERRORATTACK ,			//����
		PACKET_CG_SKILL ,				//����
		PACKET_GC_SKILL ,				//����
		PACKET_GC_OTHERSKILL ,			//����
		//71
		PACKET_GC_ERRORSKILL ,			//����
		PACKET_GC_SKILLPREPARE ,		//����
		PACKET_CG_OPENITEMBOX,				//��Obj_ItemBox
		PACKET_CG_PICKBOXITEM,				//��ȡObj_ItemBox���һ����Ʒ
		
		PACKET_CG_USEEQUIP,					//װ��������װ��
		PACKET_CG_UNEQUIP,					//ж��װ��
		PACKET_CG_USEITEM,					//ʹ�õ���
		PACKET_GC_USEITEMRESULT,			//ʹ�õ��߽��
		PACKET_CG_DISCARDITEM,				//�������������Ʒ
		PACKET_CG_DISCARDEQUIP,				//��������װ��
		//81
		PACKET_GC_BOXITEMLIST,				//Obj_ItemBox ��Ʒ�б�
		PACKET_GC_PICKRESULT,				//��ȡ���
		
		PACKET_GC_USEEQUIPRESULT,			//װ��װ�����
		PACKET_GC_DELOBJECT,				//ɾ��OBJ
		PACKET_GC_UNEQUIPRESULT,			//ж��װ�����
		PACKET_GC_DISCARDITEMRESULT,		//������Ʒ���
		PACKET_SS_CONNECT ,				// ������֮����������ʱ��ĵ�һ��֪ͨ��Ϣ
		PACKET_GS_ASKDATA ,				//��Դ�������˳���Ҫ��õ�Player����
		PACKET_SG_RETDATA ,				//��GameServer�˳������û�����
		PACKET_GW_ASKUSERDATA ,			//���������ݷ�����Ҫ���û�����
		//91
		PACKET_WG_RETUSERDATA ,			//�����û�������Ϣ
		PACKET_GW_ASKCHANGESCENE ,		//���������ݷ����������û����ϣ�Ҫ��ת�Ƴ���
		PACKET_WG_RETCHANGESCENE ,		//����ת�Ƴ��������Ϣ
		PACKET_GW_NOTIFYUSER ,			//��Ϸ������֪ͨ����������û�״̬��Ҫ�ı�
		PACKET_WG_NOTIFYUSER ,			//���������֪ͨ��Ϸ�������û�״̬�仯
		PACKET_CG_COMMAND ,				//�ͻ����������������Ϸ�޸�ָ��
		PACKET_CG_ASKCHANGESCENE ,		//�ͻ�������ı䳡��
		PACKET_GC_RETCHANGESCENE ,		//��������Ӧ�ͻ��˵ĳ����任����
		PACKET_GC_CHAT,					//˵��
		PACKET_GC_NOTIFYCHANGESCENE ,	//������֪ͨ�ͻ��˿��Ըı䳡����
		//101
		PACKET_CG_ASKITEMINFO,			//����ĳ����Ʒ����ϸ��Ϣ
		PACKET_GC_ITEMINFO,				//����������ĳ��װ������ϸ��Ϣ
		PACKET_CG_ASKDETAILEQUIPLIST,	//��������װ������ϸ��Ϣ
		PACKET_GC_DETAILEQUIPLIST,		//��������������װ����ϸ��Ϣ
		PACKET_GC_NEWPORTAL,			//��һ�����͵���Կ�����
		PACKET_CG_CHAT,					//˵��
		PACKET_GC_DETAILXINFALIST,		//ˢ�µ�ǰ�����ķ��б�
		PACKET_CG_ASKDETAILXINFALIST,	//������µ�ǰ�����ķ��б�
		PACKET_GC_DISCARDEQUIPRESULT,		//������Ʒ���
		PACKET_GC_NOTIFYEQUIP,				//֪ͨ�ͻ��˰����������װ����
		
		//111
		PACKET_GC_NEWSKILLOBJ,			//����SkillObj����Ϣ
		PACKET_CG_USEGEM,				//ʹ�ñ�ʯ
		PACKET_GC_USEGEMRESULT,			//ʹ�ñ�ʯ���
		PACKET_CG_REMOVEGEM,			//ж�ر�ʯ
		PACKET_GC_REMOVEGEMRESULT,		//ж�ر�ʯ���
		PACKET_GC_CHARIMPACT,			//��ɫ����Ч��
		PACKET_GC_OPERATE_RESULT,		//�����ķ���
		PACKET_CG_CHARDEFAULTEVENT,		//ȱʡ�¼�

		//121
		PACKET_CG_USEABILITY,			// ���䷽�������ָ��
		PACKET_CG_TEAMINVITE,			//�������
		PACKET_GC_TEAMERROR,			//
		PACKET_GW_TEAMINVITE,			//
		PACKET_WG_TEAMERROR,			//
		PACKET_WG_TEAMRESULT,			//
		PACKET_WG_NOTIFYTEAMINFO,		//
		PACKET_WG_TEAMASKINVITE,		//
		PACKET_GC_TEAMASKINVITE,		//
		PACKET_CG_TEAMRETINVITE,		//��ӻ�Ӧ
		//131
		PACKET_GW_TEAMRETINVITE,		//
		PACKET_WG_TEAMLEADERASKINVITE,	//
		PACKET_GC_TEAMLEADERASKINVITE,	//
		PACKET_CG_TEAMLEADERRETINVITE,	//�ӳ���Ӧ
		PACKET_GW_TEAMLEADERRETINVITE,	//
		PACKET_GC_TEAMRESULT,			//
		PACKET_CG_TEAMLEAVE,			//���
		PACKET_GW_TEAMLEAVE,			//
		PACKET_CG_TEAMDISMISS,			//��ɢ����
		PACKET_GW_TEAMDISMISS,			//
		//141
		PACKET_CG_TEAMKICK,				//�߳���Ա
		PACKET_GW_TEAMKICK,				//
		PACKET_CG_TEAMAPPLY,			//�����������
		PACKET_GW_TEAMAPPLY,			//
		PACKET_WG_TEAMASKAPPLY,			//
		PACKET_GW_TEAMRETAPPLY,			//
		PACKET_GC_TEAMASKAPPLY,			//
		PACKET_CG_TEAMRETAPPLY,			//
		PACKET_GC_SCRIPTCOMMAND,		// �ű����ͻ��˵�ָ��
		PACKET_GC_ABILITY_RESULT,		// ����ܵĲ������ؽ��
		//151
		PACKET_CG_ASKDETAILABILITYINFO, // �ͻ�����ҵ��������Ϣ
		PACKET_GC_DETAILABILITYINFO,	// �������˷�������������Ϣ
		PACKET_CG_TEAMAPPOINT ,			//�����¶ӳ�
		PACKET_GW_TEAMAPPOINT ,			//
		PACKET_CG_ABILITYGAMBLE,		// ��������䷽�ϳ�ָ��
		PACKET_GC_COOLDOWN_UPDATE,		// ��ȴʱ�����
		PACKET_CG_ASK_COOLDOWN_UPDATE,	//�ͻ������������ȴʱ����Ϣ --���ڲ�����.
		PACKET_CG_DOEVENT,				//�ͻ���ѡ���¼��б��е�ĳ��ִ��
		PACKET_GC_ABILITY_ACTION,		// ����ܵĲ�������
		PACKET_GC_ABILITY_SUCC,			// ����ܵĳɹ���Ϣ
		//161
		PACKET_GC_NOTIFYTEAMINFO ,		// ֪ͨ�ͻ���,��������Ϣ�仯
		PACKET_CG_MISSIONACCEPT ,		// ��������
		PACKET_CG_MISSIONREFUSE ,		// �ܾ�����
		PACKET_CG_MISSIONABANDON ,		// ��������
		PACKET_CG_MISSIONCONTINUE ,		// ��������
		PACKET_CG_MISSIONSUBMIT ,		// �ݽ�����
		PACKET_CG_EVENTREQUEST ,		// ��ѯһ���ű��¼�
		PACKET_CG_ASKMISSIONLIST ,		// �����б��ѯ
		PACKET_GC_MISSIONLIST ,			// ���������б�
		PACKET_GC_MISSIONADD ,			// ����һ������
		//171
		PACKET_GC_MISSIONREMOVE ,		// ɾ��һ������
		PACKET_GC_MISSIONMODIFY ,		// ����һ���������
		PACKET_CG_ASKMAIL ,				//
		PACKET_CG_CHANNELCREATE ,		//
		PACKET_CG_CHANNELDISMISS ,		//
		PACKET_CG_CHANNELINVITE ,		//
		PACKET_CG_CHANNELKICK ,			//
		PACKET_CG_MAIL ,				//
		PACKET_GC_CHANNELERROR ,		//
		PACKET_GC_CHANNELRESULT ,		//
		//181
		PACKET_GC_MAIL ,				//
		PACKET_GC_NOTIFYMAIL ,			//
		PACKET_GW_CHANNELCREATE ,		//
		PACKET_GW_CHANNELDISMISS ,		//
		PACKET_GW_CHANNELINVITE ,		//
		PACKET_GW_CHANNELKICK ,			//
		PACKET_GW_CHAT ,				//
		PACKET_GW_MAIL ,				//
		PACKET_WG_CHANNELERROR ,		//
		PACKET_WG_CHANNELRESULT ,		//
		//191
		PACKET_WG_CHAT ,				//
		PACKET_WG_MAIL ,				//
		PACKET_GC_NEWPLATFORM,			// ��������̨����Ϣ
		PACKET_CG_GEM_COMPOUND,			// ��ʯ�ϳ���Ϣ
		PACKET_GC_ABILITYLEVEL,			// �������Ϣ
		PACKET_GC_ABILITYEXP,			// �������������Ϣ
		PACKET_GC_PRESCRIPTION,			// ������䷽��Ϣ
		PACKET_GC_ABANDON_ABILITY,		// ���������
		PACKET_GW_ASKMAIL ,				// 
		PACKET_WG_NOTIFYMAIL ,				//
		//201
		PACKET_GC_ITEMBOXCHANGESTAGE,		//Obj_ItemBox�ı����
		PACKET_CG_QUERYITEMBOXSTAGE,		//����Itembox���
		PACKET_GC_CANPICKMISSIONITEMLIST,	//����ʰȡ��������Ʒ�б�
		PACKET_GC_ADDCANPICKMISSIONITEM,	//���һ������ʰȡ��������Ʒ
		PACKET_GC_REMOVECANPICKMISSIONITEM,	//ɾ��һ������ʰȡ��������Ʒ
		PACKET_GC_MODIFYSPEED,				//�����ٶȸı�
		PACKET_GC_MONSTERSPEAK,				//��������˵��
		PACKET_CG_ASKTEAMINFO,				// ���������Ϣ
		PACKET_GW_ASKTEAMINFO,				// ���������Ϣ
		PACKET_CG_ASKTEAMMEMBERINFO,		// ���������Ϣ
		//211
		PACKET_GC_TEAMMEMBERINFO,			// ���ض�����Ϣ
		PACKET_CG_LOCK_TARGET,				//�������Ŀ��
		PACKET_GC_LOCK_TARGET,				//�������Ŀ��
		PACKET_GW_ASKTEAMMEMBERINFO,		// ���������Ϣ
		PACKET_WG_TEAMMEMBERINFO,			// ���ض�����Ϣ
		PACKET_GC_SHOPMERCHANDISELIST,			//��Ʒ�б�	
		PACKET_GC_SHOPSOLDLIST,					//�ع���Ʒ�б�
		PACKET_GC_SHOPUPDATEMERCHANDISELIST,	//������Ʒ�б�
		PACKET_CG_SHOPBUY,		//��Ҵ���Ʒ�б�ѡ�������Ʒ	
		PACKET_GC_SHOPBUY,		//֪ͨ�ͻ������Ƿ�ɹ�
		//221
		PACKET_CG_SHOPSELL,		//��Ҵ���Ʒ�б�ѡ��Ҫ������Ʒ	
		PACKET_GC_SHOPSELL,		//֪ͨ�ͻ������Ƿ�ɹ�
		PACKET_CG_SHOPREPAIR,	//֪ͨ������������Ʒ
		PACKET_GC_SHOPREPAIR,	//֪ͨ������������Ʒ
		PACKET_CG_SHOPCLOSE,//֪ͨ�ͻ��˽��״��ڹرչر�
		PACKET_CG_ASKJOINMENPAI,		// ��������
		PACKET_GC_MENPAIINFO,			// ������Ϣ��ID��
		PACKET_GC_JOINMENPAI,				// �������ɵĽ��
		PACKET_CG_ASKSTUDYXINFA,			// ����ѧϰĳ��ĳ���ķ�
		PACKET_GC_XINFASTUDYINFO,			// ����ѧϰ���ķ�������������Ϣ
		//231
		PACKET_GC_STUDYXINFA_H,				// ѧϰ�Ľ��
		PACKET_GC_ABILITY_STUDYINFO,		// ѧϰ����ܵ���Ϣ
		PACKET_CG_ASKSTUDYABILITY,			// ��������ύ����ѧϰ����ܵ���Ϣ
		PACKET_GC_ASKSTUDYABILITY,			// ����������ѧϰ����ܵĽ��---���� ��PACKET_GC_ABILITYLEVELͳһ
		PACKET_CG_CHARALLTITLES,			// �ͻ��������������������еĳƺ�
		PACKET_GC_CHARALLTITLES,			// ������֪ͨ�ͻ��˴������еĳƺ�
		PACKET_CG_CHARUPDATECURTITLE,		// ������֪ͨ�ͻ��˴������еĳƺ�
		PACKET_CG_ASK_ACTIVETIME_UPDATE,	// �ͻ���������¼�����Чʱ����Ϣ
		PACKET_GC_ACTIVETIME_UPDATE,		// ����
		PACKET_GC_OBJ_TELEPORT,				// ��ɫ˲��
		PACKET_GW_TEAMMEMBERENTERSCENE,		// ��Ա�����³���
		//241
		PACKET_GC_BANKBEGIN,				// ֪ͨ�ͻ��˿�ʼ������
		PACKET_CG_BANKACQUIRELIST,			// ���������ȡ�洢���б�
		PACKET_GC_BANKACQUIRELIST,			// �����е���Ʒ���͸��ͻ���
		PACKET_CG_BANKADDITEM,			// ֪ͨ�������������м���Ʒ
		PACKET_GC_BANKADDITEM,			// ֪ͨ�ͻ��˼���Ʒ���
		PACKET_CG_BANKREMOVEITEM,			// ֪ͨ���������������Ƴ���Ʒ
		PACKET_GC_BANKREMOVEITEM,			// ֪ͨ�ͻ����Ƴ���Ʒ���
		PACKET_CG_BANKSWAPITEM,			// ֪ͨ���������������뱳����װ�����н�����Ʒ
		PACKET_GC_BANKSWAPITEM,			// ֪ͨ�ͻ��˴��������뱳����װ�����н�����Ʒ�Ľ��
		PACKET_CG_BANKMONEY,			// ֪ͨ��������ǮȡǮ
		//251
		PACKET_GC_BANKMONEY,			// ֪ͨ�ͻ��˴�ǮȡǮ���
		PACKET_CG_BANKCLOSE,			// ֪ͨ�������رմ�������
		PACKET_GC_DETAILATTRIB_PET,			// �������ϸ����
		PACKET_GC_EXCHANGEERROR,			//���׳���
		PACKET_GC_EXCHANGEAPPLYI,			//���״�
		PACKET_GC_EXCHANGEESTABLISHI,		//����ȷ��
		PACKET_CG_EXCHANGEAPPLYI,			//���뽻��
		PACKET_CG_EXCHANGEREPLYI,			//���״�����
		PACKET_CG_EXCHANGESYNCHLOCK,		//����ͬ����
		PACKET_GC_EXCHANGESYNCHLOCK,		//����ͬ����
		//261
		PACKET_CG_EXCHANGESYNCHITEMII,		//����ͬ����Ʒ
		PACKET_CG_EXCHANGESYNCHMONEYII,		//����ͬ����Ǯ
		PACKET_GC_EXCHANGESYNCHCONFIRMII,	//����ͬ��ȷ��
		PACKET_GC_EXCHANGESYNCHII,			//����ͬ��
		PACKET_CG_EXCHANGEOKIII,			//����ͬ����Ϣ
		PACKET_GC_EXCHANGESUCCESSIII,		//���׳ɹ���Ϣ
		PACKET_CG_EXCHANGECANCEL,			//����ȡ��
		PACKET_GC_EXCHANGECANCEL,			//����ȡ��
		PACKET_GW_LEVEUP,					// ��Ա�������¼���ˢ�µ� World ����
		PACKET_GC_PLAYER_DIE,			// ��������
		//271
		PACKET_CG_PLAYER_DIE_RESULT,	// ���������Ĳ������
		PACKET_GC_PLAYER_RELIVE,		// ���Ǹ���
		PACKET_GW_ASKSCENEDATA,			//��Ϸ�������������ݷ��������󳡾�������Ϣ
		PACKET_WG_RETSCENEDATA,			//�������ݷ������򳡾����ͳ���������Ϣ
		PACKET_GG_SCENENOTIFY,			//����֪ͨ��Ϣ
		PACKET_CG_SET_PETATTRIB,		//	֪ͨ�����������ĳ�������
		PACKET_CG_MANIPULATEPET,		// ������������,���ٻ�,�ջ�,�����Ȳ���
		PACKET_GC_MANIPULATEPETRET,		// �����������ķ��ؽ��
		PACKET_CG_ASK_TEAMFOLLOW,			// �ӳ����������Ӹ���
		PACKET_CG_STOP_TEAMFOLLOW,			// ĳ��Ա������ֹͣ�������״̬
		//281
		PACKET_GC_ASK_TEAMFOLLOW,			// ����Ա���ͽ�����Ӹ�������
		PACKET_CG_RETURN_TEAMFOLLOW,		// ��Ա�ظ��Ƿ������Ӹ���״̬
		PACKET_GC_RETURN_TEAMFOLLOW,		// ���ͻ��˷�����Ӹ���״̬
		PACKET_GC_TEAMFOLLOW_LIST,			// ��Ӹ����б�
		PACKET_GC_TEAMFOLLOW_ERROR,			// ��Ӹ����µĴ�����Ϣ
		PACKET_GW_ENTER_TEAMFOLLOW,			// ĳ��Ա����������״̬
		PACKET_GW_STOP_TEAMFOLLOW,			// ĳ��Աֹͣ�������״̬
		PACKET_WG_TEAMFOLLOW_LIST,			// ������Ӹ����б�
		PACKET_CG_MISSIONCHECK,				// ��֤�����Ƿ��Ѿ����
		PACKET_GC_MISSIONRESULT,			// ����������
		//291
		PACKET_GC_CHAR_DIRECT_IMPACT = 291, //Ч��:һ����Ч��
		PACKET_GC_CHAR_BUFF, 				//Ч��: ������Ч��
		PACKET_GC_DETAIL_BUFF,				//Ч��: ������Ч������ϸ����.��������,ֻ�����Լ�
		PACKET_GC_CHARSKILL_MISSED,			//����: ����û������
		PACKET_CG_DISPEL_BUFF,				//�ͻ�������ȡ��������Ч��
		PACKET_CG_CHAR_STOP_LOGIC,			//�ͻ���������ֹ��ǰ�߼�
		PACKET_GC_CHAR_IMPACT_LIST_UPDATE,	//����Ч���б�(��Ҫ��Ϣ���㲥��)
		PACKET_GC_DETAIL_IMPACT_LIST_UPDATE,//����Ч���б�(��ϸ��Ϣ��ֻ������Լ���)
		PACKET_GC_DETAIL_HEALS_AND_DAMAGES, //HP,MP,Rage,Strike Point�ı仯�����ͻ�����ʾ�˺���������
		PACKET_GC_TARGET_LIST_AND_HIT_FLAGS,//Ŀ���б�ͻ���������ڿͻ�����ʾ�ӵ��ͻ������
		//301
		PACKET_CG_PACKAGE_SWAPITEM,			// Ҫ�󽻻��������������Ʒ
		PACKET_GC_PACKAGE_SWAPITEM,			// �����������������Ʒ�Ľ��
		PACKET_CG_STALLAPPLY,				// ������������ڴ˴���̯
		PACKET_GC_STALLAPPLY,				// ������֪ͨ�ͻ��������ڴ˴���̯
		PACKET_CG_STALLESTABLISH,			// �ͻ��˽���̯λ����˰��
		PACKET_GC_STALLESTABLISH,			// ֪ͨ�ͻ����ڴ˴���̯
		PACKET_CG_STALLOPEN,				// �����������̯λ��Ʒ�б�
		PACKET_GC_STALLOPEN,				// ̯λ��Ʒ�б�̯λ��Ϣ
		PACKET_CG_STALLADDITEM,				// ֪ͨ��������̯λ�м���Ʒ
		PACKET_GC_STALLADDITEM,				// ���߿ͻ�����̯λ�м���Ʒ
		//311
		PACKET_CG_STALLREMOVEITEM,			// ֪ͨ��������̯λ���ó���Ʒ
		PACKET_GC_STALLREMOVEITEM,			// ֪ͨ�ͻ�����̯λ���ó���Ʒ
		PACKET_CG_STALLITEMPRICE,			// ֪ͨ��������̯λ�е���Ʒ�ı��ۼ�
		PACKET_GC_STALLITEMPRICE,			// ֪ͨ��������̯λ�е���Ʒ�ı��ۼ�
		PACKET_CG_STALLSHOPNAME,			// ֪ͨ���������̸���
		PACKET_CG_STALLBUY,					// ֪ͨ���������������Ʒ
		PACKET_GC_STALLBUY,					// ֪ͨ�ͻ��˹��������Ʒ
		PACKET_CG_STALLCLOSE,				// ֪ͨ�������Լ��رյ���
		PACKET_GC_STALLCLOSE,				// ֪ͨ�ͻ����Լ��رյ���
		PACKET_GC_STALLERROR,				// ֪ͨ��̯����
		//321
		PACKET_GC_BBSMESSAGES,				// ���԰����
		PACKET_CG_BBSSYCHMESSAGES,			// ���԰����
		PACKET_CG_BBSAPPLY,					// ���԰����
		PACKET_CG_RELATION,					//�ͻ�������Ϸ���������ͺ��������Ϣ
		PACKET_GC_RELATION,					//��Ϸ��������ͻ��˷��ͺ��������Ϣ
		PACKET_GW_RELATION,					//��Ϸ���������������ݷ��������ͺ��������Ϣ
		PACKET_WG_RELATION,					//�������ݷ���������Ϸ���������ͺ��������Ϣ
		PACKET_CG_GUILD,					// �����Ϣ
		PACKET_GW_GUILD,					// �����Ϣ
		PACKET_WG_GUILD,					// �����Ϣ
		// 331
		PACKET_GC_GUILD,					// �����Ϣ
		PACKET_WG_GUILDLIST,				// ���ذ���б�(PlayerID_t, UCHAR, {GuildID_t,NAME,Level,UserCount,Status})
		PACKET_GC_GUILDLIST,				// ���ذ���б�(UCHAR, {GuildID_t,NAME,Level,UserCount,Status})
		PACKET_WG_GUILDERROR,				// ���ذ���������(PlayerID_t, enum)
		PACKET_GC_GUILDERROR,				// ���ذ���������(enum)
		PACKET_WG_GUILDRETURN,				// ���ذ��������(PlayerID_t, enum)
		PACKET_GC_GUILDRETURN,				// ���ذ��������(enum)
		PACKET_WG_GUILDBASEINFO,			// ��������Ϣ
		PACKET_GC_GUILDBASEINFO,			// ��������Ϣ
		PACKET_WG_GUILDMEMBERS,				// ������Ϣ
		// 341
		PACKET_GC_GUILDMEMBERS,				// ������Ϣ
		PACKET_WG_GUILDAUTHORITY,			// ���Ȩ�ޱ�
		PACKET_GC_GUILDAUTHORITY,			// ���Ȩ�ޱ�
		PACKET_CG_ITEMSYNCH,				// ��Ʒ�ƶ���Ϣ
		PACKET_GC_ITEMSYNCH,				// ��Ʒ�ƶ���Ϣ
		PACKET_GC_ITEMLIST,					// ������Ʒ�б���ͻ���
		PACKET_CL_ASKLOGIN,					//�ͻ�������Login��¼
		PACKET_LC_RETLOGIN,					//Login ���ؿͻ��˵�������

		//351
		PACKET_CL_ASKCHARLIST,				//�����ɫ�б�
		PACKET_LC_RETCHARLIST,				//���ؽ�ɫ�б�
		PACKET_LC_STATUS,					//���ؿͻ��˵��Ŷ�״̬
		PACKET_CL_ASKCHARLOGIN,				//�����½ָ����ɫ
		PACKET_LC_RETCHARLOGIN,				//�����½ָ����ɫ
		PACKET_LW_ASKCHARLOGIN,				//�����ɫ��¼
		PACKET_WL_RETCHARLOGIN,				//World�����ɫ��¼
		PACKET_CL_ASKCREATECHAR,			//���󴴽���ɫ
		PACKET_LC_RETCREATECHAR,			//���ش�����ɫ���
		PACKET_CL_ASKDELETECHAR,			//����ɾ����ɫ
		//361
		PACKET_LC_RETDELETECHAR,			//����ɾ����ɫ���
		PACKET_WG_TEAM_LIST,				// ���ض�Ա�б�
		PACKET_GC_TEAM_LIST,				// ���ض�Ա�б�
		PACKET_CG_EXECUTESCRIPT,			//����ִ�з������˵�ĳ���ű�����
		PACKET_CL_CONNECT,					//Client����Login����
		PACKET_LC_RETCONNECT,				//Login����Client����������
		PACKET_GC_NEWSPECIAL,				//����һ��SpecialObj���ͻ���
		PACKET_GC_UICOMMAND,				//���Ϳͻ��˵�ĳ���û��ӿ�����
		PACKET_SS_SCENEPLAYERCOUNT,			//�������ݷ�����֪ͨ��Ϸ�������͵�½�����������е�������Ϣ
		PACKET_CG_SPLITITEM,				//�����Ʒ
		//371
		PACKET_GC_SPLITITEMRESULT,			//�����Ʒ
		PACKET_GC_BANKITEMINFO,				//������Ʒ��Ϣ
		PACKET_GC_SPECIAL_OBJ_ACT_NOW,		//���������ʾ��Ч����
		PACKET_GC_SPECIAL_OBJ_FADE_OUT,		//����������������
		PACKET_GC_PLAYERSHOPERROR,			// ����̵�ĳ�����Ϣ
		PACKET_CG_PLAYERSHOPAPPLY,			// ����������봴������̵�
		PACKET_GC_PLAYERSHOPAPPLY,			// ������֪ͨ�ͻ��˴�������̵�������ж���Ϣ
		PACKET_CG_PLAYERSHOPESTABLISH,		// �ͻ�ȷ���ж���Ϣ���ݽ��������
		PACKET_GC_PLAYERSHOPESTABLISH,		// ����̵��Ƿ����ɹ�
		PACKET_CG_PLAYERSHOPACQUIRESHOPLIST,// ����������봴������̵�
		//381
		PACKET_GC_PLAYERSHOPACQUIRESHOPLIST,// ������֪ͨ�ͻ��˴�������̵�������ж���Ϣ
		PACKET_CG_PLAYERSHOPACQUIREITEMLIST,// �ͻ�ȷ���ж���Ϣ���ݽ��������
		PACKET_GW_COMMAND,					//��Ҫ�������ݷ����������GMָ��
		PACKET_CG_PACKUP_PACKET,			//����������
		PACKET_GC_PACKUP_PACKET,			//�����������Ľ��
		PACKET_CG_PLAYERSHOPONSALE,			//֪ͨ��������Ʒ���¼�
		PACKET_GC_PLAYERSHOPONSALE,			//֪ͨ�ͻ�����Ʒ���¼�
		PACKET_CG_PLAYERSHOPBUYITEM,		//֪ͨ������������Ʒ
		PACKET_CG_PLAYERSHOPMONEY,			//֪ͨ��������Ǯ��ȡ
		PACKET_GC_PLAYERSHOPMONEY,			//֪ͨ�ͻ��˽�Ǯ��ȡ
		//391
		PACKET_CG_ASKSETTING,				//����õ��û���������
		PACKET_GC_RETSETTING,				//��ͻ��˷����û���������
		PACKET_CG_MODIFYSETTING,			//�޸��û���������
		PACKET_CG_PLAYERSHOPNAME,			//�޸�����̵����
		PACKET_CG_PLAYERSHOPDESC,			//�޸�����̵������
		PACKET_CG_PLAYERSHOPOPENSTALL,		//���Ż�رչ�̨
		PACKET_GC_PLAYERSHOPOPENSTALL,		//���Ż�رչ�̨
		PACKET_GC_REMOVEPET,				//�޸��û���������
		PACKET_GW_BATCHMAIL,				//����������Ϣ
		PACKET_GC_PETPLACARDLIST,			//���﹫�������
		//401
		PACKET_GW_CITYAPPLYNEWCITY,			//��World�������뽨���µĳ���
		PACKET_WG_CITYAPPLYNEWCITY,			//�ظ�Server�������뽨���µĳ���
		PACKET_WG_CITYERROR,				//�ظ�Server���д�����Ϣ
		PACKET_GC_CITYERROR,				//�ͻ��˵ĳ��д�����Ϣ
		PACKET_GC_CITYNOTIFY,				//�ͻ��˵ĳ���֪ͨ
		PACKET_WG_CITYATTR,					//worldˢ�³�������
		PACKET_GC_CITYATTR,					//server�ѳ�������ˢ���ͻ���
		PACKET_CG_CITYASKATTR,				//������������������
		PACKET_GW_CITYASKINITINFO,			//����ʱ�������������г�ʼ����Ϣ
		PACKET_GW_CITYCLOSE,				//�رճ���
		//411
		PACKET_GW_CITYCLOSESUCCESS,			//�رճ��гɹ�
		PACKET_WG_CITYCLOSE,				//�رճ��гɹ�
		PACKET_WG_CITYCLOSESUCCESS,			//�رճ��гɹ�
		PACKET_WG_CITYINITINFO,				//���������Ϣ
		PACKET_GW_CITYOPT,					//���в���
		PACKET_CG_ISSUEPETPLACARD,			//�������﹫��
		PACKET_CG_STALLFIRSTPAGE,			//��̯�ĳ�ʼҳ��
		PACKET_GC_NEWPET,					//���µĳ�������Լ�����Ұ��Χ
		PACKET_GC_NEWPET_DEATH,				//���������ĳ���
		PACKET_GC_NEWPET_MOVE,				//�����ƶ��ĳ���
		//421
		PACKET_CG_MINORPASSWD,				//��������
		PACKET_GC_MINORPASSWD,				//��������
		PACKET_GC_CHARMOVERESULT,			//�ƶ�ָ���ִ�з���
		PACKET_CG_CHARPOSITIONWARP,			//λ��ƫ���������
		PACKET_CG_PLAYERSHOPSALEOUT,		//�̳��̵�
		PACKET_GC_PLAYERSHOPSALEOUT,		//�̳��̵�
		PACKET_CG_PLAYERSHOPBUYSHOP,		//����˵�
		PACKET_GC_PLAYERSHOPBUYSHOP,		//����˵�
		PACKET_GC_CHARSTOPACTION,			//ֹͣAction�߼�
		PACKET_GC_GUILDAPPLY,				//���뽨������
		//431
		PACKET_CG_GUILDAPPLY,				//���뽨������
		PACKET_CG_GUILDJOIN,				//������빤��
		PACKET_CG_GUILDASKMEMBERINFO,		//�쿴�����Ա
		PACKET_GW_CHANGEMENPAI,				//�ı�����
		PACKET_GW_UPDATETITLE,				//���³ƺ�
		PACKET_GC_PRIVATEINFO,				//������ϣ�ˢ���ͻ��ˣ�
		PACKET_CG_ASKPRIVATEINFO,			//������ϣ�����
		PACKET_CG_APPLYPRIVATEINFO,			//����޸��������
		PACKET_WG_WORLDTIME,				//World->Server ˢ������ʱ��
		PACKET_GC_WORLDTIME,				//Server->Client ˢ������ʱ��
		//441
		PACKET_CG_CHARSIT,					//�������
		PACKET_GC_CHARACTION,				//��һ������
		PACKET_GC_CHARMODIFYACTION,			//���Ŷ�����ִ��
		PACKET_GC_PLAYER_CALLOF,			//����ȥĳ��
		PACKET_CG_PLAYER_CALLOF_RESULT,		//����ȥĳ�صĲ������
		PACKET_GW_CALLOFHUMAN,				//���˵���Ϣ
		PACKET_WG_CALLOFHUMAN,				//���˵���Ϣ
		PACKET_CG_STALL_DEFAULT_PAGE,		//��̯ȱʡҳ���ύ
		PACKET_CG_SETMOODTOHEAD,			//����������ʾ
		PACKET_CG_FINGER,					//�������
		// 451
		PACKET_GC_FINGER,					//�������
		PACKET_GW_FINGER,					//������ҵĽ��
		PACKET_WG_FINGER,					//������ҵĽ��
		PACKET_CG_PLAYERSHOPASKFORRECORD,	//��������̵꽻�׼�¼��������¼
		PACKET_GC_PLAYERSHOPRECORDLIST,		//��¼�б�
		PACKET_CG_PLAYERSHOPPARTNER,		//�������
		PACKET_GC_PLAYERSHOPUPDATEPARTNERS,	//���ºϻ����б�
		PACKET_CG_PLAYERSHOPFAVORITE,		//�����ɾ���ղؼ�
		PACKET_GC_PLAYERSHOPUPDATEFAVORITE,	//�����µ��ղؼ�����ˢ���̵��б�
		PACKET_CG_PLAYERSHOPTYPE,			//����������ı��̵�����
		//461
		PACKET_GC_PLAYERSHOPTYPE,			//֪ͨ�ͻ��˸ı��̵�����
		PACKET_CG_PLAYERSHOPSIZE,			//�����������ı��̵��С
		PACKET_GC_PLAYERSHOPSTALLSTATUS,	//�����������ı�ĳ����̨״̬
		PACKET_GC_CHARDOACTION,				//��ĳ��ɫ��һ��ָ������
		PACKET_CG_CGASKLOCKOBJ,				//��������һ��Obj
		PACKET_GC_ADDLOCKOBJ,				//�����������뱣���Ľ��
		PACKET_GC_NOTIFYGOODBAD,			// ֪ͨ�ƶ�ֵ
		PACKET_CG_CHARMOODSTATE,			// �����¼�
		PACKET_GW_HEARTBEAT,				//������ҵ�������Ϣ��World
		PACKET_GC_USEBALLIMPACT,			//��֪��ͬ�༼���ڴ���ʱ��Ҫ֪ͨ�ͻ��˴����ض�Ч��
		
		//add by liam
		PACKET_GC_NEWOBJITEM,			//���µĶ�����Ʒ�������Լ�����Ұ��Χ
		PACKET_CG_PICKDROPITEM,				//ʰȡ��Ʒ
		PACKET_GC_PICKDROPITEMRESULT,       //ʰȡ���
		PACKET_GC_DETAILEQUIPATT,			//����װ������
		PACKET_CG_EQUIPADDHOLE,				//װ�����
		PACKET_GC_EQUIPADDHOLERESULT,		//��׽��
		PACKET_CG_EQUIPLEVELUP,				//װ������
		PACKET_GC_EQUIPLEVELUPRESULT,		//װ�����ǽ��
		PACKET_CG_EQUIPBIND,				//װ����
		PACKET_GC_EQUIPBINDRESULT,			//װ���󶨽��
		PACKET_CG_SAVEEQUIPSUIT,			//����װ����Ϣ������һ����װ��
		PACKET_GC_SAVEEQUIPSUITRESULT,      //����װ����Ϣ���
		PACKET_CG_EQUIPSUITEXCHANGE,        //һ����װ
		PACKET_GC_EQUIPSUITEXCHANGERESULT,  //һ����װ���
		PACKET_CG_SHOPSPECIALREPAIR,		//����
		PACKET_GC_EQUIPSPECIALREPAIRRESULT,	//���޽��
		PACKET_CG_ITEMCREATE,				//������Ʒ
		PACKET_GC_ITEMCREATERESULT,			//������Ʒ���
		PACKET_CG_EQUIPREMOULD,				//װ������
		PACKET_GC_EQUIPREMOULDRESULT,		//װ��������
		PACKET_CG_EQUIPDECOMPOUND,			//װ�����
		PACKET_GC_EQUIPDECOMPOUNDRESULT,	//װ�������
		PACKET_CG_EQUIPEXCHANGEPAITYPE,		//����װ����������
		PACKET_GC_EQUIPEXCHANGEPAITYPERESULT,//װ���������ͽ��
		PACKET_CG_EQUIPCHANGEPAITYPE,		//����װ��������
		PACKET_GC_EQUIPCHANGEPAITYPERESULT,	//װ�������ͽ��
		PACKET_CG_USEEXTRABAG,				//ʹ����չ����
		PACKET_GC_USEEXTRABAGRESULT,		//ʹ����չ�������
		PACKET_CG_UNEXTRABAG,				//ж����չ����
		PACKET_GC_UNEXTRABAGRESULT,			//ж����չ�������
		PACKET_CG_ASKSUBTRAINSETTING,		//���������Ϣ
		PACKET_GC_RETSUBTRAINSETTING,		//������Ϣ���
		PACKET_CG_ASKBEGINSUBTRAIN,			//����ʼ����
		PACKET_CG_ASKENDSUBTRAIN,			//������ֹ����
		PACKET_GC_ENDSUBTRAINRESULT,		//��ֹ�������
		PACKET_CG_ASKSUBTRAINMODE,			//�������ģʽ
		PACKET_GC_SUBTRAINMODERESULT,		//����ģʽ���
		PACKET_CG_ASKGETSUBTRAINEXP,		//������ȡ��������
		PACKET_GC_GETSUBTRAINEXPRESULT,		//��ȡ����������
		PACKET_CG_CLEANSUBTRAININFO,		//���������Ϣ
		PACKET_GC_SUBTRAINRESULT,			//�������
		PACKET_CG_EQUIPADSORBMAGIC,			//װ����ħ
		PACKET_GC_EQUIPADSORBMAGICRESULT,	//װ����ħ���
		PACKET_CG_EQUIPCANCELMAGIC,			//װ����ħ
		PACKET_GC_EQUIPCANCELMAGICRESULT,	//װ����ħ���


		PACKET_CG_SETTEAMDISTRIBMODE,		//�ͻ�������������ӷ���ģʽ
		PACKET_GC_RETTEAMDISTRIBMODE,		//��ӷ���ģʽ����
		PACKET_GW_SETTEAMDISTRIBMODE,		//����GL������ӷ���ģʽ
		PACKET_WG_RETTEAMDISTRIBMODE,		//��ӷ���ģʽ����mpserver����
		PACKET_CG_ASKTEAMDISTRIBMODE,		//������ӷ���ģʽ
		PACKET_ITEM_END				 = 600,

		PACKET_CG_SKILL_UPGRADE,			//����������
		PACKET_GC_SKILL_UPGRADE,			//�ظ���������
		
		PACKET_GC_SKILL_SUCCESSFUL_USE,		//���ܺϲ���Ϣ
		PACKET_GC_HORSE_LEVEUP,				//��������Ϣ

		PACKET_WL_BILLINGKICK,		//world ����login��billing����������Ϣ
		PACKET_LW_RETPRIZE,		//Login���ظ�World�齱��Ϣ
		PACKET_LW_RETBILLINGSTART,
		PACKET_CL_ASKREGPASSPORT,		//�ͻ�������ע��SOHU��PASSPORT
		PACKET_LW_RETBILLINGEND,		//login ����world billingend ���
		PACKET_LW_RETBILLINGKEEP,
		PACKET_WL_BILLINGKEEP,
		PACKET_WL_BILLINGEND,
		PACKET_WL_BILLINGSTART,
		PACKET_WL_CHANGENAME,
		PACKET_LW_CHANGENAME,		//ACKET_LW_CHANGENAME,
		PACKET_CL_REGNEWLICENSE,
		PACKET_CL_ASKCREATECODE,
		PACKET_CL_REGSAFESIGN,
		PACKET_LC_RETCREATECODE,
		PACKET_CL_ASKLOGINCODE,
		PACKET_WL_NOTIFYUSER,
		PACKET_KEY_EXCHANGE,
		PACKET_WL_ASKPRI,
		PACKET_LW_NOTIFYUSER,
		PACKET_GWLWG_COSTLOG,
		PACKET_LC_RETLOGINCODE,
		PACKET_WL_ASKPRIZE,

        //�ƺ����
        PACKET_GW_CHARUPDATECURTITLE,		// ת����WORLD�����ƶ���ҵ��ض��ƺ����ֵ���Ϣ
        PACKET_WG_CHARUPDATECURTITLE,       // ֪ͨMapServer�����ƶ���ҵ��ض��ƺ�����

        //����
		PACKET_CG_REQUEST_KINDOM,			//�����ι���
		PACKET_CG_COUNTRY_APPOINT,			//���������Ա
		PACKET_CG_COUNTRY_REMOVE,			//���������Ա

        PACKET_GW_REQUEST_KINDOM,			//�����ι���
        PACKET_GW_COUNTRY_APPOINT,			//���������Ա
		PACKET_GW_COUNTRY_REMOVE,			//���������Ա

        PACKET_WG_COUNTRYRETURN,			//���ع��Ҳ������
        PACKET_GC_COUNTRYRETURN,			//���ع��Ҳ������

		PACKET_CG_TOP_LIST,					//���а�
		PACKET_GC_TOP_LIST,					//���а�
		PACKET_GW_TOP_LIST,					//���а�
		PACKET_WG_TOP_LIST,					//���а�

		PACKET_CG_MAP_INFO,					//������ת�ĵ�ͼ��Ϣ

		PACKET_GW_ATTRIBUP,					// ������Ա仯

        //����
        PACKET_CG_COUNTRY_INFO_REQUEST,		//������Ϣ����
        PACKET_GC_COUNTRY_INFO_CONFIRM,		//������Ϣ�ظ�

        PACKET_GW_COUNTRY_INFO_REQUEST,		//������Ϣ����
        PACKET_WG_COUNTRY_INFO_CONFIRM,		//������Ϣ�ظ�


        PACKET_GC_EXPANDBANKSIZE,			// ��չ����Size
        PACKET_GC_BANKSIZE,			        // ���е�ǰSize

		PACKET_CG_REGISTER_TEAM_RECRUIT,	// �ӳ��������ն�Ա��Ϣ
		PACKET_GW_REGISTER_TEAM_RECRUIT,	// �ӳ��������ն�Ա��Ϣ

		PACKET_CG_ASK_TEAM_RECRUIT,			// ������������б�
		PACKET_GW_ASK_TEAM_RECRUIT,			// ������������б�
		PACKET_WG_RET_TEAM_RECRUIT,			// ������������б�
		PACKET_GC_RET_TEAM_RECRUIT,			// ������������б�

		// by soongxl 
		PACKET_CG_MODIFY_PK_MODE, //�ͻ���֪ͨ������pkģʽ����

		//by zheng
		//���������
		PACKET_CG_OPENBOX,				//�ͻ��������������򿪱���
		PACKET_GC_OPENBOX,				//���������ظ��ͻ��˱����е���Ʒ�б�
		PACKET_CG_OPENBOXMANIPULATE,	//����������еĲ���
		PACKET_GC_OPENBOXMANIPULATE,	//���ڲ����ķ���


		//�������
		PACKET_CG_ISSUEHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]�����������
		PACKET_CG_CANCELHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]ȡ���������
		PACKET_CG_SEARCHHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]�����������
		PACKET_CG_QUERYHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]��ѯ�������
		PACKET_GC_RETURNHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]�����������
		PACKET_GW_ISSUEHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]�����������	
		PACKET_GW_CANCELHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]ȡ���������
		PACKET_GW_SEARCHHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]�����������
		PACKET_GW_QUERYHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]��ѯ�������
		PACKET_GW_HORSECHANGESTATE,			//  [6/4/2008 wuwenbin]���״̬�仯���绹ͯ��������
		PACKET_WG_RETURNHORSEFINDMATEINFO,	//  [6/4/2008 wuwenbin]�����������

		PACKET_WG_OPERATE_RESULT,		// [6/4/2008 wuwenbin] �����ķ��� 
		PACKET_GW_ASKDETAILATTRIB,		// [6/4/2008 wuwenbin] �鿴������ҵ���ϸ��Ϣ

		PACKET_GW_ASKOTHERONLINEATTRIB_HUMAN,		//  [6/4/2008 wuwenbin]�鿴������ҵ���ϸ��Ϣ
		PACKET_WG_DETAILOTHERONLINEATTRIB_HUMAN,	//  [6/4/2008 wuwenbin]����������ҵ���ϸ��Ϣ
		PACKET_WG_DETAILOTHERONLINEEQUIPLIST_HUMAN,	//  [6/4/2008 wuwenbin]����������ҵ�װ����Ϣ
		PACKET_WG_DETAILOTHERONLINEATTRIB_HORSE,	//  [6/4/2008 wuwenbin]����������ҵ���˵���ϸ��Ϣ
		PACKET_CG_ASKOTHERONLINEDETAILATTRIB,		//  [6/4/2008 wuwenbin]�鿴������ҵ���ϸ��Ϣ
		PACKET_GW_REFRESHDATATOWORLD,				//  [6/4/2008 wuwenbin]ˢ�����ݵ�World

		PACKET_CG_SEARCHONLINEUSER,		//  [6/4/2008 wuwenbin]�鿴�������
		PACKET_GW_SEARCHONLINEUSER,		//  [6/4/2008 wuwenbin]�鿴�������
		PACKET_WG_RETURNONLINEUSER,		//  [6/4/2008 wuwenbin]�����������
		PACKET_GC_RETURNONLINEUSER,		//  [6/4/2008 wuwenbin]�����������

		PACKET_WG_ASKOTHERONLINEATTRIB_HUMAN,		//  [6/4/2008 wuwenbin]�鿴������ҵ���ϸ��Ϣ
		PACKET_GW_DETAILOTHERONLINEATTRIB_HUMAN,	//  [6/4/2008 wuwenbin]����������ҵ���ϸ��Ϣ
		PACKET_GW_DETAILOTHERONLINEEQUIPLIST_HUMAN,	//  [6/4/2008 wuwenbin]����������ҵ�װ����Ϣ
		PACKET_GW_DETAILOTHERONLINEATTRIB_HORSE,	//  [6/4/2008 wuwenbin]����������ҵ���˵���ϸ��Ϣ


        //����_v2
        PACKET_CG_COUNTRY_COLLECT,		    //����ļ����Ϣ����
        PACKET_GC_COUNTRY_COLLECT,		    //����ļ����Ϣ�ظ�

        PACKET_GW_COUNTRY_COLLECT,		    //����ļ����Ϣ����
        PACKET_WG_COUNTRY_COLLECT,		    //����ļ����Ϣ�ظ�

        PACKET_CG_COUNTRY_DONATIVE,		    //���
        PACKET_GW_COUNTRY_DONATIVE,		    //���

        PACKET_CG_COUNTRY_FETCH,		    //����
        PACKET_GC_COUNTRY_FETCH,		    //����

        PACKET_GW_COUNTRY_FETCH,		    //����
        PACKET_WG_COUNTRY_FETCH,		    //����

        PACKET_GW_COUNTRY_AP,	    	    //�����ж���

        PACKET_GW_COUNTRY_DELATE_REQUEST,	//��������
        PACKET_WG_COUNTRY_DELATE_CONFIRM,	//����ȷ��

        PACKET_CG_COUNTRY_DELATE,		    //������ͶƱ
        PACKET_GW_COUNTRY_DELATE,		    //������ͶƱ

        PACKET_CG_COUNTRY_VOTEON,		    //����ͶƱ
        PACKET_GW_COUNTRY_VOTEON,		    //����ͶƱ

        PACKET_GC_COUNTRY_DELATE_BEGIN,	    //������ʼ

        PACKET_GW_COUNTRY_VOTE_REQUEST,	    //ͶƱ����
        PACKET_WG_COUNTRY_VOTE_CONFIRM,	    //ͶƱȷ��

		PACKET_GC_COUNTRY_VOTE_SHOW,	    //ͶƱ��

		PACKET_GW_TEAM_LEADER_DIE,			//�ӳ�������֪ͨGL

		PACKET_GC_MISSIONHAVEDONEFLAG,	//�޸��������
		
		// ����¼��� add by gh 2010/05/11
		PACKET_GC_SOUXIA_LIST,			//�������������б�	
		PACKET_CG_SOUXIASKILL_ADD,		//��������¼��������
		PACKET_GC_SOUXIASKILL_ADD,		//����¼���������ɹ���Ҫ���µ�ǰ����¼
		PACKET_CG_SOUXIADISMANTLE,		//������¼	
		PACKET_GC_SOUXIADISMANTLE,		//���������صĲ����
		PACKET_CG_SOUXIASEW,			//��ϒ���¼
		PACKET_GC_SOUXIASEW,			//���������صķ�Ͻ��
		PACKET_CG_SOUXIAREFINING,		//��������¼
		PACKET_GC_SOUXIAREFINING,		//���������ص��������
		PACKET_CG_SOUXIADISCARD,		//ɾ������¼
		PACKET_GC_SOUXIADISCARD,		//ɾ������¼���
		PACKET_GC_SOUXIA_UPDATE,		//����¼�����ݸ�����Ҫ�������ٻ��������ٻ����ܵĴ�Ϊ0���λ�ñ仯
		PACKET_GC_SOUXIASKILL_UPDATE,	//�����ٻ��������ٻ����ܵĴ�������

		// ��ɫʹ�ü��ܣ�����ͷ��ð�������� 20100602 BLL
		PACKET_GC_CHAR_USESKILL,	

		PACKET_CG_EQUIP_REFINING,		//����װ��	
		PACKET_GC_EQUIP_REFINING,		//����װ�����


		PACKET_GC_BUSADDPASSENGER,		//����һ���˿͵�BUS
		PACKET_GC_BUSMOVE,
		PACKET_GC_BUSREMOVEPASSENGER,
		PACKET_GC_BUSSTOPMOVE,
		PACKET_GC_NEWBUS,
		PACKET_GC_NEWBUS_MOVE,
		PACKET_GC_BUSREMOVEALLPASSENGER,

		// ���̯λ���У��Ӷ�ʹ������ҿ����Լ�̯λ	20100706 BLL
		PACKET_CG_STALLOPENPUBLIC,
		PACKET_GC_STALLOPENPUBLIC,

		// ������ [7/7/2010 �¾���]
		PACKET_CG_MANIPULATEMOUNT,		    //��˲���	
		PACKET_GC_MANIPULATEMOUNTRESULT,	//��˲������
		PACKET_CG_ASKDETAILMOUNTLIST,	    //�����������ϸ��Ϣ
		PACKET_GC_DETAILMOUNTLIST,          //�����������ϸ��Ϣ

		PACKET_MAX	//��Ϣ���͵����ֵ
	};

	enum BILLPACKET_DEFINE
	{
		BPACKET_LB_CONNECT			= 0xA0,
		BPACKET_BL_RETCONNECT		/*= 0xA0*/,
		BPACKET_LB_KEEPLIVE			/*= 0xA1*/,
		BPACKET_BL_RETKEEPLIVE		/*= 0xA1*/,
		BPACKET_LB_ACCCHECK			/*= 0xA2*/,
		BPACKET_BL_RETACCCHECK		/*= 0xA2*/,
		BPACKET_LB_BILLINGSTART		/*= 0xA3*/,
		BPACKET_BL_RETBILLINGSTART	/*= 0xA3*/,
		BPACKET_LB_BILLINGEND		/*= 0xA4*/,
		BPACKET_BL_RETBILLINGEND	/*= 0xA4*/,
		BPACKET_LB_BILLINGKEEP		/*= 0xA6*/,
		BPACKET_BL_RETBILLINGKEEP	/*= 0xA6*/,
		//���Э���Ƿ����
		BPACKET_LBL_REGLICENSE		/*= 0xA7*/,
		BPACKET_LB_KICKALL			/*= 0xA9*/,
		BPACKET_BL_RETKICKALL		/*= 0xA9*/,
		//�ʺ�ע��
		BPACKET_LB_REGPASSPORT		/*= 0xF1*/,
		BPACKET_BL_RETREGPASSPORT	/*= 0xF1*/,
		//���ֿ�
		BPACKET_LBL_NEWUSERCARD		/*= 0xC1*/,
		//�³齱��
		BPACKET_LBL_NEWPRIZE		/*= 0xC3*/,
		//CD-KEY��ȡ
		BPACKET_LB_PRIZE			/*= 0xC4*/,
		BPACKET_BL_PRIZE			/*= 0xC4*/,
		//Ԫ�����Ѽ�¼
		BPACKET_LBL_COSTLOG			/*= 0xC5*/,
		//Ԫ������
		BPACKET_LB_ASKBUY			/*= 0xE1*/,		
		BPACKET_BL_ASKBUY			/*= 0xE1*/,
		//������ѯ
		BPACKET_LB_ASKPOINT			/*= 0xE2*/,
		BPACKET_BL_ASKPOINT			/*= 0xE2*/,
		//��Ʒ��ѯ
		BPACKET_LBL_CHECKPRIZE		/*= 0xE4*/,
		//��������
		BPACKET_LBL_ASKMIG			/*= 0xE5*/,
		PACKET_LB_ASKAUTH,					//Login ����BillingSystem��֤
		PACKET_BL_RETAUTH,					//BillingSystem ������֤���

		PACKET_LB_ASKREGAUTH,		//����passport��֤loginplayermanager -> servermanager
		PACKET_LB_ASKREGLICENSE,

		BPACKET_MAX = 254
	};

};


#endif

