#pragma once

#include "cppCodeMaker.h"
#include "RPC_preBuilder.h"

class cppServerCodeMaker : public cppCodeMakerFile
{
public:
	void make(RPCscript& script);
	void makeHeader(RPCscript& script);
	void makeCpp(RPCscript& script);
};