#include "cppServerCodeMaker.h"

#include <iostream>

using namespace std;

void cppServerCodeMaker::make(RPCscript& newScript, RPCscript& duplicationScript, RPCscript& removeScript)
{
	makeHeader(newScript, duplicationScript, removeScript);
	makeCpp(newScript, duplicationScript, removeScript);
}

void cppServerCodeMaker::makeHeader(RPCscript& newScript, RPCscript& duplicationScript, RPCscript& removeScript)
{
	string fileName = "RPC_server_implement";

	clear();
	addPreprocessing("#pragma once");
	commonInit();
	
	for (auto RPCf : duplicationScript) {
		addInclude(include(dir + fileName + "_" + RPCf.second.name + ".h", false));
	}
	for (auto RPCf : newScript) {
		addInclude(include(dir + fileName + "_" + RPCf.second.name + ".h", false));
	}

	parameter p;

	p.push_back(argument("UINT64", "sessionID"));
	p.push_back(argument("serializer&", "s"));
	p.push_back(argument("serializer&", "outputs"));

	function f("void", "RPC_implement_table", p);
	addFunction(f);

	writeHeader(fileName);
}

void cppServerCodeMaker::makeCpp(RPCscript& newScript, RPCscript& duplicationScript, RPCscript& removeScript)
{
	clear();
	string fileName = "RPC_server_implement";

	clear();
	addInclude(include(dir + fileName + ".h", false));

	parameter p;
	p.push_back(argument("UINT64", "sessionID"));
	p.push_back(argument("serializer&", "s"));
	p.push_back(argument("serializer&", "outputs"));
	function f("void", "RPC_implement_table", p);
	f.implement += "{\n";
	f.implement += "\tshort id;\n";
	f.implement += "\ts >> id;\n";
	f.implement += "\n";
	f.implement += "\tswitch(id)\n";
	f.implement += "\t{\n";
	for (auto RPCf : duplicationScript)
	{
		for (auto func : RPCf.second.functions)
		{
			parameter p;
			f.implement += "\t\tcase " + to_string(func.ID) + " :\n";
			f.implement += "\t\t{\n";
			if (func.returnType != "void") {
				f.implement += "\t\t\t" + func.returnType + " ret;\n";
			}
			if (!func.inputArgs.empty()) {
				for (auto arg : func.inputArgs) {
					f.implement += "\t\t\t" + arg.type + " " + arg.name + ";\n";
				}
			}
			if (!func.inputArgs.empty()) {
				f.implement += "\t\t\ts";
				for (auto arg : func.inputArgs) {
					f.implement += " >> " + arg.name;
				}
				f.implement += ";\n";
				f.implement += "\n";
			}
			if (!func.outputArgs.empty()) {
				for (auto arg : func.outputArgs) {
					f.implement += "\t\t\t" + arg.type + " " + arg.name + ";\n";
				}
			}
			f.implement += "\n";

			f.implement += "\t\t\t";
			if (func.returnType != "void") {
				f.implement += "ret = ";
			}
			f.implement += func.functionName + "_implement(";
			f.implement += "sessionID";
			if (!func.inputArgs.empty()) {
				for (int i = 0; i < func.inputArgs.size(); i++) {
					f.implement += ", " + func.inputArgs[i].name;
				}
			}
			if (!func.outputArgs.empty()) {
				for (auto arg : func.outputArgs) {
					f.implement += ", " + arg.name;
				}
			}
			f.implement += ");\n";
			f.implement += "\n";

			f.implement += "\t\t\toutputs << (short)" + to_string(func.ID);
			if (func.returnType != "void") {
				f.implement += " << ret";
			}
			if (!func.outputArgs.empty()) {
				for (auto arg : func.outputArgs) {
					f.implement += " << " + arg.name;
				}
			}
			f.implement += ";\n";
			f.implement += "\n";
			f.implement += "\t\t}\n";
			f.implement += "\t\tbreak;\n";
			f.implement += "\n";
		}
	}
	for (auto RPCf : newScript)
	{
		for (auto func : RPCf.second.functions)
		{
			parameter p;
			f.implement += "\t\tcase " + to_string(func.ID) + " :\n";
			f.implement += "\t\t{\n";
			if (func.returnType != "void") {
				f.implement += "\t\t\t" + func.returnType + " ret;\n";
			}
			if (!func.inputArgs.empty()) {
				for (auto arg : func.inputArgs) {
					f.implement += "\t\t\t" + arg.type + " " + arg.name + ";\n";
				}
			}
			if (!func.inputArgs.empty()) {
				f.implement += "\t\t\ts";
				for (auto arg : func.inputArgs) {
					f.implement += " >> " + arg.name;
				}
				f.implement += ";\n";
				f.implement += "\n";
			}
			if (!func.outputArgs.empty()) {
				for (auto arg : func.outputArgs) {
					f.implement += "\t\t\t" + arg.type + " " + arg.name + ";\n";
				}
			}
			f.implement += "\n";

			f.implement += "\t\t\t";
			if (func.returnType != "void") {
				f.implement += "ret = ";
			}
			f.implement += func.functionName + "_implement(";
			f.implement += "sessionID";
			if (!func.inputArgs.empty()) {
				for (int i = 0; i < func.inputArgs.size(); i++) {
					f.implement += ", " + func.inputArgs[i].name;
				}
			}
			if (!func.outputArgs.empty()) {
				for (auto arg : func.outputArgs) {
					f.implement += ", " + arg.name;
				}
			}
			f.implement += ");\n";
			f.implement += "\n";

			f.implement += "\t\t\toutputs << (short)" + to_string(func.ID);
			if (func.returnType != "void") {
				f.implement += " << ret";
			}
			if (!func.outputArgs.empty()) {
				for (auto arg : func.outputArgs) {
					f.implement += " << " + arg.name;
				}
			}
			f.implement += ";\n";
			f.implement += "\n";
			f.implement += "\t\t}\n";
			f.implement += "\t\tbreak;\n";
			f.implement += "\n";
		}
	}
	f.implement += "\t}\n";
	f.implement += "}\n";
	addFunction(f);

	writeCpp(fileName);
}