#include <iostream>
#include <boost/filesystem.hpp>
#include <data/Database.h>
#include <csignal>
#include <fstream>
#include "HTTP/Server.h"

int main() {
  signal(SIGPIPE, SIG_IGN);

  hl::data::Database database;
  std::ifstream in;
  in.open("/opt/options.txt");
  in >> database.time();
  in.close();
  for (const auto &entry : boost::filesystem::directory_iterator("/opt/data")) {
    auto path = entry.path().string();
    database.LoadJson(path.c_str());
  }
  std::cout << "Size: " << database.len() << std::endl;

  hl::http::Server server(&database);

  server.SpawnThreads(4);
}