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

}
