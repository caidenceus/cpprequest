#include <stdexcept>


namespace cppr {
  namespace error {
    
    class RequestError final: public std::logic_error {
      public:
        using std::logic_error::logic_error;
    };


    class SocketIoError final: public std::runtime_error {
      public:
        using std::runtime_error::runtime_error;
    };

  }
}
