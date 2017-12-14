#pragma once

#include <cmath>
using namespace std;

#include <Zend/zend.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_API.h>
#include <Zend/zend_inheritance.h>
#include <php.h>


namespace arangodb { namespace fuerte { namespace php {

    static void castArray(arangodb::velocypack::Builder* b, HashTable* myht);
    static void castAssocArray(arangodb::velocypack::Builder* b, HashTable* myht);

    static void castAssocArray(arangodb::velocypack::Builder* b, HashTable* myht)
    {
        zend_string* key;
        zval* data;
        zend_ulong index;

        char numberBuffer[20];
        char* vpackKey;

        ZEND_HASH_FOREACH_KEY_VAL_IND(myht, index, key, data) {

            if(key) {
                vpackKey = ZSTR_VAL(key);
            } else {
                snprintf(numberBuffer, sizeof(numberBuffer), "%d", index);
                vpackKey = numberBuffer;
            }

            switch(Z_TYPE_P(data)) {
                case IS_LONG:
                    b->add(vpackKey, arangodb::velocypack::Value(Z_LVAL_P(data)));
                    break;
                case IS_STRING:
                    b->add(vpackKey, arangodb::velocypack::Value(Z_STRVAL_P(data)));
                    break;
                case IS_DOUBLE:
                    b->add(vpackKey, arangodb::velocypack::Value(Z_DVAL_P(data)));
                    break;
                case IS_TRUE:
                    b->add(vpackKey, arangodb::velocypack::Value(true));
                    break;
                case IS_FALSE:
                    b->add(vpackKey, arangodb::velocypack::Value(false));
                    break;
                case IS_NULL:
                    b->add(vpackKey, arangodb::velocypack::Value(arangodb::velocypack::ValueType::Null));
                    break;
                case IS_ARRAY:
                    if(HT_IS_PACKED(Z_ARRVAL_P(data)) && HT_IS_WITHOUT_HOLES(Z_ARRVAL_P(data))) {
                        b->add(vpackKey, arangodb::velocypack::Value(arangodb::velocypack::ValueType::Array));
                        castArray(b, Z_ARRVAL_P(data));
                        b->close();
                    } else {
                        b->add(vpackKey, arangodb::velocypack::Value(arangodb::velocypack::ValueType::Object));
                        castAssocArray(b, Z_ARRVAL_P(data));
                        b->close();
                    }
                    break;
                case IS_OBJECT:
                    break;
                default:
                    break;
            }

        } ZEND_HASH_FOREACH_END();
    }


    static void castArray(arangodb::velocypack::Builder* b, HashTable* myht)
    {
       zval* data;

       ZEND_HASH_FOREACH_VAL(myht, data) {

           switch(Z_TYPE_P(data)) {
               case IS_LONG:
                   b->add(arangodb::velocypack::Value(Z_LVAL_P(data)));
                   break;
               case IS_STRING:
                   b->add(arangodb::velocypack::Value(Z_STRVAL_P(data)));
                   break;
               case IS_DOUBLE:
                   b->add(arangodb::velocypack::Value(Z_DVAL_P(data)));
                   break;
               case IS_TRUE:
                   b->add(arangodb::velocypack::Value(true));
                   break;
               case IS_FALSE:
                   b->add(arangodb::velocypack::Value(false));
                   break;
               case IS_NULL:
                   b->add(arangodb::velocypack::Value(arangodb::velocypack::ValueType::Null));
                   break;
               case IS_ARRAY:
                   if(HT_IS_PACKED(Z_ARRVAL_P(data)) && HT_IS_WITHOUT_HOLES(Z_ARRVAL_P(data))) {
                       b->add(arangodb::velocypack::Value(arangodb::velocypack::ValueType::Array));
                       castArray(b, Z_ARRVAL_P(data));
                       b->close();
                   } else {
                       b->add(arangodb::velocypack::Value(arangodb::velocypack::ValueType::Object));
                       castAssocArray(b, Z_ARRVAL_P(data));
                       b->close();
                   }
                   break;
               case IS_OBJECT:
                   break;
               default:
                   break;
           }

       } ZEND_HASH_FOREACH_END();
    }

}}}


namespace {

    void castArray(arangodb::velocypack::Builder* b, HashTable* myht);
    void castAssocArray(arangodb::velocypack::Builder* b, HashTable* myht);

    /**
     * Simple wrapper class just to gain access to the internal _val property
     */
    class _Object : public Php::Object
    {
    public:
        using Php::Object::Object;

        zval* internalZval()
        {
            return this->_val;
        }
    };


    PHP_METHOD(VpackImpl, fromArray)
    {
        zval *arrayValue;
        HashTable *myht;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arrayValue) == FAILURE) {
            return;
        }

        myht = Z_ARRVAL_P(arrayValue);

        arangodb::fuerte::php::Vpack* vpack = new arangodb::fuerte::php::Vpack();
        auto b = vpack->accessBuilder();

        if(HT_IS_PACKED(myht) && HT_IS_WITHOUT_HOLES(myht)) {
            b->add(arangodb::velocypack::Value(arangodb::velocypack::ValueType::Array));
            arangodb::fuerte::php::castArray(b, myht);
            b->close();
        } else {
            b->add(arangodb::velocypack::Value(arangodb::velocypack::ValueType::Object));
            arangodb::fuerte::php::castAssocArray(b, myht);
            b->close();
        }

        auto instance = _Object("ArangoDb\\Vpack", vpack);
        RETURN_ZVAL(instance.internalZval(), 1, 0);
    }


    ZEND_BEGIN_ARG_INFO_EX(arginfo_array_to_vpack, 0, 0, 1)
        ZEND_ARG_INFO(0, array)
    ZEND_END_ARG_INFO()

    const zend_function_entry array_to_vpack_functions[] = {
        PHP_ME(VpackImpl, fromArray, arginfo_array_to_vpack, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_FE_END
    };
}

namespace arangodb { namespace fuerte { namespace php {

    void registerVpackImpl()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\VpackImpl", array_to_vpack_functions);
        zend_register_internal_class(&ce);

        /**
         *  Here ArangoDb\Vpack extends ArangoDb\VpackImpl in order to inject the
         *  fromArray() method which is directly implemented using the zend api into
         *  the ArangoDb\Vpack class
         */
        zend_string *name = zend_string_tolower(zend_string_init(ZEND_STRL("ArangoDb\\VpackImpl"), 1));
        zend_class_entry *base_ce = static_cast<zend_class_entry*>(zend_hash_find_ptr(CG(class_table), name));
        zend_string_release(name);

        name = zend_string_tolower(zend_string_init(ZEND_STRL("ArangoDb\\Vpack"), 1));
        zend_class_entry* derived_ce = static_cast<zend_class_entry*>(zend_hash_find_ptr(CG(class_table), name));
        zend_string_release(name);

        assert(base_ce != nullptr);
        assert(derived_ce != nullptr);
        assert(!derived_ce->create_object || !base_ce->create_object || base_ce->create_object == derived_ce->create_object);

        zend_do_inheritance(derived_ce, base_ce);
    }

}}}
