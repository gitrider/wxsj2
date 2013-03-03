
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

//是否执行更严格的策划数据校验
#define CHECK_DATA_TABLE 1
/////////////////////////////////////////////////////////////////////////////////
//游戏基本信息宏定义
/////////////////////////////////////////////////////////////////////////////////
//世界号最大值
#define MAX_WORLD 255

#define MAX_WORLD_USER	3000

//用户角色名字可以输入的最大值
#define MAX_CHARACTER_INPUTNAME 24
//玩家称号长度
#define MAX_CHARACTER_TITLE 34

//玩家账号长度
#define MAX_ACCOUNT_LENGTH	50

//GamePlayer管理器的上限
#define MAX_PLAYER 1024

//玩家池上限
//#define MAX_POOL_SIZE 1280
//玩家池上限
#define MAX_POOL_SIZE (2048)
#define MAX_DUMMY_POOL_SIZE 64
//登陆服务器玩家池
#define MAX_LOGIN_POOL_SIZE 1024*3
//工会上限
#define MAX_GUILD_SIZE 1024

//没节的邮件数
#define MAX_MAILNODE_SIZE 102400

//用户角色名字结构 |CharacterName|@|WorldID|00|
//					角色名字      1  3      2个零
//注意：角色名字后面的后缀：“@WorldID”是在用户创建角色时候自动添加进的；
//		服务器端处理时候都是使用完整名字；
//		客户端显示的时候，如果当前角色是本世界创建，则需去掉后面的后缀；
//用户角色名称的最大值
#define MAX_CHARACTER_NAME 30

#define MAX_TIME  6

#define MAX_COUNTRY_COLLECT_DESC 68

//排行榜名字上限
#define MAX_TOP_LIST_NAME 32

//榜单上最多显示100人
#define TOP_USDR_NUM 100				

//玩家自定义组名字最大值
#define MAX_GROUP_NAME		12

//玩家称号长度
#define MAX_CHARACTER_TITLE 34

#define MAX_TITLE_COMBINATION 10

//NPC（或怪物）名称长度
#define NPC_NAME_LEN 32

//玩家自定义称号ID  列表
#define GUANZHI_TITLE_ID   0   //官职称号ID

#define BANGPAI_TITLE_ID   1   //帮会称号ID

#define JIAZU_TITLE_ID     2   //家族称号ID

#define JIEHUN_TITLE_ID    3   //接婚称号ID

//NPC（或怪物）称号长度
#define NPC_TITLE_LEN 32

//角色的昵称
#define MAX_NICK_NAME 34

//摊位的名字
#define MAX_SHOP_NAME 34

//摊位的名字
#define MAX_STALL_NAME 34

//留言的数目
#define MAX_BBS_MESSAGE_NUM 20

//留言的长度
#define MAX_BBS_MESSAGE_LENGTH 70

//留言作者名长度
#define MAX_BBS_MESSAGE_AUTHORLENGTH 40

//二级密码最短长度
#define MIN_PWD 8

//二级密码的最大长度 + 1
#define MAX_PWD 20

////用户帐号的最大值
//#define MAX_ACCOUNT 12

//用户密码的最大值
#define MAX_PASSWORD 12

//用户帐号的最大值
#define MAX_ACCOUNT		50
#define MAX_BILLACCOUNT 50
#define MAX_REBOOT_INFO	32
#define MAX_EXT_INFO	32

//用户密码的最大值
#define MAX_PASSWORD 12

//飞行的速度缩放
#define DEF_CLIENT_FLY_SPEED_RATE (1.2f)
#define DEF_SERVER_FLY_SPEED_RATE (1.3f)		// 为了保证服务器会比客户端先做完

//最大计费IP大小
#define MAX_BILLING_IP_SIZE  15
//抽奖流水号长度
#define MAX_PRIZE_SERIAL_LENGTH		20
//购买商品代码长度
#define MAX_PRIZE_GOODS_LENGTH	6
//最大md5串长度
#define MAX_MD5_SIZE		 32
//最大email串长度
#define MAX_EMAIL_SIZE		 255
//最大超级密码长度
#define MAX_SUPER_CODE_SIZE	 32
//最大额外信息长度
#define MAX_OTHER_INFO_SIZE	 32
//元宝消费记录最大额外信息长度
#define MAX_COST_OTHER_SIZE	 255

