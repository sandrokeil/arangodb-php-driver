#include "vpack.h"

namespace arangodb { namespace fuerte { namespace php {

    zend_object_handlers Vpack::handler_vpack = zend_object_handlers();

    zend_object* Vpack::create_object(zend_class_entry* ce TSRMLS_CC)
    {
        Vpack* intern = (Vpack*)ecalloc(1, sizeof(Vpack) + zend_object_properties_size(ce));

        zend_object_std_init(&intern->std, ce TSRMLS_CC);

        intern->std.handlers = &Vpack::handler_vpack;
        return &intern->std;
    }

    Vpack* Vpack::fetch_object(zend_object* obj)
    {
        return (Vpack *)((char *)obj - XtOffsetOf(Vpack, std));
    }


    Vpack::Vpack()
    {
    }

    Vpack::~Vpack()
    {
    }

    void Vpack::from_json(const char* json, size_t size)
    {
        vp::Parser parser;

        try {
            parser.parse(json, size);
        }
        catch(std::bad_alloc const &e) {
            /* @todo add exception */
        }
        catch(vp::Exception const &e) {
            /* @todo add exception */
        }

        this->builder = *parser.steal();
    }
    
    void Vpack::from_array(HashTable* myht) 
    {
        this->builder = vp::Builder();

        if(HT_IS_PACKED(myht) && HT_IS_WITHOUT_HOLES(myht)) {
            this->builder.add(vp::Value(vp::ValueType::Array));
            this->cast_numeric_array(myht);
            this->builder.close();
        } else {
            this->builder.add(vp::Value(vp::ValueType::Object));
            this->cast_assoc_array(myht);
            this->builder.close();
        }
    }

    void Vpack::from_slice(const vp::Slice& slice)
    {
        this->builder = vp::Builder(slice);
    }

    std::string Vpack::to_json()
    {
        try {
            std::string json;

            vp::StringSink sink(&json);
            vp::Options dumperOptions;

            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(this->builder.slice());

            return json;
        }
        catch(vp::Exception const &e) {
            /* @todo add exception */
        }
    }

    std::string Vpack::to_hex()
    {
        return vp::HexDump(this->builder.slice()).toString().c_str();
    }


    void Vpack::cast_assoc_array(HashTable* myht)
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
                    this->builder.add(vpackKey, vp::Value(Z_LVAL_P(data)));
                    break;
                case IS_STRING:
                    this->builder.add(vpackKey, vp::Value(Z_STRVAL_P(data)));
                    break;
                case IS_DOUBLE:
                    this->builder.add(vpackKey, vp::Value(Z_DVAL_P(data)));
                    break;
                case IS_TRUE:
                    this->builder.add(vpackKey, vp::Value(true));
                    break;
                case IS_FALSE:
                    this->builder.add(vpackKey, vp::Value(false));
                    break;
                case IS_NULL:
                    this->builder.add(vpackKey, vp::Value(vp::ValueType::Null));
                    break;
                case IS_ARRAY:
                    if(HT_IS_PACKED(Z_ARRVAL_P(data)) && HT_IS_WITHOUT_HOLES(Z_ARRVAL_P(data))) {
                        this->builder.add(vpackKey, vp::Value(vp::ValueType::Array));
                        this->cast_numeric_array(Z_ARRVAL_P(data));
                        this->builder.close();
                    } else {
                        this->builder.add(vpackKey, vp::Value(vp::ValueType::Object));
                        this->cast_assoc_array(Z_ARRVAL_P(data));
                        this->builder.close();
                    }
                    break;
                case IS_OBJECT: //for now objects will just result in an empty json object
                    this->builder.add(vpackKey, vp::Value(vp::ValueType::Object));
                    this->builder.close();
                    break;
                default:
                    break;
            }

        } ZEND_HASH_FOREACH_END();
    }

    void Vpack::cast_numeric_array(HashTable* myht)
    {
        zval* data;

        ZEND_HASH_FOREACH_VAL(myht, data) {

            switch(Z_TYPE_P(data)) {
                case IS_LONG:
                    this->builder.add(vp::Value(Z_LVAL_P(data)));
                    break;
                case IS_STRING:
                    this->builder.add(vp::Value(Z_STRVAL_P(data)));
                    break;
                case IS_DOUBLE:
                    this->builder.add(vp::Value(Z_DVAL_P(data)));
                    break;
                case IS_TRUE:
                    this->builder.add(vp::Value(true));
                    break;
                case IS_FALSE:
                    this->builder.add(vp::Value(false));
                    break;
                case IS_NULL:
                    this->builder.add(vp::Value(vp::ValueType::Null));
                    break;
                case IS_ARRAY:
                    if(HT_IS_PACKED(Z_ARRVAL_P(data)) && HT_IS_WITHOUT_HOLES(Z_ARRVAL_P(data))) {
                        this->builder.add(vp::Value(vp::ValueType::Array));
                        this->cast_numeric_array(Z_ARRVAL_P(data));
                        this->builder.close();
                    } else {
                        this->builder.add(vp::Value(vp::ValueType::Object));
                        this->cast_assoc_array(Z_ARRVAL_P(data));
                        this->builder.close();
                    }
                    break;
                case IS_OBJECT:
                    this->builder.add(vp::Value(vp::ValueType::Object));
                    this->builder.close();
                    break;
                default:
                    break;
           }

        } ZEND_HASH_FOREACH_END();
    }

    void Vpack::get(zval* return_value, HashTable* accessor)
    {
        try {
            zval* value;
            vp::Slice tmpSlice(this->builder.slice());

            ZEND_HASH_FOREACH_VAL(accessor, value) {

                if((Z_TYPE_P(value) == IS_LONG)) {
                    tmpSlice = vp::Slice(tmpSlice.at(Z_LVAL_P(value)));
                } else if(Z_TYPE_P(value) == IS_STRING) {
                    tmpSlice = vp::Slice(tmpSlice.get(Z_STRVAL_P(value)));
                } else {
                    //@todo exception
                }

            } ZEND_HASH_FOREACH_END();

            this->return_slice_to_php_value(return_value, tmpSlice);
        }
        catch(const vp::Exception& e) {
            //@todo exception
        }
    }

    void Vpack::get(zval* return_value, const char* accessor)
    {
        try {
            this->return_slice_to_php_value(return_value, this->builder.slice().get(accessor));
        }
        catch(const vp::Exception& e) {
            //@todo exception
        }
    }

    void Vpack::return_slice_to_php_value(zval* return_value, const vp::Slice& slice)
    {
        switch(slice.type()) {
            case vp::ValueType::String:
                RETURN_STRING(slice.copyString().c_str());
                break;

            case vp::ValueType::Int:
            case vp::ValueType::UInt:
            case vp::ValueType::SmallInt:
                RETURN_LONG(slice.getInt());
                break;

            case vp::ValueType::Double:
                RETURN_DOUBLE(slice.getDouble());
                break;

            case vp::ValueType::Null:
                RETURN_NULL();
                break;

            /*case vp::ValueType::Array:
            case vp::ValueType::Object:
                return this->sliceToJson(slice);
                break;*/

            default:
                //@todo exception
                break;
        }
    }

}}}
