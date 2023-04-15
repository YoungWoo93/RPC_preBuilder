#include "cppClientCallbackCodeMaker.h"

#include <iostream>

using namespace std;

void cppCallbackCodeMaker::make(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile)
{

	makeHeader(newFile, duplicationFile, removeFile);
	makeCpp(newFile, duplicationFile, removeFile);
}

void cppCallbackCodeMaker::makeHeader(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile)
{
	clear();

	string fileName = "RPC_client_callback_" + newFile.name;

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
			if (RPCf.returnType != "void")
				p.push_back(argument(RPCf.returnType, "ret"));
			for (auto arg : RPCf.outputArgs) {
				p.push_back(arg);
			}

			function f("void", RPCf.functionName + "_callback", p);
			addFunction(f);
		}
	}
	else
	{
		for (auto removeFunc : removeFile.functions)
		{
			parameter p;
			if (removeFunc.returnType != "void")
				p.push_back(argument(removeFunc.returnType, "ret"));
			for (auto arg : removeFunc.outputArgs) {
				p.push_back(arg);
			}

			for (auto &func : functionsBlock)
			{
				if (func == function("void", removeFunc.functionName + "_callback", p))
					func.flag = false;
			}
			functionsBlock.erase(
				remove_if(functionsBlock.begin(), functionsBlock.end(), [](auto& func) { return func.flag == false; }), 
				functionsBlock.end());

		}
	}

	for (auto RPCf : newFile.functions)
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

void cppCallbackCodeMaker::makeCpp(RPCfile& newFile, RPCfile& duplicationFile, RPCfile& removeFile)
{
	clear();

	string fileName = "RPC_client_callback_" + newFile.name;
	
	if (!open(fileName +".cpp"))
	{
		cout << "open fail " << dir << fileName + ".cpp" << endl;
		//return;
	}

	if (preprocessingBlock.empty() && libraryBlock.empty() && includeBlock.empty() && functionsBlock.empty())
	{
		// 최초 생성 이므로 기본 양식 넣어줘야함
		addInclude(include(dir + fileName + ".h", false));

		for (auto RPCf : duplicationFile.functions)
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
	}
	else
	{
		for (auto removeFunc : removeFile.functions)
		{
			parameter p;
			if (removeFunc.returnType != "void")
				p.push_back(argument(removeFunc.returnType, "ret"));
			for (auto arg : removeFunc.outputArgs) {
				p.push_back(arg);
			}

			for (auto &func : functionsBlock)
			{
				if (func == function("void", removeFunc.functionName + "_callback", p))
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
		if (RPCf.returnType != "void")
			p.push_back(argument(RPCf.returnType, "ret"));
		for (auto arg : RPCf.outputArgs) {
			p.push_back(arg);
		}

		function f("void", RPCf.functionName + "_callback", p);
		f.implement = "{\n\t//구현\n\n}\n";

		addFunction(f);
	}

	

	writeCpp(fileName);
}