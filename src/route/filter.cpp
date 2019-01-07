#include <unordered_set>

#include "Router.h"
namespace hl::route {

template<typename F>
void iterate_page8(int pageNum, int shift, data::Account *accounts, uint8_t page, F func) {
  if (page == 0)
    return;

  for (int i = 7; i >= 0; i--) {
    if (page & (((uint8_t) 1) << i)) {
      int ind = pageNum * 64 + shift + i;
      func(ind, accounts[ind], pageNum);
    }
  }
}
template<typename F>
void iterate_page16(int pageNum, int shift, data::Account *accounts, uint16_t page, F func) {
  if (page == 0)
    return;
  uint16_t mask = 0x00FF;
  auto left = static_cast<uint8_t>(page >> 8);
  auto right = static_cast<uint8_t>(page & mask);
  iterate_page8(pageNum, shift + 8, accounts, left, func);
  iterate_page8(pageNum, shift, accounts, right, func);
}
template<typename F>
void iterate_page32(int pageNum, int shift, data::Account *accounts, uint32_t page, F func) {
  if (page == 0)
    return;
  uint32_t mask = 0x0000FFFF;
  auto left = static_cast<uint16_t>(page >> 16);
  auto right = static_cast<uint16_t>(page & mask);
  iterate_page16(pageNum, shift + 16, accounts, left, func);
  iterate_page16(pageNum, shift, accounts, right, func);
}
template<typename F>
void iterate_page64(int pageNum, data::Account *accounts, uint64_t page, F func) {
  if (page == 0)
    return;
  uint64_t mask = 0x00000000FFFFFFFF;
  auto left = static_cast<uint32_t>(page >> 32);
  auto right = static_cast<uint32_t>(page & mask);
  iterate_page32(pageNum, 32, accounts, left, func);
  iterate_page32(pageNum, 0, accounts, right, func);
}
template<typename F>
void iterate_pages(data::Account *accounts, uint64_t *pages, F func) {
  for (int i = pages_count - 1; i >= 0; i--) {
    iterate_page64(i, accounts, pages[i], func);
  }
}

int nextArg(char *&args, char *&arg) {
  int len = 0;
  if (*args == '&' || *args == ',')
    args++;
  arg = args;
  for (; *args && *args != ',' && *args != '&'; args++, len++);
  return len;
}
int Router::nextParam(char *&params, filters_t &filters, int &limit) {
  if (*params == '&')
    params++;
  if (!*params)
    return 0;
  bool found = false;
  if (strncmp(params, "sex", 3) == 0) {
    params += 4;
    if (strncmp(params, "eq", 2) == 0) {
      params += 3;
      sex_filter(Param::eq, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "email", 5) == 0) {
    params += 6;
    if (strncmp(params, "domain", 6) == 0) {
      params += 7;
      email_filter(Param::domain, params, filters);
      found = true;
    }
    if (strncmp(params, "lt", 2) == 0) {
      params += 3;
      email_filter(Param::lt, params, filters);
      found = true;
    }
    if (strncmp(params, "gt", 2) == 0) {
      params += 3;
      email_filter(Param::gt, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "status", 6) == 0) {
    params += 7;
    if (strncmp(params, "eq", 2) == 0) {
      params += 3;
      status_filter(Param::eq, params, filters);
      found = true;
    }
    if (strncmp(params, "neq", 3) == 0) {
      params += 4;
      status_filter(Param::neq, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "fname", 5) == 0) {
    params += 6;
    if (strncmp(params, "eq", 2) == 0) {
      params += 3;
      fname_filter(Param::eq, params, filters);
      found = true;
    }
    if (strncmp(params, "any", 3) == 0) {
      params += 4;
      fname_filter(Param::any, params, filters);
      found = true;
    }
    if (strncmp(params, "null", 4) == 0) {
      params += 5;
      fname_filter(Param::null, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "sname", 5) == 0) {
    params += 6;
    if (strncmp(params, "eq", 2) == 0) {
      params += 3;
      sname_filter(Param::eq, params, filters);
      found = true;
    }
    if (strncmp(params, "starts", 6) == 0) {
      params += 7;
      sname_filter(Param::starts, params, filters);
      found = true;
    }
    if (strncmp(params, "null", 4) == 0) {
      params += 5;
      sname_filter(Param::null, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "phone", 5) == 0) {
    params += 6;
    if (strncmp(params, "code", 4) == 0) {
      params += 5;
      phone_filter(Param::code, params, filters);
      found = true;
    }
    if (strncmp(params, "null", 4) == 0) {
      params += 5;
      phone_filter(Param::null, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "country", 7) == 0) {
    params += 8;
    if (strncmp(params, "eq", 2) == 0) {
      params += 3;
      county_filter(Param::eq, params, filters);
      found = true;
    }
    if (strncmp(params, "null", 4) == 0) {
      params += 5;
      county_filter(Param::null, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "city", 4) == 0) {
    params += 5;
    if (strncmp(params, "eq", 2) == 0) {
      params += 3;
      city_filter(Param::eq, params, filters);
      found = true;
    }
    if (strncmp(params, "any", 3) == 0) {
      params += 4;
      city_filter(Param::any, params, filters);
      found = true;
    }
    if (strncmp(params, "null", 4) == 0) {
      params += 5;
      city_filter(Param::null, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "birth", 5) == 0) {
    params += 6;
    if (strncmp(params, "lt", 2) == 0) {
      params += 3;
      birth_filter(Param::lt, params, filters);
      found = true;
    }
    if (strncmp(params, "gt", 2) == 0) {
      params += 3;
      birth_filter(Param::gt, params, filters);
      found = true;
    }
    if (strncmp(params, "year", 4) == 0) {
      params += 5;
      birth_filter(Param::year, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "interests", 9) == 0) {
    params += 10;
    if (strncmp(params, "contains", 8) == 0) {
      params += 9;
      interests_filter(Param::contains, params, filters);
      found = true;
    }
    if (strncmp(params, "any", 3) == 0) {
      params += 4;
      interests_filter(Param::any, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "likes", 5) == 0) {
    params += 6;
    if (strncmp(params, "contains", 8) == 0) {
      params += 9;
      likes_filter(Param::contains, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "premium", 7) == 0) {
    params += 8;
    if (strncmp(params, "now ", 3) == 0) {
      params += 4;
      premium_filter(Param::now, params, filters);
      found = true;
    }
    if (strncmp(params, "null", 4) == 0) {
      params += 5;
      premium_filter(Param::null, params, filters);
      found = true;
    }
  }
  if (strncmp(params, "query_id", 8) == 0) {
    params += 9;
    found = true;
  }
  if (strncmp(params, "limit", 5) == 0) {
    params += 6;
    limit = static_cast<int>(strtol(params, &params, 10));
    found = true;
  }
  if (!found)
    return -1;
  for (; *params && *params != '&'; params++);
  return 1;
}
char *Router::filter(char *params) {
  static char header[] =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: application/json\r\n"
      "Connection: keep-alive\r\n"
      "Content-Length: ";

  TRACE_CALL(__PRETTY_FUNCTION__)
  static thread_local filters_t filters;
  static thread_local uint64_t pages[pages_count];
  static thread_local uint64_t tmp_pages[pages_count];

  filters.clear();
  int limit = -1;
  while (true) {
    int code = nextParam(params, filters, limit);
    if (code == 1)
      continue;
    if (code == 0)
      break;
    return badRequest();
  }
  if (limit == -1) {
    return badRequest();
  }

  memcpy(pages, m_database->pages(), sizeof(uint64_t) * pages_count);

  int fields = 0;
  for (auto &filter : filters) {
    memset(tmp_pages, 0, sizeof(uint64_t) * pages_count);
    if (filter(m_database->accounts(), pages, tmp_pages, fields) == -1) {
      return badRequest();
    }

    for (int i = 0; i < pages_count; i++) {
      pages[i] &= tmp_pages[i];
    }
  }

  static thread_local char buf[1024 * 16];

  int size = 0;
  int len = 0;

  size_t header_len = strlen(header);
  len += header_len + 4 + 6;

  strncpy(buf, header, header_len);
  buf[header_len + 0] = ' ';
  buf[header_len + 1] = ' ';
  buf[header_len + 2] = ' ';
  buf[header_len + 3] = ' ';
  buf[header_len + 4] = ' ';
  buf[header_len + 5] = ' ';
  buf[header_len + 6] = '\r';
  buf[header_len + 7] = '\n';
  buf[header_len + 8] = '\r';
  buf[header_len + 9] = '\n';
  strcpy(buf + len, R"({"accounts":[)");
  len += 13;
  iterate_pages(m_database->accounts(), pages, [&size, &limit, &len, fields](int ind, auto &a, int pageNum) {
    if (limit <= size)
      return;
    if (size != 0) {
      buf[len++] = ',';
    }
    size++;
    len += a.Serialize(ind, buf + len, fields);
  });

  buf[len++] = ']';
  buf[len++] = '}';

  int offset = sprintf(buf + header_len, "%ld", len - header_len - 10);
  buf[header_len + offset] = ' ';
  buf[len++] = '\0';
  return buf;
}

void Router::sex_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    fields |= data::SEX;
    if (param != Param::eq) {
      std::cout << "SEX incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "SEX incorrect argument" << std::endl;
      return -1;
    }
    iterate_pages(accounts, pages, [&tmp_pages, arg](int ind, data::Account &a, int pageNum) {
      if (a.sex() == arg[0]) {
        tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
      }
    });
    return 0;
  });
}
void Router::email_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::domain && param != Param::lt && param != Param::gt) {
      std::cout << "Email incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "Email incorrect argument" << std::endl;
      return -1;
    }
    iterate_pages(accounts, pages, [&tmp_pages, arg, param, len](int ind, data::Account &a, int pageNum) {
      if (param == Param::domain) {
        const char *email = a.email().c_str();
        for (; *email != '@'; email++);
        email++;
        if (strncmp(email, arg, static_cast<size_t>(len)) == 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      } else if (param == Param::lt) {
        if (strncmp(a.email().c_str(), arg, static_cast<size_t>(len)) < 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      } else if (param == Param::gt) {
        if (strncmp(a.email().c_str(), arg, static_cast<size_t>(len)) >= 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
void Router::status_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    fields |= data::STATUS;
    if (param != Param::eq && param != Param::neq) {
      std::cout << "Status incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "Status incorrect argument" << std::endl;
      return -1;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, arg](int ind, data::Account &a, int pageNum) {
      bool cmp = strncmp(a.status().c_str(), arg, 3) == 0;
      if ((cmp && param == Param::eq) || (!cmp && param == Param::neq)) {
        tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
      }
    });
    return 0;
  });
}
void Router::fname_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::eq && param != Param::any && param != Param::null) {
      std::cout << "Fname incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "Fname incorrect argument" << std::endl;
      return -1;
    }
    if ((param != Param::null) || (arg[0] == '0')) {
      fields |= data::FNAME;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, arg, args, len](int ind, data::Account &a, int pageNum) {
      char *as = args;
      if (param == Param::eq || param == Param::any) {
        if (strncmp(a.fname().c_str(), arg, static_cast<size_t>(len)) == 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
      if (param == Param::any) {
        while (true) {
          char *ar;
          int l = nextArg(as, ar);
          if (l == 0)
            break;
          if (strncmp(a.fname().c_str(), ar, static_cast<size_t>(l)) == 0) {
            tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
            break;
          }
        }
      } else if (param == Param::null) {
        bool cmp = a.fname().empty();
        if ((cmp && *arg == '1') || (!cmp && *arg == '0')) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
void Router::sname_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::eq && param != Param::starts && param != Param::null) {
      std::cout << "Sname incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "Sname incorrect argument" << std::endl;
      return -1;
    }

    if ((param != Param::null) || (arg[0] == '0')) {
      fields |= data::SNAME;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, arg, len](int ind, data::Account &a, int pageNum) {
      if (param == Param::eq) {
        if (strncmp(a.sname().c_str(), arg, static_cast<size_t>(len)) == 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      } else if (param == Param::null) {
        bool cmp = a.sname().empty();
        if ((cmp && *arg == '1') || (!cmp && *arg == '0')) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      } else if (param == Param::starts) {
        if (strncmp(a.sname().c_str(), arg, static_cast<size_t>(len)) == 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
void Router::phone_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::code && param != Param::null) {
      std::cout << "Phone incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "Phone incorrect argument" << std::endl;
      return -1;
    }
    if ((param != Param::null) || (arg[0] == '0')) {
      fields |= data::PHONE;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, arg](int ind, data::Account &a, int pageNum) {
      if (param == Param::code) {
        if (strncmp(a.phone().c_str() + 2, arg, 3) == 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      } else if (param == Param::null) {
        bool cmp = a.phone().empty();
        if ((cmp && *arg == '1') || (!cmp && *arg == '0')) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
void Router::county_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::eq && param != Param::null) {
      std::cout << "Country incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "Country incorrect argument" << std::endl;
      return -1;
    }
    if ((param != Param::null) || (arg[0] == '0')) {
      fields |= data::COUNTRY;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, arg, len](int ind, data::Account &a, int pageNum) {
      if (param == Param::eq) {
        if (strncmp(a.country().c_str(), arg, static_cast<size_t>(len)) == 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      } else if (param == Param::null) {
        bool cmp = a.country().empty();
        if ((cmp && *arg == '1') || (!cmp && *arg == '0')) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
void Router::city_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::eq && param != Param::any && param != Param::null) {
      std::cout << "City incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "City incorrect argument" << std::endl;
      return -1;
    }
    if ((param != Param::null) || (arg[0] == '0')) {
      fields |= data::CITY;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, arg, args, len](int ind, data::Account &a, int pageNum) {
      char *as = args;
      if (param == Param::eq || param == Param::any) {
        if (strncmp(a.city().c_str(), arg, static_cast<size_t>(len)) == 0) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
      if (param == Param::any) {
        while (true) {
          char *ar;
          int l = nextArg(as, ar);
          if (l == 0)
            break;
          if (strncmp(a.city().c_str(), ar, static_cast<size_t>(l)) == 0) {
            tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
            break;
          }
        }
      } else if (param == Param::null) {
        bool cmp = a.city().empty();
        if ((cmp && *arg == '1') || (!cmp && *arg == '0')) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
void Router::birth_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args, this](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    fields |= data::BIRTH;
    if (param != Param::lt && param != Param::gt && param != Param::year) {
      std::cout << "Birth incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "Birth incorrect argument" << std::endl;
      return -1;
    }
    uint64_t time = strtoull(arg, &arg, 10);
    uint64_t year = 1950 + time / 60 / 60 / 24 / 31 / 12;
    uint64_t curr_year = 1950 + m_database->time() / 60 / 60 / 24 / 31 / 12;
    iterate_pages(accounts, pages, [param, &tmp_pages, time, year, curr_year](int ind, data::Account &a, int pageNum) {
      if ((param == Param::lt && a.birth() < time) || (param == Param::gt && a.birth() > time)) {
        tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
      } else if (param == Param::year) {
        if (year == curr_year) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
void Router::interests_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::contains && param != Param::any) {
      std::cout << "Interests incorrect parameter" << std::endl;
      return -1;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, args](int ind, data::Account &a, int pageNum) {
      if (param == Param::contains) {
        char *as = args;
        bool success = true;
        while (true) {
          char *ar;
          int l = nextArg(as, ar);
          if (l == 0)
            break;
          if (!a.interests().count(ar)) {
            success = false;
            break;
          }
        }
        if (success) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      } else if (param == Param::any) {
        char *as = args;
        while (true) {
          char *ar;
          int l = nextArg(as, ar);
          if (l == 0)
            break;
          if (a.interests().count(std::string(ar, ar+l))) {
            tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
            break;
          }
        }
      }
    });
    return 0;
  });
}
void Router::likes_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::contains) {
      std::cout << "Likes incorrect parameter" << std::endl;
      return -1;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, args](int ind, data::Account &a, int pageNum) {
      if (param == Param::contains) {
        char *as = args;
        bool success = true;
        while (true) {
          char *ar;
          int l = nextArg(as, ar);
          if (l == 0)
            break;
          if (!a.likes().count((int) strtol(ar, &ar, 10))) {
            success = false;
            break;
          }
        }
        if (success) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
void Router::premium_filter(Param param, char *args, filters_t &filters) {
  filters.push_back([param, args, this](auto accounts, auto pages, auto tmp_pages, int &fields) mutable {
    if (param != Param::now && param != Param::null) {
      std::cout << "Premium incorrect parameter" << std::endl;
      return -1;
    }
    char *arg;
    int len = nextArg(args, arg);
    if (len == 0) {
      std::cout << "Premium incorrect argument" << std::endl;
      return -1;
    }
    if(param != Param::null || arg[0]!= '1') {
      fields |= data::PREMIUM;
    }
    iterate_pages(accounts, pages, [param, &tmp_pages, this, arg](int ind, data::Account &a, int pageNum) {
      if (param == Param::now) {
        uint64_t now_time = m_database->time();
        uint64_t finish = a.premiumFinish();
        uint64_t start = a.premiumStart();
        if ((now_time < finish) && (now_time > start)) {
          if (arg[0] == '1')
            tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        } else if (arg[0] == '0')
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
      } else if (param == Param::null) {
        if ((arg[0] == '1' && a.premiumFinish() == 0)
            || ((arg[0] == '0' && a.premiumFinish() != 0))) {
          tmp_pages[pageNum] |= ((uint64_t) 1) << ind % 64;
        }
      }
    });
    return 0;
  });
}
}
