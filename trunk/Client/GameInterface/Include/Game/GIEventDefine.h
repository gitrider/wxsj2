
/**	GIEventDefine.h
 *
 *	���ܣ�	�¼�����
 *
 *	������  Nick 080314
 *			��lua�ű����͵���Ϣ��ö�ٶ��壬
 *          lua�ű�ʹ������ö��ֵ��Ӧ��������
 *			���������ö��ֵһ��Ҫ�ӵ�����Ա���lua�ű���Ϣ����
 *	
 *			Nick 080327
 *			�����ڽű���PushEvent����GAME_EVENT_ID��Ӧ���ַ����Ĺ��ܡ�
 *			���Ը�ö�ٿ���������ˡ�
 *	
 *
 *	�޸ļ�¼��
 *			080305	GE_GUILD_INVITE_OBJECT
 *			080306	GE_GUILD_NOTIFY_INTIVE
 *					GE_GUILD_INTIVE_CONFIRM
 *					GE_GUILD_MODIFY_TITLE
 *			080308	GE_GUILD_CREATE_FAMILY
 *					GE_GUILD_CREATE_FAMILY_CONFIRM
 *					GE_GUILD_DESTORY_FAMILY_CONFIRM
 *			080314	GE_OPEN_FRIEND_SEARCH
 *			080318	GE_FRIEND_CREATE_GROUP
 *			080319	GE_OPEN_FRIEND_SEARCH_REASON
 *			080320	GE_OPEN_BUY_CONFIRM
 *			080320	GE_OPEN_SALE_CONFIRM
 *			080324	GE_QUICK_CHANGE_NAME
 *			080325	GE_ADD_GROUP
 *			080327	GE_ITEM_LINK
 *			080331	GE_SHOW_TITLE
 *					GE_UPDATE_TITLE
 *			080402	GE_SHOW_FRIEND_INFO
 *			080407	GE_PERSONAL_MESSAGE		// ������Ϣ
 *					GE_SYSTEM_MESSAGE		// ϵͳ��Ϣ
 *					GE_SHOW_SYSTEM_PROMPT
 *			080410	GE_SHOW_SUPERTOOLTIP_TITLE,		// ��ʾ�ƺ���ʾ
 *					GE_RICH_SAFE_GUARD,				// ������Χ����
 *					GE_RICH_SAFE_TIME,				// ��ȫʱ�޵�������
 *					GE_RICH_SET_PASSWORD,			// �������롢�޸��������
 *					GE_RICH_SETUP,					// �Ʋ������������ý���
 *					GE_UNITE_GF_SHOW
 *			080418	GE_INPUT_PASSWD_COMMAND
 *			080331	GE_OPEN_DAY_TIPS
 *					GE_UPDATE_DAYTIPS_TIME	
 *					GE_UPDATE_DAY_TIPS
 *					GE_UPDATE_CALENDAR
 *					GE_OPEN_TOP_LIST
 *			080410	GE_UPDATE_TOP_LIST
 *
 *			080417	GE_OPEN_MISSION_SCOUT
 *					GE_UPDATE_MISSION_SCOUT
 *			080331	GE_OPEN_DAY_TIPS
 *					GE_UPDATE_DAYTIPS_TIME	
 *					GE_UPDATE_DAY_TIPS
 *					GE_UPDATE_CALENDAR
 *					GE_OPEN_TOP_LIST
 *			080410	GE_UPDATE_TOP_LIST
 *
 *			080417	GE_OPEN_MISSION_SCOUT
 *					GE_UPDATE_MISSION_SCOUT
 *			080421	GE_OPEN_BANK_EVENT
 *					GE_QUEST_INPUT_PASSWD_COMMAND
 *					GE_PACKET_OPEN_SALE
 *					GE_COUNTRY_MANAGER_INFO,		// ������Ϣ
 *					GE_COUNTRY_MANAGER_ADJUST_AUTH,	// ���ҹ�ְ����
 *			080422	GE_INPUT_COUNTRY_DLG
 *					GE_COUNTRY_KING_CREATE_CONFIRM
 *			080425	GE_COUNTRY_MANAGER_CREATE_KING
 *					GE_COUNTRY_MANAGER_REQUEST_INFO
 *
 */

#pragma once



/// 
enum GAME_EVENT_ID
{
	GE_APPLICATION_INITED,			/// ��Ϸ�����ʼ��
	GE_ON_SCENE_TRANS,				/// ����ת��
	GE_SCENE_TRANSED,				/// ����ת�ƺ�
									/// arg0- С��ͼ������
									//
	GE_ON_SCENE_UPDATED,			/// ��������(һ��������ҳ���)
									/// arg0- С��ͼ������
									//
	GE_ON_SERVER_TRANS,				/// ������ת��
	GE_PLAYER_ENTER_WORLD,			/// ��������
	GE_PLAYER_LEAVE_WORLD,			/// �˳�����
	GE_TEAM_CHANG_WORLD,			/// ���Ѹı䳡����
	GE_SKILL_CHANGED,				/// ��ҵ�ĳ��ܷ����ı�,
									// arg0 - ����id
	GE_PACKAGE_ITEM_CHANGED,		/// �����е�ĳ����Ʒ�����ı�
									// arg0 - ��Ʒ�ڰ����еı��

	GE_MAINTARGET_CHANGED,			/// ��ѡ��������
									// arg0 - ����ServerId

	
	GE_MAINTARGET_OPEND,			// ��target ����ѡ���Ƕ��ѵ������ �򿪴���
									

	GE_TOGLE_SKILL_BOOK,			// ��/�رռ�����
	GE_CLOSE_SKILL_BOOK,			// �رռ�����

	GE_TOGLE_PET_PAGE,				// �򿪳������
	GE_TOGLE_PETATTRIBUTE_PAGE,		//�򿪳�����ϸ���Խ���
	GE_UPDATE_PETATTRIBUTE_PAGE,	//ˢ�³�����ϸ���Խ���
	GE_PETPAGE_CLOSE,		//�������ر���
	GE_TOGLE_LIFE_PAGE,				// ������ܽ���
	GE_TOGLE_COMMONSKILL_PAGE,		// ����ͨ���ܽ���
	GE_SKILL_UPDATE,				//��Ҽ������ݷ����ı�

	GE_TOGLE_CONTAINER,				// ��/�رհ���

