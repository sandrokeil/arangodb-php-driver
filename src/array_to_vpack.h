#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>

namespace vp = ::arangodb::velocypack;

namespace arangodb { namespace fuerte { namespace php {

    class ArrayToVpack
    {
    private:
        static void cast_assoc_array(const HashTable* ht, vp::Builder* builder);
        static void cast_numeric_array(const HashTable* ht, vp::Builder* builder);

    public:
        static void cast_array(const HashTable* ht, vp::Builder* builder);
    };

}}}
