#pragma once

#include "cppCodeMaker.h"
#include "RPC_preBuilder.h"

class cppImplementCodeMaker : public cppCodeMakerFile
{
public:
	void make(RPCfile& info);
	void makeHeader(RPCfile& info);
	void makeCpp(RPCfile& info);
};