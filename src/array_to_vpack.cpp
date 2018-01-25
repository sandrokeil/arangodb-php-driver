#include "array_to_vpack.h"

namespace arangodb { namespace fuerte { namespace php {

    void ArrayToVpack::cast_assoc_array(const HashTable* ht, vp::Builder* builder)
    {
        zend_string* key;
        zval* data;
        zend_ulong index;

        char numberBuffer[20];
        char* vpackKey;

        ZEND_HASH_FOREACH_KEY_VAL_IND(ht, index, key, data) {

            if(key) {
                vpackKey = ZSTR_VAL(key);
            } else {
                snprintf(numberBuffer, sizeof(numberBuffer), "%d", index);
                vpackKey = numberBuffer;
            }

            switch(Z_TYPE_P(data)) {
                case IS_LONG:
                    builder->add(vpackKey, vp::Value(Z_LVAL_P(data)));
                    break;
                case IS_STRING:
                    builder->add(vpackKey, vp::Value(Z_STRVAL_P(data)));
                    break;
                case IS_DOUBLE:
                    builder->add(vpackKey, vp::Value(Z_DVAL_P(data)));
                    break;
                case IS_TRUE:
                    builder->add(vpackKey, vp::Value(true));
                    break;
                case IS_FALSE:
                    builder->add(vpackKey, vp::Value(false));
                    break;
                case IS_NULL:
                    builder->add(vpackKey, vp::Value(vp::ValueType::Null));
                    break;
                case IS_ARRAY:
                    if(HT_IS_PACKED(Z_ARRVAL_P(data)) && HT_IS_WITHOUT_HOLES(Z_ARRVAL_P(data))) {
                        builder->add(vpackKey, vp::Value(vp::ValueType::Array));
                        ArrayToVpack::cast_numeric_array(Z_ARRVAL_P(data), builder);
                        builder->close();
                    } else {
                        builder->add(vpackKey, vp::Value(vp::ValueType::Object));
                        ArrayToVpack::cast_assoc_array(Z_ARRVAL_P(data), builder);
                        builder->close();
                    }
                    break;
                case IS_OBJECT: //for now objects will just result in an empty json object
                    builder->add(vpackKey, vp::Value(vp::ValueType::Object));
                    builder->close();
                    break;
                default:
                    break;
            }

        } ZEND_HASH_FOREACH_END();
    }

    void ArrayToVpack::cast_numeric_array(const HashTable* ht, vp::Builder* builder)
    {
        zval* data;

        ZEND_HASH_FOREACH_VAL(ht, data) {

            switch(Z_TYPE_P(data)) {
                case IS_LONG:
                    builder->add(vp::Value(Z_LVAL_P(data)));
                    break;
                case IS_STRING:
                    builder->add(vp::Value(Z_STRVAL_P(data)));
                    break;
                case IS_DOUBLE:
                    builder->add(vp::Value(Z_DVAL_P(data)));
                    break;
                case IS_TRUE:
                    builder->add(vp::Value(true));
                    break;
                case IS_FALSE:
                    builder->add(vp::Value(false));
                    break;
                case IS_NULL:
                    builder->add(vp::Value(vp::ValueType::Null));
                    break;
                case IS_ARRAY:
                    if(HT_IS_PACKED(Z_ARRVAL_P(data)) && HT_IS_WITHOUT_HOLES(Z_ARRVAL_P(data))) {
                        builder->add(vp::Value(vp::ValueType::Array));
                        ArrayToVpack::cast_numeric_array(Z_ARRVAL_P(data), builder);
                        builder->close();
                    } else {
                        builder->add(vp::Value(vp::ValueType::Object));
                        ArrayToVpack::cast_assoc_array(Z_ARRVAL_P(data), builder);
                        builder->close();
                    }
                    break;
                case IS_OBJECT:
                    builder->add(vp::Value(vp::ValueType::Object));
                    builder->close();
                    break;
                default:
                    break;
           }

        } ZEND_HASH_FOREACH_END();
    }

    void ArrayToVpack::cast_array(const HashTable* ht, vp::Builder* builder)
    {
        if(HT_IS_PACKED(ht) && HT_IS_WITHOUT_HOLES(ht)) {
            builder->add(vp::Value(vp::ValueType::Array));
            ArrayToVpack::cast_numeric_array(ht, builder);
            builder->close();
        } else {
            builder->add(vp::Value(vp::ValueType::Object));
            ArrayToVpack::cast_assoc_array(ht, builder);
            builder->close();
        }
    }

}}}
