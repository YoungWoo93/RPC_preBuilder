#include "cppClientCodeMaker.h"

#include <iostream>

using namespace std;

void cppClientCodeMaker::make(RPCscript& script)
{
	makeHeader(script);
	makeCpp(script);
}

void cppClientCodeMaker::makeHeader(RPCscript& script)
{
	string fileName = "RPC_client_callback";

	clear();
	addPreprocessing("#pragma once");
	commonInit();

	for (auto RPCf : script) {
		addInclude(include(dir + fileName + "_" + RPCf.second.name + ".h", false));
	}
	addInclude(include("RemoteProcedureCall/RemoteProcedureCall/client.h", false));

	for (auto RPCf : script)
	{
		for (auto func : RPCf.second.functions)
		{
			//parameter p;
			//for (auto arg : func.inputArgs) {
			//	p.push_back(arg);
			//}

			parameter p(func.inputArgs.begin(), func.inputArgs.end());
			function f("void", func.functionName, p);
			addFunction(f);
		}
		
	}

	parameter p;
	p.push_back(argument("serializer&", "s"));
	function f("void", "RPC_callback_table", p);
	addFunction(f);

	writeHeader(fileName);
}

void cppClientCodeMaker::makeCpp(RPCscript& script)
{
	string fileName = "RPC_client_callback";

	clear();
	addInclude(include(dir + fileName + ".h", false));
	addGlobalVariable(argument("extern RPCclient*", "rpcClient"));

	for (auto RPCf : script)
	{
		for (auto func : RPCf.second.functions)
		{
			parameter p(func.inputArgs.begin(), func.inputArgs.end());
			function f("void", func.functionName, p);
			f.implement += "{\n";
			f.implement += "\tpacket sendBuffer;\n";
			f.implement += "\n";
			f.implement += "\t*(sendBuffer.buffer) << (short)" + to_string(func.ID);
			for (auto arg : p) {
				f.implement += " << ";
				f.implement += arg.name;
			}
			f.implement += ";\n";
			f.implement += "\tsendBuffer.fillHeader(packetHeader(sendBuffer.buffer->size()));\n";
			f.implement += "\n";
			f.implement += "\trpcClient->SendPacket(sendBuffer);\n";
			f.implement += "}\n";
			f.implement += "\n";
			addFunction(f);
		}
	}

	parameter p;
	p.push_back(argument("serializer&", "s"));
	function f("void", "RPC_callback_table", p);
	f.implement += "{\n";
	f.implement += "\tshort id;\n";
	f.implement += "\ts >> id;\n";
	f.implement += "\n";
	f.implement += "\tswitch(id)\n";
	f.implement += "\t{\n";
	for (auto RPCf : script)
	{
		for (auto func : RPCf.second.functions)
		{
			parameter p;
			f.implement += "\t\tcase " + to_string(func.ID) + " :\n";
			f.implement += "\t\t{\n";
			if (func.returnType != "void") {
				f.implement += "\t\t\t" + func.returnType + " ret;\n";
				f.implement += "\t\t\ts >> ret;\n";
			}
			if (!func.outputArgs.empty()) {
				for (int i = 0; i < func.outputArgs.size(); i++) {
					f.implement += "\t\t\t" + func.outputArgs[i].type + " " + func.outputArgs[i].name + ";\n";
				}
			}

			f.implement += "\t\t\t" + func.functionName + "_callback(";
			if (func.returnType != "void") {
				f.implement += "ret";
			}
			if (!func.outputArgs.empty()) {
				f.implement += ", " + func.outputArgs[0].name;
				for (int i = 1; i < func.outputArgs.size(); i++) {
					f.implement += ", " + func.outputArgs[i].name;
				}
			}
			f.implement += ");\n";
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