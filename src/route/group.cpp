#include "Router.h"
namespace hl::route {
char *Router::group(char *params) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
}