#include "stdafx.h"
#include "ItemHelper.h"
#include "Rand.h"
//�����Ʒ����ֱ���
#define			MAX_ITEM_RAND_QUOATE	100000000


double	ItemRander::DoubleRand()
{
	
	return g_RandGen.RandDouble();
}

INT		ItemRander::IntRand()
{
	return (INT)(MAX_ITEM_RAND_QUOATE*DoubleRand());
}