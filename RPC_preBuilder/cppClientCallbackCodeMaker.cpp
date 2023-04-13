#include "cppClientCallbackCodeMaker.h"

#include <iostream>

using namespace std;

void cppCallbackCodeMaker::make(RPCfile& info)
{
	makeHeader(info);
	makeCpp(info);
}

void cppCallbackCodeMaker::makeHeader(RPCfile& info)
{
	string fileName = "RPC_client_callback_" + info.name;

	clear();
	addPreprocessing("#pragma once");
	commonInit();

	for (auto RPCf : info.functions)
	{
		parameter p;
		if (RPCf.returnType != "void")
			p.push_back(argument(RPCf.returnType, "ret"));
		for (auto arg : RPCf.outputArgs) {
			p.push_back(arg);
		}

		function f("void", RPCf.functionName + "_callback", p);
		addFunction(f);
	}

	writeHeader(fileName);
}

void cppCallbackCodeMaker::makeCpp(RPCfile& info)
{
	string fileName = "RPC_client_callback_" + info.name;
	
	clear();
	if (!open(fileName))
	{
		cout << "open fail " << dir << fileName << ".cpp" << endl;
		//return;
	}

	if (preprocessingBlock.empty() && libraryBlock.empty() && includeBlock.empty() && functionsBlock.empty())
	{
		// 최초 생성 이므로 기본 양식 넣어줘야함
		addInclude(include(dir + fileName + ".h", false));
	}


	for (auto& f : functionsBlock)
		f.flag = false;

	for (auto RPCf : info.functions)
	{
		parameter p;
		if (RPCf.returnType != "void")
			p.push_back(argument(RPCf.returnType, "ret"));
		for (auto arg : RPCf.outputArgs) {
			p.push_back(arg);
		}

		function f("void", RPCf.functionName + "_callback", p);
		f.implement = "{\n\t//구현\n\n}\n";

		addFunction(f);
	}

	for (auto f = functionsBlock.begin(); f != functionsBlock.end(); f++)
	{
		if (!f->flag) {
			f = functionsBlock.erase(f);
			f--;
		}
	}

	writeCpp(fileName);
}