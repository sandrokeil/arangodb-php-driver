#include "vpack.h"

namespace ArangoDb {

    void Vpack::fromArray(Php::Parameters &params)
    {
        std::map<std::string, std::string> array = params[0];

        this->builder.openObject();
        for(auto const& entry : array) {
            this->builder.add(entry.first, vp::Value(entry.second));
        }

        this->builder.close();
    }

    void Vpack::fromJson(Php::Parameters &params)
    {
        vp::Parser parser;
        try {
            parser.parse(params[0]);
        }
        catch (std::bad_alloc const& e) {
            throw Php::Exception("Out of memory");
        }
        catch (vp::Exception const& e) {
            throw Php::Exception(e.what());
        }

        this->builder = *parser.steal();
    }

    void Vpack::__construct()
    {

    }

    Php::Value Vpack::toHex()
    {
        return vp::HexDump(this->builder.slice()).toString();
    }

    vp::Slice Vpack::getSlice()
    {
        return this->builder.slice();
    }

}