#include "exception.h"

namespace arangodb { namespace fuerte { namespace php {

    ArangoDbException::ArangoDbException(int code, const char* message):
        code(code), message(std::string(message))
    {
    }

    ArangoDbException::ArangoDbException(int code, const std::string& message):
        code(code), message(message)
    {
    }

}}}
