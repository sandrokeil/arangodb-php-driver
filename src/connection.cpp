#include "connection.h"

namespace arangodb { namespace fuerte { namespace php {

    zend_object_handlers Connection::handler_connection = zend_object_handlers();

    zend_object* Connection::create_object(zend_class_entry* ce TSRMLS_CC)
    {
        Connection* intern = (Connection*)ecalloc(1, sizeof(Connection) + zend_object_properties_size(ce));
        new (intern) Connection();

        zend_object_std_init(&intern->std, ce TSRMLS_CC);

        intern->std.handlers = &Connection::handler_connection;
        return &intern->std;
    }

    Connection* Connection::fetch_object(zend_object* obj)
    {
        return (Connection *)((char *)obj - XtOffsetOf(Connection, std));
    }


    Connection::Connection():
        thread_count(std::thread::hardware_concurrency()),
        default_timeout(3)
    {
        //if getting the number of CPU cores fails, thread_count will default to 1
        if(this->thread_count < 1) {
            this->thread_count = 1;
        }
    }

    void Connection::set_options(HashTable* options)
    {
        this->options = options;
    }

    void Connection::set_thread_count(int thread_count)
    {
        this->thread_count = thread_count;
    }

    void Connection::set_default_timeout(int default_timeout)
    {
        this->default_timeout = default_timeout;
    }

    void Connection::connect() 
    {
        fu::ConnectionBuilder cbuilder = this->create_connection_builder();
        cbuilder.onFailure([&](fu::Error errorCode, const std::string &errorMessage) {
            //@todo add an error callback
        });

        this->event_loop_service = std::unique_ptr<fu::EventLoopService>(
            new fu::EventLoopService(this->thread_count)
        );

        this->connection = cbuilder.connect(*this->event_loop_service);
    }

    fu::ConnectionBuilder Connection::create_connection_builder() 
    {
        fu::ConnectionBuilder cbuilder;
        int option_key;

        zend_string* key;
        zval* data;
        zend_ulong index;

        ZEND_HASH_FOREACH_KEY_VAL_IND(this->options, index, key, data) {

            option_key = index;

            if(key) {
                option_key = this->option_string_key_to_int_key(ZSTR_VAL(key));
            }

            switch(option_key) {
                case Connection::OPTION_HOST:
                    cbuilder.host(Z_STRVAL_P(data));
                    break;
                case Connection::OPTION_USER:
                    cbuilder.user(Z_STRVAL_P(data));
                    break;
                case Connection::OPTION_PASSWORD:
                    cbuilder.password(Z_STRVAL_P(data));
                    break;
                case Connection::OPTION_MAX_CHUNK_SIZE:
                    cbuilder.maxChunkSize(Z_LVAL_P(data));
                    break;
                case Connection::OPTION_VST_VERSION:
                    cbuilder.vstVersion((fu::vst::VSTVersion)Z_LVAL_P(data));;
                    break;
                default:
                    ARANGODB_THROW_CE(invalid_option_exception_ce, 0, "Unknown option provided in %s on line %d");
                    break;
            }

        } ZEND_HASH_FOREACH_END();

        return cbuilder;
    }

    std::unique_ptr<fu::Response> Connection::send_request(Request* request)
    {
        auto result = this->connection->sendRequest(
            std::move(request->get_fuerte_request())
        );

        return result;
    }

    int Connection::option_string_key_to_int_key(const char* string_key)
    {
        if(strcmp(string_key, "host") == 0) {
            return Connection::OPTION_HOST;
        } else if(strcmp(string_key, "user") == 0) {
            return Connection::OPTION_USER;
        } else if(strcmp(string_key, "password") == 0) {
            return Connection::OPTION_PASSWORD;
        } else if(strcmp(string_key, "max_chunk_size") == 0) {
            return Connection::OPTION_MAX_CHUNK_SIZE;
        } else if(strcmp(string_key, "vst_version") == 0) {
            return Connection::OPTION_VST_VERSION;
        } else {
            ARANGODB_THROW_CE(invalid_option_exception_ce, 0, "Unknown option provided in %s on line %d");
            return 0;
        }
    }


    std::unique_ptr<fu::Response> Connection::send(int http_method, const char* path, const HashTable* vpack_value, const HashTable* query_params)
    {
        vp::Builder builder;
        VpackConversion::array_to_vpack(vpack_value, &builder);
        std::unique_ptr<fu::Request> request;

        if(query_params != NULL) {
            zend_string* key;
            zval* data;
            auto query_params_map = std::map<std::string, std::string>();

            ZEND_HASH_FOREACH_STR_KEY_VAL(query_params, key, data) {
                query_params_map[ZSTR_VAL(key)] = Z_STRVAL_P(data);
            } ZEND_HASH_FOREACH_END();

            request = fu::createRequest(static_cast<fu::RestVerb>(http_method), path, query_params_map);
        } else {
            request = fu::createRequest(static_cast<fu::RestVerb>(http_method), path);

        }

        request->addVPack(builder.slice());
        return this->connection->sendRequest(std::move(request));
    }

    std::unique_ptr<fu::Response> Connection::send(int http_method, const char* path, const char* vpack_value, const HashTable* query_params)
    {
        std::unique_ptr<fu::Request> request;
        vp::Builder builder;
        vp::Parser parser;

        try {
            parser.parse(vpack_value);
        }
        catch(std::bad_alloc const &e) {
            ARANGODB_THROW_CE(runtime_exception_ce, 0, "Out of memory in %s on line %d");
            return NULL;
        }
        catch(vp::Exception const &e) {
            ARANGODB_THROW_CE(runtime_exception_ce, 0, e.what());
            return NULL;
        }

        builder = *parser.steal();

        if(query_params != NULL) {
            zend_string* key;
            zval* data;
            auto query_params_map = std::map<std::string, std::string>();

            ZEND_HASH_FOREACH_STR_KEY_VAL(query_params, key, data) {
                query_params_map[ZSTR_VAL(key)] = Z_STRVAL_P(data);
            } ZEND_HASH_FOREACH_END();

            request = fu::createRequest(static_cast<fu::RestVerb>(http_method), path, query_params_map);
        } else {
            request = fu::createRequest(static_cast<fu::RestVerb>(http_method), path);

        }

        request->addVPack(builder.slice());
        return this->connection->sendRequest(std::move(request));
    }

}}}
