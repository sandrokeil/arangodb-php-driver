#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "request.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Connection
    {
    private:
        std::shared_ptr<fu::Connection> connection;

    public:
        HashTable* options;
        zend_object std;

        static zend_object_handlers handler_connection;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Connection* fetch_object(zend_object* obj);

        void connect();
        void send_request(Request* request);

    private:
        fu::ConnectionBuilder create_connection_builder();
    };

}}}
