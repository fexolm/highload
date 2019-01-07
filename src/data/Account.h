#ifndef HIGHLOAD_ACCOUNT_H
#define HIGHLOAD_ACCOUNT_H

#include <string>
#include <vector>
namespace hl::data {
class Account {
private:
  bool m_created;
  std::string m_fname;
  std::string m_sname;
  std::string m_email;
  std::vector<std::string> m_interests;
  std::string m_status;
  long m_premiumStart;
  long m_premiumFinish;
  char m_sex;
  std::string m_phone;
  std::vector<std::pair<int, long>> m_likes;
  long m_birth;
  std::string m_city;
  std::string m_country;
  long m_joined;

public:
  bool &created();
  std::string &fname();
  std::string &sname();
  std::string &email();
  std::vector<std::string> &interests();
  std::string &status();
  long &premiumStart();
  long &premiumFinish();
  char &sex();
  std::string &phone();
  std::vector<std::pair<int, long>> &likes();
  long &birth();
  std::string &city();
  std::string &country();
  long &joined();

  int Serialize(int id, char *buf);
};
}
#endif //HIGHLOAD_ACCOUNT_H
