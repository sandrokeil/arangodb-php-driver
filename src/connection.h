#pragma once

#include <phpcpp.h>
#include <stdlib.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>
#include <velocypack/vpack.h>

namespace f = ::arangodb::fuerte;


namespace ArangoDb {
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
         *  @var    std::shared_ptr<f::Connection>
         */
        std::shared_ptr<f::Connection> connection;

        /**
         *  The underlying event loop service object
         *  @var    std::unique_ptr<f::EventLoopService>
         */
        std::unique_ptr<f::EventLoopService> eventLoopService{};

        /**
         * We use PHP::Array here because it makes type mapping easier
         *
         * @var Php::Array
         */
        Php::Array options;

        f::ConnectionBuilder createConnectionBuilder();

    public:
        Connection() = default;

        /**
         *  Constructor
         *
         *  @param  Php::Parameters  connection options
         */
        void __construct(Php::Parameters &params);

        /**
         * Connect to database
         */
        void connect();

        virtual ~Connection() = default;
    };
}
