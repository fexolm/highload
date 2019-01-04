#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>

#include "HTTP/Server.h"
#include <data/Database.h>
int main() {

  hl::data::Database database;
  for (const auto &entry : boost::filesystem::directory_iterator("/opt/data")) {
    auto path = entry.path().string();
    database.LoadJson(path.c_str());
  }
  hl::http::Server server;

  server.SpawnThreads(4);
}