#pragma once

#include "cppCodeMaker.h"
#include "RPC_preBuilder.h"

class cppRPCClientCodeMaker : public cppCodeMakerFile
{
public:
	void make();
	void makeHeader();
	void makeCpp();
};