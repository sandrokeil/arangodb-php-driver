#include "exception.h"

namespace arangodb { namespace fuerte { namespace php {

    ArangoDbException::ArangoDbException(int type, int code, const char* message):
        type(type), code(code), message(std::string(message))
    {
    }

    ArangoDbException::ArangoDbException(int type, int code, const std::string& message):
        type(type), code(code), message(message)
    {
    }

}}}
