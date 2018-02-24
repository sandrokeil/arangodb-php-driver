#pragma once

extern "C" {
#include <php.h>
}

namespace {

    zend_class_entry *connection_ce;
    zend_class_entry *cursor_ce;
    zend_class_entry *request_ce;
    zend_class_entry *response_ce;

    #define Z_OBJECT_CONNECTION(zo) (arangodb::fuerte::php::Connection::fetch_object(zo))
    #define Z_OBJECT_CONNECTION_P(zo) (arangodb::fuerte::php::Connection::fetch_object(Z_OBJ_P(zo)))
    #define Z_OBJECT_CURSOR(zo) (arangodb::fuerte::php::Cursor::fetch_object(zo))
    #define Z_OBJECT_CURSOR_P(zo) (arangodb::fuerte::php::Cursor::fetch_object(Z_OBJ_P(zo)))
    #define Z_OBJECT_REQUEST(zo) (arangodb::fuerte::php::Request::fetch_object(zo))
    #define Z_OBJECT_REQUEST_P(zo) (arangodb::fuerte::php::Request::fetch_object(Z_OBJ_P(zo)))
    #define Z_OBJECT_RESPONSE(zo) (arangodb::fuerte::php::Response::fetch_object(zo))
    #define Z_OBJECT_RESPONSE_P(zo) (arangodb::fuerte::php::Response::fetch_object(Z_OBJ_P(zo)))


    zend_class_entry *exception_ce;
    zend_class_entry *runtime_exception_ce;
    zend_class_entry *invalid_option_exception_ce;
    zend_class_entry *request_failed_exception_ce;
    zend_class_entry *invalid_argument_exception_ce;

    #define ARANGODB_THROW_CE(ce, code, message) zend_throw_exception_ex(ce, code, message, __FILE__, __LINE__);
    #define ARANGODB_THROW(ce, obj, code, message) \
        zend_update_property_string(ce, obj, "message", sizeof("message") - 1, message); \
        zend_update_property_long(ce, obj, "code", sizeof("code") - 1, code); \
        zend_throw_exception_object(obj);
}
