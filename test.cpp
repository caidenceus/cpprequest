#include <iostream>
#include <string>

#include "request.h"

int main() {
  using namespace std;

  string test = "http://192.168.1.75/public/index.php";
  Get req = new Get(test);
  req.request();

  return 0;
}
