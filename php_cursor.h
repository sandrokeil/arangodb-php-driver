#pragma once

extern "C" {
#include <php.h>
#include <Zend/zend_interfaces.h>
}

#include "src/cursor.h"

namespace {
    zend_class_entry *cursor_ce;

    #define Z_OBJECT_CURSOR(zo) (arangodb::fuerte::php::Cursor::fetch_object(zo))
    #define Z_OBJECT_CURSOR_P(zo) (arangodb::fuerte::php::Cursor::fetch_object(Z_OBJ_P(zo)))


    PHP_METHOD(Cursor, __construct)
    {

    }

    ZEND_BEGIN_ARG_INFO_EX(arangodb_cursor_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    zend_function_entry cursor_methods[] = {
        PHP_ME(Cursor, __construct, arangodb_cursor_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_FE_END
    };

    void init_arangodb_cursor_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ArangoDb\\Cursor", cursor_methods);
        cursor_ce = zend_register_internal_class(&ce TSRMLS_CC);
        cursor_ce->create_object = arangodb::fuerte::php::Cursor::create_object;
        cursor_ce->get_iterator = arangodb::fuerte::php::Cursor::get_iterator;

        memcpy(&arangodb::fuerte::php::Cursor::handler_cursor, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Cursor::handler_cursor.offset = XtOffsetOf(arangodb::fuerte::php::Cursor, std);

        zend_class_implements(cursor_ce TSRMLS_CC, 1, zend_ce_traversable);
    }

}