	//-----------------------------------------------------
	//��ɫ����
	GE_UNIT_HP,						//ĳ����Ԫ��HP�����仯
									// arg0 - ��Ԫ����
									//		"player"	- ����Լ�
									//		"target"	- ��Ŀ��
									//		"pet"		- �Լ��ĳ���
									//		"party*"	- ����(1-4)
									//		"partypet*"	- ���ѵĳ���(1-4)
									//		...
	GE_UNIT_MP,						//ĳ����Ԫ��MANA�����仯
									// arg0 - ��Ԫ����(ͬ��)
	GE_UNIT_PENUMA,					//ĳ����Ԫ��Ԫ�������仯
									// arg0 - ��Ԫ����(ͬ��)

	GE_UNIT_RAGE,					//ĳ����Ԫ��ŭ�������仯
									// arg0 - ��Ԫ����(ͬ��)

//	GE_UNIT_XINFA,					//ĳ����Ԫ���ķ��ȼ������仯
									// arg0 - ��Ԫ����(ͬ��)
									// arg1 - �ķ�����

	GE_UNIT_EXP,					//ĳ����Ԫ�ľ���ֵ�����仯
									// arg0 - ��Ԫ����(ͬ��)

	GE_UNIT_MONEY,					//ĳ����Ԫ��Ǯ�����仯
									// arg0 - ��Ԫ����(ͬ��)

	GE_UNIT_RACE_ID,				// ���ݱ��е�ID
	GE_UNIT_NAME,					// ��ɫ�����֣��ӷ���������
	GE_UNIT_CAMP_ID,				// ��ӪID
	GE_UNIT_LEVEL,					// �ȼ�
	GE_UNIT_MOVE_SPEED,				// �ƶ����ٶ�
	GE_UNIT_FIGHT_STATE,			// ս��״̬	
	GE_UNIT_MAX_EXP,				// ���EXP
	GE_UNIT_TITLE,					// �ƺ�
	GE_UNIT_STRIKEPOINT,			// ������
	GE_UNIT_RELATIVE,				// ��������

	//-------------
	//һ��ս������
	GE_UNIT_STR,					// �⹦
	GE_UNIT_SPR,					// �ڹ�
	GE_UNIT_CON,					// ��
	GE_UNIT_INT,					// ����
	GE_UNIT_DEX,					// �ǻ�
	GE_UNIT_POINT_REMAIN,			// ʣ����������

	//-------------
	//����ս������
	GE_UNIT_ATT_NEAR,				// ����
	GE_UNIT_DEF_NEAR,				// ����
	GE_UNIT_ATT_FAR,				// Զ��
	GE_UNIT_DEF_FAR,				// Զ��

//	GE_UNIT_ATT_PHYSICS,			// ��������			
	GE_UNIT_ATT_MAGIC_NEAR,				// �����ڹ�			
	GE_UNIT_ATT_MAGIC_FAR,				// Զ���ڹ�			
//	GE_UNIT_DEF_PHYSICS,			// ���������			
	GE_UNIT_DEF_MAGIC_NEAR,			// �����ڹ�������	
	GE_UNIT_DEF_MAGIC_FAR,			//Զ���ڹ�������
	GE_UNIT_MAX_HP,					// ���������			
	GE_UNIT_MAX_MP,					// ���ħ����		
	GE_UNIT_MAX_PNEUMA,				// ���Ԫ����	
	GE_UNIT_MAX_RAGE,				// ���ŭ����
	GE_UNIT_HP_RE_SPEED,			// HP�ָ��ٶ�  ��/��	
	GE_UNIT_MP_RE_SPEED,			// MP�ָ��ٶ�  ��/��	
	GE_UNIT_TOUGHNESS,				// ����
	GE_UNIT_HIT,					// ������				
	GE_UNIT_MISS,					// ������				
	GE_UNIT_CRIT_RATE,				// ������				
	GE_UNIT_ATT_SPEED,				// �����ٶ�	
	GE_UNIT_ATT_GOLD,				// ������	
	GE_UNIT_DEF_GOLD,				// �����	
	GE_UNIT_ATT_FIRE,				// �𹥻�	
	GE_UNIT_DEF_FIRE,				// �����	
	GE_UNIT_ATT_WATER,				// ˮ����	
	GE_UNIT_DEF_WATER,				// ˮ����	
	GE_UNIT_ATT_WOOD,				// ľ����	
	GE_UNIT_DEF_WOOD,				// ľ����
	GE_UNIT_ATT_SOIL,				// ������	
	GE_UNIT_DEF_SOIL,				// ������
	GE_UNIT_VIGOR,					// ����ֵ
	GE_UNIT_MAX_VIGOR,				// ����ֵ����
	GE_UNIT_ENERGY,					// ����ֵ
	GE_UNIT_MAX_ENERGY,				// ����ֵ����

	GE_UNIT_MENPAI,					// ���ɺ�
	GE_UNIT_HAIR_MESH,				// -> DBC_CHARACTER_HAIR_GEO
	GE_UNIT_HAIR_COLOR,				// ͷ����ɫ
	GE_UNIT_FACE_MESH,				// -> DBC_CHARACTER_HEAD_GEO		char_head.tab
	GE_UNIT_FACE_IMAGE,				// ͷ����Ϣ�ı�
	GE_UNIT_EQUIP_VER,				// ��ɫ��װ��״̬�汾�ţ����ںͷ�����ͬ�����ݵ�����
	GE_UNIT_EQUIP,					// װ����(������)
	GE_UNIT_EQUIP_WEAPON,			// ����


	//--------------------------------------------------------------------------------

	GE_SHOW_CONTEXMENU,				//��ʾ�Ҽ��˵�
									// arg0 - �˵���
									//		"player"		- ����Լ�
									//		"other_player"  - �������
									//		"npc"			- npc
									// arg1 - ���λ��_x
									// arg2 - ���λ��_y

	GE_TOGLE_COMPOSE_WINDOW,		// �򿪹رպϳɽ���

	GE_TOGLE_CONSOLE,				// ��ʾ����̨

	GE_ON_SKILL_ACTIVE,				// ĳ�����ܿ�ʼʹ��
									// arg0 - ����ID

	GE_MAINTARGET_BUFF_REFRESH,		// ���½�ɫ��buff�����¼�

	GE_POSITION_MODIFY,				// �����λ�øı���

	GE_CHAT_MESSAGE,				// �õ�������Ϣ
									/*
									|  arg0 - ��������
									|		"near"		- �������
									|		"scene"		- ����
									|		"sys"		- ϵͳ
									|		"team"		- ����
									|		"guild"		- ���
									|		"user"		- �Խ�
									|		"private"	- ˽��
									|
									|  arg1 - ˵���ߵ�����
									|
									|  arg2 - ˵������
									|
									*/

