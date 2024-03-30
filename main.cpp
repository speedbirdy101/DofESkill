#include "sqlite3.h"
#include <cstdlib> // for system()
#include "library/library.h"

#include "utils/utils.h"
#include "database/database.h"

using namespace std;

int main() {

    SetupDevMode();

    loadDB(database, "");

    system("clear");

    startSystem();

    return (0);
}