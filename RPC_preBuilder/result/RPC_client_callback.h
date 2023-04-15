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
#include "E:\블로그\lib\RPC_preBuilder\RPC_preBuilder\RPC_client_callback_chatting.h"
#include "E:\블로그\lib\RPC_preBuilder\RPC_preBuilder\RPC_client_callback_default.h"
#include "RemoteProcedureCall/RemoteProcedureCall/client.h"

//INCLUDE END



void joinRoom(unsigned int roomID);
void leaveRoom(unsigned int roomID);
void sendChat(unsigned int roomID, std::string msg);
void RPC_callback_table(serializer& s);
