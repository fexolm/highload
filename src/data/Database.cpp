#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include "Database.h"

namespace hl::data {
void Database::LoadJson(const char *path) {
  auto fp = fopen(path, "rb");
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document document;

  document.ParseStream(is);
  fclose(fp);

  auto &accounts = document["accounts"];
  assert(accounts.IsArray());
  for (auto &v : accounts.GetArray()) {
    Account a;
    auto id = v["id"].GetInt();
    a.email() = v["email"].GetString();
    if (v.HasMember("fname"))
      a.fname() = v["fname"].GetString();
    if (v.HasMember("sname"))
      a.sname() = v["sname"].GetString();
    if (v.HasMember("phone"))
      a.phone() = v["phone"].GetString();
    a.sex() = v["sex"].GetString()[0];
    a.birth() = v["birth"].GetInt64();
    if (v.HasMember("country"))
      a.country() = v["country"].GetString();
    if (v.HasMember("city"))
      a.city() = v["city"].GetString();
    a.joined() = v["joined"].GetInt64();
    a.status() = v["status"].GetString();
    if (v.HasMember("interests")) {
      for (auto &i : v["interests"].GetArray()) {
        a.interests().emplace_back(i.GetString());
      }
    }
    if (v.HasMember("premium")) {
      auto &premium = v["premium"];
      a.premiumStart() = premium["start"].GetInt64();
      a.premiumFinish() = premium["finish"].GetInt64();
    }
    if (v.HasMember("likes")) {
      for (auto &like : v["likes"].GetArray()) {
        a.likes().emplace_back(like["id"].GetInt(), like["ts"].GetInt64());
      }
    }
    m_accounts[id] = a;
    m_accounts[id].created() = true;
    m_len = std::max(id, m_len);
    m_pages[id / 64] |= (((uint64_t )1) << id % 64);
  }
}
Account *Database::accounts() {
  return m_accounts;
}
int Database::len() {
  return m_len;
}
Database::Database()
    : m_accounts(new Account[max_records_count]),
      m_pages(new uint64_t[pages_count]) {
  for (int i = 0; i < max_records_count; i++) {
    m_accounts[i].created() = false;
    memset(m_pages, 0, sizeof(uint64_t) * pages_count);
  }
}
uint64_t *Database::pages() {
  return m_pages;
}
}
