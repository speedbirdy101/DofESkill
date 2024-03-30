#include <iostream>
#include "sqlite3.h"
#include "utils.h"

using namespace std;

vector<ActionType> Actions;
int pin;
string name;
string usrName;
bool devMode = true; // You can provide an initial value if needed

bool isInt(const std::string& s) {
    size_t i = 0;

    // Skip leading whitespaces
    while (i < s.length() && isspace(s[i]))
        i++;

    // Check for optional sign
    if (i < s.length() && (s[i] == '+' || s[i] == '-'))
        i++;

    // Check for digits
    bool hasDigits = false;
    while (i < s.length() && isdigit(s[i])) {
        hasDigits = true;
        i++;
    }

    // Check if there are no non-digit characters after skipping whitespaces and optional sign
    return i == s.length() && hasDigits;
}

bool isFourDigitInteger(const std::string& str) {
    // Check if the length of the string is exactly 4
    if (str.length() != 4) {
        return false;
    }

    try {
        // Attempt to convert the string to an integer
        int number = std::stoi(str);

        // Check if the integer falls within the range of 1000 to 9999
        return (number >= 1000 && number <= 9999);
    } catch (const std::invalid_argument&) {
        // Conversion failed due to invalid argument (not an integer)
        return false;
    } catch (const std::out_of_range&) {
        // Conversion failed due to out of range (integer too large)
        return false;
    }
}

vector<int> reorderVector(const vector<int>& originalVector, int valueToSort) {
    vector<pair<int, int>> indexedValues;
    for (size_t i = 0; i < originalVector.size(); ++i) {
        indexedValues.push_back(make_pair(originalVector[i], i));
    }

    sort(indexedValues.begin(), indexedValues.end(),
         [&valueToSort](const pair<int, int>& a, const pair<int, int>& b) {
             if (a.first == valueToSort) return false;
             if (b.first == valueToSort) return true;
             return a.first < b.first;
         });

    vector<int> reorderedVector;
    for (const auto& pair : indexedValues) {
        reorderedVector.push_back(pair.first);
    }

    return reorderedVector;
}

bool compareActionType(const ActionType& a, const ActionType& b) {
    return a.number < b.number;
}

void SetupDevMode() {
    if (devMode) {
        usrName = "rr";
        pin = 1234;
    }
}

void SortActions() {
    sort(Actions.begin(), Actions.end(), compareActionType);
}