//职业信息最大值
#define MAX_JOBINFO_LENGTH	50
//毕业院校信息最大值
#define MAX_SCHOOLINFO_LENGTH	50
//省份信息最大值
#define MAX_PROVINCE_LENGTH		16
//City信息最大值
#define MAX_CITY_LENGTH		16
//Email信息最大值
#define MAX_EMAIL_LENGTH	50
//心情寄语最大值
#define MAX_LUCKWORD_LENGTH	255

//最多能创建角色的数量
#define MAX_CREATE_PLAYER_NUM 3

//CharacterManager中缺省的ObjID缓存大小
#define DEFAULT_OBJ_COUNT 1024

//
#define ZONE_SIZE	10

//
#define MAX_SEND_TIME 1

//搜索Zone格子的范围,0=1格(仅仅包含自己在的那个格子)，1=9格;2=25格;3=49格;
#define MAX_RADIUS 2

#define MAX_REFESH_OBJ_ZONE_RADIUS		2

#define MAX_REFESH_OBJ_ZONE_ARRAY_SIZE	((MAX_REFESH_OBJ_ZONE_RADIUS - 1) * (MAX_REFESH_OBJ_ZONE_RADIUS - 1) + 1 )

//掉落包拾取的最大距离
#define MAX_BOX_OP_DISTANCE				3.0f

#define ITEM_DROP_DISTANCE  0.6F	//掉落距离

//掉落包拾取的最大距离
#define MAX_DROP_ITEM_OP_DISTANCE				3.0f

//背包的最大容量
#define MAX_BAG_SIZE		184
#define MAX_EXT_BAG_NUM 5 // by soongxl

//背包的最大容量
#define MAX_BASE_BAG_SIZE   184
#define MAX_BASE_BAG_REAL_SIZE 180

#define MAX_SINGLEBAG_SIZE  36

#define MAX_NPC_DISTANCE	5.0

//坐骑的最大容量  [7/8/2010 陈军龙]
#define MAX_MOUNT_SIZE   5

//开宝箱过程中，玩家身上暂放物品容器的大小
#define OPENBOX_CONTAINER_SIZE 2
#define TMP_OPENBOX_CONTAINER_SIZE 20

//Container 偏移
//基本包偏移
#define EXTRA_CONTAINER_OFFSET	180

//基本包偏移
#define BASE_CONTAINER_OFFSET	0

//扩展包偏移
#define EXTRA_BAG1_OFFSET	36

//扩展包偏移
#define EXTRA_BAG2_OFFSET	72

//扩展包偏移
#define EXTRA_BAG3_OFFSET	108

//扩展包偏移
#define EXTRA_BAG4_OFFSET	144

////材料包偏移
//#define MAT_CONTAINER_OFFSET	20
//
////任务包偏移
//#define TASK_CONTAINER_OFFSET	40

//装备偏移量
#define EQUIP_CONTAINER_OFFSET	184

//银行偏移量
#define BANK_CONTAINER_OFFSET	58
#define DB_BAG_POS_START			0
#define	DB_BAG_POS_END				184
#define DB_EQUIP_POS_START			DB_BAG_POS_END//  原来会越界 [6/11/2008 wuwenbin](DB_BAG_POS_END+1)
#define DB_EQUIP_POS_END			(DB_EQUIP_POS_START+HEQUIP_NUMBER)
#define DB_BANK_POS_START			DB_EQUIP_POS_END// 原来会越界 [6/11/2008 wuwenbin](DB_EQUIP_POS_END+1)
#define DB_BANK_POS_END				(MAX_BANK_SIZE+DB_BANK_POS_START-1)


#define MAX_BANK_MONEY		10000000

//错误包格表示
#define	INVALID_BAG_INDEX (-1)

//一个场景最多可以有多少个玩家商店
#define MAX_SHOP_NUM_PER_SCENE 512

//一个玩家最多可以拥有的玩家商店个数
#define MAX_SHOP_NUM_PER_PLAYER 2

//商店收藏夹大小
#define MAX_FAVORITE_SHOPS		10

//一个玩家商店的柜台数
#define MAX_STALL_NUM_PER_SHOP 10

//一个玩家商店的合作伙伴数
#define MAX_PARTNER_PER_SHOP 6

