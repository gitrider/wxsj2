
#ifndef __GAMEDEFINE_H__
#define __GAMEDEFINE_H__

//
#define INVALID_COUNTRY		-1
#define INVALID_JOB			-1
#define INVALID_SEX			-1
#define INVALID_GUILD_ID  	-1
#define INVALID_RESOURCE    -1
#define INVALID_RULERID		-1
#define INVALID_PRICE		 0
#define INVALID_ITEM_ID		 0
#define INVALID_ITEM_POS	-1
#define INVALID_TIME		 0xFFFFFFFF
#define INVALID_PAI_TYPE	 0
#define INVALID_SOUL_TYPE	 0
#define BASE_BAG_POS		 185

//�Ƿ�ִ�и��ϸ�Ĳ߻�����У��
#define CHECK_DATA_TABLE 1
/////////////////////////////////////////////////////////////////////////////////
//��Ϸ������Ϣ�궨��
/////////////////////////////////////////////////////////////////////////////////
//��������ֵ
#define MAX_WORLD 255

#define MAX_WORLD_USER	3000

//�û���ɫ���ֿ�����������ֵ
#define MAX_CHARACTER_INPUTNAME 24
//��ҳƺų���
#define MAX_CHARACTER_TITLE 34

//����˺ų���
#define MAX_ACCOUNT_LENGTH	50

//GamePlayer������������
#define MAX_PLAYER 1024

//��ҳ�����
//#define MAX_POOL_SIZE 1280
//��ҳ�����
#define MAX_POOL_SIZE (2048)
#define MAX_DUMMY_POOL_SIZE 64
//��½��������ҳ�
#define MAX_LOGIN_POOL_SIZE 1024*3
//��������
#define MAX_GUILD_SIZE 1024

//û�ڵ��ʼ���
#define MAX_MAILNODE_SIZE 102400

//�û���ɫ���ֽṹ |CharacterName|@|WorldID|00|
//					��ɫ����      1  3      2����
//ע�⣺��ɫ���ֺ���ĺ�׺����@WorldID�������û�������ɫʱ���Զ���ӽ��ģ�
//		�������˴���ʱ����ʹ���������֣�
//		�ͻ�����ʾ��ʱ�������ǰ��ɫ�Ǳ����紴��������ȥ������ĺ�׺��
//�û���ɫ���Ƶ����ֵ
#define MAX_CHARACTER_NAME 30

#define MAX_TIME  6

#define MAX_COUNTRY_COLLECT_DESC 68

//���а���������
#define MAX_TOP_LIST_NAME 32

//���������ʾ100��
#define TOP_USDR_NUM 100				

//����Զ������������ֵ
#define MAX_GROUP_NAME		12

//��ҳƺų���
#define MAX_CHARACTER_TITLE 34

#define MAX_TITLE_COMBINATION 10

//NPC���������Ƴ���
#define NPC_NAME_LEN 32

//����Զ���ƺ�ID  �б�
#define GUANZHI_TITLE_ID   0   //��ְ�ƺ�ID

#define BANGPAI_TITLE_ID   1   //���ƺ�ID

#define JIAZU_TITLE_ID     2   //����ƺ�ID

#define JIEHUN_TITLE_ID    3   //�ӻ�ƺ�ID

//NPC�������ƺų���
#define NPC_TITLE_LEN 32

//��ɫ���ǳ�
#define MAX_NICK_NAME 34

//̯λ������
#define MAX_SHOP_NAME 34

//̯λ������
#define MAX_STALL_NAME 34

//���Ե���Ŀ
#define MAX_BBS_MESSAGE_NUM 20

//���Եĳ���
#define MAX_BBS_MESSAGE_LENGTH 70

//��������������
#define MAX_BBS_MESSAGE_AUTHORLENGTH 40

//����������̳���
#define MIN_PWD 8

//�����������󳤶� + 1
#define MAX_PWD 20

////�û��ʺŵ����ֵ
//#define MAX_ACCOUNT 12

//�û���������ֵ
#define MAX_PASSWORD 12

