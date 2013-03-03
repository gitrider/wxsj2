/********************************************************************************
 *	�ļ�����	AbilityGeneric.h
 *	ȫ·����	d:\Prj\Server\Server\Ability\AbilityGeneric.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 3 �� 23 ��	14:50
 *
 *	����˵����	1��ͨ������ܣ�������һЩ�޽�������ϵͳ��ʵ�ֵ�һЩû�и����߼��Ĺ���
 *				2��ͨ�úϳɼ��ܺ�ͨ�òɼ����ܣ�
 *				�����ڿ���ֻͨ������ܱ����ݶ�����ȷ���߼�����������ʹ��
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __ABILITYGENERIC_H__
#define __ABILITYGENERIC_H__

#include "Ability.h"
#include "AbilityCompound.h"
#include "AbilityGather.h"

class AbilityGeneric : public Ability
{
public:
	AbilityGeneric(const Ability& a) : Ability(a) {}

private:
};

class AbilityGenericCompound : public AbilityCompound
{

public:
	AbilityGenericCompound(const Ability& a) : AbilityCompound(a) {}

private:
};

class AbilityGenericGather:public AbilityGather
{

public:
	AbilityGenericGather(Ability& a):AbilityGather(a) {}

};

#endif // __ABILITYGENERIC_H__
