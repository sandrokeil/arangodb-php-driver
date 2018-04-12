#pragma once

extern "C" {
#include <php.h>
}

#include "src/response.h"

namespace {

    PHP_METHOD(Response, getHttpCode)
    {
        ARANGODB_EXCEPTION_CONVERTER_TRY

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_RESPONSE_P(getThis());
        RETURN_LONG(intern->get_http_code());

        ARANGODB_EXCEPTION_CONVERTER_CATCH
    }

    PHP_METHOD(Response, getBody)
    {
        ARANGODB_EXCEPTION_CONVERTER_TRY

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_RESPONSE_P(getThis());
        intern->return_body(return_value);

        ARANGODB_EXCEPTION_CONVERTER_CATCH
    }

    PHP_METHOD(Response, get)
    {
        ARANGODB_EXCEPTION_CONVERTER_TRY

        zval *accessor;
        HashTable *accessor_ht;
        const char* accessor_string;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &accessor) == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_RESPONSE_P(getThis());

        if(Z_TYPE_P(accessor) == IS_STRING) {
            accessor_string = Z_STRVAL_P(accessor);
            intern->get(return_value, accessor_string);
        } else if(Z_TYPE_P(accessor) == IS_ARRAY) {
            accessor_ht = Z_ARRVAL_P(accessor);
            intern->get(return_value, accessor_ht);
        } else {
            throw arangodb::fuerte::php::ArangoDbInvalidArgumentException(
                0,
                "Accessor must be of type string (top level) or array (nested values)"
            );
            return;
        }

        ARANGODB_EXCEPTION_CONVERTER_CATCH
    }

    PHP_METHOD(Response, assertSuccess)
    {
        ARANGODB_EXCEPTION_CONVERTER_TRY

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_RESPONSE_P(getThis());
        intern->assert_success();

        ARANGODB_EXCEPTION_CONVERTER_CATCH
    }

    ZEND_BEGIN_ARG_INFO_EX(arangodb_response_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_response_get, 0, 0, 1)
        ZEND_ARG_INFO(0, accessor)
    ZEND_END_ARG_INFO()

    zend_function_entry response_methods[] = {
        PHP_ME(Response, getHttpCode, arangodb_response_void, ZEND_ACC_PUBLIC)
        PHP_ME(Response, getBody, arangodb_response_void, ZEND_ACC_PUBLIC)
        PHP_ME(Response, get, arangodb_response_get, ZEND_ACC_PUBLIC)
        PHP_ME(Response, assertSuccess, arangodb_response_void, ZEND_ACC_PUBLIC)
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
