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

    ArangoDbRequestFailedException::ArangoDbRequestFailedException(int code, const char* message, int http_code, const std::string& body):
        ArangoDbRuntimeException(code, message), http_code(http_code), body(body)
    {
    }

    ArangoDbRequestFailedException::ArangoDbRequestFailedException(int code, const std::string& message, int http_code, const std::string& body):
        ArangoDbRuntimeException(code, message), http_code(http_code), body(body)
    {
    }

}}}
