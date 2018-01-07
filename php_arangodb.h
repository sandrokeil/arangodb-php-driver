#ifndef PHP_ARANGODB_H
#define PHP_ARANGODB_H

#define PHP_ARANGODB_EXTNAME  "arangodb"
#define PHP_ARANGODB_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>

extern zend_module_entry arangodb_module_entry;
#define phpext_arangodb_ptr &arangodb_module_entry;

#endif /* PHP_ARANGODB_H */
