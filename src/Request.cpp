#include <cstring>
#include <cassert>
#include <iostream>
#include "Request.h"

namespace hl {

Request parse_request(boost::string_ref request_string) {
  //const int headers_length = 70;

  Request result;
  if (request_string[0] == 'G') {
    result.method = RequestMethod::GET;
    size_t urlstart = 4;
    size_t urlend = urlstart;
    for (; request_string[urlend] != ' '; urlend++);
    result.url = request_string.substr(urlstart, urlend - urlstart);
  } else {
    result.method = RequestMethod::POST;
    size_t urlstart = 5;
    size_t urlend = urlstart;
    for (; request_string[urlend] != ' '; urlend++);
    result.url = request_string.substr(urlstart, urlend - urlstart);
    size_t body_start = urlend;

    for(;body_start < request_string.length() && request_string[body_start] != '{'; body_start++);

    if(body_start >= request_string.length()) {
      std::cout << "error" << std::endl;
//      std::cout << request_string << std::endl;
      return result;
    }
    result.body = request_string.substr(body_start, request_string.length() - body_start);
  }
  return result;
}
}