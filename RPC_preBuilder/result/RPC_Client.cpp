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

#include <WS2tcpip.h>
#include <map>
#include "IOCP/IOCP/network.h"
#include "IOCP/IOCP/session.h"

#include "RPC_Client.h"

RPCclient::RPCclient()
{
	return;
}

RPCclient::~RPCclient()
{
	return;
}

bool RPCclient::start(const std::map<std::string, std::pair<std::string, USHORT>>& serverList)
{
	return true;
}

void RPCclient::stop()
{
	return;
}

DWORD WINAPI RPCclient::workerThread(LPVOID arg)
{
	return 0;
}

void RPCclient::OnNewConnect(UINT64 sessionID)
{
	return;
}

void RPCclient::OnDisconnect(UINT64 sessionID)
{
	return;
}

bool RPCclient::OnConnectionRequest(ULONG ip, USHORT port)
{
	return true;
}

void RPCclient::OnRecv(UINT64 sessionID, packet & _packet)
{
	return;
}


void RPCclient::OnSend(UINT64 sessionID, int sendsize)
{
	return;
}


void RPCclient::OnError(int code, const char* msg)
{
	return;
}

