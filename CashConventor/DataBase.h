#pragma once
#include"Header.h"

using Record = vector<string>;
using Records = vector<Record>;

class DataBase
{
public:
	DataBase(const char* dir) :dir(dir) {}
	const char* getDir() { return this->dir; }
	void setDir(const char* dir) { this->dir = dir; }
	static int select_callback(void* p_data, int num_fields, char** p_fields, char** p_col_names);
	Records select_stmt(string sqlCommand);
	int createBD();
	int createTable(string sqlCommand);
	int insertData(string sqlCommand);
	//int selectData(string sqlCommand);
	
private:

	const char* dir;
};

