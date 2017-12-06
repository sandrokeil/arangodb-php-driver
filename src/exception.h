#pragma once

#include <cmath>
using namespace std;

#include <Zend/zend.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_API.h>

#define ARANGODB_THROW(ex, message) zend_throw_exception_ex(ex, 0, message, __FILE__, __LINE__);


namespace arangodb { namespace fuerte { namespace php {

    static zend_class_entry* _RuntimeException;
    static zend_class_entry* _InvalidOptionException;


    static zend_class_entry* RuntimeException() {
        zend_string *exceptionClassName = zend_string_tolower(zend_string_init(ZEND_STRL("ArangoDb\\RuntimeException"), 1));
        zend_class_entry *exceptionClass = static_cast<zend_class_entry*>(zend_hash_find_ptr(CG(class_table), exceptionClassName));
        zend_string_release(exceptionClassName);

        return exceptionClass;
    }

    static zend_class_entry* InvalidOptionException() {
        zend_string *exceptionClassName = zend_string_tolower(zend_string_init(ZEND_STRL("ArangoDb\\InvalidOptionException"), 1));
        zend_class_entry *exceptionClass = static_cast<zend_class_entry*>(zend_hash_find_ptr(CG(class_table), exceptionClassName));
        zend_string_release(exceptionClassName);

        return exceptionClass;
    }


    static void registerRuntimeException()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\RuntimeException", NULL);
        _RuntimeException = zend_register_internal_class_ex(&ce, zend_exception_get_default());
    }

    static void registerInvalidOptionException()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\InvalidOptionException", NULL);
        _InvalidOptionException = zend_register_internal_class_ex(&ce, _RuntimeException);
    }

    static void registerCustomExceptions()
    {
        registerRuntimeException();
        registerInvalidOptionException();
    }

}}}
