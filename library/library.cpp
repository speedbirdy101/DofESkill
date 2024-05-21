#include <iostream>
#include <vector>
#include "sqlite3.h"
#include <cstdlib> // for system()
#include "library.h"
#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include "map"
#include "../database/database.h"

using namespace std;
using json = nlohmann::json;

// Callback function to write received data into a string
size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *buffer) {
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}


map<std::string, json> getBookByISBN(const char* isbn_code) {
    CURL *curl;
    CURLcode res;
    std::string buffer; // To store the response

    // Initialize libcurl
    curl = curl_easy_init();
    if(curl) {
        // Set the URL you want to retrieve
        string url = "https://openlibrary.org/api/volumes/brief/isbn/" + std::string(isbn_code) + ".json";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to handle received data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        // Perform the request
        res = curl_easy_perform(curl);
        // Check for errors
        if(res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        // Clean up
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return {};
    }

    // Parse JSON response
    json responseData = json::parse(buffer);

    // Access data from JSON
    for (auto& element : responseData["records"].items()) {
        if (element.value().find("data") != element.value().end()) {
            auto& data = element.value();

            // Get info from other fields
            json dataToReturn = {};

            // Get the ISBN Codes
            for (const auto& isbnCode: data["isbns"]) {
                if (string(isbnCode).length() == 10) {
                    dataToReturn["isbn10"] = isbnCode;
                } else if (string(isbnCode).length() == 13) {
                    dataToReturn["isbn13"] = isbnCode;
                }
            }

            // Get the publishing dates
            if (!data["data"]["publish_date"].is_null()){
                dataToReturn["publishDates"] = data["data"]["publish_date"];
            } else if (!data["details"]["details"]["publish_date"].is_null()) {
                dataToReturn["publishDates"] = data["details"]["details"]["publish_date"];
            }

            // Get General data (title, authors, pages)
            if (!data["details"]["details"]["full_title"].is_null()){
                dataToReturn["title"] = data["details"]["details"]["full_title"];
            } else if (!data["data"]["title"].is_null()) {
                dataToReturn["title"] = data["data"]["title"];
            }

            if (!data["data"]["number_of_pages"].is_null()){
                dataToReturn["pages"] = data["data"]["number_of_pages"];
            } else if (!data["details"]["details"]["number_of_pages"].is_null()) {
                dataToReturn["pages"] = data["details"]["details"]["number_of_pages"];
            }
            dataToReturn["subjectLocations"] = data["data"]["subject_places"];
            dataToReturn["subjects"] = data["data"]["subjects"];
            dataToReturn["authors"] = data["data"]["authors"];
            dataToReturn["publishers"] = data["details"]["details"]["publishers"];

            if (dataToReturn["publishers"].is_null() || dataToReturn["publishers"].is_array() && dataToReturn["publishers"].empty()) {
                dataToReturn["publishers"] = data["data"]["publishers"];
            }

            if (dataToReturn["authors"].is_null() || dataToReturn["authors"].is_array() && dataToReturn["authors"].empty()) {
                dataToReturn["authors"] = data["data"]["authors"];
            }

            return dataToReturn;
        }
    }
    return {};
}


void displayVector(const vector <ActionType>& vectorToDisplay) {
    for (const auto& vec : vectorToDisplay) {
        cout << "(" + to_string(vec.number) + ") " + vec.description + "\n";
    }
}

int getAction(bool intro, bool logup, bool bookPage, bool home_quit) {
    int actionIntegar;

    if (intro && !logup && !bookPage && !home_quit) {
        displayVector(MainActions);
    } else if (!intro && logup && !bookPage && !home_quit) {
        cout << "Select an Option:\n(1) Login\n(2) Sign-Up" << endl;
    } else if (!intro && !logup && bookPage && !home_quit) {
        cout << "\nSelect an Option:\n(1) Go Home\n(2) Take Book Out \n(3) Return Book" << endl;
    } else if (!intro && !logup && !bookPage && home_quit) {
        cout << "Select an Option:\n(1) Go Home\n(2) Quit" << endl;
    }

    // Check for integer input

    while (!(cin >> actionIntegar)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        system("clear");
        cout << "Welcome to the Library Management system " << usrName << "!" << endl;
        cout << "Invalid input. Please enter an integer." << endl;

        if (intro && !logup && !bookPage && !home_quit) {
            displayVector(MainActions);
        } else if (!intro && logup && !bookPage && !home_quit) {
            cout << "Select an Option:\n(1) Login\n(2) Sign-Up" << endl;
        } else if (!intro && !logup && bookPage && !home_quit) {
            cout << "\nSelect an Option:\n(1) Go Home\n(2) Take Book Out \n(3) Return Book" << endl;
        } else if (!intro && !logup && !bookPage && home_quit) {
            cout << "Select an Option:\n(1) Go Home\n(2) Quit" << endl;
        }
    }

    return actionIntegar;
}

void evalLogin() {
    system("clear");
    cout << "Welcome to the Library Management system, you need to login or sign-up first." << endl;
    int actionIntegar;

    actionIntegar = getAction(false, true);

    bool goToPswd = false;

    if (actionIntegar == 1) {
        // Login
        system("clear");
        cout << "USERNAME:" << endl;

        string usr;
        cin >> usr;

        if (!checkUsernameExists(usr)) {
            while (!goToPswd) {
                system("clear");
                cout << "INCORRECT!\nENTER USERNAME AGAIN:" << endl;

                cin >> usr;
                if (checkUsernameExists(usr)) {
                    goToPswd = true;
                }
            }
        }

        system("clear");
        bool allowLogin = false;
        cout << "PIN:" << endl;

        string pin_entered;
        cin >> pin_entered;

        if (!checkPswdExists(usr, pin_entered)) {
            while (!allowLogin) {
                system("clear");
                cout << "INCORRECT!\nENTER PIN AGAIN:" << endl;


                cin >> pin_entered;
                if (checkPswdExists(usr, pin_entered)) {
                    allowLogin = true;
                }
            }
        }

        cout << "LOGGED IN!";
        usrName = usr;
        pin = stoi(pin_entered);


    } else if (actionIntegar == 2) {
        bool goToNewPswd = false;
        // Signup
        system("clear");
        cout << "ENTER YOUR CHOSEN USERNAME:" << endl;

        string usr;
        cin >> usr;

        if (checkUsernameExists(usr)) {
            while (!goToNewPswd) {
                system("clear");
                cout << "USERNAME ALREADY EXISTS!\nENTER A NEW USERNAME:" << endl;

                cin >> usr;
                if (!checkUsernameExists(usr)) {
                    goToNewPswd = true;
                }
            }
        }

        string pin_entered;
        system("clear");
        cout << "ENTER YOUR PIN:" << endl;

        cin >> pin_entered;
        while (!isFourDigitInteger(pin_entered)) {
            system("clear");
            cout << "PIN MUST BE AN INTEGAR & 4 DIGITS\nRE-ENTER PIN:" << endl;

            cin >> pin_entered;
        }

        system("clear");
        cout << "ENTER YOUR NAME:" << endl;

        string name_entered;
        cin >> name_entered;

        cout << "LOGGED IN!";
        usrName = usr;
        pin = stoi(pin_entered);

        addAccount(usrName, pin, name_entered);
    }

}

void goHome() {
    system("clear");
    // Welcome message

    cout << "Welcome to the Library Management system " << usrName << "!" << endl;
    cout << "Enter an action:" << endl;

    // run the functions based on the action
    evaluateAction(getAction(), database);
}

void evaluateAction(int number, sqlite3* db) {
    if (number == 0) {
        system("clear");
        cout << "QUIT THE SYSTEM!";
        sqlite3_close(db);

    } else if (number == 1) {
        system("clear");

        pin = 0;
        usrName = "";

        // run the login system
        while (usrName.empty() && pin == 0) {
            evalLogin();
        }

        // Welcome message
        system("clear");
        cout << "Welcome to the Library Management system " << usrName << "!" << endl;
        cout << "Enter an action:" << endl;

        // run the functions based on the action
        evaluateAction(getAction(), database);

    } else if (number == 2) {
        system("clear");
        char *bookISBN = nullptr; // Initialize to nullptr
        bool continueToDisplay = false;

        while (!continueToDisplay) {
            cout << "Enter the book ISBN-10 OR ISBN-13 (ONLY DIGITS, NO SYMBOLS OR LETTERS)" << endl;
            string input;
            cin >> input;

            // Allocate memory for bookISBN based on the length of input
            bookISBN = new char[input.length() + 1]; // +1 for null terminator

            // Copy input string into bookISBN
            strcpy(bookISBN, input.c_str());

            if (!isInt(bookISBN)) {
                system("clear");
                cout << "THE ISBN CODE MAY NOT INCLUDE LETTERS OR SYMBOLS" << endl;
                continue;
            }

            // Check the length of the input
            if (input.length() != 10 && input.length() != 13) {
                system("clear");
                cout << "IT MUST BE 10 OR 13 DIGITS ONLY. Eg: 1234567890 OR 1234567890123" << endl;
                continue;
            }

            continueToDisplay = true;
        }

        cout << "Loading.. Please Wait" << endl;

        map<string, json> bookData = getBookByISBN(bookISBN);
        system("clear");
        cout << "Welcome to the Library Management system " << usrName << "!" << endl;

        string bookTitle = bookData["title"];
        if (bookData["title"].is_null()) {
            bookTitle = "[BOOK NAME NOT FOUND]";
        }
        cout << "Book Data: " << bookTitle << endl;
        cout << "    ISBN Codes:" << endl;
        cout << "        - ISBN-10: " << bookData["isbn10"] << endl;
        cout << "        - ISBN-13: " << bookData["isbn13"] << endl;
        cout << "    Date Published: " << bookData["publishDates"] << endl;
        cout << "    Number of Pages: " << bookData["pages"] << endl;

        cout << "    Locations: " << endl;
        for (int i = 0; i < bookData["subjectLocations"].size(); i += 1) {
            cout << "        - " << bookData["subjectLocations"][i]["name"] << endl;
        }

        /* cout << "    Subjects: " << endl;
        for (int i = 0; i < bookData["subjects"].size(); i += 1) {
            cout << "        - " << bookData["subjects"][i]["name"] << endl;
        } */

        cout << "    Authors: " << endl;
        for (int i = 0; i < bookData["authors"].size(); i += 1) {
            cout << "        - " << bookData["authors"][i]["name"] << endl;
        }
        if (bookData["publishers"].is_array()) {
            cout << "    Publishers: " << endl;
            for (int i = 0; i < bookData["publishers"].size(); i += 1) {
                if (bookData["publishers"].contains("name")) {
                    cout << "        - " << bookData["publishers"][i]["name"] << endl;
                } else {
                    cout << "        - " << bookData["publishers"][i];
                }
            }
        } else {
            cout << "    Publishers: " << bookData["publishers"] << "\n\n" << endl;
        }

        bool firstLoop = true;
        while (true) {
            if (!firstLoop) {
                cout << "Invalid input. Please enter an integer which is displayed below within the brackets." << endl;
            }
            int actionChosen_book_page = getAction(false, false, true);

            if (actionChosen_book_page == 1) {
                break;
            } else if (actionChosen_book_page == 2) {
                // Take out book
                takeOutBook(bookData["isbn10"], bookData["isbn13"], bookData["title"]);
                break;
            } else if (actionChosen_book_page == 3) {
                // Return book
                returnBook(bookData["isbn10"], bookData["isbn13"]);
                //break;
            } else {
                system("clear");
                firstLoop = false;
                continue;
            }
        }
        goHome();

    } else if (number == 3) {
        system("clear");
        cout << "Welcome to the Library Management system " << usrName << "!" << endl;

        vector<json> takingsOut = getUsrTakingsOut();

        cout << "All the Books you Took Out/Currently Have Out:" << endl;
        for (auto & x : takingsOut) {
            char deliminator = '\n';
            vector<std::string> tokens = splitString(x["taken_on"], deliminator);

            cout << "- " << x["name"] << " " << "This Book is " << (x["out"] == "0" ? "Not " : "") << "Out" << endl ;
            cout << "   - Taken Out: " << tokens[0] << endl;
            cout << "   - ISBN 10: " << x["isbn10"] << endl;
            cout << "   - ISBN 13: " << x["isbn13"] << endl;

        }

        // Add a space in between data & options
        cout << endl;

        // Options
        bool firstLoop = true;
        while (true) {
            if (!firstLoop) {
                cout << "Invalid input. Please enter an integer which is displayed below within the brackets." << endl;
            }
            int actionChosen_book_page = getAction(false, false, false, true);

            if (actionChosen_book_page == 1) {
                break;
            } else if (actionChosen_book_page == 2) {
                system("clear");
                cout << "QUITTING THE PROCESS";
                return;
            } else {
                system("clear");
                firstLoop = false;
                continue;
            }
        }
        goHome();

    } else {
        system("clear");
        cout << "Welcome to the Library Management system " << usrName << "!" << endl;
        cout << "Invalid input. Please enter an integer which is displayed below within the brackets." << endl;
        evaluateAction(getAction(), database);
    }
}

void startSystem() {
    // run the login system
    while (usrName.empty() && pin == 0) {
        evalLogin();
    }

    // Welcome message
    system("clear");
    cout << "Welcome to the Library Management system " << usrName << "!" << endl;
    cout << "Enter an action:" << endl;

    // run the functions based on the action
    evaluateAction(getAction(), database);
}