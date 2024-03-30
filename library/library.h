//
// Created by Robin Bhugun on 26/03/2024.
//

#ifndef DOFEPROJECT_LIBRARY_H
#define DOFEPROJECT_LIBRARY_H

#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include "map"
#include <cstdlib> // for system()
#include <iostream>
#include <vector>
using json = nlohmann::json;

using namespace std;

size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *buffer);

map<std::string, json> getBookByISBN(const char* isbn_code);

void loadActions(sqlite3* db);

void evalLogin();

void goHome();

void evaluateAction(int number, sqlite3* db);

void startSystem();

int getAction(bool intro = true, bool logup = false, bool bookPage = false, bool home_quit = false);

#endif //DOFEPROJECT_LIBRARY_H
