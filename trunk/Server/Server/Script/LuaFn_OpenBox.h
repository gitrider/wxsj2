#ifndef _LUAFN_OPENBOX_H_
#define _LUAFN_OPENBOX_H_
#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SceneManager.h"
#include "HumanItemLogic.h"
#include "OpenBox.h"
#include "GCOpenBox.h"
#include "HumanOpenBox.h"



INT LuaFnProcessOpenBox( Lua_State* L) //��Щ��Ϣ�Ǳ����
{
	LUA_ENTER_FUNCTION
	
	INT sceneId			=	Lua_ValueToNumber(L,1);
	INT selfId			=	Lua_ValueToNumber(L,2);	

	INT index0			=   Lua_ValueToNumber(L,3);	//�м����Ʒ����
	INT index1			=   Lua_ValueToNumber(L,4); //���������������л���ֵ���Ʒ������
	INT num1			=   Lua_ValueToNumber(L,5);
	INT index2			=   Lua_ValueToNumber(L,6);
	INT num2			=   Lua_ValueToNumber(L,7);
	INT index3			=   Lua_ValueToNumber(L,8);
	INT num3			=   Lua_ValueToNumber(L,9);
	INT index4			=   Lua_ValueToNumber(L,10);
	INT num4			=   Lua_ValueToNumber(L,11);
	INT index5			=   Lua_ValueToNumber(L,12);
	INT num5			=   Lua_ValueToNumber(L,13);


	BEGINHUMANDEFINE("LuaFnProcessOpenBox")

	GCOpenBox Msg;

	OpenBox _OpenBox;

	//ÿ�ζ�����һ��Obj_Human�ϵ�����
	HumanOpenBox *pHumanOpenBox = pHuman->GetHumanOpenBox();


	Assert( pHumanOpenBox && "�ò���HumanOpenBox�ڴ�ռ�" );

	pHumanOpenBox->CleanUp();
	//if ( pHuman->GetHumanOpenBox()->GetState() )
	//{
	//	Msg.SetRet( GCOpenBox::OPENBOX_HAVE_OPENED );
	//	pHuman->GetPlayer()->SendPacket(&Msg);
	//	return INVALID_ID;
	//}
	//else
	//{
	//         pHuman->GetHumanOpenBox()->SetState( TRUE );
	//}
	if ( !_OpenBox.Process( index0,
		index1, num1,
		index2, num2,
		index3, num3,
		index4, num4,
		index5, num5 ) )
	{
		Msg.SetRet( GCOpenBox::OPENBOX_ERROR );
		pHuman->GetPlayer()->SendPacket(&Msg);
		return INVALID_ID;

	}

	BOOL bNewPos;
	UINT uContainerPos;
	UINT uQuality = 1;

	ITEM_LOG_PARAM LogParam;

	BOOL bRet = FALSE;

	TmpOpenBoxContainer _TmpOpenBoxContainer;
	ItemContainer* pTmpContainer = _TmpOpenBoxContainer.GetContainer();
	ItemContainer* pContainer = pHumanOpenBox->GetContainer();
	Assert( pTmpContainer );
	Assert( pContainer );

	//�������ĵ���Ʒ���ŵ������� 
	bRet =  g_ItemOperator.CreateItem(  &LogParam,
		_OpenBox.GetItemArray()[OpenBox::CENTER_ARRAY_INDEX].m_ItemID, 
		uQuality,
		pContainer,
		bNewPos,
		uContainerPos,
		HumanOpenBox::CENTER_ITEM_INDEX);

	if ( !bRet )
	{
		Msg.SetRet( GCOpenBox::OPENBOX_ERROR );
		pHuman->GetPlayer()->SendPacket(&Msg);
		return INVALID_ID;

	}

	//����Χ20����Ʒ�洢����ʱ������ 
	int i = 0;
	for ( ; i<TMP_OPENBOX_CONTAINER_SIZE; ++i )			
	{
		bRet =  g_ItemOperator.CreateItem(  &LogParam,
			_OpenBox.GetItemArray()[i+OpenBox::AROUND_INDEX_BEGIN].m_ItemID, 
			uQuality,
			pTmpContainer,
			bNewPos,
			uContainerPos,
			i );

		//Assert( bRet );
		if ( !bRet )
		{
			Msg.SetRet( GCOpenBox::OPENBOX_ERROR );
			pHuman->GetPlayer()->SendPacket(&Msg);
			return INVALID_ID;

		}

	}

	//���������Ʒ�ŵ�������
	if ( !_OpenBox.RandTheAround( pTmpContainer, pContainer ) )
	{
		Msg.SetRet( GCOpenBox::OPENBOX_ERROR );
		pHuman->GetPlayer()->SendPacket(&Msg);
		return INVALID_ID;

	}

	Msg.SetRet( GCOpenBox::OPENBOX_NORMAL );

	pContainer->GetItem(OpenBox::CENTER_ARRAY_INDEX)->SaveValueTo( Msg.GetItem(OpenBox::CENTER_ARRAY_INDEX) );


	for ( i=0; i<TMP_OPENBOX_CONTAINER_SIZE; ++i )
	{
		pTmpContainer->GetItem(i)->SaveValueTo( Msg.GetItem(i+OpenBox::AROUND_INDEX_BEGIN) );

	}

	pHuman->GetPlayer()->SendPacket(&Msg);


	ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION	

		return 1;
}



#endif