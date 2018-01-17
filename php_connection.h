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
        intern->options = Z_ARRVAL_P(options);
    }


    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_construct, 0, 0, 0)
    ZEND_END_ARG_INFO()

    zend_function_entry connection_methods[] = {
        PHP_ME(Connection, __construct, arangodb_connection_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_FE_END
    };

    void init_arangodb_connection_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ArangoDb\\Connection", connection_methods);
        connection_ce = zend_register_internal_class(&ce TSRMLS_CC);
        connection_ce->create_object = arangodb::fuerte::php::Connection::create_object;

        memcpy(&arangodb::fuerte::php::Connection::handler_connection, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Connection::handler_connection.offset = XtOffsetOf(arangodb::fuerte::php::Connection, std);
    }

}
