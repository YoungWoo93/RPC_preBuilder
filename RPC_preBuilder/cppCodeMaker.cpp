#include "cppCodeMaker.h"

#include <fstream>
#include <vector>
#include <iostream>

#include "cppParser.h"
using namespace std;

cppCodeMakerFile::cppCodeMakerFile(const string& _dir) : codeMakerFile(_dir) {
};

bool cppCodeMakerFile::open(const string& _fileName)
{
	clear();

	inFile.open(dir + _fileName);

	if (!inFile.is_open())
		return false;

	//string line;
	//while (getline(inFile, line))
	//{
	//	cout << line << endl;
	//}



	parsingPreprocessing();
	parsingLibrary();
	parsingInclude();
	parsingFunction();

	inFile.close();

	return true;
}

void cppCodeMakerFile::commonInit()
{
	addPreprocessing("#define _WINSOCKAPI_");

	addLibrary(library("SerializerD", "Serializer"));
	addLibrary(library("RemoteProcedureCallD", "RemoteProcedureCall"));

	addInclude(include("string", true));
	addInclude(include("Serializer/Serializer/Serializer.h", false));
	addInclude(include("RemoteProcedureCall/RemoteProcedureCall/RemoteProcedureCall.h", false));
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool cppCodeMakerFile::writeHeader(const string& _fileName)
{
	outFile.open(dir + _fileName + ".h");

	if (!outFile.is_open())
		return false;

	writePreprocessing();
	writeLibrary();
	writeInclude();
	writeGlobalVariable();
	writeFunctionDeclaration();

	outFile.close();

	return true;
}


bool cppCodeMakerFile::writeCpp(const string& _fileName)
{
	outFile.open(dir + _fileName + ".cpp");

	if (!outFile.is_open())
		return false;

	writePreprocessing();
	writeLibrary();
	writeInclude();
	writeGlobalVariable();
	writeFunction();

	outFile.close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cppCodeMakerFile::parsingPreprocessing()
{
	string line;
	while (getline(inFile, line)) 
	{
		if (line == "//PREPROCESSING START")
		{
			while (getline(inFile, line))
			{
				if (line == "//PREPROCESSING END")
					return;

				preprocessingBlock.push_back(line);
			}
		}
	}
}
void cppCodeMakerFile::parsingLibrary()
{
	string line;

	int state = 0;
	//	0 : find LIBRARY START block
	//	1 : find _DEBUG block
	//	2 : read debug lib, find else block (= find release lib block)
	//	3 : read release lib, find endif block
	//	4 : find LIBRARY END block, return;

	while (getline(inFile, line))
	{
		switch (state)
		{
		case 0:
		{
			if (line == "//LIBRARY START")
				state++;

			break;
		}

		case 1:
		{
			if (line == "//LIBRARY END")
				return;
			if (line == "#ifdef _DEBUG")
				state++;

			break;
		}

		case 2:
		{
			if (line == "#else")
			{
				state++;
				break;
			}

			if (line.empty())
				continue;

			string debugLib = string(line.begin() + line.find('"') + 1, line.begin() + line.rfind('"'));
			libraryBlock.push_back(library(debugLib, ""));

			break;
		}

		case 3:
		{
			if (line == "#endif") {
				state++;
				break;
			}

			if (line.empty())
				continue;

			string releaseLib = string(line.begin() + line.find('"') + 1, line.begin() + line.rfind('"'));
			libraryBlock.push_back(library("", releaseLib));
		}

		case 4:
		{
			if (line == "//LIBRARY END")
				return;
		}
		}
	}
}
void cppCodeMakerFile::parsingInclude()
{
	string line;

	int state = 0;
	//	0 : find INCLUDE START block
	//	1 : find SYSTEM HEADER block
	//	2 : read system header, find USER HEADER block
	//	3 : read user header, find INCLUDE END block (return);

	while (getline(inFile, line))
	{
		switch (state)
		{
		case 0:
		{
			if (line == "//INCLUDE START")
				state++;

			break;
		}

		case 1:
		{
			if (line == "//INCLUDE END")
				return;
			if (line == "//\tSYSTEM HEADER")
			{
				state++;
				continue;
			}
			if (line.empty())
				continue;

			break;
		}

		case 2:
		{
			if (line == "//INCLUDE END")
				return;
			if (line == "//\tUSER HEADER")
			{
				state++;
				continue;
			}
			if (line.empty())
				continue;

			includeBlock.push_back(include(string(line.begin() + line.find('<') + 1, line.begin() + line.find('>')), true));
			continue;
		}

		case 3:
		{
			if (line == "//INCLUDE END")
				return;
			if (line.empty())
				continue;

			includeBlock.push_back(include(string(line.begin() + line.find('"') + 1, line.begin() + line.rfind('"')), false));
			continue;
		}
		}
	}
}
void cppCodeMakerFile::parsingFunction()
{
	int bracketCount = 0;
	string line;
	
	
	while (getline(inFile, line))
	{
		function f;
		string::iterator iter;

		do
		{
			if (getCommentsCpp(line))
				continue;

			iter = line.begin();
			string type;
			if (getTypeCpp(line, iter, type))
			{
				f.returnType = type;
				getNameCpp(line, iter, f.functionName);

				string bracketBlock;
				getBracketBlockCpp(line, iter, bracketBlock);

				vector<pair<string, string>> args;
				argsParserCpp(bracketBlock, args);

				for (auto arg : args){
					f.args.push_back(argument(arg.first, arg.second));
				}

				if (*iter == ';')
				{
					functionsBlock.push_back(f);
					f.returnType.clear();
					f.functionName.clear();
					f.args.clear();
					f.implement.clear();
					continue;
				}
			}

			if (line.find('{') != string::npos) {
				bracketCount++;

				iter = line.begin();
				while (iter != line.end() && (*iter == ' ' || *iter == '\t'))
					iter++;

				if (*iter == '{')
					f.implement += line;
				else
					f.implement += '{';

				f.implement += "\n";
			}
		} while (bracketCount == 0 && getline(inFile, line));

		while (bracketCount != 0 && getline(inFile, line))
		{
			f.implement += line;
			f.implement += "\n";

			if (getCommentsCpp(line))
				continue;

			auto offset = line.find('{');
			while (offset != string::npos) {
				bracketCount++;
				offset = line.find('{', offset + 1);
			}

			offset = line.find('}');
			while (offset != string::npos) {
				bracketCount--;
				offset = line.find('}', offset + 1);
			}
		}

		if(!f.functionName.empty())
			functionsBlock.push_back(f);
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cppCodeMakerFile::writePreprocessing()
{
	outFile << "//PREPROCESSING START" << endl;
	for (auto pre : preprocessingBlock)
		outFile << pre << endl;
	outFile << "//PREPROCESSING END" << endl << endl;

	return;
}
void cppCodeMakerFile::writeLibrary()
{
	outFile << "//LIBRARY START" << endl;
	outFile << "#ifdef _DEBUG" << endl;

	for (auto lib : libraryBlock)
	{
		if (!lib.debugMode.empty()) {
			outFile << "#pragma comment(lib, \"";
			outFile << lib.debugMode;
			outFile << "\")" << endl;
		}
	}
	outFile << "#else" << endl;
	for (auto lib : libraryBlock)
	{
		if (!lib.releaseMode.empty()) {
			outFile << "#pragma comment(lib, \"";
			outFile << lib.releaseMode;
			outFile << "\")" << endl;
		}
	}
	outFile << "#endif" << endl;
	outFile << "//LIBRARY END" << endl << endl;

	return;
}
void cppCodeMakerFile::writeInclude()
{
	outFile << "//INCLUDE START" << endl;
	outFile << "//\tSYSTEM HEADER" << endl;
	for (auto _header : includeBlock)
	{
		if (_header.flag) {
			outFile << "#include <";
			outFile << _header.header;
			outFile << ">" << endl;
		}
		else
			break;
	}
	outFile << endl;

	outFile << "//\tUSER HEADER" << endl;
	for (auto _header : includeBlock)
	{
		if (!_header.flag) {
			outFile << "#include \"";
			outFile << _header.header;
			outFile << "\"" << endl;
		}
	}
	outFile << endl;

	outFile << "//INCLUDE END" << endl << endl;

	return;
}
void cppCodeMakerFile::writeFunction()
{
	for (auto f : functionsBlock)
	{
		outFile << f.returnType << " ";
		outFile << f.functionName << "(";
		if (!f.args.empty())
		{
			outFile << f.args[0].type << " " << f.args[0].name;
			for (int i = 1; i < f.args.size(); i++) {
				outFile << ", " << f.args[i].type << " " << f.args[i].name;
			}
		}
		outFile << ")" << endl;

		outFile << f.implement;
		outFile << endl;
	}

	return;
}
void cppCodeMakerFile::writeFunctionDeclaration()
{
	for (auto f : functionsBlock)
	{
		outFile << f.returnType << " ";
		outFile << f.functionName << "(";
		if (!f.args.empty())
		{
			outFile << f.args[0].type << " " << f.args[0].name;
			for (int i = 1; i < f.args.size(); i++) {
				outFile << ", " << f.args[i].type << " " << f.args[i].name;
			}
		}
		outFile << ");" << endl;
	}

	return;
}
void cppCodeMakerFile::writeGlobalVariable()
{
	for (auto v : globalVariableBlock){
		outFile << v.type << " " << v.name << ";\n";
	}
	outFile << "\n\n";
}






void outFileInit(ofstream& file)
{
	file << "#pragma once" << endl;
	file << endl;
	file << "#ifdef _DEBUG" << endl;
	file << "#pragma comment(lib, \"SerializerD\")" << endl;
	file << "#pragma comment(lib, \"RemoteProcedureCallD\")" << endl;
	file << "#else" << endl;
	file << "#pragma comment(lib, \"Serializer\")" << endl;
	file << "#pragma comment(lib, \"RemoteProcedureCall\")" << endl;
	file << "#endif" << endl;
	file << endl;
	file << "#include \"Serializer/Serializer/Serializer.h\"" << endl;
	file << "#include \"RemoteProcedureCall/RemoteProcedureCall/RemoteProcedureCall.h\"" << endl;
	file << endl;
}



// 직렬화 버퍼에 패킹해서 전송하는 부분
void makeClientRPCheaderFunction(ofstream& clientFile, vector<RPCfunction>& functions)
{
	for (auto f : functions)
	{
		clientFile << "void " << f.functionName << "(";
		string temp;
		for (auto p : f.inputArgs) {
			temp += p.type + " " + p.name + ", ";
		}
		if (!temp.empty())
			temp.erase(temp.end() - 2, temp.end());

		clientFile << temp << ")" << endl;
		clientFile << "{" << endl;
		clientFile << "\tserializer sendBuffer;" << endl;
		clientFile << endl;
		clientFile << "\tsendBuffer.moveRear(2);" << endl;
		{
			clientFile << "\tsendBuffer << (short)" << f.ID;
			for (auto p : f.inputArgs) {
				clientFile << " << ";
				clientFile << p.name;
			}
			clientFile << ";" << endl;
		}
		clientFile << "\t*(short*)sendBuffer.getHeadPtr() = sendBuffer.size();" << endl;
		clientFile << endl;
		clientFile << "\tRPCcall(sendBuffer);" << endl;
		clientFile << "}" << endl << endl;;
	}

}

// 이후 RPC 응답이 왔을때 실행될 함수 구현 틀
void makeClientRPCheader_callbackFunction(ofstream& clientFile, vector<RPCfunction>& functions)
{
	for (auto f : functions)
	{
		clientFile << "void " << f.functionName << "_callback(";

		string temp;
		if (f.returnType != "void")
			temp += f.returnType + " ret, ";

		for (auto p : f.outputArgs) {
			temp += p.type + " " + p.name + ", ";
		}

		if (!temp.empty())
			temp.erase(temp.end() - 2, temp.end());

		clientFile << temp << ")" << endl;
		clientFile << "{" << endl;
		clientFile << "\t//input logic" << endl << endl;
		clientFile << "}" << endl << endl;
	}

}

// RPC 코어에서 응답이 온 직렬화 버퍼를 넘겨줬을때, 그걸 까서 어떤 콜백을 실행 시켜야 하는지 분기를 타는 코드
void makeClientRPCheaderTable(ofstream& clientFile, map<string, vector<RPCfunction>>& summary)
{
	for (auto s : summary) {
		clientFile << "#include \"RPC_client_callback_" + s.first + ".h\"" << endl;
	}
	clientFile << endl;

	clientFile << "void RPC_callback_table(serializer& s)" << endl;
	clientFile << "{" << endl;
	clientFile << "\tshort size;" << endl;
	clientFile << "\tshort id;" << endl;
	clientFile << "\ts >> size >> id;" << endl;
	clientFile << endl;
	clientFile << "\tswitch(id)" << endl;
	clientFile << "\t{" << endl;

	for (auto functions : summary)
	{
		for (auto f : functions.second)
		{
			clientFile << "\t\tcase " << f.ID << " :" << endl;
			clientFile << "\t\t{" << endl;

			if (f.returnType != "void") {
				clientFile << "\t\t\t" << f.returnType << " ret;" << endl;
				clientFile << "\t\t\ts >> ret;" << endl;
			}

			if (!f.outputArgs.empty())
			{
				for (auto p : f.outputArgs) {
					clientFile << "\t\t\t" << p.type << " " << p.name << ";" << endl;
				}

				clientFile << "\t\t\ts ";
				for (auto p : f.outputArgs) {
					clientFile << ">> " << p.name << " ";
				}
				clientFile << ";" << endl;
			}

			string args;
			if (f.returnType != "void")
				args += "ret, ";
			for (auto p : f.outputArgs) {
				args += p.name;
				args += ", ";
			}
			if (!args.empty())
				args.erase(args.end() - 2, args.end());


			clientFile << "\t\t\t" << f.functionName << "_callback(" << args << ");" << endl;

			clientFile << "\t\t}" << endl;
			clientFile << "\t\tbreak;" << endl;
			clientFile << endl;
		}
	}


	clientFile << "\t}" << endl;
	clientFile << "}" << endl;
}


// RPC 코어에서 요청이 온 직렬화 버퍼를 받았을때, 그걸 까서 어떤 implement 함수를 실행시켜야 하는지 분기를 타는 코드 + 그 결과를 다시 시리얼라이저로 패킹해서 코어로 넘겨주는 코드가 추가되어야 함
void makeServerRPCheaderFunction(ofstream& serverFile, map<string, vector<RPCfunction>>& summary)
{
	for (auto s : summary) {
		serverFile << "#include \"RPC_server_implement_" + s.first + ".h\"" << endl;
	}
	serverFile << endl;

	serverFile << "void RPC_implement_table(serializer& s, serializer& outputs)" << endl;
	serverFile << "{" << endl;
	serverFile << "\tshort size;" << endl;
	serverFile << "\tshort id;" << endl;
	serverFile << "\ts >> size >> id;" << endl;
	serverFile << endl;
	serverFile << "\tswitch(id)" << endl;
	serverFile << "\t{" << endl;
	for (auto functions : summary)
	{
		for (auto f : functions.second)
		{
			serverFile << "\t\tcase " << f.ID << " :" << endl;
			serverFile << "\t\t{" << endl;

			if (f.returnType != "void") {
				serverFile << "\t\t\t" << f.returnType << " ret;" << endl;
				serverFile << endl;
			}

			if (!f.inputArgs.empty())
			{
				for (auto p : f.inputArgs) {
					serverFile << "\t\t\t" << p.type << " " << p.name << ";" << endl;
				}

				serverFile << "\t\t\ts ";
				for (auto p : f.inputArgs) {
					serverFile << ">> " << p.type << " ";
				}
				serverFile << ";" << endl;

				serverFile << endl;
				for (auto p : f.outputArgs) {
					serverFile << "\t\t\t" << p.type << " " << p.name << ";" << endl;
				}
			}

			string args;
			serverFile << "\t\t\t";
			if (f.returnType != "void")
				serverFile << "ret = ";
			for (auto p : f.inputArgs) {
				args += p.name;
				args += ", ";
			}
			for (auto p : f.outputArgs) {
				args += p.name;
				args += ", ";
			}
			if (!args.empty())
				args.erase(args.end() - 2, args.end());

			serverFile << f.functionName << "_implement(" << args << ");" << endl;
			serverFile << endl;

			serverFile << "\t\t\toutputs.moveRear(2);" << endl;

			serverFile << "\t\t\toutputs << (short)" << f.ID;
			if (f.returnType != "void")
				serverFile << " << ret";

			for (auto p : f.outputArgs) {
				serverFile << " << ";
				serverFile << p.name;
			}
			serverFile << ";" << endl;
			serverFile << endl;
			serverFile << "\t\t\t*(short*)(outputs.getHeadPtr()) = outputs.size();" << endl;
			serverFile << endl;

			serverFile << "\t\t}" << endl;
			serverFile << "\t\tbreak;" << endl;
			serverFile << endl;
		}
	}

	serverFile << "\t}" << endl;
	serverFile << "}" << endl;
}

// RPC 요청이 왔을 때, 서버단에서 실행할 동작을 구현하는 틀이 추가되어야 함
void makeServerRPCheader_implementFunction(ofstream& serverFile, vector<RPCfunction>& functions)
{
	for (auto f : functions)
	{
		serverFile << f.returnType << " " << f.functionName << "_implement(";

		string temp;

		for (auto p : f.inputArgs) {
			temp += p.type + " " + p.name + ", ";
		}

		for (auto p : f.outputArgs) {
			temp += p.type + "& " + p.name + ", ";
		}

		if (!temp.empty())
			temp.erase(temp.end() - 2, temp.end());

		serverFile << temp << ")" << endl;
		serverFile << "{" << endl;
		serverFile << "\t//input logic" << endl << endl;
		serverFile << "}" << endl << endl;
	}
}