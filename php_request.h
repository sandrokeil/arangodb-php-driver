#pragma once

extern "C" {
#include <php.h>
}

#include "src/request.h"

namespace {
    zend_class_entry *request_ce;

    #define Z_OBJECT_REQUEST(zo) (arangodb::fuerte::php::Request::fetch_object(zo))
    #define Z_OBJECT_REQUEST_P(zo) (arangodb::fuerte::php::Request::fetch_object(Z_OBJ_P(zo)))


    PHP_METHOD(Request, __construct)
    {
        long method;
        const char* path;
        int path_length;
        zval vpack;
        zval query_params;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lso|a", &method, &path, &path_length, &vpack, &query_params) == FAILURE) {
            return;
        }

        arangodb::fuerte::php::Request* intern;
        intern = Z_OBJECT_REQUEST_P(getThis());

        intern->http_method = static_cast<fu::RestVerb>(method);
        intern->path = std::string(path, path_length);
    }


    ZEND_BEGIN_ARG_INFO_EX(arangodb_request_construct, 0, 0, 0)
    ZEND_END_ARG_INFO()

    zend_function_entry request_methods[] = {
        PHP_ME(Request, __construct, arangodb_request_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_FE_END
    };

    void init_arangodb_request_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ArangoDb\\Request", request_methods);
        request_ce = zend_register_internal_class(&ce TSRMLS_CC);
        request_ce->create_object = arangodb::fuerte::php::Request::create_object;

        memcpy(&arangodb::fuerte::php::Request::handler_request, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Request::handler_request.offset = XtOffsetOf(arangodb::fuerte::php::Request, std);
    }

}
