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
        Response(const fu::Response &response);

        Php::Value getHttpCode();

        Php::Value getBody();
    };

}}}
