#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "RPC_preBuilder.h"
using namespace std;

struct argument;
typedef vector<argument> parameter;

struct argument {
	argument(const string& _type, const string& _name) : type(_type), name(_name) {
	}
	argument(RPCargument& rpcArgument) : type(rpcArgument.type), name(rpcArgument.name) {
	}

	bool operator== (const argument& arg) {
		return type == arg.type && name == arg.name;
	}
	bool operator!= (const argument& arg) {
		return type != arg.type || name != arg.name;
	}

	argument& operator= (const argument& arg) {
		type = arg.type;
		name = arg.name;

		return *this;
	}

	string type;
	string name;
};

struct function {
	function(const string& _returnType, const string& _functionName, parameter& _args)
		: flag(false), returnType(_returnType), functionName(_functionName), args(_args){
	}
	function() {
	}

	bool operator== (const function& f) {
		if (returnType != f.returnType)
			return false;
		if (functionName != f.functionName)
			return false;
		if (args.size() != f.args.size())
			return false;

		for (int i = 0; i < args.size(); i++) {
			if (args[i] != f.args[i])
				return false;
		}


		return true;
	}

	bool operator!= (const function& f) {
		if (returnType != f.returnType)
			return true;
		if (functionName != f.functionName)
			return true;
		if (args.size() != f.args.size())
			return true;

		for (int i = 0; i < args.size(); i++) {
			if (args[i] != f.args[i])
				return true;
		}


		return false;
	}

	function& operator= (const function& f) {
		returnType = f.returnType;
		functionName = f.functionName;
		args.assign(f.args.begin(), f.args.end());
		implement = f.implement;

		return *this;
	}
	bool flag;
	string returnType;
	string functionName;
	parameter args;

	string implement;
};

struct include {
	include(const string& _header, bool isSystem) : header(_header), flag(isSystem) {
	}

	string header;
	bool flag;

	bool operator== (const include& i) {
		return header == i.header;
	}
	bool operator!= (const include& i) {
		return header != i.header;
	}
	include& operator= (const include& i) {
		header = i.header;

		return *this;
	}
};

struct library {
	library(const string& _debugMode, const string& _releaseMode) : debugMode(_debugMode), releaseMode(_releaseMode) {
	}

	string debugMode;
	string releaseMode;

	bool operator== (const library& l) {
		return debugMode == l.debugMode && releaseMode == l.releaseMode;
	}
	bool operator!= (const library& l) {
		return debugMode != l.debugMode || releaseMode != l.releaseMode;
	}

	library& operator= (const library& l) {
		debugMode = l.debugMode;
		releaseMode = l.releaseMode;

		return *this;
	}
};

class codeMakerFile {
public:
	codeMakerFile(const string& _dir);

	void clear();

	bool addPreprocessing(const string& _line);
	bool removePreprocessing(const string& _line);

	bool addLibrary(const library& _lib);
	bool removeLibrary(const library& _lib);

	bool addInclude(const include& _header);
	bool removeInclude(const include& _header);

	bool addFunction(const function& _func);
	bool removeFunction(const function& _func);

	bool addGlobalVariable(const argument& _var);
	bool removeGlobalVariable(const argument& _var);
public:
	string dir;
	ofstream outFile;
	ifstream inFile;

	vector<string> preprocessingBlock;
	vector<library> libraryBlock;
	vector<include> includeBlock;
	vector<function> functionsBlock;
	vector<argument> globalVariableBlock;
};