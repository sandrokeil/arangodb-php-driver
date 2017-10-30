#include <phpcpp.h>

#include <algorithm>
#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <thread>
#include <arangodb.h>

Php::Value vpack(Php::Parameters &params)
{
    // there is one input array, cast the PHP variable to a vector of ints
    std::vector<int> input = params[0];

    std::cout << "Before Go" << std::endl;

    test();

    std::cout << "After Go" << std::endl;

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