//�û��ʺŵ����ֵ
#define MAX_ACCOUNT		50
#define MAX_BILLACCOUNT 50
#define MAX_REBOOT_INFO	32
#define MAX_EXT_INFO	32

//�û���������ֵ
#define MAX_PASSWORD 12

//���е��ٶ�����
#define DEF_CLIENT_FLY_SPEED_RATE (1.2f)
#define DEF_SERVER_FLY_SPEED_RATE (1.3f)		// Ϊ�˱�֤��������ȿͻ���������

//���Ʒ�IP��С
#define MAX_BILLING_IP_SIZE  15
//�齱��ˮ�ų���
#define MAX_PRIZE_SERIAL_LENGTH		20
//������Ʒ���볤��
#define MAX_PRIZE_GOODS_LENGTH	6
//���md5������
#define MAX_MD5_SIZE		 32
//���email������
#define MAX_EMAIL_SIZE		 255
//��󳬼����볤��
#define MAX_SUPER_CODE_SIZE	 32
//��������Ϣ����
#define MAX_OTHER_INFO_SIZE	 32
//Ԫ�����Ѽ�¼��������Ϣ����
#define MAX_COST_OTHER_SIZE	 255

//ְҵ��Ϣ���ֵ
#define MAX_JOBINFO_LENGTH	50
//��ҵԺУ��Ϣ���ֵ
#define MAX_SCHOOLINFO_LENGTH	50
//ʡ����Ϣ���ֵ
#define MAX_PROVINCE_LENGTH		16
//City��Ϣ���ֵ
#define MAX_CITY_LENGTH		16
//Email��Ϣ���ֵ
#define MAX_EMAIL_LENGTH	50
//����������ֵ
#define MAX_LUCKWORD_LENGTH	255

//����ܴ�����ɫ������
#define MAX_CREATE_PLAYER_NUM 3

//CharacterManager��ȱʡ��ObjID�����С
#define DEFAULT_OBJ_COUNT 1024

//
#define ZONE_SIZE	10

//
#define MAX_SEND_TIME 1

//����Zone���ӵķ�Χ,0=1��(���������Լ��ڵ��Ǹ�����)��1=9��;2=25��;3=49��;
#define MAX_RADIUS 2

#define MAX_REFESH_OBJ_ZONE_RADIUS		2

#define MAX_REFESH_OBJ_ZONE_ARRAY_SIZE	((MAX_REFESH_OBJ_ZONE_RADIUS - 1) * (MAX_REFESH_OBJ_ZONE_RADIUS - 1) + 1 )

//�����ʰȡ��������
#define MAX_BOX_OP_DISTANCE				3.0f

#define ITEM_DROP_DISTANCE  0.6F	//�������

//�����ʰȡ��������
#define MAX_DROP_ITEM_OP_DISTANCE				3.0f

//�������������
#define MAX_BAG_SIZE		184
#define MAX_EXT_BAG_NUM 5 // by soongxl

//�������������
#define MAX_BASE_BAG_SIZE   184
#define MAX_BASE_BAG_REAL_SIZE 180

#define MAX_SINGLEBAG_SIZE  36

#define MAX_NPC_DISTANCE	5.0

//������������  [7/8/2010 �¾���]
#define MAX_MOUNT_SIZE   5

//����������У���������ݷ���Ʒ�����Ĵ�С
#define OPENBOX_CONTAINER_SIZE 2
#define TMP_OPENBOX_CONTAINER_SIZE 20

//Container ƫ��
//������ƫ��
#define EXTRA_CONTAINER_OFFSET	180

//������ƫ��
#define BASE_CONTAINER_OFFSET	0

//��չ��ƫ��
#define EXTRA_BAG1_OFFSET	36

//��չ��ƫ��
#define EXTRA_BAG2_OFFSET	72

//��չ��ƫ��
#define EXTRA_BAG3_OFFSET	108

//��չ��ƫ��
#define EXTRA_BAG4_OFFSET	144

////���ϰ�ƫ��
//#define MAT_CONTAINER_OFFSET	20
//
////�����ƫ��
//#define TASK_CONTAINER_OFFSET	40

//װ��ƫ����
#define EQUIP_CONTAINER_OFFSET	184

