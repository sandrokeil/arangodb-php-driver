#pragma once

#include <cmath>
using namespace std;

#include <Zend/zend.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_API.h>
#include <php.h>

#define ARANGODB_THROW(ex, message) zend_throw_exception_ex(ex, 0, message, __FILE__, __LINE__);



namespace {
    /*
        Exception hierarchy

        \Exception
            \ArangoDb\Exception
                \ArangoDb\RuntimeException
                    \ArangoDb\InvalidOptionException
                    \ArangoDb\RequestFailedException

                \Arangodb\InvalidArgumentException
    */

    static zend_class_entry* _Exception;
    static zend_class_entry* _RuntimeException;
    static zend_class_entry* _InvalidOptionException;
    static zend_class_entry* _RequestFailedException;
    static zend_class_entry* _InvalidArgumentException;


    PHP_METHOD(RequestFailedException, getBody)
    {
        zval *obj, *value;

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        obj = getThis();

        value  = zend_read_property(_RequestFailedException, obj, "body", sizeof("body") - 1, 1, NULL TSRMLS_CC);

        RETURN_ZVAL(value, 1, 0);
    }

    PHP_METHOD(RequestFailedException, getHttpCode)
    {
        zval *obj, *value;

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        obj = getThis();

        value  = zend_read_property(_RequestFailedException, obj, "code", sizeof("code") - 1, 1, NULL TSRMLS_CC);

        RETURN_ZVAL(value, 1, 0);
    }


    ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    const zend_function_entry request_failed_exception_functions[] = {
        PHP_ME(RequestFailedException, getBody, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(RequestFailedException, getHttpCode, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_FE_END
    };
}


namespace arangodb { namespace fuerte { namespace php {

    static zend_class_entry* Exception() {
        zend_string *exceptionClassName = zend_string_tolower(zend_string_init(ZEND_STRL("ArangoDb\\Exception"), 1));
        zend_class_entry *exceptionClass = static_cast<zend_class_entry*>(zend_hash_find_ptr(CG(class_table), exceptionClassName));
        zend_string_release(exceptionClassName);

        return exceptionClass;
    }

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

    static zend_class_entry* RequestFailedException() {
        zend_string *exceptionClassName = zend_string_tolower(zend_string_init(ZEND_STRL("ArangoDb\\RequestFailedException"), 1));
        zend_class_entry *exceptionClass = static_cast<zend_class_entry*>(zend_hash_find_ptr(CG(class_table), exceptionClassName));
        zend_string_release(exceptionClassName);

        return exceptionClass;
    }

    static void throwRequestFailedException(const char* message, const int code, const char* body) {
        zval exObj;
        zend_class_entry* exceptionClass = RequestFailedException();

        object_init_ex(&exObj, exceptionClass);

        //zend_declare_property_long(_RequestFailedException, "code", sizeof("code") - 1, code, ZEND_ACC_PROTECTED TSRMLS_CC);

        zend_update_property_string(exceptionClass, &exObj, "message", sizeof("message") - 1, message);
        zend_update_property_long(exceptionClass, &exObj, "code", sizeof("code") - 1, code);
        zend_update_property_string(exceptionClass, &exObj, "body", sizeof("body") - 1, body);

        zend_throw_exception_object(&exObj);
    }

    static zend_class_entry* InvalidArgumentException() {
        zend_string *exceptionClassName = zend_string_tolower(zend_string_init(ZEND_STRL("ArangoDb\\InvalidArgumentException"), 1));
        zend_class_entry *exceptionClass = static_cast<zend_class_entry*>(zend_hash_find_ptr(CG(class_table), exceptionClassName));
        zend_string_release(exceptionClassName);

        return exceptionClass;
    }


    static void registerException()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\Exception", NULL);
        _Exception = zend_register_internal_class_ex(&ce, zend_exception_get_default());
    }

    static void registerRuntimeException()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\RuntimeException", NULL);
        _RuntimeException = zend_register_internal_class_ex(&ce, _Exception);
    }

    static void registerInvalidOptionException()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\InvalidOptionException", NULL);
        _InvalidOptionException = zend_register_internal_class_ex(&ce, _RuntimeException);
    }

    static void registerRequestFailedException()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\RequestFailedException", request_failed_exception_functions);
        _RequestFailedException = zend_register_internal_class_ex(&ce, _RuntimeException);
    }

    static void registerInvalidArgumentException()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\InvalidArgumentException", NULL);
        _RuntimeException = zend_register_internal_class_ex(&ce, _Exception);
    }

    static void registerCustomExceptions()
    {
        registerException();
        registerRuntimeException();
        registerInvalidOptionException();
        registerRequestFailedException();
        registerInvalidArgumentException();
    }

}}}
