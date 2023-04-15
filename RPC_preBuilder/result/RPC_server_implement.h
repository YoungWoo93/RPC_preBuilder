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
#include "E:\블로그\lib\RPC_preBuilder\RPC_preBuilder\RPC_server_implement_chatting.h"
#include "E:\블로그\lib\RPC_preBuilder\RPC_preBuilder\RPC_server_implement_default.h"

//INCLUDE END

extern RPCclient* rpcClient;
extern RPCserver* rpcServer;


void RPC_implement_table(UINT64 sessionID, serializer& s, serializer& outputs);
