//
// Created by Artem on 29.12.2018.
//

#include <cstring>
#include "Request.h"

namespace hl {

Request parse_request(boost::string_ref request_string) {
  const int headers_length = 70;

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
    size_t count_n = 0;
    size_t body_start = urlend + headers_length;
    for(; count_n < 2; body_start++) {
      switch (request_string[body_start]) {
      case '\n':
        count_n++;
        break;
      case '\r': break;
      default:
        count_n=0;
        break;
      }
    }
    result.body = request_string.substr(body_start, strlen(request_string.data()));
  }
  return result;
}
}