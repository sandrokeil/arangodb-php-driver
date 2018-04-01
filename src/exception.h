#pragma once

#include <string>

namespace arangodb { namespace fuerte { namespace php {

    class ArangoDbException : std::exception
    {
    public:
        int code;
        std::string message;

        ArangoDbException(int code, const char* message);
        ArangoDbException(int code, const std::string& message);
    };

    class ArangoDbRuntimeException : public ArangoDbException
    {
        using ArangoDbException::ArangoDbException;
    };

    class ArangoDbInvalidOptionException : public ArangoDbRuntimeException
    {
        using ArangoDbRuntimeException::ArangoDbRuntimeException;
    };

    class ArangoDbRequestFailedException : public ArangoDbRuntimeException
    {
        using ArangoDbRuntimeException::ArangoDbRuntimeException;
    };

    class ArangoDbInvalidArgumentException : public ArangoDbException
    {
        using ArangoDbException::ArangoDbException;
    };

}}}
