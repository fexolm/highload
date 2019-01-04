//
// Created by Artem on 02.01.2019.
//

#ifndef HIGHLOAD_ROUTER_H
#define HIGHLOAD_ROUTER_H

#include "HTTP/Connection.h"
namespace hl::route {
class Router {
private:
  char *routeGet(http::Connection *connection);
  char *routePost(http::Connection *connection);

  char *create(char *body);
  char *addLikes(char *params, char *body);
  char *update(char *params, int id, char *body);
  char *filter(char *params);
  char *group(char *params);
  char *recommend(char *params, int id);
  char *suggest(char *params, int id);

  char *badRequest();
  char *notFound();
public:
  char *Route(http::Connection *connection);
};
}

#endif //HIGHLOAD_ROUTER_H
