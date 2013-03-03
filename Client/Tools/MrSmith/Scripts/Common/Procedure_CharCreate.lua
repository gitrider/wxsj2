--------------------------------------
-- 创建角色流程
--------------------------------------
local STATE_CREATECHAR 					= "create_char";					--创建角色
local STATE_WAIT_CREATECHAR 		= "wait_create_char";			--等待创建角色
local STATE_CREATECHAR_OK				= "create_char_success";	--创建角色成功
local STATE_CREATECHAR_REPEAT		= "create_char_repeat";		--创建角色失败，服务器忙，重试
local STATE_CREATECHAR_HIGHFRE	= "create_char_too_fast";	--创建角色失败，操作过于频繁
local STATE_CREATECHAR_FULL			= "create_char_full";			--创建角色失败，角色已经满了
local STATE_CREATECHAR_PROC			= "create_char_proc";			--创建角色失败，错误操作流程
local STATE_CREATECHAR_IN				= "create_char_inner";		--创建角色失败，内部错误
local STATE_CREATECHAR_NAME			= "create_char_name";			--创建角色失败，名字重复
local STATE_CREATECHAR_UNKNOW		= "create_char_unknow";		--创建角色失败，未知错误
local STATE_DELETECHAR 					= "delete_char";					--删除角色

local STATE_CURRENT; --状态

--设置状态
function Procedure_CharCreate_SetState(stateName)
	STATE_CURRENT = stateName
end

function Procedure_CharCreate_Init()

end

function Procedure_CharCreate_Tick()

	if( STATE_CURRENT == STATE_CREATECHAR ) then
		Util:Sleep( 1000 );
		Loger:AddLog("创建角色..." );
		NetManager:SendMessage("CLAskCreateChar");
		--Procedure_CharCreate_SetState("wait_create_char");
		Procedure_CharSel_SetState("wait_ask_char_list");
		Procedure:SetCurrentProcedure("CharSel");
	
	elseif( STATE_CURRENT == STATE_WAIT_CREATECHAR ) then
		Loger:AddLog("等待创建角色，进入选择用户的流程了");
		--Procedure_CharSel_SetState("wait_ask_char_list");
		--Procedure:SetCurrentProcedure("CharSel");
		--Loger:AddLog(STATE_CURRENT);
		
	elseif( STATE_CURRENT == STATE_CREATECHAR_OK ) then
		--Loger:AddLog("创建角色成功，进入什么流程呢");

		Procedure_CharSel_SetState("wait_ask_char_list");
		Procedure:SetCurrentProcedure("CharSel");
		--Loger:AddLog(STATE_CURRENT);
		
	elseif( STATE_CURRENT == STATE_CREATECHAR_REPEAT ) then
		Loger:AddLog("创建角色失败，服务器忙，重试...");
		Util:Sleep( 800 );
		Procedure_CharCreate_SetState("create_char");

	elseif( STATE_CURRENT == STATE_CREATECHAR_HIGHFRE ) then
		Loger:AddLog("创建角色失败，操作过于频繁，重试...");
		Util:Sleep( 800 );
		Procedure_CharCreate_SetState("create_char");

	elseif( STATE_CURRENT == STATE_CREATECHAR_FULL ) then
		Loger:AddLog("创建角色失败，角色已经满了...");
		Util:Sleep( 800 );
		Procedure_CharSel_SetState("enter_scene");
		Procedure:SetCurrentProcedure("CharSel");

	elseif( STATE_CURRENT == STATE_CREATECHAR_PROC ) then
		Loger:AddLog("创建角色失败，错误操作流程，重新登陆...");
		Util:Sleep( 2000 );
		Procedure_Login_SetState("disconnect");
		Procedure:SetCurrentProcedure("Login");

	elseif( STATE_CURRENT == STATE_CREATECHAR_IN ) then
		Loger:AddLog("创建角色失败，内部错误，重试...");
		Util:Sleep( 800 );
		Procedure_CharCreate_SetState("create_char");

	elseif( STATE_CURRENT == STATE_CREATECHAR_NAME ) then
		Loger:AddLog("创建角色失败，名字重复，重试...");
		--Procedure_CharCreate_SetState("create_char");
		Util:Sleep( 5000 );
		Procedure_Login_SetState("disconnect2");
		Procedure:SetCurrentProcedure("Login");

	elseif( STATE_CURRENT == STATE_CREATECHAR_UNKNOW ) then
		Loger:AddLog("创建角色失败，未知错误，重试...");
		Procedure_CharCreate_SetState("create_char");

	elseif( STATE_CURRENT == STATE_DELETECHAR ) then
		Loger:AddLog("删除角色..." );
		Util:Sleep( 1000 );
		NetManager:SendMessage("CLAskDeleteChar");
		Procedure_CharSel_SetState("wait_ask_char_list");
		Procedure:SetCurrentProcedure("CharSel");

	end

end

function Procedure_CharCreate_Release()

end

