#include "DataBase.h"

int DataBase::createBD()
{
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open(this->dir, &DB);
	sqlite3_close(DB);

	return exit;
}

int DataBase::createTable()
{
	return 0;
}

int DataBase::insertData()
{
	return 0;
}

int DataBase::selectData()
{
	return 0;
}

int DataBase::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	return 0;
}
