require('LuaXml')
local Cmake_result = {}
local path_result = {}

function Split(szFullString, szSeparator)
	local nFindStartIndex = 1
	local nSplitIndex = 1
	local nSplitArray = {}
	while true do
	   local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)
	   if not nFindLastIndex then
			nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))
			break
	   end
	   nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)
	   nFindStartIndex = nFindLastIndex + string.len(szSeparator)
	   nSplitIndex = nSplitIndex + 1
	end
	return nSplitArray
end

function addFilter(ParentName, Filter)
  ParentName = string.gsub(ParentName, " ", "")
	local FilterListName = ParentName .. "_LIST"
	local Paths = Split(ParentName, "_")

	local sourcegroupline = ""
	for i = 1, table.getn(Paths) - 1 do
		-- print(Files[i])
		sourcegroupline = sourcegroupline .. Paths[i] .. "\\\\"
	end

	sourcegroupline = sourcegroupline .. Paths[table.getn(Paths)]

	FilterListName = string.upper(FilterListName)
	local FilterLine = "SET (" .. FilterListName .. "\n"
	local FileLines = ""
	if type(Filter) == "table" then
		--print(Filter:tag() .. " " .. " " .. type(Filter) .. " " .. tostring(table.getn(Filter)))
		for i = 1, table.getn(Filter) do
			if Filter[i]:tag() == "Filter" then
				addFilter(ParentName .. "_" .. Filter[i].Name, Filter[i])
			elseif Filter[i]:tag() == "File" then
				 -- print(Filter[i]:tag() .. " " ..  " " .. type(Filter[i]) .. " " .. tostring(table.getn(Filter[i])))
				FileLines = FileLines .. addFile(Filter[i]) .. "\n"
			end
		end
	end
	FilterLine = FilterLine .. FileLines .. ")\n"
	--print(FilterListName)
	--print(FilterLine)

	local tablesize = table.getn(Cmake_result)
	Cmake_result[tablesize + 1] = {}
	Cmake_result[tablesize + 1].FilterListName = FilterListName
	Cmake_result[tablesize + 1].FilterLine = FilterLine
	Cmake_result[tablesize + 1].sourcegroupline = sourcegroupline

	return FilterListName, FilterLine
end

function addFile(File)
	local FileLine = ""
	if type(File) ~= "table" then return FileLine end
	if string.find(File.RelativePath,"CMakeLists.txt") ~= nil then return FileLine end
	if string.sub(File.RelativePath, 1, 2) == ".\\" then
		File.RelativePath = string.sub(File.RelativePath, 3)
	end
	File.RelativePath = string.gsub(File.RelativePath, " ", "")
	local Files = Split(File.RelativePath, "\\")
	-- print(File.RelativePath)
	for i = 1, table.getn(Files) - 1 do
		-- print(Files[i])
		FileLine = FileLine .. Files[i] .. "/"
	end

	FileLine = "\t" .. FileLine

	local FilePath = ""

	for i = 1, table.getn(Files) - 2 do
		-- print(Files[i])
		FilePath  = FilePath .. Files[i] .. "\\\\"
	end

	if(table.getn(Files) > 1) then
		FilePath  = FilePath .. Files[table.getn(Files) - 1]
	end

	path_result[FilePath] = FilePath

	FileLine = FileLine .. Files[table.getn(Files)]
	--print(FileLine)
	return FileLine
end

function fill_cmake(vcprojname)

	local xfile = xml.load(vcprojname)
	local Files = xfile:find("Files")
	
	--print("**************************")
	--print(Files)
	--print("**************************")

	local FilterListName = vcprojname .. "_LIST"
	FilterListName = string.upper(FilterListName)
	local FilterLine = "SET (" .. FilterListName .. "\n"
	local FileLines = ""

	if Files ~= nil then
		if type(Files) == "table" then
			-- print(table.getn(Files))
			for i = 1, table.getn(Files) do
				-- print(Files[i]:tag())
				if Files[i]:tag() == "Filter" then
					addFilter(Files[i].Name, Files[i])
				elseif Files[i]:tag() == "File" then
					FileLines = FileLines .. addFile(Files[i]).. "\n"
				end
			end
			FilterLine = FilterLine .. FileLines .. ")\n"
			if string.len(FileLines) > 0 then
				local tablesize = table.getn(Cmake_result)
				Cmake_result[tablesize + 1] = {}
				Cmake_result[tablesize + 1].FilterListName = FilterListName
				Cmake_result[tablesize + 1].FilterLine = FilterLine
				Cmake_result[tablesize + 1].sourcegroupline = ""
			end
		end
	end
end

function get_vcproj_cmake(vcprojname)

	local cmake_text = ""
	
	Cmake_result = {}
  path_result = {}

	fill_cmake(vcprojname)

	-- print(path_result["EventModule"])
	-- print(type(path_result))


	for key, value in pairs(path_result) do
		if(string.len(value) > 0) then
			cmake_text = cmake_text .. "INCLUDE_DIRECTORIES(" ..value .. ")\n"
		end
	end


	cmake_text = cmake_text .. "\n\n"

	for i = 1, table.getn(Cmake_result) do
		if(string.len(Cmake_result[i].FilterLine) > 0) then
			cmake_text = cmake_text .. Cmake_result[i].FilterLine .. "\n"
		end
		-- print(Cmake_result[i].FilterListName)
		-- print(Cmake_result[i].FilterLine)
		-- print(Cmake_result[i].PathLine)
	end

	cmake_text = cmake_text .. "\n\n"

	cmake_text = cmake_text .. "if (WIN32)\n"

	for i = 1, table.getn(Cmake_result) do
		if(string.len(Cmake_result[i].FilterLine) > 0) then
			cmake_text = cmake_text .. "\tsource_group(" .. Cmake_result[i].sourcegroupline .. " FILES ${" .. Cmake_result[i].FilterListName .. "})\n"
		end
	end

	cmake_text = cmake_text .. "endif()\n"

	cmake_text = cmake_text .. "\n\n"
	
	local ExeFiles = Split(vcprojname, "/")
	
	-- print(ExeFiles[table.getn(ExeFiles)])
	
	local PName = string.sub(ExeFiles[table.getn(ExeFiles)], 1, string.len(ExeFiles[table.getn(ExeFiles)]) - 7)
	
	cmake_text = cmake_text .. "ADD_EXECUTABLE(" .. PName .. "\n"

	for i = 1, table.getn(Cmake_result) do
		if(string.len(Cmake_result[i].FilterLine) > 0) then
			cmake_text = cmake_text .. "\t${" .. Cmake_result[i].FilterListName .. "}\n"
		end
	end

	cmake_text = cmake_text .. ")"

	return cmake_text
end

--print (arg[1])
--print(get_vcproj_cmake("Billing"))
