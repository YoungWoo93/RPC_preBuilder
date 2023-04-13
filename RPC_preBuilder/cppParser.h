#pragma once

#include <string>
#include <vector>

using namespace std;

bool getCommentsCpp(string& line);

bool getTypeCpp(string& line, string::iterator& iter, string& type);
bool getNameCpp(string& line, string::iterator& iter, string& name);

bool getBracketBlockCpp(string& line, string::iterator& iter, string& block);
bool argsParserCpp(string& args, vector<pair<string, string>>& parsedArgs);