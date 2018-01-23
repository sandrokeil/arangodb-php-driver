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
            //@todo exception
        }

        RETURN_STRING(body.c_str());
    }

    fu::Response* Response::get_fuerte_response()
    {
        return &this->response;
    }

}}}
