//
// Created by Artem on 02.01.2019.
//

#ifndef HIGHLOAD_ACCOUNT_CONTROLLER_H
#define HIGHLOAD_ACCOUNT_CONTROLLER_H

namespace hl::controllers {
class Account {
public:
  static char * Filter(char *params);
  static char * Group(char *params);
  static char * Recommend(char *params, int id);
  static char * Suggest(char *params, int id);
  static char * Create();
  static char * Update(char *params, int id);
  static char * AddLikes(char *params);
};
}
#endif //HIGHLOAD_ACCOUNT_CONTROLLER_H
