#pragma once
#include"Header.h"


class DataBase
{
public:
	DataBase(const char* dir) :dir(dir) {}
	const char* getDir() { return this->dir; }
	void setDir(const char* dir) { this->dir = dir; }
	int createBD();
	int createTable();
	int insertData();
	int selectData();
	int callback(void* NotUsed, int argc, char** argv, char** azColName);
private:

	const char* dir;
};

