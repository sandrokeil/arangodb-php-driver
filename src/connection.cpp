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


    void Connection::connect() 
    {
        fu::ConnectionBuilder cbuilder = this->create_connection_builder();
        cbuilder.onFailure([&](fu::Error errorCode, const std::string &errorMessage) {
            //@todo add an error callback
        });

        std::unique_ptr<fu::EventLoopService> eventLoopService = std::unique_ptr<fu::EventLoopService>(
            new fu::EventLoopService(1) //@todo add thread count here
        );

        this->connection = cbuilder.connect(*eventLoopService);
    }

    fu::ConnectionBuilder Connection::create_connection_builder() 
    {
        fu::ConnectionBuilder cbuilder;

        try {
            zend_string* key;
            zval* data;

            ZEND_HASH_FOREACH_STR_KEY_VAL(this->options, key, data) {

                //@todo configure cbuilder

            } ZEND_HASH_FOREACH_END();
        } 
        catch(const std::exception &ex) {

        }

        return cbuilder;
    }

    /*Response* */void Connection::send_request(Request* request)
    {
        auto result = this->connection->sendRequest(
            std::move(request->get_fuerte_request())
        );

        /*auto response = new Response(*result);
        bool failure = false;

        if (response->getFuerteResponse()->slices().front().isObject()
            && response->getFuerteResponse()->slices().front().hasKey("error")
        ) {
            failure = response->getFuerteResponse()->slices().front().get("error").getBool();
        }

        auto statusCode = response->getHttpCode();
        if((!(statusCode >= 200 && statusCode <= 299)) || failure) {
            std::string errorMessage = "Response contains an error";

            if(response->getFuerteResponse()->slices().front().isObject()
               && response->getFuerteResponse()->slices().front().hasKey("errorMessage")
            ) {
                errorMessage = response->getFuerteResponse()->slices().front().get("errorMessage").copyString();
            }

            throwRequestFailedException(errorMessage.c_str(), statusCode, response->getBody());
        }

        return response;*/
    }
}}}
