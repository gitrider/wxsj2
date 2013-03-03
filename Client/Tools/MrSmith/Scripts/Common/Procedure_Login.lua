
--------------------------------------
-- 登录Login流程
--------------------------------------
local STATE_DISCONNECT_L				= "disconnect_L";			--断网状态，连接Login服务器
local STATE_DISCONNECT_G				= "disconnect_G";			--断网状态，直接连接游戏服务器
local STATE_CONNECTING 					= "connecting";				--正在连接Login服务器
local STATE_CONNECTED_SUCCESS 	= "conn_success";			--成功连接Login
local STATE_CONNECTED_FAILED 		= "conn_failed";			--连接Login失败
local STATE_CONNECT_VALIDATING	= "conn_validating"		--已经发送CLConnect
local STATE_CONNECT_VAL_SUCCESS	= "conn_val_success"	--连接验证成功
local STATE_CONNECT_VAL_FAILED	= "conn_val_failed"		--连接验证失败
local STATE_ACCOUNT_VALIDATING	= "account_validating"--帐号验证中
local STATE_ACCOUNT_VAL_SUCCESS	= "accnt_val_success"	--帐号验证成功
local STATE_ACCOUNT_VAL_FAILED	= "accnt_val_failed"	--帐号验证失败

local STATE_CURRENT;

--设置状态
function Procedure_Login_SetState(stateName)
	STATE_CURRENT = stateName;	
end

--初始化
function Procedure_Login_Init()

end

--Tick
function Procedure_Login_Tick()

	-----------------------------------------------------------
	if(STATE_CURRENT == STATE_DISCONNECT_L) then
		--连接Login服务器
		local ip	= Variable:GetVariable("LoginServer_Address");
		local port= Variable:GetVariable("LoginServer_Port");
		local	name= Variable:GetVariable("AccountName");
		Loger:AddLog("Connect to loginserver..." .. ip .. "," .. port);
		
		--反复尝试
		local	connErrCode	= -2;
		local	log	= 0;
		while log < 1000 do
			log							= log + 1;
			connErrCode			= NetManager:ConnectToServer(ip, tonumber(port), "LoginServer");
			if connErrCode >= 0 then
				break
			end
		end

		if(connErrCode == 0) then
			Loger:AddLog("LoginServer SOCKET 连接成功..." .. name );
			Procedure_Login_SetState(STATE_CONNECTED_SUCCESS);
		else
			Procedure_Login_SetState(STATE_CONNECTED_FAILED);
		end

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_DISCONNECT_G) then
		--直接连接游戏服务器
		local	ip	= Variable:GetVariable("LoginServer_Address");
		local	port= Variable:GetVariable("LoginServer_Port");
		local	name= Variable:GetVariable("AccountName");
		Variable:SetVariable("GameServer_Address",ip);
		Variable:SetVariable("GameServer_Port",port);
		Variable:SetVariable("User_NAME",name);
		Procedure_GameServer_SetState("disconnect");
		Procedure:SetCurrentProcedure("GameServer");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTING) then
		--Loger:AddLog(STATE_CURRENT);
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTED_SUCCESS) then
		Util:Sleep( 1000 );
		NetManager:SendMessage("CLConnect");
		Procedure_Login_SetState(STATE_CONNECT_VALIDATING);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTED_FAILED) then
		Util:ThrowException("LoginServer SOCKET 连接失败...");
				
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VALIDATING) then
		--Loger:AddLog(STATE_CURRENT);
	
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VAL_SUCCESS) then
		--发送帐号密码
		Util:Sleep( 800 );
		NetManager:SendMessage("CLAskLogin");
		Procedure_Login_SetState(STATE_ACCOUNT_VALIDATING);
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VAL_FAILED) then
		Util:ThrowException("LoginServer 验证失败!");
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ACCOUNT_VALIDATING) then
		--等待帐号验证
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ACCOUNT_VAL_SUCCESS) then
		--请求角色列表
		Loger:AddLog("帐号验证通过，进入角色选择流程...");
		Procedure_CharSel_SetState("ask_char_list"); --发送askcharlist
		Procedure:SetCurrentProcedure("CharSel");
		
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ACCOUNT_VAL_FAILED) then
		Util:ThrowException("LoginServer 帐号登陆失败 " .. Variable:GetVariable("LoginErrorDesc"));
	else
	
	end
end

--流程退出
function Procedure_Login_Release()

end

