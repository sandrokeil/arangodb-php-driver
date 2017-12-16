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

    Php::Value Response::accessResponse(Php::Parameters& params)
    {
        std::vector<std::string> accessor;
        std::istringstream accessorStr(params[0]);

        for(std::string token; std::getline(accessorStr, token, '.'); ) {
            accessor.push_back(std::move(token));
        }

        vp::Slice slice = this->response.slices().front().get(accessor);
        return this->sliceToPhpValue(slice);
    }

    Php::Value Response::accessResponseTop(Php::Parameters& params)
    {
        std::string accessor = params[0];

        vp::Slice slice = this->response.slices().front().get(accessor);
        return this->sliceToPhpValue(slice);
    }

    Php::Value Response::sliceToPhpValue(const vp::Slice& slice)
    {
        switch(slice.type()) {
            case vp::ValueType::String:
                return slice.copyString();
                break;

            case vp::ValueType::Int:
            case vp::ValueType::UInt:
            case vp::ValueType::SmallInt:
                return slice.getInt();
                break;

            case vp::ValueType::Double:
                return slice.getDouble();
                break;

            case vp::ValueType::Null:
                return NULL;
                break;

            case vp::ValueType::Array:
                return -1;
                break;

            case vp::ValueType::Object:
                return -1;
                break;

            default:
                return -1;
        }
    }


    fu::Response* Response::getFuerteResponse()
    {
        return &this->response;
    }

}}}
