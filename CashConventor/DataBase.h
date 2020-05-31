#pragma once
#include"Header.h"

class DataBase
{
public:
	int createBD(const char* dir);
	int createTable(const char* dir);
	int insertData(const char* dir);
	int selectData(const char* s);
	int callback(void* NotUsed, int argc, char** argv, char** azColName);
private:

};

