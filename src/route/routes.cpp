#include <utils/utils.h>
#include "Router.h"

namespace hl::route {
char *Router::create(char *body) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
char *Router::addLikes(char *params, char *body) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
char *Router::update(char *params, int id, char *body) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
char *Router::filter(char *params) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
char *Router::group(char *params) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
char *Router::recommend(char *params, int id) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
char *Router::suggest(char *params, int id) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
char *Router::badRequest() {
  TRACE_CALL(__PRETTY_FUNCTION__)
  static char reply[] =
      "HTTP/1.1 400 Bad Request\r\n"
      "Content-Type: application/json\r\n"
      "Connection: keep-alive\r\n"
      "Content-Length: 2\r\n\r\n"
      "{}";
  return reply;
}
char *Router::notFound() {
  TRACE_CALL(__PRETTY_FUNCTION__)
  static char reply[] =
      "HTTP/1.1 404 Not Found\r\n"
      "Content-Type: application/json\r\n"
      "Connection: keep-alive\r\n"
      "Content-Length: 2\r\n\r\n"
      "{}";
  return reply;
}
}