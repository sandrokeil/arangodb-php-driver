#pragma once

#include <phpcpp.h>

#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "connection.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Cursor : public Php::Base, public Php::Traversable
    {
    private:
        friend class CursorIterator;

        Connection* connection;
        Vpack* vpack;

        bool hasMore;
        std::string id;
        int batchSize;

        Response* response;

        void loadFirstBatch();
        void loadMore();

    public:
        Cursor(Connection* connection, Vpack* vpack);

        virtual Php::Iterator *getIterator() override;

        virtual ~Cursor() = default;
    };

}}}
