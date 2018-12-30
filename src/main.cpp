#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <sys/uio.h>
#include <cassert>
#include <fcntl.h>

#include "Request.h"

std::string reply =
    "HTTP/1.1 200 OK\n"
    "Content-Type: application/json\n"
    "Connection: keep-alive\n"
    "Content-Length: ";

const int new_buf_size = 1024 * 512;
const int turn_on = 1;
const unsigned int thread_nums = 1;
const size_t read_buffer_size = 2048;
const int max_events = 1000;

static inline int start_listen() {
  int listener;
  sockaddr_in addr{};
  listener = socket(AF_INET, SOCK_STREAM, 0);
  if (listener < 0) {
    perror("socket");
    exit(1);
  }

  setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (void *) &turn_on, sizeof(turn_on));
  setsockopt(listener, IPPROTO_TCP, TCP_QUICKACK, (void *) &turn_on, sizeof(turn_on));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listener, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(2);
  }
  listen(listener, 100);
  return listener;
}

static inline std::vector<std::thread> start_workers(int epollfd) {
  std::vector<std::thread> threads;

  for (unsigned int i = 0; i < thread_nums; ++i) {
    threads.emplace_back([epollfd]() {
      epoll_event events[max_events];
      char buf[read_buffer_size * 10];
      int nfds = 0;
      while (true) {
        nfds = epoll_wait(epollfd, events, max_events, 0);
        if (nfds == -1) {
          if (errno == EINTR) {
            continue;
          }
          perror("epoll_wait");
          exit(EXIT_FAILURE);
        }
        for (int n = 0; n < nfds; ++n) {
          int sock = events[n].data.fd;

          size_t data_len = 0;
          while (auto len = read(sock, buf + data_len, read_buffer_size)) {
            if (len < 0)
              break;
            data_len += len;
          }

          if (data_len == 0) {
            continue;
          }

          auto req = hl::parse_request(boost::string_ref(buf, data_len));
          auto response = reply;
          if (req.method == hl::RequestMethod::POST) {
            auto body = req.body.to_string();
            std::replace(body.begin(), body.end(), '\n', ' ');
            std::replace(body.begin(), body.end(), '\r', ' ');
            response += std::to_string(req.body.length()) + "\n\n" + body;
          } else {
            response += "adsfasdfasdfasdfasdf";
          }

          std::cout << response << std::endl;

          write(sock, response.data(), response.size());

          epoll_event ev{};
          ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
          ev.data.fd = sock;
          if (epoll_ctl(epollfd, EPOLL_CTL_MOD, sock, &ev) == -1) {
            perror("epoll_ctl: conn_sock");
            exit(EXIT_FAILURE);
          }
        }
      }
    });
  }
  return threads;
}

static inline void main_loop(int listener, int epollfd) {
  while (true) {
    int sock = accept(listener, nullptr, nullptr);
    if (sock == -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    setsockopt(sock, SOL_SOCKET, SO_SNDBUF,
               (void *) &new_buf_size, sizeof(new_buf_size));
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF,
               (void *) &new_buf_size, sizeof(new_buf_size));
    setsockopt(sock, SOL_SOCKET, SO_DONTROUTE, (void *) &turn_on, sizeof(turn_on));
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *) &turn_on, sizeof(turn_on));
    setsockopt(sock, SOL_SOCKET, SOCK_NONBLOCK, (void *) &turn_on, sizeof(turn_on));
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    ev.data.fd = sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev) == -1) {
      perror("epoll_ctl: conn_sock");
      exit(EXIT_FAILURE);
    }
  }
}

int main() {
  signal(SIGPIPE, SIG_IGN);

  int listener = start_listen();

  int epollfd = epoll_create1(0);
  if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  auto threads = start_workers(epollfd);

  main_loop(listener, epollfd);
}