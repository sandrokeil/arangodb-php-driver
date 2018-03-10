#pragma once

extern "C" {
#include <php.h>
}

#include "src/connection.h"
#include "php_response.h"
#include "php_cursor.h"

namespace {

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
        zval object;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &request, request_ce) == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_CONNECTION_P(getThis());
        auto fuerte_response = intern->send_request(Z_OBJECT_REQUEST_P(request));

        if(!fuerte_response) {
            return;
        }

        object_init_ex(&object, response_ce);
        auto response = Z_OBJECT_RESPONSE(Z_OBJ(object));
        new (response) arangodb::fuerte::php::Response(*fuerte_response);

        RETURN_ZVAL(&object, 1, 0);
    }


    #define PHP_CONNECTION_METHOD_X(http_method)                                                                                        \
        zval object;                                                                                                                    \
        const char* path;                                                                                                               \
        size_t path_length;                                                                                                             \
        zval* vpack_value;                                                                                                              \
        zval* query_params = NULL;                                                                                                      \
                                                                                                                                        \
        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz|a", &path, &path_length, &vpack_value, &query_params) == FAILURE) {     \
            return;                                                                                                                     \
        }                                                                                                                               \
                                                                                                                                        \
        auto intern = Z_OBJECT_CONNECTION_P(getThis());                                                                                 \
        std::unique_ptr<fu::Response> fuerte_response;                                                                                  \
                                                                                                                                        \
        if(Z_TYPE_P(vpack_value) == IS_STRING) {                                                                                        \
            fuerte_response = intern->send(http_method, path, Z_STRVAL_P(vpack_value), query_params ? Z_ARRVAL_P(query_params) : NULL); \
        } else if(Z_TYPE_P(vpack_value) == IS_ARRAY) {                                                                                  \
            fuerte_response = intern->send(http_method, path, Z_ARRVAL_P(vpack_value), query_params ? Z_ARRVAL_P(query_params) : NULL); \
        } else {                                                                                                                        \
            ARANGODB_THROW_CE(invalid_argument_exception_ce, 0, "Vpack must be of type string (JSON) or array in %s on line %d");       \
            return;                                                                                                                     \
        }                                                                                                                               \
                                                                                                                                        \
        if(!fuerte_response) {                                                                                                          \
            return;                                                                                                                     \
        }                                                                                                                               \
                                                                                                                                        \
        object_init_ex(&object, response_ce);                                                                                           \
        auto response = Z_OBJECT_RESPONSE(Z_OBJ(object));                                                                               \
        new (response) arangodb::fuerte::php::Response(*fuerte_response);                                                               \
                                                                                                                                        \
        RETURN_ZVAL(&object, 1, 0);

    ZEND_NAMED_FUNCTION(zim_Connection_delete)
    {
        PHP_CONNECTION_METHOD_X(0)
    }

    PHP_METHOD(Connection, get)
    {
        PHP_CONNECTION_METHOD_X(1)
    }

    PHP_METHOD(Connection, post)
    {
        PHP_CONNECTION_METHOD_X(2)
    }

    PHP_METHOD(Connection, put)
    {
        PHP_CONNECTION_METHOD_X(3)
    }

    PHP_METHOD(Connection, head)
    {
        PHP_CONNECTION_METHOD_X(4)
    }

    PHP_METHOD(Connection, patch)
    {
        PHP_CONNECTION_METHOD_X(5)
    }

    PHP_METHOD(Connection, options)
    {
        PHP_CONNECTION_METHOD_X(6)
    }

    PHP_METHOD(Connection, query)
    {
        zval cursor_object;
        zval* vpack_value;
        zval* cursor_options = NULL;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|a", &vpack_value, &cursor_options) == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_CONNECTION_P(getThis());
        std::unique_ptr<fu::Response> fuerte_response;

        if(Z_TYPE_P(vpack_value) == IS_STRING) {
            fuerte_response = intern->send(2, "/_api/cursor", Z_STRVAL_P(vpack_value), NULL);
        } else if(Z_TYPE_P(vpack_value) == IS_ARRAY) {
            fuerte_response = intern->send(2, "/_api/cursor", Z_ARRVAL_P(vpack_value), NULL);
        } else {
            ARANGODB_THROW_CE(invalid_argument_exception_ce, 0, "Vpack must be of type string (JSON) or array in %s on line %d");
            return;
        }

        if(!fuerte_response) {
            return;
        }

        object_init_ex(&cursor_object, cursor_ce);
        auto cursor = Z_OBJECT_CURSOR(Z_OBJ(cursor_object));
        new (cursor) arangodb::fuerte::php::Cursor(intern, std::move(fuerte_response));

        if(cursor_options) {
            long key;
            zval* data;
            ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(cursor_options), key, data) {
                cursor->set_option(key, Z_LVAL_P(data));
            } ZEND_HASH_FOREACH_END();
        }

        RETURN_ZVAL(&cursor_object, 1, 0)
    }

    PHP_METHOD(Connection, setThreadCount) {
        zend_long thread_count;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &thread_count) == FAILURE) {
            return;
        }

        auto connection = Z_OBJECT_CONNECTION_P(getThis());
        connection->set_thread_count(static_cast<int>(thread_count));
    }

    PHP_METHOD(Connection, setDefaultTimeout) {
        zend_long default_timeout;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &default_timeout) == FAILURE) {
            return;
        }

        auto connection = Z_OBJECT_CONNECTION_P(getThis());
        connection->set_default_timeout(static_cast<int>(default_timeout));
    }


    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_construct, 0, 0, 1)
        ZEND_ARG_INFO(0, options)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_void, 0, 0, 0) 
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_send, 0, 0, 1)
        ZEND_ARG_INFO(0, request)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_method_x, 0, 0, 3)
        ZEND_ARG_INFO(0, path)
        ZEND_ARG_INFO(0, vpackValue)
        ZEND_ARG_INFO(0, queryParams)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_query, 0, 0, 2)
        ZEND_ARG_INFO(0, vpackValue)
        ZEND_ARG_INFO(0, cursorOptions)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_set_thread_count, 0, 0, 1)
        ZEND_ARG_INFO(0, thread_count)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_connection_set_default_timeout, 0, 0, 1)
        ZEND_ARG_INFO(0, default_timeout)
    ZEND_END_ARG_INFO()

    zend_function_entry connection_methods[] = {
        PHP_ME(Connection, __construct, arangodb_connection_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(Connection, connect, arangodb_connection_void, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, send, arangodb_connection_send, ZEND_ACC_PUBLIC)
        ZEND_RAW_FENTRY("delete", zim_Connection_delete, arangodb_connection_method_x, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, get, arangodb_connection_method_x, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, post, arangodb_connection_method_x, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, put, arangodb_connection_method_x, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, head, arangodb_connection_method_x, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, patch, arangodb_connection_method_x, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, options, arangodb_connection_method_x, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, query, arangodb_connection_query, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, setThreadCount, arangodb_connection_set_thread_count, ZEND_ACC_PUBLIC)
        PHP_ME(Connection, setDefaultTimeout, arangodb_connection_set_default_timeout, ZEND_ACC_PUBLIC)
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
