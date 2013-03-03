#ifndef __LOGINDATASTRUCTDEF_H__
#define __LOGINDATASTRUCTDEF_H__

#define  LOGINSERVER_IN_AREA_COUNT 6

// login server 状态.
enum LOGIN_SERVER_STATUS
{
	LOGIN_SERVER_FULL	= 0, // 爆满
	LOGIN_SERVER_BUSY,		 // 拥挤
	LOGIN_SERVER_NORMAL,	 // 正常
	LOGIN_SERVER_IDLE,		 // 空闲
	LOGIN_SERVER_STOP,		 // 维护

};

// 网络接入商
enum NET_PROVIDE
{
	NET_PROVIDE_CNII	= 0, // 电信
	NET_PROVIDE_CNC,		 // 网通
	NET_PROVIDE_OTHER,		 // 其他
};

// login server 结构信息
struct __LOGINSERVER_INOF
{

	__LOGINSERVER_INOF()
	{
		memset(this, 0, sizeof(__LOGINSERVER_INOF));
	}

	char szLoginServerName[128];					// login server的名字
	char szIp[64];									// ip地址				
	int  iPort;										// 端口号
	LOGIN_SERVER_STATUS  iLoginServerStatus;		// login server的状态.
	INT	nTuiJian;			//推荐等级
	INT nIsNew;			//是否新服务器
	INT	nServerAreaID;	//服务器ID
	char szTooltips[128];	//tip描述

};
typedef __LOGINSERVER_INOF LOGINSERVER_INOF, *PLOGINSERVER_INOF;

#define MAX_SERVER_EVERY_AREA	6
// 区域服务器结构信息
struct __AREA_INFO	//区域服务器信息
{

	__AREA_INFO()
	{
		memset(this, 0, sizeof(__AREA_INFO));
	}

	char szAreaName[128];			// 区域名字
	NET_PROVIDE iProvide;			// 网络供应商
	int  iLoginServerCount;			// 区域中login server的个数
	LOGINSERVER_INOF LoginInfo[MAX_SERVER_EVERY_AREA];	// login server的具体信息

};
typedef __AREA_INFO AREA_INFO, *PAREA_INFO;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 自动选择服务器需要的数据
//

// login server 服务器的的坐标索引
struct __LOGIN_SERVER_POS
{
	int iAreaIndex;
	int iLoginServerIndex;
};
typedef __LOGIN_SERVER_POS LOGIN_SERVER_POS, *PLOGIN_SERVER_POS;

// Login server 数组
typedef std::vector<LOGIN_SERVER_POS> LOGIN_SERVER_POS_VECTOR;

// 网络供应商信息
struct __PROVIDE_INFO
{
	LOGIN_SERVER_POS_VECTOR	IdleLoginServerVector;
	LOGIN_SERVER_POS_VECTOR	NorLoginServerVector;
	LOGIN_SERVER_POS_VECTOR	BusyLoginServerVector;
	LOGIN_SERVER_POS_VECTOR	FullLoginServerVector;		
};

typedef __PROVIDE_INFO PROVIDE_INFO, *PPROVIDE_INFO;



#endif //__LOGINDATASTRUCTDEF_H__