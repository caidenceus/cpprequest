#include <iostream>
#include <string>

#include "uri.h"

int main() {
  using namespace std;

  string test = "ssh://admin:penis@192.168.1.75/public/index.php?test=penis#penis";
  cppr::Uri testing = cppr::parse_uri(test);

  cout << "\nscheme: " + testing.scheme;
  cout << "\nuser: " + testing.user;
  cout << "\npassword: "+ testing.password;
  cout << "\nhost: " + testing.host;
  cout << "\nport: " + testing.port;
  cout << "\npath: " + testing.path;
  cout << "\nquery: " + testing.query;
  cout << "\nfragment: " + testing.fragment + "\n";

  return 0;
}
