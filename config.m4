PHP_ARG_ENABLE(arangodb,
    [Whether to enable the "arangodb" extension],
    [  --enable-arangodb      Enable "arangodb" extension support])

PHP_ADD_INCLUDE("deps/velocypack/include")

if test $PHP_ARANGODB != "no"; then
    CXXFLAGS="-std=c++11"
    PHP_REQUIRE_CXX()

    PHP_SUBST(ARANGODB_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, ARANGODB_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(velocypack, "/usr/local/lib", ARANGODB_SHARED_LIBADD)
    PHP_NEW_EXTENSION(arangodb, php_arangodb.cpp vpack.cpp, $ext_shared)
fi
