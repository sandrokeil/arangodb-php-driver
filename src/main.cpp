#include <phpcpp.h>

#include "connection.h"
#include "request.h"
#include "response.h"
#include "vpack.h"
#include "cursor.h"
#include "exception.h"

extern "C" {

    void exportClassConnection(Php::Extension* extension);
    void exportClassVpack(Php::Extension* extension);
    void exportClassRequest(Php::Extension* extension);
    void exportClassResponse(Php::Extension* extension);
    void exportClassCursor(Php::Extension* extension);

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

        exportClassConnection(&extension);
        exportClassVpack(&extension);
        exportClassRequest(&extension);
        exportClassResponse(&extension);
        exportClassCursor(&extension);

        extension.onStartup([]() {
            arangodb::fuerte::php::registerCustomExceptions();
        });

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
        connection.method<&arangodb::fuerte::php::Connection::setDefaultTimeout>("setDefaultTimeout", {
            Php::ByVal("defaultTimeout", Php::Type::Numeric, true)
        });

        Php::Arguments methodArgs = {
            Php::ByVal("path", Php::Type::String, true),
            Php::ByVal("vpack", "ArangoDb\\Vpack", true),
            Php::ByVal("queryParams", Php::Type::Array, false)
        };

        connection.method<&arangodb::fuerte::php::Connection::methodDelete>("delete", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodGet>("get", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodPost>("post", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodPut>("put", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodHead>("head", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodPatch>("patch", methodArgs);
        connection.method<&arangodb::fuerte::php::Connection::methodOptions>("options", methodArgs);

        connection.method<&arangodb::fuerte::php::Connection::wait>("wait");

        connection.method<&arangodb::fuerte::php::Connection::query>("query", {
            Php::ByVal("vpack", "ArangoDb\\Vpack", true),
            Php::ByVal("options", Php::Type::Array, false)
        });

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
            Php::ByVal("vpack", "ArangoDb\\Vpack", true),
            Php::ByVal("queryParams", Php::Type::Array, false)
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


    void exportClassCursor(Php::Extension* extension)
    {
        Php::Class<arangodb::fuerte::php::Cursor> cursor("ArangoDb\\Cursor");

        cursor.method<&arangodb::fuerte::php::Cursor::valid>("valid");
        cursor.method<&arangodb::fuerte::php::Cursor::current>("current");
        cursor.method<&arangodb::fuerte::php::Cursor::key>("key");
        cursor.method<&arangodb::fuerte::php::Cursor::next>("next");
        cursor.method<&arangodb::fuerte::php::Cursor::rewind>("rewind");
        cursor.method<&arangodb::fuerte::php::Cursor::getCount>("count");

        cursor.method<&arangodb::fuerte::php::Cursor::toArray>("toArray");
        cursor.method<&arangodb::fuerte::php::Cursor::getResponse>("getResponse");

        cursor.property("ENTRY_TYPE", arangodb::fuerte::php::Cursor::ENTRY_TYPE, Php::Const);
        cursor.property("ENTRY_TYPE_JSON", arangodb::fuerte::php::Cursor::ENTRY_TYPE_JSON, Php::Const);
        cursor.property("ENTRY_TYPE_ARRAY", arangodb::fuerte::php::Cursor::ENTRY_TYPE_ARRAY, Php::Const);
        cursor.property("ENTRY_TYPE_OBJECT", arangodb::fuerte::php::Cursor::ENTRY_TYPE_OBJECT, Php::Const);

        extension->add(std::move(cursor));
    }
}
