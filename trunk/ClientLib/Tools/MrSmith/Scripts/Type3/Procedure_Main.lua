--------------------------------------
-- Main����
--------------------------------------
local STATE_REQ_SCENE			= "request_scene";			--���󳡾�
local STATE_PATH_MOVE			= "path_move"; 				--����һ��·������

local STATE_CURRENT;

local g_PathNodeIndex=0;		--��ǰ·���ڵ�

--����״̬
function Procedure_Main_SetState(stateName)
	STATE_CURRENT = stateName;	
end

--------------------------------------
-- ��Ϸ������
--------------------------------------
function Procedure_Main_Init()
	Loger:AddLog("====����Main����====");
	
end

function Procedure_Main_Tick()
	Util:Sleep( 1000 );
	if(STATE_CURRENT == STATE_REQ_SCENE) then
		--���󻷾�
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
-- ����·���ƶ�
--------------------------------------------------
function AI_PathMoveNext()
	local AIState = MySelf:GetLogicMode();
	
	--�Ƿ���Idle״̬
	if AIState ~= "Idle" then
		return;
	end
	
	--Ŀ���	
	local posNext = Variable:GetVariable("PathNode" .. g_PathNodeIndex);

	local dot;
	dot,_ = string.find(posNext, ",");
	
	MySelf:Goto(tonumber(string.sub(posNext, 1, dot-1)), tonumber(string.sub(posNext, dot+1, string.len(posNext))));
	
	--��һ���ڵ�
	g_PathNodeIndex = g_PathNodeIndex+1;
	local strNodeNum = Variable:GetVariable("PathNodeNum");
	if(g_PathNodeIndex >= tonumber(strNodeNum)) then
		g_PathNodeIndex = 0;
	end
	
end