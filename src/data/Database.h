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
  uint64_t m_time;
public:
  Database();
  void LoadJson(const char *json);
  Account *accounts();
  uint64_t *pages();
  int len();
  uint64_t &time();
};
}
#endif //HIGHLOAD_DATABASE_H
