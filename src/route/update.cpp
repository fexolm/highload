#include "Router.h"
namespace hl::route {
char *Router::update(char *params, int id, char *body) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
}