	GE_CHAT_CHANNEL_CHANGED,		//����Ƶ�������ı�
	GE_CHAT_CHANGE_PRIVATENAME,		//�޸������е��������
	GE_CHAT_CHANGE_COPY,				//���쳬���ӵ����˵��еĸ�������
	GE_CHAT_TAB_CREATE,				//��������ҳ��
	GE_CHAT_TAB_CREATE_FINISH,		//��������ҳ�����
	GE_CHAT_TAB_CONFIG,				//��������ҳ��
	GE_CHAT_TAB_CONFIG_FINISH,		//��������ҳ�����
	GE_CHAT_FACEMOTION_SELECT,		//�������ѡ��
	GE_CHAT_TEXTCOLOR_SELECT,		//����������ɫѡ��
	GE_CHAT_CONTEX_MENU,			//������صĿ�ݲ˵�
	GE_CHAT_ACTSET,					//���춯������
	GE_CHAT_ACT_SELECT,				//���춯�����������ʾ
	GE_CHAT_ACT_HIDE,				//���춯���������رգ���Ϊ�����������Ƚ����⣬��Ҫ֧����ק��
	GE_CHAT_PINGBI_LIST,			//�������ν���
	GE_CHAT_PINGBI_UPDATE,			//�����б����
	GE_CHAT_ADJUST_MOVE_CTL,		//֪ͨ���촰�ڵ�����С
	GE_CHAT_INPUTLANGUAGE_CHANGE,	//���뷨���
	GE_CHAT_SHOWUSERINFO,			//����鿴�����Ϣ����
	GE_CHAT_LOAD_TAB_CONFIG,		//����ҳ��������Ϣ

	GE_ACCELERATE_KEYSEND,			//���̿�ݼ�����
	
	GE_LOOT_OPENED,					// ���Ӵ�
	GE_LOOT_SLOT_CLEARED,			// �����е�ĳ��λ�����
									// arg0 - �����е�λ��

	GE_LOOT_CLOSED,					// ���ӹر�

	GE_PROGRESSBAR_SHOW,			// ��������ʾ
	GE_PROGRESSBAR_HIDE,			// ����������
	GE_PROGRESSBAR_WIDTH,			// ��������ȵ���
									// arg0 - �������İٷֱ� 1 < x < 100

	GE_CHANGE_BAR,					//�ı乤�����ϵ�Action
									// arg0 - Bar����
									// arg1 - Index
									// arg2 - ActionItem ID

	GE_COLSE_SECOND_MENU,			/// �ر����ж����˵�
	GE_TOGLE_MISSION,				/// �����������
	GE_UPDATE_MISSION,				/// ˢ�������б�
	GE_REPLY_MISSION,				/// �ύ�������
	GE_UPDATE_REPLY_MISSION,		/// ˢ���ύ�������

	GE_TOGLE_LARGEMAP,				// �رմ򿪴��ͼ
	GE_TOGLE_CLOSELARGEMAP,			// �رմ��ͼ
	GE_TOGLE_SCENEMAP,				// �رմ򿪳�����ͼ
	GE_TOGLE_CLOSESCENEMAP,			// �رճ�����ͼ
	GE_OPEN_MINIMAP,				// ��С��ͼ
	GE_OPEN_MINIMAPEXP,				// ����չС��ͼ

	GE_QUEST_EVENTLIST,				//�򿪺�npc��һ�ζԻ�ʱ�Ŀ�ִ�нű��б�
	GE_QUEST_INFO,					//�򿪺�npc�ڶ��ζԻ�(�ڽ�����ʱ��������������Ϣ���ȴ�����)
	GE_QUEST_REGIE,					//��������
	GE_QUEST_CONTINUE_DONE,			//����������ٴκ�npc�Ի������õ�������������Ϣ��(�������)
	GE_QUEST_CONTINUE_NOTDONE,		//����������ٴκ�npc�Ի������õ�������������Ϣ��(����δ���)
	GE_QUEST_AFTER_CONTINUE,		//���������֮�󡱣���Ʒѡ�����
	GE_QUEST_TIPS,					//��Ϸ�����е������չ��ʾ��Ϣ
	GE_QUEST_INPUT_PASSWD_COMMAND,	// ͨ�������������Ʒ����

	GE_TOGLE_COMPOSEITEM,			// ��/�ر� �ϳɽ���
									// arg0 - ����ܵ�����
	GE_TOGLE_COMPOSEGEM,			// ��/�ر� ��ʯ�ϳɽ���
									// 
	GE_ON_LIFEABILITY_ACTIVE,		// ĳ������ܿ�ʼʹ��
									// arg0 - ����ID
									// arg1 - �䷽
									// arg2 - ƽ̨
	GE_NEW_DEBUGMESSAGE,			//�µ�debug string����������Ļ����listbox��ʾ��

	//-------------------------------------------------------------------------------------------------------
	GE_QUICK_CHANGE_EQUIP,			// �򿪹ر�һ����װ����
	GE_UPDATE_QUICK_CHANGE,			// ����һ����װ����
	GE_QUICK_CHANGE_SUCCESS,		// һ����װ�ɹ�
	GE_UPDATE_QUICK_CHANGE_EQUIPNAME,		//����һ����װ��װ������


	//-------------------------------------------------------------------------------------------------------
	// �������Խ���
	GE_UPDATE_EQUIP,				// ����װ��
	GE_OPEN_BOOTH,					// �򿪻�����
	GE_CLOSE_BOOTH,					// �رջ�����
	GE_MANUAL_ATTR_SUCCESS_EQUIP,	// �ֶ��������Գɹ�.
	GE_CUR_TITLE_CHANGEED,			// ��ǰ����ĳƺŸı���.

	GE_UPDATE_BOOTH,				// ���ܸ���

	GE_OPEN_CHARACTOR,				// ������������
	GE_OPEN_EQUIP,					// ��װ����
	GE_TOGLE_JOINSCHOOL,			// �򿪼������ɵĶԻ���



	GE_OPEN_SOUXIA,			//������¼����
	GE_CLOSE_SOUXIA,			//�ر�����¼����
	GE_UPDATE_SOUXIA_TAB,

	GE_OPEN_SOUXIASEW,				//������¼��Ͻ���
	GE_OPEN_SOUXIACOMPLEX,		//������¼��������
	GE_OPEN_SOUXIADESTORY,		//������¼���ٽ���

	GE_OPEN_SOUXIADISMANTLE,	//������¼������
	GE_ADD_SOUXIADISMANTLE_ITEM,	//����¼�������а�������¼����
	GE_ADD_SOUXIADISMANTLE_DIS,		//����¼�������а�������¼������
	GE_SHOW_SOUXIADISMANTLE_BUILD,	//����¼����������ʾ���ɵ���

