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

        static Php::Value fromJson(Php::Parameters &params);
        Php::Value toHex();
        Php::Value toJson();

        vp::Slice getSlice();
        const vp::Builder& getBuilder() const;

        /**
         * Grants direct access to the internal builder instance
         * @todo find a better solution for this
         */
        vp::Builder* accessBuilder();

        virtual ~Vpack() = default;

    };

}}}
