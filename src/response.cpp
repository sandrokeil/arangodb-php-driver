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
        this->assert_success();
    }

    int Response::get_http_code()
    {
        return static_cast<int>(this->response.statusCode());
    }

    std::string Response::get_body()
    {
        std::string body;

        try {
            vp::Slice slice = this->response.slices().front();
            vp::Options dumperOptions;

            vp::StringSink sink(&body);
            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(slice);
        } catch(vp::Exception const& e) {
            throw ArangoDbRuntimeException(0, e.what());
        }

        return body;
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
            throw ArangoDbRuntimeException(0, e.what());
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
            throw ArangoDbRuntimeException(0, "Value not found");
        }
    }

    void Response::get(zval* return_value, const char* accessor)
    {
        try {
            this->return_slice_to_php_value(return_value, this->response.slices().front().get(accessor));
        }
        catch(const vp::Exception& e) {
            throw ArangoDbRuntimeException(0, "Value not found");
        }
    }

    void Response::return_slice_to_php_value(zval* return_value, const vp::Slice& slice)
    {
        //@todo move all of this to vpack_conversion

        switch(slice.type()) {
            case vp::ValueType::String:
                {
                    std::string slice_string = slice.copyString();
                    RETURN_STRING(slice_string.c_str());
                    break;
                }

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

            case vp::ValueType::UTCDate:
            case vp::ValueType::None:
            case vp::ValueType::Binary:
            case vp::ValueType::Illegal:
            case vp::ValueType::MinKey:
            case vp::ValueType::MaxKey:
                RETURN_NULL();
                break;

            default:
                throw ArangoDbRuntimeException(0, "Could not convert vpack value to php value");
        }
    }

    void Response::assert_success()
    {
        bool failure = false;

        if (this->get_fuerte_response()->slices().front().isObject()
            && this->get_fuerte_response()->slices().front().hasKey("error")
        ) {
            failure = this->get_fuerte_response()->slices().front().get("error").getBool();
        }

        auto status_code = this->get_http_code();
        if((!(status_code >= 200 && status_code <= 299)) || failure) {
            std::string error_message = "Response contains an error";

            if(this->get_fuerte_response()->slices().front().isObject()
               && this->get_fuerte_response()->slices().front().hasKey("errorMessage")
            ) {
                error_message = this->get_fuerte_response()->slices().front().get("errorMessage").copyString();
            }

            throw ArangoDbRequestFailedException(
                0,
                error_message,
                status_code,
                this->get_body()
            );
        }
    }

}}}
