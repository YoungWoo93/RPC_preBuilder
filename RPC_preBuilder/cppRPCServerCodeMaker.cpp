#include "cppRPCServerCodeMaker.h"

#include <iostream>

using namespace std;

void cppRPCServerCodeMaker::make()
{
	makeHeader();
	makeCpp();
}

void cppRPCServerCodeMaker::makeHeader()
{
	inFile.open(dir + "RPC_Server.h");
	if (inFile.good()){
		inFile.close();
		return;
	}
	inFile.close();


	outFile.open(dir + "RPC_Server.h");

	outFile << "#include \"IOCP/IOCP/network.h\"" << endl;
	outFile << "#include \"IOCP/IOCP/session.h\"" << endl;
	outFile << endl;
	outFile << "class RPCserver : public Network {" << endl;
	outFile << "\tpublic:" << endl;
	outFile << "\tvirtual void OnNewConnect(UINT64 sessionID);" << endl;
	outFile << endl;
	outFile << "\tvirtual void OnDisconnect(UINT64 sessionID);" << endl;
	outFile << "\tvirtual bool OnConnectionRequest(ULONG ip, USHORT port);" << endl;
	outFile << "\tvirtual void OnRecv(UINT64 sessionID, packet& _packet);" << endl;
	outFile << endl;
	outFile << "\tvirtual void OnSend(UINT64 sessionID, int sendsize);" << endl;
	outFile << endl;
	outFile << "\tvirtual void OnError(int code, const char* msg = \"\");" << endl;
	outFile << "};" << endl;

	outFile.close();
}

void cppRPCServerCodeMaker::makeCpp()
{
	inFile.open(dir + "RPC_Server.cpp");
	if (inFile.good()){
		inFile.close();
		return;
	}
	inFile.close();

	outFile.open(dir + "RPC_Server.cpp");

	outFile << "#ifdef _DEBUG" << endl;
	outFile << "#pragma comment(lib, \"RingBufferD\")" << endl;
	outFile << "#pragma comment(lib, \"MemoryPoolD\")" << endl;
	outFile << "#pragma comment(lib, \"SerializerD\")" << endl;
	outFile << "#pragma comment(lib, \"MessageLoggerD\")" << endl;
	outFile << "#pragma comment(lib, \"crashDumpD\")" << endl;
	outFile << "#pragma comment(lib, \"IOCPD\")" << endl;
	outFile << endl;
	outFile << "#else" << endl;
	outFile << "#pragma comment(lib, \"RingBuffer\")" << endl;
	outFile << "#pragma comment(lib, \"MemoryPool\")" << endl;
	outFile << "#pragma comment(lib, \"Serializer\")" << endl;
	outFile << "#pragma comment(lib, \"MessageLogger\")" << endl;
	outFile << "#pragma comment(lib, \"crashDump\")" << endl;
	outFile << "#pragma comment(lib, \"IOCP\")" << endl;
	outFile << endl;
	outFile << "#endif" << endl;
	outFile << endl;
	outFile << "#include \"IOCP/IOCP/network.h\"" << endl;
	outFile << "#include \"IOCP/IOCP/session.h\"" << endl;
	outFile << endl;
	outFile << "#include \"RPC_Server.h\"" << endl;
	outFile << endl;
	outFile << "void RPCserver::OnNewConnect(UINT64 sessionID)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << endl;
	outFile << "void RPCserver::OnDisconnect(UINT64 sessionID)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << endl;
	outFile << "bool RPCserver::OnConnectionRequest(ULONG ip, USHORT port)" << endl;
	outFile << "{" << endl;
	outFile << "	return true;" << endl;
	outFile << "}" << endl;
	outFile << endl;
	outFile << "void RPCserver::OnRecv(UINT64 sessionID, packet & _packet)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << endl;
	outFile << endl;
	outFile << "void RPCserver::OnSend(UINT64 sessionID, int sendsize)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << endl;
	outFile << endl;
	outFile << "void RPCserver::OnError(int code, const char* msg)" << endl;
	outFile << "{" << endl;
	outFile << "	return;" << endl;
	outFile << "}" << endl;
	outFile << endl;
	outFile.close();
}