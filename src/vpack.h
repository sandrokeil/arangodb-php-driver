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

    public:
        Vpack() = default;

        void __construct();
        void fromArray(Php::Parameters &params);
        void fromJson(Php::Parameters &params);
        Php::Value toHex();

        vp::Slice getSlice();

        virtual ~Vpack() = default;

    };

}
