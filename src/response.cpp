#include "response.h"

namespace arangodb { namespace fuerte { namespace php {

    zend_object_handlers Response::handler_response = zend_object_handlers();

    zend_object* Response::create_object(zend_class_entry* ce TSRMLS_CC)
    {
        Response* intern = (Response*)ecalloc(1, sizeof(Response) + zend_object_properties_size(ce));

        zend_object_std_init(&intern->std, ce TSRMLS_CC);

        intern->std.handlers = &Response::handler_response;
        return &intern->std;
    }

    Response* Response::fetch_object(zend_object* obj)
    {
        return (Response *)((char *)obj - XtOffsetOf(Response, std));
    }

    Response::Response(const fu::Response &response) : response(response)
    {
    }

    int Response::get_http_code()
    {
        return static_cast<int>(this->response.statusCode());
    }

    void Response::return_body(zval* return_value)
    {
        std::string body;

        try {
            vp::Slice slice = this->response.slices().front();
            vp::Options dumperOptions;

            vp::StringSink sink(&body);
            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(slice);
        } catch(vp::Exception const& e) {
            ARANGODB_THROW_CE(runtime_exception_ce, 0, e.what());
            return;
        }

        RETURN_STRING(body.c_str());
    }

    fu::Response* Response::get_fuerte_response()
    {
        return &this->response;
    }

    void Response::get(zval* return_value, HashTable* accessor)
        {
            try {
                zval* value;
                vp::Slice tmpSlice(this->response.slices().front());

                ZEND_HASH_FOREACH_VAL(accessor, value) {

                    if((Z_TYPE_P(value) == IS_LONG)) {
                        tmpSlice = vp::Slice(tmpSlice.at(Z_LVAL_P(value)));
                    } else if(Z_TYPE_P(value) == IS_STRING) {
                        tmpSlice = vp::Slice(tmpSlice.get(Z_STRVAL_P(value)));
                    } else {
                        ARANGODB_THROW_CE(invalid_argument_exception_ce, 0, "Accessor array may only contain strings and integers in %s on line %d");
                        return;
                    }

                } ZEND_HASH_FOREACH_END();

                this->return_slice_to_php_value(return_value, tmpSlice);
            }
            catch(const vp::Exception& e) {
                ARANGODB_THROW_CE(runtime_exception_ce, 0, "Value not found in %s on line %d");
                return;
            }
        }

        void Response::get(zval* return_value, const char* accessor)
        {
            try {
                this->return_slice_to_php_value(return_value, this->response.slices().front().get(accessor));
            }
            catch(const vp::Exception& e) {
                ARANGODB_THROW_CE(runtime_exception_ce, 0, "Value not found in %s on line %d");
                return;
            }
        }

        void Response::return_slice_to_php_value(zval* return_value, const vp::Slice& slice)
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

                case vp::ValueType::Array:
                case vp::ValueType::Object:
                    VpackConversion::vpack_to_array(&slice, return_value);
                    break;

                default:
                    ARANGODB_THROW_CE(runtime_exception_ce, 0, "Could not convert vpack value to php value in %s on line %d");
                    return;
            }
        }

}}}
