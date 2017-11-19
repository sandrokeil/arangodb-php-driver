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

    void Request::__construct(Php::Parameters &params)
    {
        if(!params[2].instanceOf("ArangoDb\\Vpack")) {
            throw Php::Exception("Expected vpack to be of type Vpack");
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