	GE_ADD_SOUXIADESTORY_ITEM,			//����¼���ٽ����а�������¼����

	GE_ADD_SOUXIACOMPLEX_ITEM1,			//����¼���������а�������¼����1
	GE_ADD_SOUXIACOMPLEX_ITEM2,			//����¼���������а�������¼����2
	GE_ADD_SOUXIACOMPLEX_DIS,			//����¼���������а�������¼��������
	GE_SHOW_SOUXIACOMPLEX_BUILD,		//����¼������������ʾ���ɵ���Ʒ

	GE_ADD_SOUXIASEW_ITEM1,			//����¼��Ͻ����а�������¼����
	GE_ADD_SOUXIASEW_DISITEM,			//����¼��Ͻ����а�������¼��ҳ����
	GE_ADD_SOUXIASEW_DIS,			//����¼��Ͻ����а�������¼��ϵ���
	GE_SHOW_SOUXIASEW_BUILD,	//����¼��Ͻ�������ʾ���ɵ���Ʒ
	
	// Nick 2007.11.22
	GE_OPEN_MAIN_FUNC_BTN,			// �����ܰ�ť�����ڴ�8�������ܰ�ť

 
	GE_OPEN_ADD_HOLE,				//�򿪴�׽���
	GE_UPDATE_ADD_HOLE,				//���´�׽���
	GE_ADD_HOLE_SUCCESS,			//�ɹ����
    
	GE_OPEN_BANG_DING,				//�򿪰󶨽���				
	GE_UPDATE_BANG_DING,			//���°󶨽���		     
	GE_ADD_BANG_DING_SUCCESS,		//�󶨳ɹ�

	GE_OPEN_DA_XING,				//�򿪴��ǽ���	
	GE_UPDATE_DA_XING,				//���´��ǽ���	
	GE_OPEN_DA_XING_SUCCESS,		//���ǳɹ�	

	GE_OPEN_SP_REPAIR,				//�����޽���	
	GE_UPDATE_SP_REPAIR,			//�������޽���	
	GE_OPEN_SP_REPAIR_SUCCESS,		//���޳ɹ�	
	GE_OPEN_REPAIR_SURE,			//��������������ʾ�Ի���

	GE_OPEN_SYNTHESIZE,				//�򿪺ϳ������Ի���
	GE_CLOSE_SYNTHESIZE,			//�رպϳ������Ի���

	GE_OPEN_EQUIP_REWORK,			//�򿪸���Ի���
	GE_CLOSE_EQUIP_REWORK,			//�رո���Ի���

	GE_OPEN_EQUIP_DISASS,			//�򿪲��װ���Ի���
	GE_UPDATE_EQUIP_DISASS,         //����
	GE_CLOSE_EQUIP_DISASS,			//�رղ��װ���Ի���
									
	GE_UPDATE_CONTAINER,			// ����״̬����
	GE_IMPACT_SELF_UPDATE,			// �Լ�����Ч���� 
	GE_IMPACT_SELF_UPDATE_TIME,		// �Լ�����Чʱ��ĸ��� 
	GE_TOGLE_SKILLSTUDY,			// �����ķ���ѧϰ������
	GE_SKILLSTUDY_SUCCEED,			// �����ķ�ѧϰ�ɹ�

	GE_TOGLE_ABILITY_STUDY,			// �����ѧϰ����
	GE_OPEN_AGNAME,					// ��ѡ��ƺŽ���
	GE_CLOSE_AGNAME,				// �ر�ѡ��ƺŽ���
	
	GE_TOGLE_BANK,					// �����н���
	GE_UPDATE_BANK,					// �������н���
	GE_TOGLE_INPUT_MONEY,			// ������Ǯ�Ľ���
	GE_CLOSE_INPUT_MONEY,			// �ر������Ǯ����
	GE_OPEN_BANK_EVENT,				// �ж��������뱣��

	GE_RECEIVE_EXCHANGE_APPLY,		// �յ���������
	GE_STOP_EXCHANGE_TIP,			// ֹͣ����������ʾ
	GE_OPEN_EXCHANGE_FRAME,			// �򿪽��׶Ի���
	GE_UPDATE_EXCHANGE,				// ���½��׽���
	GE_CANCEL_EXCHANGE,				// ȡ������
	GE_SUCCEED_EXCHANGE_CLOSE,		// ���׳ɹ���֪ͨ�رս��׽��棨���ܻ���ر���ؽ��棩
	GE_EXCHANGE_INPUT_MONEY,		// ���׿������money
	GE_PACKET_OPEN_SALE,			// ���뱣����̯����

	GE_UPDATE_PET_PAGE,				// ˢ�³������
	GE_UPDATE_LIFESKILL_PAGE,		// ˢ������ܽ���
	GE_OPEN_COMPOSE_ITEM,			// ��������Ʒ����
	GE_UPDATE_COMPOSE_ITEM,			// ˢ��������Ʒ����
	GE_OPEN_COMPOSE_GEM,			// �򿪱�ʯ�ϳ�/��Ƕ����
	GE_UPDATE_COMPOSE_GEM,			// ˢ�±�ʯ�ϳ�/��Ƕ����
	GE_COMPOSE_GEM_SUCCESS,			// ˢ�±�ʯ�ϳ�/��Ƕ����
	GE_ENCHASE_CONFIRM_DLG,			// ��Ƕ��ʯ�滻ȷ�Ͻ���

	GE_AFFIRM_SHOW,					// �򿪷�������ȷ�Ͻ���


	GE_OPEN_STALL_SALE,				// ��̯(��)
	GE_OPEN_STALL_BUY,				// ��̯(��)
	GE_UPDATE_STALL_BUY,			// ����(��)
	GE_UPDATE_STALL_SALE,			// ����(��)
	GE_OPEN_STALL_RENT_FRAME,		// ��̯(���˰����ʾ)
	GE_STALL_SALE_DLG,				// ��̯ȷ�� add by gh 2010/04/15
	GE_CLOSE_STALL_SALE,			// ��̯(�ر�̯�ĶԻ���) 
	GE_STALL_SALE_SELECT,			// ��̯(��)ʱ��ѡ��̯λ�ϵ���Ʒ
	GE_STALL_BUY_SELECT,			// ��̯(��)ʱ��ѡ��̯λ�ϵ���Ʒ
	GE_OPEN_STALL_MESSAGE,			// ��̯(��Ϣ��������)
	GE_CLOSE_STALL_MESSAGE,			// ��̯(��ϵ��Ϣ����)

	GE_STALL_OPEN_PUBLIC,			// ֪ͨ��̯����������ҿ���״̬		20100706BLL

