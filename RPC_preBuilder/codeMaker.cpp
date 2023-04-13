#include "codeMaker.h"


codeMakerFile::codeMakerFile(const string& _dir)
{
	dir = _dir + '\\';
}

void codeMakerFile::clear()
{
	preprocessingBlock.clear();
	libraryBlock.clear();
	includeBlock.clear();
	functionsBlock.clear();
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool codeMakerFile::addPreprocessing(const string& _line)
{
	for (auto p : preprocessingBlock) {
		if (p == _line)
			return false;
	}

	preprocessingBlock.push_back(_line);
	return true;
}
bool codeMakerFile::removePreprocessing(const string& _line)
{
	for (auto it = preprocessingBlock.begin(); it != preprocessingBlock.end(); it++) {
		if (*it == _line)
		{
			preprocessingBlock.erase(it);
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool codeMakerFile::addLibrary(const library& _lib)
{
	for (auto p : libraryBlock) {
		if (p == _lib)
			return false;
	}

	libraryBlock.push_back(_lib);
	return true;
}
bool codeMakerFile::removeLibrary(const library& _lib)
{
	for (auto it = libraryBlock.begin(); it != libraryBlock.end(); it++) {
		if (*it == _lib)
		{
			libraryBlock.erase(it);
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool codeMakerFile::addInclude(const include& _header)
{
	for (auto p : includeBlock) {
		if (p == _header)
			return false;
	}

	includeBlock.push_back(_header);
	return true;
}
bool codeMakerFile::removeInclude(const include& _header)
{
	for (auto it = includeBlock.begin(); it != includeBlock.end(); it++) {
		if (*it == _header)
		{
			includeBlock.erase(it);
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool codeMakerFile::addFunction(const function& _func)
{
	for (auto& p : functionsBlock) {
		if (p == _func) {
			p.flag = true;
			return false;
		}
	}

	functionsBlock.push_back(_func);
	functionsBlock.back().flag = true;
	return true;
}
bool codeMakerFile::removeFunction(const function& _func)
{
	return true;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool codeMakerFile::addGlobalVariable(const argument& _var)
{
	for (auto& a : globalVariableBlock) {
		if (a == _var) {
			return false;
		}
	}

	globalVariableBlock.push_back(_var);
	return true;
}

bool codeMakerFile::removeGlobalVariable(const argument& _var)
{
	for (auto it = globalVariableBlock.begin(); it != globalVariableBlock.end(); it++) {
		if (*it == _var)
		{
			globalVariableBlock.erase(it);
			return true;
		}
	}

	return false;
}