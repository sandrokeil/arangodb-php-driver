#pragma once

extern "C" {
#include "php.h"
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

        void fromJson(const char* json);
        const char* toJson();
        const char* toHex();
    };

}}}
