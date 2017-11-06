#include "vpack.h"

namespace ArangoDb {

    //temporary: replace it with either a c++ json parser (to save overhead from calling json_encode())
    //or a native php array to vpack conversion
    Php::Value Vpack::fromArray(Php::Parameters &params)
    {
        auto json = Php::call("json_encode", params[0]);
        Vpack* instance = new Vpack();

        vp::Parser parser;
        try {
            parser.parse(json);
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

    Php::Value Vpack::toJson()
    {
        try {
            std::string json;

            vp::StringSink sink(&json);
            vp::Dumper dumper(&sink, &this->dumperOptions);
            dumper.dump(this->builder.slice());

            return json;
        } catch(vp::Exception const& e) {
            throw Php::Exception(e.what());
        }
    }

    vp::Slice Vpack::getSlice()
    {
        return this->builder.slice();
    }

}