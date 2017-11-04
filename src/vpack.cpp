#include "vpack.h"

namespace ArangoDb {

    Php::Value Vpack::fromArray(Php::Parameters &params)
    {
        std::map<std::string, std::string> array = params[0];

        Vpack* instance = new Vpack();
        instance->builder.openObject();
        for(auto const& entry : array) {
            instance->builder.add(entry.first, vp::Value(entry.second));
        }

        instance->builder.close();
        return Php::Object("ArangoDb\\Vpack", instance);
    }

    Php::Value Vpack::fromJson(Php::Parameters &params)
    {
        Vpack* instance = new Vpack();

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

        instance->builder = *parser.steal();
        return Php::Object("ArangoDb\\Vpack", instance);
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