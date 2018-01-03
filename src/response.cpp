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

    Php::Value Response::get(Php::Parameters& params)
    {
        try {

            if(params[0].isString()) {
                return this->sliceToPhpValue(this->response.slices().front().get(params[0].rawValue()));

            } else if(params[0].isArray()) {
                vp::Slice tmpSlice(this->response.slices().front());

                for(auto const &value : params[0]) {
                    if(value.second.isNumeric()) {
                        tmpSlice = vp::Slice(tmpSlice.at(value.second.numericValue()));
                    } else if(value.second.isString()) {
                        tmpSlice = vp::Slice(tmpSlice.get(value.second.rawValue()));
                    } else {
                        ARANGODB_THROW(InvalidArgumentException(), "The accessor may only contain strings and integers in %s on line %d");
                        return NULL;
                    }
                }

                return this->sliceToPhpValue(tmpSlice);
            } else {
                ARANGODB_THROW(InvalidArgumentException(), "The accessor must be either of type array or of type string in %s on line %d");
                return NULL;
            }

        } catch(vp::Exception e) {
            ARANGODB_THROW(RuntimeException(), e.what());
            return NULL;
        }
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
            case vp::ValueType::Object:
                return this->sliceToJson(slice);
                break;

            default:
                return -1;
        }
    }


    Php::Value Response::sliceToJson(const vp::Slice& slice)
    {
        std::string json;

        try {
            vp::Options dumperOptions;

            vp::StringSink sink(&json);
            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(slice);
        } catch(vp::Exception const& e) {
            ARANGODB_THROW(InvalidArgumentException(), e.what());
            return NULL;
        }

        return json;
    }


    fu::Response* Response::getFuerteResponse()
    {
        return &this->response;
    }

}}}
