#include "cppRPCClientCodeMaker.h"

#include <iostream>

using namespace std;

void cppRPCClientCodeMaker::make()
{
	makeHeader();
	makeCpp();
}

void cppRPCClientCodeMaker::makeHeader()
{
	inFile.open(dir + "RPC_Client.h");
	if (inFile.good()){
		inFile.close();
		return;
	}
	inFile.close();

	outFile.open(dir + "RPC_Client.h");

	outFile << "#include \"IOCP/IOCP/network.h\"" << endl;
	outFile << "#include \"IOCP/IOCP/session.h\"" << endl;
	outFile << "#include <map>" << endl;
	outFile << "class RPCclient : public Network {" << endl;
	outFile << "public:" << endl;
	outFile << "\RPCclient();" << endl;
	outFile << "\t~RPCclient();" << endl;
	outFile << endl;
	outFile << "\tbool start(const std::map<std::string, std::pair<std::string, USHORT>>&serverList);" << endl;
	outFile << "\tvoid stop();" << endl;
	outFile << "\tstatic DWORD WINAPI workerThread(LPVOID arg);" << endl;
	outFile << "\tvirtual void OnNewConnect(UINT64 sessionID);" << endl;
	outFile << endl;
	outFile << "\tvirtual void OnDisconnect(UINT64 sessionID);" << endl;
	outFile << "\tvirtual bool OnConnectionRequest(ULONG ip, USHORT port);" << endl;
	outFile << "\tvirtual void OnRecv(UINT64 sessionID, packet& _packet);" << endl;
	outFile << endl;
	outFile << "\tvirtual void OnSend(UINT64 sessionID, int sendsize);" << endl;
	outFile << endl;
	outFile << "\tvirtual void OnError(int code, const char* msg = \"\");" << endl;
	outFile << endl;
	outFile << endl;
	outFile << "public:" << endl;
	outFile << "\tstd::map<std::string, UINT64> infoIDMap; " << endl;
	outFile << "\tstd::map<UINT64, std::string> IDInfoMap; " << endl;
	outFile << "};" << endl;
	outFile << endl;

	outFile.close();
}

void cppRPCClientCodeMaker::makeCpp()
{
	inFile.open(dir + "RPC_Client.cpp");
	if (inFile.good())
	{
		inFile.close();
		return;
	}
	inFile.close();

	outFile.open(dir + "RPC_Client.cpp");

	//if (!inFile.is_open())
	// 존재한다면 끝내는 코드 필요 (코드 덮어쓰기 방지)
	//	return;


	outFile << "#ifdef _DEBUG" << endl;
	outFile << "#pragma comment(lib, \"RingBufferD\")" << endl;
	outFile << "#pragma comment(lib, \"MemoryPoolD\")" << endl;
	outFile << "#pragma comment(lib, \"SerializerD\")" << endl;
	outFile << "#pragma comment(lib, \"MessageLoggerD\")" << endl;
	outFile << "#pragma comment(lib, \"crashDumpD\")" << endl;
	outFile << "#pragma comment(lib, \"IOCPD\")" << endl;
	outFile << "" << endl;
	outFile << "#else" << endl;
	outFile << "#pragma comment(lib, \"RingBuffer\")" << endl;
	outFile << "#pragma comment(lib, \"MemoryPool\")" << endl;
	outFile << "#pragma comment(lib, \"Serializer\")" << endl;
	outFile << "#pragma comment(lib, \"MessageLogger\")" << endl;
	outFile << "#pragma comment(lib, \"crashDump\")" << endl;
	outFile << "#pragma comment(lib, \"IOCP\")" << endl;
	outFile << "" << endl;
	outFile << "#endif" << endl;
	outFile << "" << endl;
	outFile << "#include <WS2tcpip.h>" << endl;
	outFile << "#include <map>" << endl;
	outFile << "#include \"IOCP/IOCP/network.h\"" << endl;
	outFile << "#include \"IOCP/IOCP/session.h\"" << endl;
	outFile << "" << endl;
	outFile << "#include \"RPC_Client.h\"" << endl;
	outFile << "" << endl;
	outFile << "RPCclient::RPCclient()" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "RPCclient::~RPCclient()" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "bool RPCclient::start(const std::map<std::string, std::pair<std::string, USHORT>>& serverList)" << endl;
	outFile << "{" << endl;
	outFile << "	return true;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "void RPCclient::stop()" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "DWORD WINAPI RPCclient::workerThread(LPVOID arg)" << endl;
	outFile << "{" << endl;
	outFile << "	return 0;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "void RPCclient::OnNewConnect(UINT64 sessionID)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "void RPCclient::OnDisconnect(UINT64 sessionID)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "bool RPCclient::OnConnectionRequest(ULONG ip, USHORT port)" << endl;
	outFile << "{" << endl;
	outFile << "	return true;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "void RPCclient::OnRecv(UINT64 sessionID, packet & _packet)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "" << endl;
	outFile << "void RPCclient::OnSend(UINT64 sessionID, int sendsize)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;
	outFile << "" << endl;
	outFile << "void RPCclient::OnError(int code, const char* msg)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << "" << endl;

	outFile.close();
}