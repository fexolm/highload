//
// Created by Artem on 31.12.2018.
//

#include <sys/unistd.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <iostream>
#include <assert.h>

#include "Server.h"
#include "utils/utils.h"
#include "constants.h"
#include "Connection.h"

std::string reply =
    "HTTP/1.1 200 OK\n"
    "Content-Type: application/json\n"
    "Connection: keep-alive\n"
    "Content-Length: \n\n"
    "adsfasdfasdfasdfasdf";

namespace hl::http {
void Server::SpawnThreads(int threads) {
  TRACE_CALL(__FUNCTION__)
  for (int i = 0; i < threads; i++) {
    m_threads.emplace_back(&Server::workerThread, this);
  }
}
Server::Server() {
  TRACE_CALL(__FUNCTION__)
  m_listener = ::socket(AF_INET, SOCK_STREAM, 0);
  HL_CLOSE_ON_FAIL(::setsockopt(m_listener, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)));
  HL_CLOSE_ON_FAIL(::setsockopt(m_listener, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)));
  HL_CLOSE_ON_FAIL(::setsockopt(m_listener, SOL_SOCKET, SO_SNDBUF, &yes, sizeof(yes)));
  HL_CLOSE_ON_FAIL(::setsockopt(m_listener, SOL_SOCKET, SO_RCVBUF, &yes, sizeof(yes)));

  ::sockaddr_in addr{0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  HL_CLOSE_ON_FAIL(::bind(m_listener, (struct sockaddr *) &addr, sizeof(addr)));
  HL_CLOSE_ON_FAIL(::listen(m_listener, 100));
}

bool check_err(epoll_event &event) {
  bool err = false;
  if ((event.events & EPOLLERR)) {
    std::cout << "EPOLLERR" <<std::endl;
    err = true;
  }
  if (event.events & EPOLLHUP) {
    std::cout << "EPOLLHUP" <<std::endl;
    err = true;
  }
  if (!(event.events & EPOLLIN)) {
    std::cout << "NO EPOLLIN" <<std::endl;
    err = true;
  }
  return err;
}
void Server::workerThread() {
  TRACE_CALL(__FUNCTION__)
  int epollFd = epoll_create1(0);
  HL_CLOSE_ON_FAIL(epollFd);
  ConnectionPool connectionPool(max_events);
  auto serverConnection = new Connection;
  serverConnection->fd = m_listener;
  ::epoll_event ev{};
  ev.data.ptr = serverConnection;
  ev.events = EPOLLIN;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, m_listener, &ev) == -1) {
    perror("epoll_ctl");
    throw;
  }
  ::epoll_event events[max_events];
  while (true) {
    int nfds = ::epoll_wait(epollFd, events, max_events, 0);
    if (nfds == -1) {
      if (errno == EINTR) {
        continue;
      }
      perror("worker epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nfds; ++i) {
      auto conn = (Connection *) events[i].data.ptr;
      if (check_err(events[i])) {
        std::cout << "ERROR IN " << (conn == serverConnection ? "SERVER " : "WORKER ") << " DESC" << std::endl;
        ::close(conn->fd);
        connectionPool.PutConnection(conn);
        continue;
      }
      if (conn == serverConnection) {
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
        ev.data.ptr = connectionPool.GetConnection(sock);
        HL_CLOSE_ON_FAIL(::epoll_ctl(epollFd, EPOLL_CTL_ADD, sock, &ev));
        continue;
      }
      processConnection(conn, epollFd, connectionPool);
    }
  }
}
void Server::processConnection(Connection *conn, int epollFd, ConnectionPool &connectionPool) {
  TRACE_CALL(__FUNCTION__)
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
      if (epoll_ctl(epollFd, EPOLL_CTL_DEL, conn->fd, &ev) == -1) {
        perror("epoll_ctl: conn_sock");
        exit(EXIT_FAILURE);
      }
      close(conn->fd);
      connectionPool.PutConnection(conn);
    } else {
      conn->Reset();
    }
  }
}
bool validate_json(Request &req) {
  TRACE_CALL(__FUNCTION__)
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

void Server::processRequest(Connection *connection) {
  TRACE_CALL(__FUNCTION__)
  if (connection->request.method == RequestMethod::POST) {
    std::cout << "POST! " << connection->bufLen << std::endl;
    if (connection->request.contentlength)
      assert(validate_json(connection->request));
  }

  write(connection->fd, reply.data(), reply.size());
}
}