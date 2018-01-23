#pragma once

extern "C" {
#include <php.h>
}

#include "src/response.h"

namespace {
    zend_class_entry *response_ce;

    #define Z_OBJECT_RESPONSE(zo) (arangodb::fuerte::php::Response::fetch_object(zo))
    #define Z_OBJECT_RESPONSE_P(zo) (arangodb::fuerte::php::Response::fetch_object(Z_OBJ_P(zo)))

    PHP_METHOD(Response, getHttpCode)
    {
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_RESPONSE_P(getThis());
        RETURN_LONG(intern->get_http_code());
    }

    PHP_METHOD(Response, getBody)
    {
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_RESPONSE_P(getThis());
        intern->return_body(return_value);
    }

    PHP_METHOD(Response, getVpack)
    {
        zval vpack_object;

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_RESPONSE_P(getThis());

        object_init_ex(&vpack_object, vpack_ce);
        auto vpack = Z_OBJECT_VPACK(Z_OBJ(vpack_object));
        vpack->from_slice(intern->get_fuerte_response()->slices().front());

        RETURN_ZVAL(&vpack_object, 1, 0)
    }

    ZEND_BEGIN_ARG_INFO_EX(arangodb_response_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    zend_function_entry response_methods[] = {
        PHP_ME(Response, getHttpCode, arangodb_response_void, ZEND_ACC_PUBLIC)
        PHP_ME(Response, getBody, arangodb_response_void, ZEND_ACC_PUBLIC)
        PHP_ME(Response, getVpack, arangodb_response_void, ZEND_ACC_PUBLIC)
        PHP_FE_END
    };

    void init_arangodb_response_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ArangoDb\\Response", response_methods);
        response_ce = zend_register_internal_class(&ce TSRMLS_CC);
        response_ce->create_object = arangodb::fuerte::php::Response::create_object;

        memcpy(&arangodb::fuerte::php::Response::handler_response, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Response::handler_response.offset = XtOffsetOf(arangodb::fuerte::php::Response, std);
    }

}
