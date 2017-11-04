#include "connection.h"

namespace f = ::arangodb::fuerte;

namespace ArangoDb {

    const std::map<std::string, ConnectionOptions> connectionOptions =
            {
                    {"host",           ConnectionOptions::HOST},
                    {"user",           ConnectionOptions::USER},
                    {"password",       ConnectionOptions::PASSWORD},
                    {"max_chunk_size", ConnectionOptions::MAX_CHUNK_SIZE},
                    {"vst_version",    ConnectionOptions::VST_VERSION}
            };

    void Connection::__construct(Php::Parameters &params) {
        this->options = params[0];
    }

    f::ConnectionBuilder Connection::createConnectionBuilder() {
        try {
            f::ConnectionBuilder cbuilder{};

            for (const auto &p : this->options) {
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
                        cbuilder.maxChunkSize(static_cast<size_t>(p.second.numericValue()));
                        break;
                    case ConnectionOptions::VST_VERSION:
                        cbuilder.vstVersion((f::vst::VSTVersion) p.second.numericValue());
                        break;
                    default:
                        // map ensures integrity
                        break;
                }
            }

            return cbuilder;
        }
        catch (const std::exception &ex) {
            throw Php::Exception("Unknown option provided.");
        }
    }

    void Connection::connect() {
        f::WaitGroup wg;
        wg.add();

        f::ConnectionBuilder cbuilder = createConnectionBuilder();
        cbuilder.onFailure([&](f::Error errorCode, const std::string &errorMessage) {
            wg.done();
        });

        this->eventLoopService = std::unique_ptr<f::EventLoopService>(new f::EventLoopService(1));
        this->connection = cbuilder.connect(*eventLoopService);

        auto failure = wg.wait_for(std::chrono::seconds(1));
        if (failure) {
            throw Php::Exception("Could not connect to ArangoDB");
        }
    }
}