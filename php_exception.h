#pragma once

extern "C" {
#include <php.h>
#include <Zend/zend_exceptions.h>
}

/*
    Exception hierarchy

    \Exception
        \ArangoDb\Exception
            \ArangoDb\RuntimeException
                \ArangoDb\InvalidOptionException
                \ArangoDb\RequestFailedException
            \Arangodb\InvalidArgumentException
*/

namespace {

    PHP_METHOD(RequestFailedException, getBody)
    {
        zval *obj, *value;

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        obj = getThis();

        value  = zend_read_property(request_failed_exception_ce, obj, "body", sizeof("body") - 1, 1, NULL TSRMLS_CC);

        RETURN_ZVAL(value, 1, 0);
    }

    PHP_METHOD(RequestFailedException, getHttpCode)
    {
        zval *obj, *value;

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        obj = getThis();

        value  = zend_read_property(request_failed_exception_ce, obj, "httpCode", sizeof("httpCode") - 1, 1, NULL TSRMLS_CC);

        RETURN_ZVAL(value, 1, 0);
    }


    ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    const zend_function_entry request_failed_exception_functions[] = {
        PHP_ME(RequestFailedException, getBody, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(RequestFailedException, getHttpCode, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_FE_END
    };


    void register_exception()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\Exception", NULL);
        exception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default());
    }

    void register_runtime_exception()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\RuntimeException", NULL);
        runtime_exception_ce = zend_register_internal_class_ex(&ce, exception_ce);
    }

    void register_invalid_option_exception()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\InvalidOptionException", NULL);
        invalid_option_exception_ce = zend_register_internal_class_ex(&ce, runtime_exception_ce);
    }

    void register_request_failed_exception()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\RequestFailedException", request_failed_exception_functions);
        request_failed_exception_ce = zend_register_internal_class_ex(&ce, runtime_exception_ce);
    }

    void register_invalid_argument_exception()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\InvalidArgumentException", NULL);
        invalid_argument_exception_ce = zend_register_internal_class_ex(&ce, exception_ce);
    }

    void init_custom_exceptions()
    {
        register_exception();
        register_runtime_exception();
        register_invalid_option_exception();
        register_request_failed_exception();
        register_invalid_argument_exception();
    }
}
