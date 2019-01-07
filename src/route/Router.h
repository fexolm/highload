//
// Created by Artem on 02.01.2019.
//

#ifndef HIGHLOAD_ROUTER_H
#define HIGHLOAD_ROUTER_H

#include <data/Database.h>
#include <unordered_set>
#include <functional>

#include "HTTP/Connection.h"
#include "utils/utils.h"

enum class Param {
  eq,
  domain,
  lt,
  gt,
  neq,
  any,
  null,
  starts,
  code,
  year,
  contains,
  now,
};

namespace hl::route {
using filters_t = std::vector<std::function<int(data::Account *, uint64_t *, uint64_t *, int &)>>;

class Router {
private:
  data::Database *m_database;

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

  void sex_filter(Param param, char *args, filters_t &filters);
  void email_filter(Param param, char *args, filters_t &filters);
  void status_filter(Param param, char *args, filters_t &filters);
  void fname_filter(Param param, char *args, filters_t &filters);
  void sname_filter(Param param, char *args, filters_t &filters);
  void phone_filter(Param param, char *args, filters_t &filters);
  void county_filter(Param param, char *args, filters_t &filters);
  void city_filter(Param param, char *args, filters_t &filters);
  void birth_filter(Param param, char *args, filters_t &filters);
  void interests_filter(Param param, char *args, filters_t &filters);
  void likes_filter(Param param, char *args, filters_t &filters);
  void premium_filter(Param param, char *args, filters_t &filters);
  int nextParam(char *&params, filters_t &filters, int &limit);
public:
  char *Route(http::Connection *connection);
  explicit Router(data::Database *db);
};
}

#endif //HIGHLOAD_ROUTER_H
