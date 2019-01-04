#include "Account.h"

namespace hl::data {
int &Account::id() {
  return m_id;
}
std::string &Account::fname() {
  return m_fname;
}
std::string &Account::sname() {
  return m_sname;
}
std::string &Account::email() {
  return m_email;
}
std::vector<std::string> &Account::interests() {
  return m_interests;
}
std::string &Account::status() {
  return m_status;
}
long &Account::premiumStart() {
  return m_premiumStart;
}
long &Account::premiumFinish() {
  return m_premiumFinish;
}
char &Account::sex() {
  return m_sex;
}
std::string &Account::phone() {
  return m_phone;
}
std::vector<std::pair<int, long>> &Account::likes() {
  return m_likes;
}
long &Account::birth() {
  return m_birth;
}
std::string &Account::city() {
  return m_city;
}
std::string &Account::country() {
  return m_country;
}
long &Account::joined() {
  return m_joined;
}
}