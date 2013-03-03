// Obj_Static.h
//
//////////////////////////////////////////////////////

#ifndef __OBJ_STATIC_H__
#define __OBJ_STATIC_H__

#include "Type.h"
#include "Obj.h"

struct _OBJ_STATIC_INIT : public _OBJ_INIT
{
	_OBJ_STATIC_INIT( VOID )
	{
	}

	virtual VOID CleanUp( VOID )
	{
		_OBJ_INIT::CleanUp();
	}
};

class Obj_Static : public Obj
{
public :
	enum StaticClass
	{
		ITEM_OBJ_STATIC_1 = 0 ,
		ITEM_OBJ_STATIC_PLATFORM,		// ���������ĸ���ƽ̨��������ҩ���õ�����¯��
		ITEM_OBJ_STATIC_PLANTFIELD ,	// ��ֲ���������Ҫ�����
		ITEM_OBJ_STATIC_4 ,
	};

public :
	Obj_Static( ) ;
	~Obj_Static( ) ;

	virtual ObjType			GetObjType( VOID )const{ return OBJ_TYPE_INVALID; }

	virtual BOOL			HeartBeat( UINT uTime = 0 );
};

#endif	// __OBJ_STATIC_H__
