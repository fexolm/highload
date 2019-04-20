//
// Created by Artem on 31.12.2018.
//

#ifndef HIGHLOAD_SERVER_H
#define HIGHLOAD_SERVER_H

#include "Connection.h"
#include <thread>
#include <vector>

namespace hl::http {
class Server {
private:
  std::vector<std::thread> m_threads;
  int m_listener;

public:
  explicit Server();
  void SpawnThreads(int threads);
};
} // namespace hl::http

#endif // HIGHLOAD_SERVER_H
