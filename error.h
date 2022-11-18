#include <system_error>


namespace cppr {
  namespace error {
    
    class RequestError final: public std::logic_error {
      public:
        using std::logic_error::logic_error;
    };

  }
}
