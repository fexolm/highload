#include <utils/utils.h>
#include "Router.h"
namespace hl::route {
char *Router::suggest(char *params, int id) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  utils::percent_decode(params, params);
  std::cout << params << std::endl;
  return notFound();
}
}