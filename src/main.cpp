#include <phpcpp.h>

#include <velocypack/Parser.h>
#include <velocypack/Iterator.h>
#include <velocypack/velocypack-aliases.h>

#include "connection.h"
#include "vpack.h"
#include "request.h"

namespace fu = ::arangodb::fuerte;
namespace vp = ::arangodb::velocypack;

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

    std::shared_ptr<fu::Connection> connection;
    std::unique_ptr<fu::EventLoopService> eventLoopService = std::unique_ptr<fu::EventLoopService>(new fu::EventLoopService(1));

    fu::WaitGroup wg;
    wg.add();

    fu::ConnectionBuilder cbuilder;
    cbuilder.host("vst://arangodb:8529");
    cbuilder.onFailure([&](fu::Error errorCode, const std::string& errorMessage){  });
    connection = cbuilder.connect(*eventLoopService);

    VPackBuilder builder;
    builder.openObject();
    builder.add("name" , VPackValue(collectionName));
    builder.close();

    auto request = fu::createRequest(fu::RestVerb::Post, "/_api/collection");
    request->addVPack(builder.slice());
    connection->sendRequest(std::move(request),[&](fu::Error, std::unique_ptr<fu::Request>, std::unique_ptr<fu::Response> response){
        std::cout << response->statusCode() << std::endl;
        wg.done();
    });


    auto success = wg.wait_for(std::chrono::seconds(timeout));
    if(!success) {
        throw Php::Exception("Could not connect to arangodb");
    }

    return true;
}











extern "C" {

    void exportClassConnection(Php::Extension* extension);
    void exportClassVpack(Php::Extension* extension);
    void exportClassRequest(Php::Extension* extension);
    void exportClassResponse(Php::Extension* extension);

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

        exportClassConnection(&extension);
        exportClassVpack(&extension);
        exportClassRequest(&extension);
        exportClassResponse(&extension);

        return extension;
    }


    void exportClassConnection(Php::Extension* extension)
    {
        Php::Class<arangodb::fuerte::php::Connection> connection("ArangoDb\\Connection");

        connection.method<&arangodb::fuerte::php::Connection::__construct>("__construct",{
            Php::ByVal("options", Php::Type::Array, true),
        });
        connection.method<&arangodb::fuerte::php::Connection::connect>("connect");
        connection.method<&arangodb::fuerte::php::Connection::send>("send", {
            Php::ByVal("request", "ArangoDb\\Request", true)
        });
        connection.method<&arangodb::fuerte::php::Connection::sendAsync>("sendAsync", {
            Php::ByVal("request", "ArangoDb\\Request", true),
            Php::ByVal("callback", Php::Type::Callable, true)
        });
        connection.method<&arangodb::fuerte::php::Connection::setThreadCount>("setThreadCount", {
            Php::ByVal("threadCount", Php::Type::Numeric, true)
        });

        Php::Arguments methodArgs = {
            Php::ByVal("path", Php::Type::String, true),
            Php::ByVal("vpack", "ArangoDb\\Vpack", true)
        };

        connection.method<&arangodb::fuerte::php::Connection::methodDelete>("delete", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodGet>("get", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodPost>("post", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodPut>("put", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodHead>("head", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodPatch>("patch", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodOptions>("options", methodArgs);

        connection.method<&arangodb::fuerte::php::Connection::wait>("wait");

        connection.property("HOST", "host", Php::Const);
        connection.property("USER", "user", Php::Const);
        connection.property("PASSWORD", "password", Php::Const);
        connection.property("MAX_CHUNK_SIZE", "max_chunk_size", Php::Const);
        connection.property("VST_VERSION", "vst_version", Php::Const);
        connection.property("VST_VERSION_10", 0, Php::Const);
        connection.property("VST_VERSION_11", 1, Php::Const);

        // add the class to the extension
        extension->add(std::move(connection));
    }


    void exportClassVpack(Php::Extension* extension)
    {
        Php::Class<arangodb::fuerte::php::Vpack> vpack("ArangoDb\\Vpack");

        vpack.method<&arangodb::fuerte::php::Vpack::__construct>("__construct");
        vpack.method<&arangodb::fuerte::php::Vpack::fromArray>("fromArray", {
            Php::ByVal("array", Php::Type::Array, true)
        });
        vpack.method<&arangodb::fuerte::php::Vpack::fromJson>("fromJson", {
            Php::ByVal("json", Php::Type::String, true)
        });
        vpack.method<&arangodb::fuerte::php::Vpack::toHex>("toHex");
        vpack.method<&arangodb::fuerte::php::Vpack::toJson>("toJson");

        extension->add(std::move(vpack));
    }


    void exportClassRequest(Php::Extension* extension)
    {
        Php::Class<arangodb::fuerte::php::Request> request("ArangoDb\\Request");

        request.method<&arangodb::fuerte::php::Request::__construct>("__construct", {
            Php::ByVal("method", Php::Type::Numeric, true),
            Php::ByVal("path", Php::Type::String, true),
            Php::ByVal("vpack", "ArangoDb\\Vpack", true)
        });

        request.property("METHOD_DELETE", arangodb::fuerte::php::Request::METHOD_DELETE, Php::Const);
        request.property("METHOD_GET", arangodb::fuerte::php::Request::METHOD_GET, Php::Const);
        request.property("METHOD_POST", arangodb::fuerte::php::Request::METHOD_POST, Php::Const);
        request.property("METHOD_PUT", arangodb::fuerte::php::Request::METHOD_PUT, Php::Const);
        request.property("METHOD_HEAD", arangodb::fuerte::php::Request::METHOD_HEAD, Php::Const);
        request.property("METHOD_PATCH", arangodb::fuerte::php::Request::METHOD_PATCH, Php::Const);
        request.property("METHOD_OPTIONS", arangodb::fuerte::php::Request::METHOD_OPTIONS, Php::Const);

        extension->add(std::move(request));
    }


    void exportClassResponse(Php::Extension* extension)
    {
        Php::Class<arangodb::fuerte::php::Response> response("ArangoDb\\Response");

        response.method<&arangodb::fuerte::php::Response::getHttpCode>("getHttpCode");
        response.method<&arangodb::fuerte::php::Response::getBody>("getBody");

        extension->add(std::move(response));
    }
}