//����ƫ����
#define BANK_CONTAINER_OFFSET	58
#define DB_BAG_POS_START			0
#define	DB_BAG_POS_END				184
#define DB_EQUIP_POS_START			DB_BAG_POS_END//  ԭ����Խ�� [6/11/2008 wuwenbin](DB_BAG_POS_END+1)
#define DB_EQUIP_POS_END			(DB_EQUIP_POS_START+HEQUIP_NUMBER)
#define DB_BANK_POS_START			DB_EQUIP_POS_END// ԭ����Խ�� [6/11/2008 wuwenbin](DB_EQUIP_POS_END+1)
#define DB_BANK_POS_END				(MAX_BANK_SIZE+DB_BANK_POS_START-1)


#define MAX_BANK_MONEY		10000000

//��������ʾ
#define	INVALID_BAG_INDEX (-1)

//һ�������������ж��ٸ�����̵�
#define MAX_SHOP_NUM_PER_SCENE 512

//һ�����������ӵ�е�����̵����
#define MAX_SHOP_NUM_PER_PLAYER 2

//�̵��ղؼд�С
#define MAX_FAVORITE_SHOPS		10

//һ������̵�Ĺ�̨��
#define MAX_STALL_NUM_PER_SHOP 10

//һ������̵�ĺ��������
#define MAX_PARTNER_PER_SHOP 6

//���׼�¼�������
#define MAX_EXCHANGE_RECORD 40

//�����¼�������
#define MAX_MANAGER_RECORD 128

//����¼����
#define MAX_RECORD_LEN_ENTRY	128		

//����¼��
#define MAX_RECORD_NUM_ENTRY	128		

//ÿҳ��¼��
#define MAX_RECORD_NUM_PER_PAGE	10	

//һ������̵�����ֳ���
#define MAX_SHOP_NAME_SIZE 12

//һ������̵���������ݳ���
#define MAX_SHOP_DESC_SIZE 82

//�̵��˰��
#define	SHOP_TAX 0.03

//���е��������
#define MAX_BANK_SIZE 180

//���������Դ��Ǯ��
#define MAX_BANK_MONEY 10000000

//���г�ʼ��С
#define CUR_BANK_SIZE 36

//��һ����������ʼ����
#define RENTBOX1_START_INDEX 0

//�ڶ�����������ʼ����
#define RENTBOX2_START_INDEX 36

//��������������ʼ����
#define RENTBOX3_START_INDEX 72

//���ĸ���������ʼ����
#define RENTBOX4_START_INDEX 108

//�������������ʼ����
#define RENTBOX5_START_INDEX 144

//���׺е��������
#define	MISSION_BOX_SIZE	4

//�����б������Ʒ���������
#define	QUESTLOG_BOX_SIZE	255

//���׺е��������
#define	EXCHANGE_BOX_SIZE	5

//���׺��г�����������
#define	EXCHANGE_PET_BOX_SIZE	3

//����˫����������������
#define EXCHANGE_MAX_DISTANCE	8	// modify by gh for new 45->8 2010/04/28

//̯λ�е��������
#define	STALL_BOX_SIZE		36

//̯λ�е��������(һ��Ҫ��STALL_BOX_SIZEһ���󣡣���)
#define	STALL_PET_BOX_SIZE		20

//����������߶��еĳ���
#define	MAX_EXCHANGE_APPLICANTS_NUM	10

//�ƺŵ��������
#define MAX_STRING_TITLE_SIZE 4

//��̬�ƺŵ��������
#define MAX_DYNAMIC_TITLE_SIZE 256

//�ƺŵ��������
#define MAX_TITLE_SIZE 20 // �ƺŵ�����̫���ˣ�����ʱ��С [6/10/2008 wuwenbin]256

//��Ч�ĳƺ�ID
#define INVALID_TITLE_ID -1

//��̬̬�ƺŵ��������
#define MAX_TITLE_ID_SIZE 16

//��Ʒ����Ƕ��ʯ��������
#define MAX_ITEM_GEM 7

//��Ʒ����Ƕ��ʯ��������
#define MAX_GEM_HOLE 7

