#ifndef HIGHLOAD_DATABASE_H
#define HIGHLOAD_DATABASE_H

#include <rapidjson/document.h>
#include <constants.h>
#include "Account.h"

namespace hl::data {
class Database {
private:
  Account *m_accounts;
  uint64_t *m_pages;
  int m_len {0};
public:
  Database();
  void LoadJson(const char *json);
  Account *accounts();
  uint64_t *pages();
  int len();
};
}
#endif //HIGHLOAD_DATABASE_H
