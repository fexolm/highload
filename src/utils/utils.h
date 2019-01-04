//
// Created by Artem on 01.01.2019.
//

#ifndef HIGHLOAD_UTILS_H
#define HIGHLOAD_UTILS_H
#include <cstdio>
#include <cstdlib>
#include <iostream>
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define HL_CLOSE_ON_FAIL(f) \
  do { \
    if((f) == -1) { \
      ::perror("Error: "); \
      throw; \
    }} while(0)

class ScopeLogger {
public:
  explicit ScopeLogger(std::string const &msg);
  ~ScopeLogger();
  std::string msg{};
};

#ifdef TRACE_CALLS
#define TRACE_CALL(x) ScopeLogger l_##x##_scope(x);
#else
#define TRACE_CALL(x)
#endif

namespace hl::utils {
int percent_decode(char *out, char *in);
}
#endif //HIGHLOAD_UTILS_H
