--------------------------------------
-- 连接游戏服务器流程
--------------------------------------
local STATE_DISCONNECT 			= "disconnect";			--断网状态
local STATE_CONNECTING 			= "connecting";			--正在连接GameServer
local STATE_CONNECTED_SUCCESS 	= "conn_success";		--成功连接GameServer
local STATE_CONNECTED_FAILED 	= "conn_failed";		--连接GameServer失败
local STATE_CONNECT_VALIDATING	= "conn_validating"		--已经发送CGConnect
local STATE_CONNECT_VAL_SUCCESS	= "conn_val_success"	--连接验证成功
local STATE_CONNECT_VAL_FAILED	= "conn_val_failed"		--连接验证失败

local STATE_CURRENT;

local	g_IsEnterLogin						= 1;									--是否登录Login

--设置状态
function Procedure_GameServer_SetState(stateName)
	STATE_CURRENT = stateName
end

function Procedure_GameServer_Init()

end

function Procedure_GameServer_Tick()

	-----------------------------------------------------------
	if(STATE_CURRENT == STATE_DISCONNECT) then
		--连接Game服务器
		local ip = Variable:GetVariable("GameServer_Address");
		local port = Variable:GetVariable("GameServer_Port");
		Loger:AddLog("Connect to gameserver..." .. ip .. "," .. port);
		local connErrCode = NetManager:ConnectToServer(ip, tonumber(port), "GameServer");

		if(connErrCode == 0) then
			Loger:AddLog("GameServer SOCKET 连接成功...");
			Procedure_GameServer_SetState(STATE_CONNECTED_SUCCESS);
		else
			Procedure_GameServer_SetState(STATE_CONNECTED_FAILED);
		end
				
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTING) then

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTED_SUCCESS) then
	
		local	name	= Variable:GetVariable("AccountName");
		local	hash	= Util:HashString2CRC(name);
		local	sex		= Variable:GetVariable("AccountGender");
		if sex == "" then
			sex				= "0"
		end

		--直接连接游戏服务器
		if g_IsEnterLogin ~= 1 then
			Variable:SetVariable("User_NAME", name);
			Variable:SetVariable("User_GUID", hash);
			Variable:SetVariable("User_GENDER", sex);
			Variable:SetVariable("GameServer_Key", "-1");
		end

		NetManager:SendMessage("CGConnect");

		local	str		= string.format( "等待连接验证中...(NAME:%s,GUID:%X", name, hash )
		Loger:AddLog(str);
		Procedure_GameServer_SetState(STATE_CONNECT_VALIDATING);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECTED_FAILED) then
		Util:ThrowException("GameServer SOCKET 连接失败...");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VALIDATING) then

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VAL_SUCCESS) then
		--转入进入场景流程
		Loger:AddLog("连接验证成功!");
	
		Procedure_EnterScene_SetState("ready");
		Variable:SetVariable("Scene_Entertype", "0");
		Procedure:SetCurrentProcedure("EnterScene");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CONNECT_VAL_FAILED) then
		--do something!!!!

	else

	end
end

function Procedure_GameServer_Release()

end
