#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include "treemm.h"
#include <vector>
#include <string>

class User;

class UserDatabase {
  public:
    UserDatabase();
    ~UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
    TreeMultimap<std::string, User*> m_UDB;
    std::vector<std::string> m_allUserEmails;
};

#endif // USERDATABASE_INCLUDED
