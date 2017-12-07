#include "request.h"

namespace arangodb { namespace fuerte { namespace php {

    Request::Request(std::string path, const Vpack* vpack): path(path), builder(vpack->getBuilder())
    {
    }

    Request::Request(std::string path, const Vpack* vpack, std::map<std::string, std::string> queryParams): path(path), builder(vpack->getBuilder()), queryParams(queryParams)
    {
    }

    Request::Request()
    {
    }

    Request::Request(Php::Parameters params)
    {
        if(!params[1].instanceOf("ArangoDb\\Vpack")) {
            ARANGODB_THROW(InvalidArgumentException(), "Expected vpack to be of type Vpack in %s on line %d");
            return;
        }

        if(params.size() == 3 && params[2].size() > 0) {
            this->queryParams = params[2];
        }

        this->path = params[0].stringValue();
        this->builder = ((Vpack*)params[1].implementation())->getBuilder();
    }

    void Request::__construct(Php::Parameters &params)
    {
        if(!params[2].instanceOf("ArangoDb\\Vpack")) {
            ARANGODB_THROW(InvalidArgumentException(), "Expected vpack to be of type Vpack in %s on line %d");
            return;
        }

        if(params.size() == 4 && params[3].size() > 0) {
            this->queryParams = params[3];
        }

        this->httpMethod = static_cast<fu::RestVerb>(params[0].numericValue());
        this->path = params[1].stringValue();

        this->builder = ((Vpack*)params[2].implementation())->getBuilder();
    }


    void Request::setHttpMethod(int8_t httpMethod)
    {
        this->httpMethod = static_cast<fu::RestVerb>(httpMethod);
    }


    std::unique_ptr<fu::Request> Request::getFuerteRequest()
    {
        auto request = fu::createRequest(this->httpMethod, this->path, this->queryParams);
        request->addVPack(this->builder.slice());

        return request;
    }

}}}
