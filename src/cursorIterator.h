#pragma once

#include <phpcpp.h>

#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "cursor.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class CursorIterator : public Php::Base, public Php::Iterator, public Php::Countable
    {
    private:
        Cursor* cursor;
        int position = 0;

    public:
        CursorIterator(Cursor* cursor);
        virtual ~CursorIterator() = default;

        virtual bool valid() override;
        virtual Php::Value current() override;
        virtual Php::Value key() override;
        virtual void next() override;
        virtual void rewind() override;
        virtual long count() override;
    };

}}}
