#include <iostream>
#include <string>

#include "uri.h"

int main() {
  using namespace std;

  string test = "http://192.168.1.75/public/index.php";
  Uri::Uri testing = Uri::parse_uri(test);

  return 0;
}