//交易记录最大数量
#define MAX_EXCHANGE_RECORD 40

//管理记录最大数量
#define MAX_MANAGER_RECORD 128

//最大记录长度
#define MAX_RECORD_LEN_ENTRY	128		

//最大记录数
#define MAX_RECORD_NUM_ENTRY	128		

//每页记录数
#define MAX_RECORD_NUM_PER_PAGE	10	

//一个玩家商店的名字长度
#define MAX_SHOP_NAME_SIZE 12

//一个玩家商店的描述内容长度
#define MAX_SHOP_DESC_SIZE 82

//商店的税率
#define	SHOP_TAX 0.03

//银行的最大容量
#define MAX_BANK_SIZE 180

//银行最多可以存的钱数
#define MAX_BANK_MONEY 10000000

//银行初始大小
#define CUR_BANK_SIZE 36

//第一个租赁箱起始索引
#define RENTBOX1_START_INDEX 0

//第二个租赁箱起始索引
#define RENTBOX2_START_INDEX 36

//第三个租赁箱起始索引
#define RENTBOX3_START_INDEX 72

//第四个租赁箱起始索引
#define RENTBOX4_START_INDEX 108

//第五个租赁箱起始索引
#define RENTBOX5_START_INDEX 144

//交易盒的最大容量
#define	MISSION_BOX_SIZE	4

//任务列表界面物品的最大数量
#define	QUESTLOG_BOX_SIZE	255

//交易盒的最大容量
#define	EXCHANGE_BOX_SIZE	5

//交易盒中宠物的最大容量
#define	EXCHANGE_PET_BOX_SIZE	3

//交易双方允许的最大相距距离
#define EXCHANGE_MAX_DISTANCE	8	// modify by gh for new 45->8 2010/04/28

//摊位盒的最大容量
#define	STALL_BOX_SIZE		36

//摊位盒的最大容量(一定要跟STALL_BOX_SIZE一样大！！！)
#define	STALL_PET_BOX_SIZE		20

//缓存的申请者队列的长度
#define	MAX_EXCHANGE_APPLICANTS_NUM	10

//称号的最大容量
#define MAX_STRING_TITLE_SIZE 4

//动态称号的最大容量
#define MAX_DYNAMIC_TITLE_SIZE 256

//称号的最大容量
#define MAX_TITLE_SIZE 20 // 称号的数据太大了，先临时改小 [6/10/2008 wuwenbin]256

//无效的称号ID
#define INVALID_TITLE_ID -1

//静态态称号的最大容量
#define MAX_TITLE_ID_SIZE 16

//物品可镶嵌宝石的最大个数
#define MAX_ITEM_GEM 7

//物品可镶嵌宝石的最大个数
#define MAX_GEM_HOLE 7

////最大蓝色装备随机属性个数
//#define		MAX_BLUE_ITEM_EXT_ATTR				4

//最大装备天然属性（没有镶嵌宝石）
#define		MAX_ITEM_NATUREATTR		8

//最大职业数量
#define     MAX_JOB_SUM				(MATTRIBUTE_NUMBER)

//最大基本属性个数(远程外功攻击、远程外功防御、远程内功攻击、远程内功防御、近程外功攻击、近程外功防御、近程内功攻击、近程内功防御)
#define MAX_BASE_ITEM_ATTR				8		

//add by gh for Soul_bead 2010/06/24
//力道,身法,内力,体质,近程外功攻击力,远程外功攻击力,近程内功攻击力,远程内功攻击力
#define MAX_WU_SOUL_BEAD_ATTR		8
//力道,身法,内力,体质,近程外功防御,远程外功防御,近程内功防御,远程内功防御
#define MAX_QI_SOUL_BEAD_ATTR		8
// end of add
//最大物品属性
#define MAX_ITEM_ATTR (MAX_BASE_ITEM_ATTR+MAX_ITEM_NATUREATTR)

//最大装备附加属性个数
#define		MAX_EQUIP_EXT_ATTR					8

//最大等级随机数
#define  MAX_RAND_LEVEL_NUM						10

//最大装备附加属性个数
#define		MAX_EQUIP_LEVEL						100

//最大普通装备个数
#define		MAX_COMMON_EQUIP_COUNT				50