	GE_OPEN_DISCARD_ITEM_FRAME,		// ������Ʒ
	GE_OPEN_CANNT_DISCARD_ITEM,		// ����������Ʒ

	GE_SHOW_SPLIT_ITEM,				// �򿪲����ƽ�Ի���
	GE_HIDE_SPLIT_ITEM,				// �رղ����ƽ�Ի���

	GE_TOGLE_FRIEND_INFO,			// �򿪹رպ����б�Ի���
	GE_TOGLE_FRIEND,				// �򿪹رպ��ѶԻ���
	GE_UPDATE_FRIEND,				// ���º���������
	GE_UPDATE_FRIEND_INFO,			// �������ݽ����
	GE_OPEN_EMAIL,					// ���ż�����
	GE_OPEN_EMAIL_WRITE,			// ���ż�����
	GE_HAVE_MAIL,					// ���ʼ�
	GE_SEND_MAIL,					// �����ʼ�
	GE_UPDATE_EMAIL,				// ˢ�µ�ǰ�ʼ�
	GE_FRIEND_SET,					// ����
	GE_MOOD_CHANGE,					// ����ı���
	GE_OPEN_HISTROY,				// �����ʷ��Ϣ
	GE_ADD_TEMPFRIEND,				// ���뵽��ʱ����

	// ��������
	GE_OPEN_FRIEND_SEARCH,			// �������ѽ���

	//ϵͳ�������
	GE_TOGLE_SYSTEMFRAME,			// ��ϵͳ������
	GE_TOGLE_VIEWSETUP,				// ����Ƶ���ý���
	GE_TOGLE_SOUNDSETUP,			// ���������ý���
	GE_TOGLE_UISETUP,				// �򿪽�������
	GE_TOGLE_INPUTSETUP,			// �򿪰�������
	GE_TOGLE_GAMESETUP,				// ��Ϸ������

	//����̵�
	GE_PS_OPEN_OTHER_SHOP,			// �򿪱��˵��̵�
	GE_PS_OPEN_MY_SHOP,				// ���Լ����̵�
	GE_PS_OPEN_CREATESHOP,			// �򿪴����̵����
	GE_PS_CLOSE_CREATESHOP,			// �رմ����̵����
	GE_PS_OPEN_SHOPLIST,			// ���̵��б����
	GE_PS_SELF_ITEM_CHANGED,		// �Լ��̵�����Ʒ�����ı�
	GE_PS_OTHER_SELECT,				// ѡ��һ����Ʒ
	GE_PS_SELF_SELECT,				// ѡ��һ����Ʒ

	GE_PS_UPDATE_MY_SHOP,			// �����Լ��̵������
	GE_PS_UPDATE_OTHER_SHOP,		// �����̵������

	GE_PS_OPEN_OTHER_TRANS,			// ���Լ����̳�״̬�е��̵�
	GE_PS_UPDATE_OTHER_TRANS,		// ����
	GE_PS_OTHER_TRANS_SELECT,		// ѡ��

	GE_OPEN_PS_MESSAGE_FRAME,		// ����̵��������Ҫ������ȷ�϶Ի���

	GE_PS_OPEN_MESSAGE,				// ������̵����Ϣ��
	GE_PS_UPDATE_MESSAGE,			// ��������̵����Ϣ��

	GE_OPEN_PET_LIST,				// ���׵ȹ���ʹ�õĳ����б�
	GE_VIEW_EXCHANGE_PET,			// ���׵ȹ�������ʾ����
	GE_CLOSE_PET_LIST,				// ���׵ȹ��̽�����ʱ��رճ����б����
	GE_UPDATE_PET_LIST,				// ���³����б����

	GE_OPEN_PRIVATE_INFO,			// �������Ϣ�Ի���

	GE_OPEN_CALLOF_PLAYER,			// ������Ҫ����ĶԻ���
	GE_NET_CLOSE,					// �Ͽ�����

	GE_OPEN_ITEM_COFFER,			// ��������Ʒ����

	GE_PS_INPUT_MONEY,				// ��������̵������Ǯ�Ŀ�


	//--------------------------------------------------------------------------------------------------------------------
	// �������¼�

	GE_TEAM_OPEN_TEAMINFO_DLG,			// �򿪶�����Ϣ�Ի���.
										// 
										// arg0 - ������, ��0��ʼ
										// 0 : ����������������
										// 1 : ��������������
										// 2 : �򿪶�����Ϣ
										// -1: �رնԻ���


	GE_TEAM_NOTIFY_APPLY,				// ֪ͨ�ӳ�, ��������������.
										// 
										// arg0 - ������, ��0��ʼ
										// 0 : ����������������
										// 1 : ��������������
										// ������˵ľ�����Ϣ���������Ϣ�����л��.


	GE_TEAM_APPLY,						// ����������������.
										// 
										// arg0 - �ַ���
										// �������.
	

	GE_TEAM_INVITE,						// ����������������.
										// 
										// arg0 - �ַ���
										// ���������.


	GE_TEAM_CLEAR_UI,					// ��ս���
										//
										// �޲���


	GE_TEAM_REFRESH_UI,					// ˢ�½���
										//
										// �޲���


	GE_TEAM_MEMBER_ENTER,				// ���µĶ�Ա����
										//
										// arg0 - ������, ��1 ��ʼ
										// ��ui�����е���ʾλ��


	GE_TEAM_UPTEDATA_MEMBER_INFO,		// ���¶�Ա��Ϣ
										// arg0 - ������, ��1 ��ʼ
										// ��ui�����е���ʾλ��


	GE_TEAM_MESSAGE,					// ������Ϣ��ʾ��Ϣ
										// arg0 - �ַ���
										// ��Ҫ��ʾ����Ϣ.

										// ע��, ������Ϣ��������ʾ���������Ϣ
										// ��: xxx�뿪����
										//     xxx�Ѿ���һ��������.


	GE_TEAM_MEMBER_INVITE,				// ��Ա����ĳһ���˼����������ӳ�ͬ��
										//
										// arg0 - ��Ա����
										// arg1 - �������˵�����

	GE_TEAM_FOLLOW_INVITE,				// �ӳ������Ա������Ӹ���ģʽ
										//
										// arg0 - �ӳ�����

	GE_TEAM_REFRESH_MEMBER,				// ˢ�¶�Ա��Ϣ
										//
										// arg0 - ��Ա��λ������

	/**********************************************************************************************************
	**
	** �����Ժ���Ҫɾ��
	**
	**
	**
	***********************************************************************************************************/

	GE_ON_TEAM_ENTER_MEMBER,		// ���¶�Ա���

