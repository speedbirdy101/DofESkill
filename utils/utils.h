#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Create a data type for the object of the action list
struct ActionType {
    int number{};
    string description;
};

// Declare the external variables for login details
extern int pin;
extern string name;
extern string usrName;
extern bool devMode;

// Declare the external variable for MainActions
extern vector<ActionType> MainActions;

bool isInt(const std::string& s);
bool isFourDigitInteger(const std::string& str);
void SetupDevMode();
std::vector<std::string> splitString(const std::string& str, char delimiter);

#endif // UTILS_H
