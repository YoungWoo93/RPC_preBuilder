#include "scriptParser.h"


map<string, string> dataTypeMap{
	{"void", "void"},

	{"bool", "bool"},
	{"char", "char"},
	{"uchar", "unsigned char"},
	{"wchar_t", "wchar_t"},

	{"short", "short"},
	{"int", "int"},
	{"int32", "int"},
	{"long long int", "long long int"},
	{"int64", "long long int"},

	{"unsigned short", "unsigned short"},
	{"unsigned int", "unsigned int"},
	{"unsigned long long int", "unsigned long long int"},
	{"uint", "unsigned int"},
	{"uint32", "unsigned int"},
	{"uint64", "unsigned long long int"},

	{"float", "float"},
	{"double", "double"},

	{"__int8", "__int8"},
	{"__int16", "__int16"},
	{"__int32", "__int32"},
	{"__int64", "__int64"},
	{"__int128", "__int128"},

	{"string", "std::string"},
	{"std::string", "std::string"}
};


void whiteSpaceCleanup(string& line)
{
	string temp = "";
	bool whiteSpaceFlag = false;

	for (auto c : line) {
		if (c == ' ' || c == '\t') {
			if (whiteSpaceFlag)
				continue;

			whiteSpaceFlag = true;
			temp += ' ';
		}
		else {
			whiteSpaceFlag = false;
			temp += c;
		}
	}

	line = temp;
}

bool getType(string& line, string::iterator& iter, string& type) {
	if (*iter == ' ')
		iter++;

	string temp;
	while (iter != line.end()) {
		if (*iter <= -1 || 256 <= *iter)
			break;
		if (*iter == ' ') {
			if (temp == "long" || temp == "unsigned")
			{
				type += temp + " ";
				temp.clear();
			}
			else
				break;
		}
		else
			temp += tolower(*iter);
		iter++;
	}
 	type += temp;

	if (dataTypeMap.find(type) != dataTypeMap.end()) {
		type = dataTypeMap[type];
		return true;
	}

	return false;
}

bool getName(string& line, string::iterator& iter, string& name) {
	if (*iter == ' ')
		iter++;

	while (iter != line.end()) {
		if (*iter <= -1 || 256 <= *iter)
			break;
		if (isalpha(*iter) || isdigit(*iter) || *iter == '_') {
			name += *iter;
			iter++;
		}
		else if (*iter == ')' || *iter == '(' || *iter == ' ' || *iter == ',')
			break;
		else
			return false;
	}

	if (!name.empty())
		return true;

	return false;
}

bool getBracketBlock(string& line, string::iterator& iter, string& block) {
	while (iter != line.end()) {
		if (*iter <= -1 || 256 <= *iter)
			break;
		if (*iter == ' ')
			iter++;
		else
			break;
	}

	block += '(';
	if (*iter == '(')
		*iter++;

	while (iter != line.end()) {
		if (*iter <= -1 || 256 <= *iter)
			break;
		if (isalpha(*iter) || isdigit(*iter) || *iter == '_' || *iter == ' ' || *iter == ',' || *iter == '*' || *iter == '&' || *iter == ':') {

			block += *iter;
			iter++;
		}
		else if (*iter == ')') {
			block += *iter;
			iter++;
			break;
		}
		else
			return false;
	}

	return true;
}

bool argsParser(string& args, unordered_set<string>& preRegistArgNames, RPCparameter& parsedArgs) {
	auto iter = args.begin();
	iter++;

	while (iter != args.end()) {
		if (*iter <= -1 || 256 <= *iter)
			break;
		if (*iter == ')')
			break;

		string type;
		string name;

		if (!getType(args, iter, type))
			return false;
		if (!getName(args, iter, name))
			return false;

		if (preRegistArgNames.find(name) == preRegistArgNames.end()) {
			preRegistArgNames.insert(name);
			parsedArgs.push_back({ type, name });
			iter++;
		}
		else
			return false;
	}

	return true;
}

int lineParsing(string& line, string& returnType, string& functionName, RPCparameter& inputArgs, RPCparameter& outputArgs)
{
	int state = 1;
	// state 1 : parsing return type
	// state 2 : parsing function name
	// state 3 : parsing input params
	// state 4 : parsing output params
	string::iterator iter = line.begin();

	unordered_set<string> preRegistArgNames;
	{
		if (!getType(line, iter, returnType))
			return state;
		state = 2;
	}
	{
		if (!getName(line, iter, functionName))
			return state;
		state = 3;
	}
	{
		string argsBlock;
		if (!getBracketBlock(line, iter, argsBlock))
			return state;

		if (!argsParser(argsBlock, preRegistArgNames, inputArgs))
			return state;

		preRegistArgNames.clear();
		state = 4;
	}
	{
		string argsBlock;
		if (!getBracketBlock(line, iter, argsBlock))
			return state;

		if (!argsParser(argsBlock, preRegistArgNames, outputArgs))
			return state;

		return 0;
	}
}
