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
#include "Worker.h"
namespace hl::http {
void Server::SpawnThreads(int threads) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  for (int i = 1; i < threads; i++) {
    m_threads.emplace_back(&Worker::Start, new Worker(m_listener));
  }
  (new Worker(m_listener))->Start();
}
Server::Server() {
  TRACE_CALL(__PRETTY_FUNCTION__)
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
}