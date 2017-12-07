#include "response.h"

namespace arangodb { namespace fuerte { namespace php {

    Response::Response(const fu::Response &response): response(response)
    {
    }

    Php::Value Response::getHttpCode()
    {
        return static_cast<int>(this->response.statusCode());
    }

    Php::Value Response::getBody()
    {
        std::string body;

        try {
            vp::Slice slice = this->response.slices().front();
            vp::Options dumperOptions;

            vp::StringSink sink(&body);
            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(slice);
        } catch(vp::Exception const& e) {
            ARANGODB_THROW(InvalidArgumentException(), e.what());
            return NULL;
        }

        return body;
    }

    fu::Response* Response::getFuerteResponse()
    {
        return &this->response;
    }

}}}
