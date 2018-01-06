#include "php_arangodb.h"

zend_class_entry *vpack_ce;

#define Z_OBJECT_VPACK(zo) (arangodb::fuerte::php::Vpack::fetch_object(zo))
#define Z_OBJECT_VPACK_P(zo) (arangodb::fuerte::php::Vpack::fetch_object(Z_OBJ_P(zo)))


PHP_METHOD(Vpack, __construct)
{
    arangodb::fuerte::php::Vpack* intern;

    if(zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = Z_OBJECT_VPACK_P(getThis());
}


PHP_METHOD(Vpack, fromJson)
{
    arangodb::fuerte::php::Vpack* intern;
    const char* json;
    size_t len;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &json, &len) == FAILURE) {
        return;
    }

    intern = Z_OBJECT_VPACK_P(getThis());
    intern->fromJson(json);
}

PHP_METHOD(Vpack, toJson)
{
    arangodb::fuerte::php::Vpack* intern;

    if(zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = Z_OBJECT_VPACK_P(getThis());
    auto json = intern->toJson();

    RETURN_STRING(json);
}

PHP_METHOD(Vpack, toHex)
{
    arangodb::fuerte::php::Vpack* intern;

    if(zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = Z_OBJECT_VPACK_P(getThis());
    auto hex = intern->toHex();

    RETURN_STRING(hex);
}


ZEND_BEGIN_ARG_INFO_EX(arangodb_vpack_void, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arangodb_vpack_to_json, 0, 0, 1)
    ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()

zend_function_entry vpack_methods[] = {
    PHP_ME(Vpack, __construct, arangodb_vpack_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Vpack, fromJson, arangodb_vpack_to_json, ZEND_ACC_PUBLIC)
    PHP_ME(Vpack, toJson, arangodb_vpack_void, ZEND_ACC_PUBLIC)
    PHP_ME(Vpack, toHex, arangodb_vpack_void, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(arangodb)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Vpack", vpack_methods);
    vpack_ce = zend_register_internal_class(&ce TSRMLS_CC);
    vpack_ce->create_object = arangodb::fuerte::php::Vpack::create_object;

    memcpy(&arangodb::fuerte::php::Vpack::php_arangodb_handler_vpack, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    arangodb::fuerte::php::Vpack::php_arangodb_handler_vpack.offset = XtOffsetOf(arangodb::fuerte::php::Vpack, std);

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
