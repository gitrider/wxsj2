--------------------------------------
--Global Define
--------------------------------------
local	g_ProcedureTable						= {};	--�����ܱ�

local g_ProcedureTable_Login			= {};
local g_ProcedureTable_CharSel		= {};
local g_ProcedureTable_CharCreate	= {};
local g_ProcedureTable_GameServer	= {};
local g_ProcedureTable_EnterScene	= {};
local g_ProcedureTable_Main				= {};

local	g_IsEnterLogin							= 1;	--�Ƿ��¼Login

--------------------------------------
--�����˳�ʼ������
--------------------------------------
function Agent_Init()
	--��¼Login����
	g_ProcedureTable_Login["Init"] 				= Procedure_Login_Init;
	g_ProcedureTable_Login["Tick"] 				= Procedure_Login_Tick;
	g_ProcedureTable_Login["Release"] 		= Procedure_Login_Release;
		                                  	
	--ѡ���ɫ����                      	
	g_ProcedureTable_CharSel["Init"] 			= Procedure_CharSel_Init;
	g_ProcedureTable_CharSel["Tick"] 			= Procedure_CharSel_Tick;
	g_ProcedureTable_CharSel["Release"]		= Procedure_CharSel_Release;

	--������ɫ����
	if g_IsEnterLogin == 1 then
		g_ProcedureTable_CharCreate["Init"] 	= Procedure_CharCreate_Init;
		g_ProcedureTable_CharCreate["Tick"] 	= Procedure_CharCreate_Tick;
		g_ProcedureTable_CharCreate["Release"]= Procedure_CharCreate_Release;
	end

	--������Ϸ����������
	g_ProcedureTable_GameServer["Init"] 	= Procedure_GameServer_Init;
	g_ProcedureTable_GameServer["Tick"] 	= Procedure_GameServer_Tick;
	g_ProcedureTable_GameServer["Release"]= Procedure_GameServer_Release;
	
	--���볡������
	g_ProcedureTable_EnterScene["Init"] 	= Procedure_EnterScene_Init;
	g_ProcedureTable_EnterScene["Tick"] 	= Procedure_EnterScene_Tick;
	g_ProcedureTable_EnterScene["Release"]= Procedure_EnterScene_Release;

	--������
	g_ProcedureTable_Main["Init"] 				= Procedure_Main_Init;
	g_ProcedureTable_Main["Tick"] 				= Procedure_Main_Tick;
	g_ProcedureTable_Main["Release"] 			= Procedure_Main_Release;
	
	--�����ܱ�
	g_ProcedureTable["Login"] 						= g_ProcedureTable_Login;
	g_ProcedureTable["CharSel"] 					= g_ProcedureTable_CharSel;
	if g_IsEnterLogin == 1 then
		g_ProcedureTable["CharCreate"]			= g_ProcedureTable_CharCreate;
	end
	g_ProcedureTable["GameServer"]				= g_ProcedureTable_GameServer;
	g_ProcedureTable["EnterScene"]				= g_ProcedureTable_EnterScene;
	g_ProcedureTable["Main"]							= g_ProcedureTable_Main;
	
	--��ʼ���������
	math.randomseed( Util:HashString2CRC( Util:CreateRandName() ) );

	local	StartWay
	if g_IsEnterLogin == 1 then
		StartWay	= "disconnect_L"	--����Login������
	else
		StartWay	= "disconnect_G"	--ֱ��������Ϸ������
	end

	Procedure_Login_SetState( StartWay );
	Procedure:SetCurrentProcedure( "Login" );

end

--------------------------------------
--����ָ������ָ������
--------------------------------------
function Robot_ExcuteProcedure(procedureName, funcName)
	local currentPro = g_ProcedureTable[procedureName];
	if(currentPro == nil) then
		--δ���������
		return;
	end
	
	local functionPro = currentPro[funcName];
	if(functionPro == nil) then
		--δ����ĺ���
		Loger:AddLog("UNDEFINE FUNC ".. procedureName .. "[" .. funcName .. "]");
		return;
	end
	
	--��ʼ��
	functionPro();
end