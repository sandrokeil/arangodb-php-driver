#include "connection.h"

namespace arangodb { namespace fuerte { namespace php {

    zend_object_handlers Connection::handler_connection = zend_object_handlers();

    zend_object* Connection::create_object(zend_class_entry* ce TSRMLS_CC)
    {
        Connection* intern = (Connection*)ecalloc(1, sizeof(Connection) + zend_object_properties_size(ce));

        zend_object_std_init(&intern->std, ce TSRMLS_CC);

        intern->std.handlers = &Connection::handler_connection;
        return &intern->std;
    }

    Connection* Connection::fetch_object(zend_object* obj)
    {
        return (Connection *)((char *)obj - XtOffsetOf(Connection, std));
    }
}}}
