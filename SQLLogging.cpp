#include "SQLLogging.h"

sqlite3     *SQLLogging::database = NULL;
const char  *SQLLogging::path = NULL;

void SQLLogging::InitSQLLogging(const char *path) {
    SQLLogging::path = path;
    OpenLog();
    CreateLogTable();
    CloseLog();
}
void SQLLogging::OpenLog() {
    if(sqlite3_open(path, &database) != 0)
        perror("Failed to open database");
}
void SQLLogging::CloseLog() {
    sqlite3_close(database);
}
void SQLLogging::CreateLogTable() {
    OpenLog();
    if (sqlite3_exec(database, "CREATE TABLE CHATLOG (DATE VARCHAR(32), NICKNAME VARCHAR(32), CHANNEL VARCHAR(32), LOG TEXT)", NULL, NULL, NULL))
        perror("Failed to create table...");
    CloseLog();
}
void SQLLogging::DropLogTable() {
    OpenLog();
    sqlite3_exec(database, "DROP TABLE CHATLOG", NULL, NULL, NULL);
    CloseLog();
}
void SQLLogging::ClearLogTable() {
    OpenLog();
    sqlite3_exec(database, "DELETE FROM CHATLOG", NULL, NULL, NULL);
    CloseLog();
}

void SQLLogging::AddLog(const char *date, const char *name, const char *channel, const char *log) {
    OpenLog();
    stringstream sstr;
    sstr << "INSERT INTO CHATLOG (DATE, NICKNAME, CHANNEL, LOG) VALUES ('"<< date <<"','"<< name <<"','"<< channel <<"','"<< log <<"')";

    if (sqlite3_exec(database, sstr.str().c_str(), NULL, NULL, NULL))
        perror("Failed to insert...");
	CloseLog();
}
string SQLLogging::LastSeen(const char *name) {
    OpenLog();
    sqlite3_stmt *stmt;

	stringstream sstr;
	sstr << "SELECT * FROM CHATLOG WHERE NICKNAME = '" << name << "' ORDER BY DATE DESC LIMIT 1";
	sqlite3_prepare(database, sstr.str().c_str(), -1, &stmt, NULL);

    struct  tm   *timeinfo;
    time_t  t;

	stringstream sstr2;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        t = sqlite3_column_int(stmt, 0);
	    timeinfo = localtime(&t);
		sstr2 << asctime(timeinfo) << (char*)sqlite3_column_text(stmt, 1) << " " << endl;
	} sqlite3_finalize(stmt);

    CloseLog();
	return sstr2.str();
}
string SQLLogging::GetLog() {
    OpenLog();
    sqlite3_stmt *stmt;
    const char *tail;

    struct  tm   *timeinfo;
    time_t  t;

    if (sqlite3_prepare_v2(database, "SELECT * FROM CHATLOG LIMIT 20", 1000, &stmt, &tail))
        CreateLogTable();
	stringstream sstr;
    int i = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        t = sqlite3_column_int(stmt, 0);
        timeinfo = localtime(&t);
        string strtime = asctime(timeinfo);
        strtime = strtime.erase(strtime.length()-1);

        sstr << strtime << " "
            << sqlite3_column_text(stmt, 1) << " "
			<< sqlite3_column_text(stmt, 2) << " "
			<< sqlite3_column_text(stmt, 3) << " || ";
	} sqlite3_finalize(stmt);
	CloseLog();
	return sstr.str();
}
