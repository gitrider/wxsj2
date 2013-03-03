CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (ShareMemory)

ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ADD_DEFINITIONS(-D__SHAREMEMORY__)
ADD_DEFINITIONS(-D_USE_32BIT_TIME_T)

IF(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__LINUX__)
  ADD_DEFINITIONS(-D_REENTRANT)
  ADD_DEFINITIONS(-DDONT_TD_VOID)
ELSE(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__WINDOWS__)
ENDIF(CMAKE_SYSTEM MATCHES Linux)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_BIN})

##update_begin


INCLUDE_DIRECTORIES(..\\..\\Server\\Server\\Other)
INCLUDE_DIRECTORIES(DB)
INCLUDE_DIRECTORIES(..\\..\\Server\\Server\\SMU)
INCLUDE_DIRECTORIES(..\\..\\Common)
INCLUDE_DIRECTORIES(..\\..\\Server\\Server\\Base)
INCLUDE_DIRECTORIES(Main)
INCLUDE_DIRECTORIES(ShareData)
INCLUDE_DIRECTORIES(..\\..\\Common\\DBSystem\\DataBase)
INCLUDE_DIRECTORIES(..\\..\\Common\\DataBase)


SET (BASE_LIST
	../../Server/Server/Base/Config.cpp
	../../Server/Server/Base/Config.h
	../../Server/Server/Base/Ini.cpp
	../../Server/Server/Base/Ini.h
	../../Server/Server/Base/Log.cpp
	../../Server/Server/Base/Log.h
	../../Server/Server/Base/LogDefine.cpp
	../../Server/Server/Base/LogDefine.h
	../../Server/Server/Base/Thread.cpp
	../../Server/Server/Base/Thread.h
	../../Server/Server/Base/TimeManager.cpp
	../../Server/Server/Base/TimeManager.h
	../../Common/DataBase/WXSJ_DBC.cpp
	../../Common/DataBase/WXSJ_DBC.h
)

SET (MAIN_LIST
	Main/CommandThread.cpp
	Main/CommandThread.h
	Main/ShareMemory.cpp
	Main/ShareMemory.h
)

SET (COMMON_DATABASE_LIST
	../../Common/DBSystem/DataBase/DBAbilityList.cpp
	../../Common/DBSystem/DataBase/DBAbilityList.h
	../../Common/DBSystem/DataBase/DBChardbVersion.cpp
	../../Common/DBSystem/DataBase/DBChardbVersion.h
	../../Common/DBSystem/DataBase/DBCharFullData.cpp
	../../Common/DBSystem/DataBase/DBCharFullData.h
	../../Common/DBSystem/DataBase/DBCoolDownInfo.cpp
	../../Common/DBSystem/DataBase/DBCoolDownInfo.h
	../../Common/DBSystem/DataBase/DBCreateCharOp.cpp
	../../Common/DBSystem/DataBase/DBCreateCharOp.h
	../../Common/DBSystem/DataBase/DBDeleteCharOp.cpp
	../../Common/DBSystem/DataBase/DBDeleteCharOp.h
	../../Common/DBSystem/DataBase/DBEquipBaseList.cpp
	../../Common/DBSystem/DataBase/DBEquipBaseList.h
	../../Common/DBSystem/DataBase/DBGuildInfo.cpp
	../../Common/DBSystem/DataBase/DBGuildInfo.h
	../../Common/DBSystem/DataBase/DBImpactList.cpp
	../../Common/DBSystem/DataBase/DBImpactList.h
	../../Common/DBSystem/DataBase/DBItemList.cpp
	../../Common/DBSystem/DataBase/DBItemList.h
	../../Common/DBSystem/DataBase/DBItemSerial.cpp
	../../Common/DBSystem/DataBase/DBItemSerial.h
	../../Common/DBSystem/DataBase/DBMailInfo.cpp
	../../Common/DBSystem/DataBase/DBMailInfo.h
	../../Common/DBSystem/DataBase/DBManager.cpp
	../../Common/DBSystem/DataBase/DBManager.h
	../../Common/DBSystem/DataBase/DBPetList.cpp
	../../Common/DBSystem/DataBase/DBPetList.h
	../../Common/DBSystem/DataBase/DBPrivateInfo.cpp
	../../Common/DBSystem/DataBase/DBPrivateInfo.h
	../../Common/DBSystem/DataBase/DBRelationList.cpp
	../../Common/DBSystem/DataBase/DBRelationList.h
	../../Common/DBSystem/DataBase/DBSkillList.cpp
	../../Common/DBSystem/DataBase/DBSkillList.h
	../../Common/DBSystem/DataBase/DBSouXiaList.cpp
	../../Common/DBSystem/DataBase/DBSouXiaList.h
	../../Common/DBSystem/DataBase/DBTaskList.cpp
	../../Common/DBSystem/DataBase/DBTaskList.h
	../../Common/DBSystem/DataBase/DBTitleInfo.cpp
	../../Common/DBSystem/DataBase/DBTitleInfo.h
	../../Common/DBSystem/DataBase/DBXinFaList.cpp
	../../Common/DBSystem/DataBase/DBXinFaList.h
	../../Common/DBSystem/DataBase/ODBCBase.cpp
	../../Common/DBSystem/DataBase/ODBCBase.h
	../../Common/DBSystem/DataBase/ODBCInterface.cpp
	../../Common/DBSystem/DataBase/ODBCInterface.h
	../../Common/DBSystem/DataBase/SqlTemplate.cpp
	../../Common/DBSystem/DataBase/SqlTemplate.h
)

