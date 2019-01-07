#ifndef HIGHLOAD_ACCOUNT_H
#define HIGHLOAD_ACCOUNT_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
namespace hl::data {
enum {
  FNAME = 1 << 0,
  SNAME = 1 << 1,
  INTERESTS = 1 << 2,
  STATUS = 1 << 3,
  PREMIUM = 1 << 4,
  SEX = 1 << 5,
  PHONE = 1 << 6,
  LIKES= 1 << 7,
  BIRTH= 1 << 8,
  CITY= 1 << 9,
  COUNTRY= 1 << 10,
  JOINED= 1 << 11
};
class Account {
private:
  bool m_created;
  std::string m_fname;
  std::string m_sname;
  std::string m_email;
  std::unordered_set<std::string> m_interests;
  std::string m_status;
  uint64_t m_premiumStart{0};
  uint64_t m_premiumFinish{0};
  char m_sex;
  std::string m_phone;
  std::unordered_map<int, uint64_t> m_likes;
  uint64_t m_birth;
  std::string m_city;
  std::string m_country;
  uint64_t m_joined;

public:
  bool &created();
  std::string &fname();
  std::string &sname();
  std::string &email();
  std::unordered_set<std::string> &interests();
  std::string &status();
  uint64_t &premiumStart();
  uint64_t &premiumFinish();
  char &sex();
  std::string &phone();
  std::unordered_map<int, uint64_t> &likes();
  uint64_t &birth();
  std::string &city();
  std::string &country();
  uint64_t &joined();

  int Serialize(int id, char *buf, int options);
};
}
#endif //HIGHLOAD_ACCOUNT_H
