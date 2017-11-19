#include "connection.h"

namespace arangodb { namespace fuerte { namespace php {

    const std::map<std::string, ConnectionOptions> connectionOptions =
            {
                    {"host",           ConnectionOptions::HOST},
                    {"user",           ConnectionOptions::USER},
                    {"password",       ConnectionOptions::PASSWORD},
                    {"max_chunk_size", ConnectionOptions::MAX_CHUNK_SIZE},
                    {"vst_version",    ConnectionOptions::VST_VERSION}
            };

    Connection::Connection():
        threadCount(std::thread::hardware_concurrency()),
        asyncWaitGroup(new fu::WaitGroup())
    {
        if(this->threadCount < 1) {
            this->threadCount = 1;
        }
    }

    Connection::~Connection()
    {
        delete this->asyncWaitGroup;
    }

    void Connection::__construct(Php::Parameters &params)
    {
        this->options = params[0];
    }

    void Connection::setThreadCount(Php::Parameters &params)
    {
        int threadCount = params[0];

        if(threadCount < 1) {
            throw Php::Exception("Invalid threadCount provided, must be >= 1");
        }

        this->threadCount = threadCount;
    }

    fu::ConnectionBuilder Connection::createConnectionBuilder()
    {
        try {
            fu::ConnectionBuilder cbuilder{};

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
                        cbuilder.vstVersion((fu::vst::VSTVersion) p.second.numericValue());
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

    void Connection::connect()
    {
        fu::ConnectionBuilder cbuilder = createConnectionBuilder();
        cbuilder.onFailure([&](fu::Error errorCode, const std::string &errorMessage) {
            //connection failed - do something
        });

        this->eventLoopService = std::unique_ptr<fu::EventLoopService>(
            new fu::EventLoopService(this->threadCount)
        );

        this->connection = cbuilder.connect(*eventLoopService);
    }


    Response* Connection::sendRequest(Request* request)
    {
        fu::WaitGroup wg;
        wg.add();

        Response* response;

        this->connection->sendRequest(
            std::move(request->getFuerteRequest()),
            [&](fu::Error, std::unique_ptr<fu::Request>, std::unique_ptr<fu::Response> res){
                response = new Response(*res);

                wg.done();
            }
        );

        auto success = wg.wait_for(std::chrono::seconds(this->defaultTimeout));
        if(!success) {
            throw Php::Exception("Sending request to ArangoDB failed");
        }

        return response;
    }

    void Connection::sendRequestAsync(Request* request, Php::Value& callback)
    {
        this->asyncWaitGroup->add();

        this->connection->sendRequest(
            std::move(request->getFuerteRequest()),
            [=](fu::Error, std::unique_ptr<fu::Request>, std::unique_ptr<fu::Response> res){
                callback(Php::Object("ArangoDb\\Response", new Response(*res)));

                this->asyncWaitGroup->done();
            }
        );
    }


    Php::Value Connection::send(Php::Parameters &params)
    {
        if(!params[0].instanceOf("ArangoDb\\Request"))
            throw Php::Exception("Expected request to be of type Request");

        Request* request = (Request*)params[0].implementation();

        Response* response = this->sendRequest(request);
        return Php::Object("ArangoDb\\Response", response);
    }

    void Connection::sendAsync(Php::Parameters &params)
    {
        if(!params[0].instanceOf("ArangoDb\\Request"))
            throw Php::Exception("Expected request to be of type Request");

        if(!params[1].isCallable())
            throw Php::Exception("Expected callback to be of type Callable");

        Request* request = (Request*)params[0].implementation();
        this->sendRequestAsync(request, params[1]);
    }


    Php::Value Connection::methodDelete(Php::Parameters &params)
    {
        if(!params[1].instanceOf("ArangoDb\\Vpack")) {
            throw Php::Exception("Expected vpack to be of type Vpack");
        }

        Request request(params[0].stringValue(), (Vpack*)params[1].implementation());
        request.setHttpMethod(Request::METHOD_DELETE);
        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodGet(Php::Parameters &params)
    {
        if(!params[1].instanceOf("ArangoDb\\Vpack"))
            throw Php::Exception("Expected vpack to be of type Vpack");

        Request request(params[0].stringValue(), (Vpack*)params[1].implementation());
        request.setHttpMethod(Request::METHOD_GET);
        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodPost(Php::Parameters &params)
    {
        if(!params[1].instanceOf("ArangoDb\\Vpack"))
            throw Php::Exception("Expected vpack to be of type Vpack");

        Request request(params[0].stringValue(), (Vpack*)params[1].implementation());
        request.setHttpMethod(Request::METHOD_POST);
        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodPut(Php::Parameters &params)
    {
        if(!params[1].instanceOf("ArangoDb\\Vpack"))
            throw Php::Exception("Expected vpack to be of type Vpack");

        Request request(params[0].stringValue(), (Vpack*)params[1].implementation());
        request.setHttpMethod(Request::METHOD_PUT);
        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodHead(Php::Parameters &params)
    {
        if(!params[1].instanceOf("ArangoDb\\Vpack"))
            throw Php::Exception("Expected vpack to be of type Vpack");

        Request request(params[0].stringValue(), (Vpack*)params[1].implementation());
        request.setHttpMethod(Request::METHOD_HEAD);
        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodPatch(Php::Parameters &params)
    {
        if(!params[1].instanceOf("ArangoDb\\Vpack"))
            throw Php::Exception("Expected vpack to be of type Vpack");

        Request request(params[0].stringValue(), (Vpack*)params[1].implementation());
        request.setHttpMethod(Request::METHOD_PATCH);
        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodOptions(Php::Parameters &params)
    {
        if(!params[1].instanceOf("ArangoDb\\Vpack"))
            throw Php::Exception("Expected vpack to be of type Vpack");

        Request request(params[0].stringValue(), (Vpack*)params[1].implementation());
        request.setHttpMethod(Request::METHOD_OPTIONS);
        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    void Connection::wait()
    {
        auto success = this->asyncWaitGroup->wait_for(std::chrono::seconds(this->defaultTimeout));

        delete this->asyncWaitGroup;
        this->asyncWaitGroup = new fu::WaitGroup();

        if(!success) {
            throw Php::Exception("Sending request to ArangoDB failed");
        }
    }

}}}