////�����ɫװ��������Ը���
//#define		MAX_BLUE_ITEM_EXT_ATTR				4

//���װ����Ȼ���ԣ�û����Ƕ��ʯ��
#define		MAX_ITEM_NATUREATTR		8

//���ְҵ����
#define     MAX_JOB_SUM				(MATTRIBUTE_NUMBER)

//���������Ը���(Զ���⹦������Զ���⹦������Զ���ڹ�������Զ���ڹ������������⹦�����������⹦�����������ڹ������������ڹ�����)
#define MAX_BASE_ITEM_ATTR				8		

//add by gh for Soul_bead 2010/06/24
//����,��,����,����,�����⹦������,Զ���⹦������,�����ڹ�������,Զ���ڹ�������
#define MAX_WU_SOUL_BEAD_ATTR		8
//����,��,����,����,�����⹦����,Զ���⹦����,�����ڹ�����,Զ���ڹ�����
#define MAX_QI_SOUL_BEAD_ATTR		8
// end of add
//�����Ʒ����
#define MAX_ITEM_ATTR (MAX_BASE_ITEM_ATTR+MAX_ITEM_NATUREATTR)

//���װ���������Ը���
#define		MAX_EQUIP_EXT_ATTR					8

//���ȼ������
#define  MAX_RAND_LEVEL_NUM						10

//���װ���������Ը���
#define		MAX_EQUIP_LEVEL						100

//�����ͨװ������
#define		MAX_COMMON_EQUIP_COUNT				50

//���װ���ȼ������θ���
#define		MAX_EQUIP_MODEL						100

//���װ��Ӱ�켼����Ŀ
#define		MAX_EQUIP_EFFECT_SKILL				4

//���װ�����Ӽ�����Ŀ
#define		MAX_EQUIP_AFFIX_SKILL				4

//����������豦ʯ��
#define    MAX_LEVELUP_GEMREQ_NUM				4

//���װ���Ǽ�
#define    MAX_EQUIP_LEVELUP_NUM				15

//���װ���󶨼���
#define    MAX_EQUIP_BIND_LEVEL					1

//һ����װ������
#define    MAX_EQUIP_SUIT_NUM					10

#define	   MAX_EQUIPSUIT_POINT_NUMBER			16	

//�����Ч���ϵȼ�
#define    MAX_VALIDMATERIAL_LEVEL				15		

//Ŀǰ���ŵĲ��ϵȼ�
#define    MAX_MATERIAL_LEVEL					10

//װ��������������������
#define    MAX_CREATE_MATERIAL_NUM				4

//װ��������������������
#define    MAX_REMOULD_MATERIAL_NUM				3

#define    MAX_EXTRA_BAG_NUM					4

#define    MAX_BAG_NUM							(MAX_EXTRA_BAG_NUM+1)

#define	   MAX_SOUL_EFFECTATT					4


//---------------------------------------------------------
//��ʯ���

//��Ƕ��Чװ��
#define MAX_BASE_EQUIP_COUNT 38

//��ʯ��Ӱ������
#define MAX_GEM_ATT 4

//��ʯ��Ӱ������
#define MAX_GEM_REQ 3


//�����ڴ�ItemSerial�̶�key
#define ITEM_SERIAL_KEY		536081

//��ʯ����߼���
#define MAX_GEM_QUALITY 9

//��������ʹ�ñ�����Ʒ��
#define MAX_ABILITY_ITEM 5

//���װ����װ����
#define MAX_ITEM_SET_ATTR 4

//���ҩƷЧ��
#define	MAX_MEDIC_EFFECT_NUM 3

#define MAX_GROW_POINT_RATE_NUM		4
#define MAX_LOGIC_FUNC_LEN			128
#define ITEM_LOGIC_FUNC_LEN			32
#define MAX_SCENE_GROW_POINT_NUM	128

#define MAX_SCENE_GP_OWNER			255

// �������ӵ�еļ�����Ŀ
#define MAX_PET_SKILL_COUNT		(6)

//Obj_ItemBox ��������Ʒ����
#define MAX_BOXITEM_NUMBER 10

