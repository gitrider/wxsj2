// AI_Monster_Def.h

#ifndef _AI_MONSTER_DEF_H_
#define _AI_MONSTER_DEF_H_

#define 	GET_MONSTER_SCENE(TYPEOFRETURN) \
	Obj_Monster* pMonster = GetCharacter(); \
	if (!pMonster) { Assert(false); return TYPEOFRETURN; } \
	if (false == pMonster->IsAlive()) { return TYPEOFRETURN;} \
	Scene* pScene = pMonster->getScene(); \
	if (!pScene) {	Assert(false); return TYPEOFRETURN; } \

enum MONSTERAI_CHAR_STATUS
{
	MONSTERAI_CHAR_IDLE = 0 ,
	MONSTERAI_CHAR_MOVE ,
	MONSTERAI_CHAR_USESKILL ,

	MONSTERAI_CHAR_NUMBER ,
};

enum MONSTERAI_FLEE_TYPE
{
	MONSTERAI_FLEE_RANDOM = 0,		//�������
	MONSTERAI_FLEE_NEARESTFRIEND,	//������Ķ�������
	MONSTERAI_FLEE_NEARESTPOINT,	//������Ĺ̶�������
};

//enum MONSTERAI_STATUS
//{
//	MONSTERAI_IDLE = 0 ,			//����
//	MONSTERAI_APPROACH  ,			//׷��
//	MONSTERAI_FLEE ,				//����
//	MONSTERAI_USESKILL ,			//ʹ�ü���
//	MONSTERAI_ATTACK ,				//����
//	MONSTERAI_CHANGEENEMY ,			//�ı����
//	MONSTERAI_WAIT ,				//�ȴ�
//	MONSTERAI_FASTFLEE ,			//��������
//	MONSTERAI_ARRIVE ,				//����Ŀ�ĵ�
//	MONSTERAI_FOLLOW ,				//����
//	MONSTERAI_PATROL ,				//Ѳ��
//	MONSTERAI_RETURN,				//����׷��
//	MONSTERAI_DEAD,					//����
//	MONSTERAI_TALK,					//��̸
//
//	MONSTERAI_NUMBER ,				//����AI��״̬����
//};

enum MONSTERAI_ARRIVE_TYPE
{
	MONSTERAI_ARRIVE_ATTACK = 0 ,	//����Ŀ�ĵ�ʱ���й���
	MONSTERAI_ARRIVE_PICKUP ,		//����Ŀ�ĵ�ʱ���е������ʰȡ
	MONSTERAI_ARRIVE_FOLLOW ,		//����Ŀ�ĵ�ʱ���и���

	MONSTERAI_ARRIVE_NUMBER ,
};

enum MONSTERAI_SUMMON_TYPE
{
	MONSTERAI_SUMMON_REGION = 0 ,	//�����ٻ�
	MONSTERAI_SUMMON_TEAM ,			//��Ⱥ�ٻ�

	MONSTERAI_SUMMON_NUMBER,
};


#define  ZERO_VALUE (0.0001f)
#define  MIN_DIST (1.0f)
#define  NORMAL_ATTACK  (0)
#define  MAGIC_ATTACK	(1)
#define  AIPARAM_ARRIVE_DIST (1.0f)
#define  AIPARAM_ARRIVE_DIST_MIN (0.8f)
#define  MODIFYSPEED_STEP (0.2f)
#define  CHANGE_ENEMY_RATE (20)
#define  MAX_TEAMMATE_COUNT (10)
#define  MAX_ENEMY_COUNT (5)
#define  MAX_RAND_TIMES (10)
#define  MAX_FLEE_LENGTH (10)
#define  REFUSEPAOPAO_TIME (10000)
#define	 FX_DELTA (0.5f)
#define	 FZ_DELTA (0.5f)


#endif	

