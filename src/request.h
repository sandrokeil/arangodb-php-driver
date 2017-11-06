#pragma once

#include <phpcpp.h>
#include <stdlib.h>
#include <iostream>

#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "velocypack/vpack.h"
#include "velocypack/velocypack-exception-macros.h"

#include "vpack.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace ArangoDb {
    class Request : public Php::Base
    {
    private:
        Vpack* vpack;
        std::unique_ptr<fu::Request> request;

    public:
        static const int8_t METHOD_DELETE = 0;
        static const int8_t METHOD_GET = 1;
        static const int8_t METHOD_POST = 2;
        static const int8_t METHOD_PUT = 3;
        static const int8_t METHOD_HEAD = 4;
        static const int8_t METHOD_PATCH = 5;
        static const int8_t METHOD_OPTIONS = 6;

        Request() = default;

        void __construct(Php::Parameters &params);

        const fu::Request& getFuerteRequest();

        virtual ~Request() = default;

    };

}
