�ű�ϵͳC���������ĵ�	LuaFnTbl_Ability.h
	 */
	�ú���ע��Ϊ GetHumanGUID
	INT	LuaFnGetHumanGUID(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetGemEmbededCount
	*     ����˵�� : 
	*     ����˵�� :
	*/
	�ú���ע��Ϊ GetGemEmbededCount
	INT LuaFnGetGemEmbededCount(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-17
	*     �������� : LuaFnGetBagItemParam
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				UINT				StartPos
	*				INT					Type
	*     ����˵�� :
	*		��ȡ��������Ʒ�Ĳ���
	*/
	�ú���ע��Ϊ GetBagItemParam
	INT LuaFnGetBagItemParam(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-17
	*     �������� : LuaFnSetBagItemParam
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				UINT				StartPos
	*				INT					Type
	*				INT					Value
	*     ����˵�� :
	*		���ñ�������Ʒ�Ĳ���
	*/
	�ú���ע��Ϊ SetBagItemParam
	INT LuaFnSetBagItemParam(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-17
	*     �������� : LuaFnGetBagItemDur
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     ����˵�� :
	*		��ȡ��������Ʒ���;ö�
	*/
	�ú���ע��Ϊ GetBagItemDur
	INT LuaFnGetBagItemDur(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-17
	*     �������� : LuaFnSetBagItemDur
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				INT					Dur
	*     ����˵�� :
	*		���ñ�������Ʒ���;ö�
	*/
	�ú���ע��Ϊ SetBagItemDur
	INT LuaFnSetBagItemDur(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :�����
	*     ��дʱ�� : 2008-4-21
	*     �������� : LuaFnSetBodyItemDur
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				Pos
	*				INT					Dur
	*     ����˵�� :
	*		�������������Ʒ���;ö�
	*/
	�ú���ע��Ϊ LuaFnSetBodyItemDur
	INT LuaFnSetBodyItemDur(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :�����
	*     ��дʱ�� : 2008-4-21
	*     �������� : LuaFnGetBodyItemDur
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				Pos
	*				INT					Dur
	*     ����˵�� :
	*		�õ����������Ʒ���;ö�
	*/
	�ú���ע��Ϊ LuaFnGetBodyItemDur
	INT LuaFnGetBodyItemDur(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :�����
	*     ��дʱ�� : 2008-4-21
	*     �������� : LuaFnGetNotBindItemCountInBody
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*     ����˵�� :
	*		�õ��������δ����Ʒ��
	*/
	�ú���ע��Ϊ LuaFnGetNotBindItemCountInBody
	INT LuaFnGetNotBindItemCountInBody(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :�����
	*     ��дʱ�� : 2008-4-21
	*     �������� : LuaFnGetNotBindItemCountInBag
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*     ����˵�� :
	*		�õ���ұ�����δ����Ʒ��
	*/
	�ú���ע��Ϊ LuaFnGetNotBindItemCountInBag
	INT LuaFnGetNotBindItemCountInBag(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-4-21
	*     �������� : LuaFnDropNotBindItemInBag
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				DropIndex			����λ��
	*     ����˵�� :
	*		��ұ�����δ����Ʒ����
	*/
	�ú���ע��Ϊ LuaFnDropNotBindItemInBag
	INT LuaFnDropNotBindItemInBag(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-4-21
	*     �������� : LuaFnDropNotBindItemInBody
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*     ����˵�� :
	*		�������δ����Ʒ����
	*/
	�ú���ע��Ϊ LuaFnDropNotBindItemInBody
	INT LuaFnDropNotBindItemInBody(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-3-27
	*     �������� : LuaFnGetBagItemIdent
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     ����˵�� :
	*		��ȡ��������Ʒ�ļ�������
	*/
	�ú���ע��Ϊ GetBagItemIdent
	INT LuaFnGetBagItemIdent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-3-27
	*     �������� : LuaFnSetBagItemDur
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     ����˵�� :
	*		���ñ�������Ʒ�ļ�������
	*/
	�ú���ע��Ϊ SetBagItemIdent
	INT LuaFnSetBagItemIdent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemQuality
	*     ����˵�� : 
	*     ����˵�� :
	*/
	�ú���ע��Ϊ GetItemQuality
	�ú���ע��Ϊ LuaFnGetItemQuality
	INT LuaFnGetItemQuality(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-6	15:04
	 *	�������ƣ�	LuaFnGetItemType
	 *	����˵����
	 *				uSerialNumber
	 *					��Ʒ���к�
	 *
	 *	����˵����	������Ʒ���кŵõ���Ʒ�� Type
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetItemType
	INT LuaFnGetItemType(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-6	14:00
	 *	�������ƣ�	LuaFnGetBagEquipType
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					��ɫ���
	 *				nBagPos
	 *					��������
	 *
	 *	����˵����	ͨ���������Ӳ鵽����Ʒ������װ�����������װ�����򷵻� -1��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetBagEquipType
	INT LuaFnGetBagEquipType(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :�����
	*     ��дʱ�� : 2008-3-20
	*     �������� : LuaFnGetItemIsEmpty
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :�жϱ������Ƿ��п�λ ,���أ�INVALID_INDEX(-1) ��������
	*/
	�ú���ע��Ϊ GetItemIsEmpty
	INT LuaFnGetItemIsEmpty(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemIndex
	*     ����˵�� : 
	*     ����˵�� :
	*/
	�ú���ע��Ϊ GetItemIndex
	INT LuaFnGetItemIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxRequireAbilityID
	*     ����˵�� : 
	*     ����˵�� :
	*				���Obj_ItemBox��Ҫ������ܱ��
	*		SceneID
	*				�������
	*		ItemBoxID
	*				Obj_ItemBox���
	*
	*		����
	*			AbilityID
	*				��Ҫ����ܵı��
	*/
	�ú���ע��Ϊ GetItemBoxRequireAbilityID
	INT	LuaFnGetItemBoxRequireAbilityID(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemToBox
	*     ����˵�� : 
	*     ����˵�� :
	*				�����Ʒ��Obj_ItemBox
	*
	*
	*	  SceneID
	*			����ID
	*	  ItemBoxID
	*			Obj_ItemBox���
	*	  ItemCount
	*			����Obj_ItemBox����Ʒ����,�ܹ����10��
	*	  Item0
	*			0����Ʒ
	*	  ...
	*
	*     Item9 
	*			9����Ʒ
	*     
	*/
	�ú���ע��Ϊ AddItemToBox
	INT LuaFnAddItemToBox(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnItemBoxEnterScene
	*     ����˵�� : 
	*     ����˵�� :
	*				Obj_ItemBox���볡��
	*  
	*	  fx
	*				Obj_ItemBox���볡��X����
	*	  fz	
	*				Obj_ItemBox���볡��Z����
	*	  iBoxType  
	*				Obj_ItemBox����
	*	  SceneID	
	*				�������
	*	  
	*	  ItemCount  
	*				�������Ʒ���� ���10��
	*	  Item0    
	*				�����0����Ʒ
	*	  ������
	*	  Item9		�����9����Ʒ
	*
	*	  ItemBoxID 
	*               ���ص�ItemBoxID
	*/
	�ú���ע��Ϊ ItemBoxEnterScene
	INT LuaFnItemBoxEnterScene(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnQueryHumanAbility
	*     ����˵�� : 
	*     ����˵�� :
	*				��ѯhuman Ability�Ƿ�ѧϰ
	*	
	*		sceneId
	*					�������
	*		selfId
	*					��ɫ���
	*		abilityID
	*					�����ID
	*
	*/
	�ú���ע��Ϊ QueryHumanAbility
	INT LuaFnQueryHumanAbility(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnQueryHumanAbilityLevel
	*     ����˵�� : 
	*     ����˵�� :
	*				��ѯObj_Human����ܵȼ� SceneID,HumanID,AbilityID
	*/
	�ú���ע��Ϊ QueryHumanAbilityLevel
	INT LuaFnQueryHumanAbilityLevel(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetHumanAbilityLevel
	*     ����˵�� : 
	*     ����˵�� :
	*				������������ܼ���,SceneID,HumanID,AbilityID,AbilityLevel
	*/
	�ú���ע��Ϊ SetHumanAbilityLevel
	INT LuaFnSetHumanAbilityLevel(Lua_State* L)


	/** ----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-28
	*     �������� : LuaFnSetAbilityOperaTime
	*     ����˵�� : 
	*     ����˵�� :
	*				��������ܲ���ʱ��
	*
	*	  SceneID
	*				�������
	*	  HumanID
	*				��ɫ���
	*	  OperaTime
	*				����ʱ��
	*
	*
	*
	*/
	�ú���ע��Ϊ SetAbilityOperaTime
	INT LuaFnSetAbilityOperaTime(Lua_State* L)


	/** ----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-28
	*     �������� : LuaFnGetAbilityOperaRobotTime
	*     ����˵�� : 
	*     ����˵�� :
	*				�������ܹһ���ʱ��
	*
	*	  SceneID
	*				�������
	*	  HumanID
	*				��ɫ���
	*    ����
	*	  RobotTime
	*				����ʱ��
	*
	*
	*
	*/
	�ú���ע��Ϊ GetAbilityOperaRobotTime
	INT LuaFnGetAbilityOperaRobotTime(Lua_State* L)


	/** ----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-28
	*     �������� : LuaFnSetAbilityOperaRobotTime
	*     ����˵�� : 
	*     ����˵�� :
	*				��������ܹһ���ʱ��
	*
	*	  SceneID
	*				�������
	*	  HumanID
	*				��ɫ���
	*	  
	*	  RobotTime
	*				����ʱ��
	*
	*
	*
	*/
	�ú���ע��Ϊ SetAbilityOperaRobotTime
	INT LuaFnSetAbilityOperaRobotTime(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-29	15:54
	 *	�������ƣ�	LuaFnSetAbilityExp
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				abilityId
	 *					����ܱ��
	 *				abilityExp
	 *					����ܾ���
	 *
	 *	����˵����	��������ܾ��飬���͵�ʱ�� / 100 ��Ϊ���������㷽��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ SetAbilityExp
	INT LuaFnSetAbilityExp(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetAbilityExpPlus
	*     ����˵�� : 		
	*		SceneID
	*				�������
	*		HumanID
	*				��ɫ���
	*		AbilityID
	*				����ܱ��
	*     ����˵�� :
	*				�������ܾ���
	*     ���㹫ʽ
	*	            1���PlayerAbilityLevel <= C  ��һ�ü��ܾ��� =B
	*				2��� B-(PlayerAbilityLevel-C)*D <0 ����һ�ü��ܾ���=0
	*				3���� ��һ�ü��ܾ���= B-(PlayerAbilityLevel-C)*D ֮��ȡ��
	*/
	�ú���ע��Ϊ GetAbilityExpPlus
	INT	LuaFnGetAbilityExpPlus(Lua_State* L)


		{*/	
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetAbilityExp
	*     ����˵�� : 		
	*		SceneID
	*				�������
	*		HumanID
	*				��ɫ���
	*		AbilityID
	*				����ܱ��
	*     ����˵�� :
	*				�������ܾ���
	*/
	�ú���ע��Ϊ GetAbilityExp
	INT	LuaFnGetAbilityExp(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-17
	*     �������� : LuaFnSetPrescription
	*     ����˵�� : 
	*     ����˵�� :
	*				ѧ��ĳ��ϳ��䷽��ֻ�ܲ���ѧ�ᣬ���ܲ�����������ĳ���䷽������
	*				���䷽���������ɼ��ܷ��������ṩ��
	*/
	�ú���ע��Ϊ SetPrescription
	INT LuaFnSetPrescription(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-17
	*     �������� : LuaFnIsPrescrLearned
	*     ����˵�� : 
	*     ����˵�� :
	*				�ж��Ƿ��Ѿ�ѧ��ĳ���䷽
	*/
	�ú���ע��Ϊ IsPrescrLearned
	�ú���ע��Ϊ LuaFnIsPrescrLearned
	INT LuaFnIsPrescrLearned(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetAbilityObjIndex
	*     ����˵�� : 
	*     ����˵�� :
	*
	*/
	�ú���ע��Ϊ GetAbilityObjIndex
	INT LuaFnGetAbilityObjIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetItemBoxRecycleTimes
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*				���øú�������ID,short
	*				ItemBoxID
	*					Obj_ItemBox ���
	*				RecycleTimes
	*					���մ���
	*				
	*				���itemBox���մ���
	*/
	�ú���ע��Ϊ SetItemBoxRecycleTimes
	INT		LuaFnSetItemBoxRecycleTimes(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxRecycleTimes
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			   ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*			   ����
	*				RecycleTimes
	*					���մ���
	*
	*				����Obj_ItemBox���մ���
	*/
	�ú���ע��Ϊ GetItemBoxRecycleTimes
	INT		LuaFnGetItemBoxRecycleTimes(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetItemBoxOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*				OwnerID
	*					������ID			
	*				����Obj_ItemBox������
	*/
	�ú���ע��Ϊ SetItemBoxOwner
	INT		LuaFnSetItemBoxOwner(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxOpenFlag
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*/
	�ú���ע��Ϊ GetItemBoxOpenFlag
	INT		LuaFnGetItemBoxOpenFlag(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*			����
	*				OwnerID
	*					������ID	
	*
	*				���Obj_ItemBox������
	*/
	�ú���ע��Ϊ GetItemBoxOwner
	INT		LuaFnGetItemBoxOwner(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-20
	*     �������� : LuaFnAddGPOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*			   OwnerID
	*					������ID	
	*
	*				
	*/
	�ú���ע��Ϊ AddGPOwner
	INT		LuaFnAddGPOwner(Lua_State* L) 


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-17	10:56
	 *	�������ƣ�	LuaFnSetItemCreator
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				nBagPos
	 *					��Ʒ�ڱ����е�λ��
	 *				szCreatorName
	 *					����������
	 *
	 *	����˵����	���������ĳ����Ʒ���Ӵ���������
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSetItemCreator
	INT LuaFnSetItemCreator( Lua_State* L )


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-20
	*     �������� : LuaFnDelGPOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*			   OwnerID
	*					������ID	
	*
	*				
	*/
	�ú���ע��Ϊ DelGPOwner
	INT		LuaFnDelGPOwner(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-20
	*     �������� : LuaFnGetGPCountByOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*			   OwnerID
	*					������ID	
	*			  ���ض�ӦOwnerID ���е�gpCount
	*				
	*/
	�ú���ע��Ϊ GetGPCountByOwner
	INT		LuaFnGetGPCountByOwner(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxWorldPosX
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*			����
	*				fX
	*					Obj_ItemBox��������X
	*
	*				���Obj_ItemBox��������X
	*/
	�ú���ע��Ϊ GetItemBoxWorldPosX
	INT		LuaFnGetItemBoxWorldPosX(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxWorldPosZ
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*			����
	*				fZ
	*					Obj_ItemBox��������Z
	*				���Obj_ItemBox��������Z
	*/
	�ú���ע��Ϊ GetItemBoxWorldPosZ
	INT		LuaFnGetItemBoxWorldPosZ(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxMaxGrowTime
	*     ����˵�� : 
	*     ����˵�� :
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ItemBoxID
	*					Obj_ItemBox���
	*			����
	*				maxGrowTime
	*					Obj_ItemBox�������ʱ��
	*
	*					���Obj_ItemBox�������ʱ��
	*/
	�ú���ע��Ϊ GetItemBoxMaxGrowTime
	INT		LuaFnGetItemBoxMaxGrowTime(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetItemBoxMaxGrowTime
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ItemBoxID
	*					Obj_ItemBox���
	*				maxGrowTime
	*					Obj_ItemBox�������ʱ��
	*				����Obj_ItemBox�������ʱ��
	*/
	�ú���ע��Ϊ SetItemBoxMaxGrowTime
	INT		LuaFnSetItemBoxMaxGrowTime(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetItemBoxPickOwnerTime
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ItemBoxID
	*					Obj_ItemBox���
	*				PickProtTime
	*					���ñ���ʱ�䳤��
	*				����Obj_ItemBox�����߱���ʱ��
	*/
	�ú���ע��Ϊ SetItemBoxPickOwnerTime
	INT		LuaFnSetItemBoxPickOwnerTime(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEnableItemBoxPickOwnerTime
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ItemBoxID
	*					Obj_ItemBox���
	*				����ʱ�俪ʼ��ʱ
	*/
	�ú���ע��Ϊ EnableItemBoxPickOwnerTime
	INT		LuaFnEnableItemBoxPickOwnerTime(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnCreateMonsterOnScene
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				MonsterType
	*					��������
	*				fx
	*					λ��x
	*				fz
	*					λ��z
	*				AIType
	*					����AI����
	*				AIScriptID
	*					����AIScriptID
	*
	*				�ڳ����������� SceneID,MonsterType,x,z,AIType
	*/
	�ú���ע��Ϊ CreateMonsterOnScene
	INT	  LuaFnCreateMonsterOnScene(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnCreateMonster
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				MonsterType
	*					��������
	*				fx
	*					λ��x
	*				fz
	*					λ��z
	*				AIType
	*					����AI����
	*				AIScriptID
	*					����AIScriptID
	*				ScriptID
	*					�¼��ű�ID
	*
	*				�ڳ����������� SceneID,MonsterType,x,z,AIType
	*/
	�ú���ע��Ϊ LuaFnCreateMonster
	INT	  LuaFnCreateMonster(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnDeleteMonster
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ObjID
	*					�����
	*				
	*				ɾ����̬���������Ĺ�(��NPC)��ע�⣺������ڵĹֲ��ܱ�ɾ��
	*/
	�ú���ע��Ϊ LuaFnDeleteMonster
	INT	  LuaFnDeleteMonster(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddStorePointOnScene
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*						�������
	*				StorePointType
	*						�ر�������
	*				fx
	*						�ر���x
	*				fz		
	*						�ر���z
	*				
	*				�ڳ��������һ���ر���
	*/
	�ú���ע��Ϊ AddStorePointOnScene
	INT	LuaFnAddStorePointOnScene(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : DelStorePointOnScene
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*						�������
	*				StorePointType
	*						�ر�������
	*				fx
	*						�ر���x
	*				fz		
	*						�ر���z
	*				
	*				�ڳ�����ɾ��һ���ر���
	*/
	�ú���ע��Ϊ DelStorePointOnScene
	INT	LuaFnDelStorePointOnScene(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnFindStorePointOnScene
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*						�������
	*				StorePointType
	*						�ر�������
	*				fx
	*						�ر���x
	*				fz		
	*						�ر���z
	*				
	*				�ڳ��������һ���ر���
	*/
	�ú���ע��Ϊ FindStorePointOnScene
	INT	LuaFnFindStorePointOnScene(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetStorePointType
	*     ����˵�� : 
	*				SceneID
	*						�������
	*				StorePointType
	*						�ر�������
	*				fx
	*						�ر���x
	*				fz		
	*						�ر���z
	*	
	*     ����˵�� :				
	*				�ڳ�������һ���ر�������
	*/
	�ú���ע��Ϊ GetStorePointType
	INT	LuaFnGetStorePointType(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnTryRecieveItem
	*     ����˵�� : 
	*				SceneID
	*						�������
	*				HumanID
	*						������
    *				ItemType
	*						��Ʒ9λ��ʾ��
	*				Quality
	*						��ɫװ��Ʒ�ʶ�,����װ�������һ������
	*					
	*	
	*     ����˵�� :
	*				���Խ�����Ʒ
	*/
	�ú���ע��Ϊ TryRecieveItem
	�ú���ע��Ϊ LuaFnTryRecieveItem
	INT	LuaFnTryRecieveItem(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetHumanWorldX
	*     ����˵�� : 
	*     ����˵�� :
	*				���Obj_Human����������X ����SceneID,HumanID,����WorldX
	*/
	�ú���ע��Ϊ GetHumanWorldX
	INT	LuaFnGetHumanWorldX(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetHumanWorldZ
	*     ����˵�� : 
	*     ����˵�� :
	*				���Obj_Human����������Z ����SceneID,HumanID,����WorldZ
	*/
	�ú���ע��Ϊ GetHumanWorldZ
	INT	LuaFnGetHumanWorldZ(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetStoreMapWorldX
	*     ����˵�� : 

	*				SceneID
	*					�������
	*				HumanID
	*					������
	*				BagIndex
	*					����λ��
	*				fx
	*					�ر�ͼx
	*     ����˵�� :
	*			��òر�ͼλ��X
	*/
	�ú���ע��Ϊ GetStoreMapX
	INT	LuaFnGetStoreMapX(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetStoreMapWorldZ
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				HumanID
	*					������
	*				BagIndex
	*					����λ��
	*				fz
	*					�ر�ͼz
	*			��òر�ͼλ��Z
	*/
	�ú���ע��Ϊ GetStoreMapZ
	INT	LuaFnGetStoreMapZ(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetStoreMapSceneID
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				HumanID
	*					������
	*				BagIndex
	*					����λ��
	*				MapSceneID
	*					�ر�ͼ����
	*     ����˵�� :
	*			��òر�ͼ����ID
	*/
	�ú���ע��Ϊ GetStoreMapSceneID
	INT	LuaFnGetStoreMapSceneID(Lua_State* L) 


	/****************************************************************************
	*	������Ա��	����
	*	��дʱ�䣺	2005-11-1	10:15
	*	�������ƣ�	LuaFnQueryAreaStandingTime
	*	����˵����
	*				sceneId
	*					�������
	*				selfId
	*					������ ID���޷�������
	*
	*	����˵����	��ѯ������¼�����ͣ����ʱ��
	*	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ QueryAreaStandingTime
	INT LuaFnQueryAreaStandingTime(Lua_State* L)


	/****************************************************************************
	*	������Ա��	����
	*	��дʱ�䣺	2005-11-1	11:02
	*	�������ƣ�	LuaFnResetAreaStandingTime
	*	����˵����
	*				sceneId
	*					�������
	*				selfId
	*					������ ID���޷�������
	*				nTimeStampOffset
	*					�µ�ʱ���ƫ���������룩
	*
	*	����˵����	����������ҽ����¼������ʱ�䣨��Ϊ��һ�δ����¼���ʱ�����
	*	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ ResetAreaStandingTime
	INT LuaFnResetAreaStandingTime(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-10	15:06
	 *	�������ƣ�	LuaFnSendAbilitySuccessMsg
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				AbilityId
	 *					����� ID ��
	 *				RecipeId
	 *					�䷽ ID ��
	 *				ItemSerialNum
	 *					��Ʒ���к�
	 *
	 *	����˵����	��ͻ��˷�����Ʒ����ɹ�����Ϣ
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSendAbilitySuccessMsg
	INT LuaFnSendAbilitySuccessMsg(Lua_State* L)


/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-6-7
	*     �������� : LuaFnIsEquipItem
	*     ����˵�� : 
	*				UINT			ItemTableIndex
	*				
	*     ����˵�� :
	*		�Ƿ���װ��
	*/
	�ú���ע��Ϊ IsEquipItem
	INT LuaFnIsEquipItem(Lua_State* L)


	/****************************************************************************
	*	������Ա��	
	*	��дʱ�䣺	2006-8-17	14:00
	*	�������ƣ�	LuaFnGetItemEquipPoint
	*	����˵����
	*
	*	����˵����	����װ����װ���
	*	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ GetItemEquipPoint
	INT LuaFnGetItemEquipPoint(Lua_State* L)


/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-6-7
	*     �������� : LuaFnGeEquipReqLevel
	*     ����˵�� : 
	*
	*				INT				ItemTableIndex
	*				
	*     ����˵�� :
	*		������Ʒ����
	*/
	�ú���ע��Ϊ GeEquipReqLevel
	INT LuaFnGeEquipReqLevel(Lua_State* L)


/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-12-21
	 *	�������ƣ�	LuaFnGetAbilityLevelUpConfig
	 *	����˵����
	 *				abilityId
	 *					�����ID
	 *				level
	 *					��Ҫȡ�õļ���
	 *
	 *	����˵����	���������������Ӧ������
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetAbilityLevelUpConfig
	INT LuaFnGetAbilityLevelUpConfig( Lua_State* L )


	/****************************************************************************
	*	������Ա��	
	*	��дʱ�䣺	2006-12-21
	*	�������ƣ�	LuaFnGetAbilityLevelUpConfig2
	*	����˵����
	*				abilityId
	*					�����ID
	*				level
	*					��Ҫȡ�õļ���
	*
	*	����˵����	���������������Ӧ������
	*	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetAbilityLevelUpConfig2
	INT LuaFnGetAbilityLevelUpConfig2( Lua_State* L )


�ű�ϵͳC���������ĵ�	LuaFnTbl_Attr.h
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetPos
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
    *					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*				x
	*					��Ҫwarp������λ�õ�X����ֵ
	*				z
	*					��Ҫwarp������λ�õ�Z����ֵ
	*     ����˵�� :
	*				��ɫ��ͬ����ת��
	*				���ý�ɫ(��Һ͹���)��ת����ǰ��ͼ�ϵ�ĳ��
	*/
	�ú���ע��Ϊ SetPos
	INT LuaFnSetPos( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetRevPos
	*     ����˵�� : 
	*				L
	*				LuaStateִ�л���ָ��
	*				sceneId
	*				���øú�������ID,short
	*				selfId
	*				������ID,�޷�������
	*				x
	*					λ��x����
	*				z
	*					λ��z����
	*     ����˵�� :
	*				������ҵ������㣨�ٴε��뵱ǰ�����λ�ã�
	*				��������д�ϸ��
	*/
	�ú���ע��Ϊ SetRevPos
	INT LuaFnSetRevPos( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnNewWorld
	*     ����˵�� : 
	*				L
	*				LuaStateִ�л���ָ��
	*				sceneId
	*				���øú�������ID,short
	*				selfId
	*				������ID,�޷�������
	*				targetSceneId
	*				��Ҫ��ת����ID,short
	*				x
	*					λ��x����
	*				z
	*					λ��z����
	*     ����˵�� :
	*				�����л�����
	*				��selfId����Ҵ�sceneId��־�ĳ�����ת��targetSceneId��־�ĳ�����(x,z)λ��
	*				ʾ��:NewWorld(sceneId,selfId,4,100,102)
	*/
	�ú���ע��Ϊ NewWorld
	INT LuaFnNewWorld(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetWorldPos
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
    *				�õ���ɫ(character)��ǰ����
	*				����x��z��FLOAT����ֵ
	*				ʾ��: x,z = GetWorldPos(sceneId,selfId) 
	*/
	�ú���ע��Ϊ GetWorldPos
	�ú���ע��Ϊ LuaFnGetWorldPos
	INT LuaFnGetWorldPos(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �¾�
	*     ��дʱ�� : 2010-6-7
	*     �������� : LuaFnGetSceneNPCPos
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				uDataID
	*					�޷�������
	*     ����˵�� :
    *				�õ���ɫ(character)��ǰ����
	*				����x��z��FLOAT����ֵ
	*				ʾ��: x,z = GetWorldPos(sceneId,npcId) 
	*/
	�ú���ע��Ϊ GetSceneNPCPos
	INT LuaFnGetSceneNPCPos(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetPatrolId
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����Obj_Monster��Ѳ��·��
	*				ʾ��:SetPatrolId(sceneId,selfId,0)
	*				patrolId����Чֵ�� 
	*				0 �� ��sceneId��ָ����_patrolpoint.ini�ļ�PATROLNUMBER(������)֮���ֵ
	*/
	�ú���ע��Ϊ SetPatrolId
	INT LuaFnSetPatrolId(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnIsCaptain
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ�Ϊ�ӳ�������ֵΪ0�����Ƕӳ�������ֵΪ1���ӳ���
	*				û�в���
	*/
	�ú���ע��Ϊ IsCaptain
	INT LuaFnIsCaptain( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����
	*     ����˵�� :
	*				����ǰ��Ҽ�ĳ���ķ�
	*				û�в���
	*/
	�ú���ע��Ϊ AddXinFa
	INT LuaFnAddXinFa( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��
	*     ����˵�� :
	*				����ǰ��Ҽ�ĳ������
	*				û�в���
	*/
	�ú���ע��Ϊ AddSkill
	INT LuaFnAddSkill( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ���ķ�
	*				û�в���
	*/
	�ú���ע��Ϊ DelXinFa
	INT LuaFnDelXinFa( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ������
	*				û�в���
	*/
	�ú���ע��Ϊ DelSkill
	INT LuaFnDelSkill( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnHaveXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����				
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ���ĳ���ķ���δѧ���ü��ܷ���-1�����򷵻��ķ��ȼ�
	*				û�в���
	*/
	�ú���ע��Ϊ HaveXinFa
	INT LuaFnHaveXinFa( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnHaveSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ���ĳ�����ܡ�δѧ���ü��ܷ���-1�����򷵻ؼ��ܵȼ���//gaoqi? ����û�еȼ�
	*/
	�ú���ע��Ϊ HaveSkill
	INT LuaFnHaveSkill( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnUseSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				�д���߻���ȶ
	*/
	�ú���ע��Ϊ UseSkill
	INT LuaFnUseSkill( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-3-20
	*     �������� : LuaFnGetCurCountry
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵĵ�ǰ����
	*				û�в���
	*/
	�ú���ע��Ϊ GetCurCountry
	INT LuaFnGetCurCountry( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetCurCamp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵĵ�ǰ��Ӫ
	*				û�в���
	*/
	�ú���ע��Ϊ GetCurCamp
	INT LuaFnGetCurCamp( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetCurCamp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				campIndex
	*					��Ӫ���,INT����
	*     ����˵�� :
	*				�޸������Ӫ,��������δ��д
	*/
	�ú���ע��Ϊ SetCurCamp
	INT LuaFnSetCurCamp ( Lua_State* L)	


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	19:42
	 *	�������ƣ�	LuaFnGetGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������ߵ� GUID
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetGUID
	INT LuaFnGetGUID( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetExp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵĵ�ǰս������ֵ
	*/
	�ú���ע��Ϊ GetExp
	INT LuaFnGetExp( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddExp
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				exp
	*					����ֵ����,INT
	*     ����˵�� :
	*				������ҵ�ս������ֵ
	*/
	�ú���ע��Ϊ AddExp
	�ú���ע��Ϊ LuaFnAddExp
	INT LuaFnAddExp( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetHp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ�����ֵ
	*				������ҵ�HPֵ
	*/
	�ú���ע��Ϊ GetHp
	INT LuaFnGetHp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreHp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*		
	*				percent
	*					�ָ��İٷֱ�,Ϊ0ʱ��ԭ�����߼�����0ʱֱ��SetMP
	*     ����˵�� :
	*				�ָ���ɫ������ȫ��
	*				�ѽ�ɫ������ֵ��ΪMaxHP*percent/100ֵ
	*/
	�ú���ע��Ϊ RestoreHp
	INT LuaFnRestoreHp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ý�ɫ������ֵ
	*				���ؽ�ɫ��MPֵ
	*/
	�ú���ע��Ϊ GetMp
	INT LuaFnGetMp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreMp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				percent
	*					�ָ��İٷֱ�
	*     ����˵�� :
	*				�ָ���ҵ�����ΪMaxMP*percent/100
	*/
	�ú���ע��Ϊ RestoreMp
	INT LuaFnRestoreMp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetCon
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ�����ֵ
	*/
	�ú���ע��Ϊ GetCon
	INT LuaFnGetCon(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetSex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ��Ա�
	*				����ֵΪ0��Ů
	*				����ֵΪ1����
	*/
	�ú���ע��Ϊ GetSex
	�ú���ע��Ϊ LuaFnGetSex
	INT LuaFnGetSex(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetName
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ý�ɫ(Obj_Character)���֣�����һ���ַ���
	*				��ɫ������Һ�Obj_Monster
	*				ʾ��: npcName = GetName(sceneId,npcId)
	*/
	�ú���ע��Ϊ GetName
	�ú���ע��Ϊ LuaFnGetName
	INT LuaFnGetName(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ��ֽ�
	*				�������ӵ�еĽ�Ǯ��
	*/
	�ú���ע��Ϊ GetMoney
	�ú���ע��Ϊ LuaFnGetMoney
	INT LuaFnGetMoney(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnCostMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				payment
	*					��Ǯ����
	*     ����˵�� :
	*				�۳���ҽ�Ǯ
	*				�ɹ�����1
	*				�Ժ��Ƿ��Ƿ����޸ĺ����ҽ�Ǯ��?
	*				ĿǰpaymentӦ��Ϊ��ֵ
	*				ʧ�ܷ���-1
	*/
	�ú���ע��Ϊ CostMoney
	�ú���ע��Ϊ LuaFnCostMoney
	INT LuaFnCostMoney(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				profit
	*					��Ǯ������Ŀ
	*     ����˵�� :
	*				������ҽ�Ǯ
	*				����1,�ɹ�
	*				�Ժ��Ƿ��Ƿ����޸ĺ����ҽ�Ǯ��?
	*				����-1,ʧ��
	*/
	�ú���ע��Ϊ AddMoney
	�ú���ע��Ϊ LuaFnAddMoney
	INT LuaFnAddMoney(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetLevel
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ȡ����ҵ�ǰ�ȼ�
	*				Level�ĺϷ���Χ��?-?
	*				����-1,ʧ��
	*/
	�ú���ע��Ϊ GetLevel
	�ú���ע��Ϊ LuaFnGetLevel
	INT LuaFnGetLevel(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : ��
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnSetLevel
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				level
	*					��ɫ����,INT
	*     ����˵�� :
	*				���������ҵȼ�
	*				Level�ĺϷ���Χ��1-60
	*				����-1,ʧ��
	*/
	�ú���ע��Ϊ SetLevel
	INT LuaFnSetLevel(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	16:36
	 *	�������ƣ�	LuaFnCanLevelUp
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�жϸ�����Ƿ��������
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnCanLevelUp
	INT LuaFnCanLevelUp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-28
	*     �������� : LuaFnGetItemTableIndexByIndex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemIndex
	*					��Ʒ�����ϵ�λ������
	*     ����˵�� :
	*				ȡ��ǰλ�õ���Ʒ����
	*				
	*/
	�ú���ע��Ϊ GetItemTableIndexByIndex
	�ú���ע��Ϊ LuaFnGetItemTableIndexByIndex
	INT LuaFnGetItemTableIndexByIndex(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-30	14:47
	 *	�������ƣ�	LuaFnGetMaterialStartBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������в��ϵ���ʼλ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetMaterialStartBagPos
	INT LuaFnGetMaterialStartBagPos(Lua_State* L)	


		ENDHUMANDEFINE*/
	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-30	14:47
	 *	�������ƣ�	LuaFnGetMaterialEndBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������в��ϵĽ���λ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetMaterialEndBagPos
	INT LuaFnGetMaterialEndBagPos(Lua_State* L)	


	LUA_LEAVE_FUNCTION*/
	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-30	19:30
	 *	�������ƣ�	LuaFnGetItemCountInBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				uBagPos
	 *					������λ��
	 *
	 *	����˵����	�õ�����ĳ������Ʒ������
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetItemCountInBagPos
	INT LuaFnGetItemCountInBagPos(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ����ж��ٸ�����
	*				ֻ����ҵ�װ������Ʒ���в���
	*/
	�ú���ע��Ϊ GetItemCount
	�ú���ע��Ϊ LuaFnGetItemCount
	INT LuaFnGetItemCount(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-4-22
	*     �������� : LuaFnGetPKValue
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�õ���ҵ�ǰPKֵ
	*/
	�ú���ע��Ϊ LuaFnGetPKValue
	INT LuaFnGetPKValue(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-4-22
	*     �������� : LuaFnSetPKValue
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				Val:
	*					����µ�PKֵ
	*     ����˵�� :
	*				������ҵ�ǰPKֵ
	*/
	�ú���ע��Ϊ LuaFnSetPKValue
	INT LuaFnSetPKValue(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-1-22
	*     �������� : LuaFnHaveItemInBag
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ��Ұ����Ƿ�ӵ��ĳ������
	*				����ɹ�,���ش���0
	*				���ʧ��,����-1
	*/
	�ú���ע��Ϊ HaveItemInBag
	INT LuaFnHaveItemInBag ( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnHaveItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ�ӵ��ĳ������
	*				����ɹ�,���ش���0
	*				���ʧ��,����-1
	*/
	�ú���ע��Ϊ HaveItem
	INT LuaFnHaveItem ( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2008-4-28
	*     �������� : LuaFnUseItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				���ʹ��ĳ������
	*				����ɹ�,���ش���0
	*				���ʧ��,����-1
	*/
	�ú���ע��Ϊ UseItem
	INT LuaFnUseItem ( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*				delCount
	*					ɾ����Ŀ
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ������	
	*				������ָ�������	
	*				������װ���ĵ���ɾ��
	*				ɾ���ɹ�,����1
	*				ɾ��ʧ��,����-1
	*/
	�ú���ע��Ϊ DelItem
	INT LuaFnDelItem ( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEraseItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				bagIndex
	*					�����ĸ���
	*     ����˵�� :
	*				ɾ����ǰ��ұ���ĳһ��ĵ���
	*				ɾ���ɹ�,����1
	*				ɾ��ʧ��,����-1
	*/
	�ú���ע��Ϊ EraseItem
	�ú���ע��Ϊ LuaFnEraseItem
	INT LuaFnEraseItem ( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ȡ��ҵ�ǰ��������ID
	*/
	�ú���ע��Ϊ GetMenPai
	�ú���ע��Ϊ LuaFnGetMenPai
	INT LuaFnGetMenPai(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-13
	*     �������� : LuaFnSetMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				menpaiId
	*					����ID,INT
	*     ����˵�� :
	*				������ҵ�ǰ��������ΪmenpaiId
	*/
	�ú���ע��Ϊ SetMenPai
	INT LuaFnSetMenPai(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : yangei
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiMaster
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ȡ������ڰ��İ������ƣ��ַ�����
	*				������û��ʵ��
	*/
	�ú���ע��Ϊ GetMenPaiMaster
	INT LuaFnGetMenPaiMaster(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiFigure
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��������ڰ���еĽ׼���0���ڣ�1�ӳ���2���ϣ�4����
	*				������û��ʵ��
	*/
	�ú���ע��Ϊ GetMenPaiFigure
	INT LuaFnGetMenPaiFigure(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiTitle
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��������ڰ���е�ͷ�Σ�����ͷ���в����������
	*				������û��ʵ��
	*/
	�ú���ע��Ϊ GetMenPaiTitle
	INT LuaFnGetMenPaiTitle(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDestroyMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				menpaiId
	*					�����
	*     ����˵�� :
	*				��ɢ���
	*				������û��ʵ��
	*/
	�ú���ע��Ϊ DestroyMenPai
	INT LuaFnDestroyMenPai(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetNpcCurCamp
	*     ����˵�� : 
	*     ����˵�� :
	*				�ı�NPC����Ӫ,��������δ��д
	*/
	�ú���ע��Ϊ SetNpcCurCamp
	INT LuaFnSetNpcCurCamp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreNpcCamp
	*     ����˵�� : 
	*     ����˵�� :
	*				�ָ�NPCԭ�е���Ӫ,��������δ��д
	*/
	�ú���ע��Ϊ RestoreNpcCamp
	INT LuaFnRestoreNpcCamp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-1-23
	*     �������� : LuaFnSetDamage
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					�Լ���ObjID
	*				targetId
	*					Ŀ���ObjID
	*				damage
	*					��Ҫ���õ��˺�
	*     ����˵�� :
	*				����ĳ��obj���˺�
	*/
	�ú���ע��Ϊ LuaFnSetDamage
	INT LuaFnSetDamage(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : gaoqi
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGmKillObj
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					�Լ���ObjID
	*				targetId
	*					Ŀ���ObjID
	*     ����˵�� :
	*				ֱ��ɱ��ָ����obj;
	*/
	�ú���ע��Ϊ LuaFnGmKillObj
	INT LuaFnGmKillObj(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-1-23
	*     �������� : LuaFnFindMonsterByGUID
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				guid
	*					��ҪѰ�ҵĹ���GUID
	*     ����˵�� :
	*				Ѱ�ҳ����е�ĳ������,����GUID
	*/
	�ú���ע��Ϊ LuaFnFindMonsterByGUID
	INT LuaFnFindMonsterByGUID(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanMaxVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ������ֵ
	*/
	�ú���ע��Ϊ GetHumanMaxVigor
	INT LuaFnGetHumanMaxVigor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanMaxEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ������ֵ
	*/
	�ú���ע��Ϊ GetHumanMaxEnergy
	INT LuaFnGetHumanMaxEnergy(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ���ֵ
	*/
	�ú���ע��Ϊ GetHumanVigor
	INT LuaFnGetHumanVigor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ���ֵ
	*/
	�ú���ע��Ϊ GetHumanEnergy
	INT LuaFnGetHumanEnergy(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanMaxVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ������ֵ
	*/
	�ú���ע��Ϊ SetHumanMaxVigor
	INT LuaFnSetHumanMaxVigor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanMaxEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ������ֵ
	*/
	�ú���ע��Ϊ SetHumanMaxEnergy
	INT LuaFnSetHumanMaxEnergy(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ���
	*/
	�ú���ע��Ϊ SetHumanVigor
	INT LuaFnSetHumanVigor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ���
	*/
	�ú���ע��Ϊ SetHumanEnergy
	INT LuaFnSetHumanEnergy(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanVigorRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ����ָ��ٶ�
	*/
	�ú���ע��Ϊ GetHumanVigorRegeneRate
	INT LuaFnGetHumanVigorRegeneRate(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanEnergyRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ����ָ��ٶ�
	*/
	�ú���ע��Ϊ GetHumanEnergyRegeneRate
	INT LuaFnGetHumanEnergyRegeneRate(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanVigorRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ����ָ��ٶ�
	*/
	�ú���ע��Ϊ SetHumanVigorRegeneRate
	INT LuaFnSetHumanVigorRegeneRate(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanEnergyRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ����ָ��ٶ�
	*/
	�ú���ע��Ϊ SetHumanEnergyRegeneRate
	INT LuaFnSetHumanEnergyRegeneRate(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanHairColor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ����ɫ
	*/
	�ú���ע��Ϊ SetHumanHairColor
	INT LuaFnSetHumanHairColor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanHairColor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ����ɫ
	*/
	�ú���ע��Ϊ GetHumanHairColor
	INT LuaFnGetHumanHairColor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanHairModel
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				uHairStyle
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ��ģ��
	*/
	�ú���ע��Ϊ SetHumanHairModel
	INT LuaFnSetHumanHairModel(Lua_State* L)	


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-15	14:36
	 *	�������ƣ�	LuaFnChangeHumanHairModel
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				uHairStyle
	 *					����ֵ
	 *
	 *	����˵����	����ҵķ��ͻ��� uHairStyle
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnChangeHumanHairModel
	INT LuaFnChangeHumanHairModel(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanHairModel
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ��ģ��
	*/
	�ú���ע��Ϊ GetHumanHairModel
	INT LuaFnGetHumanHairModel(Lua_State* L)	


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-15	14:26
	 *	�������ƣ�	LuaFnGetHumanGoodBadValue
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ� selfId ���ƶ�ֵ
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetHumanGoodBadValue
	INT LuaFnGetHumanGoodBadValue(Lua_State* L)	


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-15	14:26
	 *	�������ƣ�	LuaFnSetHumanGoodBadValue
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				value
	 *					�ƶ�ֵ
	 *
	 *	����˵����	���� selfId ���ƶ�ֵ
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSetHumanGoodBadValue
	INT LuaFnSetHumanGoodBadValue(Lua_State* L)	


    /****************************************************************************
    *	������Ա��	amos
    *	��дʱ�䣺	2008-6-23	14:26
    *	�������ƣ�	LuaFnSetExchangStatus
    *	����˵����
    *				sceneId
    *					�������
    *				selfId
    *					������ ID���޷�������
    *				
    *					
    *
    *	����˵����	��ս���״̬
    *	�޸ļ�¼��
    *****************************************************************************/
	�ú���ע��Ϊ LuaFnSetExchangStatus
    INT LuaFnSetExchangStatus(Lua_State* L)	


�ű�ϵͳC���������ĵ�	LuaFnTbl_Battle.h
	 */
	�ú���ע��Ϊ LuaFnGetPetCount
	INT	LuaFnGetPetCount(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_DataID
	INT	LuaFnGetPet_DataID(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_Level
	INT	LuaFnGetPet_Level(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_TakeLevel
	INT	LuaFnGetPet_TakeLevel(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_HP
	INT	LuaFnGetPet_HP(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_MaxHP
	INT	LuaFnGetPet_MaxHP(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_Life
	INT	LuaFnGetPet_Life(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_Type
	INT	LuaFnGetPet_Type(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_Generation
	INT	LuaFnGetPet_Generation(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_Happyness
	INT	LuaFnGetPet_Happyness(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_StrPerception
	INT	LuaFnGetPet_StrPerception(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_ConPerception
	INT	LuaFnGetPet_SmartnessPerception(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_DexPerception
	INT	LuaFnGetPet_MindPerception(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_IntPerception
	INT	LuaFnGetPet_ConstitutionPerception(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_GenGu
	INT	LuaFnGetPet_GenGu(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_RemainPoint
	INT	LuaFnGetPet_RemainPoint(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_Exp
	INT	LuaFnGetPet_Exp(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_AttrLevel1
	INT	LuaFnGetPet_AttrLevel1(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnGetPet_Skill
	INT	LuaFnGetPet_Skill(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnCreatePet
	INT	LuaFnCreatePet(Lua_State* L)


	 */
	�ú���ע��Ϊ LuaFnDeletePet
	INT	LuaFnDeletePet(Lua_State* L)


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-05-22
    *     �������� : LuaFnGetPKLevel
    *     ����˵�� :
    *				L
    *					LuaStateִ�л���ָ��
    *				sceneId
    *					������
    *				selfId
    *					���ID
    *				deltaLevel
    *					�ȼ���
    *     ����˵�� :
    *				���ݵȼ����õȼ�����
    */
	�ú���ע��Ϊ LuaFnGetPKLevel
    INT LuaFnGetPKLevel(Lua_State* L)	


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-05-22
    *     �������� : LuaFnGetPKPoint
    *     ����˵�� :
    *				L
    *					LuaStateִ�л���ָ��
    *				sceneId
    *					������
    *				selfId
    *					���ID
    *				PKValue
    *					PKֵ
    *     ����˵�� :
    *				����PKֵ����PK����
    */
	�ú���ע��Ϊ LuaFnGetPKPoint
    INT LuaFnGetPKPoint(Lua_State* L)	


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-05-22
    *     �������� : LuaFnGetPKPunish
    *     ����˵�� :
    *				L
    *					LuaStateִ�л���ָ��
    *				sceneId
    *					������
    *				selfId
    *					���ID
    *				PunishValue
    *					�ͷ�ֵ
    *     ����˵�� :
    *				���ݳͷ�ֵ���óͷ�ֵ����
    */
	�ú���ע��Ϊ LuaFnGetPKPunish
    INT LuaFnGetPKPunish(Lua_State* L)	


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaAddHonor
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nHonor 
    *					��������
    *				
    */
	�ú���ע��Ϊ AddHonor
    INT	LuaAddHonor(Lua_State* L) 


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaSubHonor
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nHonor 
    *					����
    *				
    */
	�ú���ע��Ϊ SubHonor
    INT	LuaSubHonor(Lua_State* L) 


�ű�ϵͳC���������ĵ�	LuaFnTbl_DoAction.h
	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-17	11:29
	 *	�������ƣ�	LuaFnDoAction
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *				nActionID
	 *					�������
	 *				TargetID
	 *					Ŀ���ţ�û��Ŀ���� -1 �Ϳ����ˣ�
	 *	����˵����	��Ŀ��������
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnDoAction
	INT	LuaFnDoAction(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnUnitDoEmoteAction
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSkillID
	*					���ܱ��
	*				nTargetID
	*					Ŀ����
	*     ����˵�� :
	*/
	�ú���ע��Ϊ LuaFnUnitDoEmoteAction
	INT	LuaFnUnitDoEmoteAction(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnUnitUseSkill
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSkillID
	*					���ܱ��
	*				nTargetID
	*					Ŀ����
	*				fPos_X
	*					Ŀ��λ�õ�X����
	*				fPos_Z
	*					Ŀ��λ�õ�Z����
	*				fDir
	*					Ŀ�귽��
	*				bPassSkillCheck
	*					�Ƿ���Լ��ܵ�����������Ʒ����
	*     ����˵�� :
	*/
	�ú���ע��Ϊ LuaFnUnitUseSkill
	INT	LuaFnUnitUseSkill(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnSendSpecificImpactToUnit
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSenderID
	*					Ч��������ID,�޷�������
	*				nTargetID
	*					Ŀ���ɫID,�޷�������
	*				nImpactDataIndex
	*					Ч����������,�޷�������
	*				nDelayTime
	*					�ӳ�ʱ��
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnSendSpecificImpactToUnit
	INT	LuaFnSendSpecificImpactToUnit(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnCancelSpecificImpact
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nImpactDataIndex
	*					Ч����������, �޷������ͣ��ڱ�׼Ч�������ѯ
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnCancelSpecificImpact
	INT	LuaFnCancelSpecificImpact(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnVerifyUsedItem
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnVerifyUsedItem
	INT	LuaFnVerifyUsedItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnDepletingUsedItem
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnDepletingUsedItem
	INT	LuaFnDepletingUsedItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetObjID
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnGetTargetObjID
	INT	LuaFnGetTargetObjID(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetPosition_X
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnGetTargetPosition_X
	INT	LuaFnGetTargetPosition_X(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetPosition_Z
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnGetTargetPosition_Z
	INT	LuaFnGetTargetPosition_Z(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetBagSlot
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnGetTargetBagSlot
	INT	LuaFnGetTargetBagSlot(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-11	19:29
	 *	�������ƣ�	LuaFnGetBagIndexOfUsedItem
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	?
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetBagIndexOfUsedItem
	INT LuaFnGetBagIndexOfUsedItem( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-8-16	12:23
	 *	�������ƣ�	LuaFnGetItemIndexOfUsedItem
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	ȡ��ʹ�õ���Ʒ����Ʒ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetItemIndexOfUsedItem
	INT LuaFnGetItemIndexOfUsedItem( Lua_State* L )


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetLowSectionOfTargetPetGuid
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnGetLowSectionOfTargetPetGuid
	INT	LuaFnGetLowSectionOfTargetPetGuid(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetHighSectionOfTargetPetGuid
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnGetHighSectionOfTargetPetGuid
	INT	LuaFnGetHighSectionOfTargetPetGuid(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnIsScriptDynamicParamIndexLegal
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnIsScriptDynamicParamIndexLegal
	INT	LuaFnIsScriptDynamicParamIndexLegal(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetScriptDynamicParamByIndex
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnGetScriptDynamicParamByIndex
	INT	LuaFnGetScriptDynamicParamByIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnSetScriptDynamicParamByIndex
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*				nValue
	*					����ֵ
	*     ����˵�� :
	*				
	*/
	�ú���ע��Ϊ LuaFnSetScriptDynamicParamByIndex
	INT	LuaFnSetScriptDynamicParamByIndex(Lua_State* L)


�ű�ϵͳC���������ĵ�	LuaFnTbl_Guild.h
	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-3-21
	*     �������� : LuaFnGuildList
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				
	*/
	�ú���ע��Ϊ GuildList
	INT	LuaFnGuildList(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-3-21
	*     �������� : LuaFnGuildCreate
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				
	*/
	�ú���ע��Ϊ GuildCreate
	INT	LuaFnGuildCreate(Lua_State* L) 


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaAddGuildUserPoint
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nGp
    *					���ӵİ�ṱ�׶�
    *				
    */
	�ú���ע��Ϊ AddGuildUserPoint
    INT	LuaAddGuildUserPoint(Lua_State* L) 


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaSubGuildUserPoint
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nGp
    *					��ṱ�׶�
    *				
    */
	�ú���ע��Ϊ SubGuildUserPoint
    INT	LuaSubGuildUserPoint(Lua_State* L) 


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaAddTotalGP
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nGp
    *					���ӵİ�ṱ�׶�
    *				
    */
	�ú���ע��Ϊ AddTotalGP
    INT	LuaAddTotalGP(Lua_State* L) 


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaSubTotalGP
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nGp
    *					���ӵİ�ṱ�׶�
    *				
    */
	�ú���ע��Ϊ SubTotalGP
    INT	LuaSubTotalGP(Lua_State* L) 


�ű�ϵͳC���������ĵ�	LuaFnTbl_Misc.h
/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetTimer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 selfId
	*					������ID,�޷�������
	*				 scriptId
	*					ִ�нű�ID,short����
	*				 funcName
	*					��ʱ���ص���������,�ַ�������
	*				 tickTime
	*					��ʱ�����ü��ʱ��
	*     ����˵�� :
	*				 ����Ҵ򿪼�ʱ��,ʱ�䵽ʱ���Զ�����OnTimer����	
	*				 ��ұ������ϲ�û�м�ʱ�����ű�������Ҫ��ʱ��ʱ��ϵͳ���롰���á�����ϵͳ����
	*                ���أ�1,��ʾ����
	*/
	�ú���ע��Ϊ SetTimer
	INT LuaFnSetTimer( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnStopTimer
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				sceneTimerIndex
	*					��ʱ���ڳ�����ʱ�����е������±�,��SetTimerʱ����		
	*     ����˵�� :
    *				�رյ�ǰ��ҵļ�ʱ��	//ÿ��������������԰�������ʱ��
	*/
	�ú���ע��Ϊ StopTimer
	INT LuaFnStopTimer(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnCheckTimer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 TimeIndex
	*					��ʱ������
	*     ����˵�� : �ж�TimeIndex�ż�ʱ���Ƿ��Ѿ���ʹ����
	*                ����1,��ʾ��ʹ�ã�����0����ʾû��ʹ��; ����-1����ʾ�쳣
	*/
	�ú���ע��Ϊ CheckTimer
	INT LuaFnCheckTimer( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetGameTime
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
    *				��ȡ��ǰ�������ʱ�䣬��λΪ��
	*				�õ�ǰʱ���ȥ����ʱ��
	*				û�в���
	*/
	�ú���ע��Ϊ GetGameTime
	INT LuaFnGetGameTime(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-1-10
	*     �������� : LuaFnGetCurrentTime
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*     ����˵�� :
	*				��ȡ��ǰʱ�䣬��λΪ��
	*/
	�ú���ע��Ϊ LuaFnGetCurrentTime
	INT LuaFnGetCurrentTime(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-4-21
	*     �������� : LuaFnIsInStall
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����Ƿ��ڰ�̯״̬
	*/
	�ú���ע��Ϊ LuaFnIsInStall
	INT LuaFnIsInStall(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddGlobalCountNews
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				newstr
	*					��������,�ַ���ָ������
	*				count
	*					���ʹ���
	*     ����˵�� :
    *				��ȫ��������������Ź��棬���淢��ָ������
	*				û�в���
	*/
	�ú���ע��Ϊ AddGlobalCountNews
	�ú���ע��Ϊ LuaFnAddGlobalCountNews
	INT LuaFnAddGlobalCountNews(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnPrintNum
	*     ����˵�� : 
	*				num
	*				��Ϣ�����ʾ����
	*     ����˵�� :
	*				������Ϣ��,���Խű���
	*/
	�ú���ע��Ϊ PrintNum
	INT LuaFnPrintNum(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnPrintStr
	*     ����˵�� : 
	*				strText
	*				��Ϣ�����ʾ�ַ���
	*     ����˵�� :
	*				������Ϣ��,���Խű���
	*/
	�ú���ע��Ϊ PrintStr
	INT LuaFnPrintStr(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnCallScriptFunction
	*     ����˵�� : 
	*     ����˵�� :
	*					���ÿɱ����������ű�����
	*/
	�ú���ע��Ϊ CallScriptFunction
	INT LuaFnCallScriptFunction(Lua_State* L,...) 


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnShowNpcEventList
	*     ����˵�� : 
	*     ����˵�� :
	*				���ÿɱ����������ű�����
	*/
	�ú���ע��Ϊ ShowNpcEventList
	INT LuaFnShowNpcEventList(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-19
	*     �������� : LuaFnIsInDist
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					�Է���ID���,ObjID
	*				dist
	*					self��target����
	*     ����˵�� :
	*				�ж�selfId��targetId֮��ľ����Ƿ���dist��(����)
	*				Ŀǰ���ڻ�������,�ж���Һͱ�������֮��ľ���
	*/
	�ú���ע��Ϊ IsInDist
	INT LuaFnIsInDist(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-3-17
	*     �������� : LuaFnAddEnemy
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				EnemyId
	*					����ID,�޷�������
	*     ����˵�� :
	*				��ӳ���
	*/	
	�ú���ע��Ϊ LuaFnAddEnemy
	INT LuaFnAddEnemy(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-3-17
	*     �������� : LuaFnGetObjType
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					��������ID,�޷�������
	*     ����˵�� :
	*				�õ�������ָ��OBJ������
	*/	
	�ú���ע��Ϊ LuaFnGetObjType
	INT LuaFnGetObjType(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-15
	*     �������� : LuaFnGetDist
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					�Է���ID���,ObjID
	*     ����˵�� :
	*				�õ�selfId��targetId֮��ľ���
	*/
	�ú���ע��Ϊ GetDist
	INT LuaFnGetDist(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnGuid2ObjId
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				guid
	*					GUID,GUID_t���ͣ�����������ҵ�GUIDת��
	*     ����˵�� :
	*				��Guidת��ΪobjId
	*/
	�ú���ע��Ϊ LuaFnGuid2ObjId
	INT LuaFnGuid2ObjId(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnObjId2Guid
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� : ����selfId��GUID
	*/
	�ú���ע��Ϊ LuaFnObjId2Guid
	INT LuaFnObjId2Guid(Lua_State* L)


	///** -----------------------------------------------------------------------
	//*     ������Ա : 
	//*     ��дʱ�� : 2005-10-21
	//*     �������� : LuaFnRemoveMonster
	//*     ����˵�� : 
	//*     ����˵�� :
	//*				SceneID
	//*					�������
	//*				selfId
	//*					������ID,�޷�������
	//*     ����˵�� :
	//*				ɾ��Obj_Monster,��Obj_Monster�ӵ�ͼɾ��
	//*/
	�ú���ע��Ϊ RemoveMonster
	//INT LuaFnRemoveMonster(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-2
	*     �������� : LuaFnAwardTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*				TitleId
	*					�ƺ�ID
	*     ����˵�� :
	*				ΪselfId����һ���ƺ�
	*/
	�ú���ע��Ϊ AwardTitle
	�ú���ע��Ϊ LuaFnAwardTitle
	INT	LuaFnAwardTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-2
	*     �������� : LuaFnDeleteTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*     ����˵�� :
	*				ɾ��ĳ���ƺ�
	*/
	�ú���ע��Ϊ DeleteTitle
	INT	LuaFnDeleteTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnDispatchAllTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				���´��˵����гƺ�
	*/
	�ú���ע��Ϊ DispatchAllTitle
	�ú���ע��Ϊ LuaFnDispatchAllTitle
	INT	LuaFnDispatchAllTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnGetTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*     ����˵�� :
	*				��ô��˵�ĳ���ƺ�
	*/
	�ú���ע��Ϊ GetTitle
	INT	LuaFnGetTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnGetDyTitlesNum
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ô��˵Ķ�̬�ƺŸ���
	*/
	�ú���ע��Ϊ GetDyTitlesNum
	INT	LuaFnGetDyTitlesNum( Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnAddDyTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleId
	*					�ƺ�ID
	*     ����˵�� :
	*				�����µĶ�̬�ƺţ�������˷��ڵ�һ��
	*/
	�ú���ע��Ϊ AddDyTitle
	INT	LuaFnAddDyTitle( Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardNickTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					ǩ�������ʵ��ǳ�
	*     ����˵�� :
	*				���ô��˵�ǩ�������ʵ��ǳ�
	*/
	�ú���ע��Ϊ AwardNickTitle
	INT LuaFnAwardNickTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardSpouseTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					��ż��
	*     ����˵�� :
	*				������ż��
	*/
	�ú���ע��Ϊ AwardSpouseTitle
	�ú���ע��Ϊ LuaFnAwardSpouseTitle
	INT LuaFnAwardSpouseTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardJieBaiTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					��ݳƺ�
	*     ����˵�� :
	*				���ý�ݳƺ�
	*/
	�ú���ע��Ϊ AwardJieBaiTitle
	INT LuaFnAwardJieBaiTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardBangPaiTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
	*/
	�ú���ע��Ϊ AwardBangpaiTitle
	INT LuaFnAwardBangPaiTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardShiTuTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
	*/
	�ú���ע��Ϊ AwardShiTuTitle
	INT LuaFnAwardShiTuTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardShangDianTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
	*/
	�ú���ע��Ϊ AwardShangDianTitle
	INT LuaFnAwardShangDianTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-8
	*     �������� : LuaFnEnableBankRentIndex
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				RentIndex
	*					�򿪵�������
	*     ����˵�� :
	*				�򿪴��˵�������
	*/
	�ú���ע��Ϊ EnableBankRentIndex
	INT LuaFnEnableBankRentIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2006-1-12
	*     �������� : LuaFnGetBankRentIndex
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��õ�ǰ�������Ѵ򿪵�����
	*/
	�ú���ע��Ϊ GetBankRentIndex
	INT LuaFnGetBankRentIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-8
	*     �������� : LuaFnBankBegin
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				֪ͨ�ͻ��˿�ʼ�������н���
	*/
	�ú���ע��Ϊ BankBegin
	INT LuaFnBankBegin(Lua_State* L)


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-14
    *     �������� : LuaFnExpandBankSize
    *     ����˵�� : 
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *               nSize
    *                   ��չ�ĸ�����
    *     ����˵�� :
    *				��չ���и�����
    */
	�ú���ע��Ϊ ExpandBankSize
    INT LuaFnExpandBankSize(Lua_State* L)


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-14
    *     �������� : LuaFnGetBankSize
    *     ����˵�� : 
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *     ����˵�� :
    *				���и�����
    */
	�ú���ע��Ϊ GetBankSize
    INT LuaFnGetBankSize(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2005-11-14
	*     �������� : LuaSetPlayerDefaultReliveInfo
	*     ����˵�� : 
	*				sceneId
	*					�������
	*				playerId
	*					���볡�������ID
	*				pszHP
	*					HP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				pszMP
	*					MP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				pszSP
	*					SP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				defaultSceneID
	*					����ĳ���ID
	*				defaultPosX
	*					����X
	*				defaultPosZ
	*					����Z
	*     ����˵�� :
	*				��ҵ�ȱʡ������ص���������
	*/
	�ú���ע��Ϊ SetPlayerDefaultReliveInfo
	INT LuaSetPlayerDefaultReliveInfo(Lua_State* L )


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-20
	*     �������� : LuaGetDayTime
	*     ����˵�� : 
	*					��
	*     ����˵�� :
	*				ȡ�õ�ǰʱ������������磺1140 ��ʾ2001��ĵ�140��
	*										  6099 ��ʾ2006��ĵ�99��
	*/
	�ú���ע��Ϊ GetDayTime
	INT LuaGetDayTime(Lua_State* L )


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaGetObjCreateTime
	*     ����˵�� : 
	*				sceneId
    *					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ȡ��ObjID Ϊ selfId��obj�Ĵ���ʱ��
	*					
	*/
	�ú���ע��Ϊ GetObjCreateTime
	INT LuaGetObjCreateTime(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2006-3-28
	*     �������� : LuaFnUpdateAttr
	*     ����˵�� : 
	*				sceneId
	*					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ͨ���ʼ�ϵͳ������������
	*					
	*/
	�ú���ע��Ϊ LuaFnUpdateAttr
	INT LuaFnUpdateAttr(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-26	17:08
	 *	�������ƣ�	LuaFnIsPasswordSetup
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				flag
	 *					��ǣ��������δ���ö���������Ч����� 1�����õ��������ö������룬0 ����Ҫ����
	 *
	 *	����˵����	�ж�����Ƿ����ö������룬0 Ϊδ���ã�1 Ϊ������
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnIsPasswordSetup
	INT LuaFnIsPasswordSetup(Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-26	17:21
	 *	�������ƣ�	LuaFnIsPasswordUnlocked
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				flag
	 *					��ǣ��������δ��������������Ч����� 1�����õ����߽����������룬0 ��ǿ�ƽ���
	 *
	 *	����˵����	�ж�����Ƿ�����������룬0 Ϊδ������1 Ϊ�ѽ���
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnIsPasswordUnlocked
	INT LuaFnIsPasswordUnlocked(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnStartupSystemTimer
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*     ����˵�� :
    *				����ϵͳ��ʱ���¼�
	*/
	�ú���ע��Ϊ StartupSystemTimer
	INT LuaFnStartupSystemTimer(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnStopSystemTimer
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*     ����˵�� :
    *				ֹͣϵͳ��ʱ���¼�
	*/
	�ú���ע��Ϊ StopSystemTimer
	INT LuaFnStopSystemTimer(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnSetSystemTimerTick
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*				tick
	*					����ʱ��
	*     ����˵�� :
    *				����ϵͳ��ʱ������ʱ��
	*/
	�ú���ע��Ϊ SetSystemTimerTick
	INT LuaFnSetSystemTimerTick(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnSetSystemTimerParam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*				paramIndex
	*					��ʱ��������index
	*				val
	*					��Ӧλ�õ�ֵ
	*     ����˵�� :
    *				����ϵͳ��ʱ������
	*/
	�ú���ע��Ϊ SetSystemTimerParam
	INT LuaFnSetSystemTimerParam(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnGetSystemTimerParam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*				paramIndex
	*					��ʱ��������index
	*     ����˵�� :
    *				�õ�ϵͳ��ʱ������
	*/
	�ú���ע��Ϊ GetSystemTimerParam
	INT LuaFnGetSystemTimerParam(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnCheckSystemTimerValidity
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*     ����˵�� :
    *				���ϵͳ��ʱ���Ƿ����
	*/
	�ú���ע��Ϊ CheckSystemTimerValidity
	INT LuaFnCheckSystemTimerValidity(Lua_State* L)	


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-27
    *     �������� : LuaAddActionPoint
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nAP 
    *					�ж���
    *				
    */
	�ú���ע��Ϊ AddCountryActionPoint
    INT	LuaAddActionPoint(Lua_State* L) 


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-27
    *     �������� : LuaSubActionPoint
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nAP 
    *					�ж���
    *				
    */
	�ú���ע��Ϊ SubCountryActionPoint
    INT	LuaSubActionPoint(Lua_State* L) 


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-28
    *     �������� : LuaCountryDeletaBegin
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				
    */
	�ú���ע��Ϊ CountryDeletaBegin
    INT	LuaCountryDeletaBegin(Lua_State* L) 


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-28
    *     �������� : LuaCountryVoteBegin
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				
    */
	�ú���ע��Ϊ CountryVoteBegin
    INT	LuaCountryVoteBegin(Lua_State* L) 


�ű�ϵͳC���������ĵ�	LuaFnTbl_Mission.h
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnBeginEvent
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				��EndEvent���ʹ��
	*				Ϊ��һ������ز�����׼��
	*/
	�ú���ע��Ϊ BeginEvent
	INT LuaFnBeginEvent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				msg
	*					��ʾ����
	*     ����˵�� :
	*				����ַ�����ʾ
	*/
	�ú���ע��Ϊ AddText
	INT LuaFnAddText(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddNumber
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				id
	*					��Ŀ,INT
	*     ����˵�� :
	*				�����Ŀ��ʾ
	*/
	�ú���ע��Ϊ AddNumber
	INT LuaFnAddNumber(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				id
	*					�ڼ�����ʾ,INT
	*				msg
	*					��ʾ����
	*				state 
	*					�Ƿ���Խ�����ͽ�����
	*				index
	*					�߻���������
	*     ����˵�� :
	*				�����Ŀ���ַ�����ʾ
	*				state��index���Բ���
	*/
	�ú���ע��Ϊ AddEventList
	�ú���ע��Ϊ AddNumText
	INT LuaFnAddNumText(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-12-9
	*     �������� : LuaFnSetNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				number
	*					�����õ�ֵ,short
	*     ����˵�� :
	*				���ýű�����������
	*/
	�ú���ע��Ϊ LuaFnSetNumText
	�ú���ע��Ϊ LuaFnSetNumText
	INT LuaFnSetNumText(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-12-9
	*     �������� : LuaFnGetNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				number
	*					�����õ�ֵ,short
	*     ����˵�� :
	*				ȡ���ű�����������
	*/
	�ú���ע��Ϊ GetNumText
	�ú���ע��Ϊ GetEventList
	INT LuaFnGetNumText(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :�����
	*     ��дʱ�� : 2008-1-31
	*     �������� : LuaFnGetOneMissionInfo
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				nPhase
	*					���������׶�
	*				MissionType
	*					�������� 0��ɱ��  1��Ѱ��  2������	
	*				MisIndex
	*					�������� Ϊ-1ʱ��Ҫ�����������Ϊ-1ʱMisIndexΪ��һ�������
	*     ����˵�� :
	*				�õ�ѭ��������Ŀ��NPC��ITEM����Ϣ��ID��NAME��COUNT�����ڳ�������ID��λ�á�����
	*				�õ�ѭ��������������Ҫ�������ű�ʹ��
	*/
	�ú���ע��Ϊ LuaFnGetOneMissionInfo
	INT LuaFnGetOneMissionInfo(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddMoneyBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				count
	*					������Ǯ��Ŀ
	*     ����˵�� :
	*				���Money
	*/
	�ú���ע��Ϊ AddMoneyBonus
	INT LuaFnAddMoneyBonus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					������Ʒ��ItemType��9λ���
	*				count
	*					������Ŀ
	*     ����˵�� :
	*				���count�����ΪitemType����
	*/
	�ú���ע��Ϊ AddItemBonus
	INT LuaFnAddItemBonus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemDemand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					������Ʒ��ItemType��9λ���
	*				count
	*					������Ŀ

	*     ����˵�� :
	*				����count�����ΪitemType����
	*/
	�ú���ע��Ϊ AddItemDemand
	INT LuaFnAddItemDemand(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddRandItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*					��ʾ�����������
	*/
	�ú���ע��Ϊ AddRandItemBonus
	INT LuaFnAddRandItemBonus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddRadioItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					��Ʒ��ItemType9λ���
	*				count
	*					������ѡһ��Ʒ����Ŀ
	*     ����˵�� :
	*				��Ӷ�ѡһ����
	*/
	�ú���ע��Ϊ AddRadioItemBonus
	INT LuaFnAddRadioItemBonus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEndEvent
	*     ����˵�� : 
	*     ����˵�� :
	*				��BeginEvent���ʹ��
	*				֮�������ĳ��Dispatch***����
	*/
	�ú���ע��Ϊ EndEvent
	INT LuaFnEndEvent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchEventList
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*     ����˵�� :
	*				��ͻ��˷��ظ�Npc�ɴ����¼��б�
	*				�ɴ����¼��ɸ�npc�Ĺҵ��¼�OnEnumerate�����ж�
	*				������������������,ͨ��AddNumText���
	*				BeginEvent
	*				AddText
	*				����: AddNumText
	*				EndEvent
	*/
	�ú���ע��Ϊ DispatchEventList
	INT LuaFnDispatchEventList(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchRegieEventList
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*     ����˵�� :
	*				��ͻ��˷��ظ�Npc�ɴ����¼��б�
	*				�ɴ����¼��ɸ�npc�Ĺҵ��¼�OnEnumerate�����ж�
	*				������������������,ͨ��AddNumText���
	*				BeginEvent
	*				AddText
	*				����: AddNumText
	*				EndEvent
	*/
	�ú���ע��Ϊ DispatchRegieEventList
	INT LuaFnDispatchRegieEventList(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionInfo
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*     ����˵�� :
	*				��ͻ��˷���������Ϣ����
	*				
	*/
	�ú���ע��Ϊ DispatchMissionInfo
	INT LuaFnDispatchMissionInfo(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionDemandInfo
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*				done
	*					�Ƿ����
	*     ����˵�� :
	*				��ͻ��˷�������������
	*				done���ΪTRUE,�ͻ��˽���������������ɰ�ť
	*				֮ǰ��ʹ�õĺ�������Ϊ
	*				BeginEvent
	*				AddText,�������ַ���
	*				AddItemDemand,������Ʒ
	*				EndEvent
	*/
	�ú���ע��Ϊ DispatchMissionDemandInfo
	INT LuaFnDispatchMissionDemandInfo(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionContinueInfo
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*     ����˵�� :
	*				��ͻ��˷��������������
	*/
	�ú���ע��Ϊ DispatchMissionContinueInfo
	INT LuaFnDispatchMissionContinueInfo(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionTips
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ͻ��˷������������ʾ��Ϣ
	*				
	*/
	�ú���ע��Ϊ DispatchMissionTips
	INT LuaFnDispatchMissionTips(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-28
	*     �������� : LuaFnDispatchMissionResult
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ͻ��˷���������֤���
	*				
	*/
	�ú���ע��Ϊ DispatchMissionResult
	INT LuaFnDispatchMissionResult(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnMissionCom
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				�������
	*				֮����DelMission
	*/
	�ú���ע��Ϊ MissionCom
	INT LuaFnMissionCom(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				��õ�ǰ���ĳһ����ŵ�ֵ	//����,������,δ��
	*/
	�ú���ע��Ϊ GetMission
	INT LuaFnGetMission(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMissionIndexByID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				���õ�ǰ���ĳһ����ŵ�ֵ
	*/
	�ú���ע��Ϊ GetMissionIndexByID
	INT LuaFnGetMissionIndexByID(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMissionParam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misIndex
	*					����λ������
	*				paramIndex
	*					�������ֶα��
	*     ����˵�� :
	*				 ȡ�������
	*/
	�ú���ע��Ϊ GetMissionParam
	INT LuaFnGetMissionParam(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetMissionByIndex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				paramIndex
	*					�������ֶα��
	*				value
	*					��paramIndex�ֶε�ֵ
	*     ����˵�� :
	*				 �����������
	*/
	�ú���ע��Ϊ SetMissionByIndex
	INT LuaFnSetMissionByIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-18
	*     �������� : LuaFnGetMissionCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				 �õ��ѽ����������
	*				 û������ʱ����0
	*/
	�ú���ע��Ϊ GetMissionCount
	INT LuaFnGetMissionCount(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				scriptId
	*					���ýű����
	*				killObjEvent
	*					�Ƿ�ص�OnKillObject
	*				enterAreaEvent
	*					�Ƿ�ص�OnEnterArea
	*				itemChangeEvent
	*					�Ƿ�ص�OnItemChange
	*     ����˵�� :
	*				������������misId��ŵ�����
	*				�ɹ�����1
	*/
	�ú���ע��Ϊ AddMission
	INT LuaFnAddMission( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-13
	*     �������� : LuaFnAddMissionEx
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				scriptId
	*					���ýű����
	*     ����˵�� :
	*				������������misId��ŵ�����
	*				�ɹ�����1
	*/
	�ú���ע��Ϊ AddMissionEx
	INT LuaFnAddMissionEx( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-13
	*     �������� : LuaFnSetMissionEvent
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				eventId
	*					���ýű���ţ�0 KillObj, 1 EnterArea, 2 ItemChanged, 3 PetChanged
	*     ����˵�� :
	*				�޸��������misId��ŵ�������Ӧ�¼�
	*				�ɹ�����1
	*/
	�ú���ע��Ϊ SetMissionEvent
	INT LuaFnSetMissionEvent( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				ɾ������
	*/
	�ú���ע��Ϊ DelMission
	INT LuaFnDelMission(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnIsMissionHaveDone
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				�Ƿ�����������
	*				����ֵ1Ϊ�ɹ�
	*				����ֵ0Ϊʧ��
	*/
	�ú���ע��Ϊ IsMissionHaveDone
	INT LuaFnIsMissionHaveDone(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : IsHaveMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				����Ƿ�������������
	*				����ֵ����0Ϊ�ɹ�
	*				
	*/
	�ú���ע��Ϊ IsHaveMission
	INT LuaFnIsHaveMission(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnBeginAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				��ʼ��ӵ��߲���
	*				��EndAddItem���ʹ��
	*/
	�ú���ע��Ϊ BeginAddItem
	�ú���ע��Ϊ LuaFnBeginAddItem
	INT LuaFnBeginAddItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)	
	*				count
	*					�����Ŀ
	*     ����˵�� :
	*				�������Ʒ��������count��itemType��ŵĵ���
	*/
	�ú���ע��Ϊ AddItem
	�ú���ע��Ϊ LuaFnAddItem
	INT LuaFnAddItem( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEndAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				������ӵ��߲���
	*				��BeginAddItem���ʹ��
	*				����1��ʾ�пռ���������Ʒ
	*/
	�ú���ע��Ϊ EndAddItem
	�ú���ע��Ϊ LuaFnEndAddItem
	INT LuaFnEndAddItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemListToHuman
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����Ʒ�б��е�������Ʒ���õ���Ұ���
	*				�ڵ���BeginAddItem��EndAddItem֮��ʹ��
	*
	*	  ��ע:
	*				�˺�����ص���ӦOnItemChanged�¼��Ľű�
	*/
	�ú���ע��Ϊ AddItemListToHuman
	�ú���ע��Ϊ LuaFnAddItemListToHuman
	INT LuaFnAddItemListToHuman(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMissionData
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					���ݿ�����
	*     ����˵�� :
	*				�õ�Humanȫ�����ݿ��е�nIndex������ֵ
	*				GetMissionData(L,18,12,3)
	*/
	�ú���ע��Ϊ GetMissionData
	INT LuaFnGetMissionData(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnSetMissionData
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					���ݿ�����
	*				nData
	*					����ֵ
	*     ����˵�� :
	*				����Human��nIndex�����ݿ��ֵΪnData
	*				�ڵ���BeginAddItem��EndAddItem֮��ʹ��
	*/
	�ú���ע��Ϊ SetMissionData
	INT LuaFnSetMissionData(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnAddMonsterDropItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				HumanID
	*					��ɫ���
	*				ItemIndex
	*					�������Ʒ����
	*				
	*     ����˵�� :
	*			�ڹ����������һ������������Ʒ,������������
	*/
	�ú���ע��Ϊ AddMonsterDropItem
	INT LuaFnAddMonsterDropItem(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMonsterOwnerCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				
	*     ����˵�� :
	*			��ȡ��������������
	*/
	�ú���ע��Ϊ GetMonsterOwnerCount
	INT LuaFnGetMonsterOwnerCount(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMonsterOwnerID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				OwnerOffSet
	*					������ƫ����
	*     ����˵�� :
	*			��ȡ���������߱��
	*			����������id
	*/
	�ú���ע��Ϊ GetMonsterOwnerID
	INT LuaFnGetMonsterOwnerID(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnBeginUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*			��ʼ����UICommand������
	*			�޷���
	*/
	�ú���ע��Ϊ BeginUICommand
	INT LuaFnBeginUICommand(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnEndUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	�ú���ע��Ϊ EndUICommand
	INT LuaFnEndUICommand(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnDispatchUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nUIIndex
	*					��������ֵ
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	�ú���ע��Ϊ DispatchUICommand
	INT LuaFnDispatchUICommand(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnUICommand_AddInt
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				nValue
	*					����������ӵ�����,����
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	�ú���ע��Ϊ UICommand_AddInt
	INT LuaFnUICommand_AddInt(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnUICommand_AddString
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				szValue
	*					����������ӵ�����,�ַ���
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	�ú���ע��Ϊ UICommand_AddString
	INT LuaFnUICommand_AddString(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-10
	*     �������� : LuaFnGetQuestionsRecord
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				nIndex
	*					����ţ�����
	*     ����˵�� :
	*			��ȡ�����Ϣ
	*			����ֵ����������(�ַ���),
	*					ѡ��0(�ַ���),
	*					ѡ��1(�ַ���),
	*					ѡ��2(�ַ���),
	*					ѡ��3(�ַ���),
	*					ѡ��4(�ַ���),
	*					ѡ��5(�ַ���),
	*					��0(����),
	*					��1(����),
	*					��2(����),
	*					��3(����),
	*					��4(����),
	*					��5(����),
	*/
	�ú���ע��Ϊ GetQuestionsRecord
	INT LuaFnGetQuestionsRecord(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-4-19
	*     �������� : LuaFnSetMissionDataBit
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					MissionData�ı������
	*				nBits
	*					ƫ�Ƶ�λ����0��31��
	*				nValue
	*					����, 0���0
	*     ����˵�� :
	*			�޷���
	*/
	�ú���ע��Ϊ SetMissionDataBit
	INT LuaFnSetMissionDataBit(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-4-19
	*     �������� : LuaFnGetMissionDataBit
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					MissionData�ı������
	*				nBits
	*					ƫ�Ƶ�λ����0��31��
	*     ����˵�� :
	*			0 ���� 1
	*/
	�ú���ע��Ϊ GetMissionDataBit
	INT LuaFnGetMissionDataBit(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-22
	*     �������� : LuaFnIsMissionInTime
	*					
	*     ����˵�� :
	*			����ÿ�ջ�����Ƿ��ڻ����ʱ����
	*			��ÿ�ջʱ�ô˺����ж�
	*	
	*/
	�ú���ע��Ϊ IsMissionInTime
	INT LuaFnIsMissionInTime(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-22
	*     �������� : LuaFnMissionCommonCheck
	*					
	*     ����˵�� :
	*			��������Ƿ������ÿ�ջ�Ļ�������
	*			��ÿ�ջʱ�ô˺����ж�
	*
	*/
	�ú���ע��Ϊ MissionCommonCheck
	INT LuaFnMissionCommonCheck(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-23
	*     �������� : LuaFnIsTodayMission
	*					
	*     ����˵�� :
	*			����ÿ�ջ�Ƿ��ǽ��տɼ�����
	*/
	�ú���ע��Ϊ IsTodayMission
	INT LuaFnIsTodayMission(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-23
	*     �������� : LuaFnSetTimeParam								
	*					
	*     ����˵�� :
	*			�������һ����ɻ��ʱ��
	*			�±��ڽű�ScriptGlobal.lua������
	*
	*/
	�ú���ע��Ϊ SetTimeParam
	INT LuaFnSetTimeParam(Lua_State* L)


	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-30
	*     �������� : LuaFnGetTodayDate								
	*					
	*     ����˵�� :
	*			��ȡ��ǰ���ڣ���������ת����ĸ�ʽ
	*			
	*/
	�ú���ע��Ϊ GetTodayDate
	INT LuaFnGetTodayDate(Lua_State* L)


�ű�ϵͳC���������ĵ�	LuaFnTbl_Pet.h
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpj
	*     ��дʱ�� : 2006-021-20
	*     �������� : LuaFnPetStudySkill
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				skillId
	*					����ID
	*     ����˵�� :
	*				����ѧϰ����
	*/
	�ú���ע��Ϊ PetStudySkill
	INT LuaFnPetStudySkill(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpj
	*     ��дʱ�� : 2006-021-20
	*     �������� : LuaFnPetStudySkill
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				skillId
	*					����ID
	*     ����˵�� :
	*				����ѧϰ���ɼ���
	*/
	�ú���ע��Ϊ PetStudySkill_MenPai
	INT LuaFnPetStudySkill_MenPai(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpj
	*     ��дʱ�� : 2006-021-20
	*     �������� : LuaFnCalcPetDomesticationMoney
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					����ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				�õ�ѵ�������������
	*/
	�ú���ע��Ϊ CalcPetDomesticationMoney
	INT LuaFnCalcPetDomesticationMoney(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-021-20
	*     �������� : LuaFnPetDomestication
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					����ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				ѵ������
	*/
	�ú���ע��Ϊ PetDomestication
	INT LuaFnPetDomestication(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnGetPetHP
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ó���HP
	*/
	�ú���ע��Ϊ LuaFnGetPetHP
	INT LuaFnGetPetHP(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnGetPetMaxHP
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ó���HP���ֵ
	*/
	�ú���ע��Ϊ LuaFnGetPetMaxHP
	INT LuaFnGetPetMaxHP(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnGetPetLife
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ó�������
	*/
	�ú���ע��Ϊ LuaFnGetPetLife
	INT LuaFnGetPetLife(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnGetPetHappiness
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ó�����ֶ�
	*/
	�ú���ע��Ϊ LuaFnGetPetHappiness
	INT LuaFnGetPetHappiness(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnSetPetHP
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				hp
	*					Ҫ���õ�HPֵ	
	*     ����˵�� :
	*				���ó���HP
	*/
	�ú���ע��Ϊ LuaFnSetPetHP
	INT LuaFnSetPetHP(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnSetPetMaxHP
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				maxHP
	*					Ҫ���õ�HP���ֵ	
	*     ����˵�� :
	*				���ó���HP���ֵ
	*/
	�ú���ע��Ϊ LuaFnSetPetMaxHP
	INT LuaFnSetPetMaxHP(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnSetPetLife
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				life
	*					Ҫ���õ�����ֵ	
	*     ����˵�� :
	*				���ó�������ֵ	
	*/
	�ú���ע��Ϊ LuaFnSetPetLife
	INT LuaFnSetPetLife(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnSetPetHappiness
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				happiness
	*					Ҫ���õĿ��ֶ�
	*     ����˵�� :
	*				���ó�����ֶ�
	*/
	�ú���ע��Ϊ LuaFnSetPetHappiness
	INT LuaFnSetPetHappiness(Lua_State* L)	


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-11	16:31
	 *	�������ƣ�	LuaFnPetCanUseFood
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				petGUID_H
	 *					����GUID��λ
	 *				petGUID_L
	 *					����GUID��λ
	 *				nIndex
	 *					��Ʒ����λ��
	 *
	 *	����˵����	�ж���Ʒ�Ƿ��ʺϳ���ʳ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnPetCanUseFood
	INT LuaFnPetCanUseFood(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : wuwenbin
	*     ��дʱ�� : 2006-04-19
	*     �������� : LuaFnPetReturnToChild
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ﻹͯLua�ӿ�
	*/
	�ú���ע��Ϊ LuaFnPetReturnToChild
	INT LuaFnPetReturnToChild(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wuwenbin
	*     ��дʱ�� : 2006-04-19
	*     �������� : LuaFnPetReturnToChild
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ﻹͯLua�ӿ�
	*/
	�ú���ע��Ϊ LuaFnPetCanReturnToChild
	INT LuaFnPetCanReturnToChild(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-05-12
	*     �������� : LuaFnGetPetGUID
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petIndex
	*					��������
	*     ����˵�� :
	*				ȡ�ó����GUID
	*/
	�ú���ע��Ϊ LuaFnGetPetGUID
	INT LuaFnGetPetGUID(Lua_State* L)	


�ű�ϵͳC���������ĵ�	LuaFnTbl_PetPlacard.h
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-07
	*     �������� : LuaFnIssuePetPlacardByIndex
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				pszMessage
	*					����������
	*				
	*     ����˵�� :
	*				�������﹫��
	*				
	*/
	�ú���ע��Ϊ IssuePetPlacardByIndex
	INT	LuaFnIssuePetPlacardByIndex(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-07
	*     �������� : LuaFnIssuePetPlacard
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				pszMessage
	*					����������
	*				
	*     ����˵�� :
	*				�������﹫��
	*				
	*/
	�ú���ע��Ϊ IssuePetPlacard
	INT	LuaFnIssuePetPlacard(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-07
	*     �������� : LuaFnIssuePetPlacard
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				
	*     ����˵�� :
	*				ȡ�����﹫��
	*				
	*/
	�ú���ע��Ϊ CancelPetPlacard
	INT	LuaFnCancelPetPlacard(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-07
	*     �������� : LuaFnDispatchPetPlacardList
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				firstCheckId
	*					�����ҵ����Ӧ�ô���һ����Ϣ��ʼ��������ˢ��
	*					˳��=���ϴ�ˢ�µĵ�����һ��ID��
	*					����=���ϴ�ˢ�µĵ�һ��ID��
	*				secondCheckId
	*					˳��=���ϴ�ˢ�µĵ����ڶ���ID)
	*					����=���ϴ�ˢ�µĵڶ���ID��
	*				nextPageOrPrevPage
	*					!=0Ϊ˳��
	*				
	*     ����˵�� :
	*				�������Ĺ����б�
	*				
	*/
	�ú���ע��Ϊ DispatchPetPlacardList
	INT	LuaFnDispatchPetPlacardList(Lua_State* L) 


�ű�ϵͳC���������ĵ�	LuaFnTbl_Relation.h
	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	16:13
	 *	�������ƣ�	LuaFnIsFriend
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendID
	 *					���ѵ� ID
	 *
	 *	����˵����	�ж� selfId �� friendID �Ƿ�Ϊ����
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnIsFriend
	INT LuaFnIsFriend( Lua_State* L )


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	13:48
	 *	�������ƣ�	LuaFnGetFriendName
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendGUID
	 *					���ѵ� GUID
	 *
	 *	����˵����	�õ� GUID Ϊ friendGUID �ĺ��ѵ�����
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetFriendName
	INT LuaFnGetFriendName( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	14:42
	 *	�������ƣ�	LuaFnGetFriendPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendID
	 *					���ѵ� ID
	 *
	 *	����˵����	�õ��� ID Ϊ friendID �ĺ��ѵ��Ѻö�
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetFriendPoint
	INT LuaFnGetFriendPoint( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	20:07
	 *	�������ƣ�	LuaFnSetFriendPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendID
	 *					���ѵ� ID
	 *				friendPoint
	 *					�Ѻö�ֵ
	 *
	 *	����˵����	���� selfId �� friendID ���Ѻö�Ϊ friendPoint
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSetFriendPoint
	INT LuaFnSetFriendPoint( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	�����
	 *	��дʱ�䣺	2008-3-28	20:07
	 *	�������ƣ�	LuaFnSetDeadLinkFriendPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendID
	 *					���ѵ� GUID
	 *				friendPoint
	 *					�Ѻö�ֵ
	 *
	 *	����˵����	���� selfId �� friendID ���Ѻö�Ϊ friendPoint,
	 *				��LuaFnSetFriendPoint���������ں��ѿ��Դ�������״̬
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSetDeadLinkFriendPoint
	INT LuaFnSetDeadLinkFriendPoint( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	12:14
	 *	�������ƣ�	LuaFnSetFriendPointByGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				friendGUID
	 *					���ѵ� GUID
	 *				friendPoint
	 *					�Ѻö�ֵ
	 *
	 *	����˵����	���� selfId �� friendGUID ���Ѻö�Ϊ friendPoint
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSetFriendPointByGUID
	INT LuaFnSetFriendPointByGUID( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	15:04
	 *	�������ƣ�	LuaFnIsMarried
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�жϵ������Ƿ��ѻ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnIsMarried
	INT LuaFnIsMarried( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	12:17
	 *	�������ƣ�	LuaFnGetSpouseGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ���ż�� GUID
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetSpouseGUID
	INT LuaFnGetSpouseGUID( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	16:11
	 *	�������ƣ�	LuaFnMarry
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				spouseId
	 *					��ż ID
	 *
	 *	����˵����	sceneId �� spouseId ��Ϊ����
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnMarry
	INT LuaFnMarry( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	19:24
	 *	�������ƣ�	LuaFnIsSpouses
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	�ж� selfId �� targetId �Ƿ��Ƿ���
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnIsSpouses
	INT LuaFnIsSpouses( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	11:26
	 *	�������ƣ�	LuaFnUnMarry
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				spouseId
	 *					��ż ID
	 *
	 *	����˵����	sceneId �� spouseId ���
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnUnMarry
	INT LuaFnUnMarry( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	13:11
	 *	�������ƣ�	LuaFnDivorce
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	���������
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnDivorce
	INT LuaFnDivorce( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-6	17:07
	 *	�������ƣ�	LuaFnIsMaster
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	�ж� targetId �Ƿ��� selfId ��ʦ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnIsMaster
	INT LuaFnIsMaster( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:03
	 *	�������ƣ�	LuaFnIsPrentice
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	�ж� targetId �Ƿ��� selfId ��ͽ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnIsPrentice
	INT LuaFnIsPrentice( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	20:17
	 *	�������ƣ�	LuaFnGetPrenticeGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				nIndex
	 *					�ڼ���ͽ��
	 *
	 *	����˵����	?
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetPrenticeGUID
	INT LuaFnGetPrenticeGUID( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:04
	 *	�������ƣ�	LuaFnHaveMaster
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�ж� selfId �Ƿ���ʦ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnHaveMaster
	INT LuaFnHaveMaster( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	19:57
	 *	�������ƣ�	LuaFnGetMasterGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ�ʦ���� GUID
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetMasterGUID
	INT LuaFnGetMasterGUID( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	19:21
	 *	�������ƣ�	LuaFnGetMasterMoralPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	ȡ�� selfId ��ʦ�µ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetMasterMoralPoint
	INT LuaFnGetMasterMoralPoint( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	19:22
	 *	�������ƣ�	LuaFnSetMasterMoralPoint
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				uMoralPoint
	 *					ʦ�µ�
	 *
	 *	����˵����	����ʦ�µ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSetMasterMoralPoint
	INT LuaFnSetMasterMoralPoint( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:07
	 *	�������ƣ�	LuaFnGetPrenticeCount
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	��Ҫͽ������
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetPrenticeCount
	INT LuaFnGetPrenticeCount( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:10
	 *	�������ƣ�	LuaFnGetPrenticeBetrayTime
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ����һ��ͽ����ʦ��ʱ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnGetPrenticeBetrayTime
	INT LuaFnGetPrenticeBetrayTime( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:11
	 *	�������ƣ�	LuaFnAprentice
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	selfId �� targetId Ϊʦ
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnAprentice
	INT LuaFnAprentice( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:18
	 *	�������ƣ�	LuaFnFinishAprentice
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	selfId �뿪 targetId ʦ�ǣ���ʽ��ʦ
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnFinishAprentice
	INT LuaFnFinishAprentice( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	19:47
	 *	�������ƣ�	LuaFnBetrayMaster
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	��ʦ
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnBetrayMaster
	INT LuaFnBetrayMaster( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	19:47
	 *	�������ƣ�	LuaFnExpelPrentice
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				PrenticeGUID
	 *					ͽ�ܵ� GUID
	 *
	 *	����˵����	����ĳ��ͽ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnExpelPrentice
	INT LuaFnExpelPrentice( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	10:40
	 *	�������ƣ�	LuaFnIsBrother
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				targetId
	 *					�Է� ID
	 *
	 *	����˵����	�ж� targetId �Ƿ��� selfId �Ľ���ֵ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnIsBrother
	INT LuaFnIsBrother( Lua_State* L)


	/****************************************************************************
	*	������Ա��	yuyazhou
	*	��дʱ�䣺	2010-5-21	
	*	�������ƣ�	LuaFnDelFriendByGUID
	*	����˵����
	*				sceneId
	*					�������
	*				selfId
	*					������ ID���޷�������
	*				myGUID
	*					���˵� GUID
	*				friendGUID
	*					���ѵ�GUID
	*
	*	����˵����	ͨ�����ѵ�GUID�������ߵĺ���ɾ��
	*	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnDelFriendByGUID
	INT LuaFnDelFriendByGUID(  Lua_State* L)


�ű�ϵͳC���������ĵ�	LuaFnTbl_Scene.h
/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetSceneType
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ���ص�ǰ���������ͣ������Config/SceneInfo.ini ��Type�ֶ�����
	*/
	�ú���ע��Ϊ LuaFnGetSceneType
	INT LuaFnGetSceneType( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnCreateCopyScene
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ����ֵ���������-1����ʾ����ʧ�ܣ����򷵻ش����ɹ��ĳ�����
	*				 �����Ѿ����úõ����ݴ�������, ����ѡ������ò�����
	*				 ����*SetSceneLoad_*����*SetCopySceneData_*�����͵ĺ���
	*/
	�ú���ע��Ϊ LuaFnCreateCopyScene
	INT LuaFnCreateCopyScene( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �¾�
	*     ��дʱ�� : 2010-07-21
	*     �������� : LuaFnGetSceneStatue
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 �õ�������״̬
	*/
	�ú���ע��Ϊ LuaFnGetSceneStatue
	INT LuaFnGetSceneStatue( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Map
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 mapName
	*					����ͼ�ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����ĵ���ͼ�ļ�������"*.nav", ����Ҫ��·��
	*				 ע�⣺�ڴ�������ʱ�򣬱������õ�ͼ�ļ��������ң��˵�ͼ�Ѿ�
	*					   ���õ�SceneInfo.ini��
	*/
	�ú���ע��Ϊ LuaFnSetSceneLoad_Map
	INT LuaFnSetSceneLoad_Map( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Monster
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 monsterName
	*					����ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����Ĺ���ֲ��ļ�,����"*_monster.ini",
	*				 ����Ҫ��·��
	*/
	�ú���ע��Ϊ LuaFnSetSceneLoad_Monster
	INT LuaFnSetSceneLoad_Monster( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Platform
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 platformName
	*					����̨�ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����Ĳ���̨�ֲ��ļ�,����"*_platform.ini",
	*				 ����Ҫ��·��
	*/
	�ú���ע��Ϊ LuaFnSetSceneLoad_Platform
	INT LuaFnSetSceneLoad_Platform( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_GrowPointData
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 growpointdataName
	*					������ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ�����������ֲ��ļ�,����"*_growpoint.ini",
	*				 ����Ҫ��·��
	*/
	�ú���ע��Ϊ LuaFnSetSceneLoad_GrowPointData
	INT LuaFnSetSceneLoad_GrowPointData( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_GrowPointSetup
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 growpointsetupName
	*					���������������ɵ���Ʒ�ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����Ĵ��������������ɵ���Ʒ�ֲ��ļ�,����"*_growpointsetup.ini",
	*				 ����Ҫ��·��
	*/
	�ú���ע��Ϊ LuaFnSetSceneLoad_GrowPointSetup
	INT LuaFnSetSceneLoad_GrowPointSetup( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Area
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 areaName
	*					�¼����ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ������¼����ֲ��ļ�,����"*_area.ini",
	*				 ����Ҫ��·��
	*/
	�ú���ע��Ϊ LuaFnSetSceneLoad_Area
	INT LuaFnSetSceneLoad_Area( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Pet
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 petName
	*					����ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����ĳ���ֲ��ļ�,����"*_pet.ini",
	*				 ����Ҫ��·��
	*/
	�ú���ע��Ϊ LuaFnSetSceneLoad_Pet
	INT LuaFnSetSceneLoad_Pet( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_PatrolPointData
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 patrolpointdataName
	*					Ѳ��·���ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ�����Ѳ��·���ֲ��ļ�,����"*_patrolpoint.ini",
	*				 ����Ҫ��·��
	*/
	�ú���ע��Ϊ LuaFnSetSceneLoad_PatrolPointData
	INT LuaFnSetSceneLoad_PatrolPointData( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetCopySceneData_TeamLeader
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 guidLeader
	*					�ӳ���GUID,uint
	*     ����˵�� :
	*				 ���ø�������������Ķӳ�GUID
	*/
	�ú���ע��Ϊ LuaFnSetCopySceneData_TeamLeader
	INT LuaFnSetCopySceneData_TeamLeader( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetCopySceneData_NoUserCloseTime
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 closeTime
	*					ʱ�䣨���룩,uint
	*     ����˵�� :
	*				 ���ø�������������ģ�û���˺󸱱������Զ��ر������ʱ�䡣
	*/
	�ú���ע��Ϊ LuaFnSetCopySceneData_NoUserCloseTime
	INT LuaFnSetCopySceneData_NoUserCloseTime( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetCopySceneData_Timer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 uTime
	*					ʱ�䣨���룩,uint
	*     ����˵�� :
	*				 ���ø�������������ģ�������ʱ����ʱ���������Ϊ0��ʾû�����á�
	*/
	�ú���ע��Ϊ LuaFnSetCopySceneData_Timer
	INT LuaFnSetCopySceneData_Timer( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetCopySceneData_Param
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 uIndex
	*					����,uint �����Ե�32
	*				 nValue
	*					ֵ,uint
	*     ����˵�� :
	*				 ���ø�������������ģ�����������Ϣ
	*/
	�ú���ע��Ϊ LuaFnSetCopySceneData_Param
	INT LuaFnSetCopySceneData_Param( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetCopySceneData_TeamLeader
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ���ظ�������������Ķӳ�GUID��ʧ�ܣ�1 
	*/
	�ú���ע��Ϊ LuaFnGetCopySceneData_TeamLeader
	INT LuaFnGetCopySceneData_TeamLeader( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetCopySceneData_NoUserCloseTime
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ����û���˺󸱱������Զ��ر������ʱ�䣨���룩��
	*/
	�ú���ע��Ϊ LuaFnGetCopySceneData_NoUserCloseTime
	INT LuaFnGetCopySceneData_NoUserCloseTime( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetCopySceneData_Timer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ��ȡ������ʱ����ʱ���������Ϊ0��ʾû�����á�
	*/
	�ú���ע��Ϊ LuaFnGetCopySceneData_Timer
	INT LuaFnGetCopySceneData_Timer( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetCopySceneData_Param
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 uIndex
	*					����,uint �����Ե�32
	*     ����˵�� :
	*				 ��ȡ����������Ϣ
	*/
	�ú���ע��Ϊ LuaFnGetCopySceneData_Param
	INT LuaFnGetCopySceneData_Param( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-18
	*     �������� : LuaFnGetCopyScene_HumanCount
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ���ص�ǰ����������������
	*/
	�ú���ע��Ϊ LuaFnGetCopyScene_HumanCount
	INT LuaFnGetCopyScene_HumanCount( Lua_State* L )	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-18
	*     �������� : LuaFnGetCopyScene_HumanObjId
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 uIndex
	*					����,uint �����Ե�32
	*     ����˵�� :
	*				 ��ȡ����������Ϣ
	*/
	�ú���ע��Ϊ LuaFnGetCopyScene_HumanObjId
	INT LuaFnGetCopyScene_HumanObjId( Lua_State* L)	


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnIsObjValid
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 objId
	*					objId ��
	*     ����˵�� :
	*				 �ж��Ƿ�ӵ��objId�ŵ�Obj
	*/
	�ú���ע��Ϊ LuaFnIsObjValid
	INT LuaFnIsObjValid( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnIsCharacterLiving
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 objId
	*					objId ��
	*     ����˵�� :
	*				 �ж�objId�ŵ�Obj�Ƿ��ǻ��
	*/
	�ú���ע��Ϊ LuaFnIsCharacterLiving
	INT LuaFnIsCharacterLiving( Lua_State* L)	


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	10:28
	 *	�������ƣ�	LuaFnSendNormalMail
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				objId
	 *					������ ID���޷�������
	 *				szName
	 *					�ռ�������
	 *				szMail
	 *					�ʼ�����
	 *
	 *	����˵����	������ͨ�ʼ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSendNormalMail
	INT LuaFnSendNormalMail( Lua_State* L )


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	13:30
	 *	�������ƣ�	LuaFnSendScriptMail
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				szName
	 *					�ռ�������
	 *				uParam0 ~ uParam3
	 *					��ִ���ʼ��Դ��� 4 ������
	 *
	 *	����˵����	���Ϳ�ִ���ʼ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSendScriptMail
	INT LuaFnSendScriptMail( Lua_State* L)


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-4	17:46
	 *	�������ƣ�	LuaFnSendMailToAllFriend
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				szMail
	 *					�ʼ����� 
	 *				SpouseId����żID��Ĭ��Ϊʱ��ʾ������ż
	 *
	 *	����˵����	�����к��ѷ�����ͨ�ʼ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ LuaFnSendMailToAllFriend
	INT LuaFnSendMailToAllFriend( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaFnGetMonsterCount
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ȡ�ô˳����еĹ�������
	*/
	�ú���ע��Ϊ GetMonsterCount
	INT LuaFnGetMonsterCount( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaFnGetMonsterObjID
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 nIndex
	*					����������
	*     ����˵�� :
	*				 ����������ȡ�ù����ObjID
	*/
	�ú���ע��Ϊ GetMonsterObjID
	INT LuaFnGetMonsterObjID( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-05-15
	*     �������� : LuaFnIsCanEnterCopyScene
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 guid
	*					���GUID
	*     ����˵�� :
	*				 �ж�����Ƿ���Խ��볡��sceneId, 
	*					����1 ��ʾ�ܹ���������0��ʾ���ܽ�, -1��ʾ����
	*/
	�ú���ע��Ϊ IsCanEnterCopyScene
	INT LuaFnIsCanEnterCopyScene( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-04-17
	*     �������� : LuaFnGetNewSceneInfoByIndex
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 index
	*					�ɽ���ĳ�����������
	*     ����˵�� :
	*				 ���ݿ���ת���������±꣬ �õ���ҽ���ó���������С�ȼ���λ�á�ID��Ϣ 
	*					
	*/
	�ú���ע��Ϊ LuaFnGetNewSceneInfoByIndex
	INT LuaFnGetNewSceneInfoByIndex( Lua_State* L)	


�ű�ϵͳC���������ĵ�	LuaFnTbl_Shop.h
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-12
	*     �������� : LuaFnDispatchShopItem
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				ShopId
	*					�̵���
	*				
	*/
	�ú���ע��Ϊ DispatchShopItem
	INT	LuaFnDispatchShopItem(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : yu
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchXinfaInfo
	*     ����˵�� : 
	*     ����˵�� : �����ķ���ʦʹ��
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				MenpaiId
	*					�����ʦ��ID������ID��
	*/
	�ú���ע��Ϊ DispatchXinfaLevelInfo
	INT LuaFnDispatchXinfaLevelInfo( Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : yu
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchMenpaiInfo
	*     ����˵�� : 
	*     ����˵�� : ���ɵ�ʦʹ��
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				MenpaiId
	*					�����ʦ��ID������ID��
	*/				
	�ú���ע��Ϊ DispatchMenpaiInfo
	INT LuaFnDispatchMenpaiInfo( Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : yu
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchAbilityInfo
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				AbilityId
	*					�����ʦ���̵�����ܵ�ID
	*/
	�ú���ע��Ϊ DispatchAbilityInfo
	INT LuaFnDispatchAbilityInfo( Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     ������Ա : yu
	*     ��дʱ�� : 2006-01-09
	*     �������� : LuaFnJoinMenpai
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				AbilityId
	*					�����ʦ���̵�����ܵ�ID
	*/
	�ú���ע��Ϊ LuaFnJoinMenpai
	INT LuaFnJoinMenpai( Lua_State* L)


�ű�ϵͳC���������ĵ�	LuaFnTbl_Team.h
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetTeamId
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				������Ҷ���ID
	*				û�в���
	*/
	�ú���ע��Ϊ GetTeamId
	INT LuaFnGetTeamId( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetTeamSize
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����������ڶ�������
	*				û�в���
	*/
	�ú���ע��Ϊ GetTeamSize
	�ú���ע��Ϊ LuaFnGetTeamSize
	INT LuaFnGetTeamSize( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnLeaveTeam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				������뿪�������
	*				û�в���
	*/
	�ú���ע��Ϊ LeaveTeam
	INT LuaFnLeaveTeam( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnMsg2Player
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				msg
	*					��������,�ַ���ָ������
	*				type
	*					CHAT_TYPE_NORMAL =0 ,	//��ͨ˵����Ϣ
	*					CHAT_TYPE_TEAM = 1,		//������Ϣ
	*					CHAT_TYPE_WORLD,		//����
	*					CHAT_TYPE_USER ,		//˽����Ϣ
	*					CHAT_TYPE_SYSTEM = 4,	//ϵͳ��Ϣ
	*					CHAT_TYPE_JIAZU ,		//������Ϣ
	*					CHAT_TYPE_GUILD = 6,	//������Ϣ
	*					CHAT_TYPE_COUNTRY,		//����
	*					CHAT_TYPE_SELF = 8,		//���ͻ���ʹ�õ���Ϣ
	*					
	*				pos
	*					ϵͳ����ʱ���ͻ�����ʾ��λ�ã�Ĭ��Ϊ0
	*					CHAT_MAIN_WINDOW = 0,	//��ʾ������������촰����
	*					CHAT_LEFTDOWN,			//���½Ǹ��˵�ϵͳ��ʾ��
	*					CHAT_RIGHTDOWN,			//���½�ϵͳ��ϵͳ��ʾ��
	*					CHAT_PLUMB_SCROLL,		//��ֱ������ʾ��
	*					CHAT_PLANE_SCROLL,		//ˮƽ������ʾ��
	*     ����˵�� :
	*				����Ϣ���͸����
	*				��Ϣ��ʾ�����½ǵ��������
	*				typeĬ��ֵΪCHAT_TYPE_NORMAL,��0
	*
	*				[junyang 5/8/2006]
	*				��Ǯ��ʾ��Ϣ����Ҫд��#{_MONEY%d}��ʽ
	*				���磺������#{_MONEY208934}�� 
	*				��仰�ڿͻ�����ʾ��ʱ����Զ������������ͭ�����������ö�Ӧ��ͼ���ʾ������
	*				
	*/
	�ú���ע��Ϊ Msg2Player
	�ú���ע��Ϊ LuaFnMsg2Player
	INT LuaFnMsg2Player( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-19
	*     �������� : LuaFnNpcMsg2Player
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				msg
	*					��������,�ַ���ָ������
	*				type
	*					CHAT_TYPE_NORMAL =0 ,	//��ͨ˵����Ϣ
	*					CHAT_TYPE_TEAM = 1,		//������Ϣ
	*					CHAT_TYPE_WORLD,		//����
	*					CHAT_TYPE_USER ,		//˽����Ϣ
	*					CHAT_TYPE_SYSTEM = 4,	//ϵͳ��Ϣ
	*					CHAT_TYPE_JIAZU ,		//������Ϣ
	*					CHAT_TYPE_GUILD = 6,	//������Ϣ
	*					CHAT_TYPE_COUNTRY,		//����
	*					CHAT_TYPE_SELF = 8,		//���ͻ���ʹ�õ���Ϣ
	*					
	*				DestId:
	*					��ʶ���͵�Ƶ������NPC����λ�õĸ���Ƶ����ָ���Ķ���Ƶ����								
	*					��������ڰ�ᣬ����ID�Ӱ��ID����Ψһ��ʶ
	*					��˼���Ƶ��ʱ��DestId����ʾ����IDȴ��ʾ���ID,ʲô���߰����
	*
	*				JiaZuID�������ڼ���Ƶ��
	*
	*     ����˵�� :
	*				NPC����Ϣ���͸����
	*				��Ϣ��ʾ�����½ǵ��������
	*				typeĬ��ֵΪCHAT_TYPE_NORMAL,��0
	*
	*				[junyang 5/8/2006]
	*				��Ǯ��ʾ��Ϣ����Ҫд��#{_MONEY%d}��ʽ
	*				���磺������#{_MONEY208934}�� 
	*				��仰�ڿͻ�����ʾ��ʱ����Զ������������ͭ�����������ö�Ӧ��ͼ���ʾ������
	*				
	*/
	�ú���ע��Ϊ NpcMsg2Player
	INT LuaFnNpcMsg2Player( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnMsg2Team
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				msg
	*					��������,�ַ���ָ������
	*     ����˵�� :
	*				����Ϣ���͸��������������г�Ա
	*				��������δ��д
	*/
	�ú���ע��Ϊ Msg2Team
	INT LuaFnMsg2Team( Lua_State* L)	


	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-1-17	17:53
	 *	�������ƣ�	LuaFnIsTeamFollow
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ� selfId ��Ӧ������Ƿ�����Ӹ���״̬
	 *	�޸ļ�¼��
	*****************************************************************************/
	�ú���ע��Ϊ IsTeamFollow
	INT LuaFnIsTeamFollow( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnGetFollowedMembersCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				�õ�����״̬�Ķ�������
	*				û�в���
	*/
	�ú���ע��Ϊ GetFollowedMembersCount
	�ú���ע��Ϊ LuaFnGetFollowedMembersCount
	INT LuaFnGetFollowedMembersCount( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-27
	*     �������� : LuaFnGetFollowedMember
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				memberIndex
	*					�ڸ�������е��±�����,��0��ʼ,����			

	*     ����˵�� :
	*				�õ�����״̬��memberIndex�±�Ķ���objId
	*				û�в���
	*				2006-3-16 �޸� objid �Ļ�ȡ������fancy
	*/
	�ú���ע��Ϊ GetFollowedMember
	INT LuaFnGetFollowedMember( Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnStopTeamFollow
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				ֹͣ����,���selfId��־�����
	*				û�в���
	*/
	�ú���ע��Ϊ StopTeamFollow
	INT LuaFnStopTeamFollow( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnClearFollowedMembers
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				������еĸ�������б�
	*				û�в���
	*/
	�ú���ע��Ϊ ClearFollowedMembers
	INT LuaFnClearFollowedMembers( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnGetTeamSceneMemberCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				������ͬ������Ա���� human
	*				û�в���
	*/
	�ú���ע��Ϊ GetTeamSceneMemberCount
	�ú���ע��Ϊ LuaFnGetTeamSceneMemberCount
	INT LuaFnGetTeamSceneMemberCount( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnGetTeamSceneMember
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*				memberIndex
	*					�ڶ����е��±�����,��0��ʼ,����			
	*     ����˵�� :
	*				ȡ�ö�����ĳͬ������Ա�� OBJID
	*				û�в���
	*/
	�ú���ע��Ϊ LuaFnGetTeamSceneMember
	INT LuaFnGetTeamSceneMember( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnSetTeamFollowSpeed
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				������Ӹ�����ٶ�
	*				û�в���
	*/
	�ú���ע��Ϊ SetTeamFollowSpeed
	INT LuaFnSetTeamFollowSpeed( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnIsTeamLeader
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				selfId����Ƿ�Ϊ�ӳ�
	*				����1,��
	*				����0,��
	*				û�в���
	*/
	�ú���ע��Ϊ LuaFnIsTeamLeader
	INT LuaFnIsTeamLeader( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnGetTeamMemberGuid
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*				memberIndex
	*					�ڶ����е��±�����,��0��ʼ,����			
	*     ����˵�� :
	*				���selfId����Team�е�memberIndex���±���ҵ�Guid
	*				û�в���
	*/
	�ú���ע��Ϊ GetTeamMemberGuid
	INT LuaFnGetTeamMemberGuid( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnHasTeam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*     ����˵�� :
	*				�ж�selfId����Ƿ������״̬
	*				û�в���
	*/
	�ú���ע��Ϊ LuaFnHasTeam
	INT LuaFnHasTeam( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-17
	*     �������� : LuaFnGetNearTeamCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*     ����˵�� :
	*				ȡ��selfId��Ҹ����Ķ�������(�����Լ�)
	*				�����Ҳ�����ĳ�����飬�򷵻�-1
	*/
	�ú���ע��Ϊ GetNearTeamCount
	INT LuaFnGetNearTeamCount( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnGetNearTeamMember
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*				index
	*					�������ѵ�������
	*     ����˵�� :
	*				ȡ��selfId��Ҹ������ѵ�objid(�����Լ�)
	*/
	�ú���ע��Ϊ GetNearTeamMember
	INT LuaFnGetNearTeamMember( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-33
	*     �������� : LuaFnGetMonsterGroupID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					��Ҫȡ�����ݵĹ����objid
	*     ����˵�� :
	*				ȡ��selfId�����GroupID, monster.ini��������
	*/
	�ú���ע��Ϊ GetMonsterGroupID
	INT LuaFnGetMonsterGroupID( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-33
	*     �������� : LuaFnGetMonsterTeamID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					��Ҫȡ�����ݵĹ����objid
	*     ����˵�� :
	*				ȡ��selfId�����TeamID, monster.ini��������
	*/
	�ú���ע��Ϊ GetMonsterTeamID
	INT LuaFnGetMonsterTeamID( Lua_State* L)


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaFnGetMonsterDataID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					��Ҫȡ�����ݵĹ����objid
	*     ����˵�� :
	*				ȡ��selfId�����DataID, monster.ini��������
	*/
	�ú���ע��Ϊ GetMonsterDataID
	INT LuaFnGetMonsterDataID( Lua_State* L)


�ű�ϵͳC���������ĵ�	LuaFnRegister.cpp
	/**/
	/**/
�ĵ���������:2010��7��2��19ʱ