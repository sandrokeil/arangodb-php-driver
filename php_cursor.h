#pragma once

extern "C" {
#include <php.h>
#include <Zend/zend_interfaces.h>
}

#include "src/cursor.h"

namespace {

    PHP_METHOD(Cursor, count)
    {
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        auto intern = Z_OBJECT_CURSOR_P(getThis());
        RETURN_LONG(intern->count());
    }

    ZEND_BEGIN_ARG_INFO_EX(arangodb_cursor_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    zend_function_entry cursor_methods[] = {
        PHP_ME(Cursor, count, arangodb_cursor_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_FE_END
    };

    void init_arangodb_cursor_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ArangoDb\\Cursor", cursor_methods);
        cursor_ce = zend_register_internal_class(&ce TSRMLS_CC);
        cursor_ce->create_object = arangodb::fuerte::php::Cursor::create_object;
        cursor_ce->get_iterator = arangodb::fuerte::php::Cursor::get_iterator;

        zend_declare_class_constant_long(cursor_ce, "ENTRY_TYPE", sizeof("ENTRY_TYPE") - 1,
            arangodb::fuerte::php::Cursor::ENTRY_TYPE TSRMLS_CC);
        zend_declare_class_constant_long(cursor_ce, "ENTRY_TYPE_JSON", sizeof("ENTRY_TYPE_JSON") - 1,
            arangodb::fuerte::php::Cursor::ENTRY_TYPE_JSON TSRMLS_CC);
        zend_declare_class_constant_long(cursor_ce, "ENTRY_TYPE_ARRAY", sizeof("ENTRY_TYPE_ARRAY") - 1,
            arangodb::fuerte::php::Cursor::ENTRY_TYPE_ARRAY TSRMLS_CC);

        memcpy(&arangodb::fuerte::php::Cursor::handler_cursor, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Cursor::handler_cursor.offset = XtOffsetOf(arangodb::fuerte::php::Cursor, std);

        zend_class_implements(cursor_ce TSRMLS_CC, 1, zend_ce_traversable);
    }

}
