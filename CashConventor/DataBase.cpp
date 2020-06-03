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

int DataBase::insertData(string sqlCommand)
{
	sqlite3* DB;
	char* massageError;

	int exit = sqlite3_open(this->dir, &DB);

	exit = sqlite3_exec(DB, sqlCommand.c_str(), NULL, 0, &massageError);

	if (exit != SQLITE_OK)
	{
		cerr << "Error insert" << endl;
		sqlite3_free(massageError);
	}
	else
	{
		cout << "all_ok> insert" << endl;
	}

	return 0;
}

int DataBase::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		cout << azColName[i] << " : " << argv[i] << endl;
	}
	cout << endl;

	return 0;
}

//int DataBase::selectData(string sqlCommand)
//{
//	sqlite3* DB;
//
//
//	int exit = sqlite3_open(this->dir, &DB);
//
//	string sql = "SELECT * FROM RATE_HIST_UA;";
//
//	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
//
//	return 0;
//}


