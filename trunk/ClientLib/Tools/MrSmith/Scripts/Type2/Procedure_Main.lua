--------------------------------------
-- Main流程
--------------------------------------
local STATE_REQ_SCENE			= "request_scene";			--请求场景
local STATE_ASK_BAITAN			= "baitan_ask"; 			--请求摆摊
local STATE_BAITAN_WAITING		= "baitan_waiting";			--等待服务器确认
local STATE_BAITAN_CONFIRM_SUC	= "baitan_confirm_suc";		--摆摊确认成功
local STATE_BAITAN_CONFIRM_ERR	= "baitan_confirm_err";		--摆摊确认失败
local STATE_BAITAN_WORKING		= "baitan_working";			--摆摊中...

local STATE_CURRENT;

local g_PathNodeIndex=0;		--当前路径节点

--设置状态
function Procedure_Main_SetState(stateName)
	STATE_CURRENT = stateName;	
end

--------------------------------------
-- 游戏主流程
--------------------------------------
function Procedure_Main_Init()
	Loger:AddLog("====进入Main流程====");
	
end

function Procedure_Main_Tick()
	Util:Sleep( 1000 );
	if(STATE_CURRENT == STATE_REQ_SCENE) then
		--请求环境
		Util:Sleep(10);
		NetManager:SendMessage("CGEnvRequest");
		Procedure_Main_SetState(STATE_ASK_BAITAN );

	elseif(STATE_CURRENT == STATE_ASK_BAITAN) then
		Util:Sleep(10);
		NetManager:SendMessage("CGStallApply");
		--等待服务器确认
		Procedure_Main_SetState(STATE_BAITAN_WAITING );

	elseif(STATE_CURRENT == STATE_BAITAN_WAITING) then

	elseif(STATE_CURRENT == STATE_BAITAN_CONFIRM_SUC) then
		--发送确认消息
		NetManager:SendMessage("CGStallEstablish");
		Procedure_Main_SetState(STATE_BAITAN_WORKING );

	elseif(STATE_CURRENT == STATE_BAITAN_CONFIRM_ERR) then
		Util:ThrowException("申请摆摊失败");
		
	elseif(STATE_CURRENT == STATE_BAITAN_WORKING) then
		
	end
	
end

function Procedure_Main_Release()

end

--------------------------------------------------
-- 按照路径移动
--------------------------------------------------
function AI_PathMoveNext()
	local AIState = MySelf:GetLogicMode();
	
	--是否处于Idle状态
	if AIState ~= "Idle" then
		return;
	end
	
	--目标点	
	local posNext = Variable:GetVariable("PathNode" .. g_PathNodeIndex);

	local dot;
	dot,_ = string.find(posNext, ",");
	
	MySelf:Goto(tonumber(string.sub(posNext, 1, dot-1)), tonumber(string.sub(posNext, dot+1, string.len(posNext))));
	
	--下一个节点
	g_PathNodeIndex = g_PathNodeIndex+1;
	local strNodeNum = Variable:GetVariable("PathNodeNum");
	if(g_PathNodeIndex >= tonumber(strNodeNum)) then
		g_PathNodeIndex = 0;
	end
	
end