#include "DataBase.h"

int DataBase::createBD()
{
	sqlite3* DB;
	int result = 0;
	result = sqlite3_open(this->dir, &DB);
	sqlite3_close(DB);

	return result;
}

int DataBase::createTable(string sql)
{
	sqlite3* DB;
	/*string sql = "CREATE TABLE IF NOT EXISTS RATE_HIST_UA("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"DATE_RATE DATE NOT NULL,"
		"CURRENCY TEXT,"
		"RATE NUMERIC);";*/
	try
	{
		int result = 0;
		result = sqlite3_open(this->dir, &DB);
		char* massageError{};
		result = sqlite3_exec(DB, sql.c_str(), NULL, 0, &massageError);

		if (result != SQLITE_OK)
		{
			cerr << "Error Create table" << endl;
			sqlite3_free(massageError);
		}
		else
		{
			cout << "Table was created" << endl;
		}

		sqlite3_close(DB);
	}
	catch (const exception & e)
	{
		cerr << e.what();
	}
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
