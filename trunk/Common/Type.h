//
//�ļ����ƣ�	Type.h
//����������	���Ͷ��壬���ֲ�ͬ����ϵͳ��ϵͳ���õ����ã����е�ǰ����
//				�ڵĴ��붼�����һ�����ô��ļ�
//�汾˵����	Windows����ϵͳ��Ҫ����꣺__WINDOWS__
//				Linux����ϵͳ��Ҫ����꣺__LINUX__
//
//�޸������	2005.3.2 �����ļ�
//				2005.3.9 Version 1.0.0 ��ɻ�������
//				2005.3.25 Version 1.0.1 ���GUID_t GameDefine.h ������
//				2005.11.1 �޸����ݣ�����BaseType.h ������
//

#ifndef __TYPE_H__
#define __TYPE_H__

//��ǰλ�ÿ��Զ������ϵͳʹ�����������Ҳ�����ڹ����ļ���Ԥ����
//#ifndef __WINDOWS__
//	#define __WINDOWS__
//#endif
//#ifndef __LINUX__
//	#define __LINUX__
//#endif

//

#include <vector>
#include "BaseType.h"




///////////////////////////////////////////////////////////////////////
//�����������Ͷ���
///////////////////////////////////////////////////////////////////////


typedef SHORT	ID_t;
typedef USHORT	SMUID_t;
typedef	ULONG	SM_KEY;
typedef UINT	ObjID_t;			//�����й̶�������OBJӵ�в�ͬ��ObjID_t
									//

typedef INT				PrescriptionID_t;	//�ϳ��䷽��ÿһ����һ�� ID
typedef ID_t			AbilityID_t;		//����� ID ��
typedef ID_t			TeamID_t;			//����ID
typedef ID_t			SceneID_t;			//����ID
typedef ID_t			PlayerID_t;			//�������
typedef ID_t			ZoneID_t;			//����
typedef ID_t			SkillID_t;			//����
typedef ID_t			ActionID_t;			//������ID
typedef ID_t			ImpactID_t;			//Ч��ID
typedef ID_t			ImpactClassID_t;	//Ч������ID
typedef ID_t			Coord_t;			//���������
typedef INT				ScriptID_t;			//�ű�
typedef INT				MissionID_t;		//����
typedef ID_t			GroupID_t;			//��ID
typedef ID_t			ChannelID_t;		//Ƶ��ID
typedef ID_t			CooldownID_t;   //��ȴʱ��ID
typedef ID_t			XinFaID_t;			//�ķ�ID
typedef ID_t			CampID_t;				//��ӪID
typedef ID_t			MenPaiID_t;			//����ID
typedef ID_t			GuildID_t;			//����ID
typedef ID_t			CityID_t;			//����ID
typedef ID_t			TopListID_t;		//���а�ID

//by soongxl
typedef ID_t           CountryID_t;     //����ID
typedef ID_t           JiaZuID_t;// ����ID

typedef INT				Time_t; //ʱ������

typedef struct _Int64
{
	LONG				a;
	ULONG				b;
}Int64;





typedef UINT			GUID_t;	//32λΨһ��š�
								//������ڱ�ʾ�û�ΨһID�ţ��û�����Ϊ9λ
								//ǰ��λΪ�����,�����0��200Ϊ��ʽ���ݣ�
								//					   201��MAX_WORLD-1Ϊ��������
								//����Ų��ܳ�������� MAX_WORLD
								//ͨ�� GETWORLD ����Դ�GUID_t��ȡ�������

#define INVALID_GUID			((GUID_t)(-1))
#define INVALID_INDEX			(-1)
#define INVALID_ID_EX			-2

#if defined(__LINUX__) || defined(__WIN_CONSOLE__)
	typedef UCHAR  BYTE;
	typedef USHORT WORD;
	typedef	ULONG  DWORD;
#endif


#if defined(__WINDOWS__)
	typedef DWORD		TID ;
	//����������Ҹ�����ģ���Ƿ�ɼ� 
	typedef unsigned __int64 MODEL_PART;
#elif defined(__LINUX__)
	typedef pthread_t	TID ;
	typedef unsigned long long MODEL_PART;
	typedef const char* LPCSTR;
#endif

#if defined(__LINUX__)
	typedef		INT		SMHandle;
#elif defined(__WINDOWS__)
	typedef		VOID*	SMHandle;
#endif

typedef		VOID*	DBHandle;




//�ж�ĳλ�Ƿ���
//15.14....3.2.1.0 
#define ISSET0(x) ((x)&0x1)
#define ISSET1(x) ((x)&0x2)
#define ISSET2(x) ((x)&0x4)
#define ISSET3(x) ((x)&0x8)
#define ISSET4(x) ((x)&0x10)
#define ISSET5(x) ((x)&0x20)
#define ISSET6(x) ((x)&0x40)
#define ISSET7(x) ((x)&0x80)
#define ISSET8(x) ((x)&0x100)
#define ISSET9(x) ((x)&0x200)
#define ISSET10(x) ((x)&0x400)
#define ISSET11(x) ((x)&0x800)
#define ISSET12(x) ((x)&0x1000)
#define ISSET13(x) ((x)&0x2000)
#define ISSET14(x) ((x)&0x4000)
#define ISSET15(x) ((x)&0x8000)


///////////////////////////////////////////////////////////////////////
//����ͷ�ļ�����
///////////////////////////////////////////////////////////////////////

#include "Assertx.h"
#include "PlayerStatus.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "GameDefine_Attr.h"
#include "GameDefine_Item.h"
#include "GameDefine_Result.h"
#include "GameDefine_Scene.h"
#include "GameDefine_Guild.h"
#include "GameDefine_City.h"
#include "GameDefine_PlayerShop.h"
#include "GameStruct.h"
#include "GameUtil.h"
#include "ResultDefine.h"
#include "PacketDefine.h"

#if defined(GAME_CLIENT)
	#define __ENTER_FUNCTION 
	#define __LEAVE_FUNCTION
#elif 1

#if defined(__WINDOWS__)
	#if defined(NDEBUG)
		#define __ENTER_FUNCTION {try{
		#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
	#else
		#define __ENTER_FUNCTION {try{
		#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
	#endif
#else	//linux
	#define __ENTER_FUNCTION {try{
	#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__PRETTY_FUNCTION__);}}
#endif 
#endif



#endif
