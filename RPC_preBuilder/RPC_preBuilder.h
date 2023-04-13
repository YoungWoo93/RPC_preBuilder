#pragma once
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct RPCargument;
struct RPCfile;
struct RPCfunction;

typedef map<string, RPCfile> RPCscript;
typedef vector<RPCfunction> RPCfunctions;
typedef vector<RPCargument> RPCparameter;

struct RPCargument {
	RPCargument(const string& _type, const string& _name) : type(_type), name(_name) {
	}

	bool operator== (const RPCargument& arg) {
		return type == arg.type && name == arg.name;
	}
	bool operator!= (const RPCargument& arg) {
		return type != arg.type || name != arg.name;
	}

	string type;
	string name;
};
struct RPCfile {
	RPCfile(const string& _fileName, RPCfunctions _functions) : name(_fileName), functions(_functions) {
	}
	RPCfile() {
	}
	string name;
	RPCfunctions functions;
};

struct RPCfunction {
	RPCfunction(int _ID, const string& _returnType, const string& _functionName, RPCparameter& _inputArgs, RPCparameter& _outputArgs)
		: ID(_ID), returnType(_returnType), functionName(_functionName), inputArgs(_inputArgs), outputArgs(_outputArgs) {
	}

	bool operator== (const RPCfunction& f) {
		if (returnType != f.returnType)
			return false;
		if (functionName != f.functionName)
			return false;
		if (inputArgs.size() != f.inputArgs.size())
			return false;
		if (outputArgs.size() != f.outputArgs.size())
			return false;

		for (int i = 0; i < inputArgs.size(); i++) {
			if (inputArgs[i] != f.inputArgs[i])
				return false;
		}

		for (int i = 0; i < outputArgs.size(); i++) {
			if (outputArgs[i] != f.outputArgs[i])
				return false;
		}

		return true;
	}

	bool operator!= (const RPCfunction& f) {
		if (returnType != f.returnType)
			return true;
		if (functionName != f.functionName)
			return true;
		if (inputArgs.size() != f.inputArgs.size())
			return true;
		if (outputArgs.size() != f.outputArgs.size())
			return true;

		for (int i = 0; i < inputArgs.size(); i++) {
			if (inputArgs[i] != f.inputArgs[i])
				return true;
		}

		for (int i = 0; i < outputArgs.size(); i++) {
			if (outputArgs[i] != f.outputArgs[i])
				return true;
		}

		return false;
	}

	int ID;
	string returnType;
	string functionName;
	RPCparameter inputArgs;
	RPCparameter outputArgs;
};


bool openScript(const string& _dir, RPCscript& script);
void saveScript(const string& _dir, RPCscript& script);

void compareScript(RPCscript& pastScript, RPCscript& newScript, RPCscript& adds, RPCscript& removes);