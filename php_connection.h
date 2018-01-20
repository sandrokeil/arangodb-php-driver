#pragma once

extern "C" {
#include <php.h>
}

#include "src/connection.h"

namespace {
    zend_class_entry *connection_ce;

    #define Z_OBJECT_CONNECTION(zo) (arangodb::fuerte::php::Connection::fetch_object(zo))
    #define Z_OBJECT_CONNECTION_P(zo) (arangodb::fuerte::php::Connection::fetch_object(Z_OBJ_P(zo)))


    PHP_METHOD(Connection, __construct)
    {
        zval* options;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &options) == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_CONNECTION_P(getThis());
        intern->set_options(Z_ARRVAL_P(options));
    }

    PHP_METHOD(Connection, connect) 
    {
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_CONNECTION_P(getThis());
        intern->connect();
    }

    PHP_METHOD(Connection, send)
    {
        zval* request;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &request, request_ce) == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_CONNECTION_P(getThis());
        intern->send_request(Z_OBJECT_REQUEST_P(request));
    }


    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_construct, 0, 0, 1)
        ZEND_ARG_INFO(0, options)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_void, 0, 0, 0) 
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_send, 0, 0, 1)
        ZEND_ARG_INFO(0, request)
    ZEND_END_ARG_INFO()

    zend_function_entry connection_methods[] = {
        PHP_ME(Connection, __construct, arangodb_connection_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(Connection, connect, arangodb_connection_void, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, send, arangodb_connection_send, ZEND_ACC_PUBLIC)
        PHP_FE_END
    };

    void init_arangodb_connection_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ArangoDb\\Connection", connection_methods);
        connection_ce = zend_register_internal_class(&ce TSRMLS_CC);
        connection_ce->create_object = arangodb::fuerte::php::Connection::create_object;

        zend_declare_class_constant_long(connection_ce, "HOST", sizeof("HOST") - 1,
            arangodb::fuerte::php::Connection::OPTION_HOST TSRMLS_CC);
        zend_declare_class_constant_long(connection_ce, "USER", sizeof("USER") - 1,
            arangodb::fuerte::php::Connection::OPTION_USER TSRMLS_CC);
        zend_declare_class_constant_long(connection_ce, "PASSWORD", sizeof("PASSWORD") - 1,
            arangodb::fuerte::php::Connection::OPTION_PASSWORD TSRMLS_CC);
        zend_declare_class_constant_long(connection_ce, "MAX_CHUNK_SIZE", sizeof("MAX_CHUNK_SIZE") - 1,
            arangodb::fuerte::php::Connection::OPTION_MAX_CHUNK_SIZE TSRMLS_CC);
        zend_declare_class_constant_long(connection_ce, "VST_VERSION", sizeof("VST_VERSION") - 1,
            arangodb::fuerte::php::Connection::OPTION_VST_VERSION TSRMLS_CC);

        memcpy(&arangodb::fuerte::php::Connection::handler_connection, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Connection::handler_connection.offset = XtOffsetOf(arangodb::fuerte::php::Connection, std);
    }

}
