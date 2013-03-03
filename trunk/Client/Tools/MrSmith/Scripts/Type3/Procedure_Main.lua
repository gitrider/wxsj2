--------------------------------------
-- Main流程
--------------------------------------
local STATE_REQ_SCENE			= "request_scene";			--请求场景
local STATE_PATH_MOVE			= "path_move"; 				--按照一定路径行走

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
		Util:Sleep( 10 );
		NetManager:SendMessage("CGEnvRequest");
		Procedure_Main_SetState(STATE_PATH_MOVE );

	elseif(STATE_CURRENT == STATE_PATH_MOVE) then
		AI_PathMoveNext();
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