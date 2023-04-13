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

extern RPCclient* rpcClient;
extern RPCserver* rpcServer;


int add_implement(UINT64 sessionID, /*IN*/int num1, int num2, /*OUT*/int& returnValue);
