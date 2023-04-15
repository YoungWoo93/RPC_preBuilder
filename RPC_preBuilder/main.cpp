#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <algorithm>

#include <Windows.h>

using namespace std;

#include "RPC_preBuilder.h"
#include "cppCodeMaker.h"
#include "cppClientCallbackCodeMaker.h"
#include "cppClientCodeMaker.h"
#include "cppServerImplementCodeMaker.h"
#include "cppServerCodeMaker.h"

#include "cppRPCClientCodeMaker.h"
#include "cppRPCServerCodeMaker.h"
#ifndef MAX_PATH
#define MAX_PATH	260
#endif

void splitFullPath(string fullPath, string& dir, string& file)
{
	int index = fullPath.rfind('\\');
	dir = string(fullPath.begin(), fullPath.begin() + index);
	file = string(fullPath.begin() + index + 1, fullPath.end());

}


void main(int argc, char* argv[])
{
	char fullName[MAX_PATH];
	string dir, file;

	GetFullPathNameA(argv[1], MAX_PATH, fullName, NULL);
	splitFullPath(fullName, dir, file);

	cout << "dir : " << dir << endl;
	cout << "file : " << file << endl;

	RPCscript newScript;
	RPCscript oldScript;
	openScript(file, newScript);
	openScript("RPCdata.data", oldScript);

	RPCscript addScript;
	RPCscript removeScript;
	RPCscript remainingScript;
	RPCscript duplicationScript;
	compareScript(oldScript, newScript, addScript, removeScript, duplicationScript);


	cppCodeMakerFile CM(dir);
	for (auto file : newScript)
	{
		string name = file.first;

		((cppCallbackCodeMaker*)&CM)->make(addScript[name], duplicationScript[name], removeScript[name]);

	}
	((cppClientCodeMaker*)&CM)->make(addScript, duplicationScript, removeScript);
	for (auto file : newScript)
	{
		string name = file.first;

		((cppImplementCodeMaker*)&CM)->make(addScript[name], duplicationScript[name], removeScript[name]);

	}
	((cppServerCodeMaker*)&CM)->make(addScript, duplicationScript, removeScript);
	((cppRPCServerCodeMaker*)&CM)->make();
	((cppRPCClientCodeMaker*)&CM)->make();
	saveScript("RPCdata.data", newScript);
	return;
}


/*
void main(int argc, char* argv[])
{
	char fullName[MAX_PATH];
	string dir, file;

	GetFullPathNameA(argv[1], MAX_PATH, fullName, NULL);
	splitFullPath(fullName, dir, file);

	cout << "dir : " << dir << endl;
	cout << "file : " << file << endl;

	ifstream past(dir + "RPCdata.data");
	string line;
	int count = 0;
	short RPCID = 0;
	string fileName = "default";

	if (past.goodbit)
	{
		while (getline(past, line)) {
			string returnType;
			string functionName;
			vector<pair<string, string>> inputArgs;
			vector<pair<string, string>> outputArgs;

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

				pastRPCsummary[fileName] = vector<RPCfunction>();
				continue;
			}

			int ret = lineParsing(line, returnType, functionName, inputArgs, outputArgs);
	
			pastRPCsummary[fileName].push_back(RPCfunction(0, returnType, functionName, inputArgs, outputArgs));
		}
	}


	ifstream f(fullName);
	
	vector<RPCfunction> functionList;

	
	RPCsummary[fileName] = vector<RPCfunction>();

	while (getline(f, line)) {
		count++;
		string returnType;
		string functionName;
		vector<pair<string, string>> inputArgs;
		vector<pair<string, string>> outputArgs;


		cout << count << "\t" << line << endl;
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
			
			RPCsummary[fileName] = vector<RPCfunction>();
			continue;
		}

		int ret = lineParsing(line, returnType, functionName, inputArgs, outputArgs);
		if (ret == 0) {
			cout << "\treturn type : " << returnType << endl;
			cout << "\tfunction name : " << functionName << endl;
			cout << "\tinputArgs : " << endl;
			for (auto p : inputArgs) {
				cout << "\t\t" << p.first << "\t" << p.second << endl;
			}
			cout << "\toutputArgs : " << endl;
			for (auto p : outputArgs) {
				cout << "\t\t" << p.first << "\t" << p.second << endl;
			}
			cout << endl;

			RPCsummary[fileName].push_back(RPCfunction(RPCID++, returnType, functionName, inputArgs, outputArgs));
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
	f.close();


	ofstream clientHeader(dir + "\\RPC_client.h", std::ofstream::app);
	if(!clientHeader.goodbit)
		outFileInit(clientHeader);

	for (auto files : RPCsummary)
		makeClientRPCheaderFunction(clientHeader, files.second);

	makeClientRPCheaderTable(clientHeader, RPCsummary);

	for (auto files : RPCsummary)
	{
		ofstream clientCallbackHeader(dir + "\\RPC_client_callback_" + files.first + ".h");
		if (!clientCallbackHeader.goodbit)
			outFileInit(clientCallbackHeader);

		makeClientRPCheader_callbackFunction(clientCallbackHeader, files.second);

		clientCallbackHeader.close();
	}
	clientHeader.close();



	ofstream serverHeader(dir + "\\RPC_server.h");
	if (!serverHeader.goodbit)
		outFileInit(serverHeader);

	makeServerRPCheaderFunction(serverHeader, RPCsummary);
	serverHeader.close();

	for (auto files : RPCsummary)
	{
		ofstream serverImplementHeader(dir + "\\RPC_server_implement_" + files.first + ".h");
		if (!serverImplementHeader.goodbit)
			outFileInit(serverImplementHeader);
		
		makeServerRPCheader_implementFunction(serverImplementHeader, files.second);
		
		serverImplementHeader.close();
	}
	
}

*/