# Create a new class
Since it's usually required to store additional data, that is not accessible from PHP userspace, together with the 
PHP-Object the setup is a little more complex than usual. For further information about creating simple php classes
refer to [PHP Internals Book](http://www.phpinternalsbook.com/classes_objects/simple_classes.html) (this covers PHP5 so
some information might be outdated). Also take a look at [phpng upgrading](https://wiki.php.net/phpng-upgrading) which
covers a lot of changes required for PHP7 (information about the basic principle described here can also be found there).

## Create a struct/class 
In order to store additional data together with the PHP object we first have to create a container for that purpose. 
A classic C extension would most likely have a struct like the following with an arbitrary number of custom variables 
as well as a `zend_object std;` which is the internal PHP object at the end.
```
typedef {
   int my_custom_integer;
   zend_object std;
} custom_object;
```
Since we are working with C++ we can also use classes as said container. This is nice since it enables us to also store
a lot of the logic together with the data. There are also a few static methods and properties we have to add so we just 
put the into this class as well. Because of our class requiring more memory than what would be needed for the 
`zend_object` we have to manually tell PHP how to create an object of that instance which is what 
`php_arangodb_handler_vpack` and `create_object` are for. Since we still get a zend_object back from PHP we also need 
a way to get the corresponding instance of our class back which is why we also have to create a `fetch_object` 
method. Most of this is just copy paste with minor changes.
```
//vpack.h
 
class Vpack
{
public:
    vp::Builder builder;    //our velocypack builder object, we don't want that to even reach PHP userspace
    zend_object std;        //the internal PHP object 
 
    //additional required static class methods and variables
    static zend_object_handlers php_arangodb_handler_vpack;
    static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
    static Vpack* fetch_object(zend_object* obj);
}
```
```
//vpack.cpp
 
zend_object_handlers Vpack::php_arangodb_handler_vpack = zend_object_handlers();
 
zend_object* Vpack::create_object(zend_class_entry* ce TSRMLS_CC)
{
    Vpack* intern = (Vpack*)ecalloc(1, sizeof(Vpack) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);

    intern->std.handlers = &Vpack::php_arangodb_handler_vpack;
    return &intern->std;
}
 
Vpack* Vpack::fetch_object(zend_object* obj)
{
    return (Vpack *)((char *)obj - XtOffsetOf(Vpack, std));
}
```

## Tell PHP about our class
In order to tell PHP about our class we have to the basic PHP class setup (take a look at the PHP internals book).
We simply create a `php_vpack.h` file for that purpose (Note: the namespace is required to prevent linking issues):
```
namespace {
    zend_class_entry *vpack_ce;
 
    #define Z_OBJECT_VPACK(zo) (arangodb::fuerte::php::Vpack::fetch_object(zo))
    #define Z_OBJECT_VPACK_P(zo) (arangodb::fuerte::php::Vpack::fetch_object(Z_OBJ_P(zo)))
    
    PHP_METHOD(Vpack, __construct)
    {
        arangodb::fuerte::php::Vpack* intern;
 
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }
 
        intern = Z_OBJECT_VPACK_P(getThis()); //get the instance of our class from the zend_object
        intern->builder->add(....) //do stuff with our custom variables
    }
    
    ZEND_BEGIN_ARG_INFO_EX(arangodb_vpack_void, 0, 0, 0)
    ZEND_END_ARG_INFO()
 
    zend_function_entry vpack_methods[] = {
        PHP_ME(Vpack, __construct, arangodb_vpack_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_FE_END
    };
 
    PHP_MINIT_FUNCTION(arangodb)
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "ArangoDb\\Vpack", vpack_methods);
        vpack_ce = zend_register_internal_class(&ce TSRMLS_CC);
        vpack_ce->create_object = arangodb::fuerte::php::Vpack::create_object;
 
        memcpy(&arangodb::fuerte::php::Vpack::php_arangodb_handler_vpack, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        arangodb::fuerte::php::Vpack::php_arangodb_handler_vpack.offset = XtOffsetOf(arangodb::fuerte::php::Vpack, std);
 
        return SUCCESS;
    }
}
```
