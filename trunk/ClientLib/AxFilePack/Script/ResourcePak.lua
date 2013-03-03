

local Media_Path_table = 
{
	"../Media/",
	"../Media/overlays",
	"../Media/EffectSystem",
	"../Media/ProjTex",
	"../Media/PostFilter",
	"../Media/Water",
	"../Media/Skybox",
	"*../Media/ģ��",
	"../Media/����/ˮ��",
	"../Media/����/������",
	"../Media/����/֩���",
	"../Media/����/�ټ���",
	"../Media/����/������",
	"../Media/����/���Ż���",
	"../Media/����/����",
	"../Media/����/�׵�����",
	"../Media/����/ʯʨ��",
	"../Media/����/Ů����",
	"../Media/����/������",
	"../Media/����/Ů����װ��",
	"../Media/����/����",
	"../Media/����/����",
	"../Media/����/Npc",
	"../Media/����/���Ʊ�ٸ",
	"../Media/����/������",
	"../Media/����/�ؼ�կ̽��",
	"../Media/����/ɽ��",
	"../Media/����/�����Գ",
	"../Media/����/��",
	"../Media/����/ç��",
	"../Media/����/���",
	"../Media/����/��",
	"../Media/����/ʯ��",
	"../Media/����/а����",
	"../Media/����/��",
	"../Media/����/ҰԳ",
	"../Media/����/��",
	"../Media/����/����",
	"../Media/����/�����",
	"../Media/����/����",
	"../Media/����/������",
	"../Media/����/ľ��",
	"../Media/����/ͭ��",
	"../Media/����/����",
	"../Media/����/ѩ��",
	"../Media/����/ҩ¨",
	"../Media/����/ɽ��",
	"../Media/����/���",
	"../Media/����/�ڷ�",
	"../Media/����/����",
	"../Media/����/���",
};

local Sound_path_table = 
{	
	"*../Sound",
};

local Interface_path_table =
{
	"*../Interface",
};

local PublicLua_path_table = 
{
	"*../Config",
	"*../../Public/Config",
	"*../../Public/Data/Script",
	"../../Public/Data",
};

local Brushes_group_table = 
{
	"*../Brushes"
};

local Scene_path_table =
{
	"../../Public/Scene",
	"../../Public/Scene/Schema"
};

function Main()

	print( "");
	print( "1:Model" );
	print( "2:Material" );
	print( "3:Effect" );
	print( "4:Interface" );
	print( "5:Sound" );
	print( "6:Scene" );
	print( "7:Brushes" );
	print( "8:Config" );
	print( "a:All" );
	print( "");
	io.write( "������Ҫ����ı��:");
	local sel = io.read();
	sel = "a";
	
	local groupSel = {0, 0, 0, 0, 0, 0, 0, 0};
	
	if(sel == "a") then
		groupSel[1] = 1;
		groupSel[2] = 1;
		groupSel[3] = 1;
		groupSel[4] = 1;
		groupSel[5] = 1;
		groupSel[6] = 1;
		groupSel[7] = 1;
		groupSel[8] = 1;
	elseif(tonumber(sel)~=nil and (tonumber(sel) >= 1 and tonumber(sel) <= 8)) then
		groupSel[tonumber(sel)] = 1;
	end

	-----------------------
	--		Model		---
	-----------------------
	if(	groupSel[1] == 1) then
		PakMaker:moveSkeleton("../Media/ģ��");
		pakPath("Model", {Media_Path_table}, "mesh;model;skeleton;obj;frame", "stand");
	end
	
	-----------------------
	--		Material	---
	-----------------------
	if(	groupSel[2] == 1) then
		pakPath("Material", {Media_Path_table, Interface_path_table}, 
			"material;cg;asm;program;overlay;bmp;dib;cut;dcx;dds;gif;hdr;ico;cur;jpg;jpe;jpeg;lif;mdl;mng;jng;pcx;pic;pix;png;pbm;pgm;pnm;ppm;psd;pdd;psp;pxr;sgi;bw;rgb;rgba;tga;vda;icb;vst;tif;tiff;wal;xpm;raw", 
			"stand");
	end
	
	-----------------------
	--		Effect		---
	-----------------------
	if(	groupSel[3] == 1) then
		pakPath("Effect", {Media_Path_table}, "particle;effect;skill", "stand");
	end
	
	-----------------------
	--	  Interface		---
	-----------------------
	if(	groupSel[4] == 1) then
		pakPath("Interface", {Interface_path_table}, "xml;lua;ttf;font;shema;bin;imageset", "stand");
	end
	
	-----------------------
	--	   Sound		---
	-----------------------
	if(	groupSel[5] == 1) then
		pakPath("Sound", {Sound_path_table}, "wav;mp2;mp3;ogg;raw;mod;s3m;xm;it;mid", "stand");
	end
	
	-----------------------
	--	    Scene		---
	-----------------------
	if(	groupSel[6] == 1) then
		pakPath("Scene", {Scene_path_table}, 
			"jpg;dds;esd;gridinfo;heightmap;png;region;scene;preload;terrain;wcollision;xsd;xml;material;cg;program", "stand");
	end
	
	-----------------------
	--	    Brushes		---
	-----------------------
	if(	groupSel[7] == 1) then
		pakPath("Brushes", {Brushes_group_table}, 
			"bmp;dib;cut;dcx;dds;gif;hdr;ico;cur;jpg;jpe;jpeg;lif;mdl;mng;jng;pcx;pic;pix;png;pbm;pgm;pnm;ppm;psd;pdd;psp;pxr;sgi;bw;rgb;rgba;tga;vda;icb;vst;tif;tiff;wal;xpm;raw"
			, "stand");
	end
	
	-----------------------
	--	    Config		---
	-----------------------
	if(	groupSel[8] == 1) then
		pakPath("Config", {PublicLua_path_table}, "dat;txt;lua", "config");
	end
	
end

---------------------------------------------------
--pakNormal path
function pakPath(pakName, pathTable, extFilter, callBack)
	PakMaker:clear();

	local tblPathGroup;
	for _, tblPathGroup in ipairs(pathTable) do
		local strPath="";
		for _, strPath in ipairs(tblPathGroup) do
			local bRecursive = string.sub(strPath, 1, 1) == "*";
			if bRecursive then strPath = string.sub(strPath, 2, -1) end
			
			if( not PakMaker:addDiskFold(strPath, "", extFilter, bRecursive)) then
				OnError();
				return;
			end
		end
	end
		
	if( not PakMaker:savePakFile(pakName .. ".axp", callBack)) then
		OnError();
		return;
	end
end

function OnError()
	local errId, errDesc = getLastError();
	local strErr = "\nErr: " .. tostring(errId) .. "\nErrDesc: " .. errDesc;

	error(strErr);
end