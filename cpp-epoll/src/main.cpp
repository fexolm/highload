#include "HTTP/Server.h"
#include <boost/filesystem.hpp>
#include <csignal>
#include <fstream>
#include <iostream>

int main() {
  signal(SIGPIPE, SIG_IGN);

  hl::http::Server server;

  server.SpawnThreads(4);
}
