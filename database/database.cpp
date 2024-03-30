#include "sqlite3.h"
#include "iostream"
#include "../library/library.h"
#include "../utils/utils.h"
#include "database.h"
#include<unistd.h>
using namespace std;

sqlite3* database;

// Connect to the DB
int loadDB(sqlite3*& db, const char* filename) {
    int rc;
    rc = sqlite3_open("/Users/robin/Desktop/DofE/Evidence/Skills/DofeProject/library.db", &db);

    if (rc) {
        cerr << "Error open DB " << sqlite3_errmsg(db) << endl;
        return (-1);
    }
    else {
        cout << "[LOG] Opened Database Successfully" << endl;
        return (0);
    }
}


static int checkUsernameExistsCallback(void* usernameExists, int argc, char** argv, char** azColName) {
    // Cast the pointer to a bool pointer
    bool* exists = static_cast<bool*>(usernameExists);

    // If there is at least one row, set the flag to true
    bool found = false;
    for (int i = 0; i < argc; i++) {
        // Compare column names using strcmp (string compare)
        found = true;
    }
    *exists = found;

    // We don't need to process further rows, so return non-zero value
    return 0;
}

bool checkUsernameExists(const string& usrname) {
    int rc;
    char* zErrMsg = nullptr;
    bool usernameExists = false; // Flag to indicate if the username exists

    string sql_str = "SELECT * FROM accounts WHERE username='" + usrname + "'";
    const char* sql_char = sql_str.c_str();

    rc = sqlite3_exec(database, sql_char, checkUsernameExistsCallback, &usernameExists, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    // Return the flag indicating whether the username exists
    return usernameExists;
}

static int checkPswdExistsCallback(void* pswdExists, int argc, char** argv, char** azColName) {
    // Cast the pointer to a bool pointer
    bool* exists = static_cast<bool*>(pswdExists);

    // If there is at least one row, set the flag to true
    bool found = false;
    for (int i = 0; i < argc; i++) {
        // Compare column names using strcmp (string compare)
        found = true;
    }
    *exists = found;

    // We don't need to process further rows, so return non-zero value
    return 0;
}

bool checkPswdExists(const string& usrname, string pin_ent) {
    int rc;
    char* zErrMsg = nullptr;
    bool pinExists = false; // Flag to indicate if the username exists

    string sql_str = "SELECT * FROM accounts WHERE username='" + usrname + "' AND pin=" + pin_ent;
    const char* sql_char = sql_str.c_str();

    rc = sqlite3_exec(database, sql_char, checkPswdExistsCallback, &pinExists, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    // Return the flag indicating whether the username exists
    return pinExists;
}

static int addAccountCallback(void* pswdExists, int argc, char** argv, char** azColName) {return 0;}

void addAccount(const string username, const int password, const string name_) {
    int rc;
    char* zErrMsg = nullptr;
    bool pinExists = false; // Flag to indicate if the username exists

    string sql_str = "INSERT INTO accounts (pin, name, username) VALUES (" + to_string(password) + ",'" + name_ + "','" + username + "')";
    const char* sql_char = sql_str.c_str();

    rc = sqlite3_exec(database, sql_char, addAccountCallback, &pinExists, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}

static int takeOutBookCallback(void* pswdExists, int argc, char** argv, char** azColName) {
    return 0;
}

static int checkIfBookOutCallback(void* bookOut, int argc, char** argv, char** azColName) {
    // Cast the pointer to a bool pointer
    bool* out = static_cast<bool*>(bookOut);

    // If there is at least one row, set the flag to true
    bool found = false;
    for (int i = 0; i < argc; i++) {
        // Compare column names using strcmp (string compare)
        found = true;
    }
    *out = found;

    // We don't need to process further rows, so return non-zero value
    return 0;
}

bool checkIfBookOut(string bookISBN) {
    int rc;
    char* zErrMsg = nullptr;
    bool bookOut = false; // Flag to indicate if the username exists
    cout << "hhh";
    string sql_str = "SELECT * FROM books_out WHERE isbn10='" + bookISBN + "'";
    const char* sql_char = sql_str.c_str();

    rc = sqlite3_exec(database, sql_char, checkIfBookOutCallback, &bookOut, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }


    return bookOut;
}

void takeOutBook(const string isbn10, const string isbn13) {
    // Check if book is already out
    if (!checkIfBookOut(isbn10) || !checkIfBookOut(isbn13)) {
        int rc;
        char* zErrMsg = nullptr;
        bool pinExists = false; // Flag to indicate if the username exists

        // Get current local time
        auto now = chrono::system_clock::now();
        auto time_point = chrono::system_clock::to_time_t(now);
        string timestamp = std::ctime(&time_point); // Convert to string


        string sql_str = "INSERT INTO books_out (isbn10, isbn13, takenby_usr, taken_on) VALUES (" + isbn10 + "," + isbn13 + ",'" + usrName + "','" + timestamp + "')";
        const char* sql_char = sql_str.c_str();

        rc = sqlite3_exec(database, sql_char, takeOutBookCallback, &pinExists, &zErrMsg);

        if (rc != SQLITE_OK) {
            cerr << "SQL error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg);
        }

        system("clear");
        cout << "The book was successfully taken out!\n";
        while (true) {
            int nextAction = getAction(false, false, false, true);
            if (nextAction == 1) {
                break;
            } else if (nextAction == 2) {
                return;
            } else {
                system("clear");
                continue;
            }
        }
        goHome();
    } else {
        system("clear");
        cout << "Unfortunately this book is already out, please try again soon\n";

        bool firstLoop = true;
        while (true) {
            if (!firstLoop) {
                cout << "Invalid: Please select an option for the options below." << endl;
            }
            int nextAction = getAction(false, false, false, true);
            if (nextAction == 1) {
                break;
            } else if (nextAction == 2) {
                return;
            } else {
                system("clear");
                firstLoop = false;
                continue;
            }
        }
        goHome();
    }
}

static int checkUsrHasBookCallback(void* holderUsrName, int argc, char** argv, char** azColName) {
    // Cast the pointer to a bool pointer
    bool* out = static_cast<bool*>(holderUsrName);

    // If there is at least one row, set the flag to true
    bool found = false;
    for (int i = 0; i < argc; i++) {
        // Compare column names using strcmp (string compare)
        found = true;
    }
    *out = found;

    // We don't need to process further rows, so return non-zero value
    return 0;
}

bool checkUsrHasBook(string bookISBN) {
    int rc;
    char* zErrMsg = nullptr;
    bool holderUsrName; // Flag to indicate if the username exists

    string sql_str = "SELECT * FROM books_out WHERE isbn10='" + bookISBN + "' AND " + "takenby_usr='" + usrName + "'";
    const char* sql_char = sql_str.c_str();

    rc = sqlite3_exec(database, sql_char, checkUsrHasBookCallback, &holderUsrName, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    return holderUsrName;
}

void returnBook(string bookISBN) {
    cout << checkIfBookOut(bookISBN);
    if (checkIfBookOut(bookISBN) && checkUsrHasBook(bookISBN)) {
        cout << "done";
        sleep(100);
    } else {
        system("clear");
        cout << "You currently do not have this book out.\n";
        while (true) {
            int nextAction = getAction(false, false, false, true);
            if (nextAction == 1) {
                break;
            } else if (nextAction == 2) {
                return;
            } else {
                system("clear");
                continue;
            }
        }
        goHome();
    }
}
// 9781447988090