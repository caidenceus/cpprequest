#include "url.h"


std::string Url::parse_domain(std::string const url) {
  std::string domain;
  int f_slash_cnt = 0;
  int colon_cnt = 0;

  for (std::string::iterator it = url.begin(); it != url.end(); ++it) {
    if (colon_cnt == 2 || f_slash_cnt == 3)
      break;
    else if (*it == ':')
      colon_cnt++;
    else if (*it == '/')
      f_slash_cnt++;
    else if (f_slash_cnt == 2)
      domain.push_back(*it);
  }

  return domain;
}


std::string Url::parse_resource(std::string const url) {
  std::string resource;
  int f_slash_cnt = 0;

  for (std::string::iterator it = url.begin(); it != url.end(); ++it) {
    if (*it == '/')
      f_slash_cnt++;
    if (f_slash_cnt >= 3)
      resource.push_back(*it);
  }

  return resource;
}
