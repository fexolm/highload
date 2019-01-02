//
// Created by Artem on 02.01.2019.
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <utils/utils.h>
#include "Router.h"
#include "controllers/Account.h"
namespace hl::route {

char *BadRequest() {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return nullptr;
}

char *NotFound() {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return nullptr;
}

char *Router::Route(http::Connection *connection) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  auto method = connection->request.method;
  switch (method) {
  case http::RequestMethod::GET:return routeGet(connection);
  case http::RequestMethod::POST:return routePost(connection);
  default:std::cout << "NOT PARSED METHOD PROCESSED";
    return BadRequest();
  }
}
char *Router::routePost(http::Connection *connection) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  auto url = connection->request.url;
  auto params = connection->request.params;
  if (strncmp(url, "/accounts", 9) != 0) {
    return NotFound();
  }
  url += 9;
  if (strncmp(url, "/new", 4) == 0) {
    return controllers::Account::Create();
  }
  if (strncmp(url, "/likes", 6) == 0) {
    return controllers::Account::AddLikes(params);
  }
  if (std::isdigit(*(url + 1))) {
    int id = (int) strtol(url + 1, &url, 10);
    return controllers::Account::Update(params, id);
  }
  return NotFound();
}
char *Router::routeGet(http::Connection *connection) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  auto url = connection->request.url;
  auto params = connection->request.params;
  if (strncmp(url, "/accounts", 9) != 0) {
    return NotFound();
  }
  url += 9;
  if (strncmp(url, "/filter", 7) == 0) {
    return controllers::Account::Filter(params);
  }
  if (strncmp(url, "/group", 6) == 0) {
    return controllers::Account::Group(params);
  }
  if (std::isdigit(*(url + 1))) {
    int id = (int) strtol(url + 1, &url, 10);
    if (strncmp(url, "/recommend", 10) == 0) {
      return controllers::Account::Recommend(params, id);
    }
    if (strncmp(url, "/suggest", 6) == 0) {
      return controllers::Account::Suggest(params, id);
    }
  }
  return NotFound();
}
}