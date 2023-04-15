#ifdef _DEBUG
#pragma comment(lib, "RingBufferD")
#pragma comment(lib, "MemoryPoolD")
#pragma comment(lib, "SerializerD")
#pragma comment(lib, "MessageLoggerD")
#pragma comment(lib, "crashDumpD")
#pragma comment(lib, "IOCPD")

#else
#pragma comment(lib, "RingBuffer")
#pragma comment(lib, "MemoryPool")
#pragma comment(lib, "Serializer")
#pragma comment(lib, "MessageLogger")
#pragma comment(lib, "crashDump")
#pragma comment(lib, "IOCP")

#endif

#include "IOCP/IOCP/network.h"
#include "IOCP/IOCP/session.h"

#include "RPC_Server.h"

void RPCserver::OnNewConnect(UINT64 sessionID)
{
	return;
}

void RPCserver::OnDisconnect(UINT64 sessionID)
{
	return;
}

bool RPCserver::OnConnectionRequest(ULONG ip, USHORT port)
{
	return true;
}

void RPCserver::OnRecv(UINT64 sessionID, packet & _packet)
{
	return;
}


void RPCserver::OnSend(UINT64 sessionID, int sendsize)
{
	return;
}


void RPCserver::OnError(int code, const char* msg)
{
	return;
}

