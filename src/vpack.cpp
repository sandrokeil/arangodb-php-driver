#include "vpack.h"

namespace arangodb { namespace fuerte { namespace php {

    zend_object_handlers Vpack::php_arangodb_handler_vpack = zend_object_handlers();

    zend_object* Vpack::create_object(zend_class_entry* ce TSRMLS_CC)
    {
        Vpack* intern = (Vpack*)ecalloc(1, sizeof(Vpack) + zend_object_properties_size(ce));
        std::cout << "memmm: " << sizeof(Vpack) + zend_object_properties_size(ce) << std::endl;

        zend_object_std_init(&intern->std, ce TSRMLS_CC);

        intern->std.handlers = &Vpack::php_arangodb_handler_vpack;
        return &intern->std;
    }

    Vpack* Vpack::fetch_object(zend_object* obj)
    {
        return (Vpack *)((char *)obj - XtOffsetOf(Vpack, std));
    }


    Vpack::Vpack()
    {
    }

    Vpack::~Vpack()
    {
    }

    void Vpack::fromJson(const char* json)
    {
        vp::Parser parser;

        try {
            parser.parse(json);
        }
        catch(std::bad_alloc const &e) {
            std::cout << "bad_alloc" << std::endl;
        }
        catch(vp::Exception const &e) {
            std::cout << "Exception" << std::endl;
        }

        this->builder = *parser.steal();
    }

    const char* Vpack::toJson()
    {
        try {
            std::string json;

            vp::StringSink sink(&json);
            vp::Options dumperOptions;

            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(this->builder.slice());

            return json.c_str();
        }
        catch(vp::Exception const &e) {
            std::cout << "exception" << std::endl;
        }
    }

    const char* Vpack::toHex()
    {
        return vp::HexDump(this->builder.slice()).toString().c_str();
    }

}}}
