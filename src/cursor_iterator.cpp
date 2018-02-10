#include "cursor_iterator.h"

namespace arangodb { namespace fuerte { namespace php {

    zend_object_iterator_funcs CursorIterator::iterator_funcs = {
        CursorIterator::iterator_dtor,
        CursorIterator::iterator_valid,
        CursorIterator::iterator_get_current_data,
        CursorIterator::iterator_get_current_key,
        CursorIterator::iterator_move_forward,
        CursorIterator::iterator_rewind,
        NULL /* invalidate_current is not used */
    };


    void CursorIterator::iterator_dtor(zend_object_iterator *iter TSRMLS_DC)
    {
    	CursorIterator *cursor_it = (CursorIterator *)iter;

    	if (!Z_ISUNDEF(cursor_it->intern.data)) {
    		zval_ptr_dtor(&cursor_it->intern.data);
    	}
    }

    int CursorIterator::iterator_valid(zend_object_iterator *iter TSRMLS_DC)
    {
    	Cursor *cursor = ((CursorIterator *)iter)->cursor;

        return cursor->valid() ? SUCCESS : FAILURE;
    }

    void CursorIterator::iterator_get_current_key(zend_object_iterator *iter, zval *key TSRMLS_DC)
    {
    	Cursor *cursor = ((CursorIterator *)iter)->cursor;

        ZVAL_LONG(key, cursor->key());
    }

    zval* CursorIterator::iterator_get_current_data(zend_object_iterator *iter)
    {
    	Cursor *cursor = ((CursorIterator *)iter)->cursor;

        return cursor->current();
    }

    void CursorIterator::iterator_move_forward(zend_object_iterator *iter TSRMLS_DC)
    {
        Cursor *cursor = ((CursorIterator *)iter)->cursor;

        Cursor::free_current(cursor);

        cursor->move_forward();
    }

    void CursorIterator::iterator_rewind(zend_object_iterator *iter TSRMLS_DC)
    {
        //@todo probably an error
    }

}}}
