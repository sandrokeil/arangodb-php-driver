#include "cursor.h"
#include "cursor_iterator.h"
#include "php_arangodb_classes.h"

namespace arangodb { namespace fuerte { namespace php {

    zend_object_handlers Cursor::handler_cursor = zend_object_handlers();

    zend_object* Cursor::create_object(zend_class_entry* ce TSRMLS_CC)
    {
        Cursor* intern = (Cursor*)ecalloc(1, sizeof(Cursor) + zend_object_properties_size(ce));

        zend_object_std_init(&intern->std, ce TSRMLS_CC);

        intern->std.handlers = &Cursor::handler_cursor;
        return &intern->std;
    }

    Cursor* Cursor::fetch_object(zend_object* obj)
    {
        return (Cursor *)((char *)obj - XtOffsetOf(Cursor, std));
    }


    Cursor::Cursor(Connection* connection, std::unique_ptr<fu::Response> response):
        connection(connection),
        response(std::move(response))
    {
        this->has_more = this->response->slices().front().get("hasMore").getBool();
        this->batch_size = this->response->slices().front().get("result").length();

        if(this->has_more) {
            this->id = this->response->slices().front().get("id").copyString();
        }

        this->batch_count = this->batch_size;
        if(this->response->slices().front().hasKey("count")) {
            this->batch_count = this->response->slices().front().get("count").getInt();
        }
    }


    void Cursor::set_option(int option, int value)
    {
        if(this->options.size() <= option) {
            throw ArangoDbInvalidArgumentException(0, "Invalid cursor option provided");
        }

        this->options[option] = value;
    }


    zend_object_iterator* Cursor::get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC)
    {
        CursorIterator* cursor_it = NULL;
        Cursor* cursor = Z_OBJECT_CURSOR_P(object);

        if (by_ref) {
            //@todo exception/error
        }

        cursor_it = static_cast<CursorIterator*>(ecalloc(1, sizeof(CursorIterator)));
        zend_iterator_init(&cursor_it->intern);

        ZVAL_COPY(&cursor_it->intern.data, object);

        cursor_it->intern.funcs = &CursorIterator::iterator_funcs;
        cursor_it->cursor = cursor;

        Cursor::free_current(cursor);

        return &cursor_it->intern;
    }

    void Cursor::free_current(Cursor* cursor)
    {
        if(!Z_ISUNDEF(cursor->current_value)) {
            zval_ptr_dtor(&cursor->current_value);
            ZVAL_UNDEF(&cursor->current_value);
        }
    }


    bool Cursor::valid()
    {
        if(this->batch_size > this->position) {
            return true;
        } else if(this->has_more) {
            this->load_more();
            this->position = 0;
            return true;
        }

        return false;
    }

    zval* Cursor::current()
    {
        if(this->options[Cursor::ENTRY_TYPE] == Cursor::ENTRY_TYPE_JSON) {
            std::string body;

            try {
                vp::Slice slice = this->response->slices().front().get("result").at(this->position);
                vp::Options dumperOptions;

                vp::StringSink sink(&body);
                vp::Dumper dumper(&sink, &dumperOptions);
                dumper.dump(slice);
            } catch(vp::Exception const& e) {
                throw ArangoDbInvalidArgumentException(0, e.what());
            }

            ZVAL_STRINGL(&this->current_value, body.c_str(), body.size());

        } else if(this->options[Cursor::ENTRY_TYPE] == Cursor::ENTRY_TYPE_ARRAY) {
            auto slice = this->response->slices().front().get("result").at(this->position);
            VpackConversion::vpack_to_array(&slice, &this->current_value);
        }

        return &this->current_value;
    }

    int Cursor::key()
    {
        return this->position;
    }

    void Cursor::move_forward()
    {
        this->position++;
    }

    int Cursor::count()
    {
        return this->batch_count;
    }

    void Cursor::load_more()
    {
        this->response = this->connection->send(3, std::string("/_api/cursor/" + this->id).c_str(), "{}", NULL);
        this->has_more = this->response->slices().front().get("hasMore").getBool();
        this->batch_size = this->response->slices().front().get("result").length();

        this->batch_count = this->batch_size;
        if(this->response->slices().front().hasKey("count")) {
            this->batch_count = this->response->slices().front().get("count").getInt();
        }
    }

    int Cursor::get_http_code()
    {
        return this->response->statusCode();
    }

}}}