#pragma once

extern "C" {
#include <php.h>
}

#include <iostream>

#include <velocypack/vpack.h>

namespace vp = ::arangodb::velocypack;

namespace arangodb { namespace fuerte { namespace php {

    class Vpack
    {
    public:
        vp::Builder builder;
        zend_object std;

        static zend_object_handlers php_arangodb_handler_vpack;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Vpack* fetch_object(zend_object* obj);

        Vpack();
        ~Vpack();

        void from_json(const char* json);
        void from_array(HashTable* myht);
        const char* to_json();
        const char* to_hex();

    private:
        void cast_assoc_array(HashTable* myht);
        void cast_numeric_array(HashTable* myht);
    };

}}}
