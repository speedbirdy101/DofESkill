#include <iostream>
#include "utils.h"
#include <sstream>

using namespace std;

// Define the login detail variables
int pin;
string name;
string usrName;
bool devMode = true; // You can provide an initial value if needed

// Define and initialize the MainActions vector
vector<ActionType> MainActions = {
        {0, "Quit Process"},
        {1, "Log Out"},
        {2, "Find book by ISBN Code"},
        {3, "View Your Taken Out Books"}
};

bool isInt(const std::string& s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool isFourDigitInteger(const std::string& str) {
    if (str.length() != 4) {
        return false;
    }
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

void SetupDevMode() {
    if (devMode) {
        usrName = "rr";
        pin = 1234;
    }
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
