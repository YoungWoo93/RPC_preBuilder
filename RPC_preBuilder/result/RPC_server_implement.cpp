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
#include "E:\ºí·Î±×\lib\RPC_preBuilder\RPC_preBuilder\RPC_server_implement.h"

//INCLUDE END

extern RPCclient* rpcClient;
extern RPCserver* rpcServer;


void RPC_implement_table(UINT64 sessionID, serializer& s, serializer& outputs)
{
	short id;
	s >> id;

	switch(id)
	{
		case 0 :
		{
			bool ret;
			unsigned int roomID;
			s >> roomID;


			ret = joinRoom_implement(sessionID, roomID);

			outputs << (short)0 << ret;

		}
		break;

		case 1 :
		{
			bool ret;
			unsigned int roomID;
			s >> roomID;


			ret = leaveRoom_implement(sessionID, roomID);

			outputs << (short)1 << ret;

		}
		break;

		case 2 :
		{
			bool ret;
			unsigned int roomID;
			std::string msg;
			s >> roomID >> msg;


			ret = sendChat_implement(sessionID, roomID, msg);

			outputs << (short)2 << ret;

		}
		break;

	}
}

