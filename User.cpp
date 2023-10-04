#include "User.h"

#include <string>
#include <vector>
using namespace std;

User::User(const string& full_name, const string& email, const vector<string>& watch_history) {
    m_fullName = full_name;
    m_email = email;
    m_watchHistory = watch_history;
}
