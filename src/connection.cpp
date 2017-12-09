#include "connection.h"
#include "cursor.h"

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
            ARANGODB_THROW(InvalidArgumentException(), "Invalid threadCount provided, must be >= 1 in %s on line %d");
            return;
        }

        this->threadCount = threadCount;
    }

    void Connection::setDefaultTimeout(Php::Parameters &params)
    {
        int defaultTimeout = params[0];

        if(defaultTimeout < 1) {
            ARANGODB_THROW(InvalidArgumentException(), "Invalid defaultTimeout provided, must be >= 1 in %s on line %d");
            return;
        }

        this->defaultTimeout = defaultTimeout;
    }

    fu::ConnectionBuilder Connection::createConnectionBuilder()
    {
        fu::ConnectionBuilder cbuilder{};

        try {
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
        }
        catch (const std::exception &ex) {
            ARANGODB_THROW(InvalidOptionException(), "Unknown option provided in %s on line %d");
        }

        return cbuilder;
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
        auto result = this->connection->sendRequest(
            std::move(request->getFuerteRequest())
        );

        auto response = new Response(*result);

        auto statusCode = response->getHttpCode();
        if((!(statusCode >= 200 && statusCode <= 299))/* || response->getFuerteResponse()->slices().front().get("error").getBool()*/) {
            throwRequestFailedException("This is the error message", statusCode, response->getBody());
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
        if(!params[0].instanceOf("ArangoDb\\Request")) {
            ARANGODB_THROW(InvalidArgumentException(), "Expected request to be of type Request in %s on line %d");
            return NULL;
        }

        Request* request = (Request*)params[0].implementation();

        Response* response = this->sendRequest(request);
        return Php::Object("ArangoDb\\Response", response);
    }

    void Connection::sendAsync(Php::Parameters &params)
    {
        if(!params[0].instanceOf("ArangoDb\\Request")) {
            ARANGODB_THROW(InvalidArgumentException(), "Expected request to be of type Request in %s on line %d");
            return;
        }

        if(!params[1].isCallable()) {
            ARANGODB_THROW(InvalidArgumentException(), "Expected callback to be of type Callable in %s on line %d");
            return;
        }

        Request* request = (Request*)params[0].implementation();
        this->sendRequestAsync(request, params[1]);
    }

    Php::Value Connection::methodDelete(Php::Parameters &params)
    {
        Request request(params);
        request.setHttpMethod(Request::METHOD_DELETE);

        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodGet(Php::Parameters &params)
    {
        Request request(params);
        request.setHttpMethod(Request::METHOD_GET);

        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodPost(Php::Parameters &params)
    {
        Request request(params);
        request.setHttpMethod(Request::METHOD_POST);

        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodPut(Php::Parameters &params)
    {
        Request request(params);
        request.setHttpMethod(Request::METHOD_PUT);

        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodHead(Php::Parameters &params)
    {
        Request request(params);
        request.setHttpMethod(Request::METHOD_HEAD);

        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodPatch(Php::Parameters &params)
    {
        Request request(params);
        request.setHttpMethod(Request::METHOD_PATCH);

        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    Php::Value Connection::methodOptions(Php::Parameters &params)
    {
        Request request(params);
        request.setHttpMethod(Request::METHOD_OPTIONS);

        return Php::Object("ArangoDb\\Response", this->sendRequest(&request));
    }

    void Connection::wait()
    {
        auto success = this->asyncWaitGroup->wait_for(std::chrono::seconds(this->defaultTimeout));

        delete this->asyncWaitGroup;
        this->asyncWaitGroup = new fu::WaitGroup();

        if(!success) {
            ARANGODB_THROW(RuntimeException(), "Sending request to ArangoDB failed in %s on line %d");
            return;
        }
    }

    Php::Value Connection::query(Php::Parameters &params)
    {
        if(!params[0].instanceOf("ArangoDb\\Vpack")) {
            ARANGODB_THROW(InvalidArgumentException(), "Expected vpack to be of type Vpack in %s on line %d");
            return NULL;
        }

        Cursor* cursor = new Cursor(this, (Vpack*)params[0].implementation());

        for(auto option : params[1]) {
            cursor->setOption(option.first, option.second);
        }

        return Php::Object("ArangoDb\\Cursor", cursor);
    }

}}}
