#pragma once

#include <phpcpp.h>

#include <sstream>

#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "velocypack/vpack.h"
#include "velocypack/velocypack-exception-macros.h"
#include "velocypack/ValueType.h"

#include "exception.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Response : public Php::Base
    {
    private:
        fu::Response response;

        Php::Value sliceToPhpValue(const vp::Slice& slice);

    public:
        Response(const fu::Response &response);

        Php::Value getHttpCode();
        Php::Value getBody();
        Php::Value accessResponse(Php::Parameters& params);
        Php::Value accessResponseTop(Php::Parameters& params);

        fu::Response* getFuerteResponse();
    };

}}}
