#include "Router.h"
namespace hl::route {
char *Router::recommend(char *params, int id) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
}