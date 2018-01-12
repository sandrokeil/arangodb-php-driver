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

}}}