--------------------------------------
--Global Define
--------------------------------------
local	g_ProcedureTable						= {};	--流程总表

local g_ProcedureTable_Login			= {};
local g_ProcedureTable_CharSel		= {};
local g_ProcedureTable_CharCreate	= {};
local g_ProcedureTable_GameServer	= {};
local g_ProcedureTable_EnterScene	= {};
local g_ProcedureTable_Main				= {};

local	g_IsEnterLogin							= 1;	--是否登录Login

--------------------------------------
--机器人初始化函数
--------------------------------------
function Agent_Init()
	--登录Login流程
	g_ProcedureTable_Login["Init"] 				= Procedure_Login_Init;
	g_ProcedureTable_Login["Tick"] 				= Procedure_Login_Tick;
	g_ProcedureTable_Login["Release"] 		= Procedure_Login_Release;
		                                  	
	--选择角色流程                      	
	g_ProcedureTable_CharSel["Init"] 			= Procedure_CharSel_Init;
	g_ProcedureTable_CharSel["Tick"] 			= Procedure_CharSel_Tick;
	g_ProcedureTable_CharSel["Release"]		= Procedure_CharSel_Release;

	--创建角色流程
	if g_IsEnterLogin == 1 then
		g_ProcedureTable_CharCreate["Init"] 	= Procedure_CharCreate_Init;
		g_ProcedureTable_CharCreate["Tick"] 	= Procedure_CharCreate_Tick;
		g_ProcedureTable_CharCreate["Release"]= Procedure_CharCreate_Release;
	end

	--连接游戏服务器流程
	g_ProcedureTable_GameServer["Init"] 	= Procedure_GameServer_Init;
	g_ProcedureTable_GameServer["Tick"] 	= Procedure_GameServer_Tick;
	g_ProcedureTable_GameServer["Release"]= Procedure_GameServer_Release;
	
	--进入场景流程
	g_ProcedureTable_EnterScene["Init"] 	= Procedure_EnterScene_Init;
	g_ProcedureTable_EnterScene["Tick"] 	= Procedure_EnterScene_Tick;
	g_ProcedureTable_EnterScene["Release"]= Procedure_EnterScene_Release;

	--主流程
	g_ProcedureTable_Main["Init"] 				= Procedure_Main_Init;
	g_ProcedureTable_Main["Tick"] 				= Procedure_Main_Tick;
	g_ProcedureTable_Main["Release"] 			= Procedure_Main_Release;
	
	--流程总表
	g_ProcedureTable["Login"] 						= g_ProcedureTable_Login;
	g_ProcedureTable["CharSel"] 					= g_ProcedureTable_CharSel;
	if g_IsEnterLogin == 1 then
		g_ProcedureTable["CharCreate"]			= g_ProcedureTable_CharCreate;
	end
	g_ProcedureTable["GameServer"]				= g_ProcedureTable_GameServer;
	g_ProcedureTable["EnterScene"]				= g_ProcedureTable_EnterScene;
	g_ProcedureTable["Main"]							= g_ProcedureTable_Main;
	
	--初始化随机种子
	math.randomseed( Util:HashString2CRC( Util:CreateRandName() ) );

	local	StartWay
	if g_IsEnterLogin == 1 then
		StartWay	= "disconnect_L"	--连接Login服务器
	else
		StartWay	= "disconnect_G"	--直接连接游戏服务器
	end

	Procedure_Login_SetState( StartWay );
	Procedure:SetCurrentProcedure( "Login" );

end

--------------------------------------
--调用指定流程指定函数
--------------------------------------
function Robot_ExcuteProcedure(procedureName, funcName)
	local currentPro = g_ProcedureTable[procedureName];
	if(currentPro == nil) then
		--未定义的流程
		return;
	end
	
	local functionPro = currentPro[funcName];
	if(functionPro == nil) then
		--未定义的函数
		Loger:AddLog("UNDEFINE FUNC ".. procedureName .. "[" .. funcName .. "]");
		return;
	end
	
	--初始化
	functionPro();
end