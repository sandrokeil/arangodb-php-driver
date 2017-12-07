#pragma once

#include <phpcpp.h>

namespace arangodb { namespace fuerte { namespace php {

    class InsertDocument : public Php::Base
    {
    private:
        std::string collectionName;
        Php::Value options;
        Php::Value streamEvents;

    public:
        InsertDocument(std::string collectionName, Php::Value streamEvents, Php::Value options);

        Php::Value with(Php::Parameters &params);

        virtual ~InsertDocument() = default;

    };

}}}