//最大装备等级段外形个数
#define		MAX_EQUIP_MODEL						100

//最大装备影响技能数目
#define		MAX_EQUIP_EFFECT_SKILL				4

//最大装备附加技能数目
#define		MAX_EQUIP_AFFIX_SKILL				4

//最大升星所需宝石数
#define    MAX_LEVELUP_GEMREQ_NUM				4

//最大装备星级
#define    MAX_EQUIP_LEVELUP_NUM				15

//最大装备绑定级别
#define    MAX_EQUIP_BIND_LEVEL					1

//一键换装最大个数
#define    MAX_EQUIP_SUIT_NUM					10

#define	   MAX_EQUIPSUIT_POINT_NUMBER			16	

//最大有效材料等级
#define    MAX_VALIDMATERIAL_LEVEL				15		

//目前开放的材料等级
#define    MAX_MATERIAL_LEVEL					10

//装备制造最大所需材料种类
#define    MAX_CREATE_MATERIAL_NUM				4

//装备改造最大所需材料种类
#define    MAX_REMOULD_MATERIAL_NUM				3

#define    MAX_EXTRA_BAG_NUM					4

#define    MAX_BAG_NUM							(MAX_EXTRA_BAG_NUM+1)

#define	   MAX_SOUL_EFFECTATT					4


//---------------------------------------------------------
//宝石相关

//镶嵌有效装备
#define MAX_BASE_EQUIP_COUNT 38

//宝石的影响属性
#define MAX_GEM_ATT 4

//宝石的影响属性
#define MAX_GEM_REQ 3


//共享内存ItemSerial固定key
#define ITEM_SERIAL_KEY		536081

//宝石的最高级别
#define MAX_GEM_QUALITY 9

//生活技能最大使用背包物品数
#define MAX_ABILITY_ITEM 5

//最大装备套装属性
#define MAX_ITEM_SET_ATTR 4

//最大药品效果
#define	MAX_MEDIC_EFFECT_NUM 3

#define MAX_GROW_POINT_RATE_NUM		4
#define MAX_LOGIC_FUNC_LEN			128
#define ITEM_LOGIC_FUNC_LEN			32
#define MAX_SCENE_GROW_POINT_NUM	128

#define MAX_SCENE_GP_OWNER			255

// 宠物最多拥有的技能数目
#define MAX_PET_SKILL_COUNT		(6)

//Obj_ItemBox 最大掉落物品个数
#define MAX_BOXITEM_NUMBER 10

//Obj_Item 最大掉落物品个数
#define MAX_DROP_ITEM_NUMBER 100

//Obj_ItemBox 最大参与拾取距离
#define MAX_ITEMBOX_PICK_DIST		(20.0f)

//商人可以卖的最大货物个数
#define MAX_BOOTH_NUMBER	   96

//商城可以卖的最大货物个数
#define MAX_IBSHOP_NUMBER	   120

//商人可以回购的最大货物个数
#define MAX_BOOTH_SOLD_NUMBER   12

//
#define MAX_PLAYER_EXP_LEVEL	100

//
#define MAX_100_PLUS_DELTA_LEVEL	201

//
#define DEFAULT_WASHPOINT_LEVEL		60

//最大表定义级别属性点分配
#define MAX_TABLE_DEFINE_LEVEL	 100

//断开网络后，服务器继续保留数据的时间（毫秒）
#define TIME_TO_TRUE_QUIT 10000

//怪物的种类数量最大值
#define	MAXTYPE_NUMBER 10240

//怪物AI种类的数量最大值
#define MAXAI_NUMBER 256

//NPC谈话的类型最大值
#define MAXCHATTYPE_NUMBER 64

//NPC谈话记录的最大值
#define MAXCHAT_NUMBER 64

//服务器名字长度
#define MAX_SERVER_NAME	32


//Npc EventList 数量
#define MAX_NPC_EVENT  25

//角色所拥有技能的数量
#define MAX_CHAR_SKILL_NUM 256

//角色身上可以拥有的附加效果最大数量
#define MAX_IMPACT_NUM 32

//角色所拥有技能心法的数量
#define MAX_CHAR_XINFA_NUM 16

//角色所拥有生活技能的数量modify by gh 64->8,0:占位用
#define MAX_CHAR_ABILITY_NUM		8