	GE_SHOW_TEAM_YES_NO,			// ��ͬ����Ӵ���.

	GE_SHOW_TEAM_MEMBER_INFO,		// ��ʾ��Ա����ϸ��Ϣ.

	GE_SHOW_TEAM_MEMBER_NAME,		// ����ߵĶ����б������ʾ���ѵ�����

	GE_HIDE_ALL_PLAYER,				// �Լ��뿪�����, �������еĶ��ѽ���

	// ��Ա����ϸ��Ϣ
	GE_SHOW_TEAM_MEMBER_NICK,		// ����
		
	GE_SHOW_TEAM_MEMBER_FAMILY,		// ����
	GE_SHOW_TEAM_MEMBER_LEVEL,		// �ȼ�
	GE_SHOW_TEAM_MEMBER_POS,		// λ��
	GE_SHOW_TEAM_MEMBER_HP,			// hp
	GE_SHOW_TEAM_MEMBER_MP,			// mp
	GE_SHOW_TEAM_MEMBER_ANGER,		// ŭ��
	GE_SHOW_TEAM_MEMBER_DEAD_LINK,  // ������Ϣ
	GE_SHOW_TEAM_MEMBER_DEAD,		// ������Ϣ.

	GE_UPDATE_TEAM_MEMBER,			// ���¶�Ա��Ϣ
									// arg0 - ��Ա��ţ������У��Լ���ռ����ţ�
									// arg1 - ��Ա guid������ȡ�øö�Ա��Ϣ��

	GE_SHOW_TEAM_FUNC_MENU_MEMBER,	// ��ʾ��Ա�Ĺ��ܲ˵�
	GE_SHOW_TEAM_FUNC_MENU_LEADER,	// ��ʾ�ӳ��Ĺ��ܲ˵�
	
	//
	// �������¼�
	//------------------------------------------------------------------------------------------------------------------------

	
	//------------------------------------------------------------------------------------------------------------------------
	//
	// ��¼�������
	//

	GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET,	// ��ʾϵͳ��Ϣ
												//
												// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.
												//
												// ���ȷ�ϰ�ť��Ͽ����硣
	
	GE_GAMELOGIN_SHOW_SYSTEM_INFO,				// ��ʾϵͳ��Ϣ
												//
												// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.


	GE_GAMELOGIN_CLOSE_SYSTEM_INFO,				// ��ʾ�ӳ��Ĺ��ܲ˵�

	
	GE_GAMELOGIN_OPEN_SELECT_SERVER,			// �ر�ѡ�����������
																					

	GE_GAMELOGIN_CLOSE_SELECT_SERVER,			// �ر�ѡ�����������


	GE_GAMELOGIN_OPEN_COUNT_INPUT,				// ���ʺ��������
																					

	GE_GAMELOGIN_CLOSE_COUNT_INPUT,				// �ر��ʺ��������

	GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON,	// ��ʾϵͳ��Ϣ, ����ʾ��ť
												//
												// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.

	GE_GAMELOGIN_OPEN_SELECT_CHARACTOR,			// ��ʾ����ѡ�����

	GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR,		// �ر�����ѡ�����

	GE_GAMELOGIN_OPEN_CREATE_CHARACTOR,			// ��ʾ���ﴴ������

	GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR,		// �ر����ﴴ������

	GE_GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR,	// ˢ��������Ϣ

	GE_GAMELOGIN_CLOSE_BACK_GROUND,				// �رձ�������

	GE_GAMELOGIN_SYSTEM_INFO_YESNO,				// ϵͳ��Ϣ��ʾ yes_no ����.
												//
												// ����0, ��ʾ���ַ���
												// ����1,	�Ի��������
												//			0 -- �Ƿ��˳���Ϸ
												//			1 -- �Ƿ�ɾ����ɫ
												//			2 -- �Ƿ�����ʺ�

	GE_GAMELOGIN_SELECT_LOGIN_SERVER,			// ѡ��һ��login server
												//
												// ����0, iAreaIndex   ��������
												// ����1, iLoginServer ��������

	GE_GAMELOGIN_CLEAR_ACCOUNT,					// ����ʺ���ʾ.
												//

	GE_GAMELOGIN_SELECT_AREA,					// ����ʺ���ʾ.
												//
	GE_GAMELOGIN_SELECT_LOGINSERVER,			// ����ʺ���ʾ.
												//

	GE_GAMELOGIN_CREATE_CLEAR_NAME,				// ��մ�����ɫ�����֡�
												//

	GE_GAMELOGIN_CREATE_ROLE_OK,				// ������ɫ�ɹ���
												//
												
	

	//
	// ��¼�������
	//
	//------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------------------------------------------
	//
	// �Ƿ����ö�����������
	//

	GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG,			// �����ö����������

	GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG,		// ��unlock������档

	GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG,		// �����������

	GE_OPEN_SYSTEM_TIP_INFO_DLG,					// ��ʾϵͳ��Ϣ�Ի���

	GE_INPUT_PASSWORD_DLG,							// ��������������
	
	//
	// �Ƿ����ö�����������
	//
	//------------------------------------------------------------------------------------------------------------------------
	
	GE_SUPERTOOLTIP,				//Tooltips
									/*
									| arg0 - ��ʾ/���� 1,0
									| arg1 - ���� "skill", "lifeability", "item", "xinfa", "pet_skill"
									| arg2, arg3 - ���λ��
									|
									*/
											 
	// ����������
	GE_RELIVE_SHOW,					// ��ʾ�������
									// arg0 - �Ƿ��и���İ�ť
									// arg1 - ʣ��ʱ��

	GE_RELIVE_HIDE,					// ���ظ������

	GE_RELIVE_REFESH_TIME,			// ˢ�������ϵ�ʱ��
									// ��ʾʱ��

	GE_OBJECT_CARED_EVENT,			//ĳ�߼������ĳЩ�����ı�
									/*
									|  arg0 - Object��ID
									|  arg1 - ����
									|			distance: ���뷢���ı� (arg2����)
									|			destroy:  ������
									|
									*/
	GE_UPDATE_MAP,					// ���µ�ͼ
	GE_UPDATE_MAP_LIST,				// ���µ�ͼ�б�
	GE_UPDATE_LARGEMAP_LIST,		// ���´��ͼ�б�
	GE_UPDATE_SCENEMAP_LIST,		// ���³�����ͼ�б�

	GE_UPDATE_SUPERTOOLTIP,
	GE_UI_COMMAND,
	GE_OTHERPLAYER_UPDATE_EQUIP,

	GE_VARIABLE_CHANGED,			//ĳ��ȫ�ֱ��������ı䣬 
									/*
									| arg0 - ������
									| arg1 - �µı���ֵ
									*/

