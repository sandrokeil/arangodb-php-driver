#pragma once

extern "C" {
#include <php.h>
}

#include "src/vpack.h"

namespace {
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
        zval object;
        char* json;
        size_t len;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &json, &len) == FAILURE) {
            return;
        }

        object_init_ex(&object, vpack_ce);
        auto intern = Z_OBJECT_VPACK(Z_OBJ(object));

        intern->from_json(json, len);

        RETURN_ZVAL(&object, 1, 0);
    }

    PHP_METHOD(Vpack, fromArray)
    {
        zval object;
        zval *arrayValue;
        HashTable *myht;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arrayValue) == FAILURE) {
            return;
        }

        myht = Z_ARRVAL_P(arrayValue);

        object_init_ex(&object, vpack_ce);
        auto intern = Z_OBJECT_VPACK(Z_OBJ(object));

        intern->from_array(myht);

        RETURN_ZVAL(&object, 1, 0);
    }

    PHP_METHOD(Vpack, toJson)
    {
        arangodb::fuerte::php::Vpack* intern;

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        intern = Z_OBJECT_VPACK_P(getThis());
        auto json = intern->to_json();

        RETURN_STRING(json.c_str());
    }

    PHP_METHOD(Vpack, toHex)
    {
        arangodb::fuerte::php::Vpack* intern;

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        intern = Z_OBJECT_VPACK_P(getThis());
        auto hex = intern->to_hex();

        RETURN_STRING(hex.c_str());
    }


    ZEND_BEGIN_ARG_INFO_EX(arangodb_vpack_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_vpack_from_json, 0, 0, 1)
        ZEND_ARG_INFO(0, json)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(arangodb_vpack_from_array, 0, 0, 1)
        ZEND_ARG_INFO(0, array)
    ZEND_END_ARG_INFO()

    zend_function_entry vpack_methods[] = {
        PHP_ME(Vpack, __construct, arangodb_vpack_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(Vpack, fromJson, arangodb_vpack_from_json, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(Vpack, fromArray, arangodb_vpack_from_array, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(Vpack, toJson, arangodb_vpack_void, ZEND_ACC_PUBLIC)
        PHP_ME(Vpack, toHex, arangodb_vpack_void, ZEND_ACC_PUBLIC)
        PHP_FE_END
    };

    void init_arangodb_vpack_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ArangoDb\\Vpack", vpack_methods);
        vpack_ce = zend_register_internal_class(&ce TSRMLS_CC);
        vpack_ce->create_object = arangodb::fuerte::php::Vpack::create_object;

        memcpy(&arangodb::fuerte::php::Vpack::handler_vpack, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Vpack::handler_vpack.offset = XtOffsetOf(arangodb::fuerte::php::Vpack, std);
    }
}
