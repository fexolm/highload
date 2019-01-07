#include <iostream>
#include <boost/filesystem.hpp>
#include <data/Database.h>
#include <csignal>

#include "HTTP/Server.h"

int main() {
  signal(SIGPIPE, SIG_IGN);

  hl::data::Database database;
  for (const auto &entry : boost::filesystem::directory_iterator("/opt/data")) {
    auto path = entry.path().string();
    database.LoadJson(path.c_str());
  }
  std::cout << "Size: " << database.len() << std::endl;

  hl::http::Server server(&database);

  server.SpawnThreads(4);
}