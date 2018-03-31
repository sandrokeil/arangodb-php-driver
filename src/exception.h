#pragma once

#include <string>

extern "C" {
#include <php.h>
#include <Zend/zend_exceptions.h>
}

#include "php_arangodb_classes.h"

namespace arangodb { namespace fuerte { namespace php {

    class ArangoDbException : std::exception
    {
    public:
        static const int EXCEPTION = 0;
        static const int RUNTIME_EXCEPTION = 1;
        static const int INVALID_OPTION_EXCEPTION = 2;
        static const int REQUEST_FAILED_EXCEPTION = 3;
        static const int INVALID_ARGUMENT_EXCEPTION = 4;

        int type;
        int code;
        std::string message;

        ArangoDbException(int type, int code, const char* message);
        ArangoDbException(int type, int code, const std::string& message);
    };

}}}
