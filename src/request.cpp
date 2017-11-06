#include "request.h"

namespace ArangoDb {

    void Request::__construct(Php::Parameters &params)
    {
        if(!params[2].instanceOf("ArangoDb\\Vpack"))
            throw Php::Exception("Expected vpack to be of type Vpack");

        this->request = fu::createRequest(static_cast<fu::RestVerb>(params[0].numericValue()), params[1]);
        this->vpack = (ArangoDb::Vpack*)params[2].implementation();

        this->request->addVPack(this->vpack->getSlice());
    }


    const fu::Request& Request::getFuerteRequest()
    {
        return *this->request;
    }
}
