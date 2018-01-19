#pragma once

extern "C" {
#include <php.h>
}

#include "vpack.h"

#include <iostream>

#include <velocypack/vpack.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Request
    {
    private:
        fu::RestVerb http_method;
        std::string path;
        vp::Builder builder;
        std::map<std::string, std::string> query_params;

    public:
        zend_object std;

        static zend_object_handlers handler_request;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Request* fetch_object(zend_object* obj);

        void set_http_method(int http_method);
        void set_path(const std::string& path);
        void set_vpack(const Vpack* vpack);
        void set_query_params(HashTable* query_params);

        std::unique_ptr<fu::Request> get_fuerte_request();
    };

}}}
