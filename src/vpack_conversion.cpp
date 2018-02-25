#include "vpack_conversion.h"

namespace arangodb { namespace fuerte { namespace php {

    void VpackConversion::cast_assoc_array(const HashTable* ht, vp::Builder* builder)
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
                        VpackConversion::cast_numeric_array(Z_ARRVAL_P(data), builder);
                        builder->close();
                    } else {
                        builder->add(vpackKey, vp::Value(vp::ValueType::Object));
                        VpackConversion::cast_assoc_array(Z_ARRVAL_P(data), builder);
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

    void VpackConversion::cast_numeric_array(const HashTable* ht, vp::Builder* builder)
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
                        VpackConversion::cast_numeric_array(Z_ARRVAL_P(data), builder);
                        builder->close();
                    } else {
                        builder->add(vp::Value(vp::ValueType::Object));
                        VpackConversion::cast_assoc_array(Z_ARRVAL_P(data), builder);
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

    void VpackConversion::array_to_vpack(const HashTable* ht, vp::Builder* builder)
    {
        if(HT_IS_PACKED(ht) && HT_IS_WITHOUT_HOLES(ht)) {
            builder->add(vp::Value(vp::ValueType::Array));
            VpackConversion::cast_numeric_array(ht, builder);
            builder->close();
        } else {
            builder->add(vp::Value(vp::ValueType::Object));
            VpackConversion::cast_assoc_array(ht, builder);
            builder->close();
        }
    }

    void VpackConversion::vpack_to_array(const vp::Slice* slice, zval* array)
    {
        if(slice->isArray()) {
            VpackConversion::vpack_array_to_php_array(slice, array);
        } else if(slice->isObject()) {
            VpackConversion::vpack_object_to_php_array(slice, array);
        } else {
            //@todo exception or do we need this?
        }
    }

    void VpackConversion::vpack_array_to_php_array(const vp::Slice* slice, zval* array)
    {
        array_init(array);

        for (auto const& it : vp::ArrayIterator(*slice)) {
            switch(it.type()) {
                case vp::ValueType::String:
                    {
                        vp::ValueLength len;
                        auto val = it.getString(len);
                        add_next_index_stringl(array, val, len);
                    }
                    break;
                case vp::ValueType::Int:
                    add_next_index_long(array, it.getInt());
                    break;
                case vp::ValueType::UInt:
                    add_next_index_long(array, it.getUInt());
                    break;
                case vp::ValueType::SmallInt:
                    add_next_index_long(array, it.getSmallInt());
                    break;
                case vp::ValueType::Double:
                    add_next_index_double(array, it.getDouble());
                    break;
                case vp::ValueType::Null:
                    add_next_index_null(array);
                    break;
                case vp::ValueType::Bool:
                    add_next_index_bool(array, it.getBool());
                    break;
                case vp::ValueType::Array:
                    {
                        zval sub_arr;
                        VpackConversion::vpack_array_to_php_array(&it, &sub_arr);
                        add_next_index_zval(array, &sub_arr);
                    }
                    break;
                case vp::ValueType::Object:
                    {
                        zval sub_arr;
                        VpackConversion::vpack_object_to_php_array(&it, &sub_arr);
                        add_next_index_zval(array, &sub_arr);
                    }
                    break;
                default:
                    //@todo exception or just continue?
                    break;
            }
        }
    }

    void VpackConversion::vpack_object_to_php_array(const vp::Slice* slice, zval* array)
    {
        array_init(array);

        for(auto const& it : vp::ObjectIterator(*slice)) {
            vp::ValueLength key_len;
            auto key = it.key.getString(key_len);

            switch(it.value.type()) {
                case vp::ValueType::String:
                    {
                        vp::ValueLength len;
#if PHP_VERSION_ID >= 72000
                        auto val = it.value.getString(len);
#else
                        auto val = strdup(it.value.getString(len));
#endif
                        add_assoc_stringl_ex(array, key, key_len, val, len);
                    }
                    break;
                case vp::ValueType::Int:
                    add_assoc_long_ex(array, key, key_len, it.value.getInt());
                    break;
                case vp::ValueType::UInt:
                    add_assoc_long_ex(array, key, key_len, it.value.getUInt());
                    break;
                case vp::ValueType::SmallInt:
                    add_assoc_long_ex(array, key, key_len, it.value.getSmallInt());
                    break;
                case vp::ValueType::Double:
                    add_assoc_double_ex(array, key, key_len, it.value.getDouble());
                    break;
                case vp::ValueType::Null:
                    add_assoc_null_ex(array, key, key_len);
                    break;
                case vp::ValueType::Bool:
                    add_assoc_bool_ex(array, key, key_len, it.value.getBool());
                    break;
                case vp::ValueType::Array:
                    {
                        zval sub_arr;
                        VpackConversion::vpack_array_to_php_array(&it.value, &sub_arr);
                        add_assoc_zval_ex(array, key, key_len, &sub_arr);
                    }
                    break;
                case vp::ValueType::Object:
                    {
                        zval sub_arr;
                        VpackConversion::vpack_object_to_php_array(&it.value, &sub_arr);
                        add_assoc_zval_ex(array, key, key_len, &sub_arr);
                    }
                    break;
                default:
                    //@todo exception or continue?
                    break;
            }
        }
    }

}}}