//技能配方表的条目数
#define MAX_ABILITY_PRESCRIPTION_NUM (4 * 256)

//技能配方表的存档字节占用数
#define MAX_CHAR_PRESCRIPTION_BYTE (MAX_ABILITY_PRESCRIPTION_NUM>>3)

#define ABILITY_GAMBLE_NUM 5

//最大国家数量
#define MAX_COUNTRY_NUM 4

//最大国王行动力
#define MAX_KINDOM_ACTION_POINT 10000

#define MAX_PREFESSION_NUM 16
//门派数量
#define MAX_MENPAI_NUM 16

//每个门派所拥有的技能心法的数量
#define MAX_MENPAI_XINFA_NUM 16

//总心法数量
#define MAX_ALL_XINFA_NUM	54

//每个心法所能够达到的最大的等级
#define MAX_XINFA_LEVEL_NUM	120

//每个生活技能能够达到的最大等级
#define MAX_ABILITY_LEVEL_NUM 60

//技能最大数量
#define MAX_SKILL_NUMBER 1024

// 总任务数量
#define MAX_MISSION_NUM				(4096)
#define MAX_CHAR_MISSION_FLAG_LEN	((MAX_MISSION_NUM+31)/32)

//角色所拥有任务的数量
#define MAX_CHAR_MISSION_NUM		(20)
enum
{
	QUESTTIME_LIST_SIZE = MAX_CHAR_MISSION_NUM,
};

//每个任务Notify数量
#define MAX_MISSION_NOTIFY_NUM      (4)

//角色所拥有任务数据
#define MAX_CHAR_MISSION_DATA_NUM	(256)

// 每个角色最多能看见多少种任务物品的掉落
#define MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM	(MAX_CHAR_MISSION_NUM*4)

//技能OBJ最大数量
#define MAX_SKILLOBJ_NUMBER			1024

//最大可打开ITEM BOX的距离
#define MAX_OPEN_ITEMBOX_DIST		(2.f)

//最大可打开ITEM BOX的距离平方
#define MAX_OPEN_ITEMBOX_DIST_SQ (MAX_OPEN_ITEMBOX_DIST * MAX_OPEN_ITEMBOX_DIST)

//最大可触发脚本事件的距离(客户端)
#define MAX_CLIENT_DEFAULT_EVENT_DIST (3.f)

//最大可触发脚本事件的距离(服务器)
#define MAX_SERVER_DEFAULT_EVENT_DIST	(MAX_CLIENT_DEFAULT_EVENT_DIST * 2.f)

//最大可触发脚本事件的距离平方(客户端)
#define MAX_CLIENT_DEFAULT_EVENT_DIST_SQ (MAX_CLIENT_DEFAULT_EVENT_DIST * MAX_CLIENT_DEFAULT_EVENT_DIST)

//最大可触发脚本事件的距离平方(服务器)
#define MAX_SERVER_DEFAULT_EVENT_DIST_SQ (MAX_SERVER_DEFAULT_EVENT_DIST * MAX_SERVER_DEFAULT_EVENT_DIST)

//
#define MAX_ATOM_OPT_NUM 8

// 角色接受的最大路径点数量
#define MAX_CHAR_PATH_NODE_NUMBER	(16)

//场景管理模块可以管理的最大场景数量
#define MAX_SCENE 1024

// 一个 zone 里面可以加入的最大 Area 数量
#define MAX_AREA_IN_ZONE		10

//一个副本可以配置的场景最大值
#define MAX_COPYSCENE 128

//场景名字最大长度
#define SCENE_NAME_LEN 16

//最大的队伍人数
#define MAX_TEAM_MEMBER 6

// 团队中最大队伍数量
#define MAX_TEAMS_IN_GROUP 8

//队伍数量
#define MAX_TEAMS 3000

//自建聊天频道内最多人数
#define MAX_CHATCHANNEL_MEMBER 11

//频道数量
#define MAX_CHANNELS 3000

// 最多保存多少条离线邮件
#define MAX_MAIL 20

//邮件内容的最长尺寸
#define MAX_MAIL_CONTEX 256

//怪物泡泡说话的最大长度
#define MAX_MONSTERSPEAK_CONTENT 64

//最小可以分配物品伤血比例
#define MIN_DAMAGE_PERCENT	(0.2f)

