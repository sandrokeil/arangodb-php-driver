#include <phpcpp.h>

/**
 * Test includes, remove if not needed
 */
// #include <fuerte/helper.h>
// #include <fuerte/loop.h>
// #include <fuerte/FuerteLogger.h>

#define VELOCYPACK_XXHASH 1

#include <iostream>
#include <iomanip>
#include "velocypack/vpack.h"
#include "velocypack/velocypack-exception-macros.h"

using namespace arangodb::velocypack;

Php::Value vpack(Php::Parameters &params)
{
    // there is one input array, cast the PHP variable to a vector of ints
    std::vector<int> input = params[0];

//    VELOCYPACK_GLOBAL_EXCEPTION_TRY
//
//    Builder b;
//
//    b.add(Value(ValueType::Object));
//    b.add("b", Value(12));
//    b.add("a", Value(true));
//    b.add("l", Value(ValueType::Array));
//    b.add(Value(1));
//    b.add(Value(2));
//    b.add(Value(3));
//    b.close();
//    b.add("name", Value("Gustav"));
//    b.close();
//
//    // now dump the resulting VPack value
//    std::cout << "Resulting VPack:" << std::endl;
//    std::cout << "Resulting VPack:" << b.slice() << std::endl;
//    std::cout << HexDump(b.slice()) << std::endl;
//
//    VELOCYPACK_GLOBAL_EXCEPTION_CATCH

    std::cout << valueTypeName(ValueType::Object) << std::endl;
    // done
    return input;
}

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {

    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module()
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("arangodb", "0.1");

        extension.add<vpack>("vpack", {
            Php::ByVal("input", Php::Type::Array)
        });


        // return the extension
        return extension;
    }
}