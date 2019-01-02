#include <vector>
#include <iostream>

#include "HTTP/Server.h"

int main() {

  hl::http::Server server;

  server.SpawnThreads(4);
}
