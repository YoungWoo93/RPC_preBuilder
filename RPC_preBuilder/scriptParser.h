#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <algorithm>

#include <Windows.h>


#include "RPC_preBuilder.h"

using namespace std;

void whiteSpaceCleanup(string& line);

bool getType(string& line, string::iterator& iter, string& type);
bool getName(string& line, string::iterator& iter, string& name);

bool getBracketBlock(string& line, string::iterator& iter, string& block);
bool argsParser(string& args, unordered_set<string>& preRegistArgNames, RPCparameter& parsedArgs);

int lineParsing(string& line, string& returnType, string& functionName, RPCparameter& inputArgs, RPCparameter& outputArgs);