//帮会名字长度
#define MAX_GUILD_NAME_SIZE 24

//家族名字长度
#define MAX_GUILD_FAMILY_NAME_SIZE 24

//帮会等级上限
#define MAX_GUILD_LEVEL 99

//家族宣言长度
#define MAX_GUILD_FAMILY_DESC_SIZE 60

//家族人数上限
#define USER_FAMILY_SIZE 20  

//帮会家族数量上限
#define GUILD_FAMILY_SIZE 99  

//帮会职位名字长度
#define MAX_GUILD_POS_NAME_SIZE 24

//帮会职位个数
#define MAX_GUILD_POS_NUM 8

//城市名字长度
#define MAX_CITY_NAME_SIZE 24

//城市中建筑物的最大数量
#define MAX_CITY_BUILDING_NUM 20

//帮会描述长度
#define MAX_GUILD_DESC_SIZE 60

//帮会描述长度
#define MAX_GUILD_DESC_SIZE_DB 62


//帮会列表每次得到的最大个数
#define MAX_GUILD_LIST_COUNT 50

#define MAX_PROPOSER_IN_GUILD		10
#define MAX_USER_IN_GUILD			160
#define USER_ARRAY_SIZE				MAX_USER_IN_GUILD + MAX_PROPOSER_IN_GUILD

//代理类型
enum PROXY_CONNECT
{
	PERMIT_PROXY_CONNECT		= 0,	//允许使用代理
	FORBIT_PROXY_CONNECT		= 1,	//不允许使用代理
	PERMIT_EDU_PROXY_CONNECT	= 2,	//仅允许教育网使用代理
};

//创建人物的掩码长度
#define ANASWER_LENGTH_1	4
//创建人物的图片大小
#define IMAGE_BUF_LENGTH_1	576

//交易的掩码长度
#define ANASWER_LENGTH_2	4
//交易的图片大小
#define IMAGE_BUF_LENGTH_2	576

// 友好度上限
#define MAX_FRIEND_POINT 9999

// 友好度达到上下线提示
#define FRIEND_POINT_ENOUGH_NOTIFY 100

// 好友度每天减少的数量
#define FRIEND_DEL_NEMBER_ONE_DAY 50

// 友好度大到需要输入二级密码
#define FRIEND_POINT_NEED_REMOVE_VERIFY 9999

//联系人上限（包括好友、黑名单、仇人）
#define MAX_RELATION_SIZE 270

//黑名单人数上限
#define MAX_BLACK_SIZE 30

//仇人上限，当玩家被人杀死以后，该人由系统自动添加到仇人名单里
#define MAX_ENEMY_SIZE  40

//好友上限
#define MAX_FRIEND_SIZE   200

// 玩家心情档案的长度：心情现在叫个性签名
#define MOOD_DATA_SIZE 32

// 玩家心情的最大支持长度
#define MAX_MOOD_SIZE 25

// 最大收徒个数
#define MAX_PRENTICE_COUNT 2

// 最大师德点数
#define MAX_MORAL_POINT 1000000

//最大初始技能数
#define MAX_INITSKILL_COUNT 5

//最大初始生活技能
#define MAX_INITABILITY_NUM 5

//最大初始普通技能数
#define MAX_INIT_COMMON_SKILL_COUNT 5

//最大初始物品
#define MAX_INITITEM_NUM 5

//最大初始
#define MAX_INIT_EQUIP	5

//数据库名称长度
#define		DATABASE_STR_LEN			128
//数据库用户名长度
#define		DB_USE_STR_LEN				32
//数据库密码长度
#define		DB_PASSWORD_STR_LEN			32
//DB 操作时间间隔
#define		DB_OPERATION_TIME			500
//账号保存角色个数
#define		DB_CHAR_NUMBER				5
//角色中非即时刷新属性同步到ShareMemory的时间间隔
#define		DB_2_SM_TIME				60000 //60秒

//数据库连接池默认连接个数
#define		DB_CONNECTION_COUNT			40

// 服务器能接受的客气端调整位置与服务器位的差距
#define		DEF_SERVER_ADJUST_POS_WARP_DIST	(5.f)

// 玩家为一个物品（宠物）加锁需要消耗的精力
#define		LOCK_A_OBJ_NEED_ENERGY		10

