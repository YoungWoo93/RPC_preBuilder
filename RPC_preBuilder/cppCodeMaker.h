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

//기본 공통 코드 추가
void outFileInit(ofstream& file);

// 직렬화 버퍼에 패킹해서 전송하는 부분
void makeClientRPCheaderFunction(ofstream& clientFile, vector<RPCfunction>& functions);

// 이후 RPC 응답이 왔을때 실행될 함수 구현 틀
void makeClientRPCheader_callbackFunction(ofstream& clientFile, vector<RPCfunction>& functions);

// (+ RPC 코어에서 응답이 온 직렬화 버퍼를 넘겨줬을때, 그걸 까서 어떤 콜백을 실행 시켜야 하는지 분기를 타는 코드 추가되어야함)
void makeClientRPCheaderTable(ofstream& clientFile, map<string, vector<RPCfunction>>& functions);


// RPC 코어에서 요청이 온 직렬화 버퍼를 받았을때, 그걸 까서 어떤 implement 함수를 실행시켜야 하는지 분기를 타는 코드 + 그 결과를 다시 시리얼라이저로 패킹해서 코어로 넘겨주는 코드가 추가되어야 함
void makeServerRPCheaderFunction(ofstream& serverFile, map<string, vector<RPCfunction>>& surmmary);

// RPC 요청이 왔을 때, 서버단에서 실행할 동작을 구현하는 틀이 추가되어야 함
void makeServerRPCheader_implementFunction(ofstream& serverFile, vector<RPCfunction>& functions);
