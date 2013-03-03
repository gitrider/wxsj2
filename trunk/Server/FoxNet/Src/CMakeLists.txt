INCLUDE_DIRECTORIES(Encoder)

SET(LIBRARY_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_LIB})

SET(SRC_LIST dhkey.cpp 
             EncoderManager.cpp 
             FileAPI.cpp 
             Packet.cpp 
             ServerSocket.cpp 
             Socket.cpp
             SocketAPI.cpp
             SocketBillingInputStream.cpp
             SocketBillingOutputStream.cpp
             SocketInputStream.cpp
             SocketOutputStream.cpp
             BaseType.h
             dhkey.h
             Encoder.h
             EncoderManager.h
             FileAPI.h
             InstanceManager.h
             Packet.h
             ServerSocket.h
             Socket.h
             SocketAPI.h
             SocketBillingInputStream.h
             SocketBillingOutputStream.h
             SocketInputStream.h
             SocketOutputStream.h)
             
SET(ENCODER_LIST 
             Encoder/Encoder_XOR.h
             Encoder/Encoder_XOR.cpp)

if (WIN32)             
source_group(Encoder FILES ${ENCODER_LIST})
endif()

ADD_LIBRARY(FoxNet STATIC ${SRC_LIST} ${ENCODER_LIST})

IF(CMAKE_SYSTEM MATCHES Windows)
  TARGET_LINK_LIBRARIES(FoxNet ws2_32)
ENDIF(CMAKE_SYSTEM MATCHES Windows)


