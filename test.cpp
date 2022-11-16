#include <iostream>
#include <string>

#include "request.h"
#include "uri.h"


int main() {
  using namespace std;

  string test = "http://192.168.1.75/private/create/add_subject.php?new-subject=testing post method";
  cppr::Post testing{test};
  testing.request();

  return 0;
}
