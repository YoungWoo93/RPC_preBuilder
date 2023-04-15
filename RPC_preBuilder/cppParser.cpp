#include "cppParser.h"

#include <map>
#include <string>
#include <iostream>

using namespace std;

map<string, string> cppDataTypeMap{
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
	{"std::string", "std::string"},
	{"serializer", "serializer"}
};

bool getCommentsCpp(string& line)
{
	auto iter = line.begin();
	while (iter != line.end() && (*iter == ' ' || *iter == '\t'))
		iter++;

	if (*iter == '/')
	{
		if (*(++iter) == '/')
			return true;
	}

	return false;
}

bool getTypeCpp(string& line, string::iterator& iter, string& type) {
	while(*iter == ' ' || *iter == '\t')
		iter++;

	string subType = "";
	while (iter != line.end()) {
		if (*iter <= -1 || 256 <= *iter)
			break;
		if (*iter == ' ')
		{
			type += subType;
			if (subType == "unsigned" || subType == "long" || subType == "static" || subType == "const")
			{
				subType.clear();
				type += ' ';
				iter++;
				continue;
			}
			else
				break;
		}

		subType += tolower(*iter);
		iter++;
	}

	string temp;
	while (type.back() == '*' || type.back() == '&')
	{
		temp += type.back();
		type.pop_back();
	}

	if (cppDataTypeMap.find(type) != cppDataTypeMap.end()) {
		type = cppDataTypeMap[type];
		if (!temp.empty())
			type.append(temp.rbegin(), temp.rend());
		return true;
	}

	return false;
}

bool getNameCpp(string& line, string::iterator& iter, string& name) {
	while (*iter == ' ' || *iter == '\t')
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

bool getBracketBlockCpp(string& line, string::iterator& iter, string& block) {
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
		if (isalpha(*iter) || isdigit(*iter) || *iter == '_' || *iter == ' ' || *iter == ',' || *iter == '*' || *iter == '&' || *iter == ':' || *iter == '/') {

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

bool argsParserCpp(string& args, vector<pair<string, string>>& parsedArgs){
	auto iter = args.begin();
	iter++;

	while (iter != args.end()) {
		if (*iter <= -1 || 256 <= *iter)
			break;
		if (*iter == ')')
			break;

		string type;
		string name;

		if (!getTypeCpp(args, iter, type))
			return false;
		if (!getNameCpp(args, iter, name))
			return false;

		parsedArgs.push_back({ type, name });
		iter++;
	}

	return true;
}