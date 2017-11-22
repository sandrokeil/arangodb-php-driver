#pragma once

#include <phpcpp.h>

#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "connection.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Cursor : public Php::Base, public Php::Traversable, public Php::Countable
    {
    private:
        friend class CursorIterator;

        Connection* connection;
        Vpack* vpack;

        bool hasMore;
        std::string id;
        unsigned long batchSize;
        int_fast64_t number;

        Response* response;

        void loadFirstBatch();
        void loadMore();

    public:
        Cursor(Connection* connection, Vpack* vpack);

        virtual Php::Iterator *getIterator() override;

        /**
         *  Method from the Php::Countable interface, that
         *  is used when a Counter instance is passed to the
         *  PHP count() function
         *
         *  @return long
         */
        virtual long count() override;

        Php::Value getCount();

        virtual ~Cursor() = default;
    };

}}}
