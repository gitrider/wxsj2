local g_newVersionPath  = "D:\\Test\\New";
local g_newVersion		= ""

local g_oldVersionPath	= "D:\\Test\\Old";
local g_oldVersion 		= ""


function Main()
	PatchMaker:clear();
	local temp;
	
	-----------------�����°汾Ŀ¼
	io.write("�����°汾Ŀ¼[" .. g_newVersionPath .. "]:");
	temp = io.read(); 
	if(temp ~= "") then
		g_newVersionPath = temp;
	end

	------------------Ԥ����
	if(PatchMaker:analysePath(g_newVersionPath, "")) then
		g_newVersion = PatchMaker:getPathVersion(g_newVersionPath);
	else
		g_newVersion = "";
	end

	------------------�����°汾��
	io.write("�����°汾��[" .. g_newVersion .. "]:");
	temp = io.read();
	if(temp ~= "" and temp ~= g_newVersion) then
		g_newVersion = temp;
		if(not PatchMaker:analysePath(g_newVersionPath, g_newVersion)) then
			OnError();
			return;
		end
	end

	-----------------����ɰ汾Ŀ¼
	io.write("����ɰ汾Ŀ¼[" .. g_oldVersionPath .. "]:");
	temp = io.read(); 
	if(temp ~= "") then
		g_oldVersionPath = temp;
	end

	------------------Ԥ����
	if(PatchMaker:analysePath(g_oldVersionPath, "")) then
		g_oldVersion = PatchMaker:getPathVersion(g_oldVersionPath);
	else
		g_oldVersion = "";
	end

	------------------����ɰ汾��
	io.write("����ɰ汾��[" .. g_oldVersion .. "]:");
	temp = io.read();
	if(temp ~= "" and temp ~= g_newVersion) then
		g_oldVersion = temp;
		if(not PatchMaker:analysePath(g_oldVersionPath, g_oldVersion)) then
			OnError();
			return;
		end
	end
	
	--���ɲ����ļ�
	if(not PatchMaker:generatePatch(g_oldVersionPath, g_newVersionPath, "d:\\test.zip")) then
		OnError();
		return;
	end
	
	

end


function OnError()
	local errId, errDesc = getLastError();
	local strErr = "\nErr: " .. tostring(errId) .. "\nErrDesc: " .. errDesc;

	error(strErr);
end
