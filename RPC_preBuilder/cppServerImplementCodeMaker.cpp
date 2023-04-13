#include "cppServerImplementCodeMaker.h"

#include <iostream>

using namespace std;

void cppImplementCodeMaker::make(RPCfile& info)
{
	makeHeader(info);
	makeCpp(info);
}

void cppImplementCodeMaker::makeHeader(RPCfile& info)
{
	string fileName = "RPC_server_implement_" + info.name;

	clear();
	addPreprocessing("#pragma once");
	commonInit();

	for (auto RPCf : info.functions)
	{
		parameter p;
		p.push_back(argument("UINT64", "sessionID"));
		if (!RPCf.inputArgs.empty())
		{
			p.push_back(argument("/*IN*/" + RPCf.inputArgs[0].type, RPCf.inputArgs[0].name));
			p.insert(p.end(), RPCf.inputArgs.begin() + 1, RPCf.inputArgs.end());
		}
		if (!RPCf.outputArgs.empty())
		{
			p.push_back(argument("/*OUT*/" + RPCf.outputArgs[0].type + "&", RPCf.outputArgs[0].name));
			for (int i = 1; i < RPCf.outputArgs.size(); i++) {
				p.push_back(argument(RPCf.outputArgs[i].type + "&", RPCf.outputArgs[i].name));
			}
		}
		function f(RPCf.returnType, RPCf.functionName + "_implement", p);

		addFunction(f);
	}

	writeHeader(fileName);
}

void cppImplementCodeMaker::makeCpp(RPCfile& info)
{
	string fileName = "RPC_server_implement_" + info.name;

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

	addGlobalVariable(argument("extern RPCserver*", "rpcServer"));

	for (auto& f : functionsBlock)
		f.flag = false;

	for (auto RPCf : info.functions)
	{
		parameter p;

		p.push_back(argument("UINT64", "sessionID"));
		if (!RPCf.inputArgs.empty())
		{
			p.push_back(argument(RPCf.inputArgs[0].type, RPCf.inputArgs[0].name));
			p.insert(p.end(), RPCf.inputArgs.begin() + 1, RPCf.inputArgs.end());
		}
		if (!RPCf.outputArgs.empty())
		{
			p.push_back(argument(RPCf.outputArgs[0].type + "&", RPCf.outputArgs[0].name));
			for (int i = 1; i < RPCf.outputArgs.size(); i++) {
				p.push_back(argument(RPCf.outputArgs[i].type + "&", RPCf.outputArgs[i].name));
			}
		}
		function f(RPCf.returnType, RPCf.functionName + "_implement", p);
		f.implement += "{\n";
		f.implement += "\t//input logic\n";
		f.implement += "\n";
		f.implement += "}\n";

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