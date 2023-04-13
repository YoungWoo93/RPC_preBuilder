#pragma once

#include "cppCodeMaker.h"
#include "RPC_preBuilder.h"

class cppCallbackCodeMaker : public cppCodeMakerFile 
{
public:
	void make(RPCfile& info);
	void makeHeader(RPCfile& info);
	void makeCpp(RPCfile& info);
};