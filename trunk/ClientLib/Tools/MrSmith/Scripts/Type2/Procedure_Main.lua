--------------------------------------
-- Main����
--------------------------------------
local STATE_REQ_SCENE			= "request_scene";			--���󳡾�
local STATE_ASK_BAITAN			= "baitan_ask"; 			--�����̯
local STATE_BAITAN_WAITING		= "baitan_waiting";			--�ȴ�������ȷ��
local STATE_BAITAN_CONFIRM_SUC	= "baitan_confirm_suc";		--��̯ȷ�ϳɹ�
local STATE_BAITAN_CONFIRM_ERR	= "baitan_confirm_err";		--��̯ȷ��ʧ��
local STATE_BAITAN_WORKING		= "baitan_working";			--��̯��...

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
		Util:Sleep(10);
		NetManager:SendMessage("CGEnvRequest");
		Procedure_Main_SetState(STATE_ASK_BAITAN );

	elseif(STATE_CURRENT == STATE_ASK_BAITAN) then
		Util:Sleep(10);
		NetManager:SendMessage("CGStallApply");
		--�ȴ�������ȷ��
		Procedure_Main_SetState(STATE_BAITAN_WAITING );

	elseif(STATE_CURRENT == STATE_BAITAN_WAITING) then

	elseif(STATE_CURRENT == STATE_BAITAN_CONFIRM_SUC) then
		--����ȷ����Ϣ
		NetManager:SendMessage("CGStallEstablish");
		Procedure_Main_SetState(STATE_BAITAN_WORKING );

	elseif(STATE_CURRENT == STATE_BAITAN_CONFIRM_ERR) then
		Util:ThrowException("�����̯ʧ��");
		
	elseif(STATE_CURRENT == STATE_BAITAN_WORKING) then
		
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