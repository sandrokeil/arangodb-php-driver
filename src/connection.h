#pragma once

#include <phpcpp.h>
#include <thread>
#include <fuerte/helper.h>

#include "request.h"
#include "response.h"

namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    enum ConnectionOptions {
        HOST = 0,
        USER = 1,
        PASSWORD = 2,
        MAX_CHUNK_SIZE = 3,
        VST_VERSION = 4
    };

    class Connection :
            public Php::Base {
    private:
        /**
         *  The underlying connection object
         *  @var    std::shared_ptr<fu::Connection>
         */
        std::shared_ptr<fu::Connection> connection;

        /**
         *  The underlying event loop service object
         *  @var    std::unique_ptr<fu::EventLoopService>
         */
        std::unique_ptr<fu::EventLoopService> eventLoopService{};

        /**
         * We use PHP::Array here because it makes type mapping easier
         *
         * @var Php::Array
         */
        Php::Array options;

        fu::ConnectionBuilder createConnectionBuilder();

        short defaultTimeout = 3;
        int threadCount;

        fu::WaitGroup* asyncWaitGroup;

    public:
        Connection();
        ~Connection();

        void __construct(Php::Parameters &params);

        void setThreadCount(Php::Parameters &params);
        void setDefaultTimeout(Php::Parameters &params);

        void connect();
        Response* sendRequest(Request* request);
        void sendRequestAsync(Request* request, Php::Value& callback);

        Php::Value send(Php::Parameters &params);
        void sendAsync(Php::Parameters &params);

        Php::Value methodDelete(Php::Parameters &params);
        Php::Value methodGet(Php::Parameters &params);
        Php::Value methodPost(Php::Parameters &params);
        Php::Value methodPut(Php::Parameters &params);
        Php::Value methodHead(Php::Parameters &params);
        Php::Value methodPatch(Php::Parameters &params);
        Php::Value methodOptions(Php::Parameters &params);

        void wait();

        Php::Value query(Php::Parameters &params);

    };

}}}
