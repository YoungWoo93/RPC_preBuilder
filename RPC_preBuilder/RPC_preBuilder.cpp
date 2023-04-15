#include "RPC_preBuilder.h"

#include <iostream>
#include <fstream>
#include <string>

#include <Windows.h>

#include "scriptParser.h"

using namespace std;

bool openScript(const string& _dir, RPCscript& script)
{
	ifstream file(_dir);
	string line;
	int count = 0;
	short RPCID = 0;
	string fileName = "default";
	RPCfile rpcFile(fileName, RPCfunctions());

	if (!file.is_open())
		return false;

	while (getline(file, line)) {
		string returnType;
		string functionName;
		RPCparameter inputArgs;
		RPCparameter outputArgs;

		whiteSpaceCleanup(line);
		if (line.empty())
			continue;

		if (line[0] == ' ')
			line.erase(line.begin());
		if (line.length() >= 2 && line[0] == '/' && line[1] == '/')
			continue;

		if (line[0] == '[')
		{
			fileName.clear();
			for (int i = 1; i < line.length() && line[i] != ']'; i++)
				fileName += line[i];

			script[rpcFile.name] = rpcFile;

			rpcFile = RPCfile(fileName, RPCfunctions());
			continue;
		}

		int ret = lineParsing(line, returnType, functionName, inputArgs, outputArgs);
		if (ret == 0) {
			cout << "\treturn type : " << returnType << endl;
			cout << "\tfunction name : " << functionName << endl;
			cout << "\tinputArgs : " << endl;
			for (auto p : inputArgs) {
				cout << "\t\t" << p.type << "\t" << p.name << endl;
			}
			cout << "\toutputArgs : " << endl;
			for (auto p : outputArgs) {
				cout << "\t\t" << p.type << "\t" << p.name << endl;
			}
			cout << endl;

			rpcFile.functions.push_back(RPCfunction(RPCID++, returnType, functionName, inputArgs, outputArgs));
			cout << "--------------------------" << endl;
		}
		else {
			cout << "ERROR on " << count << " line, ";
			switch (ret)
			{
			case 1:
				cout << "return type check step" << endl;
				break;
			case 2:
				cout << "function name check step" << endl;
				break;
			case 3:
				cout << "input args check step" << endl;
				break;
			case 4:
				cout << "output args check step" << endl;
				break;
			}
			cout << endl;
		}
	}

	file.close();

	script[rpcFile.name] = rpcFile;
	return true;
}

void saveScript(const string& _dir, RPCscript& script)
{
	ofstream outScript(_dir);

	for (auto file : script) {
		if (file.second.name != "default") {
			outScript << endl;
			outScript << "[" << file.second.name << "]" << endl;
		}

		for (auto function : file.second.functions) {
			outScript << function.returnType << " " << function.functionName;

			outScript << "(";
			if (!function.inputArgs.empty())
			{
				outScript << function.inputArgs[0].type << " " << function.inputArgs[0].name;
				for (int i = 1; i < function.inputArgs.size(); i++) {
					outScript << ", " << function.inputArgs[i].type << " " << function.inputArgs[i].name;
				}
			}
			outScript << ")";

			outScript << "(";
			if (!function.outputArgs.empty())
			{
				outScript << function.outputArgs[0].type << " " << function.outputArgs[0].name;
				for (int i = 1; i < function.outputArgs.size(); i++) {
					outScript << ", " << function.outputArgs[i].type << " " << function.outputArgs[i].name;
				}
			}
			outScript << ")" << endl;
		}
	}

	outScript.close();
}


void compareScript(RPCscript& pastScript, RPCscript& newScript, RPCscript& adds, RPCscript& removes, RPCscript& duplication)
{
	for (auto file : newScript)
	{
		string _name = file.second.name;
		adds[_name].name = _name;
		removes[_name].name = _name;
		duplication[_name].name = _name;
	}
	for (auto file : pastScript)
	{
		string _name = file.second.name;
		adds[_name].name = _name;
		removes[_name].name = _name;
		duplication[_name].name = _name;
	}

	for (auto file : pastScript) {

		for (auto function : file.second.functions) 
		{
			bool flag = false;
			for (auto target : newScript[file.second.name].functions)
			{ 
				if (function == target)
				{
					flag = true;
					break;
					
				}
			}

			if (flag)  // 과거에 존재 & 현재 존재 = 중복 함수 (기 구현 함수)
			{
				duplication[file.second.name].functions.push_back(function);
			}
			else
			{
				removes[file.second.name].functions.push_back(function);
			}
		}
	}


	RPCfile temp;

	for (auto file : newScript) {
		for (auto function : file.second.functions)
		{
			bool flag = false;
			for (auto target : duplication[file.second.name].functions) {
				if (function == target)	
				{
					flag = true;
					break;
				}
			}

			if(!flag)	// 새 스크립트 존재 & 중복에 비존재 = 완전 쌔 함수
				adds[file.second.name].functions.push_back(function);
		}
	}
}