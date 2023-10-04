#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

UserDatabase::UserDatabase() {}

UserDatabase::~UserDatabase() { 
    vector<string>::iterator it = m_allUserEmails.begin();
    while (it != m_allUserEmails.end()) {
        User* ptr = get_user_from_email(*it);
        delete ptr;
        it = m_allUserEmails.erase(it);
    }
}

bool UserDatabase::load(const string& filename) {
    ifstream infile(filename);
    if (!infile) 
        return false;

    // keeps track of what element corresponds to what
    bool isName = true;
    bool isEmail = false, isWatchHistoryLength = false, isBlank = true;
    int movies = 0;

    string name;
    string email;
    vector<string> watchHistory;
    string s;

    while(getline(infile, s)) {
        // inserts lines into corresponding elements
        if (isName) {
            name = s;
            isName = false;
            isEmail = true;
        } else if (isEmail) {
            email = s;
            isEmail = false;
            isWatchHistoryLength = true;
        } else if (isWatchHistoryLength) {
            movies = stoi(s);
            isWatchHistoryLength = false;
        } else if (movies > 1) {
            watchHistory.push_back(s);
            movies--;
        } else if (movies == 1) {
            watchHistory.push_back(s);
            m_UDB.insert(email, new User(name, email, watchHistory));
            m_allUserEmails.push_back(email);
            for(vector<string>::iterator it = watchHistory.begin(); it != watchHistory.end(); )
                it = watchHistory.erase(it);
            movies--;
            isBlank = true;
        } else if (isBlank) {
            isName = true;
        }
    }

    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const {
    TreeMultimap<string, User*>::Iterator it = m_UDB.find(email);
    if (!it.is_valid())
        return nullptr;
    return it.get_value();
}
