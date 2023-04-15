//PREPROCESSING START
//PREPROCESSING END

//LIBRARY START
#ifdef _DEBUG
#else
#endif
//LIBRARY END

//INCLUDE START
//	SYSTEM HEADER

//	USER HEADER
#include "E:\블로그\lib\RPC_preBuilder\RPC_preBuilder\RPC_server_implement_chatting.h"

//INCLUDE END

extern RPCclient* rpcClient;
extern RPCserver* rpcServer;


bool joinRoom_implement(unsigned long long int sessionID, unsigned int roomID)
{
	//input logic

}

bool leaveRoom_implement(unsigned long long int sessionID, unsigned int roomID)
{
	//input logic
	// 삭제되면 안됨
}

bool sendChat_implement(unsigned long long int sessionID, unsigned int roomID, std::string msg)
{
	//input logic

}

