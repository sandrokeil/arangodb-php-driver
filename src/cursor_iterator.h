#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "cursor.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class CursorIterator
    {
    private:
        friend class cursor;


    public:

        zend_object_iterator intern;
        Cursor* cursor;
        zval* cursor_value;

        static zend_object_iterator_funcs iterator_funcs;
        static void iterator_dtor(zend_object_iterator *iter TSRMLS_DC);
        static int iterator_valid(zend_object_iterator *iter TSRMLS_DC);
        static void iterator_get_current_key(zend_object_iterator *iter, zval *key TSRMLS_DC);
        static zval* iterator_get_current_data(zend_object_iterator *iter);
        static void iterator_move_forward(zend_object_iterator *iter TSRMLS_DC);
        static void iterator_rewind(zend_object_iterator *iter TSRMLS_DC);

    };

}}}
