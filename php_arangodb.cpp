#include "php_arangodb.h"
#include <iostream>

zend_class_entry *vpack_ce;

PHP_METHOD(Vpack, __construct)
{
    arangodb::velocypack::Builder builder;

    builder.add(arangodb::velocypack::Value(arangodb::velocypack::ValueType::Object));
    builder.add("tteesstt", arangodb::velocypack::Value(123));
    builder.close();

    std::cout << arangodb::velocypack::HexDump(builder.slice()).toString() << std::endl;
}

zend_function_entry vpack_methods[] = {
    PHP_ME(Vpack, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(arangodb)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Vpack", vpack_methods);
    vpack_ce = zend_register_internal_class(&ce TSRMLS_CC);
    return SUCCESS;
}



zend_module_entry arangodb_module_entry = {

#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_ARANGODB_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(arangodb),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_ARANGODB_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ARANGODB
extern "C" {
ZEND_GET_MODULE(arangodb)
}
#endif
