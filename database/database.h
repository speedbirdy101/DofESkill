
#ifndef DOFEPROJECT_DATABASE_H
#define DOFEPROJECT_DATABASE_H

extern sqlite3* database;

int loadDB(sqlite3*& db, const char* filename);

static int checkUsernameExistsCallback(void* usernameExists, int argc, char** argv, char** azColName);

bool checkUsernameExists(const string& usrname);

static int checkPswdExistsCallback(void* pswdExists, int argc, char** argv, char** azColName);

bool checkPswdExists(const string& usrname, string pin_ent);

static int addAccountCallback(void* pswdExists, int argc, char** argv, char** azColName);

void addAccount(const string& username, int password, const string& name_);

void takeOutBook(string isbn10, string isbn13, string name);

static int takeOutBookCallback(void* pswdExists, int argc, char** argv, char** azColName);

static int checkIfBookOutCallback(void* bookOut, int argc, char** argv, char** azColName);

bool checkIfBookOut(string bookISBN);

void returnBook(const string& isbn10, const string& isbn13);

bool checkUsrHasBook(string bookISBN);

static int checkUsrHasBookCallback(void* holderUsrName, int argc, char** argv, char** azColName);

static int returnBookCallback(void* pswdExists, int argc, char** argv, char** azColName);

vector<json> getUsrTakingsOut();

static int usrTakingsOutCallback(void* takingsOut, int argc, char** argv, char** azColName);

#endif //DOFEPROJECT_DATABASE_H
