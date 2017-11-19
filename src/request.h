#pragma once

#include <phpcpp.h>

#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "vpack.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Request : public Php::Base
    {
    private:
        std::unique_ptr<fu::Request> request;

        fu::RestVerb httpMethod;
        std::string path;
        std::map<std::string, std::string> queryParams;
        vp::Builder builder;

    public:
        static const int8_t METHOD_DELETE = 0;
        static const int8_t METHOD_GET = 1;
        static const int8_t METHOD_POST = 2;
        static const int8_t METHOD_PUT = 3;
        static const int8_t METHOD_HEAD = 4;
        static const int8_t METHOD_PATCH = 5;
        static const int8_t METHOD_OPTIONS = 6;

        Request(std::string path, const Vpack* vpack);
        Request(std::string path, const Vpack* vpack, std::map<std::string, std::string> queryParams);
        Request();

        void __construct(Php::Parameters &params);
        void setHttpMethod(int8_t httpMethod);

        std::unique_ptr<fu::Request> getFuerteRequest();

        virtual ~Request() = default;

    };

}}}
