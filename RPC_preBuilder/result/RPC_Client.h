#include "IOCP/IOCP/network.h"
#include "IOCP/IOCP/session.h"
#include <map>
class RPCclient : public Network {
public:
RPCclient();
	~RPCclient();

	bool start(const std::map<std::string, std::pair<std::string, USHORT>>&serverList);
	void stop();
	static DWORD WINAPI workerThread(LPVOID arg);
	virtual void OnNewConnect(UINT64 sessionID);

	virtual void OnDisconnect(UINT64 sessionID);
	virtual bool OnConnectionRequest(ULONG ip, USHORT port);
	virtual void OnRecv(UINT64 sessionID, packet& _packet);

	virtual void OnSend(UINT64 sessionID, int sendsize);

	virtual void OnError(int code, const char* msg = "");


public:
	std::map<std::string, UINT64> infoIDMap; 
	std::map<UINT64, std::string> IDInfoMap; 
};

