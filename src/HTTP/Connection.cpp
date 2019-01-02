//
// Created by Artem on 01.01.2019.
//

#include <iostream>
#include <utils/utils.h>
#include "Connection.h"
namespace hl::http {
ConnectionPool::ConnectionPool(int connectionsCount) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  for (int i = 0; i < connectionsCount; i++) {
    m_freeConnections.push(new Connection);
  }
}
Connection *ConnectionPool::GetConnection(hl::http::ConnectionPool::fd_t fd) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  if (!m_mappedConnections.count(fd)) {
    auto c = m_freeConnections.top();
    m_freeConnections.pop();
    m_mappedConnections[fd] = c;
    c->fd = fd;
    c->Reset();
  }
  return m_mappedConnections[fd];
}
void ConnectionPool::PutConnection(Connection *connection) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  m_mappedConnections.erase(connection->fd);
  m_freeConnections.push(connection);
}

void Connection::Reset() {
  TRACE_CALL(__PRETTY_FUNCTION__)
  bufLen = 0;
  request.method = RequestMethod::NOT_PARSED;
}
bool Connection::Ready() {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return request.Parse(buf, bufLen);
}
}
