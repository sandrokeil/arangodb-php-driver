#include "cursorIterator.h"

namespace arangodb { namespace fuerte { namespace php {

    CursorIterator::CursorIterator(Cursor* cursor): Php::Iterator(cursor), cursor(cursor)
    {
    }

    bool CursorIterator::valid()
    {
        if(this->cursor->batchSize > this->position) {
            return true;
        } else if(this->cursor->hasMore) {
            this->cursor->loadMore();
            this->position = 0;
            return true;
        }

        return false;
    }

    Php::Value CursorIterator::current()
    {
        std::string body;

        try {
            vp::Slice slice = this->cursor->response->getFuerteResponse()->slices().front().get("result").at(this->position);
            vp::Options dumperOptions;

            vp::StringSink sink(&body);
            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(slice);
        } catch(vp::Exception const& e) {
            throw Php::Exception(e.what());
        }

        return body;
    }

    Php::Value CursorIterator::key()
    {
        return this->position;
    }

    void CursorIterator::next()
    {
        this->position++;
    }

    void CursorIterator::rewind()
    {
    }

    long CursorIterator::count() {
        return this->cursor->number;
    }

}}}
