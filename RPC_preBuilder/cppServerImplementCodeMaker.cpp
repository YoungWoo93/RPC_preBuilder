#include "cppServerImplementCodeMaker.h"

#include <iostream>

using namespace std;

void cppImplementCodeMaker::make(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile)
{
	makeHeader(newFile, duplicationFile, removeFile);
	makeCpp(newFile, duplicationFile, removeFile);
}

void cppImplementCodeMaker::makeHeader(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile)
{
	clear();
	string fileName = "RPC_server_implement_" + newFile.name;

	if (!open(fileName + ".h"))
	{
		cout << "open fail " << dir << fileName << ".h" << endl;
		//return;
	}

	if (preprocessingBlock.empty() && libraryBlock.empty() && includeBlock.empty() && functionsBlock.empty())
	{
		// 최초 생성 이므로 기본 양식 넣어줘야함
		addPreprocessing("#pragma once");
		commonInit();

		for (auto RPCf : duplicationFile.functions)
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

			addFunction(f);
		}
	}
	else
	{
		for (auto removeFunc : removeFile.functions)
		{
			parameter p;
			p.push_back(argument("UINT64", "sessionID"));
			if (!removeFunc.inputArgs.empty())
			{
				p.push_back(argument(removeFunc.inputArgs[0].type, removeFunc.inputArgs[0].name));
				p.insert(p.end(), removeFunc.inputArgs.begin() + 1, removeFunc.inputArgs.end());
			}
			if (!removeFunc.outputArgs.empty())
			{
				p.push_back(argument(removeFunc.outputArgs[0].type + "&", removeFunc.outputArgs[0].name));
				for (int i = 1; i < removeFunc.outputArgs.size(); i++) {
					p.push_back(argument(removeFunc.outputArgs[i].type + "&", removeFunc.outputArgs[i].name));
				}
			}

			for (auto& func : functionsBlock)
			{
				if (func == function(removeFunc.returnType, removeFunc.functionName + "_implement", p))
					func.flag = false;
			}
			functionsBlock.erase(std::remove_if(functionsBlock.begin(), functionsBlock.end(), [](auto& func) { return func.flag == false; }), functionsBlock.end());

		}
	}

	for (auto RPCf : newFile.functions)
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

		addFunction(f);
	}

	writeHeader(fileName);
}

void cppImplementCodeMaker::makeCpp(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile)
{
	clear();

	string fileName = "RPC_server_implement_" + newFile.name;
	if (!open(fileName + ".cpp"))
	{
		cout << "open fail " << dir << fileName << ".cpp" << endl;
		//return;
	}
	addGlobalVariable(argument("extern RPCserver*", "rpcServer"));

	if (preprocessingBlock.empty() && libraryBlock.empty() && includeBlock.empty() && functionsBlock.empty())
	{
		// 최초 생성 이므로 기본 양식 넣어줘야함
		addInclude(include(dir + fileName + ".h", false));

		for (auto RPCf : duplicationFile.functions)
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
	}
	else
	{
		for (auto RPCf : removeFile.functions)
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

			for (auto& func : functionsBlock)
			{
				if (func == function(RPCf.returnType, RPCf.functionName + "_implement", p))
					func.flag = false;
			}
			
			functionsBlock.erase(
				std::remove_if(functionsBlock.begin(), functionsBlock.end(), [](auto& func) { return func.flag == false; }),
				functionsBlock.end());
		}
	}

	for (auto RPCf : newFile.functions)
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

	writeCpp(fileName);
}