//
// Created by Artem on 02.01.2019.
//

#ifndef HIGHLOAD_WORKER_H
#define HIGHLOAD_WORKER_H

#include <route/Router.h>
#include <data/Database.h>
#include "Connection.h"
namespace hl::http {
class Worker {
private:
  int m_epollFd;
  ConnectionPool *m_connectionPool;
  Connection *m_serverConnection;
  int m_listener;
  ::epoll_event *m_events;
  data::Database *m_database;
  route::Router m_router;

  void processConnection(Connection *connection);
  void processRequest(Connection *connection);
public:
  explicit Worker(int listener, data::Database *db);
  void Start();
};
}

#endif //HIGHLOAD_WORKER_H