//Obj_Item ��������Ʒ����
#define MAX_DROP_ITEM_NUMBER 100

//Obj_ItemBox ������ʰȡ����
#define MAX_ITEMBOX_PICK_DIST		(20.0f)

//���˿����������������
#define MAX_BOOTH_NUMBER	   96

//�̳ǿ����������������
#define MAX_IBSHOP_NUMBER	   120

//���˿��Իع������������
#define MAX_BOOTH_SOLD_NUMBER   12

//
#define MAX_PLAYER_EXP_LEVEL	100

//
#define MAX_100_PLUS_DELTA_LEVEL	201

//
#define DEFAULT_WASHPOINT_LEVEL		60

//�����弶�����Ե����
#define MAX_TABLE_DEFINE_LEVEL	 100

//�Ͽ�����󣬷����������������ݵ�ʱ�䣨���룩
#define TIME_TO_TRUE_QUIT 10000

//����������������ֵ
#define	MAXTYPE_NUMBER 10240

//����AI������������ֵ
#define MAXAI_NUMBER 256

//NPC̸�����������ֵ
#define MAXCHATTYPE_NUMBER 64

//NPC̸����¼�����ֵ
#define MAXCHAT_NUMBER 64

//���������ֳ���
#define MAX_SERVER_NAME	32


//Npc EventList ����
#define MAX_NPC_EVENT  25

//��ɫ��ӵ�м��ܵ�����
#define MAX_CHAR_SKILL_NUM 256

//��ɫ���Ͽ���ӵ�еĸ���Ч���������
#define MAX_IMPACT_NUM 32

//��ɫ��ӵ�м����ķ�������
#define MAX_CHAR_XINFA_NUM 16

//��ɫ��ӵ������ܵ�����modify by gh 64->8,0:ռλ��
#define MAX_CHAR_ABILITY_NUM		8

//�����䷽�����Ŀ��
#define MAX_ABILITY_PRESCRIPTION_NUM (4 * 256)

//�����䷽��Ĵ浵�ֽ�ռ����
#define MAX_CHAR_PRESCRIPTION_BYTE (MAX_ABILITY_PRESCRIPTION_NUM>>3)

#define ABILITY_GAMBLE_NUM 5

//����������
#define MAX_COUNTRY_NUM 4

//�������ж���
#define MAX_KINDOM_ACTION_POINT 10000

#define MAX_PREFESSION_NUM 16
//��������
#define MAX_MENPAI_NUM 16

//ÿ��������ӵ�еļ����ķ�������
#define MAX_MENPAI_XINFA_NUM 16

//���ķ�����
#define MAX_ALL_XINFA_NUM	54

//ÿ���ķ����ܹ��ﵽ�����ĵȼ�
#define MAX_XINFA_LEVEL_NUM	120

//ÿ��������ܹ��ﵽ�����ȼ�
#define MAX_ABILITY_LEVEL_NUM 60

//�����������
#define MAX_SKILL_NUMBER 1024

// ����������
#define MAX_MISSION_NUM				(4096)
#define MAX_CHAR_MISSION_FLAG_LEN	((MAX_MISSION_NUM+31)/32)

//��ɫ��ӵ�����������
#define MAX_CHAR_MISSION_NUM		(20)
enum
{
	QUESTTIME_LIST_SIZE = MAX_CHAR_MISSION_NUM,
};

//ÿ������Notify����
#define MAX_MISSION_NOTIFY_NUM      (4)

//��ɫ��ӵ����������
#define MAX_CHAR_MISSION_DATA_NUM	(256)

// ÿ����ɫ����ܿ���������������Ʒ�ĵ���
#define MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM	(MAX_CHAR_MISSION_NUM*4)

//����OBJ�������
#define MAX_SKILLOBJ_NUMBER			1024

//���ɴ�ITEM BOX�ľ���
#define MAX_OPEN_ITEMBOX_DIST		(2.f)

//���ɴ�ITEM BOX�ľ���ƽ��
#define MAX_OPEN_ITEMBOX_DIST_SQ (MAX_OPEN_ITEMBOX_DIST * MAX_OPEN_ITEMBOX_DIST)

