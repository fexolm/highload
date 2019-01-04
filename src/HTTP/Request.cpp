//
// Created by Artem on 01.01.2019.
//

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <utils/utils.h>
#include "Request.h"

namespace hl::http {
inline static void parse_url(Request *req, char *url, int len) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  char *params = url;
  for (; *params && *params != '?'; ++params);
  if (*params) {
    ++params;
  } else {
    params = nullptr;
  }
  req->url = url;
  req->params = params;
}

int contentLength(char *request_string) {
  TRACE_CALL(__PRETTY_FUNCTION__)

  //std::cout << request_string << std::endl;
  char *line = request_string;
  while (true) {
    line = strchr(line, '\n');
    line++;
    if (line[0] != 'C' && line[0] != 'c')
      continue;
    if (line[13] != 'h')
      continue;
    line = line + 16;
    char *end;
    return static_cast<int>(strtol(line, &end, 10));
  }
}

bool hl::http::Request::Parse(char *request_string, int len) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  if (method == RequestMethod::NOT_PARSED) {
    parse(request_string, len);
  }
  if (method == RequestMethod::GET)
    return true;
  if (method == RequestMethod::NOT_PARSED) {
    return false;
  }
  return contentlength == 0 || len - (body - request_string) >= contentlength;
}
void Request::parse(char *request_string, int len) {

  std::cout << request_string << std::endl;
  TRACE_CALL(__PRETTY_FUNCTION__)
  int urlStart = 0;
  if (request_string[0] == 'G') {
    method = RequestMethod::GET;
    urlStart = 4;
  } else if (request_string[0] == 'P') {
    method = RequestMethod::POST;
    urlStart = 5;
  } else {
    method = RequestMethod::NOT_PARSED;
    return;
  }
  int urlLen = 0;
  for (; request_string[urlStart + urlLen] != ' ' && urlLen < len; urlLen++);
  if (urlLen == len) {
    method = RequestMethod::NOT_PARSED;
    return;
  }
  auto url_str = request_string + urlStart;
  parse_url(this, url_str, urlLen);
  if (method == RequestMethod::GET)
    return;
  contentlength = contentLength(request_string);
  if (contentlength == 0) {
    return;
  }
  size_t body_start = 0;
  for (; request_string[body_start] != '{'; body_start++);
  body = request_string + body_start;
}
}