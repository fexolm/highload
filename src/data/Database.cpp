//
// Created by Artem on 04.01.2019.
//
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include "Database.h"

namespace hl::data {
void Database::LoadJson(const char *path) {
  auto fp = fopen(path, "rb");
  static char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document document;

  document.ParseStream(is);

  auto &accounts = document["accounts"];
  assert(accounts.IsArray());
  for (auto &v : accounts.GetArray()) {
    Account a;
    a.id() = v["id"].GetInt();
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
    m_accounts.push_back(a);
  }
}
}
