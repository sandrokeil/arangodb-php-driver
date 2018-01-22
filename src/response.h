#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>
#include <fuerte/fuerte.h>
#include <fuerte/types.h>

namespace vp = ::arangodb::velocypack;
namespace fu = ::arangodb::fuerte;

namespace arangodb { namespace fuerte { namespace php {

    class Response
    {
    private:
        //@todo find out if it's worth using a std::unique_ptr<fu::Response> here
        fu::Response response;

    public:
        zend_object std;

        static zend_object_handlers handler_response;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Response* fetch_object(zend_object* obj);

        Response(const fu::Response &response);
        int get_http_code();
    };

}}}