	GE_TIME_UPDATE,					//��ʱ��ϵͳ��ʱ�������¼�			
	GE_FRIEND_INFO,					// �򿪺�����ϸ��Ϣ

	GE_UPDATE_TARGETPET_PAGE,		//��targetpet ui
	GE_UPDATE_PETSKILLSTUDY,		//���³��＼��ѧϰ����
	
	GE_UPDATE_PETINVITEFRIEND,		//���³������Ѱ����
	GE_REPLY_MISSION_PET,			//����ˢ��

	GE_GUILD_CREATE,				//��ᴴ���¼�
	GE_GUILD_CREATE_CONFIRM,		//��ᴴ��ȷ���¼�
	GE_GUILD_SHOW_LIST,				//��ʾ����б��¼�
	GE_GUILD_SHOW_MEMBERINFO,		//��ʾ�Լ�����Ա�������
	GE_GUILD_UPDATE_MEMBERINFO,		//���°���Ա����Ϣ
	GE_GUILD_SHOW_DETAILINFO,		//��ʾ�Լ������ϸ��Ϣ����
	GE_GUILD_SHOW_APPOINTPOS,		//��ʾ�Լ�������ְλ����
	GE_GUILD_DESTORY_CONFIRM,		//ɾ�����ȷ���¼�
	GE_GUILD_QUIT_CONFIRM,			//�˳����ȷ���¼�
	GE_GUILD_FORCE_CLOSE,			//ǿ�ư����ؽ���ر��¼�
	GE_GUILD_INVITE_OBJECT,			//����Ի���
	GE_GUILD_NOTIFY_INTIVE,			//֪ͨ�յ��������
	GE_GUILD_INTIVE_CONFIRM,		//����ȷ��
	GE_GUILD_MODIFY_TITLE,			//�޸ĳ�Ա�ƺ�
	GE_GUILD_CREATE_FAMILY,			//��������
	GE_GUILD_CREATE_FAMILY_CONFIRM,	//��������ȷ��
	GE_GUILD_DESTORY_FAMILY_CONFIRM,//ɾ������ȷ��
	GE_INPUT_PASSWD_COMMAND,		//ͨ����������򿪰�����

	GE_CLOSE_MISSION_REPLY,			//�ر������ύUI
	GE_TOGLE_TARGET_EQUIP_PAGE,		//�����鿴�Է���ɫװ������
	GE_CLOSE_TARGET_EQUIP,			//�رա�������������������
	GE_TOGLE_TARGET_EQUIP_ALL,		//�����鿴�Է���ɫ����
	GE_CLOSE_TARGET_EQUIP_ALL,		//�رա���������������
	GE_CHANGE_TARGET_EQUIP_ALL_PAGE,//�л�����������������
	GE_VIEW_RESOLUTION_CHANGED,		//��Ļ�ֱ��ʱ仯ʱ��֪ͨ��Ϣ
	GE_CLOSE_SYNTHESIZE_ENCHASE,	//�رպϳ�UI����ǶUI
	GE_TOGGLE_PETLIST,				//�򿪺͹رճ����б�
	GE_PET_FREE_CONFIRM,			//��������ȷ��


	// �������
	GE_TOGLE_MOUNT_PAGE,			// ��������
	GE_CLOSE_MOUNT_PAGE,			// �ر�������
	GE_MOUNT_ATTR_UPDATE,			// �����������

	GE_TOGLE_TARGET_MOUNT_PAGE,		// �򿪻�ر�Ŀ���������
	GE_TARGET_MOUNT_ATTR_UPDATE,	// ����Ŀ����������

	GE_TOGLE_MOUNT_STUDY_PAGE,		// �򿪻�ر����ѧϰ����
	GE_MOUNT_STUDY_ITEM_UPDATE,		// �������ѧϰ��Ʒ

	GE_TOGLE_MOUNT_FEED_PAGE,		// �򿪻�ر�����ι������
	GE_MOUNT_FEED_ITEM_UPDATE,		// �������ι����Ʒ

	GE_TOGLE_MOUNT_TRAIN_PAGE,		// �򿪻�ر����ѱ������
	GE_MOUNT_TRAIN_ITEM_UPDATE,		// ����ѱ����Ʒ

	GE_TOGLE_MOUNT_IDENTIFY_PAGE,	// �򿪻�ر�����������

	GE_TOGLE_MOUNT_MATING_PAGE,		// ����跱ֳ����
	GE_CLOSE_MOUNT_MATING_PAGE,		// �ر���跱ֳ����
	GE_MY_MOUNT_MATING_LOCKED,		// �Լ��Ľ����������
	GE_OTHER_MOUNT_MATING_LOCKED,	// ���˵Ľ����������
	GE_REQUEST_TOGLE_MOUNT_MATING_PAGE,	// ���뿪����ֳ����
	GE_GAIN_MATING_MOUNT,			// ��ȡ��ֳ����


	GE_OPEN_BUY_CONFIRM,			// ���̵�����ȷ�Ͻ���
	GE_OPEN_SALE_CONFIRM,			// ���̵�����ȷ�Ͻ���

	// �û����������������Ƶ��¼�
	GE_FRIEND_CREATE_GROUP,
	// �����������
	GE_OPEN_FRIEND_SEARCH_REASON,    

	// ��Ӻ�������
	GE_ADD_GROUP,
	GE_QUICK_CHANGE_NAME,			// һ����װ����

	GE_ITEM_LINK,					// ��Ʒ���� arg0����Ʒ����

	GE_TOGLE_MOUNT_CHILD_PAGE,		// �򿪻�ر���軹ͯ����
	GE_MOUNT_CHILD_ITEM_UPDATE,		// ������軹ͯ��Ʒ

	// ��ʾ���ѵ���ϸ��Ϣ 
	GE_SHOW_FRIEND_INFO,

	GE_EQUIP_SWAPCARD_OPEN,			//��װ�����ͽ�������
	GE_EQUIP_SWAPCARD_UPDATA,		//����װ�����ͽ�������
	GE_EQUIP_SWAPCARD_SUCESS,		//װ�����ͽ����ɹ�

	GE_EQUIP_CHANGECARD_OPEN,		//��װ�����͸������
	GE_EQUIP_CHANGECARD_UPDATA,		//����װ�����͸������
	GE_EQUIP_CHANGECARD_SUCESS,		//װ�����͸���ɹ�	 
	
	GE_EQUIP_ALL_OPEN,				// �򿪸���װ�� �������� ���˳ƺ�  һ����װ �������  ��������


