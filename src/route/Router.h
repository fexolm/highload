//
// Created by Artem on 02.01.2019.
//

#ifndef HIGHLOAD_ROUTER_H
#define HIGHLOAD_ROUTER_H

#include "HTTP/Connection.h"
namespace hl::route {
class Router {
private:
  char * routeGet(http::Connection *connection);
  char * routePost(http::Connection *connection);
public:
  char * Route(http::Connection *connection);
};
}

#endif //HIGHLOAD_ROUTER_H
