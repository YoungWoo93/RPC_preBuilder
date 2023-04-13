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
			std::string ret;
			std::string msg;
			s >> msg;


			ret = sendChat_implement(sessionID, msg);

			outputs << (short)0 << ret;

		}
		break;

		case 1 :
		{
			int ret;
			int num1;
			int num2;
			s >> num1 >> num2;

			int returnValue;

			ret = add_implement(sessionID, num1, num2, returnValue);

			outputs << (short)1 << ret << returnValue;

		}
		break;

	}
}

