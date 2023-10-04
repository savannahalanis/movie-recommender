#ifndef USER_INCLUDED
#define USER_INCLUDED

#include <string>
#include <vector>
#include <iostream>

class User {
  public:
    User(const std::string& full_name, const std::string& email, const std::vector<std::string>& watch_history);
    std::string get_full_name() const {return m_fullName;}
    std::string get_email() const {return m_email;}
    std::vector<std::string> get_watch_history() const {return m_watchHistory;}

  private:
    std::string m_fullName;
    std::string m_email;
    std::vector<std::string> m_watchHistory;

};

#endif // USER_INCLUDED
