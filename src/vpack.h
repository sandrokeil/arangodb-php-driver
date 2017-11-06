#pragma once

#include <phpcpp.h>
#include <stdlib.h>
#include <iostream>

#include "velocypack/vpack.h"
#include "velocypack/velocypack-exception-macros.h"

namespace vp = ::arangodb::velocypack;

namespace ArangoDb {

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

        virtual ~Vpack() = default;

    };

}
