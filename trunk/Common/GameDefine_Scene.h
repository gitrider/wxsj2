/////////////////////////////////////////////////////////////////////////////////
//�ļ�����GameDefine_Scene.h
//�������������峡���еĳ�ʼ����Ϣ����
//�޸ļ�¼��2005��11��10����
/////////////////////////////////////////////////////////////////////////////////

#ifndef __GAMEDEFINE_SCENE_H__
#define __GAMEDEFINE_SCENE_H__
//��Ӱ״̬
enum SCENE_STATUS
{
	SCENE_STATUS_SLEEP = 0 ,//û�����õĳ���״̬����û�г������ݡ��������ߵ�״̬
	SCENE_STATUS_SELECT ,	//���������������ڴ�װ��״̬
	SCENE_STATUS_LOAD ,		//��Ҫ��ȡ������Դ�ļ���״̬
	SCENE_STATUS_INIT ,		//��Ҫ��ʼ�������ݵ�״̬
	SCENE_STATUS_RUNNING ,	//�������еĳ���״̬
};
//��������
enum SCENE_TYPE
{
	SCENE_TYPE_INVALID	= -1,
	SCENE_TYPE_GAMELOGIC,		// ��ͨ��Ϸ�߼�����
	SCENE_TYPE_COPY,			// ����
	SCENE_TYPE_BATTLEFIELD,		// ս��
	SCENE_TYPE_ARENA,			// ��̨
	SCENE_TYPE_CIT,				// ����
	SCENE_TYPE_HOME,			// ͥԺ
	SCENE_TYPE_HOUSE,			// ������

	SCENE_TYPE_NUMBERS
};


#endif