//���ɴ����ű��¼��ľ���(�ͻ���)
#define MAX_CLIENT_DEFAULT_EVENT_DIST (3.f)

//���ɴ����ű��¼��ľ���(������)
#define MAX_SERVER_DEFAULT_EVENT_DIST	(MAX_CLIENT_DEFAULT_EVENT_DIST * 2.f)

//���ɴ����ű��¼��ľ���ƽ��(�ͻ���)
#define MAX_CLIENT_DEFAULT_EVENT_DIST_SQ (MAX_CLIENT_DEFAULT_EVENT_DIST * MAX_CLIENT_DEFAULT_EVENT_DIST)

//���ɴ����ű��¼��ľ���ƽ��(������)
#define MAX_SERVER_DEFAULT_EVENT_DIST_SQ (MAX_SERVER_DEFAULT_EVENT_DIST * MAX_SERVER_DEFAULT_EVENT_DIST)

//
#define MAX_ATOM_OPT_NUM 8

// ��ɫ���ܵ����·��������
#define MAX_CHAR_PATH_NODE_NUMBER	(16)

//��������ģ����Թ������󳡾�����
#define MAX_SCENE 1024

// һ�� zone ������Լ������� Area ����
#define MAX_AREA_IN_ZONE		10

//һ�������������õĳ������ֵ
#define MAX_COPYSCENE 128

//����������󳤶�
#define SCENE_NAME_LEN 16

//���Ķ�������
#define MAX_TEAM_MEMBER 6

// �Ŷ�������������
#define MAX_TEAMS_IN_GROUP 8

//��������
#define MAX_TEAMS 3000

//�Խ�����Ƶ�����������
#define MAX_CHATCHANNEL_MEMBER 11

//Ƶ������
#define MAX_CHANNELS 3000

// ��ౣ������������ʼ�
#define MAX_MAIL 20

//�ʼ����ݵ���ߴ�
#define MAX_MAIL_CONTEX 256

//��������˵������󳤶�
#define MAX_MONSTERSPEAK_CONTENT 64

//��С���Է�����Ʒ��Ѫ����
#define MIN_DAMAGE_PERCENT	(0.2f)

//������ֳ���
#define MAX_GUILD_NAME_SIZE 24

//�������ֳ���
#define MAX_GUILD_FAMILY_NAME_SIZE 24

//���ȼ�����
#define MAX_GUILD_LEVEL 99

//�������Գ���
#define MAX_GUILD_FAMILY_DESC_SIZE 60

//������������
#define USER_FAMILY_SIZE 20  

//��������������
#define GUILD_FAMILY_SIZE 99  

//���ְλ���ֳ���
#define MAX_GUILD_POS_NAME_SIZE 24

//���ְλ����
#define MAX_GUILD_POS_NUM 8

//�������ֳ���
#define MAX_CITY_NAME_SIZE 24

//�����н�������������
#define MAX_CITY_BUILDING_NUM 20

//�����������
#define MAX_GUILD_DESC_SIZE 60

//�����������
#define MAX_GUILD_DESC_SIZE_DB 62


//����б�ÿ�εõ���������
#define MAX_GUILD_LIST_COUNT 50

#define MAX_PROPOSER_IN_GUILD		10
#define MAX_USER_IN_GUILD			160
#define USER_ARRAY_SIZE				MAX_USER_IN_GUILD + MAX_PROPOSER_IN_GUILD

//��������
enum PROXY_CONNECT
{
	PERMIT_PROXY_CONNECT		= 0,	//����ʹ�ô���
	FORBIT_PROXY_CONNECT		= 1,	//������ʹ�ô���
	PERMIT_EDU_PROXY_CONNECT	= 2,	//�����������ʹ�ô���
};

//������������볤��
#define ANASWER_LENGTH_1	4
//���������ͼƬ��С
#define IMAGE_BUF_LENGTH_1	576

//���׵����볤��
#define ANASWER_LENGTH_2	4
//���׵�ͼƬ��С
#define IMAGE_BUF_LENGTH_2	576

// �Ѻö�����
#define MAX_FRIEND_POINT 9999

