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
#include "E:\ºí·Î±×\lib\RPC_preBuilder\RPC_preBuilder\RPC_client_callback.h"

//INCLUDE END

extern RPCclient* rpcClient;


void sendChat(std::string msg)
{
	packet sendBuffer;

	*(sendBuffer.buffer) << (short)0 << msg;
	sendBuffer.fillHeader(packetHeader(sendBuffer.buffer->size()));

	rpcClient->SendPacket(sendBuffer);
}


void add(int num1, int num2)
{
	packet sendBuffer;

	*(sendBuffer.buffer) << (short)1 << num1 << num2;
	sendBuffer.fillHeader(packetHeader(sendBuffer.buffer->size()));

	rpcClient->SendPacket(sendBuffer);
}


void RPC_callback_table(serializer& s)
{
	short id;
	s >> id;

	switch(id)
	{
		case 0 :
		{
			std::string ret;
			s >> ret;
			sendChat_callback(ret);
		}
		break;

		case 1 :
		{
			int ret;
			s >> ret;
			int returnValue;
			add_callback(ret, returnValue);
		}
		break;

	}
}

