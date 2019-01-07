#include "Router.h"
namespace hl::route {
char *Router::create(char *body) {
  TRACE_CALL(__PRETTY_FUNCTION__)
  return notFound();
}
Router::Router(data::Database *db)
    : m_database(db) {}
}
