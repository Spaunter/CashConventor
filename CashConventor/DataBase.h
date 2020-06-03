#pragma once
#include"Header.h"


class DataBase
{
public:
	DataBase(const char* dir) :dir(dir) {}
	const char* getDir() { return this->dir; }
	void setDir(const char* dir) { this->dir = dir; }
	int createBD();
	int createTable(string sqlCommand);
	int insertData(string sqlCommand);
	int callback(void* NotUsed, int argc, char** argv, char** azColName);
	int selectData(string sqlCommand);
	
private:

	const char* dir;
};

