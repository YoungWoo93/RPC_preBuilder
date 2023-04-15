#pragma once

#include "cppCodeMaker.h"
#include "RPC_preBuilder.h"

class cppImplementCodeMaker : public cppCodeMakerFile
{
public:
	void make(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile);
	void makeHeader(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile);
	void makeCpp(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile);
};