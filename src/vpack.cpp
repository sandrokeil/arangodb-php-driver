#include "vpack.h"

namespace arangodb { namespace fuerte { namespace php {

    Php::Value Vpack::fromJson(Php::Parameters &params)
    {
        Vpack* instance = new Vpack();

        vp::Parser parser;
        try {
            parser.parse(params[0]);
        }
        catch (std::bad_alloc const& e) {
            ARANGODB_THROW(RuntimeException(), "Out of memory in %s on line %d");
            return NULL;
        }
        catch (vp::Exception const& e) {
            ARANGODB_THROW(RuntimeException(), e.what());
            return NULL;
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
            ARANGODB_THROW(RuntimeException(), e.what());
            return NULL;
        }
    }

    void Vpack::toArray()
    {
        std::string json = this->toJson();

        zval* return_value;
        php_json_decode_ex(return_value, const_cast<char*>(json.c_str()), strlen(json.c_str()), 0, PHP_JSON_PARSER_DEFAULT_DEPTH);

        RETURN_ZVAL(return_value, 1, 0);
    }

    vp::Slice Vpack::getSlice()
    {
        return this->builder.slice();
    }

    const vp::Builder& Vpack::getBuilder() const
    {
        return this->builder;
    }

    vp::Builder* Vpack::accessBuilder()
    {
        return &this->builder;
    }

}}}
