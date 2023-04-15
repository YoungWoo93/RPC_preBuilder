#pragma once

#include "cppCodeMaker.h"
#include "RPC_preBuilder.h"

class cppClientCodeMaker : public cppCodeMakerFile
{
public:
	void make(RPCscript& newScript, RPCscript& duplicationScript, RPCscript& removeScript);
	void makeHeader(RPCscript& newScript, RPCscript& duplicationScript, RPCscript& removeScript);
	void makeCpp(RPCscript& newScript, RPCscript& duplicationScript, RPCscript& removeScript);
};