// �Ѻöȴﵽ��������ʾ
#define FRIEND_POINT_ENOUGH_NOTIFY 100

// ���Ѷ�ÿ����ٵ�����
#define FRIEND_DEL_NEMBER_ONE_DAY 50

// �Ѻöȴ���Ҫ�����������
#define FRIEND_POINT_NEED_REMOVE_VERIFY 9999

//��ϵ�����ޣ��������ѡ������������ˣ�
#define MAX_RELATION_SIZE 270

//��������������
#define MAX_BLACK_SIZE 30

//�������ޣ�����ұ���ɱ���Ժ󣬸�����ϵͳ�Զ���ӵ�����������
#define MAX_ENEMY_SIZE  40

//��������
#define MAX_FRIEND_SIZE   200

// ������鵵���ĳ��ȣ��������ڽи���ǩ��
#define MOOD_DATA_SIZE 32

// �����������֧�ֳ���
#define MAX_MOOD_SIZE 25

// �����ͽ����
#define MAX_PRENTICE_COUNT 2

// ���ʦ�µ���
#define MAX_MORAL_POINT 1000000

//����ʼ������
#define MAX_INITSKILL_COUNT 5

//����ʼ�����
#define MAX_INITABILITY_NUM 5

//����ʼ��ͨ������
#define MAX_INIT_COMMON_SKILL_COUNT 5

//����ʼ��Ʒ
#define MAX_INITITEM_NUM 5

//����ʼ
#define MAX_INIT_EQUIP	5

//���ݿ����Ƴ���
#define		DATABASE_STR_LEN			128
//���ݿ��û�������
#define		DB_USE_STR_LEN				32
//���ݿ����볤��
#define		DB_PASSWORD_STR_LEN			32
//DB ����ʱ����
#define		DB_OPERATION_TIME			500
//�˺ű����ɫ����
#define		DB_CHAR_NUMBER				5
//��ɫ�зǼ�ʱˢ������ͬ����ShareMemory��ʱ����
#define		DB_2_SM_TIME				60000 //60��

//���ݿ����ӳ�Ĭ�����Ӹ���
#define		DB_CONNECTION_COUNT			40

// �������ܽ��ܵĿ����˵���λ���������λ�Ĳ��
#define		DEF_SERVER_ADJUST_POS_WARP_DIST	(5.f)

// ���Ϊһ����Ʒ�����������Ҫ���ĵľ���
#define		LOCK_A_OBJ_NEED_ENERGY		10

// add by gh 2010/06/22
#define		 MAX_SUB_TYPE0					4
#define		 MAX_SUB_TYPE1					3
#define		 MAX_SUB_TYPE2					7
#define		 MAX_SUB_TYPE3					MAX_SUB_TYPE2
// end of add
enum CL_NetProvider
{
	CL_NP_DEF =0,		//!< Ĭ��
	CL_NP_TEL,			//!< ����
	CL_NP_CNC,			//!< ��ͨ
	CL_NP_EDU,			//!< ������

	CL_NP_UNKNOWN,		//!< δ֪
};

enum	UserPrizeResult
{
	UPR_SUCCESS,				//�齱��Ϣ�ɹ�
	UPR_ASKPOINT_SUCCESS,		//��ѯ�����ɹ�
	UPR_ASKBUY_SUCCESS,			//����ɹ�

	UPR_ERR_NO_PRIZE,			//û���н�
	UPR_ERR_PRE_REQUEST,		//���ڴ����ϴε�������Ϣ
	UPR_ERR_PRIZE_BUSY,			//�ȴ�������콱��Ϣ����
	UPR_ERR_TIME_OUT,			//�����콱��Ϣ��ʱ
	UPR_ERR_EXPIRE_PRIZE,		//��Ʒ����
	UPR_ERR_CANT_NOW,			//���ڲ��ܴ���齱����
	UPR_ERR_NOENOUGH_POINT,		//��������
	UPR_ERR_GOODSCODE_ERR,		//��Ʒ�������
	UPR_ERR_ALREADYGET_PRIZE,	//�Ѿ��콱

