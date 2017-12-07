#pragma once

#include <phpcpp.h>

#include "velocypack/vpack.h"
#include "velocypack/velocypack-exception-macros.h"

#include "exception.h"

namespace vp = ::arangodb::velocypack;

namespace arangodb { namespace fuerte { namespace php {

    class Vpack : public Php::Base
    {
    private:
        vp::Builder builder;
        vp::Options dumperOptions;

    public:
        Vpack() = default;

        void __construct();
        static Php::Value fromArray(Php::Parameters &params);
        static Php::Value fromJson(Php::Parameters &params);
        Php::Value toHex();
        Php::Value toJson();

        vp::Slice getSlice();
        const vp::Builder& getBuilder() const;

        virtual ~Vpack() = default;

    };

}}}
