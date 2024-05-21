#ifndef DOFEPROJECT_LIBRARY_H
#define DOFEPROJECT_LIBRARY_H

#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include <map>
#include <iostream>
#include <vector>
#include "../utils/utils.h"

using json = nlohmann::json;

size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *buffer);
std::map<std::string, json> getBookByISBN(const char* isbn_code);
void displayVector(const std::vector<ActionType>& vectorToDisplay);
void evalLogin();
void goHome();
void evaluateAction(int number, sqlite3* db);
void startSystem();
int getAction(bool intro = true, bool logup = false, bool bookPage = false, bool home_quit = false);

#endif //DOFEPROJECT_LIBRARY_H
