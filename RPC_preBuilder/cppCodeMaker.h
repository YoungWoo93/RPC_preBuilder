#pragma once

#include <vector>

#include "RPC_preBuilder.h"
#include "codeMaker.h"

using namespace std;

class cppCodeMakerFile : public codeMakerFile {
public:
	cppCodeMakerFile(const string& _dir);
	virtual bool open(const string& _fileName);
	virtual bool writeHeader(const string& _fileName);
	virtual bool writeCpp(const string& _fileName);

protected:
	void commonInit();

private:
	virtual void parsingPreprocessing();
	virtual void parsingLibrary();
	virtual void parsingInclude();
	virtual void parsingFunction();

	virtual void writePreprocessing();
	virtual void writeLibrary();
	virtual void writeInclude();
	virtual void writeFunction();
	virtual void writeFunctionDeclaration();
	virtual void writeGlobalVariable();
};

//�⺻ ���� �ڵ� �߰�
void outFileInit(ofstream& file);

// ����ȭ ���ۿ� ��ŷ�ؼ� �����ϴ� �κ�
void makeClientRPCheaderFunction(ofstream& clientFile, vector<RPCfunction>& functions);

// ���� RPC ������ ������ ����� �Լ� ���� Ʋ
void makeClientRPCheader_callbackFunction(ofstream& clientFile, vector<RPCfunction>& functions);

// (+ RPC �ھ�� ������ �� ����ȭ ���۸� �Ѱ�������, �װ� � � �ݹ��� ���� ���Ѿ� �ϴ��� �б⸦ Ÿ�� �ڵ� �߰��Ǿ����)
void makeClientRPCheaderTable(ofstream& clientFile, map<string, vector<RPCfunction>>& functions);


// RPC �ھ�� ��û�� �� ����ȭ ���۸� �޾�����, �װ� � � implement �Լ��� ������Ѿ� �ϴ��� �б⸦ Ÿ�� �ڵ� + �� ����� �ٽ� �ø���������� ��ŷ�ؼ� �ھ�� �Ѱ��ִ� �ڵ尡 �߰��Ǿ�� ��
void makeServerRPCheaderFunction(ofstream& serverFile, map<string, vector<RPCfunction>>& surmmary);

// RPC ��û�� ���� ��, �����ܿ��� ������ ������ �����ϴ� Ʋ�� �߰��Ǿ�� ��
void makeServerRPCheader_implementFunction(ofstream& serverFile, vector<RPCfunction>& functions);
