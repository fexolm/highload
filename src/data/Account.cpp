#include "Account.h"

namespace hl::data {

std::string &Account::fname() {
  return m_fname;
}
std::string &Account::sname() {
  return m_sname;
}
std::string &Account::email() {
  return m_email;
}
std::unordered_set<std::string> &Account::interests() {
  return m_interests;
}
std::string &Account::status() {
  return m_status;
}
uint64_t &Account::premiumStart() {
  return m_premiumStart;
}
uint64_t &Account::premiumFinish() {
  return m_premiumFinish;
}
char &Account::sex() {
  return m_sex;
}
std::string &Account::phone() {
  return m_phone;
}
std::unordered_map<int, uint64_t> &Account::likes() {
  return m_likes;
}
uint64_t &Account::birth() {
  return m_birth;
}
std::string &Account::city() {
  return m_city;
}
std::string &Account::country() {
  return m_country;
}
uint64_t &Account::joined() {
  return m_joined;
}
bool &Account::created() {
  return m_created;
}
int Account::Serialize(int id, char *buf, int options) {
  int len = 0;
  len += sprintf(buf + len, R"({"email":"%s")", m_email.c_str());

  if(options & FNAME) {
    len += sprintf(buf+len, R"(,"fname":"%s")", m_fname.c_str());
  }
  if(options & SNAME) {
    len += sprintf(buf+len, R"(,"sname":"%s")", m_sname.c_str());
  }
  if(options & STATUS) {
    len += sprintf(buf+len, R"(,"status":"%s")", m_status.c_str());
  }
  if(options & SEX) {
    len += sprintf(buf+len, R"(,"sex":"%c")", m_sex);
  }
  if(options & PHONE) {
    len += sprintf(buf+len, R"(,"phone":"%s")", m_phone.c_str());
  }
  if(options & CITY) {
    len += sprintf(buf+len, R"(,"city":"%s")", m_city.c_str());
  }
  if(options & COUNTRY) {
    len += sprintf(buf+len, R"(,"country":"%s")", m_country.c_str());
  }
  if(options & BIRTH) {
    len += sprintf(buf+len, R"(,"birth":%ld)", m_birth);
  }
  if(options & PREMIUM) {
    len += sprintf(buf+len, R"(,"premium": {"start":%ld, "finish":"%ld"})", m_premiumStart, m_premiumFinish);
  }

  len += sprintf(buf + len, R"(,"id":%d})", id);
  return len;
}
}