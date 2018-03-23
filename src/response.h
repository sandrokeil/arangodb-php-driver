#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>

#include "vpack_conversion.h"
#include "php_arangodb_classes.h"
#include "php_exception.h"

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Response
    {
    private:
        //@todo find out if it's worth using a std::unique_ptr<fu::Response> here
        fu::Response response;

        void return_slice_to_php_value(zval* return_value, const vp::Slice& slice);

    public:
        zend_object std;

        static zend_object_handlers handler_response;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Response* fetch_object(zend_object* obj);

        Response(const fu::Response &response);
        int get_http_code();
        void return_body(zval* return_value);
        fu::Response* get_fuerte_response();

        void get(zval* return_value, HashTable* accessor);
        void get(zval* return_value, const char* accessor);

        bool assert_success();
    };

}}}