	UPR_NEWUSERCARD_SUCCESS,	//���ֿ��ɹ�
	UPR_ERR_WRONGCARDNUMBER,	//���Ŵ���
	UPR_ERR_OTHERUSERUSE,		//�������Ѿ�ʹ��

	UPR_NEWPRIZE_SUCCESS,		//�齱�ɹ�
	UPR_ERR_INVALID_SERVER,		//���ǹ涨���콱������

	UPR_ERR_OVERLEVEL10,		//����10������ʹ��

	UPR_NEWCHECKPRIZE_SUCCESS,	//��ѯ��Ʒ�ɹ�
	UPR_ERR_NO_ENOUGH_ITEM_POS,	//��Ʒ�ռ䲻��
	UPR_ERR_NO_ITEM_PRIZE,			//û�н�Ʒ
};

enum PRIZE_ASK_TYPE
{
	ASK_INVALID,		//��
	ASK_PRIZE,			//CD-KEY��ȡ
	ASK_POINT,			//���������ѯ
	ASK_BUY,			//��������
	ASK_NEWPRIZE,		//�³齱����
	ASK_NEWUSER_CARD,	//���ֿ�
	ASK_NEWCHECKPRIZE,	//�½�Ʒ��ѯ

	ASK_NUMBER,
};

// MagicInt�㷨����
enum MAGIC_INT_METHOD
{
	MAGIC_INT_TYPE1,				//MagicInt�㷨1
	MAGIC_INT_TYPE2,				//MagicInt�㷨2
};

//////////////////////////////////////////////////////////////////////////
//����

//��������
#define     DELATE_GOLD                 600000

//��ѡ��������
#define     KINDOM_GOLD                 1000000


//������������ӵ�е�ѧϰ���Ē���¼�ĸ���
#define	 MAX_SOUXIA_CONTAINER		32		

#define	 MAX_SKILL_COUNT			60	// ���ļ�������
#define	 MAX_PRODUCT_COUNT			10  // ������������
#define	 MAX_PET_ZHAOHUAN_COUNT		80  // ���������ٻ���������
#define	 MAX_ZUOJI_ZHAOHUAN_COUNT	MAX_PET_ZHAOHUAN_COUNT // ���������ٻ���������

//�����������������ݰ�����������
#define MAX_MEMORY_BASE_COUNT		256

//װ�������ĵȼ�������С��40
#define	EQUIP_REFINING_MIN_LEVEL	40
/////////////////////////////////////////////////////////////////////////////////
//��Ϸ�������ݲ�����
/////////////////////////////////////////////////////////////////////////////////

//��GUID_t��ȡ�������
#define GETWORLD(u) ((u)/10000000)
//��GUID_t��ȡ���û��������к�
#define GETUSER(u) ((u)%10000000)
//�жϵ�ǰ�Ƿ��ǲ�������
#define ISTESTWORLD(w) ((w)>200?1:0)
//�ж�������Ƿ�Ϸ�
#define ISVALIDWORLD(w) (((w)>0)?(((w)<MAX_WORLD)?1:0):(0))


#define ABS(m) ((m)>0?(m):(m)*(-1))

#define LENGTH(x0,z0,x1,z1)  (ABS((x0)-(x1))+ABS((z0)-(z1)))

#define MAX_FILE_PATH  260

#define MAKE_COLOR(r,g,b,a)		(((r&0xFF)<<24)|((g&0xFF)<<16)|((b&0xFF)<<8)|(a&0xFF))
#define COLOR_R(color)			((color>>24)&0x000000FF)
#define COLOR_G(color)			((color>>16)&0x000000FF)
#define COLOR_B(color)			((color>>8)&0x000000FF)
#define COLOR_A(color)			((color)&0x000000FF)

#define	MIN_LIMIT_VALUE(__test_value_, __min_value_)	\
	(__test_value_ < __min_value_ ? __min_value_ : __test_value_)
#define	MINLV0(__test_value_)	MIN_LIMIT_VALUE(__test_value_, 0)

#define	MAX_LIMIT_VALUE(__test_value_, __max_value_)	\
	(__test_value_ > __max_value_ ? __max_value_, __test_value_)


#endif