	GE_SHOW_TITLE,					// ��ʾ�ƺŽ���
	GE_UPDATE_TITLE,				// ���³ƺŽ���

	GE_PERSONAL_MESSAGE,			// ������Ϣ
	GE_SYSTEM_MESSAGE,				// ϵͳ��Ϣ

	GE_SHOW_SYSTEM_PROMPT,			// ϵͳ��Ϣ��ʾͼ��
//	GE_SYSTEM_PROMPT_UPDATE,		// ����ϵͳ��Ϣ��ʾͼ��

	GE_SHOW_SUPERTOOLTIP_TITLE,		// ��ʾ�ƺ���ʾ

	GE_UNITE_GF_SHOW,				// ���ѺͰ�������

	GE_RICH_SAFE_GUARD,				// ������Χ����
	GE_RICH_SAFE_TIME,				// ��ȫʱ�޵�������
	GE_RICH_SET_PASSWORD,			// �������롢�޸��������
	GE_RICH_MODIFY_PASSWORD,
	GE_RICH_SETUP,					// �Ʋ������������ý���
	GE_OPEN_RICHES_PROTECT_FRAME,	// �Ʋ�������ʾ����
	GE_RICH_TEMP_CANCEL_PASSWORD,	// ��ʱȡ������	
	GE_RICH_CANCEL_PASSWORD,		// ȡ������
	GE_RICH_CHECK_PASSWORD,			// �����������


	//temp  ���ͽ����ã� �Ⱥϵ�һ��������Ͳ�����
	GE_TEMP_CLOSE_FRIEND,				// 
	GE_TEMP_CLOSE_GUILD,				// 
	GE_EQUIP_ALL_CLOSE,		// �رո���װ�� �������� ���˳ƺ�  һ����װ �������  ��������

	GE_TOGLE_PLAYER_ATTR_PAGE,		// ��������ϸ���Խ���
	GE_CLOSE_PLAYER_ATTR_PAGE,		// �ر�������ϸ���Խ���

	GE_OPEN_DAY_TIPS,				// ��ÿ�����ѹ��ܽ���
	GE_UPDATE_DATE_TIME,			// ����ÿ���������ں�ʱ��
	GE_UPDATE_DAY_TIPS,				// ����ÿ����������
	GE_UPDATE_CALENDAR,				// ������������

	GE_OPEN_TOP_LIST,				// �����а����
	GE_UPDATE_TOP_LIST,				// �������а����
									// arg0��Ϊ0��ʾ������а�����Ϊ1��ʾ������а�����
	GE_OPEN_MISSION_SCOUT,			// ��������ٽ���
	GE_UPDATE_MISSION_SCOUT,		// ����������ٽ���
	GE_UPDATA_EQUIP_ALL_POS,        //����6������λ�� ������װ�� �������� ���˳ƺ�  һ����װ �������  �������ϡ�


	GE_COUNTRY_MANAGER_REQUEST_INFO,// ���������Ϣ
	GE_COUNTRY_MANAGER_INFO,		// ������Ϣ
	GE_COUNTRY_MANAGER_ADJUST_AUTH,	// ���ҹ�ְ����
	GE_INPUT_COUNTRY_DLG,			// ������ҹ�ְ
	GE_COUNTRY_MANAGER_CREATE_KING,	// ��������
	GE_COUNTRY_KING_CREATE_CONFIRM,	// ȷ�ϲ�������
	GE_SUPERTOOLTIP2,

	GE_TEAM_CHANGE_MODE,			//ʰȡģʽ������Ϣ
									// arg0 - ����ģʽid

	GE_END_PACKUP_PACKET,			//�����������
	GE_BEGIN_PACKUP_PACKET,			//��������ʼ

	GE_OPEN_WINDOW,					//��һ������
	GE_CLOSE_WINDOW,				//�ر�һ������
	GE_TOGGLE_WINDOW,				//�л�һ������

	GE_VALIDATE_ADD_FRIEND,			//ȷ�ϼ�Ϊ����

	GE_FRIEND_DELETE_FRIEND,		//�����Ƿ�ȷ��ɾ��������Ϣ

	// 20100419 AddCodeBegin
	GE_SET_ROLE_OPENCLOSESIGN,		// �������˵����ﴰ�ڴ򿪹رձ��
	// 20100419 AddCodeEnd

	GE_SOUXIA_BANGDING_ENCHASE,		//ȷ�ϰ�����¼
	GE_SOUXIA_STUDY,		//ȷ��Ҫ����ѧϰ
	GE_UPDATE_SOUXIA_STUDY,		//��������¼��ѧ�����б��
	GE_CLEAR_SOUXIA_BAR,				//��������ܿ�����е���������
	GE_UPDATE_SOUXIA_SKILLNUM, //���¼��ܿ�����е��������ܵ�ʹ�ô���
	GE_UPDATE_SOUXIA_INDEXSKILLNUM,		//����ָ��λ���ϵ�����������Ϣ

	GE_SHOW_EQUIPLIANHUA,			//��װ����������
	GE_CLOSE_EQUIPLIANHUA,			//�ر�װ����������
	GE_ADD_LIANHUAEQUIP_ITEM,	//������װ�������ڰ�ť��
	GE_ADD_LIANHUAEQUIP_DIS,		//���������߷����ڰ�ť��

	GE_OPEN_EQUIP_FUMO,				//�򿪸�ħ����
	GE_ADD_EQUIP_FUMO_ITEM,		//��Ҫ��ħ��װ�������ڰ�ť��
	GE_ADD_EQUIP_FUMO_DIS,			//����ħ���߷����ڰ�ť��
	GE_CLOSE_EQUIP_FUMO,			//�رո�ħ����

	GE_OPEN_EQUIP_TUIMO,				//����ħ����
	GE_ADD_EQUIP_TUIMO_ITEM,		//��Ҫ��ħ��װ�������ڰ�ť��
	GE_ADD_EQUIP_TUIMO_DIS,			//����ħ���߷����ڰ�ť��
	GE_CLOSE_EQUIP_TUIMO,			//�ر���ħ����

	GE_OPEN_ADD_NEW_FRIEND,		// ����Ӻ��Ѵ���
	
	GE_MAINTARGET_BUFFTEXT_REFRESH,	// Ŀ��ͷ���·�buff��tooltip����

	GE_SET_SCROLL_INFO,				// ����������Ϣ 20100624 BLL

	//add by gh RMB�̳�
	GE_OPEN_IB_SHOP,				// ��RMB�̳�������
	GE_SHOW_ALL_IB_ITEM,			// ��ʾ����IB����
	GE_UPDATE_IB_ITEM,				// ������ʾ����
};
