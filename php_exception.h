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
        INIT_CLASS_ENTRY(ce, "ArangoDb\\RequestFailedException", NULL);
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
