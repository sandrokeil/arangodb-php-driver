#include "connection.h"

namespace f = ::arangodb::fuerte;

namespace ArangoDb {

    const std::map<std::string, ConnectionOptions> connectionOptions =
    {
        { "host", ConnectionOptions::HOST            },
        { "user", ConnectionOptions::USER            },
        { "password", ConnectionOptions::PASSWORD            },
        { "max_chunk_size", ConnectionOptions::MAX_CHUNK_SIZE            },
        { "vst_version", ConnectionOptions::VST_VERSION            },
        { "on_failure", ConnectionOptions::ON_FAILURE            }
    };


    // constructor
    void Connection::__construct(Php::Parameters &params)
    {
        // first parameter is an array
        Php::Array options = params[0];

        _options = options;
    }

    void Connection::connect() {
        f::ConnectionBuilder cbuilder;

        try
        {
            for (const auto &p : _options) {
                switch (connectionOptions.at(p.first)) {
                    case ConnectionOptions::HOST:
                        cbuilder.host(p.second);
                        break;
                    case ConnectionOptions::USER:
                        cbuilder.user(p.second);
                        break;
                    case ConnectionOptions::PASSWORD:
                        cbuilder.password(p.second);
                        break;
                    case ConnectionOptions::MAX_CHUNK_SIZE:
                        // cbuilder.maxChunkSize(p.second);
                        break;
                    case ConnectionOptions::VST_VERSION:
                        // cbuilder.vstVersion(p.second);
                        break;
                    case ConnectionOptions::ON_FAILURE:
                        cbuilder.onFailure([&](f::Error errorCode, const std::string& errorMessage){
                            throw Php::Exception("Could not connect to ArangoDb.");
                        });
                        break;
                    default:
                        // map ensures itegrity
                        break;
                }
            }
        }
        catch (...)
        {
            throw Php::Exception("Unknown option provided.");
        }
        _eventLoopService = std::unique_ptr<f::EventLoopService>(new f::EventLoopService(1));
        _connection = cbuilder.connect(*_eventLoopService);
    }
}