SET (COMMON_LIST
	../../Common/Assertx.cpp
	../../Common/Assertx.h
	../../Common/BaseType.h
	../../Common/DB_Struct.cpp
	../../Common/DB_Struct.h
	../../Common/DBSystem/DataBase/DBTypes.h
	../../Common/FileAPI.h
	../../Common/FoxEncrypt.cpp
	../../Common/FoxEncrypt.h
	../../Common/GameDefine.h
	../../Common/GameDefine2.h
	../../Common/GameDefine_Attr.h
	../../Common/GameDefine_Item.h
	../../Common/GameDefine_Result.h
	../../Common/GameDefine_Scene.h
	../../Common/GameStruct.cpp
	../../Common/GameStruct.h
	../../Common/GameStruct_Impact.h
	../../Common/GameStruct_Item.h
	../../Common/GameStruct_Pet.cpp
	../../Common/GameStruct_Pet.h
	../../Common/GameStruct_Scene.h
	../../Common/GameStruct_Skill.h
	../../Common/GameUtil.cpp
	../../Common/GameUtil.h
	../../Common/Minidump.cpp
	../../Common/Minidump.h
	../../Common/Rand.cpp
	../../Common/Rand.h
	../../Common/ServerSocket.h
	../../Common/ShareMemAPI.cpp
	../../Common/ShareMemAPI.h
	../../Common/Socket.h
	../../Common/SocketAPI.h
	../../Common/SocketInputStream.h
	../../Common/SocketOutputStream.h
	../../Common/Type.h
)

SET (SHAREDATA_LIST
	../../Server/Server/SMU/ShareMemAO.cpp
	../../Server/Server/SMU/ShareMemAO.h
	ShareData/SMULogicManager.cpp
	ShareData/SMULogicManager.h
	../../Server/Server/SMU/SMUManager.cpp
	../../Server/Server/SMU/SMUManager.h
)

SET (DATABASE_LIST
	DB/ShareDBManager.cpp
	DB/ShareDBManager.h
)

SET (OTHER_LIST
	../../Server/Server/Other/BBS.cpp
	../../Server/Server/Other/BBS.h
	../../Server/Server/Other/PlayerShopDefine.h
)

SET (STDAFX_LIST
	stdafx.cpp
	stdafx.h
)



if (WIN32)
	source_group(Base FILES ${BASE_LIST})
	source_group(Main FILES ${MAIN_LIST})
	source_group(Common\\DataBase FILES ${COMMON_DATABASE_LIST})
	source_group(Common FILES ${COMMON_LIST})
	source_group(ShareData FILES ${SHAREDATA_LIST})
	source_group(DataBase FILES ${DATABASE_LIST})
	source_group(Other FILES ${OTHER_LIST})
	source_group(StdAfx FILES ${STDAFX_LIST})
endif()


ADD_EXECUTABLE(ShareMemory
	${BASE_LIST}
	${MAIN_LIST}
	${COMMON_DATABASE_LIST}
	${COMMON_LIST}
	${SHAREDATA_LIST}
	${DATABASE_LIST}
	${OTHER_LIST}
	${STDAFX_LIST}
)


##update_end

if (WIN32)
TARGET_LINK_LIBRARIES(ShareMemory FoxNet ws2_32.lib odbc32.lib odbccp32.lib)
else()
TARGET_LINK_LIBRARIES(ShareMemory FoxNet pthread odbc)
endif()
