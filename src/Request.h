//
// Created by Artem on 29.12.2018.
//

#ifndef HIGHLOAD_REQUEST_H
#define HIGHLOAD_REQUEST_H

#include <boost/utility/string_ref.hpp>

namespace hl {
enum class RequestMethod : char {
  GET,
  POST,
};
struct Request {
  RequestMethod method {};
  boost::string_ref url {};
  boost::string_ref body {};
};

Request parse_request(boost::string_ref request_string);
}

#endif //HIGHLOAD_REQUEST_H
