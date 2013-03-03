
--------------------------------------
-- 进入场景流程
--------------------------------------
local STATE_READY 			= "ready";			--准备完毕
local STATE_REQUESTING		= "requesting";		--发送进入场景的请求,等待服务器的回应...
local STATE_ENTER_OK		= "enter_ok";		--服务器允许进入场景
local STATE_ENTER_FAILED	= "enter_failed";	--服务器拒绝进入场景
local STATE_CHANGE_SCENE	= "change_scene";	--切换场景
local STATE_CHANGE_OK		= "change_ok"; -- 切换场景成功
local STATE_CHANGE_OK2		= "change_ok2"; -- 切换不同的Server
local STATE_CHANGE_ERR		= "change_err"; -- 切换场景错误

local STATE_CURRENT;


--设置状态
function Procedure_EnterScene_SetState(stateName)
	STATE_CURRENT = stateName;
end

function Procedure_EnterScene_Init()
	
end

function Procedure_EnterScene_Tick()
	
	-----------------------------------------------------------
	if(STATE_CURRENT == STATE_READY) then
		Util:Sleep( 10 );
		Loger:AddLog("发送进入场景的请求...!");
		NetManager:SendMessage("CGEnterScene");
		Procedure_EnterScene_SetState(STATE_REQUESTING);
	
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CHANGE_SCENE) then	
		Util:Sleep( 10 );
		Loger:AddLog("发送切换场景的请求...!");
		NetManager:SendMessage("CGAskChangeScene");
		Procedure_EnterScene_SetState(STATE_REQUESTING);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CHANGE_OK) then
		Loger:AddLog("切换场景的请求成功");
		Procedure_EnterScene_SetState(STATE_READY);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CHANGE_OK2) then
		Loger:AddLog("切换场景的请求成功了!!  我去别的服务器了 ^_^");
		Procedure_GameServer_SetState("disconnect");
		Procedure:SetCurrentProcedure("GameServer");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_CHANGE_ERR) then
		Loger:AddLog("切换场景的请求失败了...  :(");
		Procedure_Main_SetState("rand_move");
		Procedure:SetCurrentProcedure("Main");
	
	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_REQUESTING) then
		--Loger:AddLog(STATE_CURRENT);

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ENTER_OK) then
		--进入场景
		Loger:AddLog("进入场景[" .. Variable:GetVariable("Scene_ID") .. "]!" );
		--WorldManager:EnterScene();

		Procedure_Main_SetState("request_scene");
		Procedure:SetCurrentProcedure("Main");

	-----------------------------------------------------------
	elseif(STATE_CURRENT == STATE_ENTER_FAILED) then
		--do something!!!
		--Loger:AddLog(STATE_CURRENT);
		
	else
	
	end
end

function Procedure_EnterScene_Release()

end

