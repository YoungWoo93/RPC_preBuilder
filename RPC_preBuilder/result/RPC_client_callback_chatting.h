//PREPROCESSING START
#pragma once
#define _WINSOCKAPI_
//PREPROCESSING END

//LIBRARY START
#ifdef _DEBUG
#pragma comment(lib, "SerializerD")
#pragma comment(lib, "RemoteProcedureCallD")
#else
#pragma comment(lib, "Serializer")
#pragma comment(lib, "RemoteProcedureCall")
#endif
//LIBRARY END

//INCLUDE START
//	SYSTEM HEADER
#include <string>

//	USER HEADER
#include "Serializer/Serializer/Serializer.h"
#include "RemoteProcedureCall/RemoteProcedureCall/RemoteProcedureCall.h"

//INCLUDE END



void joinRoom_callback(bool ret);
void leaveRoom_callback(bool ret);
void sendChat_callback(bool ret);
