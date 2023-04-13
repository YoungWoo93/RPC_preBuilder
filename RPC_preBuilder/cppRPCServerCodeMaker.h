#pragma once

#include "cppCodeMaker.h"
#include "RPC_preBuilder.h"

class cppRPCServerCodeMaker : public cppCodeMakerFile
{
public:
	void make();
	void makeHeader();
	void makeCpp();
};