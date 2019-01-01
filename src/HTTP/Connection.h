//
// Created by Artem on 01.01.2019.
//

#ifndef HIGHLOAD_CONNECTION_H
#define HIGHLOAD_CONNECTION_H

#include <stack>
#include <unordered_map>

#include "constants.h"
#include "Request.h"
namespace hl::http {
class Connection {
public:
  Request request;
  int fd;
  char buf[read_buffer_size];
  int bufLen;
  void Reset();
  bool Ready();
};

class ConnectionPool {
private:
  using fd_t = int;
  std::stack<Connection *> m_freeConnections;
  std::unordered_map<fd_t, Connection *> m_mappedConnections;
public:
  explicit ConnectionPool(int connectionsCount);
  void PutConnection(Connection *connection);
  Connection *GetConnection(fd_t fd);
};
}
#endif //HIGHLOAD_CONNECTION_H
