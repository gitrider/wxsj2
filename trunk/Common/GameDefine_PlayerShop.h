/********************************************************************************
 *	�ļ�����	GameDefine_PlayerShop.h
 *	ȫ·����	d:\Prj\Common\GameDefine_PlayerShop.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006-5-8
 *
 *	����˵����	����̵�ͻ��˷��������õĶ���
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __GAMEDEFINE_PLAYERSHOP_H__
#define __GAMEDEFINE_PLAYERSHOP_H__

namespace PLAYER_SHOP
{
	enum STATUS_STALL
	{
		STALL_INVALID = 0,					//�˹�̨��û�п��Ÿ����ʹ��
		STALL_CLOSE,						//�˹�̨�Ѿ�����
		STALL_OPEN,							//�˹�̨�Ѿ�����
	};
}

#define MAX_DY_GROUP_PRICE_NUM            10      //��̬�̵�����Ʒ�۸���������
#define MAX_DY_PER_GROUP_GOODS_NUM        10      //��̬�̵�ÿ�鷷����Ʒ����

enum    DY_PRICE_TYPE                       //��̬�̵�۸�����
{
    PRICE_GOLD = 0,                         //��Ϸ��
    PRICE_CREDIT,                           //����
    PRICE_HONOR,                            //����
    PRICE_GP,                               //�ﹱ

    PRICE_ATTRIB_STR,					    //���� ����
    PRICE_ATTRIB_CON,					    //���� ����
    PRICE_ATTRIB_INT,					    //���� ����
    PRICE_ATTRIB_DEX,					    //�� ����
    PRICE_ATTRIB_POINT_REMAIN,			    //ʣ�����
    PRICE_ATTRIB_SKILLPOINT_REMAIN,         //ʣ�༼�ܵ�

    PRICE_ATTRIB_ATT_NEAR,                  //���̹���
    PRICE_ATTRIB_ATT_FAR,                   //Զ�̹���
    PRICE_ATTRIB_DEF_NEAR,                  //���̷���
    PRICE_ATTRIB_DEF_FAR,                   //Զ�̷���
    PRICE_ATTRIB_ATT_MAGIC_NEAR,			//�����ڹ�������
    PRICE_ATTRIB_DEF_MAGIC_NEAR,			//�����ڹ�������
    PRICE_ATTRIB_MAXHP,				        //�������ֵ
    PRICE_ATTRIB_MAXMP,				        //���ħ��ֵ

    PRICE_RMB,                              //�����
    PRICE_TYPE_NUM,
};

#endif // __GAMEDEFINE_PLAYERSHOP_H__
