#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "connection.h"
#include "php_arangodb_classes.h"
#include "php_exception.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Cursor
    {
    public:
        static const int ENTRY_TYPE = 0;
        static const int ENTRY_TYPE_JSON = 100;
        static const int ENTRY_TYPE_ARRAY = 101;

    private:
        std::unique_ptr<fu::Response> response;
        Connection* connection;
        std::string id;

        std::vector<int> options = { Cursor::ENTRY_TYPE_JSON };

        int position = 0;
        int batch_size;
        int batch_count;
        bool has_more;
        zval current_value;

        void load_more();

    public:
        zend_object std;

        static zend_object_handlers handler_cursor;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Cursor* fetch_object(zend_object* obj);

        static zend_object_iterator* get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC);
        static void free_current(Cursor* cursor);

        Cursor(Connection* connection, std::unique_ptr<fu::Response> response);

        void set_option(int option, int value);

        bool valid();
        zval* current();
        int key();
        void move_forward();
        int count();

        int get_http_code();
    };

}}}
