#include "DataBase.h"

int DataBase::createBD()
{
	sqlite3* db;
	int result = 0;
	result = sqlite3_open(this->dir, &db);
	sqlite3_close(db);

	return result;
}

int DataBase::createTable(string sql)
{
	sqlite3* db;
	
	try
	{
		int result = 0;
		result = sqlite3_open(this->dir, &db);
		char* massageError{};
		result = sqlite3_exec(db, sql.c_str(), NULL, 0, &massageError);

		if (result != SQLITE_OK)
		{
			cerr << "Error Create table" << endl;
			sqlite3_free(massageError);
		}
		else
		{
			cout << "Table was created" << endl;
		}

		sqlite3_close(db);
	}
	catch (const exception & e)
	{
		cerr << e.what();
	}
	return 0;
}

int DataBase::insertData(string sqlCommand)
{
	sqlite3* db;
	char* massageError;

	int exit = sqlite3_open(this->dir, &db);

	exit = sqlite3_exec(db, sqlCommand.c_str(), NULL, 0, &massageError);

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

int DataBase::select_callback(void* p_data, int num_fields, char** p_fields, char** p_col_names)
{
	Records* records = static_cast<Records*>(p_data);
	try {
		records->emplace_back(p_fields, p_fields + num_fields);
	}
	catch (...) {
		// abort select on failure, don't let exception propogate thru sqlite3 call-stack
		return 1;
	}
	return 0;
}

Records DataBase::select_stmt(string sqlCommand)
{
	sqlite3* db;
	Records records;
	char* errmsg;
	int ret = sqlite3_exec(db, sqlCommand.c_str(), select_callback, &records, &errmsg);
	if (ret != SQLITE_OK) {
		cerr << "Error in select statement " << sqlCommand.c_str() << "[" << errmsg << "]\n";
	}
	else {
		cerr << records.size() << " records returned.\n";
	}

	return records;
}

//int DataBase::selectData(string sqlCommand)
//{
//	sqlite3* db;
//	int exit = sqlite3_open(this->dir, &db);
//
//	sqlite3_exec(db, sqlCommand.c_str(), select_callback, NULL, NULL);
//
//	return 0;
//}


