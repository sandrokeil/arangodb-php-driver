#include <phpcpp.h>

#include <algorithm>
#include <stdlib.h>

#include <fuerte/fuerte.h>
#include <fuerte/loop.h>
#include <fuerte/FuerteLogger.h>
#include <fuerte/connection.h>
#include <fuerte/requests.h>

#include <iostream>
#include <iomanip>
#include "velocypack/vpack.h"
#include "velocypack/velocypack-exception-macros.h"

#include <velocypack/Parser.h>
#include <velocypack/Iterator.h>
#include <velocypack/velocypack-aliases.h>

#include <chrono>
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <thread>

namespace f = ::arangodb::fuerte;
namespace fu = ::arangodb::fuerte;
namespace vp = ::arangodb::velocypack;

//void createCollection(std::string collectionName, int timeout);


Php::Value vpack(Php::Parameters &params)
{
    // there is one input array, cast the PHP variable to a vector of ints
    std::vector<int> input = params[0];

    vp::Builder b;

    b.add(vp::Value(vp::ValueType::Object));
    b.add("b", vp::Value(12));
    b.add("a", vp::Value(true));
    b.add("l", vp::Value(vp::ValueType::Array));
    b.add(vp::Value(1));
    b.add(vp::Value(2));
    b.add(vp::Value(3));
    b.close();
    b.add("name", vp::Value("Gustav"));
    b.close();

    // now dump the resulting VPack value
    std::cout << "Resulting VPack:" << b.slice() << std::endl;
    std::cout << vp::HexDump(b.slice()) << std::endl;

    std::cout << vp::valueTypeName(vp::ValueType::Object) << std::endl;

    std::cout << std::endl << std::endl;
    //createCollection("testobi", 3);

    return input;
}


Php::Value createCollection(Php::Parameters &params)
{
    std::string collectionName = params[0];
    int timeout = params[1];

    std::shared_ptr<f::Connection> connection;
    std::unique_ptr<f::EventLoopService> eventLoopService = std::unique_ptr<f::EventLoopService>(new f::EventLoopService(1));

    f::WaitGroup wg;
    wg.add();

    f::ConnectionBuilder cbuilder;
    cbuilder.host("vst://arangodb:8529");
    cbuilder.onFailure([&](f::Error errorCode, const std::string& errorMessage){  });
    connection = cbuilder.connect(*eventLoopService);

    VPackBuilder builder;
    builder.openObject();
    builder.add("name" , VPackValue(collectionName));
    builder.close();

    auto request = fu::createRequest(fu::RestVerb::Post, "/_api/collection");
    request->addVPack(builder.slice());
    connection->sendRequest(std::move(request),[&](f::Error, std::unique_ptr<f::Request>, std::unique_ptr<f::Response> response){
        std::cout << response->statusCode() << std::endl;
        wg.done();
    });


    auto success = wg.wait_for(std::chrono::seconds(timeout));
    if(!success) {
        throw Php::Exception("Could not connect to arangodb");
    }

    return true;
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


        extension.add<createCollection>("createCollection", {
            Php::ByVal("collectionName", Php::Type::String),
            Php::ByVal("timeout", Php::Type::Numeric)
        });

        // return the extension
        return extension;
    }
}