// add by gh 2010/06/22
#define		 MAX_SUB_TYPE0					4
#define		 MAX_SUB_TYPE1					3
#define		 MAX_SUB_TYPE2					7
#define		 MAX_SUB_TYPE3					MAX_SUB_TYPE2
// end of add
enum CL_NetProvider
{
	CL_NP_DEF =0,		//!< 默认
	CL_NP_TEL,			//!< 电信
	CL_NP_CNC,			//!< 网通
	CL_NP_EDU,			//!< 教育网

	CL_NP_UNKNOWN,		//!< 未知
};

enum	UserPrizeResult
{
	UPR_SUCCESS,				//抽奖信息成功
	UPR_ASKPOINT_SUCCESS,		//查询点数成功
	UPR_ASKBUY_SUCCESS,			//购买成功

	UPR_ERR_NO_PRIZE,			//没有中奖
	UPR_ERR_PRE_REQUEST,		//正在处理上次的请求信息
	UPR_ERR_PRIZE_BUSY,			//等待处理的领奖信息过多
	UPR_ERR_TIME_OUT,			//处理领奖信息超时
	UPR_ERR_EXPIRE_PRIZE,		//奖品过期
	UPR_ERR_CANT_NOW,			//现在不能处理抽奖请求
	UPR_ERR_NOENOUGH_POINT,		//点数不足
	UPR_ERR_GOODSCODE_ERR,		//物品代码错误
	UPR_ERR_ALREADYGET_PRIZE,	//已经领奖

	UPR_NEWUSERCARD_SUCCESS,	//新手卡成功
	UPR_ERR_WRONGCARDNUMBER,	//卡号错误
	UPR_ERR_OTHERUSERUSE,		//其他人已经使用

	UPR_NEWPRIZE_SUCCESS,		//抽奖成功
	UPR_ERR_INVALID_SERVER,		//不是规定的领奖服务器

	UPR_ERR_OVERLEVEL10,		//超过10级不能使用

	UPR_NEWCHECKPRIZE_SUCCESS,	//查询奖品成功
	UPR_ERR_NO_ENOUGH_ITEM_POS,	//物品空间不足
	UPR_ERR_NO_ITEM_PRIZE,			//没有奖品
};

enum PRIZE_ASK_TYPE
{
	ASK_INVALID,		//空
	ASK_PRIZE,			//CD-KEY领取
	ASK_POINT,			//人物点数查询
	ASK_BUY,			//购买请求
	ASK_NEWPRIZE,		//新抽奖请求
	ASK_NEWUSER_CARD,	//新手卡
	ASK_NEWCHECKPRIZE,	//新奖品查询

	ASK_NUMBER,
};

// MagicInt算法类型
enum MAGIC_INT_METHOD
{
	MAGIC_INT_TYPE1,				//MagicInt算法1
	MAGIC_INT_TYPE2,				//MagicInt算法2
};

//////////////////////////////////////////////////////////////////////////
//国家

//弹劾经费
#define     DELATE_GOLD                 600000

//当选国王经费
#define     KINDOM_GOLD                 1000000


//人身上最多可以拥有的学习过的捜侠录的个数
#define	 MAX_SOUXIA_CONTAINER		32		

#define	 MAX_SKILL_COUNT			60	// 最大的技能数量
#define	 MAX_PRODUCT_COUNT			10  // 最大的神器数量
#define	 MAX_PET_ZHAOHUAN_COUNT		80  // 最大的神兽召唤技能数量
#define	 MAX_ZUOJI_ZHAOHUAN_COUNT	MAX_PET_ZHAOHUAN_COUNT // 最大的坐骑召唤技能数量

//记忆棒可以随机的内容包含的最大基数
#define MAX_MEMORY_BASE_COUNT		256

//装备炼化的等级需求最小是40
#define	EQUIP_REFINING_MIN_LEVEL	40
/////////////////////////////////////////////////////////////////////////////////
//游戏基本数据操作宏
/////////////////////////////////////////////////////////////////////////////////

//从GUID_t中取得世界号
#define GETWORLD(u) ((u)/10000000)
//从GUID_t中取得用户部分序列号
#define GETUSER(u) ((u)%10000000)
//判断当前是否是测试世界
#define ISTESTWORLD(w) ((w)>200?1:0)
//判断世界号是否合法
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
