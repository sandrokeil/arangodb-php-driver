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
    public:
        int http_code;
        std::string body;

        ArangoDbRequestFailedException(int code, const char* message, int http_code, const std::string& body);
        ArangoDbRequestFailedException(int code, const std::string& message, int http_code, const std::string& body);
        ArangoDbRequestFailedException(int code, const char* message, int http_code);
        ArangoDbRequestFailedException(int code, const std::string& message, int http_code);
    };

    class ArangoDbInvalidArgumentException : public ArangoDbException
    {
        using ArangoDbException::ArangoDbException;
    };

}}}
