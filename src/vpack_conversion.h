#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>

#include "php_arangodb_classes.h"
#include "php_exception.h"

namespace vp = ::arangodb::velocypack;

namespace arangodb { namespace fuerte { namespace php {

    class VpackConversion
    {
    private:
        static void cast_assoc_array(const HashTable* ht, vp::Builder* builder);
        static void cast_numeric_array(const HashTable* ht, vp::Builder* builder);

        static void vpack_array_to_php_array(const vp::Slice* slice, zval* array);
        static void vpack_object_to_php_array(const vp::Slice* slice, zval* array);

    public:
        static void array_to_vpack(const HashTable* ht, vp::Builder* builder);
        static void vpack_to_array(const vp::Slice* slice, zval* array);
    };

}}}
