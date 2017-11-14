#pragma once

#include <phpcpp.h>

#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "velocypack/vpack.h"
#include "velocypack/velocypack-exception-macros.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Response : public Php::Base
    {
    private:
        fu::Response response;

    public:
        Response(const fu::Response &response): response(response)
        {
        }

        Php::Value getHttpCode()
        {
            return static_cast<int>(this->response.statusCode());
        }

        Php::Value getBody()
        {
            std::string body;

            try {
                vp::Slice slice = this->response.slices().front();
                vp::Options dumperOptions;

                vp::StringSink sink(&body);
                vp::Dumper dumper(&sink, &dumperOptions);
                dumper.dump(slice);
            } catch(vp::Exception const& e) {
                throw Php::Exception(e.what());
            }

            return body;
        }

        virtual ~Response() = default;

    };

}}}
