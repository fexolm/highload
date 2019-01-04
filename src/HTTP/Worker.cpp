//
// Created by Artem on 02.01.2019.
//

#include <sys/unistd.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <iostream>
#include <assert.h>

#include <utils/utils.h>
#include "Worker.h"

namespace hl::http {
static inline bool check_err(epoll_event &event) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  bool err = false;
  if ((event.events & EPOLLERR)) {
    std::cout << "EPOLLERR" << std::endl;
    err = true;
  }
  if (event.events & EPOLLHUP) {
    std::cout << "EPOLLHUP" << std::endl;
    err = true;
  }
  if (!(event.events & EPOLLIN)) {
    std::cout << "NO EPOLLIN" << std::endl;
    err = true;
  }
  return err;
}

Worker::Worker(int listener)
    : m_connectionPool(new ConnectionPool(max_events)),
      m_listener(listener) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  m_epollFd = epoll_create1(0);
  HL_CLOSE_ON_FAIL(m_epollFd);
  m_serverConnection = new Connection;
  m_serverConnection->fd = m_listener;
  ::epoll_event ev{};
  ev.data.ptr = m_serverConnection;
  ev.events = EPOLLIN;
  if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_listener, &ev) == -1) {
    perror("epoll_ctl");
    throw;
  }
  m_events = new ::epoll_event[max_events];
}

void Worker::Start() {
  TRACE_CALL(__PRETTY_FUNCTION__)
  while (true) {
    int nfds = ::epoll_wait(m_epollFd, m_events, max_events, 0);
    if (nfds == -1) {
      if (errno == EINTR) {
        continue;
      }
      perror("worker epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nfds; ++i) {
      auto conn = (Connection *) m_events[i].data.ptr;
      if (check_err(m_events[i])) {
        std::cout << "ERROR IN " << (conn == m_serverConnection ? "SERVER " : "WORKER ") << " DESC" << std::endl;
        ::close(conn->fd);
        m_connectionPool->PutConnection(conn);
        continue;
      }
      if (conn == m_serverConnection) {
        int sock = accept4(m_listener, nullptr, nullptr, SOCK_NONBLOCK);
        if (sock == -1) {
          if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            continue;
          } else {
            perror("accept");
            continue;;
          }
        }

        HL_CLOSE_ON_FAIL(::setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *) &buffer_size, sizeof(buffer_size)));
        HL_CLOSE_ON_FAIL(::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *) &buffer_size, sizeof(buffer_size)));
        HL_CLOSE_ON_FAIL(::setsockopt(sock, SOL_SOCKET, SO_DONTROUTE, (void *) &yes, sizeof(yes)));
        HL_CLOSE_ON_FAIL(::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *) &yes, sizeof(yes)));

        ::epoll_event ev{};
        ev.events = EPOLLIN | EPOLLET;
        ev.data.ptr = m_connectionPool->GetConnection(sock);
        HL_CLOSE_ON_FAIL(::epoll_ctl(m_epollFd, EPOLL_CTL_ADD, sock, &ev));
        continue;
      }
      processConnection(conn);
    }
  }
}
void Worker::processConnection(Connection *conn) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  bool done = false;
  bool closeFd = false;

  while (true) {
    ssize_t count = read(conn->fd, conn->buf + conn->bufLen, read_buffer_size);
    if (count == -1) {
      if (errno == EAGAIN)
        continue;
      perror("Read Eroor");
      done = true;
      break;
    } else if (count == 0) {
      done = true;
      closeFd = true;
      break;
    }
    conn->bufLen += count;
    if (conn->Ready()) {
      done = true;
      processRequest(conn);
    }
    break;
  }
  if (done) {
    if (closeFd) {
      epoll_event ev{};
      ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
      ev.data.ptr = conn;
      if (epoll_ctl(m_epollFd, EPOLL_CTL_DEL, conn->fd, &ev) == -1) {
        perror("epoll_ctl: conn_sock");
        exit(EXIT_FAILURE);
      }
      close(conn->fd);
      m_connectionPool->PutConnection(conn);
    } else {
      conn->Reset();
    }
  }
}
static inline bool validate_json(Request &req) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  char *json = req.body;
  if (json[0] != '{')
    return false;
  int count = 0;
  for (int i = 0; i < req.contentlength; i++) {
    if (*json == '{')
      count++;
    if (*json == '}')
      count--;
    json++;
  }
  return count == 0;
}

void Worker::processRequest(Connection *connection) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  static std::string reply =
      "HTTP/1.1 200 OK\n"
      "Content-Type: application/json\n"
      "Connection: keep-alive\n"
      "Content-Length: \n\n"
      "adsfasdfasdfasdfasdf";

  if (connection->request.method == RequestMethod::POST) {
    if (connection->request.contentlength)
      assert(validate_json(connection->request));
  }
  m_router.Route(connection);
  write(connection->fd, reply.data(), reply.size());
}
}

