#include "php_arangodb.h"

#include "php_vpack.h"
#include "php_request.h"
#include "php_connection.h"


PHP_MINIT_FUNCTION(arangodb)
{
    init_arangodb_vpack_ce();
    init_arangodb_request_ce();
    init_arangodb_connection_ce();

    return SUCCESS;
}

zend_module_entry arangodb_module_entry = {

#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_ARANGODB_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(arangodb),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_ARANGODB_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ARANGODB
extern "C" {
ZEND_GET_MODULE(arangodb)
}
#endif
