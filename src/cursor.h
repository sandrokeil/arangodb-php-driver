#pragma once

#include <phpcpp.h>

#include <fuerte/fuerte.h>
#include <fuerte/types.h>
#include <stdint-gcc.h>

#include "connection.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Cursor : public Php::Base, public Php::Countable
    {
    private:
        Connection* connection;
        Vpack* vpack;

        bool hasMore;
        int position = 0;
        std::string id;
        int64_t batchSize;
        int64_t number;

        Response* response;

        void loadFirstBatch();
        void loadMore();

    public:
        Cursor(Connection* connection, Vpack* vpack);

        virtual long count() override;
        Php::Value getCount();

        Php::Value valid();
        Php::Value current();
        Php::Value key();
        void next();
        void rewind();

        virtual ~Cursor() = default;
    };

}}}
