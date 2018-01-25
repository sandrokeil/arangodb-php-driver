#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include <thread>

#include "request.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Connection
    {
    public:
        static const int OPTION_HOST = 0;
        static const int OPTION_USER = 1;
        static const int OPTION_PASSWORD = 2;
        static const int OPTION_MAX_CHUNK_SIZE = 3;
        static const int OPTION_VST_VERSION = 4;

    private:
        std::shared_ptr<fu::Connection> connection;
        std::unique_ptr<fu::EventLoopService> event_loop_service;

        HashTable* options;

        int default_timeout;
        int thread_count;

    public:
        zend_object std;

        static zend_object_handlers handler_connection;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Connection* fetch_object(zend_object* obj);

        Connection();

        void set_options(HashTable* options);
        void set_default_timeout(int default_timeout);
        void set_thread_count(int thread_count);

        void connect();
        std::unique_ptr<fu::Response> send_request(Request* request);

        std::unique_ptr<fu::Response> send(int http_method, const char* path, const HashTable* vpack_value, const HashTable* query_params);
        std::unique_ptr<fu::Response> send(int http_method, const char* path, const char* vpack_value, const HashTable* query_params);

    private:
        fu::ConnectionBuilder create_connection_builder();
        int option_string_key_to_int_key(const char* string_key);
    };

}}}
