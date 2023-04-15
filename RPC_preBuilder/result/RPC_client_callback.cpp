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


void joinRoom(unsigned int roomID)
{
	packet sendBuffer;

	*(sendBuffer.buffer) << (short)0 << roomID;
	sendBuffer.fillHeader(packetHeader(sendBuffer.buffer->size()));

	rpcClient->SendPacket(sendBuffer);
}


void leaveRoom(unsigned int roomID)
{
	packet sendBuffer;

	*(sendBuffer.buffer) << (short)1 << roomID;
	sendBuffer.fillHeader(packetHeader(sendBuffer.buffer->size()));

	rpcClient->SendPacket(sendBuffer);
}


void sendChat(unsigned int roomID, std::string msg)
{
	packet sendBuffer;

	*(sendBuffer.buffer) << (short)2 << roomID << msg;
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
			bool ret;
			s >> ret;
			joinRoom_callback(ret);
		}
		break;

		case 1 :
		{
			bool ret;
			s >> ret;
			leaveRoom_callback(ret);
		}
		break;

		case 2 :
		{
			bool ret;
			s >> ret;
			sendChat_callback(ret);
		}
		break;

	}
}

