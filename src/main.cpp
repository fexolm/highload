#include <iostream>
#include <boost/filesystem.hpp>
#include <csignal>
#include <fstream>
#include "HTTP/Server.h"

int main() {
  signal(SIGPIPE, SIG_IGN);

  hl::http::Server server;

  server.SpawnThreads(4);
}
