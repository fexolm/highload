//
// Created by Artem on 01.01.2019.
//

#ifndef HIGHLOAD_REQUEST_H
#define HIGHLOAD_REQUEST_H

namespace hl::http {
enum class RequestMethod : char {
  GET,
  POST,
  NOT_PARSED
};
struct Request {
  RequestMethod method{RequestMethod::NOT_PARSED};
  char *body{};
  char *url{};
  char *params{};
  int contentlength{};
  bool Parse(char *request_string, int len);
private:
  void parse(char *request_string, int len);
};
}
#endif //HIGHLOAD_REQUEST_H
