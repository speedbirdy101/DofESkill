//
// Created by Robin Bhugun on 29/03/2024.
//

#ifndef DOFEPROJECT_UTILS_H
#define DOFEPROJECT_UTILS_H

#include <iostream>
#include <unordered_map>
#include <variant>
#include <string>
#include <vector>
using namespace std;

bool isFourDigitInteger(const std::string& str);

void evaluateAction(int number, sqlite3* db);

bool isInt(const string& s);

// Create a data type for the object of the action list
struct ActionType {
    int number{};
    string description;
};

// Define size changeable array of actions
extern vector<ActionType> Actions;

bool compareActionType(const ActionType& a, const ActionType& b);

// Define the login details
extern int pin;
extern string name;
extern string usrName;

// Setup Dev mode
extern bool devMode;

void SetupDevMode();

void SortActions();


#endif //DOFEPROJECT_UTILS_H
