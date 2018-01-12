#pragma once

extern "C" {
#include <php.h>
}

#include <iostream>

#include <velocypack/vpack.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Request
    {
    public:
        std::unique_ptr<fu::Request> request;
        fu::RestVerb http_method;
        std::string path;
        zend_object std;

        static zend_object_handlers handler_request;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Request* fetch_object(zend_object* obj);
    private:
    };

}}}
