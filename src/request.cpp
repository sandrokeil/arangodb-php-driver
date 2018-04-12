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


    bool Request::query_params_to_string_map(HashTable* query_params, std::map<std::string, std::string>* query_params_map)
    {
        zend_string* key;
        zval* data;

        ZEND_HASH_FOREACH_STR_KEY_VAL(query_params, key, data) {

            switch(Z_TYPE_P(data)) {
                case IS_STRING:
                    (*query_params_map)[ZSTR_VAL(key)] = Z_STRVAL_P(data);
                    break;
                case IS_FALSE:
                    (*query_params_map)[ZSTR_VAL(key)] = "false";
                    break;
                case IS_TRUE:
                    (*query_params_map)[ZSTR_VAL(key)] = "true";
                    break;
                case IS_LONG:
                    (*query_params_map)[ZSTR_VAL(key)] = std::to_string(static_cast<int>(Z_LVAL_P(data)));
                    break;
                default:
                    throw ArangoDbInvalidArgumentException(
                        0,
                        "The queryParams array may only contain strings, booleans or integers"
                    );
            }

        } ZEND_HASH_FOREACH_END();

        return true;
    }


    void Request::set_http_method(int http_method)
    {
        this->http_method = static_cast<fu::RestVerb>(http_method);
    }

    void Request::set_path(const std::string& path)
    {
        this->path = path;
    }

    void Request::set_vpack_from_array(HashTable* ht)
    {
        this->builder = vp::Builder();
        VpackConversion::array_to_vpack(ht, &this->builder);
    }

    void Request::set_vpack_from_json(const char* json)
    {
        vp::Parser parser;

        try {
            parser.parse(json);
        }
        catch(std::bad_alloc const &e) {
            throw ArangoDbRuntimeException(0, "Out of memory");
        }
        catch(vp::Exception const &e) {
            throw ArangoDbRuntimeException(0, e.what());
        }

        this->builder = *parser.steal();
    }

    void Request::set_query_params(HashTable* query_params)
    {
        this->query_params = std::map<std::string, std::string>();
        Request::query_params_to_string_map(query_params, &this->query_params);
    }

    std::unique_ptr<fu::Request> Request::get_fuerte_request()
    {
        auto request = fu::createRequest(this->http_method, this->path, this->query_params);
        request->addVPack(this->builder.slice());

        return request;
    }

}}}