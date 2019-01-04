#ifndef HIGHLOAD_DATABASE_H
#define HIGHLOAD_DATABASE_H

#include <rapidjson/document.h>
#include "Account.h"

namespace hl::data {
class Database {
private:
  std::vector<Account> m_accounts;
public:
  void LoadJson(const char *json);
};
}
#endif //HIGHLOAD_DATABASE_H
