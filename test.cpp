#include <iostream>
#include <string>

#include "request.h"
#include "uri.h"


int main() {
  using namespace std;

  string test = "http://192.168.1.75/public/index.php";
  cppr::Get testing{test};
  testing.request();

  return 0;
}
