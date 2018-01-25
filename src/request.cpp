#include "request.h"

namespace arangodb { namespace fuerte { namespace php {

    zend_object_handlers Request::handler_request = zend_object_handlers();

    zend_object* Request::create_object(zend_class_entry* ce TSRMLS_CC)
    {
        Request* intern = (Request*)ecalloc(1, sizeof(Request) + zend_object_properties_size(ce));

        zend_object_std_init(&intern->std, ce TSRMLS_CC);

        intern->std.handlers = &Request::handler_request;
        return &intern->std;
    }

    Request* Request::fetch_object(zend_object* obj)
    {
        return (Request *)((char *)obj - XtOffsetOf(Request, std));
    }


    void Request::set_http_method(int http_method)
    {
        this->http_method = static_cast<fu::RestVerb>(http_method);
    }

    void Request::set_path(const std::string& path)
    {
        this->path = path;
    }

    void Request::set_vpack_from_array(const HashTable* ht)
    {
        this->builder = vp::Builder();

        if(HT_IS_PACKED(ht) && HT_IS_WITHOUT_HOLES(ht)) {
            this->builder.add(vp::Value(vp::ValueType::Array));
            this->cast_numeric_array(ht);
            this->builder.close();
        } else {
            this->builder.add(vp::Value(vp::ValueType::Object));
            this->cast_assoc_array(ht);
            this->builder.close();
        }
    }

    void Request::set_vpack_from_json(const char* json)
    {
        vp::Parser parser;

        try {
            parser.parse(json);
        }
        catch(std::bad_alloc const &e) {
            /* @todo add exception */
        }
        catch(vp::Exception const &e) {
            /* @todo add exception */
        }

        this->builder = *parser.steal();
    }

    void Request::set_query_params(HashTable* query_params)
    {
        zend_string* key;
        zval* data;
        this->query_params = std::map<std::string, std::string>();

        ZEND_HASH_FOREACH_STR_KEY_VAL(query_params, key, data) {

            this->query_params[ZSTR_VAL(key)] = Z_STRVAL_P(data);

        } ZEND_HASH_FOREACH_END();
    }

    std::unique_ptr<fu::Request> Request::get_fuerte_request()
    {
        auto request = fu::createRequest(this->http_method, this->path, this->query_params);
        request->addVPack(this->builder.slice());

        return request;
    }

    void Request::cast_assoc_array(const HashTable* ht)
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

    void Request::cast_numeric_array(const HashTable* ht)
    {
        zval* data;

        ZEND_HASH_FOREACH_VAL(ht, data) {

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

}}}