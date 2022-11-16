#include <iostream>
#include <string>

#include "request.h"
#include "uri.h"


int main() {
  using namespace std;

  string test = "http://192.168.1.75/public/index.php";
  cppr::Post testing{test, cppr::HttpVersion::OneDotOne};
  testing.request();

  return 0;
}
