//
// Created by Artem on 02.01.2019.
//

#include <iostream>
#include <cstring>
#include <utils/utils.h>
#include "Router.h"
namespace hl::route {

char *Router::Route(http::Connection *connection) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  auto method = connection->request.method;
  switch (method) {
  case http::RequestMethod::GET:return routeGet(connection);
  case http::RequestMethod::POST:return routePost(connection);
  default:std::cout << "NOT PARSED METHOD PROCESSED";
    return badRequest();
  }
}
char *Router::routePost(http::Connection *connection) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  auto url = connection->request.url;
  auto params = connection->request.params;
  auto body = connection->request.body;
  if (strncmp(url, "/accounts", 9) != 0) {
    return notFound();
  }
  url += 9;
  if (strncmp(url, "/new", 4) == 0) {
    return create(body);
  }
  if (strncmp(url, "/likes", 6) == 0) {
    return addLikes(params, body);
  }
  if (std::isdigit(*(url + 1))) {
    int id = (int) strtol(url + 1, &url, 10);
    return update(params, id, body);
  }
  return notFound();
}
char *Router::routeGet(http::Connection *connection) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  auto url = connection->request.url;
  auto params = connection->request.params;
  if (strncmp(url, "/accounts", 9) != 0) {
    return notFound();
  }
  url += 9;
  if (strncmp(url, "/filter", 7) == 0) {
    return filter(params);
  }
  if (strncmp(url, "/group", 6) == 0) {
    return group(params);
  }
  if (std::isdigit(*(url + 1))) {
    int id = (int) strtol(url + 1, &url, 10);
    if (strncmp(url, "/recommend", 10) == 0) {
      return recommend(params, id);
    }
    if (strncmp(url, "/suggest", 6) == 0) {
      return suggest(params, id);
    }
  }
  return notFound();
}
}