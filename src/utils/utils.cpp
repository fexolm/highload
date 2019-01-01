//
// Created by Artem on 01.01.2019.
//

#include <thread>
#include "utils.h"

ScopeLogger::ScopeLogger(const std::string &msg) : msg(msg) {
  std::cout << "[" << std::this_thread::get_id() << "]" << " Enter: " << msg << std::endl;
}
ScopeLogger::~ScopeLogger() { std::cout << "[" << std::this_thread::get_id() << "]" << " Exit:  " << msg << std::endl; }
