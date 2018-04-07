#pragma once

extern "C" {
#include <php.h>
}

#include "src/request.h"

namespace {

    PHP_METHOD(Request, __construct)
    {
        ARANGODB_EXCEPTION_CONVERTER_TRY

        zend_long method;
        const char* path;
        size_t path_length;
        zval* vpack_value;
        zval* query_params = NULL;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lsz|a",
            &method,
            &path, &path_length,
            &vpack_value,
            &query_params) == FAILURE
        ) {
            return;
        }

        auto intern = Z_OBJECT_REQUEST_P(getThis());;
/*
        intern->set_http_method(method);
        intern->set_path(std::string(path, path_length));
/*
        if(Z_TYPE_P(vpack_value) == IS_STRING) {
            intern->set_vpack_from_json(Z_STRVAL_P(vpack_value));
        } else if(Z_TYPE_P(vpack_value) == IS_ARRAY) {
            intern->set_vpack_from_array(Z_ARRVAL_P(vpack_value));
        } else {
            ARANGODB_THROW_CE(invalid_argument_exception_ce, 0, "Vpack must be of type string (JSON) or array in %s on line %d");
            return;
        }

        if(query_params != NULL) {
            intern->set_query_params(Z_ARRVAL_P(query_params));
        }
*/
        ARANGODB_EXCEPTION_CONVERTER_CATCH
    }


    ZEND_BEGIN_ARG_INFO_EX(arangodb_request_construct, 0, 0, 3)
        ZEND_ARG_INFO(0, method)
        ZEND_ARG_INFO(0, path)
        ZEND_ARG_INFO(0, vpack_value)
        ZEND_ARG_INFO(0, query_params)
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

        zend_declare_class_constant_long(request_ce, "METHOD_DELETE", sizeof("METHOD_DELETE") - 1,      0 TSRMLS_CC);
        zend_declare_class_constant_long(request_ce, "METHOD_GET", sizeof("METHOD_GET") - 1,            1 TSRMLS_CC);
        zend_declare_class_constant_long(request_ce, "METHOD_POST", sizeof("METHOD_POST") - 1,          2 TSRMLS_CC);
        zend_declare_class_constant_long(request_ce, "METHOD_PUT", sizeof("METHOD_PUT") - 1,            3 TSRMLS_CC);
        zend_declare_class_constant_long(request_ce, "METHOD_HEAD", sizeof("METHOD_HEAD") - 1,          4 TSRMLS_CC);
        zend_declare_class_constant_long(request_ce, "METHOD_PATCH", sizeof("METHOD_PATCH") - 1,        5 TSRMLS_CC);
        zend_declare_class_constant_long(request_ce, "METHOD_OPTIONS", sizeof("METHOD_OPTIONS") - 1,    6 TSRMLS_CC);

        memcpy(&arangodb::fuerte::php::Request::handler_request, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Request::handler_request.offset = XtOffsetOf(arangodb::fuerte::php::Request, std);
    }

}
